%this utility calculates the ang of a given set of points
%with respect of another one (the pole) and the Z axis (beam)
%
% ang = abeam_angular( pole, points )

function ang = abeam_angular( pole, points )
	#calculate the lenghts on the axis
	on_x = points(:,1) - pole(1);
	on_y = points(:,2) - pole(2);

	#calculate the hypothenuse length
	on_x = on_x./sqrt( sum( (points(:,1) - pole(1)).^2 + (points(:,3) - pole(3)).^2, 2 ) );
	on_y = on_y./sqrt( sum( (points(:,2) - pole(2)).^2 + (points(:,3) - pole(3)).^2, 2 ) );
	
	%calc the angle(s)
	ang = asin( on_x );
	ang = [ang, asin( on_y )];
end
