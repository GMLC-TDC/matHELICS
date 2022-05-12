%{
definition of time signifying the federate has terminated or run until the end of the simulation
%}
function v = HELICS_TIME_MAXTIME()
	persistent vInitialized;
	if isempty(vInitialized)
		vInitialized = HELICS_BIG_NUMBER();
	end
	v = vInitialized;
end
