%This function exports a track cone as a script for BricsCAD.
%
% abeam_track_export_CAD_script( tsc_trk, ctr_trk )
%
%parameters:
%     file_name: come on, it's obvious.
%     tsc_trk: the "sliced" three sigma contours (cell)
%     ctr_trk: the centroid track (cell)
%     contour_dws: how much downsampling is needed (shall be a power of 2)
%     c_trf: since the CAD program is sane and ROOT isn't,
%            a coordinate transformation might be useful.
%            USUALLY: | 0   0   10 |
%                     | 10  0   0  |
%                     | 0   10  0  |
%            which means cm --> mm and x->y, z->x, y->z ROOT->CAD

function abeam_track_export_CAD_script( file_name, tsc_trk, ctr_trk, contour_dws, c_trf )
	%input checks
	if nargin <= 4 c_trf = [0 0 10; 10 0 0; 0 10 0]; end
	if nargin <= 3 contour_dws = 1; end
	if ~ischar( file_name )
		error( 'The file name must be a string.' );
	end
	%TODO: more and better.
	
	tsc_tb = abeam_track_tube( tsc_trk );
	
	if exist( file_name )
		error( ['File "',file_name,'" exists.'] );
	end
	sfile = fopen( file_name, 'w' );
	
	%draw the longitudinal lines
	fprintf( sfile, '_none\n' ); %make sure we aren't snapping to random junk
	for ii = 1:contour_dws:numel( tsc_tb )
		fprintf( sfile, '_line\n' ); %use single segments
		pts = tsc_tb{ii}*c_trf'; %transform everything in one go
		for jj = 1:size( pts, 1 )
			fprintf( sfile, "%f,%f,%f\n", pts(jj,1), pts(jj,2), pts(jj,3) );
		end
		fprintf( sfile, '\n' ); %close the command
	end
	
	%draw the three sigma contours where they have been sampled/interpolated
	for ii = 1:numel( tsc_trk )
		fprintf( sfile, '_line\n' );
		pts = tsc_trk{ii}*c_trf'; %transform everything in one go
		for jj = 1:contour_dws:size( pts, 1 )
			fprintf( sfile, "%f,%f,%f\n", pts(jj,1), pts(jj,2), pts(jj,3) );
		end
		fprintf( sfile, "%f,%f,%f\n", pts(1,1), pts(1,2), pts(1,3) ); %close the contour
		fprintf( sfile, '\n' ); %close the command
	end
	
	%draw the centroid line
	pts = c_trf*reshape( [ctr_trk{:}], [ 3, numel( ctr_trk ) ] );
	pts = pts';
	fprintf( sfile, '_3dpoly\n' );
	for jj = 1:size( pts, 1 )
		fprintf( sfile, "%f,%f,%f\n", pts(jj,1), pts(jj,2), pts(jj,3) );
	end
	fprintf( sfile, '\n' );
	fprintf( sfile, '_stopscript\n' );
	
	%TODO: some fancy editing may find its place here. For now, that's it.
	fflush( sfile );
	fclose( sfile );
end
		
		
			
