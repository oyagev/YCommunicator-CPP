/*
 * test.cpp
 *
 *  Created on: Jan 11, 2013
 *      Author: oyagev
 */

#include "../src/YCommunicator.h"
#include <iostream>
#include <stdlib.h>


int main(void){


	YCommSerialInputBuffer r = YCommSerialInputBuffer();
	r.begin();
	r.read(3);
	r.read('A');
	r.read('B');
	r.read('C');
	r.read(5);
	r.read(10);

	r.read(4);
	r.read('A');
	r.read('B');
	r.read('D');
	r.read('F');
	r.read(5);
	r.read(10);
	std::cout << "has inst " << (int)r.hasInstructions() << std::endl;
	YCommInstruction inst = r.shiftInstruction();


	std::cout << "Type: " << (int)inst.type << std::endl;
	std::cout << "Command: " << (int)inst.command << std::endl;
	std::cout << "Data: " << inst.data << std::endl;

	inst = r.shiftInstruction();


	std::cout << "Type: " << (int)inst.type << std::endl;
	std::cout << "Command: " << (int)inst.command << std::endl;
	std::cout << "Data: " << inst.data << std::endl;
	/*
	uint8_t command = 0xA;
	uint8_t data[5] ;
	data[1]=10;


	YCommInstruction inst = YCommInstruction(0x1,command , data, sizeof(data));
	inst.getSize();
	YCommPacket pack = YCommPacket(&inst);

	uint8_t length = pack.getSize();
	uint8_t buff[length];

	pack.serialize(buff);

	for (uint8_t i=0; i<length; i++){
		std::cout << "Serial " << (int)i << ": " << (int)buff[i] << std::endl;
	}
	*/



}
