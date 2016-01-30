/*
 * DataHandler.cpp
 *
 *  Created on: 14 févr. 2012
 *      Author: thomas
 */

#include "DataHandler.h"

#include <iostream>

DataHandler::DataHandler() {

}

DataHandler::~DataHandler() {
	// TODO Auto-generated destructor stub
}

void DataHandler::depthFunc(uint16_t* depth){

	std::cout << depth[320*7] << std::endl;

}

void DataHandler::rgbFunc(uint8_t* rgb){

	std::cout << "RGB: " << rgb[0] << ", " << rgb[1] << ", " << rgb[2] << std::endl;

}
