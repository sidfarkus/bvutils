bvutils
=======

Utility library for compressed-bitvector-based object permissions

The intent of this library is to compare visibility compressed bitvectors stored as data markings with a user's (or appliation's) authorization compressed bitvector to determine if the user/application shoudl be granted access to the object.

This library currently works with a modified version of MongoDB.

To build both this library, the modified version of MongoDB, and run the code that tests the use of these bitvectors within Mongo, perform the following steps:

1. clone this repository:
   $ git clone https://github.com/mike-schiller/bvutils.git

2. build this library
   $cd bvutils
   $./build.sh
   
      *** note that other projects will be cloned and files will be downloaded
      
3. Run the tests for this libary
   $ ./bin/cpp_example
   $ ./bin/c_example
   $ export PYTHONPATH=lib/python
   $ python test/example.py
   
4. CD back to where we started
   $ cd ..
   
5. Clone the modified version of MongoDB, switch to my branch, and build it:
   $ git clone https://github.com/mike-schiller/mongo.git
   $ cd mongo
   $ git checkout 2.6.1_bv
   $ scons all
   
   Then, wait a couple hours.
   
6. Run mongod
    $ export LD_LIBRARY_PATH=../bvutils/lib
    $ ./mongod
    
7. In another terminal, run the python sript that loads compressed bitvector-marked data into Mongo and attempts to access it using authorization bitvectors:
    $ cd bvutils
    $ export PYTHONPATH=lib/python
    $ python mongo/mongoscript.py
    
