%this tiny function returns the centroid and the 3sigma contour of the data
%assuming they distribute according to a 2D gaussian.
%
%NOTE: the multiplier for 3sigma has been found on
%      https://people.richland.edu/james/lecture/m170/tbl-chi.html
%      Better ones are welcome.
%
% [centroid, tsc] = abeam_mcov( ub_corr )

function [centroid, tsc] = abeam_mcov( ub_corr )
	%assuming a normal distribution, mean and covrinace should be as good as a fit
	%if not better.
	centroid = mean( ub_corr(:,[1 2]) );
	c_m = cov( ub_corr(:,[1 2]) );
	
	%now, the contour
	sigma_mult = sqrt( 10.597 );
	[e_vec, e_val] = eig( c_m ); %eigenvalues and vectors
	tsc.a = sigma_mult*sqrt( e_val(1,1) ); %ellipse's A (for 3sigma)
	tsc.b = sigma_mult*sqrt( e_val(2,2) ); %ellipse's B (for 3sigma)
	tsc.rot = atan2( e_vec(2,1), e_vec(1,1) ); %ellipse's rotation
	tsc.centroid = centroid;
end
