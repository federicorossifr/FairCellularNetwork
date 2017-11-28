//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#include "userDescriptor.h"

userDescriptor::userDescriptor(int id){
	this.id = id;
	fifo = new cQueue("FIFO-" + id,NULL);
	recv_bytes = 0;
}

bool userDescriptor::insertPacket(Packet* p){
	fifo.insert(p);
	return true;
}

Packet *userDescriptor::removePacket(){
	return fifo.pop();
}

void userDescriptor::setID(int value){
	id = value;
}

int userDescriptor::getID(){
	return id;
}

void userDescriptor::setRCVBT(int rcv){
	recv_bytes += rcv;
}

int userDescriptor::getRCVBT(){
	return recv_bytes;
}

void userDescriptor::setCQI(int cqi){
	curr_cqi = cqi;
}

int userDescriptor::getCQI(){
	return cqi;
}