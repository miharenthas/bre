%a script(ish) to run abeam_do on the full beam collection
%
% [ang_centroid, ang_min, ang_max] = abeam_do_all( full_beam, energy, ...
%                                                  det_placement, mag_ctr )

function [ang_centroid, ang_min, ang_max] = abeam_do_all( full_beam, energy, ...
                                                          det_placement, mag_ctr )
	p_pdg = abeam_list_pdg( full_beam );
	
	ang_centroid = {};
	ang_min = {};
	ang_max = {};
	n_pdgs = length( p_pdg );
	for pp=1:n_pdgs
		apdg = abeam_select_pdg( full_beam, p_pdg(pp) );
		if isempty( apdg ) continue; end;
		
		try
			[actr, amin, amax] = abeam_do( apdg, energy, det_placement, mag_ctr );
		catch
			continue;
		end
		ang_centroid(pp) = actr;
		ang_min(pp) = amin;
		ang_max(pp) = amax;
		
		%now, do plotting: XZ
		f1 = figure( 'position', [100, 100, 1600, 1200] );
		e_span = [energy(1):energy(end)];
		hold on;
		plot( e_span, rad2deg( actr(:,1) ), 'linewidth', 3 );
		plot( e_span, rad2deg( amin(:,1) ), 'r', 'linewidth', 2 );
		plot( e_span, rad2deg( amax(:,1) ), 'r', 'linewidth', 2 );
		hold off;
		
		set( gca, 'linewidth', 2, 'fontsize', 24 );
		ylabel( 'Angle (DEG)' );
		xlabel( 'Beam energy (AMeV)' );
		title( ['Bending (on XZ plane) for PDG ', num2str( p_pdg(pp) )] );
		legend( { 'Centroid'; 'Three-sigma boundary'} );
		grid on;
		
		%YZ
		f2 = figure( 'position', [100, 100, 1600, 1200] );
		e_span = [energy(1):energy(end)];
		hold on;
		plot( e_span, rad2deg( actr(:,2) ), 'linewidth', 3 );
		plot( e_span, rad2deg( amin(:,2) ), 'r', 'linewidth', 2 );
		plot( e_span, rad2deg( amax(:,2) ), 'r', 'linewidth', 2 );
		hold off;
		
		set( gca, 'linewidth', 2, 'fontsize', 24 );
		ylabel( 'Angle (DEG)' );
		xlabel( 'Beam energy (AMeV)' );
		title( ['Bending (on YZ plane) for PDG ', num2str( p_pdg(pp) )] );
		legend( { 'Centroid'; 'Three-sigma boundary'} );
		grid on;
		
		while true
			user_says = input( 'save?(y/n) ', 's' );
			switch( user_says )
				case 'y'
					figure( f1 );
					hgsave( ['XZ_PDG_', num2str( p_pdg(pp) )] );
					figure( f2 );
					hgsave( ['YZ_PDG_', num2str( p_pdg(pp) )] );
					break;
				case 'n'
					break;
				otherwise
					disp( [user_says, ' is not a good answer.'] );
			end
		end
				
		close( f1 );
		close( f2 );
	end
end
		
