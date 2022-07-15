function v = HELICS_DEPRECATED()
	persistent vInitialized;
	if isempty(vInitialized)
		vInitialized = helicsMex('HELICS_DEPRECATED');
	end
	v = vInitialized;
end
