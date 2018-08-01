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
protected:
    int instance_id_;
    double weight_;
    bool isLableled_;

};

#endif //STATNLP_INSTANCE_H
