%{
result returned for requesting the value of an invalid/unknown property
%}
function v = HELICS_INVALID_PROPERTY_VALUE()
	persistent vInitialized;
	if isempty(vInitialized)
		vInitialized = int32(-972);
	end
	v = vInitialized;
end
