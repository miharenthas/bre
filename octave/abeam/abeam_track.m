%tiny script(ish) to get the tracking information out of a bunch of files.
%
% [ centroid_trk, tscountour_trk ] = abeam_track( data, det_placement, cutoff )
%
% parameters:
%     data: a cell array full of data (a cell per file)
%     det_placement: the detector placement, it's a matrix with a row for each
%                    file -- [rot_y, t_x, t_y, t_z].
%     cutoff: a parameter that gets passed to the abeam_corr function --optional
% output:
%     centroid_trk: the track of the centroid, cell
%     tscontour_trk: the track of the three-sigma contour, cell
%
%NOTE: this code is a derivative of abeam_do.

function [ centroid_trk, tscontour_trk ] = abeam_track( data, det_placement, cutoff )
	%do input checking
	%... (coming soon)
	if length( data ) == size( det_placement, 1 )
		n_pts = size( det_placement, 1 );
	else
		error( "Every data sample has to have a position sample." );
	end
	
	%single file processing (basically, the correction)
	data_c = {}; %corrected data
	trfs = {}; %transformations
	centroids = []; %the centroids
	tscontours = []; %the three sigma contours
	zees =  []; %the Z coordinates
	for ii = 1:n_pts
		%correct the data to get the detector coordinates
		%and most importantly apply a cut on random junk
		%far away from the beam (optionally).
		if nargin == 3
			[data_c(ii), trfs(ii)] = abeam_corr( data{ii}, det_placement(ii,1), ...
		                                 det_placement(ii,2:4), cutoff );
			while sum( size( data_c{ii} ) ) < 9 %less than three particles
				warning( ['No particles wiithin ', ...
				         num2str(cutoff), 'cm of centroid' ] );
				cutoff +=50;
				[data_c(ii), trfs(ii)] = abeam_corr( data{ii}, det_placement(ii,1), ...
				                         det_placement(ii,2:4), cutoff );
			end
		else
			[data_c(ii), trfs(ii)] = abeam_corr( data{ii}, det_placement(ii,1), ...
		                                 det_placement(ii,2:4) );
		        if sum( size( data_c{ii} ) ) < 9; error( "Empty particle set" ); end
		end
		
		%get the three sigma contours and centroid of the data
		[ctr, tsc] = abeam_mcov( data_c{ii} );
		
		%save the stuff
		centroids = [centroids; ctr];
		tscontours = [tscontours; tsc];
		zees(ii) = mean( data_c{ii} )(3);
	end
	
	%do the interpolation now
	%and also the angular stuff
	centroid_trk = {};
	tscontour_trk = {};
	for ii = 1:n_pts
		ctr = centroids(ii,:);
		tsc = tscontours(ii);
		[~, i_tsc] = abeam_plot( ctr, tsc, 0 ); %just get the points of the
		                                        %three sigma contour
		%append the Z coordinate (not interpolated)
		zz = zees(ii);
		i_tsc = [i_tsc, zz*ones( size( i_tsc, 1 ), 1 )];
		ctr = [ctr, zz];
		
		%retransform
		i_trf = inv( trfs{ii} )'; %find the transofrmation
		                          %and invert it
		%and then apply it (note that it's affine).
		i_tsc = [i_tsc, ones( size( i_tsc, 1 ), 1 )]*i_trf;
		i_tsc = i_tsc(:,1:3);
		ctr = [ctr, 1]*i_trf;
		ctr = ctr(:,1:3);
		
		%save the track's data points
		centroid_trk(ii) = ctr;
		tscontour_trk(ii) = i_tsc;
	end
	
	%and that's it.
end
		
		
		
	
