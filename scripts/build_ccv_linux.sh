#!/usr/bin/env bash
# Rebuilds libs/ccv/lib/linux64 (x86_64) and linuxaarch64 (64-bit ARM:
# Raspberry Pi OS 64-bit, ARM servers) in Docker: static, -fPIC, so apps link as
# PIE (the default on current distros).
#
#   scripts/build_ccv_linux.sh            # both
#   scripts/build_ccv_linux.sh arm64      # or amd64
#
# ccv at a0673048 (2015-08-11), the last commit with libs/ccv/include/ccv.h as
# it is, configured like the original linux64 library: BLAS, libpng, libjpeg,
# FFTW and ccv's bundled sqlite3 (no GSL, CUDA or video). Built on Ubuntu 22.04,
# so it links on Ubuntu 22.04+, Debian 12+ and Raspberry Pi OS Bookworm, with
# the -dev packages listed in addon_config.mk.
set -euo pipefail

CCV_COMMIT=a0673048
ADDON=$(cd "$(dirname "$0")/.." && pwd)

for arch in ${*:-amd64 arm64}; do
	case $arch in
		amd64) dir=linux64 ;;
		arm64) dir=linuxaarch64 ;;
		*) echo "unknown arch: $arch (amd64 or arm64)" >&2; exit 1 ;;
	esac
	echo "ccv $CCV_COMMIT, linux/$arch -> libs/ccv/lib/$dir"
	docker run --rm --platform "linux/$arch" -v "$ADDON:/addon" ubuntu:22.04 bash -euo pipefail -c "
		export DEBIAN_FRONTEND=noninteractive
		apt-get -o Acquire::Retries=5 update -qq && apt-get -o Acquire::Retries=5 install -y -qq git gcc make \
			libpng-dev libjpeg-dev libfftw3-dev libblas-dev > /dev/null
		cd /tmp && git clone -q https://github.com/liuliu/ccv && cd ccv && git checkout -q $CCV_COMMIT
		cd lib
		# -fPIC for everything, including the bundled sqlite3 (built without CFLAGS);
		# -fcommon: this ccv defines some globals in headers (GCC 10+ defaults to -fno-common)
		./configure CC='gcc -fPIC -fcommon' > /dev/null
		# configure looks for BLAS by the symbol sgemm, but Debian / Ubuntu's libblas
		# exports sgemm_ (with the CBLAS interface ccv uses, cblas_sgemm), so it would
		# build without BLAS and assert in ccv_gemm (classification). Enable it here.
		sed -i 's/^DEFINE_MACROS := /DEFINE_MACROS := -D HAVE_CBLAS /' config.mk
		grep '^DEFINE_MACROS' config.mk
		make -j\$(nproc) libccv.a > /dev/null
		mkdir -p /addon/libs/ccv/lib/$dir
		cp libccv.a /addon/libs/ccv/lib/$dir/
		chown -R \$(stat -c %u:%g /addon) /addon/libs/ccv/lib/$dir
	"
done
