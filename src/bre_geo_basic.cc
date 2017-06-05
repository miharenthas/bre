//implementation of the basic geometry toolZ

#include "bre_geo_basic.h"

namespace BRE{
	//----------------------------------------------------------------------------
	//selection on PDG
	//auxiliary function
	inline bool isempty( const std::vector<bgeo> &given ){ return given.size() == 0; }
	int bgeo_select_PDG( bgeo_vec &geo_info, int pdg ){
		int nb_removed = 0;
		
		//loop over the events
		is_not_PDG pdg_ck( pdg );
		std::vector<bgeo>::iterator ends_here;
		for( int evt=0; evt < geo_info.size(); ++evt ){
			nb_removed += geo_info[evt].size();
			ends_here = std::remove_if( geo_info[evt].begin(),
			                            geo_info[evt].end(),
			                            pdg_ck );
			
			geo_info[evt].erase( ends_here,
			                     geo_info[evt].end() );
			
			nb_removed -= geo_info[evt].size();
		}
		
		//remove empty events
		bgeo_vec::iterator ends_also_here = std::remove_if( geo_info.begin(),
		                                                    geo_info.end(),
		                                                    isempty );
		geo_info.erase( ends_also_here, geo_info.end() );
		
		return nb_removed;
	}
	
	//----------------------------------------------------------------------------
	//apply a transformation to all the pointZ
	void bgeo_transform3( bgeo_vec &geo_info, const gsl_matrix *trf ){
		//loop over the events, of course in parallel
		gsl_vector_view vv_pt; //the vector view to transform
		#pragma omp parallel private( vv_pt )
		{
		gsl_vector *v_buf = gsl_vector_alloc( 3 );
		#pragma omp for
		for( int evt=0; evt < geo_info.size(); ++evt ){
			for( int t=0; t < geo_info[evt].size(); ++t ){
				for( int i=0; i < geo_info[evt][t].npts(); ++i ){
					vv_pt = gsl_vector_view_array( (double*)&geo_info[evt][t].pts[i], 3 );
				
					gsl_blas_dgemv( CblasNoTrans, 1, trf, &vv_pt.vector, 0, v_buf );
					
					gsl_vector_memcpy( &vv_pt.vector, v_buf );
				}
			}
		}
		gsl_vector_free( v_buf );
		} //parallel pragma ends here
	}
	
	//----------------------------------------------------------------------------
	//apply a translation to the pointZ
	void bgeo_translate3( bgeo_vec &geo_info, const gsl_vector *trl ){
		//loop over the events, of course in parallel
		gsl_vector_view vv_pt; //the vector view to transform
		#pragma omp parallel private( vv_pt )
		{
		#pragma omp for schedule( dynamic, 100 )
		for( int evt=0; evt < geo_info.size(); ++evt ){
			for( int t=0; t < geo_info[evt].size(); ++t ){
				for( int i=0; i < geo_info[evt][t].npts(); ++i ){
					vv_pt = gsl_vector_view_array( (double*)&geo_info[evt][t].pts[i], 3 );
				
					gsl_blas_daxpy( 1, trl, &vv_pt.vector );
				}
			}
		}
		} //parallel pragma ends here
	}
	
	//----------------------------------------------------------------------------
	//print the tracks to a stream
	int bgeo_fprintf( FILE *stream, bgeo_vec &geo_info, bool pdg, bool time, unsigned int n ){
		unsigned int count = 0;
		for( int evt=0; evt < geo_info.size(); ++evt ){
			for( int t=0; t < geo_info[evt].size(); ++t ){
				for( int i=0; i < geo_info[evt][t].npts(); ++i ){
					if( pdg ) fprintf( stream, "%d ", geo_info[evt][t].pdg );
					fprintf( stream, "%f %f %f ",
						       geo_info[evt][t].pts[i].x, 
						       geo_info[evt][t].pts[i].y,
						       geo_info[evt][t].pts[i].z );
					if( time ) fprintf( stream, "%f ", geo_info[evt][t].pts[i].t );
					fprintf( stream, "\n");
				}
			
				//put some zeros as separator.
				if( pdg ) fprintf( stream, "0 " );
				fprintf( stream, "0 0 0 " );
				if( time ) fprintf( stream, "0 " );
				fprintf( stream, "\n" );
			
				++count;
				if( count > n ) goto __END__;
			}
			
			//put some zeros as separator.
			if( pdg ) fprintf( stream, "0 " );
			fprintf( stream, "0 0 0 " );
			if( time ) fprintf( stream, "0 " );
			fprintf( stream, "\n" );
			
		}
		
		__END__:
		return count;
	}
	
	//----------------------------------------------------------------------------
	//write the tracks as polylines in a bricsCAD script
	int bgeo_export_to_CADscript( FILE *stream, bgeo_vec &geo_info, unsigned int n ){
		unsigned int count = 0;
		for( int evt=0; evt < geo_info.size(); ++evt ){
			for( int t=0; t < geo_info[evt].size(); ++t ){
				fprintf( stream, "_3dpoly\n" ); //init a 3d polyline
				for( int i=0; i < geo_info[evt][t].npts(); ++i ){
					fprintf( stream, "%f,%f,%f\n",
						       geo_info[evt][t].pts[i].x, 
						       geo_info[evt][t].pts[i].y,
						       geo_info[evt][t].pts[i].z );
				}
			
				//close the thing.
				fprintf( stream, "\n" );
			
				++count;
				if( count > n ) goto __END__;
			}
		}			
		
		__END__:
		fprintf( stream, "_stopscript\n" ); //close the script
		return count;
	}
}
