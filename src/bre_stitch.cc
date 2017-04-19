//implementation of the point reconstruction
#include "bre_stitch.h"

namespace BRE{
	//NOTE: these two function will do exactly the same job. The only actual
	//      difference in the code is the (type and the) value to which
	//      the type flag is set in the full point structure.
	
	//----------------------------------------------------------------------------
	//the version for the detector hit.
	std::vector<fpt> stitch_points( std::vector<mctrk> &trk, std::vector<rhit>& rh ){
		//basically, the track id of the rhit should be the index of
		//the MC tracks. But proceed with care.
		std::vector<fpt> pts( rh.size() ); //it will have the same size as rh,
		                                   //_not_ as trk.
	
		for( int i=0; i < rh.size(); ++i ){
			try{
				pts[i].trk = trk.at( rh[i].trk_id ); //this will throw if out of range
			}catch( std::exception e ){
				char e_mess[64];
				sprintf( e_mess, "MCTrack for trak ID %d not found!", i );
				throw bre_err( e_mess );
			}
			d2r( pts[i].dh ) = rh[i];
			pts[i].type_flag = 1; //set the type flag, so we know that we should
			                      //cast before we read... or be aware of it.
		}
		
		return pts;
	}
	
	//----------------------------------------------------------------------------
	//the version for the detector hit.
	std::vector<fpt> stitch_points( std::vector<mctrk> &trk,
	                                std::vector<bre_detector_hit>& dh ){
		//basically, the track id of the rhit should be the index of
		//the MC tracks. But proceed with care.
		std::vector<fpt> pts( dh.size() ); //it will have the same size as rh,
		                                   //_not_ as trk.
	
		for( int i=0; i < dh.size(); ++i ){
			try{
				pts[i].trk = trk.at( dh[i].trk_id ); //this will throw if out of range
			}catch( std::exception e ){
				char e_mess[64];
				sprintf( e_mess, "MCTrack for trak ID %d not found!", i );
				throw bre_err( e_mess );
			}
			pts[i].dh = dh[i];
			pts[i].type_flag = 0; //no type fiddling here...
		}
		
		return pts;
	}
	
	//----------------------------------------------------------------------------
	//let's calculate gamma and beta for the interaction points!
	void calc_gammabeta( std::vector<fpt> &pts ){
		double p_sq, gamma, beta;
		
		for( int i=0; i < pts.size(); ++i ){
			p_sq = pow( pts[i].dh.p_x, 2 ) +
			       pow( pts[i].dh.p_y, 2 ) +
			       pow( pts[i].dh.p_z, 2 ); //p = gamma*m_0*v;
			gamma = sqrt( 1 + p_sq/pow( pts[i].trk.mass, 2 ) );
			beta = sqrt( 1 - 1/pow( gamma, 2 ) );
			
			pts[i].gamma = gamma;
			pts[i].beta = beta;
		}
	}
}
