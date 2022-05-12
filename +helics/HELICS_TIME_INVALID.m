%{
definition of an invalid time that has no meaning
%}
function v = HELICS_TIME_INVALID()
	persistent vInitialized;
	if isempty(vInitialized)
		vInitialized = -1.785e+39;
	end
	v = vInitialized;
end
