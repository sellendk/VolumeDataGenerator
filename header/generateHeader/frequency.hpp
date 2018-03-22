#ifndef FREQUENCY_HPP
#define FREQUENCY_HPP

#include "vec3.hpp"

class Frequency
{
public:
	/**
	* @brief evaluates a function
	*/
	double evaluate(double mag, double freq, double x, const std::string &str);

	/**
	* @brief creates a density scalar data using a user defined function
	* @param mag The config magnitude
	* @param freq The config frequency
	* @param cords The Cords from the scalar data
	* @param res The resolution of the volume
	* @return scalar The scalar data calculated
	*/
	double userDef(const Vec3<int> &cords, const Vec3<double> &mag, const Vec3<int> &freq, const Vec3<unsigned int> &res, const std::string &str);

	/**
	* @brief creates a density scalar data using a square wave function
	* @param mag The config magnitude
	* @param freq The config frequency - not used
	* @param cords The Cords from the scalar data
	* @param res The resolution of the volume - not used
	* @return scalar The scalar data calculated
	*/
	double rect(const Vec3<int> &cords, const Vec3<double> &mag, const Vec3<int> &freq, const Vec3<unsigned int> &res);

	/**
	* @brief creates a density scalar data using a triangle wave function
	* @param mag The config magnitude
	* @param freq The config frequency - not used
	* @param cords The Cords from the scalar data
	* @param res The resolution of the volume - not used
	* @return scalar The scalar data calculated
	*/
	double triangle(const Vec3<int> &cords, const Vec3<double> &mag, const Vec3<int> &freq, const Vec3<unsigned int> &res);

	/**
	* @brief creates a density scalar data using sine(x)
	* @param mag The config magnitude
	* @param freq The config frequency
	* @param cords The Cords from the scalar data
	* @param res The resolution of the volume
	* @return scalar The scalar data calculated
	*/
	double sine(const Vec3<int> &cords, const Vec3<double> &mag, const Vec3<int> &freq, const Vec3<unsigned int> &res);

	/**
	* @brief creates a density scalar data using sine²(x)
	* @param mag The config magnitude
	* @param freq The config frequency
	* @param cords The Cords from the scalar data
	* @param res The resolution of the volume
	* @return scalar The scalar data calculated
	*/
	double Frequency::quadraticSine(const Vec3<int> &cords, const Vec3<double> &mag, const Vec3<int> &freq, const Vec3<unsigned int> &res);

	/**
	* @brief creates a density scalar data using sine(e^x)
	* @param mag The config magnitude
	* @param freq The config frequency
	* @param cords The Cords from the scalar data
	* @param res The resolution of the volume
	* @return scalar The scalar data calculated
	*/
	double Frequency::sineExp(const Vec3<int> &cords, const Vec3<double> &mag, const Vec3<int> &freq, const Vec3<unsigned int> &res);

	/**
	* @brief creates a density scalar data using cosine(x)
	* @param mag The config magnitude
	* @param freq The config frequency
	* @param cords The Cords from the scalar data
	* @param res The resolution of the volume
	* @return scalar The scalar data calculated
	*/
	double cosine(const Vec3<int> &cords, const Vec3<double> &mag, const Vec3<int> &freq, const Vec3<unsigned int> &res);

	/**
	* @brief creates a density scalar data using cosine²(x)
	* @param mag The config magnitude
	* @param freq The config frequency
	* @param cords The Cords from the scalar data
	* @param res The resolution of the volume
	* @return scalar The scalar data calculated
	*/
	double Frequency::quadraticCosine(const Vec3<int> &cords, const Vec3<double> &mag, const Vec3<int> &freq, const Vec3<unsigned int> &res);

	/**
	* @brief creates a density scalar data using cosine(e^x)
	* @param mag The config magnitude
	* @param freq The config frequency
	* @param cords The Cords from the scalar data
	* @param res The resolution of the volume
	* @return scalar The scalar data calculated
	*/
	double Frequency::cosineExp(const Vec3<int> &cords, const Vec3<double> &mag, const Vec3<int> &freq, const Vec3<unsigned int> &res);
};
#endif // FREQUENCY_HPP
