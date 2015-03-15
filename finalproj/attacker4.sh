#!/bin/sh

echo "##### Test 21 #####"
curl localhost:1080/testfiles/index.html

echo "##### Test 22 #####"
curl -I localhost:1080/testfiles/index.html

echo "##### Test 23 #####"
curl localhost:1080/testfiles/index.htm

echo "##### Test 24 #####"
curl -I localhost:1080/testfiles/index.htm

echo "##### Test 25 #####"
curl localhost:1080/testfiles/index.html1

echo "##### Test 26 #####"
curl -I localhost:1080/testfiles/index.html1

echo "##### Test 27 #####"
curl localhost:1080/testfiles/dir1

echo "##### Test 28 #####"
curl -I localhost:1080/testfiles/dir1

echo "##### Test 29 #####"
curl localhost:1080/testfiles/dir1/

echo "##### Test 30 #####"
curl -I localhost:1080/testfiles/dir1/
