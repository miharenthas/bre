%tiny utility to guess linearly the beam's centroid and 3sigma contour
%with respect to the energy
%
% [ i_centroid, i_tsc ] = abeam_linear_guess( given_nrg, given_ctr, given_tsc, target_nrg )

function [i_centroid, i_tsc] = abeam_linear_guess( given_nrg, given_ctr, given_tsc, target_nrg )
	i_centroid = zeros( 1, 2 );

	[b, r] = abeam_lr( given_nrg, given_ctr(:,1) );
	i_centroid(1) = b*target_nrg + r;
	[b, r] = abeam_lr( given_nrg, given_ctr(:,2) );
	i_centroid(2) = b*target_nrg + r;
	
	[b, r] = abeam_lr( given_nrg, [given_tsc(:).a] );
	i_tsc.a = b*target_nrg + r ;
	[b, r] = abeam_lr( given_nrg, [given_tsc(:).b] );
	i_tsc.b = b*target_nrg + r;
	[b, r] = abeam_lr( given_nrg, [given_tsc(:).rot] );
	i_tsc.rot = b*target_nrg + r;
	i_tsc.centroid = i_centroid;
end
