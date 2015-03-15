#!/bin/sh

echo "##### Test 31 #####"
curl localhost:1080/testfiles/dir1/index.html

echo "##### Test 32 #####"
curl -I localhost:1080/testfiles/dir1/index.html

echo "##### Test 33 #####"
curl localhost:1080/testfiles/dir1/index.htm
echo "##### Test 34 #####"
curl -I localhost:1080/testfiles/dir1/index.htm

echo "##### Test 35 #####"
curl localhost:1080/testfiles/dir2

echo "##### Test 36 #####"
curl -I localhost:1080/testfiles/dir2

echo "##### Test 37 #####"
curl localhost:1080/testfiles/dir2/

echo "##### Test 38 #####"
curl -I localhost:1080/testfiles/dir2/

echo "##### Test 39 #####"
curl localhost:1080/testfiles/pic.png

echo "##### Test 40 #####"
curl -I localhost:1080/testfiles/pic.png
