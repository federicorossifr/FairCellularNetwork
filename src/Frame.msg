cplusplus {{
//This is required to include a class definition from
//of a non cObject element
#include "ResourceBlock.h"
//Since omnet does not allow to use explicit pointers
//in message definition, we mock a pointer with this definition
typedef ResourceBlock* ResourceBlockPtr;
}}

//Declare the previous definition as non cObject
//to be embedded in a message
class noncobject ResourceBlockPtr;

message Frame {
    ResourceBlockPtr _rbs[25];
}
