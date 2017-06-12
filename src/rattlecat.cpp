//This program reads a file with Rattleplanes inside and
//prints the coordinates (and some other stuff, optionally)
//to the standard output
//
//This program was inspired by work done by Paulina Mueller.

#include <stdio.h>
#include <getopt.h>
#include <vector>

#include "bre.h"

//root garbage
#include "TFile.h"
#include "TTree.h"
#include "TKey.h"

//I know it's a bit cringeworthy, but it's a life saver here
typedef std::vector< std::vector< BRE::rhit > > V_rhit;
typedef std::vector< std::vector< BRE::rhit_mom > > V_rmom;

//------------------------------------------------------------------------------------
//some interesting functions
TTree *get_ttree( TFile &file ); //gets the (first) tree out of a file
void cat_info( V_rhit &rattles ); //just coordinates
void cat_info_pdg( V_rhit &rattles ); //coordinates with PDG code
void cat_info( V_rmom &momenta ); //just the three momentum components
void cat_info_pdg( V_rmom &momenta ); //also with leading PDG code

//------------------------------------------------------------------------------------
//da main
int main( int argc, char **argv ){
	char in_fnames[64][512]; //input file buffers
	
	//parse the filenames (un -ed arguments at the
	//beginning of the command line)
	int in_fcount = 0;
	for( int i=1; i < argc; ++i ){
		if( argv[i][0] != '-' ){
			strcpy( in_fnames[in_fcount], argv[i] );
			++in_fcount;
		} else break;
	}
	if( in_fcount == 0 ){
		fprintf( stderr, "rattlecat: no input file." );
		exit( 1 );
	}
	
	//parse the options
	struct option opts[] = {
		{ "pdg", no_argument, NULL, 'p' }, //print leading PDG code
		{ "rattler-number", required_argument, NULL, 'r' }, //target one specific rattler
		{ "momenta", no_argument, NULL, 'P' }, //print the momenta instead of the PoE
		{ NULL, 0, NULL, 0 } //null terminated array
	};
	
	//input variables
	bool pdg_flag = false, momentum_flag = false;
	int rattler_idx = 0;
	
	char iota=0;
	int opt_idx;
	const char short_opts[] = "pPr:";
	while( ( iota = getopt_long( argc, argv, short_opts, opts, &opt_idx ) ) != -1 ){
		switch( iota ){
			case 'p' :
				pdg_flag = true;
				break;
			case 'r' :
				rattler_idx = atoi( optarg );
				break;
			case 'P' :
				momentum_flag = true;
				break;
			default :
				fprintf( stderr, "Usage: rattlecat [-p|--pdg -r|--rattler-number -P|--momenta]\n" );
				exit( 2 );
				break;
		}
	}
	
	//loop on the files
	V_rhit hit_buf;
	V_rmom mom_buf;
	TTree *tree;
	TFile *file;
	char branch_namebuf[32];
	for( int f=0; f < in_fcount; ++f ){
		//get the tree out of the file
		file = TFile::Open( in_fnames[f] );
		TTree *tree = get_ttree( *file );
		
		try{
			if( !momentum_flag ) BRE::read_branch( hit_buf, tree, rattler_idx );
			else{
				sprintf( branch_namebuf, "Rattles_%06x", rattler_idx );
				BRE::read_branch( mom_buf, tree, branch_namebuf );
			}
				
		} catch( BRE::bre_err e ){
			fprintf( stderr, "There has been an error with file \"%s\": ",
			         in_fnames[f] );
			fprintf( stderr, "%s\n", e.what() );
			continue;
		}
		
		
		//print the file to stdout
		if( !momentum_flag )
			if( pdg_flag ) cat_info_pdg( hit_buf );
			else cat_info( hit_buf );
		else
			if( pdg_flag )
				cat_info_pdg( mom_buf );
			else
				cat_info( mom_buf );
		
		
		//cleanup
		file->Close();
		file->Delete();
		hit_buf.clear();
		
		//puts a little separator
		printf( "\n\n" );
	}
	
	return 0;
}

//------------------------------------------------------------------------------------
//find the first tree and return its pointer
TTree *get_ttree( TFile &file ){
	//work out what is the name of the tree
	TIter nextkey( file.GetListOfKeys() );
	TKey *key;
	TObject *obj;
	TTree *data_tree;
	while( (key = (TKey*)nextkey()) ){
		obj = key->ReadObj();
		if( obj->IsA()->InheritsFrom( TTree::Class() ) ){
			data_tree = (TTree*)obj;
			break;
		}
	}
	
	return data_tree;
}

//------------------------------------------------------------------------------------
//print the events to the standard output
//just the rattles
void cat_info( V_rhit &rattles ){
	for( int e=0; e < rattles.size(); ++e ){ //loop on the events
		for( int i=0; i < rattles[e].size(); ++i ){
			printf( "%f %f %f\n", rattles[e][i].poe_x,
			                      rattles[e][i].poe_y,
			                      rattles[e][i].poe_z );
		}
		printf( "\n" );
	}
}

//------------------------------------------------------------------------------------
//rattles with PDG
void cat_info_pdg( V_rhit &rattles ){
	for( int e=0; e < rattles.size(); ++e ){ //loop on the events
		for( int i=0; i < rattles[e].size(); ++i ){
			printf( "%d %f %f %f\n", rattles[e][i].own_pdg,
			                      rattles[e][i].poe_x,
			                      rattles[e][i].poe_y,
			                      rattles[e][i].poe_z );
		}
		printf( "\n" );
	}
}

//------------------------------------------------------------------------------------
//print the events to the standard output
//just the rattles
void cat_info( V_rmom &rattles ){
	BRE::rhit ratt;
	for( int e=0; e < rattles.size(); ++e ){ //loop on the events
		for( int i=0; i < rattles[e].size(); ++i ){
			ratt = BRE::d2r( rattles[e][i] );
			printf( "%f %f %f\n", ratt.poe_x,
			                      ratt.poe_y,
			                      ratt.poe_z );
		}
		printf( "\n" );
	}
}

//------------------------------------------------------------------------------------
//rattles with PDG
void cat_info_pdg( V_rmom &rattles ){
	BRE::rhit ratt;
	for( int e=0; e < rattles.size(); ++e ){ //loop on the events
		for( int i=0; i < rattles[e].size(); ++i ){
			ratt = BRE::d2r( rattles[e][i] );
			printf( "%d %f %f %f\n", ratt.own_pdg,
			                         ratt.poe_x,
			                         ratt.poe_y,
			                         ratt.poe_z );
		}
		printf( "\n" );
	}
}
