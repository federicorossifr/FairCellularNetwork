cplusplus {{
#include "ResourceBlock.h"
typedef ResourceBlock* ResourceBlockPtr
}}
class noncobject SomeType;

message Frame {
    ResourceBlockPtr* _rbs[25];
}
