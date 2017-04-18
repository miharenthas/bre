//data types to be used in this simple branch reader

#ifndef BRE_DATA_TYPES__H
#define BRE_DATA_TYPES__H

namespace BRE {
	//holds the data of the rattle hit
	//relevant to Paulina's recontruction
	typedef struct bre_rattle_hit {
		int trk_id; //track id
		float poe_x; //point-of-entry X
		float poe_y; //Y
		float poe_z; //and Z
		double e_loss; //energy loss
	} rhit;
	
	//holds the data out of the MCTRack
	//relevant to Paulina's reconstruction
	typedef struct bre_mctrack {
		int trk_id; //track id
		double mass; //the mass
	} mctrk;
	
	//hold the data of a detector hit
	//relevat to Paulina's reconstruction
	struct bre_detector_hit {
		int trk_id; //the track id
		double p_x; //3-momentum
		double p_y;
		double p_z;
		double e_loss; //the energy loss
	};
	
	//holds the data of the TOF wall
	//relevant to Paulina's reconstruction.
	//NOTE: it's the same structure for both DCH
	//      and the TOF wall, so it's just aliasing
	typedef struct bre_detector_hit tofp; //TOF hit
	typedef struct bre_detector_hit dchp; //DCH hit
	
	//The full relevant stuff of the event
	typedef struct bre_full_track{
		int trk_id;
		double beta;
		double gamma;
	} ftrk;
}

#endif
		
