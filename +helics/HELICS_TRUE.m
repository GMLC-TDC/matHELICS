%{
indicator used for a true response
%}
function v = HELICS_TRUE()
	persistent vInitialized;
	if isempty(vInitialized)
		vInitialized = 1;
	end
	v = vInitialized;
end
