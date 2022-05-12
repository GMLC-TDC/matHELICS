function v = HELICS_DATA_TYPE_CHAR()
	persistent vInitialized;
	if isempty(vInitialized)
		vInitialized = helicsMex("HELICS_DATA_TYPE_CHAR");
	end
	v = vInitialized;
end
