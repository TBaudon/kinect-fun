/*
 * DataHandler.h
 *
 *  Created on: 14 févr. 2012
 *      Author: thomas
 */

#ifndef DATAHANDLER_H_
#define DATAHANDLER_H_

#include <stdint.h>

class DataHandler {

public:

	DataHandler();

	virtual ~DataHandler();

	virtual void depthFunc(uint16_t* depth);

	virtual void rgbFunc(uint8_t* rgb);

};

#endif /* DATAHANDLER_H_ */
