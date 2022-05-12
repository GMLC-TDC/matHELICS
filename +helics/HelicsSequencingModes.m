classdef HelicsSequencingModes < uint64
	%{
	enumeration of sequencing modes for queries and commands fast is the default, meaning the query travels along priority channels and takes precedence of over existing messages; ordered means it follows normal priority patterns and will be ordered along with existing messages

	Attributes:
		HELICS_SEQUENCING_MODE_FAST: value:0	sequencing mode to operate on priority channels
		HELICS_SEQUENCING_MODE_ORDERED: value:1	sequencing mode to operate on the normal channels
		HELICS_SEQUENCING_MODE_DEFAULT: value:2	select the default channel
	%}
	enumeration
		HELICS_SEQUENCING_MODE_FAST (0)
		HELICS_SEQUENCING_MODE_ORDERED (1)
		HELICS_SEQUENCING_MODE_DEFAULT (2)
	end
end