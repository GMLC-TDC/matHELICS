function v = HELICS_INVALID_OPTION_INDEX()
	persistent vInitialized;
	if isempty(vInitialized)
		vInitialized = int32(-101);
	end
	v = vInitialized;
end
