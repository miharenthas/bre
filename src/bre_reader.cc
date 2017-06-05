//implementation of the branch reader
#include "bre_reader.h"

namespace BRE{
	//----------------------------------------------------------------------------
	//the MCTrack overload
	int read_branch( std::vector< std::vector<mctrk> > &data, TTree *the_tree ){
		//test if the branch "MCTrach" exists in the tree
		//if not, throw an exception
		TBranch *br = the_tree->GetBranch( "MCTrack" );
		if( br == NULL ){
			throw bre_err( "MCTrack branch not found!" );
		}
		
		//Ok, we have a branch, allocate the TClonesArray things
		TClonesArray arr( "R3BMCTrack", 16384 ), *p_arr;
		p_arr = &arr;
		
		//connect the branch to its TClonesArray
		the_tree->SetBranchAddress( "MCTrack", &p_arr );
		
		//prepare the data buffer
		data.clear();
		data.resize( the_tree->GetEntriesFast() );
		int nb_elements = 0;
		
		//loop on them
		mctrk t_buf; //track buffer
		R3BMCTrack *p_elem; //element pointer
		for( int evt=0; evt < the_tree->GetEntriesFast(); ++evt ){
			br->GetEntry( evt );
			for( int i=0; i < arr.GetEntriesFast(); ++i ){
				p_elem = (R3BMCTrack*)arr[i];
				t_buf.own_id = i; //it seems that the index is
				                  //the track number here
				t_buf.parent_id = p_elem->GetMotherId();
				t_buf.pdg = p_elem->GetPdgCode();
				t_buf.mass = p_elem->GetMass();
				
				data[evt].push_back( t_buf );
				++nb_elements; //increase the element counter.
			}
		}
		
		return nb_elements;
	}
	
	//-----------------------------------------------------------------------------
	//the rattle hits reader
	int read_branch( std::vector< std::vector<rhit> > &data, TTree *the_tree, int number ){
		//test if the branch "MCTrack" exists in the tree
		//if not, throw an exception
		char b_name[64];
		sprintf( b_name, "Rattles_%06x", number );
		TBranch *br = the_tree->GetBranch( b_name );
		if( br == NULL ){
			throw bre_err( "Rattles branch not found!" );
		}
		
		//Ok, we have a branch, allocate the TClonesArray things
		TClonesArray arr( "R3BRPHit", 16384 ), *p_arr;
		p_arr = &arr;
		
		//connect the branch to its TClonesArray
		the_tree->SetBranchAddress( b_name, &p_arr );

		//prepare the data buffer
		data.clear();
		data.resize( the_tree->GetEntriesFast() );
		int nb_elements = 0;
		
		//loop on them
		rhit t_buf; //track buffer
		R3BRPHit *p_elem; //element pointer
		for( int evt=0; evt < the_tree->GetEntriesFast(); ++evt ){
			br->GetEntry( evt );
			for( int i=0; i < arr.GetEntriesFast(); ++i ){
				//fill the buffer
				p_elem = (R3BRPHit*)arr[i];
				t_buf.trk_id = p_elem->GetTrackID();
				t_buf.own_pdg = p_elem->_pdg;
				t_buf.poe_x = p_elem->_poe.X();
				t_buf.poe_y = p_elem->_poe.Y();
				t_buf.poe_z = p_elem->_poe.Z();
				t_buf.e_loss = p_elem->_e_loss;
				
				data[evt].push_back( t_buf );
				++nb_elements; //increase the element counter.
			}
		}
		
		return nb_elements;
	}
	
	//----------------------------------------------------------------------------
	//detector branches
	int read_branch( std::vector< std::vector<bre_detector_hit> > &data,
	                   TTree *the_tree,
	                   const char *branch_name ){
		//test if the branch "MCTrach" exists in the tree
		//if not, throw an exception
		TBranch *br = the_tree->GetBranch( branch_name );
		if( br == NULL ){
			throw bre_err( "Branch not found!" );
		}
		
		//Ok, we have a branch, allocate the TClonesArray things
		char class_name[64];
		if( strstr( branch_name, "DCH" ) ) strcpy( class_name, "R3BDchPoint" );
		else if( strstr( branch_name, "TOF" ) ) strcpy( class_name, "R3BTofPoint" );
		else return 0;
		TClonesArray arr( class_name, 16384 ), *p_arr;
		p_arr = &arr;
		
		//connect the branch to its TClonesArray
		the_tree->SetBranchAddress( branch_name, &p_arr );
		
		//prepare the data buffer
		data.clear();
		data.resize( the_tree->GetEntriesFast() );
		int nb_elements = 0;
		
		//loop on them
		dchp t_buf; //track buffer
		R3BDchPoint *p_elem; //element pointer
		for( int evt=0; evt < the_tree->GetEntriesFast(); ++evt ){
			br->GetEntry( evt );
			for( int i=0; i < arr.GetEntriesFast(); ++i ){
				p_elem = (R3BDchPoint*)arr[i];
				t_buf.trk_id = p_elem->GetTrackID();
				t_buf.p_x = p_elem->GetPx();
				t_buf.p_y = p_elem->GetPy();
				t_buf.p_z = p_elem->GetPz();

				data[evt].push_back( t_buf );
				++nb_elements; //increase the element counter.
			}
		}
		
		return nb_elements;
	}

	//----------------------------------------------------------------------------
	//This overload reads the TGeoTracks into the bgeo structure
	int read_branch( std::vector< std::vector<bgeo> > &data, TTree *the_tree ){
		//try to get hold of the TGeoTracks branch.
		TBranch *br = the_tree->GetBranch( "GeoTracks" );
		if( br == NULL ){
			throw bre_err( "Branch not found!" );
		}
		
		//We have the TGeoTracks branch. Let's read it.
		TClonesArray arr( "TGeoTrack", 16384 ), *p_arr;
		p_arr = &arr;
		
		//connect the branch
		the_tree->SetBranchAddress( "GeoTracks", &p_arr );
		
		//prep the buffer
		data.clear();
		data.resize( the_tree->GetEntriesFast() );
		int nb_elements = 0;
		
		//loop on the events
		bgeo t_buf; //the track buffer.
		bgeo_pt *pt_buf; //single point buffer, same size as double[4].
		TGeoTrack *p_elem; //pointer to the current element
		for( int evt=0; evt < the_tree->GetEntriesFast(); ++evt ){
			br->GetEntry( evt );
			//loop in event's members
			for( int i=0; i < arr.GetEntriesFast(); ++i ){
				p_elem = (TGeoTrack*)arr[i];
				//if the track is empty, skip it
				if( !p_elem->HasPoints() ) continue;
				
				//allocate the track buffer
				t_buf.npts( p_elem->GetNpoints() );
				t_buf.pdg = p_elem->GetPDG();
				//loop on the track's point
				for( int p=0; p < t_buf.npts(); ++p ){
					pt_buf = (bgeo_pt*)p_elem->GetPoint( p );
					t_buf.pts[p] = *pt_buf;
				}
				//add the thing to the data buffer
				data[evt].push_back( t_buf );
				++nb_elements;
			}
		}
		
		return nb_elements;
	}
}
