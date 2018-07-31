//
// Created by  ngs on 31/07/2018.
//

class NetworkConfig{
public:
    NetworkConfig();
    ~NetworkConfig();
    void SetL2Param(double L2);
    double GetL2Param();
private:
    double L2_para_;
};