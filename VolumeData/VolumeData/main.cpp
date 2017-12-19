#include <iostream>
#include <string>

#include "datagenerator.hpp"
#include "getSetData.hpp"
#include "frequency.hpp"


int main(int argc, char *argv[])
{
	std::cout << "A volume data generator." << std::endl;

	// some declarations
	DataConfig cfg;					 // standard cfg
	DataGenerator dataGen;			 // used for the volume data generations
	GetSetData getSetData;			 // used for the config/data methods
	std::vector<DataConfig> configs; // vector of available configs read from a file

	// Configdatei einlesen
	std::string file;
	//getSetData.getJSONConfig("document.json", cfg, configs);
	while (false) {
		std::cout << "Enter a config file name: " << std::endl;
		std::getline(std::cin, file);
		if (file.find(".xml") != std::string::npos) {
			getSetData.getXMLConfig(file, cfg, configs); 
			break;
		}
		if (file.find(".json") != std::string::npos) {
			getSetData.getJSONConfig(file, cfg, configs); 
			break;
		}
		std::cerr << "No valid file name found. Please try again." << std::endl;
	}

	// print cfg for checking purposes
	std::cout << "coverage		   :" << cfg.coverage.x << " " << cfg.coverage.y << " " << cfg.coverage.z << std::endl;
	std::cout << "dimBodies	   	   :" << cfg.dimBodies.x << " " << cfg.dimBodies.y << " " << cfg.dimBodies.z << std::endl;
	std::cout << "frequency		   :" << cfg.frequency.x << " " << cfg.frequency.y << " " << cfg.frequency.z << std::endl;
	std::cout << "magnitude		   :" << cfg.magnitude.x << " " << cfg.magnitude.y << " " << cfg.magnitude.z << std::endl;
	std::cout << "res			   :" << cfg.res.x << " " << cfg.res.y << " " << cfg.res.z << std::endl;
	std::cout << "numBodies		   :" << cfg.numBodies << std::endl;
	std::cout << "precision		   :" << cfg.precision << std::endl;
	std::cout << "shape			   :" << cfg.shape << std::endl;
	std::cout << "randomBodyLayout	   :" << cfg.randomBodyLayout << std::endl;

	// generate volume
	std::cerr << "Generating volume data... ";
	dataGen.generateVolume(cfg);
	std::cerr << "done." << std::endl;

	// generate scalar data
	std::cerr << "Generating scalar data... ";
	dataGen.generateScalarData(cfg);
	std::cerr << "done." << std::endl;

	// print m_volData
	std::cerr << "Printing volume data... " << std::endl;
	//dataGen.printVolumeData();
	std::cerr << "done." << std::endl;

	// write volume data to file
	std::cerr << "Exporting data to file " << std::endl;
	std::vector<float> volData;

	if (!dataGen.getVolumeData(volData))
	{
		std::cout << "Error while exporting data: No generated data found." << std::endl;
	}

	//dataGen.finalData(volData);
	if (!getSetData.saveRawData(volData, cfg))
	{
		std::cerr << "Error while writing to file." << std::endl;
		//std::cin.get();
		return 0;
	}

	std::cerr << "done." << std::endl;

	/*Frequency freq;
	double mag = 5.0;
	double freqD = 3.0;
	double x = 0.5;
	std::string str = "2 * mag + freq^2 + 2 * x";
	std::cout << freq.evaluate(mag, freqD, x, str);*/
	
	std::cin.get();
	std::cin.get();

	return 1;
}
