#!/usr/bin/env bash

DIR=example-classify/bin/data
DIR2=example-encode/bin/data
DIR3=example-viewer/bin/data
DB_URL="https://raw.githubusercontent.com/liuliu/ccv/unstable/samples/image-net-2012.sqlite3"
DB_FILE="$DIR/image-net-2012.sqlite3"
DB_FILE2="$DIR2/image-net-2012.sqlite3"
DB_FILE3="$DIR3/image-net-2012.sqlite3"
WORDS_URL="https://raw.githubusercontent.com/liuliu/ccv/unstable/samples/image-net-2012.words"
WORDS_FILE="$DIR/image-net-2012.words"
WORDS_FILE2="$DIR2/image-net-2012.words"
WORDS_FILE3="$DIR3/image-net-2012.words"
mkdir -p $DIR
mkdir -p $DIR2
mkdir -p $DIR3
if [ ! -e $DB_FILE ] ; then
	echo "Downloading image-net-2012 network (ccv)"
	curl -o $DB_FILE $DB_URL
	curl -o $WORDS_FILE $WORDS_URL
	cp $DB_FILE $DB_FILE2
	cp $DB_FILE $DB_FILE3
    cp $WORDS_FILE $WORDS_FILE2
    cp $WORDS_FILE $WORDS_FILE3
fi
echo "Downloaded: image-net-2012 network (ccv)"
