#include "datagenerator.hpp"
#include "frequency.hpp"
#include "Perlin.hpp"

#include <cstdlib>
#include <cassert>
#include <iostream>
#include <string>

#define SQRT_3 1.732050807568877
#define M_2_PI 0.63661977236758134308

/*
* a function pointer used in generateScalarData to select different functions
* from the Frequency class
*/
double (Frequency::*mptr)(const Vec3<int>&, const Vec3<double>&, const Vec3<int>&, const Vec3<int>&) = NULL;

DataGenerator::DataGenerator()
{
	std::srand(42); // deterministic seed
}

bool DataGenerator::isPrime(const int prime) {
	for (int i = 2; i <= prime / 2; ++i) {
		if (prime % i == 0) return false;
	}
	return true;
}

double DataGenerator::Halton_Seq(int index, const int base) {
	double f = 1, r = 0;
	while (index > 0) {
		f = f / base;
		r = r + f * (index % base);
		index = index / base;
	}
	return r;
}

bool DataGenerator::isOverlapping(const Vec3<double> center,
	const Vec3<double> radius,
	const Shape shape,
	const std::vector<Vec3<double>> *elementCenters)
{
	if (elementCenters->empty())
	{
		return false;
	}

	for (auto el : *elementCenters)
	{
		if (shape == sphere)
		{
			if (center.dist(el) < 2.0*radius.length())
			{
				return true;
			}
		}
		else if (shape == cube)
		{
			if (center.dist(el) < 2.0*radius.length())
			{
				return true;
			}
		}
	}

	return false;
}


Vec3<double> DataGenerator::addRandomElement(const Vec3<double> radius)
{
	// TODO
	// 0.0 <= radius <= 0.5
	assert(radius < Vec3<double>(1.0) && radius > Vec3<double>(0.0));
	Vec3<double> center(0.0, 0.0, 0.0);
	do
	{
		center.x = static_cast<double>(std::rand()) / static_cast<double>(RAND_MAX); // RAND_MAX 32767
	} while (center.x - radius.x < 0.0 || center.x + radius.x > 1.0);
	do
	{
		center.y = static_cast<double>(std::rand()) / static_cast<double>(RAND_MAX);
	} while (center.y - radius.y < 0.0 || center.y + radius.y > 1.0);
	do
	{
		center.z = static_cast<double>(std::rand()) / static_cast<double>(RAND_MAX);
	} while (center.z - radius.z < 0.0 || center.z + radius.z > 1.0);
	return center;
}

Vec3<double> DataGenerator::addHaltonElement(const Vec3<double> radius, int baseX, int baseY, int baseZ, long long &index) {

	assert(radius < Vec3<double>(1.0) && radius > Vec3<double>(0.0));
	Vec3<double> center(0.0, 0.0, 0.0);
	/*while (!isPrime(baseX)) {
		std::cerr << "baseX is not a prime number! \n" <<
			"Please enter a prime number: " << std::endl;
		std::cin >> baseX;
	}
	while (!isPrime(baseY)) {
		std::cerr << "baseY is not a prime number! \n" <<
			"Please enter a prime number: " << std::endl;
		std::cin >> baseY;
	}
	while (!isPrime(baseZ)) {
		std::cerr << "baseZ is not a prime number! \n" <<
			"Please enter a prime number: " << std::endl;
		std::cin >> baseZ;
	}*/
	bool xBounds = false;
	bool yBounds = false;
	bool zBounds = false;

	do {
		center.x = Halton_Seq(index, baseX);
		center.y = Halton_Seq(index, baseY);
		center.z = Halton_Seq(index, baseZ);

		xBounds = center.x - radius.x < 0.0 || center.x + radius.x > 1.0;
		yBounds = center.y - radius.y < 0.0 || center.y + radius.y > 1.0;
		zBounds = center.z - radius.z < 0.0 || center.z + radius.z > 1.0;

		++index;
	} while (xBounds || yBounds || zBounds);

	return center;
}

std::vector<Vec3<double>> DataGenerator::addElementsStructured(long long n, Vec3<int> dimBodies)
{
	std::vector<Vec3<double>> centers;
	Vec3<double> spacing = Vec3<double>(1.0) / static_cast<Vec3<double>>(dimBodies);
	for (int i = 0; i < dimBodies.x; ++i)
	{
		for (int j = 0; j < dimBodies.y; ++j)
		{
			for (int k = 0; k < dimBodies.z; ++k)
			{
				if (centers.size() == (size_t)n)
				{
					return centers;
				}
				Vec3<double> center(i, j, k);
				center *= spacing;
				center += spacing * 0.5;
				centers.push_back(center);
			}
		}
	}

	assert(centers.size() == (size_t)n);
	return centers;
}

int DataGenerator::generateVolume(DataConfig cfg)
{
	long long volSize = static_cast<long long>(cfg.res.x) * static_cast<long long>(cfg.res.y) *
		static_cast<long long>(cfg.res.z);

	m_isShape.clear();
	m_isShape.resize(volSize, false);

	m_volData.clear();
	m_volData.resize(volSize, 0.0f);

	m_elementCenters.clear();
	m_elementCenters.resize(cfg.numBodies);

	Vec3<double> diameter = cfg.coverage*0.01 / cfg.dimBodies;

	if (cfg.numBodies >= volSize / 2L) {
		std::cout << "Number of bodies is greater than half the volume resolution: "
			<< cfg.numBodies << " > " << volSize / 2L << std::endl;
		return false;
	}

	if (cfg.numBodies == 1L) {
		m_elementCenters.at(0) = Vec3<double>(0.5, 0.5, 0.5);
	}
	else if (cfg.randomBodyLayout == 1 && cfg.coverage < Vec3<int>(80)) {	// pseudorandom
		long long i = 0;
		m_elementCenters.clear();

		while (i < cfg.numBodies)
		{
			Vec3<double> center = addRandomElement(diameter / 2.0);
			if (!isOverlapping(center, diameter / 2.0, cfg.shape, &m_elementCenters))
			{
				m_elementCenters.push_back(center);
				++i;
				std::cout << "Added element " << i << "/" << cfg.numBodies << std::endl;
			}
		}
		std::cout << m_elementCenters[0].x << " " << m_elementCenters[0].y << " " << m_elementCenters[0].z << std::endl;
	}
	else if (cfg.randomBodyLayout == 2 && cfg.coverage < Vec3<int>(80)) {	// halton sequence
		long long i = 0;
		long long h = 30;
		m_elementCenters.clear();

		while (i < cfg.numBodies)
		{
			Vec3<double> center = addHaltonElement(diameter / 2.0, 7, 37, 97, h);
			if (!isOverlapping(center, diameter / 2.0, cfg.shape, &m_elementCenters))
			{
				m_elementCenters.push_back(center);
				++i;
				std::cout << "Added element " << i << "/" << cfg.numBodies << std::endl;
				std::cout << "Index h = " << h << std::endl;
			}
		}
		std::cout << m_elementCenters[0].x << " " << m_elementCenters[0].y << " " << m_elementCenters[0].z << std::endl;
	}
	else {
		m_elementCenters = addElementsStructured(cfg.numBodies, cfg.dimBodies);
	}

	for (auto el : m_elementCenters)
	{
		//std::cout << el.x << " " << el.y << " " << el.z << std::endl;
		Vec3<int> iCenter = static_cast<Vec3<int>>(el * cfg.res);
		//std::cout << iCenter.x << " " << iCenter.y << " " << iCenter.z << std::endl;
		Vec3<int> iRange = static_cast<Vec3<int>>(diameter * 0.5 * cfg.res);

		//        assert(iCenter + iRange < cfg.res);
		//        assert(iCenter - iRange > Vec3<int>(0));

		double sphereRange = iRange.length() / SQRT_3;

#pragma omp parallel for
		for (int curX = iCenter.x - iRange.x; curX < iCenter.x + iRange.x; ++curX)
		{
			for (int curY = iCenter.y - iRange.y; curY < iCenter.y + iRange.y; ++curY)
			{
				for (int curZ = iCenter.z - iRange.z; curZ < iCenter.z + iRange.z; ++curZ)
				{
					long long id = curX + curY * cfg.res.x + curZ * cfg.res.x * cfg.res.y;
					double centerDist = (iCenter - Vec3<int>(curX, curY, curZ)).length();
					if (cfg.shape == cube || (cfg.shape == sphere && centerDist <= sphereRange))
					{
						m_isShape.at(id) = true;
					}
				}
			}
		}
	}
	return true;
}

int DataGenerator::generateTestVolume(DataConfig cfg)
{
	long long volSize = static_cast<long long>(cfg.res.x) * static_cast<long long>(cfg.res.y) *
		static_cast<long long>(cfg.res.z);

	m_isShape.clear();
	m_isShape.resize(volSize, false);

	m_volData.clear();
	m_volData.resize(volSize, 0.0f);

	m_elementCenters.clear();
	m_elementCenters.resize(cfg.numBodies);

	Vec3<double> diameter = cfg.coverage*0.01 / cfg.dimBodies;

	if (cfg.numBodies >= volSize / 2L)
	{
		std::cout << "Number of bodies is greater than half the volume resolution: "
			<< cfg.numBodies << " > " << volSize / 2L << std::endl;
		return false;
	}

	if (cfg.numBodies == 1L)
	{
		m_elementCenters.at(0) = Vec3<double>(0.5, 0.5, 0.5);
	}
	else if (cfg.randomBodyLayout && cfg.coverage < Vec3<int>(80))   // random layout
	{
		long long i = 0;
		while (i < cfg.numBodies)
		{
			Vec3<double> center = addRandomElement(diameter / 2.0);
			if (!isOverlapping(center, diameter / 2.0, cfg.shape, &m_elementCenters))
			{
				m_elementCenters.push_back(center);
				++i;
				std::cout << "Added element " << i << "/" << cfg.numBodies << std::endl;
			}
		}
	}
	else
	{
		m_elementCenters = addElementsStructured(cfg.numBodies, cfg.dimBodies);
	}

	for (auto el : m_elementCenters)
	{
		Vec3<int> iCenter = static_cast<Vec3<int>>(el * cfg.res);
		Vec3<int> iRange = static_cast<Vec3<int>>(diameter * 0.5 * cfg.res);

		//        assert(iCenter + iRange < cfg.res);
		//        assert(iCenter - iRange > Vec3<int>(0));

		double sphereRange = cfg.res.x / 2.0;
#pragma omp parallel for
		for (int curX = 0; curX < cfg.res.x; ++curX)
		{
			for (int curY = 0; curY < cfg.res.y; ++curY)
			{
				for (int curZ = 0; curZ < cfg.res.z; ++curZ)
				{
					long long id = curX + curY * cfg.res.x + curZ * cfg.res.x * cfg.res.y;
					double centerDist = (iCenter - Vec3<int>(curX, curY, curZ)).length();
					
					if (cfg.shape == cube || (cfg.shape == sphere && centerDist <= sphereRange))
					{
						m_isShape.at(id) = true;
					}
				}
			}
		}
	}

	return true;
}

int DataGenerator::generateScalarData(const DataConfig cfg)
{
	if (cfg.res.x < 1 || cfg.res.y < 1 || cfg.res.z < 1)
	{
		generateVolume(cfg);
	}

	if (m_volData.size() != static_cast<size_t>(cfg.res.x*cfg.res.y*cfg.res.z))
	{
		m_volData.resize(cfg.res.x*cfg.res.y*cfg.res.z);
	}

	/*std::cout << "\n" << "Press the corresponding number to select \none of the following functions to generate a global frequency: \n"
		<< "1: mag * (sin(frequency * coords) + 1.0) * 0.5 \n"
		<< "2: mag * (sin(frequency * coords)^2 + 1.0) * 0.5 \n"
		<< "3: mag * (e^(sin(frequency * coords)) / e + 1.0) * 0.5 \n"
		<< "4: mag * (cos(frequency * coords) + 1.0) * 0.5 \n"
		<< "5: mag * (cos(frequency * coords)^2 + 1.0) * 0.5 \n"
		<< "6: mag * (e^(cos(frequency * coords)) / e + 1.0) * 0.5 \n" 
		<< "7: mag * (cords % 6) < 3 ? 1 : -1 \n" 
		<< "8: mag * abs((cords % 6) - 3) / 3.0" << std::endl;*/
	int select{1};
	//std::cin >> select;
	switch (select) {
		case 1: mptr = &Frequency::sine; break;
		case 2: mptr = &Frequency::quadraticSine; break;
		case 3: mptr = &Frequency::sineExp; break;
		case 4: mptr = &Frequency::cosine; break;
		case 5: mptr = &Frequency::quadraticCosine; break;
		case 6: mptr = &Frequency::cosineExp; break;
		case 7: mptr = &Frequency::rect; break;
		case 8: mptr = &Frequency::triangle; break;
		default: mptr = &Frequency::sine; break;
	}

	Frequency freq;
	Perlin perlin;

	std::string str;
	std::cout << "Please enter a function." << std::endl;
	//std::cin >> str;

	// generate global frequency
#pragma omp parallel for
	for (int dirX = 0; dirX < cfg.res.x; ++dirX)
	{
		for (unsigned int dirY = 0; dirY < cfg.res.y; ++dirY)
		{
			for (unsigned int dirZ = 0; dirZ < cfg.res.z; ++dirZ)
			{
				long long id = dirX + dirY * cfg.res.x + dirZ * cfg.res.x * cfg.res.y;
				// fill bodies with scalar data of defined frequency

				//double x = static_cast<double>(dirX) / static_cast<double>(cfg.res.x);
				//double y = static_cast<double>(dirY) / static_cast<double>(cfg.res.y);
				//double z = static_cast<double>(dirZ) / static_cast<double>(cfg.res.z);
				//double noise = 10.0 * ((perlin.noise(Vec3<double>(x, y, z)) + 1.0) / 2.0);
				//noise = noise - floor(noise);
				//m_volData.at(id) = static_cast<float>(noise);

				if (m_isShape.at(id))
				{		
					double result = (freq.*mptr)(Vec3<int>(dirX, dirY, dirZ), cfg.magnitude, cfg.frequency, cfg.res);
					//double result = freq.userDef(Vec3<int>(dirX, dirY, dirZ), cfg.magnitude, cfg.frequency, cfg.res, str);
					m_volData.at(id) = static_cast<float>(result);
				}
			}
		}
	}

	return true;
}
