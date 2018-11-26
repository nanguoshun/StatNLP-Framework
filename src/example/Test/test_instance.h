//
// Created by  ngs on 25/11/2018.
//

#ifndef STATNLP_TEST_INSTANCE_H
#define STATNLP_TEST_INSTANCE_H

#include "src/common/types/base_instance.h"
#include "src/common/types/common.h"
#include "src/common/types/sentence.h"

class TestInstance:public BaseInstance<TestInstance, std::vector<std::string>, std::vector<std::string>>{
public:
    TestInstance();
    TestInstance(int instance_id, double weight, std::vector<std::string> *ptr_sent, std::vector<std::string> *ptr_labels);
    ~TestInstance();
    int GetSize();
    ComType::Input_Str_Vector * GetStrVect();
    void SetStrVect(ComType::Input_Str_Vector *ptr_vec);
    void ExtractStrVect();

private:
    //Sentence *ptr_sent_;
    ComType::Input_Str_Vector *ptr_str_vec_;
};


#endif //STATNLP_TEST_INSTANCE_H
