# StatNLP Framework

This is a C++ based implementation of StatNLP framework, designed and developed by StatNLP team of SUTD. For the related academic publications, please visit our website http://www.statnlp.org

## How to Start

This document describes how to develop graphical machine learning model using our StatNLP framework.

### Table of Contents
- [Requirements](#requirements)

### Requirements
* MAC
* Linux
* Windows

## Configure Eigen

Set the eigen directory in CMakeLists.txt.
```bash
set(EIGEN3_INCLUDE_DIR YOUR_PATH_TO_EIGEN)
```

## Build Dynet (Option)
The dynet source code has been placed in project folder, and it will be automatically built when you compile the StatNLP. Meanwhile, you can also build dynet manually via command line.
```bash
cmake .. -DEIGEN3_INCLUDE_DIR=YOUR_PATH_TO EIGN -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_FLAGS="-fsanitize=address"
```
The function get_args in file cl-args.h should be static.

