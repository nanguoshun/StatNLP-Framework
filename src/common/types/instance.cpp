//
// Created by  ngs on 31/07/2018.
//
#include <iostream>
#include "instance.h"


Instance::Instance() {
}

Instance::Instance(int id, double weight) {
    instance_id_ = id;
    weight_ = weight;
}

Instance::~Instance() {
    std::cout << "delete instance"<<std::endl;
}

void Instance::SetLabeled() {
    is_lableled_ = true;
}

void Instance::SetUnlabeled() {
    is_lableled_ = false;
}

bool Instance::IS_Labeled() {
    return is_lableled_;
}

double Instance::GetWeight() {
    return weight_;
}

void Instance::SetWeight(double weight) {
    weight_ = weight;
}

Instance* Instance::Duplicate() {

}

int Instance::GetInstanceId() {
    return instance_id_;
}

void Instance::SetInstanceId(int id) {
    instance_id_ = id;
}