#!/bin/bash

gcc -fPIC -shared -o hpack.so lua-hpack.c lshpack.c xxhash.c -I /usr/local/include -llua
