#include <iostream>
#include "src/hybridnetworks/network_config.h"
#include "src/hybridnetworks/data_manager.h"
#include "src/example/linear_crf_instance.h"

double NetworkConfig::L2_REGULARIZATION_CONSTANT = 0.01;
int NetworkConfig::NUM_OF_THREADS = 8;
int main(){
    std::string train_file_name = "data/conll2000/sample_train.txt";
    std::string test_file_name =  "data/conll2000/sample_test.txt";
    //LinearCRFInstance *ptr_train =
    int a, b, c,d;
    double e;
    //BaseInstance<int, int, int> ins(a,b,c,d,e);//*ptr = new LinearCRFInstance(a,b,c);
}