#!/usr/bin/env bash

mkdir -p example/bin
mkdir -p example/bin/data

DB_URL="https://raw.githubusercontent.com/liuliu/ccv/unstable/samples/image-net-2012.sqlite3"
DB_FILE="example/bin/data/image-net-2012.sqlite3"
WORDS_URL="https://raw.githubusercontent.com/liuliu/ccv/unstable/samples/image-net-2012.words"
WORDS_FILE="example/bin/data/image-net-2012.words"
if [ ! -e $DB_FILE ] ; then
	echo "Downloading image-net-2012 network (ccv)"
	curl -o $DB_FILE $DB_URL
	curl -o $WORDS_FILE $WORDS_URL
fi
echo "Downloaded: image-net-2012 network (ccv)"
