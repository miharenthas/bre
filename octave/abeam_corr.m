%a tiny tool to change the coordinate system from global to aligned to rattleplane
%
% [ub_corr, trf] = abeam_corr( ub_orig, ang, transl )
% [ub_corr, trf] = abeam_corr( ub_orig, ang, transl, cutoff )
% -- ub_orig: the freshy loade XYZ data from the rattleplane
% -- ang: the rotation of the rattleplane (along Y axis... other axis are insane).
%         angle is in DEGREES, because so it's passed to ROOT.
% -- transl: the translation of the rattleplane ( [X Y Z] )
% -- cutoff: optionally, the radius of the cut circle can be specified

function [ub_corr, trf] = abeam_corr( ub_orig, ang, transl, cutoff )
	%build the affine transformation
	ang = deg2rad( ang );
	rot = [ cos( ang ), 0, sin( ang ), 0; ...
	       0, 1, 0, 0; ...
	       -sin( ang ), 0, cos( ang ), 0; ...
	       0, 0, 0, 1 ]; %rotation matrix
	trf = eye( 4 );
	trf(1:3,4) = transl(:);
	trf = trf*rot;
	trf = inv( trf ); %invert it
	
	%correct in the affine space
	ub_corr = [ub_orig, ones( size( ub_orig, 1 ), 1 )]*trf';
	ub_corr = ub_corr(:,1:3);

	%a little cleanup is in order
	%to eliminate stray events
	%select all the events in a 50 cm circle within the centroid
	if nargin == 4
		cutoff = 50;
		centroid = mean( ub_corr(:,[1 2]) );
		ub_corr = ub_corr( find( sum( (ub_corr(:,[1 2] ) - centroid).^2, 2 ) <= cutoff^2 ), : );
	end
end
