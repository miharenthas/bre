%a tiny tool to select on pdg from a collection of mixed data
%
% fb_pdg = abeam_select_pdg( full_beam, target_pdg )
%
% fb_pdg and full_beam are cell arrays containing a 4-by-many matrix,
% representing the data.

function fb_pdg = abeam_select_pdg( full_beam, target_pdg )
	n_entries = length( full_beam );
	
	fb_pdg = cell( n_entries, 1 ); %preallocation!
	for ii=1:n_entries
		fb_pdg(ii) = full_beam{ii}( find( full_beam{ii}(:,1) == target_pdg ), 2:4 );
	end
end
