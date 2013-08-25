/*
 * test.cpp
 *
 *  Created on: Jan 11, 2013
 *      Author: oyagev
 */

#include "../src/YCommunicator.h"
#include <iostream>
#include <stdlib.h>

void printNextInstruction(YCommSerialInputBuffer * buff){
	std::cout << "has inst ? " << ((buff->hasInstructions()) ? "YES" : "NO!") << std::endl;
	if (buff->hasInstructions()){
		YCommInstruction inst = buff->shiftInstruction();

		std::cout << "Type: " << (int)inst.type << std::endl;
		std::cout << "Command: " << (int)inst.command << std::endl;
		std::cout << "Data: ";
		for (int i = 0; i<inst.data_size; i++){
			std::cout << (int)inst.data[i] << " ";
		}
		std::cout <<std::endl;
	}
	std::cout << "******************************"<<std::endl;
}

int main(void){


	YCommSerialInputBuffer r = YCommSerialInputBuffer();
	r.begin();
	r.read('A');
	r.read('B');
	r.read('C');
	r.read(2);
	r.read(66);
	r.read(0x7c);
	r.read('B');
	r.read('D');
	r.read('F');
	r.read('G');
	r.read('F');
	r.read('G');
	r.read(0);
	r.read(66);
	r.read(68);
	r.read(0x7c);

	for(int i=0;i<10;i++){
		printNextInstruction(&r);
	}




}


