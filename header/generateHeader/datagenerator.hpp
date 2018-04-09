#ifndef DATAGENERATOR_HPP
#define DATAGENERATOR_HPP

#include "frequency.hpp"
#include "Perlin.hpp"
#include "vec3.hpp"

#include <vector>
#include <cmath>
#include <iostream>

enum Shape { cube, sphere };

struct DataConfig
{
	/**
	* 0 == UCHAR
	* 1 == USHORT
	* 2 == FLOAT
	* 3 == DOUBLE
	*/
	unsigned char precision;
	Vec3<unsigned int> res;			// Volume data resolution x,y and z direction
	Vec3<double> coverage;			// Percentage coverage in x,y and z direction.
	Shape shape = sphere;			// shape of the objects
	unsigned long long numBodies;	// number of disjunct bodies
	Vec3<unsigned int> dimBodies;	// structured grid layout that is used when adding bodies structured
	/**
	* 0 == structured
	* 1 == pseudorandom
	* 2 == halton sequence
	* 3 == perlin noise
	*/
	unsigned char randomBodyLayout;
	Vec3<unsigned int> frequency;	// Wavelet frequency in x, y and z direction
	Vec3<double> magnitude;			// Wavelet magnitude in x, y and z direction
	Vec3<double> slice_thickness;	// slive thickness

	// standard config
	DataConfig() : precision{ 0 }, res{ 256, 256, 256 }, coverage{ 70.0, 70.0, 70.0 },
		numBodies{ 114 }, dimBodies{ 7, 7, 7 }, randomBodyLayout{ 1 },
		frequency{ 55, 13, 42 }, magnitude{ 0.3, 0.3, 0.3 }, slice_thickness{ 1.0, 1.0, 1.0 }
	{
	}
};


class DataGenerator
{
public:
	DataGenerator();

	/**
	* @brief helper function for addHaltonElement to check if a number is a prime number
	* @param prime the number to be checked
	* @return true, if prime is a prime number, false otherwise
	*/
	bool isPrime(unsigned long prime);

	/**
	* Skip the first 30 Elements to avoid linear correlation
	* (and from that point take every 50 values)
	*
	* @brief helper function for addHaltonElement to generate the Halton Sequence for a given base
	* @param index
	* @param base the base used for the halton sequence
	* @return r The i-th value of the halton sequence
	*/
	double Halton_Seq(int index, unsigned long base);

	/**
	* @brief Generate a volume data set with the given configuration.
	* @param cfg A data configuration object.
	* @return true if data was generated successfully, false otherwise.
	*/
	int generateVolume(DataConfig cfg);


	/**
	* @brief Update the scalar data.
	* @param cfg A data configuration object.
	* @return true if data was generated successfully, false otherwise.
	*/
	int generateScalarData(const DataConfig &cfg);


	/**
	* @brief returns the volume data
	* @return m_volData The volume data to be returned
	*/
	std::vector<double> &data() {
		return m_volData;
	}

protected:
	/**
	* @brief Check if an element is overlapping another one.
	* @param center The center postition of the element.
	* @param radius The radius of the element.
	* @param elementCenters A vector containing all center positions that should be checked against.
	* @return true If the element overlaps anotherone.
	*/
	bool isOverlapping(const Vec3<double> &center, const Vec3<double> &radius, const Shape &shape,
		const std::vector<Vec3<double> > &m_elementCenters);

	/**
	* @brief Add an element at a pseudorandom position inside the volume using.
	*        If no proper center is found within 15 seconds, stop searching and return -1.0
	* @param radius of the element generated
	* @return the pseudorandom added element
	*/
	Vec3<double> addRandomElement(const Vec3<double> &radius);

	/**
	* @brief Add an element at a random position inside the volume using the halton sequence.
	*        If no proper center is found within 15 seconds, stop searching and return -1.0
	* @param radius of the element generated
	* @param baseX the base used for the X-value
	* @param baseY the base used for the Y-value
	* @param baseZ the base used for the Z-value
	* @return the center of the halton element added
	*/
	Vec3<double> addHaltonElement(const Vec3<double> &radius, int baseX, int baseY, int baseZ, long long &index);

	/**
	* @brief addElementsStructured
	* @param n Number of disjunct bodies to add.
	* @param dimBodies Dimensions of the structured grid in which the bodies are to be laid out.
	* @return
	*/
	std::vector<Vec3<double> > addElementsStructured(long long n, Vec3<int> dimBodies);

private:
	Frequency _freq;
	Perlin _perlin;

	// The current data configuration.
	DataConfig m_cfg;

	// Generated volume data as a float representation.
	std::vector<double> m_volData;

	// representation of all covered voxels
	std::vector<bool> m_isShape;

	// center position of all elements
	std::vector<Vec3<double>> m_elementCenters;

	// used for frequency functions
	int _select = 1;
};

#endif // DATAGENERATOR_HPP