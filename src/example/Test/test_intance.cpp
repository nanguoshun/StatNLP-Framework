//
// Created by  ngs on 25/11/2018.
//

//
// Created by  ngs on 05/10/2018.
//

#include "test_instance.h"

TestInstance::TestInstance() {

}

TestInstance::TestInstance(int instance_id, double weight, std::vector<std::string> *ptr_sent,
                                   std::vector<std::string> *ptr_labels)
        :BaseInstance<TestInstance, std::vector<std::string>, std::vector<std::string>>(this, ptr_sent, ptr_labels, instance_id, weight) {
    ptr_input_ = ptr_sent;
    ptr_str_vec_ = nullptr;
}

TestInstance::~TestInstance() {
    if(nullptr != ptr_str_vec_){
        delete ptr_str_vec_;
    }
}

int TestInstance::GetSize() {
    return ptr_input_->size();
}

/**
 *
 *
 */
//FIXME: this is memory expensive
ComType::Input_Str_Vector *TestInstance::GetStrVect() {
    return ptr_str_vec_;
}

void TestInstance::SetStrVect(ComType::Input_Str_Vector *ptr_vec) {
    ptr_str_vec_ = ptr_vec;
}
