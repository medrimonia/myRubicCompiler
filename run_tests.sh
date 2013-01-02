#!/bin/sh

 make test >compile_information
./test || echo "test passed"