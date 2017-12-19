#ifndef PERLIN_HPP
#define PERLIN_HPP

#include "vec3.hpp"
#include <vector>

class Perlin {
public:
	Perlin();

	Perlin(const unsigned int seed);

	/** @brief creates noise
	* @param point
	* @return noise value from 0 to 1
	*/
	double noise(const Vec3<double> &vec);

	double getP(int i);

private:
	double fade(const double t);
	double lerp(const double t, const double a, const double b);
	double grad(const int hash, const Vec3<double> vec);

	std::vector<int> m_p;
};

#endif PERLIN_HPP