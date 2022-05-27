%{
indicator used for a false response
%}
function v = HELICS_FALSE()
	persistent vInitialized;
	if isempty(vInitialized)
		vInitialized = 0;
	end
	v = vInitialized;
end
