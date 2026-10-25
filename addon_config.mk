meta:
	ADDON_NAME = ofxCcv
	ADDON_DESCRIPTION = openFrameworks wrapper for ccv (image classification, face detection, ...)
	ADDON_AUTHOR = Kyle McDonald
	ADDON_TAGS = "computer vision" "deep learning" "ccv"
	ADDON_URL = https://github.com/kylemcdonald/ofxCcv

common:

# libccv.a uses these system libraries on Linux:
#   sudo apt install libpng-dev libjpeg-dev libfftw3-dev libblas-dev
linux64:
	ADDON_LDFLAGS = -lpng -ljpeg -lfftw3 -lfftw3f -lblas

linuxaarch64:
	ADDON_LDFLAGS = -lpng -ljpeg -lfftw3 -lfftw3f -lblas
