% enumeration of sequencing modes for queries and commands fast is the default, meaning the query travels along priority channels and takes precedence of over existing messages; ordered means it follows normal priority patterns and will be ordered along with existing messages
%
% Attributes:
%	HELICS_SEQUENCING_MODE_FAST: value:0	sequencing mode to operate on priority channels
%	HELICS_SEQUENCING_MODE_ORDERED: value:1	sequencing mode to operate on the normal channels
%	HELICS_SEQUENCING_MODE_DEFAULT: value:2	select the default channel
classdef HelicsSequencingModes
	properties (Constant)
		HELICS_SEQUENCING_MODE_FAST = int32(0);
		HELICS_SEQUENCING_MODE_ORDERED = int32(1);
		HELICS_SEQUENCING_MODE_DEFAULT = int32(2);
	end
end