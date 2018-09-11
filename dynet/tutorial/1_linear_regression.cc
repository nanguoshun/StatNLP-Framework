#include "dynet/dynet.h"
#include "dynet/training.h"
#include "dynet/expr.h"

#include <iostream>
#include <random>

using namespace std;
using namespace dynet;

int main(int argc, char** argv) {
  dynet::initialize(argc, argv);

  default_random_engine rng;
  normal_distribution<float> normal(0.0f, 1.0f);

  vector<float> xs;
  vector<float> ys;
  for (unsigned i = 0; i < 100; ++i) {
    float x = -1 + 2.0 / 100;
    float y = 2 * x + normal(rng) * 0.33f;
    std::cout <<i<<" th x: "<< x <<std::endl;
    std::cout <<i<<" th y: "<< y <<std::endl;
    xs.push_back(x);
    ys.push_back(y);
  }

  ParameterCollection model;
  Parameter pW = model.add_parameters({1});


  AdadeltaTrainer trainer(model, 0.1);

  ComputationGraph cg;
  Expression W = parameter(cg, pW);
  cout << "the weight value is: "<<as_scalar(W.value()) << endl; // something around 2
  for (unsigned i = 0; i < xs.size(); ++i) {
    Expression pred = W * xs[i];
    Expression loss = square(pred - ys[i]);
    cout << "loss value is: "<<loss.value()<<std::endl;
    cg.forward(loss);
    cg.backward(loss);
    //cout << "the gradient is: "<<W.gradient()<<std::endl;
    trainer.update();
    cout << "the weight value is: "<<as_scalar(W.value()) << endl; // something around 2
  }

  cout << as_scalar(W.value()) << endl; // something around 2
}
