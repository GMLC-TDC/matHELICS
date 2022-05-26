%{
enumeration of additional core flags

Attributes:
	HELICS_FLAG_DELAY_INIT_ENTRY: value:45	used to delay a core from entering initialization mode even if it would otherwise be ready
	HELICS_FLAG_ENABLE_INIT_ENTRY: value:47	used to clear the HELICS_DELAY_INIT_ENTRY flag in cores
	HELICS_FLAG_IGNORE: value:999	ignored flag used to test some code paths
%}
HELICS_FLAG_DELAY_INIT_ENTRY = 45;
HELICS_FLAG_ENABLE_INIT_ENTRY = 47;
HELICS_FLAG_IGNORE = 999;