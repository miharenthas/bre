//data types to be used in this simple branch reader

#ifndef BRE_DATA_TYPES__H
#define BRE_DATA_TYPES__H

#include <stdlib.h> //malloc, free
#include <string.h> //memcpy

#include <vector>
#include <exception>
#include <string.h>

#define MAX_NB_PHYL_CHILDREN 8 //the maximum number of children particles
                               //created by one at _one_interaction point.

namespace BRE {
	//holds the data of the rattle hit
	//relevant to Paulina's recontruction
	//NOTE: this structure is now binary compatible
	//      to bre_detector_hit. I'm keeping it for a while
	//      because it's a bit more comfortable.
	//      The relevant bit is: the structures can now be
	//      copied into one another (very dirty).
	typedef struct bre_rattle_hit {
		int trk_id; //track id
		int own_pdg; //another PDG save
		double poe_x; //point-of-entry X
		double poe_y; //Y
		double poe_z; //and Z
		double e_loss; //energy loss
		double time; //timestamp
	} rhit;
	
	//holds the data out of the MCTRack
	//relevant to Paulina's reconstruction
	typedef struct bre_mctrack {
		int own_id; //track id
		int parent_id; //parent track id
		int pdg; //the pdg code
		double mass; //the mass
	} mctrk;
	
	//hold the data of a detector hit
	//relevat to Paulina's reconstruction
	struct bre_detector_hit {
		long int trk_id; //the track id
		                 //NOTE: it's long to maintain
		                 //      compatibility with rhit
		double p_x; //3-momentum
		double p_y;
		double p_z;
		double e_loss; //the energy loss
		double time; //timestamp
	};
	
	//holds the data of the TOF wall
	//relevant to Paulina's reconstruction.
	//NOTE: it's the same structure for both DCH
	//      and the TOF wall, so it's just aliasing
	typedef struct bre_detector_hit tofp; //TOF hit
	typedef struct bre_detector_hit dchp; //DCH hit
	typedef struct bre_detector_hit rhit_mom; //rattlehit with momentum.
	
	//The full relevant stuff of the event
	typedef struct bre_full_point{
		mctrk trk; //the MC track information
		double gamma; //relativistic gamma
		double beta; //and boost
		unsigned char type_flag; //if this is one,
		                          //dh is cast to rhit.
		bre_detector_hit dh; //the detector hit information
		                     //or, optionally, the rattle hit.		
	} fpt;
	
	//a node of a "phylogenetic" tree for the particle
	//a particle can have _one_ parent and som children
	//ideally, you can follow the particle from its creation
	//to its death, opting out on secondaries, tertiaries etc.
	//NOTE: the "logical order" represents the order of appearence
	//      of the particle: if we are following an ion, we can
	//      count on the fact that it's logical order will be the same.
	//      If this ion has appeared with no (known) parents, it will
	//      have order 0, and so on.
	typedef struct bre_particle_phylogeny{
		bre_particle_phylogeny *parent; //the parent
		unsigned short order; //the logical order
		fpt node; //whats's in this node
		bre_particle_phylogeny *children[MAX_NB_PHYL_CHILDREN]; //the children
	} pphyl;
	
	//----------------------------------------------------------------------------
	//a couple of data structure to hold basic geometry information
	typedef struct bre_geometry_basic_point{
		double x, y, z, t;
	} bgeo_pt;
	
	//this has to be a class...
	typedef class bre_geometry_basic_info{
		public:
			int pdg;
			bgeo_pt *pts; //a buffer of points
			
			bre_geometry_basic_info();
			bre_geometry_basic_info( const unsigned int nb_points );
			bre_geometry_basic_info( const bre_geometry_basic_info &given );
			~bre_geometry_basic_info();
			
			bre_geometry_basic_info &operator=( const bre_geometry_basic_info &right );
			
			unsigned int npts( const unsigned int sz );
			unsigned int npts() const { return _npts; };
		private:
			unsigned int _npts;
	} bgeo;

	//----------------------------------------------------------------------------
	//the error structure...
	class bre_err : public std::exception {
		public:
			bre_err( const char *message ){
				if( strlen( message ) < 128 )
					strcpy( _what, message );
				else
					strncpy( _what, message, 128 );
			};
			bre_err( const bre_err &given ){ strcpy( _what, given._what ); };
			bre_err &operator=( bre_err &right ){
				strcpy( _what, right._what );
				return *this;
			}
		
			~bre_err(){};
		
			const char *what() const noexcept{ return _what; };
		private:
			char _what[128];
	};

	//----------------------------------------------------------------------------
	//inelegant type conversion
	//NOTE: these function don't copy the data structures, but
	//      do a substantial cast of one into the other. This is
	//      safe, because the structures are effectively the same
	//      but be careful fiddling about in the memory like this.
	rhit &d2r( bre_detector_hit &given ); //detector hit to rattle hit
	bre_detector_hit &r2d( rhit &given ); //rattle hit to detector hit
}	

#endif
		
