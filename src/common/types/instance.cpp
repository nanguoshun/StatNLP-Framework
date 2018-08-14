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

void* Instance::GetInput() {

}