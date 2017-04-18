//a bunch of functions that read a given branch and return
//a STL vector out of the data structures defined in bre_data_types.

#ifndef BRE_READER__H
#define BRE_READER__H

#include <vector>
#include <exception>
#include <string.h>
#include <stdio.h>

//ROOT specific includes
#include "TClonesArray.h"
#include "TTree.h"
#include "TBranch.h"

//R3B specific includes
#include "R3BMCTrack.h"
#include "R3BTofPoint.h"
#include "R3BDchPoint.h"
#include "R3BRPHit.h"

//the data types
#include "bre_data_types.h"

namespace BRE{
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
		

	//the function is a four-way overload
	//the return value is always an integer with the
	//number of entries read (kinda redundant, but OK)
	//and the storage place is a reference to an
	//already existing STL vector of the correct type.
	//NOTE: the "number" in the rhit overload is the index of the rattle plane
	//      it will be converted in the correct name into the function.
	//      obviously, it won't work with older randomly generated names for
	//      the rattle hits, but they shouldn't be around nevertheless.
	int read_branch( std::vector<std::vector<rhit>> &data, TTree *the_tree, int number=0 );
	int read_branch( std::vector<std::vector<mctrk>> &data, TTree *the_tree );
	int read_branch( std::vector<std::vector<bre_detector_hit>> &data,
	                 TTree *the_tree,
	                 const char *branch_name );
}

#endif
