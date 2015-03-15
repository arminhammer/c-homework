#!/bin/sh

echo "##### Test 41 #####"
curl localhost:1080/testfiles/pic.jpg

echo "##### Test 42 #####"
curl -I localhost:1080/testfiles/pic.jpg

echo "##### Test 43 #####"
curl localhost:1080/testfiles/pic.gif

echo "##### Test 44 #####"
curl -I localhost:1080/testfiles/pic.gif

echo "##### Test 45 #####"
curl localhost:1080/testfiles/dir3

echo "##### Test 46 #####"
curl -I localhost:1080/testfiles/dir3

echo "##### Test 47 #####"
curl localhost:1080/testfiles/dir3/

echo "##### Test 48 #####"
curl -I localhost:1080/testfiles/dir3/

echo "##### Test 49 #####"
curl localhost:1080/testfiles/dir3/pic.jpg

echo "##### Test 50 #####"
curl -I localhost:1080/testfiles/dir3/pic.jpg

echo "##### Test 51 #####"
curl localhost:1080/testfiles/dir3/pic.png

echo "##### Test 52 #####"
curl -I localhost:1080/testfiles/dir3/pic.png

echo "##### Test 53 #####"
curl localhost:1080/testfiles/dir3/pic.gif

echo "##### Test 54 #####"
curl -I localhost:1080/testfiles/dir3/pic.gif
