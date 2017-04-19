//a tool to recompose an event
//first, a single point.
//then, optionally, the full track across the event.

#ifndef BRE_STITCH__H
#define BRE_STITCH__H

#include <vector>

#include <string.h>
#include <stdio.h>
#include <math.h>

#include "bre_data_types.h"

namespace BRE{
	//----------------------------------------------------------------------------
	//this function associates a MCTrack with what happened and where.
	//it acts in the scope of a single event
	std::vector<fpt> stitch_points( std::vector<mctrk>&, std::vector<bre_detector_hit>& );
	std::vector<fpt> stitch_points( std::vector<mctrk>&, std::vector<rhit>& );
	//NOTE: an oevrload is a bit of an overkill, but saves a lot of time
	//      of engineering around a way to guess what data structure are
	//      we dealing with. So's that.
	//      The difference between the two overloads is how the type flag
	//      is set.
	
	//----------------------------------------------------------------------------
	//this is not atually a stitching problem, but since we're at it...
	void calc_gammabeta( std::vector<fpt> &pts );
}

#endif
