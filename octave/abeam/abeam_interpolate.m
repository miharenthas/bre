%tiny utility to interpolate the beam's centroid and 3sigma contour
%with respect to the energy
%
%NOTE: can't be used outside the existing energy interval. Use _linear_guess instead.
%
% [ i_centroid, i_tsc ] = abeam_interpolate( given_nrg, given_ctr, given_tsc, target_nrg )

function [i_centroid, i_tsc] = abeam_interpolate( given_nrg, given_ctr, given_tsc, target_nrg )
	i_centroid = zeros( 1, 2 );

	i_centroid(1) = interp1( given_nrg, given_ctr(:,1), target_nrg, 'spline' );
	i_centroid(2) = interp1( given_nrg, given_ctr(:,2), target_nrg, 'spline' );
	
	i_tsc.a = interp1( given_nrg, [given_tsc(:).a], target_nrg, 'spline' );
	i_tsc.b = interp1( given_nrg, [given_tsc(:).b], target_nrg, 'spline' );
	i_tsc.rot = interp1( given_nrg, [given_tsc(:).rot], target_nrg, 'spline' );
	i_tsc.centroid = i_centroid;
end	
	
