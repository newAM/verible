// Copyright 2017-2020 The Verible Authors.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef VERIBLE_COMMON_STRINGS_OBFUSCATOR_H_
#define VERIBLE_COMMON_STRINGS_OBFUSCATOR_H_

#include <functional>
#include <string>

#include "absl/status/status.h"
#include "absl/strings/string_view.h"
#include "common/strings/compare.h"
#include "common/strings/random.h"
#include "common/util/bijective_map.h"

namespace verible {

// Obfuscator translates input strings into randomly generated strings
// in a manner that tracks which inputs it has seen before, creating a mapping
// and re-using it as more unique input words are seen.  The obfuscation is
// intended to be reversible, so that a one-to-one mapping between original and
// obfuscated text is maintained.
//
// By default, Obfuscator operates in encoding mode, described above.
// In decoding mode (set by set_decode_mode()), the obfuscated strings are
// used to lookup their original counterparts, and unfamiliar strings are kept
// intact (never added to the internal map).
//
// The save() and load() functions can be used to re-apply previously used
// substitutions written/read from a text file.
class Obfuscator {
 public:
  typedef std::function<std::string(absl::string_view)> generator_type;
  typedef BijectiveMap<std::string, std::string, StringViewCompare,
                       StringViewCompare>
      translator_type;

  explicit Obfuscator(generator_type g) : generator_(g) {}

  // Declares a mapping from key-string to value-string that will be
  // used in obfuscation.  This is useful for applying previously used
  // translations.  Returns true if key-value pair was successfully inserted,
  // else returns false if either key or value were already mapped.
  bool encode(absl::string_view key, absl::string_view value);

  void set_decode_mode(bool decode) { decode_mode = decode; }

  bool is_decoding() const { return decode_mode; }

  // Obfuscates input string with a replacement, and records the substitution
  // for later re-use.  Returns the replacement string.
  absl::string_view operator()(absl::string_view input);

  // Read-only view of string translation map.
  const translator_type& GetTranslator() const { return translator_; }

  // Parses a mapping dictionary, and pre-loads the translator map with it.
  // Format: one entry per line, each line is space-separated pair of
  // identifiers.
  absl::Status load(absl::string_view);

  // Returns a string representation of the internal identifier map.
  // See format description for ::load().
  std::string save() const;

 private:
  // Generates a random substitution string, for obfuscation.
  generator_type generator_;

  // Keeps track of transformations done on seen strings.
  translator_type translator_;

  // If true, apply reverse translation of identifiers, and do not generate any
  // new obfuscation mappings.
  bool decode_mode = false;
};

class IdentifierObfuscator : public Obfuscator {
  typedef Obfuscator parent_type;

 public:
  IdentifierObfuscator() : Obfuscator(RandomEqualLengthIdentifier) {}

  // Same as inherited method, but verifies that key and value are equal length.
  bool encode(absl::string_view key, absl::string_view value);
};

}  // namespace verible

#endif  // VERIBLE_COMMON_STRINGS_OBFUSCATOR_H_
