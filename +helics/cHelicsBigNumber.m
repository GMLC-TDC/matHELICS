function v = cHelicsBigNumber()
	persistent vInitialized;
	if isempty(vInitialized)
		vInitialized = HELICS_BIG_NUMBER();
	end
	v = vInitialized;
end
