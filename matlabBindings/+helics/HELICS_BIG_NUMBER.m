function v = HELICS_BIG_NUMBER()
	persistent vInitialized;
	if isempty(vInitialized)
		vInitialized = 9223372036.854774;
	end
	v = vInitialized;
end
