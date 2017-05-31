%this function returns an interpoated "slice" at some
%Z coordinate of the centroid
%
% [i_ctr, i_tsc] = abeam_track_interp( ctr_trk, tsc_tube, Z_ctr )
%
% parameters:
%     ctr_trk: the centroids arranged in a cell array
%     tsc_tube: the three-sigma contours, as they come out from _track_tube!!!
%     Z_ctr: the Z coordinate of the target centroid.
% returns:
%     i_ctr: the interpolated centroid
%     i_tsc: the interpolated three-sigma contour at the target centroid.
%
%NOTE: For now, I will interpolate also the rotation on Y of the three sigma contours
%      this is not necessarily the best approach and may change.

function [i_ctr, i_tsc] = abeam_track_interp( ctr_trk, tsc_tube, Z_ctr )
	
	if ~iscell( ctr_trk ) || ~iscell( tsc_tube )
		error( 'expecting 2 cells (and one scalar).' );
	end
	
	%first of all, I want to convert Z_ctr into an "index" (which will
	%be fractionary, because we didn't sample there).
	n_slices = numel( ctr_trk );
	ctr = reshape( [ctr_trk{:}], [3, n_slices] )';
	
	if Z_ctr > max( ctr(:,3) )
		error( 'This method works only for points between samples.' );
	end
	
	bogoidx = interp1( ctr(:,3), [1:n_slices], Z_ctr, 'spline' );
	
	%now that we have the bogus index, let's interpolate also the data.
	i_ctr = interp1( ctr, bogoidx, 'spline' ); %the centroid
	
	%and the contour, if requested (it's slow)
	if isargout( 2 )
		tsc_npts = numel( tsc_tube ); %this means: I support downsampling.
		                              %of the particle tube.
		i_tsc = zeros( tsc_npts, 3 );
		for ii=1:tsc_npts
			i_tsc(ii, :) = interp1( tsc_tube{ii}, bogoidx, 'spline' );
		end
	end
end
