#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>

#include <Eigen/Dense>

#include "timer.h"

using namespace std;
using namespace Eigen;

int main(void)
{
  static const int R = 1536;
  static const int N = R*R;
  static const int M = 32;
  static const int HALF_M = M/2;
  static const float nsigma = 2.5f;

  MatrixXf data = MatrixXf::Random(M, N);
  MatrixXf mask = MatrixXf::Zero(M, N);
  MatrixXf result = MatrixXf::Zero(1, N);
  VectorXf std = VectorXf::Zero(N);
  VectorXf centroid = VectorXf::Zero(N);
  VectorXf mean = VectorXf::Zero(N);
  VectorXf minval = VectorXf::Zero(N);
  VectorXf maxval = VectorXf::Zero(N);

  double t = GetRealTime();

  // computes the exact median
  if (M&1)
  {
    for (int i = 0; i < N; i++)
    {
      vector<float> row(data.data()+i*M, data.data()+(i+1)*M);
      nth_element(row.begin(), row.begin()+HALF_M, row.end());
      centroid(i) = row[HALF_M];
    }
  }
  // nth_element guarantees x_0,...,x_n-1 < x_n
  else
  {
    for (int i = 0; i < N; i++)
    {
      vector<float> row(data.data()+i*M, data.data()+(i+1)*M);
      nth_element(row.begin(), row.begin()+HALF_M, row.end());
      centroid(i) = row[HALF_M];
      centroid(i) += *max_element(row.begin(), row.begin()+HALF_M);
      centroid(i) *= 0.5f;
    }
  }

  // compute the mean
  mean = data.colwise().mean();

  // compute std (x) = sqrt ( 1/(N-1) SUM_i (x(i) - mean(x))^2 )
  std = (((data.rowwise() - mean.transpose()).array().square()).colwise().sum() *
         (1.0f / (M - 1.0f)))
            .array()
            .sqrt();

  // compute n sigmas from centroid
  minval = centroid - std * nsigma;
  maxval = centroid + std * nsigma;
  
  // compute clip mask
  for (int i = 0; i < N; i++)
  {
    mask.col(i) = (data.col(i).array() > minval(i)).select(VectorXf::Ones(M), 0.0f);
    mask.col(i) = (data.col(i).array() < maxval(i)).select(VectorXf::Ones(M), 0.0f);
  }

  // apply clip mask to data
  data.array() *= mask.array();

  // compute mean such that we ignore clipped data, this is our final result
  result = data.colwise().sum().array() / mask.colwise().sum().array();

  t = GetRealTime() - t;
  printf("time:        %0.2f s\n", t);
  printf("size:        %0.2f MB\n", data.size()*sizeof(float)*1e-6f);
  printf("throughput:  %0.2f MB/s\n", data.size()*sizeof(float)/(1e6f*t));

  return 0;
}
