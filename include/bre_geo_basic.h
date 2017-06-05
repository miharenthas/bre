//a couple of function that will ramage around the bgeo tracks and will assist
//into creating a bricsCAD script to dump the tracks into a CAD file.

#ifndef BRE_GEO_BASIC__H
#define BRE_GEO_BASIC__H

#include <stdio.h>

#include <vector>
#include <algorithm>
#include <functional>

#include <gsl/gsl_matrix.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_blas.h>

#include "bre_data_types.h"

namespace BRE{
	//a type saver
	typedef std::vector< std::vector<bgeo> > bgeo_vec;

	//----------------------------------------------------------------------------
	//functions to manipulate the tracks
	
	//A tool to select on PDG
	int bgeo_select_PDG( bgeo_vec &geo_info, int pdg );

	//A tool to transform the coordinates of the tracks
	//NOTE: the transformation is taken as-is and NOT affine
	void bgeo_transform3( bgeo_vec &geo_info, const gsl_matrix *trf );
	void bgeo_translate3( bgeo_vec &geo_info, const gsl_vector *trl );

	//A tool to dump the tracks onto a stream
	int bgeo_fprintf( FILE *stream, bgeo_vec &geo_info,
	                  bool pdg, bool time, unsigned int n=-1 ); //YES, I know what I'm doing here.
	
	int bgeo_export_to_CADscript( FILE *stream, bgeo_vec &geo_info,
	                              unsigned int n=-1 );
	                              
	//----------------------------------------------------------------------------
	//some little support tools
	
	//a class to compare on PDG
	class is_not_PDG : public std::unary_function<bgeo, bool> {
		public:
			is_not_PDG(): _pdg( 0 ) {};
			is_not_PDG( int pdg ): _pdg( pdg ) {};
			is_not_PDG( is_not_PDG const &given ): _pdg( given._pdg ) {};
			~is_not_PDG() {};
			
			bool operator()( const bgeo &given ) const { return _pdg != given.pdg; };
			
			is_not_PDG &operator=( const is_not_PDG &right ){
				_pdg = right._pdg;
				return *this;
			};
			is_not_PDG &operator=( const int right ){
				_pdg = right;
				return *this;
			};

			int _pdg;
	};
		
	
}

#endif
