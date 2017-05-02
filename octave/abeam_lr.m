%I run out of patience looking for this function builtin
%
% [a, b] = abeam_lr( x, y )
% y = a*x + b;

function [a, b] = abeam_lr( x, y )
	x = x(:);
	y = y(:);
	
	fcn = @(p) sum( (p(1)*x + p(2) -y).^2 );
	
	pees = fminsearch( fcn, [0 0] );
	
	a = pees(1);
	b = pees(2);
end
