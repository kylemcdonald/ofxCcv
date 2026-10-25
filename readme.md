# ofxCcv

The binary library is precompiled for 32-bit and 64-bit OSX (thanks to [Gene Kogan](https://github.com/genekogan)) and for Linux: `linux64` (x86_64) and `linuxaarch64` (64-bit ARM: Raspberry Pi OS 64-bit, ARM servers). Additional contributions for face recognition come from [Antimodular Research](https://github.com/antimodular/).

After cloning the repository, you will need to run `./setup.sh` to download the pre-trained ImageNet classifier.

## Linux

Install the libraries ccv uses (Ubuntu / Debian / Raspberry Pi OS):

```sh
sudo apt install libpng-dev libjpeg-dev libfftw3-dev libblas-dev
```

The Linux libraries are static and built with `-fPIC`, so apps link as PIE, the default on current distros. They're built on Ubuntu 22.04 and link on Ubuntu 22.04+, Debian 12+ and Raspberry Pi OS Bookworm. To rebuild them with Docker:

```sh
scripts/build_ccv_linux.sh           # both
scripts/build_ccv_linux.sh arm64     # or amd64
```

## Setting up CUDA support

After installing CUDA and recompiling ccv with CUDA support, replace `ccv.a` in the `libs` folder with your new static library. Then make a copy of the following CUDA libraries adjacent to `ccv.a`: `libcublas_static.a`, `libcudart_static.a`, `libculibos.a`. Rename them all to remove the `lib` prefix. Include them in the linking step. Finally, add GSL to the linker step, too. On OSX after installing GSL with Homebrew, this means adding `-L/usr/local/Cellar/gsl/1.16/lib` and `-lgsl`. After all of this, classification of a single 640x480 image should drop from 750ms to 400ms.