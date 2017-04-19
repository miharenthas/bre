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
				t_buf.trk_id = i; //it seems that the index is
				                  //the track number here
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
			throw bre_err( "MCTrack branch not found!" );
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
}