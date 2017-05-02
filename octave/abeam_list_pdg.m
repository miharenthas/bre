%a tiny tool to list the PDG codes of a collection of data
%
% p_pdgs = abeam_list_pdg( full_beam )
%      full_beam: they are the full beams in a cell array
%                 one batch of data per cell.
%      p_pdgs: an array with all the pdgs found.

function p_pdgs = abeam_list_pdg( full_beam )
	n_entries = length( full_beam );	
	
	p_pdgs = [];
	for ii=1:n_entries
		p_pdgs = [p_pdgs; unique( full_beam{ii}(:,1) )(:)];
	end
	p_pdgs = unique( p_pdgs );
end
