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

#ifndef __FAIRCELLULARNETWORK_USERDESCRIPTOR_H_
#define __FAIRCELLULARNETWORK_USERDESCRIPTOR_H_

#include <omnetpp.h>

using namespace omnetpp;

class userDescriptor
{
private:
	int id;
	int recv_bytes;
	int curr_cqi;
	cQueue fifo;

	bool insertPacket(Packet* p);
	Packet *removePacket();

	virtual userDescriptor();
	//virtual ~userDescriptor();
	
	void setID(int value);
	int getID();
	void setRCVBT(int rcv);
	int getRCVBT();
	void setCQI(int cqi);
	int getCQI();

};

#endif
