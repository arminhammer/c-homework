#!/bin/sh

echo "##### Test 11 #####"
curl localhost:1080/testfiles/mod/rev

echo "##### Test 12 #####"
curl -I localhost:1080/testfiles/mod/rev

echo "##### Test 13 #####"
curl localhost:1080/testfiles/mod/rev/

echo "##### Test 14 #####"
curl -I localhost:1080/testfiles/mod/rev/

echo "##### Test 15 #####"
curl "localhost:1080/mod/rev?string=foo&string1=bar"

echo "##### Test 16 #####"
curl -I "localhost:1080/mod/rev?string=foo&string1=bar"

echo "##### Test 17 #####"
curl localhost:1080/mod/xyz

echo "##### Test 18 #####"
curl -I localhost:1080/mod/xyz

echo "##### Test 19 #####"
curl "localhost:1080/testfiles/mod/xyz?string=foo&string1=bar"

echo "##### Test 20 #####"
curl -I "localhost:1080/testfiles/mod/xyz?string=foo&string1=bar"
