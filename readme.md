# ofxCcv

![](https://farm8.staticflickr.com/7415/16250786748_f0e3984e19_o.png)

This is an addon for openFrameworks that wraps the computer vision library [libccv](libccv.org). The goal is to experiment with a few features that distinguish libccv from OpenCv, like the deep convolutional neural net implementation. ofxCcv may be merged with ofxCv to provide as much power as possible in a single addon without requiring developers to keep track of which library they are using.

Currently the binary library is only precompiled for for 32-bit and 64-bit OSX (thanks to [Gene Kogan](https://github.com/genekogan)).

After cloning the repository, you will need to run `./setup.sh` to download the pre-trained classifier.