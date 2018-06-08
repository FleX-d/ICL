#!/bin/bash

CATALOGUE="./catalogue.xml"

echo "<colection>" > $CATALOGUE
for arg in "$@"
do
    echo "    <file name=\"$arg\"/>" >> $CATALOGUE
done
echo "</colection>" >> $CATALOGUE

sync

xsltproc --stringparam outputType header --output IPCInterface.h transform.xsl $CATALOGUE
xsltproc --stringparam outputType source --output IPCInterface.cpp transform.xsl $CATALOGUE
exit 0