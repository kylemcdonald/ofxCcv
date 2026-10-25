#!/usr/bin/env bash
set -e

DIR="example-classify/bin/data/"
DIR2="example-encode/bin/data/"
DIR3="example-viewer/bin/data/"
DIR4="example-occlusion/bin/data/"

# The ImageNet model was removed from ccv's unstable branch in 2020; this is
# the commit libs/ccv is built from, so the model matches the library.
CCV_COMMIT="a0673048aac2a527ecaea5941266bb3d9a6250ae"
DB_URL="https://raw.githubusercontent.com/liuliu/ccv/$CCV_COMMIT/samples/image-net-2012.sqlite3"
WORDS_URL="https://raw.githubusercontent.com/liuliu/ccv/$CCV_COMMIT/samples/image-net-2012.words"

DB_FILE="image-net-2012.sqlite3"
WORDS_FILE="image-net-2012.words"

mkdir -p $DIR
mkdir -p $DIR2
mkdir -p $DIR3
mkdir -p $DIR4
# re-download a missing model, or a "404: Not Found" saved by an older setup.sh
if [ ! -e $DIR$DB_FILE ] || [ $(wc -c < $DIR$DB_FILE) -lt 1000000 ] ; then
echo "Downloading image-net-2012 network (ccv)"
curl -fL -o $DIR$DB_FILE $DB_URL
curl -fL -o $DIR$WORDS_FILE $WORDS_URL
cp $DIR$DB_FILE $DIR2$DB_FILE
cp $DIR$DB_FILE $DIR3$DB_FILE
cp $DIR$DB_FILE $DIR4$DB_FILE
cp $DIR$WORDS_FILE $DIR2$WORDS_FILE
cp $DIR$WORDS_FILE $DIR3$WORDS_FILE
cp $DIR$WORDS_FILE $DIR4$WORDS_FILE
fi
echo "Downloaded: image-net-2012 network (ccv)"
