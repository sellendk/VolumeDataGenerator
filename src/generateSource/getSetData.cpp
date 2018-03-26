#include <vector>
#include <fstream>
#include <iostream>
#include <string>

#include "datagenerator.hpp"
#include "rapidxml.hpp"
#include "json.hpp"
#include "getSetData.hpp"

using json = nlohmann::json;

/**
* @brief helper function for the switch in getXMLConfig
* @param str the string to convert into an int
* @param h
* @return a converted string as an integer
*/
constexpr unsigned int str2int(const char* str, int h = 0)
{
	return !str[h] ? 5381 : (str2int(str, h + 1) * 33) ^ str[h];
}

bool GetSetData::saveRawData(std::vector<float> &volData, const DataConfig &cfg)
{
	/* 
	* user-defined output file name. 
	* has to end with .raw
	*/

	/*std::string output_name;
	std::cout << "Enter your output file name \"filename.raw\"" << std::endl;
	std::cin >> output_name;*/
	std::ofstream raw("volData.raw", std::ios::out | std::ios::binary);

	/*
	* converting the scalar data to the desired precision
	* given in the current config
	* and store it in the output file which is defined above
	*/
	switch (cfg.precision)
	{
	case 0:
		for (auto voxel : volData)
		{
			unsigned char data = static_cast<unsigned char>(voxel * static_cast<float>(UCHAR_MAX));
			raw.write(reinterpret_cast<const char*>(&data), sizeof(unsigned char));
		}
		break;
	case 1:
		for (auto voxel : volData)
		{
			unsigned short data = static_cast<unsigned short>(voxel * static_cast<float>(USHRT_MAX));
			raw.write(reinterpret_cast<const char*>(&data), sizeof(unsigned short));
		}
		break;
	case 2:
		for (auto voxel : volData)
		{
			float data = voxel * static_cast<float>(FLT_MAX);
			raw.write(reinterpret_cast<const char*>(&data), sizeof(float));
		}
		break;
	case 3:
		for (auto voxel : volData)
		{
			double data = static_cast<double>(voxel * static_cast<float>(ULLONG_MAX));
			raw.write(reinterpret_cast<const char*>(&data), sizeof(double));
		}
		break;
	default:
		for (auto voxel : volData)
		{
			unsigned char data = static_cast<unsigned char>(voxel * static_cast<float>(UCHAR_MAX));
			raw.write(reinterpret_cast<const char*>(&data), sizeof(unsigned char));
		}
	}

	raw.close();

	// changes the ending of the output file name from .raw to .dat
	//output_name.replace(output_name.length() - 4, 4, ".dat");

	// saves a .dat file which is named equal to the .raw file
	std::ofstream dat("volData.dat", std::ios::out | std::ios::binary);

	dat << "ObjectFileName: \t" << "volData.raw" << std::endl;
	dat << "Resolution: \t\t" << cfg.res.x << " " << cfg.res.y << " " << cfg.res.z << std::endl;
	dat << "SliceThickness: \t" << "1.0 1.0 1.0" << std::endl;
	dat << "Format: \t\t\t";
	switch (cfg.precision) {
		case 1: dat << "UCHAR" << std::endl; break;
		case 2: dat << "USHRT" << std::endl;; break;
		case 3: dat << "FLT" << std::endl;; break;
		case 4: dat << "DBL" << std::endl;; break;
		default: dat << "UCHAR" << std::endl; break;
	}

	dat.close();

	return true;
}

bool GetSetData::getXMLConfig(const std::string &file, DataConfig &cfg, std::vector<DataConfig> &configs) {
	
	std::ifstream config(file);
	rapidxml::xml_document<> doc;

	std::vector<char> buffer((std::istreambuf_iterator<char>(config)), std::istreambuf_iterator<char>());

	buffer.push_back('\0');

	doc.parse<0>(&buffer[0]);

	/*
	* parses through all the nodes of the xml file
	* and writes the read values into the config
	* the helper function str2int defined above is used here
	* that a switch can be used to recognize the different config nodes
	*/
	rapidxml::xml_node<> *node = doc.first_node()->first_node();
	for (rapidxml::xml_node<> *outer = node; outer; outer =  outer->next_sibling()) {
		for (rapidxml::xml_node<> *sib = outer->first_node(); sib; sib = sib->next_sibling()) {

			//assert(sib->first_attribute()->value() == NULL && sib->value() == NULL && "The config in your xml contains a nullptr.");

			// case sensitive
			switch (str2int(sib->name())) {
			case str2int("Precision"): cfg.precision = std::stoi(sib->value()); break;

			case str2int("Resolution"):cfg.res = Vec3<int>(std::stoi(sib->first_attribute()->value()),
				std::stoi(sib->first_attribute()->next_attribute()->value()),
				std::stoi(sib->last_attribute()->value())); break;

			case str2int("Coverage"): cfg.coverage = Vec3<double>(std::stod(sib->first_attribute()->value()),
				std::stod(sib->first_attribute()->next_attribute()->value()),
				std::stod(sib->last_attribute()->value())); break;

			case str2int("Shape"): if (strcmp(sib->value(), "cube") == 0) cfg.shape = cube;
				if (strcmp(sib->value(), "sphere") == 0) cfg.shape = sphere; break;

			case str2int("numBodies"): cfg.numBodies = std::stoll(sib->value()); break;

			case str2int("dimBodies"): cfg.dimBodies = Vec3<int>(std::stoi(sib->first_attribute()->value()),
				std::stoi(sib->first_attribute()->next_attribute()->value()),
				std::stoi(sib->last_attribute()->value())); break;

			case str2int("randomBodyLayout"): if (strcmp(sib->value(), "false") == 0) cfg.randomBodyLayout = 0;
				if (strcmp(sib->value(), "true") == 0) cfg.randomBodyLayout = 1; break;

			case str2int("Frequency"): cfg.frequency = Vec3<int>(std::stoi(sib->first_attribute()->value()),
				std::stoi(sib->first_attribute()->next_attribute()->value()),
				std::stoi(sib->last_attribute()->value())); break;

			case str2int("Magnitude"): cfg.magnitude = Vec3<double>(std::stod(sib->first_attribute()->value()),
				std::stod(sib->first_attribute()->next_attribute()->value()),
				std::stod(sib->last_attribute()->value())); break;

			default: std::cout << "XML name do not match the config name."; break;
			}
		}
		configs.push_back(cfg);
	}
	cfg = configs[0];
	config.close();

	return true;
}

bool GetSetData::getJSONConfig(const std::string &file, DataConfig &cfg, std::vector<DataConfig> &configs) {
	std::ifstream inFile(file, std::ios::in, std::ifstream::binary);
	json config;
	inFile >> config;

	try {
		/*
		* get all given config out of the file
		* temporary store it in cfg and then store it finally in configs
		*/
		for (auto c : config) {
			// case sensitive
			cfg.precision = c["Precision"];
			cfg.res = Vec3<int>(c["Resolution"][0], c["Resolution"][1], c["Resolution"][2]);
			cfg.coverage = Vec3<double>(c["Coverage"][0], c["Coverage"][1], c["Coverage"][2]);

			std::string s = c["Shape"];
			if (strcmp(s.c_str(), "cube") == 0) cfg.shape = cube;
			if (strcmp(s.c_str(), "sphere") == 0) cfg.shape = sphere;

			cfg.numBodies = static_cast<long long>(c["numBodies"]);
			cfg.dimBodies = Vec3<int>(c["dimBodies"][0], c["dimBodies"][1], c["dimBodies"][2]);
			cfg.randomBodyLayout = c["randomBodyLayout"][0];
			cfg.frequency = Vec3<int>(c["Frequency"][0], c["Frequency"][1], c["Frequency"][2]);
			cfg.magnitude = Vec3<double>(c["Magnitude"][0], c["Magnitude"][1], c["Magnitude"][2]);

			configs.push_back(cfg);

		}
		cfg = configs[0];
	}
	catch (const std::exception &e) {
		std::cerr << e.what() << std::endl;
		inFile.close();
		return false;
	}
	inFile.close();

	return true;
}