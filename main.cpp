#include <vector>
#include <algorithm>
#include <iostream>

#include <Eigen/Dense>

#include "timer.h"
#include "sigmaclip.h"

using namespace std;
using namespace Eigen;

int main(void)
{
  cout << "Eigen v" << EIGEN_WORLD_VERSION << "." << EIGEN_MAJOR_VERSION << "." << EIGEN_MINOR_VERSION << endl;

  static const int R = 288;
  static const int N = R*(R+1)/2;
  static const int M = 63;
  static const float nsigma = 2.0f;

  MatrixXcf data = MatrixXcf::Random(M, N);
  MatrixXf mask = MatrixXf::Zero(M, N);
  MatrixXcf result = MatrixXcf::Zero(1, N);

  cout << "computing..." << flush;
  double t = GetRealTime();
  VectorXf channels = data.rowwise().mean().array().abs();
  VectorXf channel_mask = VectorXf::Ones(M);

  int iters = SigmaClip(channels, channel_mask, nsigma);

  // mask rows that contribute
  for (int i = 0; i < M; i++)
    if (channel_mask(i))
      mask.row(i).setOnes();

  // apply clip mask to data
  data.array() *= mask.array();

  // compute mean such that we ignore clipped data, this is our final result
  result = data.colwise().sum().array() / mask.colwise().sum().array();

  t = GetRealTime() - t;
  cout << "[done]" << endl << endl;

  size_t bytes = data.size()*sizeof(data(0));
  cout << "data: " << M << "x" << N << endl;
  cout << "size: " << bytes*1e-6f << " MB" << endl;
  cout << "rate: " << bytes/(1e6f*t) << " MB/s" << endl;
  cout << "time: " << t << " s" << endl;
  cout << "iter: " << iters << endl;

  return 0;
}
