//a tiny test program

#include "bre_reader.h"
#include "bre_stitch.h"
#include "bre_data_types.h"

#include "TFile.h"
#include "TTree.h"

#include <vector>
#include <stdio.h>

int main( int argc, char **argv ){
	char fname[128];

	strcpy( fname, argv[1] );
	printf( "Opening %s\n", fname );
	TFile *f = TFile::Open( fname );
	TTree *tree = (TTree*)f->Get( "evt" );
	if( tree == NULL ){
		puts( "Dang." );
		exit( 1 );
	}

	std::vector<std::vector<BRE::rhit>> rh;
	std::vector<std::vector<BRE::mctrk>> mc;
	std::vector<std::vector<BRE::bre_detector_hit>> dh;
	
	//test readers
	int nb_read = BRE::read_branch( rh, tree, 0 );
	printf( "%d entities read.\n", nb_read );
	nb_read = BRE::read_branch( rh, tree, 1 );
	printf( "%d entities read.\n", nb_read );
	nb_read = BRE::read_branch( mc, tree );
	printf( "%d entities read.\n", nb_read );
	nb_read = BRE::read_branch( dh, tree, "DCHPoint" );
	printf( "%d entities read.\n", nb_read );
	nb_read = BRE::read_branch( dh, tree, "TOFPoint" );
	printf( "%d entities read.\n", nb_read );

	//test the cast
	BRE::tofp tp = {0, 0, 0, 0, 0, 0};
	BRE::rhit rhh = {1, 2, 3, 4, 5, 6};
	BRE::d2r( tp ) = rhh;
	printf( "struct copy: %d, %f, %f, %f, %f, %f\n",
                tp.trk_id, tp.p_x, tp.p_y, tp.p_z, tp.e_loss, tp.time );

	//test the stitcher
	int count = 0;
	std::vector<BRE::fpt> fp;
	for( int i=0; i < rh.size(); ++i ){
		try{
			fp = BRE::stitch_points( mc[i], rh[i] );
			BRE::calc_gammabeta( fp );
			count += fp.size();
		}catch( BRE::bre_err e ){
			printf( "\"%s\" at %d.\n", e.what(), i );
			continue;
		}
	}
	printf( "stitched: %d\n", count );
	count = 0;
	for( int i=0; i < rh.size(); ++i ){
		try{
			fp = BRE::stitch_points( mc[i], rh[i] );
			BRE:calc_gammabeta( fp );
			count += fp.size();
		}catch( BRE::bre_err e ){
			printf( "\"%s\" at %d.\n", e.what(), i );
			continue;
		}
		count += fp.size();
	}
	printf( "stitched: %d\n", count );

	

	return 0;
}
