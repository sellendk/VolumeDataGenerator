#ifndef GETSETDATA_HPP
#define GETSETDATA_HPP

#include <vector>
#include "datagenerator.hpp"

class GetSetData
{
public:
	/**
	* @brief saves the volume data in a .raw file
	* @param volData the data to be saved
	* @param cfg the cfg with which the data shall be stored
	* @return true if saving was successfull, false otherwise
	*/
	bool saveRawData(std::vector<float> &volData, const DataConfig &cfg);

	/**
	* @brief Reads a given config from an xml file and returns cfg=configs[0]
	* @param file The file from which the config comes from
	* @param cfg The config the read .xml config gets stored into
	* @param configs The config vector in which all the configs are stored in
	* @return true If gettig config was succesfull, standard and false otherwise
	*
	* if any of the attributes (precision, resolution... etc.) is not contained
	* in the xml file, the standard cfg value will remain
	*/
	bool getXMLConfig(const std::string &file, DataConfig &cfg, std::vector<DataConfig> &configs);

	/**
	* @brief Reads a given config from a json file and returns cfg=configs[0]
	* @param file The file from which the config(s) comes from
	* @param cfg The config of the read .xml config gets (temporary) stored into
	* @param configs The config vector in which all the configs are stored in
	* @return true If gettig config was succesfull, standard and false otherwise
	*
	* if any of the attributes (precision, resolution... etc.) is not contained
	* in the json file, the standard cfg value will remain
	*/
	bool getJSONConfig(const std::string &file, DataConfig &cfg, std::vector<DataConfig> &configs);

	/**
	* @brief getting a given config from m_configs
	* @param n the n-th config
	* @return m_config[n] returns the n-th config if n >= 0, the first one otherwise
	*/
	DataConfig getConfig(int n) {
		if (n < 0) return m_configs[0];
		return m_configs[n];
	}

	/**
	* @brief pushes a cfg into m_configs
	* @param cfg the cfg to be pushed
	*/
	void setConfig(DataConfig cfg) {
		m_configs.push_back(cfg);
	}

private: 
	// a vector of configs
	std::vector<DataConfig> m_configs;
};

#endif // GETSETDATA_H
