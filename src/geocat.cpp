//a program to print to the standard output or a file the geometry information
//contained into a simulation

#include <getopt.h>
#include <stdio.h>
#include <string.h>

#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>

#include "bre_reader.h"
#include "bre_data_types.h"
#include "bre_geo_basic.h"

#include "TFile.h"
#include "TTree.h"
#include "TKey.h"

//the usual utility to get the first tree in the file
TTree *get_ttree( TFile &file );

int main( int argc, char **argv ){
	//input variables
	bool pdg = false;
	bool only_pdg = false;
	int target_pdg;
	bool time = false;
	bool export_script = false;
	char in_fnames[64][254]; int in_fcount = 0;
	char out_fname[256]; bool to_stdout = true;
	gsl_matrix *trf = gsl_matrix_calloc( 3, 3 ); bool transform = false;
	gsl_vector *trl = gsl_vector_calloc( 3 ); bool translate = false;
	unsigned int max_tracks = -1;
	bool count_evt = false, count_trk = false;
	char tree_name[32]; strcpy( tree_name, "evt" );
	
	//optionZ
	struct option opts[] = {
		{ "read-from", required_argument, NULL, 'i' },
		{ "write-to", required_argument, NULL, 'o' },
		{ "pdg", no_argument, NULL, 'p' },
		{ "only-pdg", required_argument, NULL, 'P' }, 
		{ "time", no_argument, NULL, 't' },
		{ "export-script", no_argument, NULL, 's' },
		{ "transform", required_argument, NULL, 'T' },
		{ "translate", required_argument, NULL, 'l' },
		{ "nb-tracks", required_argument, NULL, 'n' },
		{ "count-events", no_argument, NULL, 'C' },
		{ "count-tracks", no_argument, NULL, 'c' },
		{ NULL, 0, NULL, 0 }
	};
	
	//parse the un --'ed strings, that are file names..
	for( int i=1; i < argc; ++i ){
		if( argv[i][0] == '-' ) break;
		if( strlen( argv[i] ) < 256 ){
			strcpy( in_fnames[in_fcount], argv[i] );
			++in_fcount;
		} else continue;
	}
	
	char iota=0; int opix;
	while( ( iota = getopt_long( argc, argv, "i:o:pP:tsT:l:n:Cc", opts, &opix ) ) != -1 ){
		switch( iota ){
			case 'i':
				in_fcount = 1;
				if( strlen( optarg ) < 256 )
					strcpy( in_fnames[0], optarg );
				else{
					fprintf( stderr, "%s: error: file name too long.\n", argv[0] );
					exit( 1 );
				}
				break;
			case 'o':
				to_stdout = false;
				if( strlen( optarg ) < 256 )
					strcpy( out_fname, optarg );
				else{
					fprintf( stderr, "%s: error: file name too long.\n", argv[0] );
					exit( 2 );
				}
				break;
			case 'p':
				pdg = true;
				break;
			case 'P':
				only_pdg = true;
				target_pdg = atoi( optarg );
				break;
			case 't':
				time = true;
				break;
			case 's':
				export_script = true;
				break;
			case 'T':
				transform = true;
				sscanf( optarg, "[%f,%f,%f;%f,%f,%f;%f,%f,%f]",
					trf->data[0], trf->data[1], trf->data[2],
					trf->data[3], trf->data[4], trf->data[5],
					trf->data[6], trf->data[7], trf->data[8] );
				break;
			case 'l':
				translate = true;
				sscanf( optarg, "[%f,%f,%f]", trl->data[0],
				        trl->data[1], trl->data[2] );
				break;
			case 'n':
				max_tracks = (unsigned int)atoi( optarg );
				break;
			case 'C':
				count_evt = true;
				break;
			case 'c':
				count_trk = true;
				break;
			default:
				fprintf( stderr, "Unknown option, I'll die now.\n" );
				exit( 3 );
				break;
		}
	}
	
	if( in_fcount == 0 ){
		fprintf( stderr, "%s: error: no input.\n", argv[0] );
		exit( 4 );
	}
	
	//frist thing frist, open the BURP file and load it
	TFile *infile; TTree *the_tree;
	BRE::bgeo_vec geo_data;
	BRE::bgeo_vec geo_buf;
	for( int f=0; f < in_fcount; ++f ){
		infile = TFile::Open( in_fnames[f] );
		if( infile->IsZombie() ){
			fprintf( stderr, "%s: error: can't open input file \"%s\".\n",
			         argv[0], in_fnames[f] );
			exit( 500 + f );
		}
		
		//get the first tree out of the file
		the_tree = get_ttree( *infile );
		
		//get the data out of the ROOT junk
		BRE::read_branch( geo_buf, the_tree );
		
		//cat the vectors
		geo_data.insert( geo_data.end(), geo_buf.begin(), geo_buf.end() );
		
		//cleanup (hopefully)
		infile->Close();
		infile->Delete();
	}
	//a little cleanup
	geo_buf.clear();
	
	//------------------------------------
	//now let's do stuff!
	
	//first, siv on PDG
	if( only_pdg ) BRE::bgeo_select_PDG( geo_data, target_pdg );
	
	//if I want to count, I have to do it here.
	if( count_evt ){
		//assume I want it on the standard output
		printf( "%d\n", geo_data.size() );
		return 0;
	}
	
	unsigned int trk_cnt = 0;
	if( count_trk ){
		for( int evt=0; evt < geo_data.size(); ++evt )
			trk_cnt += geo_data[evt].size();
		printf( "%u\n", trk_cnt );
		return 0;
	}
	
	//then, apply transformations and translations
	//NOTE: 1) transformation 2) translation!!!
	if( transform ) BRE::bgeo_transform3( geo_data, trf );
	if( translate ) BRE::bgeo_translate3( geo_data, trl );
	gsl_matrix_free( trf );
	gsl_vector_free( trl );
	
	//and now, output!
	FILE *target;
	if( to_stdout ) target = stdout;
	else{
		target = fopen( out_fname, "w" );
		if( !target ){
			fprintf( stderr, "%s: error: couldn't open the output file.\n" );
			exit( 7 );
		}
	}
	
	if( export_script ) BRE::bgeo_export_to_CADscript( target, geo_data, max_tracks );
	else BRE::bgeo_fprintf( target, geo_data, pdg, time, max_tracks );

	//happy thoughts!	
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
