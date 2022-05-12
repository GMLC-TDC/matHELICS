function v = HELICS_INVALID_OPTION_INDEX()
	persistent vInitialized;
	if isempty(vInitialized)
		vInitialized = -101;
	end
	v = vInitialized;
end
