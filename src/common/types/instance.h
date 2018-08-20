//
// Created by  ngs on 31/07/2018.
//

#ifndef STATNLP_INSTANCE_H
#define STATNLP_INSTANCE_H
class Instance{
public:
    Instance();
    Instance(int id, double weight);
    ~Instance();
    bool IS_Labeled();
    void SetLabeled();
    void SetUnlabeled();
    double GetWeight();
    void SetWeight(double weight);
    int GetInstanceId();
    void SetInstanceId(int id);
    Instance *Duplicate();
    //virtual void * GetInput();
    
protected:
    int instance_id_;
    //it indicates the weight for an specific instance. Usually it is set as 1 for instance un-biased training.
    double weight_;
    bool is_lableled_;
};

#endif //STATNLP_INSTANCE_H
