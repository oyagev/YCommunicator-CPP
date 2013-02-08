#include "YCommunicator.h"
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <malloc.h>


YCommPacket::YCommPacket(YSerializable * payload){
	this->setPayload(payload);

};

void YCommPacket::setPayload(YSerializable * payload){
	this->payload = payload;
	this->checksum = calcChecksum();
}


void YCommPacket::serialize(uint8_t * buffer){
	uint8_t size = this->getSize();
	uint8_t payload_size = this->payload->getSize();
	uint8_t serial_payload[payload_size];

	this->payload->serialize(serial_payload);


	buffer[0] = payload_size;
	memcpy(&buffer[1],serial_payload, payload_size);
	memcpy(&buffer[size-2],&this->checksum,2);


};

uint8_t YCommPacket::getSize(void){
	uint8_t size = 0;

	uint8_t payload_size = this->payload->getSize();

	size += sizeof(payload_size);
	size += sizeof(checksum);
	size += payload_size;


	return size;
}


uint16_t YCommPacket::calcChecksum(void){
	return 0;
}

YCommPacket * YCommPacket::unserialize(uint8_t * buffer, unsigned int size) {
	YCommPacket * p;

	uint8_t psize = buffer[0];

	uint8_t * payl = (uint8_t *)malloc(sizeof (uint8_t) * psize);
	memcpy(payl,&(buffer[1]),psize);

	uint16_t * ch;
	//memcpy(ch , &(buffer[size-2]),2);

	YCommInstruction * inst = YCommInstruction::unserialize(payl, psize);


	YCommPacket * pack = new YCommPacket(inst);


	return pack;
}




YCommInstruction::YCommInstruction(uint8_t command , uint8_t * data, uint8_t data_size){
	this->data = data;
	this->data_size = data_size;
	this->command = command;
	this->type = YCommInstruction::TYPE_CMD;
};

YCommInstruction::YCommInstruction(uint8_t type, uint8_t command , uint8_t * data, uint8_t data_size){
	this->data = data;
	this->data_size = data_size;
	this->command = command;
	this->type = type;

};

uint8_t YCommInstruction::getSize(void){
	uint8_t size = 0;

	size += sizeof(this->type);

	size += sizeof(this->command);
	size += this->data_size;

	return size;
}

void YCommInstruction::serialize(uint8_t * buffer){
	buffer[0] = this->type;
	buffer[1] = this->command;
	memcpy(&buffer[2],this->data, this->data_size);
}

YCommInstruction * YCommInstruction::unserialize(uint8_t * buffer, unsigned int size) {



	uint8_t type = buffer[0];
	uint8_t command = buffer[1];
	unsigned int dsize = size - sizeof(type) - sizeof(command);

	uint8_t * data = (uint8_t *)malloc(sizeof (uint8_t) * dsize);
	memcpy(data,&(buffer[2]),dsize);

	YCommInstruction * p = new YCommInstruction(type,command,data, (uint8_t)dsize);

	return p;
}

YCommSerialInputBuffer::YCommSerialInputBuffer(){
}

void YCommSerialInputBuffer::begin(void){
	reset();
}
void YCommSerialInputBuffer::reset(void){
	tmp_checksum=0;
	tmp_payload_size = 0;
	first_byte_received = false;
	tmp_curr_payload_byte = 0;
	expected_inst_size= 0;
	tmp_curr_byte=0;
	expected_packet_size=2;
	//free(tmp_stream);
}
bool YCommSerialInputBuffer::read(uint8_t byte){

	//std::cout << "Read byte: " << (int)byte << std::endl;
	if (tmp_curr_byte==0){
		expected_packet_size = (int)byte + 3;
		tmp_stream = (uint8_t * )malloc(sizeof(uint8_t) * (int)expected_packet_size);
	}

	tmp_stream[tmp_curr_byte] = byte;


	if (tmp_curr_byte == (expected_packet_size-1)){

		buildInstruction();
		reset();

	}else{
		tmp_curr_byte++;
	}
	return true;

}
bool YCommSerialInputBuffer::read(uint8_t * buffer , unsigned int length){
	for(unsigned int i=0; i<length; i++){
		read(buffer[i]);
	}
	return true;
}
bool YCommSerialInputBuffer::hasInstructions(){
	return !instructions.empty();
}
YCommInstruction YCommSerialInputBuffer::shiftInstruction(){



		YCommInstruction inst = *(instructions.begin());



		instructions.erase(instructions.begin());


		return inst;

}

void YCommSerialInputBuffer::buildInstruction(){


	YCommPacket * pack = YCommPacket::unserialize(tmp_stream, expected_packet_size);

	YCommInstruction * inst = (YCommInstruction *)pack->payload;

	instructions.push_back(*inst);


}

YCommSerialOutputBuffer::YCommSerialOutputBuffer(){};

uint16_t YCommSerialOutputBuffer::available(void){
	return buffer.size();
}

uint8_t YCommSerialOutputBuffer::shift(){
	uint8_t byte = (uint8_t)buffer.front();
	buffer.erase(buffer.begin());
	return byte;
}

void YCommSerialOutputBuffer::write(YCommPacket packet){
	uint16_t size = (uint16_t)packet.getSize();
	uint8_t tmpbuffer[size];
	packet.serialize(tmpbuffer);
	for(uint16_t i=0; i<size;i++){
		buffer.push_back(tmpbuffer[i]);
	}

}

void YCommSerialOutputBuffer::write(YCommInstruction inst){
	YCommPacket pack = YCommPacket(&inst);
	write(pack);
}

YCommunicator::YCommunicator(){
	inp = YCommSerialInputBuffer();
	out = YCommSerialOutputBuffer();
	inp.begin();

}

void YCommunicator::registerDefaultCallback(void(*callbackfunc)(YCommInstruction inst)){
	default_callback = callbackfunc;
}

void YCommunicator::registerCallback(uint8_t command, void(*callbackfunc)(YCommInstruction inst)){
	callbacks_map[command] = callbackfunc;
}

void YCommunicator::dispatch(YCommInstruction inst){
	out.write(inst);
}

void YCommunicator::dispatch(uint8_t command, uint8_t * data, uint16_t data_size){
	YCommInstruction inst = YCommInstruction(command,data,data_size);
	dispatch(inst);
}
void YCommunicator::dispatch(uint8_t type, uint8_t command, uint8_t * data, uint16_t data_size){
	YCommInstruction inst = YCommInstruction(type,command,data,data_size);
	dispatch(inst);
}

uint16_t YCommunicator::available(void){
	return out.available();
}
uint8_t YCommunicator::read(void){
	return out.shift();
}
void YCommunicator::write(uint8_t byte){
	inp.read(byte);
	if (inp.hasInstructions()){
		YCommInstruction inst = inp.shiftInstruction();
		if (callbacks_map.count(inst.command)){
			callbacks_map[inst.command](inst);
		}else{
			default_callback(inst);
		}
	}
}



