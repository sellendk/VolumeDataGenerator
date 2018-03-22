#include "Perlin.hpp"
#include <cmath>
#include <algorithm>
#include <random>
#include <numeric>

Perlin::Perlin() {
	m_p.resize(256);

	// Fill p with values from 0 to 255
	std::iota(m_p.begin(), m_p.end(), 0);

	// Initialize a random engine with seed
	std::default_random_engine engine(237);

	// Suffle  using the above random engine
	std::shuffle(m_p.begin(), m_p.end(), engine);

	// Duplicate the permutation vector
	m_p.insert(m_p.end(), m_p.begin(), m_p.end());
}

Perlin::Perlin(const unsigned int seed) {
	m_p.resize(256);

	// Fill p with values from 0 to 255
	std::iota(m_p.begin(), m_p.end(), 0);

	// Initialize a random engine with seed
	std::default_random_engine engine(seed);

	// Suffle  using the above random engine
	std::shuffle(m_p.begin(), m_p.end(), engine);

	// Duplicate the permutation vector
	m_p.insert(m_p.end(), m_p.begin(), m_p.end());
}

double Perlin::getP(int i) {
	return this->m_p[i];
}

double Perlin::noise(const Vec3<double> &vec) {

	// find unit cube that contains point
	int X = static_cast<int>(vec.x) & 255;
	int Y = static_cast<int>(vec.y) & 255;
	int Z = static_cast<int>(vec.z) & 255;

	// find relative x, y, z of point in cube
	double x = vec.x - floor(vec.x);
	double y = vec.y - floor(vec.y);
	double z = vec.z - floor(vec.z);

	// compute fade curves for each of x, y, z
	double u = fade(x);
	double v = fade(y);
	double w = fade(z);

	// hash coordinates of the 8 cube corners
	int A = m_p[X] + Y;
	int AA = m_p[A] + Z;
	int AB = m_p[A + 1] + Z;

	int B = m_p[X + 1] + Y;
	int BB = m_p[B + 1] + Z;
	int BA = m_p[B] + Z;

	// add blended results from 8 corners of cube
	return lerp(w, lerp(v, lerp(u, grad(m_p[AA], (x, y, z)),
								   grad(m_p[BA], (x - 1, y, z))),
						   lerp(u, grad(m_p[AB], (x, y - 1, z)),
							   	   grad(m_p[BB], (x - 1, y - 1, z)))),
				   lerp(v, lerp(u, grad(m_p[AA + 1], (x, y, z - 1)),
								   grad(m_p[BA + 1], (x - 1, y, z - 1))),
						   lerp(u, grad(m_p[AB + 1], (x, y - 1, z - 1)),
								   grad(m_p[BB + 1], (x - 1, y - 1, z - 1)))));
}

double Perlin::fade(const double t) {
	return t * t * t * (t * (t * 6 - 15) + 10);
}

double Perlin::lerp(const double t, const double a, const double b) {
	return a + t * (b - a);
}

double Perlin::grad(const int hash, const Vec3<double> vec) {
	// convert lo 4 bits of hash code into 12 gradient directions
	int h = hash & 15;
	double u = h < 8 ? vec.x : vec.y;
	double v = h < 4 ? vec.y : h == 12 || h == 14 ? vec.x : vec.z;
	return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}