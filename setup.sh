#!/usr/bin/env bash

DIR="example-classify/bin/data/"
DIR2="example-encode/bin/data/"
DIR3="example-viewer/bin/data/"
DIR4="example-occlusion/bin/data/"

DB_URL="http://static.libccv.org/image-net-2012.sqlite3"
#DB_URL="http://static.libccv.org/image-net-2012-vgg-d.sqlite3"
WORDS_URL="https://raw.githubusercontent.com/liuliu/ccv/unstable/samples/image-net-2012.words"

DB_FILE="image-net-2012.sqlite3"
WORDS_FILE="image-net-2012.words"

mkdir -p $DIR
mkdir -p $DIR2
mkdir -p $DIR3
mkdir -p $DIR4
if [ ! -e $DIR$DB_FILE ] ; then
echo "Downloading image-net-2012 network (ccv)"
curl -o $DIR$DB_FILE $DB_URL
curl -o $DIR$WORDS_FILE $WORDS_URL
cp $DIR$DB_FILE $DIR2$DB_FILE
cp $DIR$DB_FILE $DIR3$DB_FILE
cp $DIR$DB_FILE $DIR4$DB_FILE
cp $DIR$WORDS_FILE $DIR2$WORDS_FILE
cp $DIR$WORDS_FILE $DIR3$WORDS_FILE
cp $DIR$WORDS_FILE $DIR4$WORDS_FILE
fi
echo "Downloaded: image-net-2012 network (ccv)"
