%this function reorganizes the three-sigma contours from slices
%to lines -- building an effective tube.
%
% tsc_tube = abeam_track_tube( tsc_trk )
%
%parameters:
% tsc_trk: the "slices" of three sigma contours. Cell array
%          one cell per slice.
%returns: a 256-cell array of tubing delimitaton.
%
%NOTE: no need to finger with the centroids, they are already well
%      organized

function tsc_tube = abeam_track_tube( tsc_trk )
	if ~iscell( tsc_trk )
		error( "Not a cell, expecting a cell array." );
	end

	n_slices = numel( tsc_trk ); %nubber of slices (detector datapoints in the track)
	tsc_tube = cell( 256, 1 ); %preallocation! rather pointless...
	for ii=1:256
		%this makes rehape work:
		% [tsc_trk{:}] is a columnwise concatenation of the contours
		%              [ contour_1, contour_2, ..., contour_n ]
		% the points in the contour are saved in row-majow
		% each row is then a point.
		% [tsc_trk{:}](ii,:) is then the ii-th point on all contours
		% saved as [x, y, z, x, y, z, ...]
		% reshaping it in 3 rows and n_slices columns puts it in the
		% shape of [ [x;y;z], [x;y;z], ... ]
		% the transposition puts it in the desireable format.
		tsc_tube{ii} = reshape( [tsc_trk{:}](ii,:), [3, n_slices] )';
	end
end
