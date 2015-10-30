#include <vector>
#include <algorithm>
#include <iostream>

#include <Eigen/Dense>

#include "timer.h"

using namespace std;
using namespace Eigen;

int main(void)
{
  static const int R = 288;
  static const int N = R*(R+1)/2;
  static const int M = 63;
  static const int HALF_M = M/2;
  static const float nsigma = 2.0f;

  MatrixXcf raw = MatrixXcf::Random(M, N);
  VectorXcf result = VectorXcf::Zero(N);
  MatrixXf tmp = MatrixXf::Zero(M, N);
  MatrixXf data = MatrixXf::Zero(M, N);
  MatrixXf mask = MatrixXf::Zero(M, N);
  VectorXf std = VectorXf::Zero(N);
  VectorXf centroid = VectorXf::Zero(N);
  VectorXf mean = VectorXf::Zero(N);
  VectorXf minval = VectorXf::Zero(N);
  VectorXf maxval = VectorXf::Zero(N);

  cout << "computing..." << flush;
  double t = GetRealTime();
  data = raw.array().abs();
  tmp = data;

  // computes the exact median
  if (M&1)
  {
#pragma omp parallel for
    for (int i = 0; i < N; i++)
    {
      vector<float> row(tmp.data()+i*M, tmp.data()+(i+1)*M);
      nth_element(row.begin(), row.begin()+HALF_M, row.end());
      centroid(i) = row[HALF_M];
    }
  }
  // nth_element guarantees x_0,...,x_{n-1} < x_n
  else
  {
#pragma omp parallel for
    for (int i = 0; i < N; i++)
    {
      vector<float> row(tmp.data()+i*M, tmp.data()+(i+1)*M);
      nth_element(row.begin(), row.begin()+HALF_M, row.end());
      centroid(i) = row[HALF_M];
      centroid(i) += *max_element(row.begin(), row.begin()+HALF_M);
      centroid(i) *= 0.5f;
    }
  }

  // compute the mean
  mean = data.colwise().mean();

  // compute std (x) = sqrt ( 1/N SUM_i (x(i) - mean(x))^2 )
  std = (((data.rowwise() - mean.transpose()).array().square()).colwise().sum() *
         (1.0f / M))
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
  raw.array() *= mask.array();

  // compute mean such that we ignore clipped data, this is our final result
  result = raw.colwise().sum().array() / mask.colwise().sum().array();

  t = GetRealTime() - t;
  cout << "[done]" << endl << endl;

  size_t bytes = data.size()*sizeof(float);
  cout << "data: " << M << "x" << N << endl;
  cout << "size: " << bytes*1e-6f << " MB" << endl;
  cout << "rate: " << bytes/(1e6f*t) << " MB/s" << endl;
  cout << "time: " << t << " s" << endl;

  return 0;
}
