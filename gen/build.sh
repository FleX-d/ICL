#!/bin/bash

changeDirection()
{
  echo "changing direction" 
  
  cp $file "${file}.new"
  workFile="${file}.new"
  
  dirFromNew=$(echo $directionFrom | sed 's/from/to/g')
  #echo $dirFromNew

  dirToNew=$(echo $directionTo | sed 's/to/from/g')
  #echo $dirToNew
  
  #replace from to new in element
  sed -i "s|$directionFrom|\t$dirFromNew|g" $workFile
  sed -i "s|$directionTo|\t$dirToNew|g" $workFile
}

changeFunctions()
{
  while read -r function; do
    echo "$function" | fgrep -q "in"
    if [ $? -eq 0 ]
    then
      functionNew=$(echo $function | sed 's/in/out/g')
    else
      functionNew=$(echo $function | sed 's/out/in/g')
    fi
    sed -i "s|$function|$functionNew|g" $workFile
    
  done < <(cat $workFile | fgrep "function name=")
}

#############
####START####
#############
param=$1
CATALOGUE="./catalogue.xml"

echo "*** start ***"

argc=$#
argv=($@)

echo "<colection>" > $CATALOGUE
#check all files
for (( j=1; j<argc; j++ )); do
  file=${argv[j]}
  echo "check file: $file"
  directionFrom=`cat $file | fgrep name="\"from\""`
  directionTo=`cat $file | fgrep name="\"to\""`

  echo $directionFrom | fgrep $param

  if [ $? -eq 1 ]
  then
    changeDirection
    changeFunctions
    cp $workFile $file
    rm $workFile
    echo "    <file name=\"$file\"/>" >> $CATALOGUE
  else
    echo "$file: direction ok"
    echo "    <file name=\"$file\"/>" >> $CATALOGUE
  fi
done
echo "</colection>" >> $CATALOGUE

sync

xsltproc --stringparam outputType header --output IPCInterface.h transform.xsl $CATALOGUE
xsltproc --stringparam outputType source --output IPCInterface.cpp transform.xsl $CATALOGUE
exit 0
