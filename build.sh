#!/bin/bash

#SWD= readlink -f $(dirname ${BASH_SOURCE[0]})

script_dir="$(dirname -- "$(readlink -f -- "$0")")"

echo $script_dir

if [ ! -d "EWAHBoolArray" ]; then
  git clone git@github.com:mike-schiller/EWAHBoolArray.git EWAHBoolArray
  cd EWAHBoolArray
  git checkout java_serialization
  cd $script_dir 
fi

if [ ! -d "boost_1_41_0" ]; then
  wget http://sourceforge.net/projects/boost/files/boost/1.41.0/boost_1_41_0.tar.gz/download
  tar xzvf download
  rm -f download
  cd boost_1_41_0
  ./bootstrap.sh
  cd $script_dir
fi

if [ -d "objs" ]; then
    rm -rf objs
fi
mkdir objs


if [ -d "lib" ]; then
    rm -rf lib
fi
mkdir lib

if [ -d "bin" ]; then
    rm -rf bin
fi
mkdir bin


g++ -I . -fPIC -c src/bitvectors.cpp -o objs/bitvectors.o
g++ -shared -o lib/libbitvectors.so objs/bitvectors.o


g++ -I . test/cpp_example.cpp -Llib lib/libbitvectors.so -o bin/cpp_example
gcc -I . test/c_example.c -Llib lib/libbitvectors.so -o bin/c_example

mkdir lib/python
swig -c++ -python -o objs/bitvectors.cpp swig/bitvectors.i
g++ -fPIC -I/opt/python-2.7.6/include/python2.7/ -I. -shared -o lib/python/_bitvectors.so objs/bitvectors.o objs/bitvectors.cpp
mv objs/bitvectors.py lib/python/

