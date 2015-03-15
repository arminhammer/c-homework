#!/bin/sh

echo "##### Test 1 #####"
curl localhost:1080

echo "##### Test 2 #####"
curl -I localhost:1080

echo "##### Test 3 #####"
curl localhost:1080/

echo "##### Test 4 #####"
curl -I localhost:1080/

echo "##### Test 5 #####"
curl localhost:1080/testfiles/

echo "##### Test 6 #####"
curl -I localhost:1080/testfiles/

echo "##### Test 7 #####"
curl localhost:1080/testfiles/mod

echo "##### Test 8 #####"
curl -I localhost:1080/testfiles/mod

echo "##### Test 9 #####"
curl localhost:1080/testfiles/mod/

echo "##### Test 10 #####"
curl -I localhost:1080/testfiles/mod/
