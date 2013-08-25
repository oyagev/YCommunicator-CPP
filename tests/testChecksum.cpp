#include "../src/YCommunicator.h"
#include <iostream>
#include <stdlib.h>


int main(void){

	uint8_t data[7] = {123,87,0,0,63,233,1};
	uint16_t expected = 17854;
	uint16_t check = YCommunicator::checksum(data,7);
	std::cout << "Checksum is: " << (int)check << std::endl;
	std::cout << "Should be: " << (int)expected << std::endl;

	uint8_t type = data[0];
	uint8_t command = data[1];
	uint8_t data2[5] = {0,0,63,233,1};
	YCommInstruction inst = YCommInstruction(type,command,data2,5);
	YCommPacket pack = YCommPacket(&inst);
	std::cout << "Packet checksum is: " << (int)pack.checksum << std::endl;
	std::cout << "Should be: " << expected << std::endl;

}
