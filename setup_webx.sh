#!/bin/bash

mkdir -p WebX/src WebX/include WebX/test WebX/include/ WebX/obj/
touch WebX/src/{main.cpp,server.cpp,client.cpp,thread_pool.cpp,cache.cpp,logger.cpp,utils.cpp}
touch WebX/include/{definitions.h,server.h,client.h,thread_pool.h,cache.h,logger.h,utils.h}
touch WebX/test/test_client.cpp
touch WebX/Makefile WebX/README.md
echo "WebX project structure created successfully!"


