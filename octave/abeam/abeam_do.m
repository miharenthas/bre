%tiny script(ish) to get the angular information out of a file
%or a bunch of files
%
% [ ang_centroid, ang_min, ang_max ] = abeam_do( data, energy,
%                                                det_placement, mag_ctr, cutoff )
%
% parameters:
%     data: a cell array full of data (a cell per file)
%     energy: the energy each file has been generated at
%     det_placement: the detector placement, it's a matrix with an entry for each
%                    file.
%     mag_ctr: the coodrdinates of the magnet's centre
% output:
%     ang_centroid: the centroid of the beams (angle from the centre of the magnet)
%                   a row for every energy (interpolated at 1AMeV steps)
%     ang_min: the minimum bending angle of each beam energy (see above)
%     ang_max: the maximum benging angle of each beam energy (see above)

function [ang_centroid, ang_min, ang_max] = abeam_do( data, energy, ...
                                                      det_placement, mag_ctr, cutoff )
	%do input checking
	%... (coming soon)
	if length( data ) == length( energy )
		n_pts = length( energy );
	else
		error( "Every data point has to have an energy point." );
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
		if nargin == 5
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
		zees.( num2str( energy(ii) ) ) = mean( data_c{ii} )(3);
	end
	
	%do the interpolation now
	%and also the angular stuff
	ang_centroid = [];
	ang_max = [];
	ang_min = [];
	for ee = energy(1):energy(end)
		[ctr, tsc] = abeam_interpolate( energy, centroids, tscontours, ee );
		[~, i_tsc] = abeam_plot( ctr, tsc, 0 ); %just get the points of the
		                                        %three sigma contour
		%append the Z coordinate (not interpolated)
		zz = zees.( num2str( energy( find( energy <= ee )(end) ) ) );
		i_tsc = [i_tsc, zz*ones( size( i_tsc, 1 ), 1 )];
		ctr = [ctr, zz];
		
		%retransform
		i_trf = inv( trfs{find( energy <= ee )(end)} )'; %find the transofrmation
		                                                 %and invert it
		%and then apply it (note that it's affine).
		i_tsc = [i_tsc, ones( size( i_tsc, 1 ), 1 )]*i_trf;
		i_tsc = i_tsc(:,1:3);
		ctr = [ctr, 1]*i_trf;
		ctr = ctr(:,1:3);
		
		%find the angles
		ctr = abeam_angular( mag_ctr, ctr );
		i_tsc = abeam_angular( mag_ctr, i_tsc );
		
		%save the angles
		ang_centroid = [ang_centroid; ctr];
		ang_max = [ang_max; max( i_tsc )];
		ang_min = [ang_min; min( i_tsc )];
	end
	
	%and that's it
end
		
		
		
	
