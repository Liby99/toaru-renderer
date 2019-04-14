#include "utility/sampler.h"
#include <chrono>
#include <random>

using namespace nash;

void Sampler::sampleSphere(std::vector<Vector3f> &samples) {
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::mt19937 gen(seed);
  std::uniform_real_distribution<double> uniform(0.0, 1.0);

  int count = samples.size();

  for (int i = 0; i < count; i++) {
    double theta = 2 * PI * uniform(gen);
    double phi = acos(1 - 2 * uniform(gen));
    double x = sin(phi) * cos(theta);
    double y = sin(phi) * sin(theta);
    double z = cos(phi);

    samples[i] = Vector3f(x, y, z);
  }
}

Vector3f Sampler::sampleSphere() {
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::mt19937 gen(seed);
  std::uniform_real_distribution<double> uniform(0.0, 1.0);
  double theta = 2 * PI * uniform(gen);
  double phi = acos(1 - 2 * uniform(gen));
  double x = sin(phi) * cos(theta);
  double y = sin(phi) * sin(theta);
  double z = cos(phi);
  return Vector3f(x, y, z);
}

void Sampler::sampleHemiSphere(std::vector<Vector3f> &samples, Vector3f normal) {
  sampleSphere(samples);
  for (int i = 0; i < samples.size(); i++) {
    if (normal.dot(samples[i]) < 0) {
      samples[i] = -samples[i];
    }
  }
}

Vector3f Sampler::sampleHemiSphere(Vector3f normal) {
  Vector3f sample = sampleSphere();
  if (normal.dot(sample) < 0) {
    sample = -sample;
  }
  return sample;
}
