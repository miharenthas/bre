%a tiny utility to add the centroid and the 3sigma contour to a figure
%or generate a figure with those things in.
%
% [fig, xy] = abeam_plot( centroid, tsc, fig )
% --xy contains the 99.5% confidence level ellipse on the Z plane where
%   the original data is (the "corrected" one).

function [fig, xy] = abeam_plot( centroid, tsc, fig )
	if nargin == 2
		fig = figure( 'position', [100, 100, 1600, 1200] );
	end
	
	t = linspace( 0, 2*pi, 256 );
	x = tsc.a*cos( t );
	y = tsc.b*sin( t );
	
	rot = [ cos( tsc.rot ), -sin( tsc.rot ); sin( tsc.rot ), cos( tsc.rot ) ];
	xy = [x(:),y(:)]*rot';
	xy(:,1) += centroid(1);
	xy(:,2) += centroid(2);
	
	if fig ~= 0
		hold on;
		plot( xy(:,1), xy(:,2), 'linewidth', 3, 'b' );
		plot( centroid(1), centroid(2), 'rx', 'linewidth', 5 );
		hold off;
	end
end
