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
	r.read('A');
	r.read('B');
	r.read('C');
	r.read(5);
	r.read(10);
	r.read(0x7c);

	r.read(0);
	r.read(0);
	r.read('B');
	r.read('D');
	r.read('F');
	r.read('G');
	r.read(0);
	r.read(0);
	r.read(0);
	r.read(0x7c);
	r.read('F');
	r.read('F');
	r.read(5);
	r.read(10);
	r.read(0x7c);
	std::cout << "has inst " << (int)r.hasInstructions() << std::endl;
	YCommInstruction inst = r.shiftInstruction();



	std::cout << "Type: " << (int)inst.type << std::endl;
	std::cout << "Command: " << (int)inst.command << std::endl;
	std::cout << "Data: " << inst.data << std::endl;

	inst = r.shiftInstruction();


	std::cout << "Type: " << (int)inst.type << std::endl;
	std::cout << "Command: " << (int)inst.command << std::endl;
	std::cout << "Data: ";
	for (int i = 0; i<inst.data_size; i++){
		std::cout << (int)inst.data[i] << " ";
	}
	std::cout << std::endl;

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
