%{
definition of time zero-the beginning of simulation
%}
function v = HELICS_TIME_ZERO()
	persistent vInitialized;
	if isempty(vInitialized)
		vInitialized = 0.0;
	end
	v = vInitialized;
end
