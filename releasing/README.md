This directory is used to create Verible releases.

The `docker-generate.sh` script creates a bunch of docker containers for
different Linux distributions which build the Verible tool.

The `docker-run.sh` script actually runs the build inside the docker container
and extracts the built content.

The general aim is to support all current versions of Ubuntu and CentOS (for
RedHat Enterprise Linux support).

Currently supported Linux distributions are:

- CentOS 7
- CentOS 8
- Ubuntu Xenial (16.04 LTS)
- Ubuntu Bionic (18.04 LTS)
- Ubuntu Eoan (19.10)
- Ubuntu Focal (20.04 LTS)
- Ubuntu Groovy (20.10)
