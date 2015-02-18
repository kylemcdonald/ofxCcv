# ofxCcv

![](https://farm8.staticflickr.com/7415/16250786748_f0e3984e19_o.png)

This is an addon for openFrameworks that wraps the computer vision library [libccv](libccv.org). The goal is to experiment with a few features that distinguish libccv from OpenCv, like the deep convolutional neural net implementation. ofxCcv may be merged with ofxCv to provide as much power as possible in a single addon without requiring developers to keep track of which library they are using.

Before you can use this addon to classify images, you will need to either run `./download-classifier.sh`, or get the files manually by downloading [libccv](http://libccv.org/) and copying `image-net-2012.sqlite3` and `image-net-2012.words` from `ccv/samples` into `openFrameworks/addons/ofxCcv/example/bin/data/`.

CUDA is not supported in 32-bit mode, but we can still build libccv to run without it for OS X 10.7+. The CPU-only binary is only 6MB and comes with this addon. If you get the error `targeted OS version does not support use of thread local variables in _ccv_dense_matrix_new for architecture i386` then you are adding ofxCcv to an old project file, and need to set your OS X Deployment Target to 10.7 or higher.

The `ccv.a` static library that comes with ofxCcv was built using `./configure CC="clang -m32" --without-cuda && make`. To use ofxCcv in 64bit mode please use the [64bit](https://github.com/kylemcdonald/ofxCcv/tree/64bit) branch.