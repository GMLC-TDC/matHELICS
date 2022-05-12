%{
definition of the minimum time resolution
%}
function v = HELICS_TIME_EPSILON()
	persistent vInitialized;
	if isempty(vInitialized)
		vInitialized = 1e-09;
	end
	v = vInitialized;
end
