cplusplus {{
#include "ResourceBlock.h"
typedef ResourceBlock* ResourceBlockPtr;
}}
class noncobject ResourceBlockPtr;

message Frame {
    ResourceBlockPtr _rbs[25];
}
