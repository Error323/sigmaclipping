#include <iostream>
#include <Eigen/Dense>

#include "timer.h"

using namespace std;
using namespace Eigen;

int main(void)
{
  static const int R = 1024;
  static const int N = R*R;
  static const int M = 60;
  static const float nsigma = 3.0f;

  MatrixXf X = MatrixXf::Random(M, N);
  MatrixXf I = MatrixXf::Zero(1, N);
  VectorXf std = VectorXf::Zero(N);
  VectorXf mean = VectorXf::Zero(N);
  VectorXf minval = VectorXf::Zero(N);
  VectorXf maxval = VectorXf::Zero(N);

  X.row(3).array() = 1e2f;
  X.row(9).array() = 1e2f;

  double t = GetRealTime();
  mean = X.colwise().mean();
  std = (((X.rowwise() - mean.transpose()).array().square()).colwise().sum() *
         (1.0f / (M - 1.0f)))
            .array()
            .sqrt();
  minval = mean - std * nsigma;
  maxval = mean + std * nsigma;
  
  for (int i = 0; i < N; i++)
  {
    X.col(i) = (X.col(i).array() > minval(i)).select(X.col(i), 1.0f);
    X.col(i) = (X.col(i).array() < maxval(i)).select(X.col(i), 1.0f);
  }

  I = X.colwise().sum() * (1.0f/M);
  cout << "T = " << GetRealTime() - t
       << "\tS = " << X.size() * sizeof(float) * 1e-6f << endl;
  cout << I.sum() << " " << mean.sum() << endl;
}
