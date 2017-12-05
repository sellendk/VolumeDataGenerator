#include <cmath>
#include "frequency.hpp"

#define M_2_PI 0.63661977236758134308

double Frequency::rect(const Vec3<int> &cords, const Vec3<double> &mag, const Vec3<int> &freq, const Vec3<int> &res) {
	Vec3<double> magS = mag;
	if (magS.x + magS.y + magS.z > 1.0) magS /= (magS.x + magS.y + magS.z);

	Vec3<double> density;
	
	// period of 6 --> 111.000.111.000
	density.x = magS.x * static_cast<double>((cords.x % 6) < 3 ? 1 : -1);
	density.y = magS.y * static_cast<double>((cords.y % 6) < 3 ? 1 : -1);
	density.z = magS.z * static_cast<double>((cords.z % 6) < 3 ? 1 : -1);

	return (density.x + density.y + density.z);
}

double Frequency::triangle(const Vec3<int> &cords, const Vec3<double> &mag, const Vec3<int> &freq, const Vec3<int> &res) {
	Vec3<double> magS = mag;
	if (magS.x + magS.y + magS.z > 1.0) magS /= (magS.x + magS.y + magS.z);

	Vec3<double> density;

	// period of 6, values from 0 to 3
	density.x = magS.x * static_cast<double>(abs((cords.x % 6) - 3));
	density.y = magS.y * static_cast<double>(abs((cords.y % 6) - 3));
	density.z = magS.z * static_cast<double>(abs((cords.z % 6) - 3));

	return (density.x + density.y + density.z) / 3.0;
}

double Frequency::sine(const Vec3<int> &cords, const Vec3<double> &mag, const Vec3<int> &freq, const Vec3<int> &res) {
	double x = cords.x / static_cast<double>(res.x) * M_2_PI;
	double y = cords.y / static_cast<double>(res.y) * M_2_PI;
	double z = cords.z / static_cast<double>(res.z) * M_2_PI;
	
	Vec3<double> magS = mag;
	if (magS.x + magS.y + magS.z > 1.0) magS /= (magS.x + magS.y + magS.z);

	Vec3<double> density;
	density.x = magS.x * (sin(freq.x * x) + 1.0) * 0.5;
	density.y = magS.y * (sin(freq.y * y) + 1.0) * 0.5;
	density.z = magS.z * (sin(freq.z * z) + 1.0) * 0.5;

	return (density.x + density.y + density.z);
}

double Frequency::quadraticSine(const Vec3<int> &cords, const Vec3<double> &mag, const Vec3<int> &freq, const Vec3<int> &res) {
	double x = cords.x / static_cast<double>(res.x) * M_2_PI;
	double y = cords.y / static_cast<double>(res.y) * M_2_PI;
	double z = cords.z / static_cast<double>(res.z) * M_2_PI;

	Vec3<double> magS = mag;
	if (magS.x + magS.y + magS.z > 1.0) magS /= (magS.x + magS.y + magS.z);

	Vec3<double> density;
	density.x = magS.x * (pow(sin(freq.x * x), 2) + 1.0) * 0.5;
	density.y = magS.y * (pow(sin(freq.y * y), 2) + 1.0) * 0.5;
	density.z = magS.z * (pow(sin(freq.z * z), 2) + 1.0) * 0.5;

	return (density.x + density.y + density.z);
}

double Frequency::sineExp(const Vec3<int> &cords, const Vec3<double> &mag, const Vec3<int> &freq, const Vec3<int> &res) {
	double x = cords.x / static_cast<double>(res.x) * M_2_PI;
	double y = cords.y / static_cast<double>(res.y) * M_2_PI;
	double z = cords.z / static_cast<double>(res.z) * M_2_PI;

	Vec3<double> magS = mag;
	if (magS.x + magS.y + magS.z > 1.0) magS /= (magS.x + magS.y + magS.z);

	Vec3<double> density;
	density.x = magS.x * (exp(sin(freq.x * x)) / exp(1) + 1.0) * 0.5;
	density.y = magS.y * (exp(sin(freq.y * y)) / exp(1) + 1.0) * 0.5;
	density.z = magS.z * (exp(sin(freq.z * z)) / exp(1) + 1.0) * 0.5;

	return (density.x + density.y + density.z);
}

double Frequency::cosine(const Vec3<int> &cords, const Vec3<double> &mag, const Vec3<int> &freq, const Vec3<int> &res) {
	double x = cords.x / static_cast<double>(res.x) * M_2_PI;
	double y = cords.y / static_cast<double>(res.y) * M_2_PI;
	double z = cords.z / static_cast<double>(res.z) * M_2_PI;

	Vec3<double> magS = mag;
	if (magS.x + magS.y + magS.z > 1.0) magS /= (magS.x + magS.y + magS.z);

	Vec3<double> density;
	density.x = magS.x * (cos(freq.x * x) + 1.0) * 0.5;
	density.y = magS.y * (cos(freq.y * y) + 1.0) * 0.5;
	density.z = magS.z * (cos(freq.z * z) + 1.0) * 0.5;

	return (density.x + density.y + density.z);
}

double Frequency::quadraticCosine(const Vec3<int> &cords, const Vec3<double> &mag, const Vec3<int> &freq, const Vec3<int> &res) {
	double x = cords.x / static_cast<double>(res.x) * M_2_PI;
	double y = cords.y / static_cast<double>(res.y) * M_2_PI;
	double z = cords.z / static_cast<double>(res.z) * M_2_PI;

	Vec3<double> magS = mag;
	if (magS.x + magS.y + magS.z > 1.0) magS /= (magS.x + magS.y + magS.z);

	Vec3<double> density;
	density.x = magS.x * (pow(cos(freq.x * x), 2) + 1.0) * 0.5;
	density.y = magS.y * (pow(cos(freq.y * y), 2) + 1.0) * 0.5;
	density.z = magS.z * (pow(cos(freq.z * z), 2) + 1.0) * 0.5;

	return (density.x + density.y + density.z);
}

double Frequency::cosineExp(const Vec3<int> &cords, const Vec3<double> &mag, const Vec3<int> &freq, const Vec3<int> &res) {
	double x = cords.x / static_cast<double>(res.x) * M_2_PI;
	double y = cords.y / static_cast<double>(res.y) * M_2_PI;
	double z = cords.z / static_cast<double>(res.z) * M_2_PI;

	Vec3<double> magS = mag;
	if (magS.x + magS.y + magS.z > 1.0) magS /= (magS.x + magS.y + magS.z);

	Vec3<double> density;
	density.x = magS.x * (exp(cos(freq.x * x)) / exp(1) + 1.0) * 0.5;
	density.y = magS.y * (exp(cos(freq.y * y)) / exp(1) + 1.0) * 0.5;
	density.z = magS.z * (exp(cos(freq.z * z)) / exp(1) + 1.0) * 0.5;

	return (density.x + density.y + density.z);
}