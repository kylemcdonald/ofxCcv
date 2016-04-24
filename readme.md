# ofxCcv

Currently the binary library is only precompiled for for 32-bit and 64-bit OSX (thanks to [Gene Kogan](https://github.com/genekogan)). Additional contributions for face recognition come from [Antimodular Research](https://github.com/antimodular/).

After cloning the repository, you will need to run `./setup.sh` to download the pre-trained ImageNet classifier.

## Setting up CUDA support

After installing CUDA and recompiling ccv with CUDA support, replace `ccv.a` in the `libs` folder with your new static library. Then make a copy of the following CUDA libraries adjacent to `ccv.a`: `libcublas_static.a`, `libcudart_static.a`, `libculibos.a`. Rename them all to remove the `lib` prefix. Include them in the linking step. Finally, add GSL to the linker step, too. On OSX after installing GSL with Homebrew, this means adding `-L/usr/local/Cellar/gsl/1.16/lib` and `-lgsl`. After all of this, classification of a single 640x480 image should drop from 750ms to 400ms.