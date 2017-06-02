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

	//----------------------------------------------------------------------------
	//The bgeo class
	
	//----------------------------------------------------------------------------
	//constructors:
	
	//default
	bgeo::bre_geometry_basic_info():
		_npts( 0 ),
		pdg( 0 ),
		pts( NULL )
	{}
	
	//parametric
	bgeo::bre_geometry_basic_info( const unsigned int nb_points ):
		_npts( nb_points )
	{
		
		//NOTE: size( bgeo_pts ) = 4*sizeof( double ).
		pts = (bgeo_pt*)malloc( nb_points*sizeof( bgeo_pt ) );
		
		if( pts == NULL ){
			throw bre_err( "bgeo: memory error." );
		}
	}
	
	//copy
	bgeo::bre_geometry_basic_info( const bgeo &given ):
		_npts( given._npts ),
		pdg( given.pdg )
	{
		//NOTE: size( bgeo_pts ) = 4*sizeof( double ).
		pts = (bgeo_pt*)malloc( _npts*sizeof( bgeo_pt ) );
		
		if( pts == NULL ){
			throw bre_err( "bgeo: memory error." );
		}
		
		memcpy( pts, given.pts, _npts*sizeof( bgeo_pt ) );
	}
	
	//dtor:
	bgeo::~bre_geometry_basic_info(){
		free( pts );
	}
	
	//----------------------------------------------------------------------------
	//operators:
	
	//----------------------------------------------------------------------------
	//copy:
	bgeo &bre_geometry_basic_info::operator=( const bgeo &right ){
		pdg = right.pdg;
		_npts = right._npts;
		
		if( pts != NULL ) pts = (bgeo_pt*)realloc( pts, _npts*sizeof( bgeo_pt ) );
		else pts = (bgeo_pt*)malloc( _npts*sizeof( bgeo_pt ) );
		
		memcpy( pts, right.pts, _npts*sizeof( bgeo_pt ) );
		
		return *this;
	}
	
	//----------------------------------------------------------------------------
	//methods:
	
	//----------------------------------------------------------------------------
	//set the buffer size
	unsigned int bre_geometry_basic_info::npts( const unsigned int size ){
		_npts = size;
		
		if( pts != NULL ) pts = (bgeo_pt*)realloc( pts, _npts*sizeof( bgeo_pt ) );
		else pts = (bgeo_pt*)malloc( _npts*sizeof( bgeo_pt ) );
		
		return _npts*sizeof( bgeo_pt );
	}
}
