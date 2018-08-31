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

## Build Dynet
```bash
cmake .. -DEIGEN3_INCLUDE_DIR=../../eigen -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_FLAGS="-fsanitize=address"
```

## Examples
### Linear Condition Random Field (CRF)
### Probability Context Free Grammer(PCFG)

![my equation](https://latex.codecogs.com/gif.download?%5Czeta%28s%29%20%3D%20%5Csum_%7Bn%3D1%7D%5E%5Cinfty%20%5Cfrac%7B1%7D%7Bn%5Es%7D)
## Develop your graphical models
StatNLP framework faciliates the customized graphical models.

## Mathmatical Models
