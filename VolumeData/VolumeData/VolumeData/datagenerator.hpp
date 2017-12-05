#ifndef DATAGENERATOR_HPP
#define DATAGENERATOR_HPP

#include <vector>
#include <cmath>
#include <iostream>

#include "vec3.hpp"

enum Shape { cube, sphere };

typedef unsigned char uchar;
typedef unsigned short ushort;

struct DataConfig
{
	// sample precision
	int precision;

	// Volume data resolution x,y and z direction
	Vec3<int> res;

	// Percentage coverage in x,y and z direction.
	Vec3<double> coverage;

	// shape of the objects
	Shape shape = cube;

	// number of disjunct bodies
	long long numBodies = 3L;

	// structured grid layout that is used when adding bodies structured
	Vec3<int> dimBodies;

	// lay out bodies in a structured way
	bool randomBodyLayout;

	// Wavelet frequency in x, y and z direction
	Vec3<int> frequency;

	// Wavelet magnitude in x, y and z direction
	Vec3<double> magnitude;

	// standard config
	DataConfig() : precision{ 1 }, res{ 32, 32, 32 }, coverage{ 60, 60, 60 },
		dimBodies{ 3, 3, 3 }, randomBodyLayout{ true },
		frequency{ 10, 10, 10 }, magnitude{ 0.3, 0.3, 0.3 }
	{
	}
};


class DataGenerator
{
public:
	DataGenerator();

	/**
	* @brief Generate a volume data set with the given configuration.
	* @param cfg A data configuration object.
	* @return true if data was generated successfully, false otherwise.
	*/
	int generateVolume(DataConfig cfg);

	/**
	* @brief Generate a test volume data set with the given configuration.
	* @param cfg A data configuration object.
	* @return true if data was generated successfully, false otherwise.
	*/
	int generateTestVolume(DataConfig cfg);

	/**
	* @brief Update the scalar data.
	* @param cfg A data configuration object.
	* @return true if data was generated successfully, false otherwise.
	*/
	int generateScalarData(const DataConfig cfg);

	/**
	* @brief Get the generated volume data.
	* @param volData Reference to a vector the data shall be stored in.
	* @return true on success, false otherwise.
	*/
	int getVolumeData(std::vector<float> &volData)
	{
		if (m_volData.empty())
		{
			return false;
		}

		volData.resize(m_volData.size());
		volData = m_volData;

		return true;
	}

	/**
	* @brief Get the shape-dependent volume final data
	* @param volData Reference to a vector the data shall be stored in.
	* @return true on success, false otherwise
	*/
	int finalData(std::vector<float> &volData) {
		volData.resize(m_volData.size());
		for (int i = 0; i < m_volData.size(); ++i) {
			volData.at(i) = m_volData.at(i) * m_isShape.at(i);
		}
		return true;
	}

	/**
	* @brief prints the volData
	* @param m_volData the volData to be printed
	*/
	void printVolumeData() {
		for (int i = 10000; i < 10150; ++i) {
			std::cout << i << ": " << m_volData[i] << std::endl;
		}
	}

protected:
	/**
	* @brief Check if an element is overlapping another one.
	* @param center The center postition of the element.
	* @param radius The radius of the element.
	* @param elementCenters A vector containing all center positions that should be checked against.
	* @return true If the element overlaps anotherone.
	*/
	bool isOverlapping(const Vec3<double> center, const Vec3<double> radius, const Shape shape,
		const std::vector<Vec3<double> > *m_elementCenters);

	/**
	* @brief Add an element at a random position inside the volume.
	* @param radius of the element generated
	* @return
	*/
	Vec3<double> addRandomElement(Vec3<double> radius);

	/**
	* @brief addElementsStructured
	* @param n Number of disjunct bodies to add.
	* @param dimBodies Dimensions of the structured grid in which the bodies are to be laid out.
	* @return
	*/
	std::vector<Vec3<double> > addElementsStructured(long long n, Vec3<int> dimBodies);

private:
	// The current data configuration.
	DataConfig m_cfg;

	// Generated volume data as a float representation.
	std::vector<float> m_volData;

	// representation of all covered voxels
	std::vector<bool> m_isShape;

	// center position of all elements
	std::vector<Vec3<double>> m_elementCenters;
};

#endif // DATAGENERATOR_HPP