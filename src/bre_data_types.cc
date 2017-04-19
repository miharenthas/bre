//implementation of the two unelegant type conversion utilities

#include "bre_data_types.h"

namespace BRE{
	//----------------------------------------------------------------------------
	//detector hit to rattle hit
	rhit &d2r( bre_detector_hit &given ){
		return *(rhit*)(void*)&given;
	}

	//----------------------------------------------------------------------------
	//rattle hit to detector hit
	bre_detector_hit &r2d( rhit &given ){
		return *(bre_detector_hit*)(void*)&given;
	}
}
