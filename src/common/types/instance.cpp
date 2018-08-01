//
// Created by  ngs on 31/07/2018.
//
#include "instance.h"

Instance::Instance() {

}

Instance::Instance(int id, double weight) {
    instance_id_ = id;
    weight_ = weight;
}

Instance::~Instance() {

}

void Instance::SetLabeled() {
    isLableled_ = true;
}

void Instance::SetUnlabeled() {
    isLableled_ = false;
}
