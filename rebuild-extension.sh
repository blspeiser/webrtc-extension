#!/bin/bash
cd extension
sh clean.sh
sh build.sh
cd ../jni
mvn clean package
cd ..
sh package.sh