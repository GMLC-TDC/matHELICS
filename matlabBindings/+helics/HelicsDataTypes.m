%{
enumeration of allowable data types for publications and inputs

Attributes:
%	HELICS_DATA_TYPE_UNKNOWN: value:-1	
%	HELICS_DATA_TYPE_STRING: value:0	a sequence of characters
%	HELICS_DATA_TYPE_DOUBLE: value:1	a double precision floating point number
%	HELICS_DATA_TYPE_INT: value:2	a 64 bit integer
%	HELICS_DATA_TYPE_COMPLEX: value:3	a pair of doubles representing a complex number
%	HELICS_DATA_TYPE_VECTOR: value:4	an array of doubles
%	HELICS_DATA_TYPE_COMPLEX_VECTOR: value:5	a complex vector object
%	HELICS_DATA_TYPE_NAMED_POINT: value:6	a named point consisting of a string and a double
%	HELICS_DATA_TYPE_BOOLEAN: value:7	a boolean data type
%	HELICS_DATA_TYPE_TIME: value:8	time data type
%	HELICS_DATA_TYPE_CHAR: value:9	data type for a single character
%	HELICS_DATA_TYPE_RAW: value:25	raw data type
%	HELICS_DATA_TYPE_JSON: value:30	type converts to a valid json string
%	HELICS_DATA_TYPE_MULTI: value:33	the data type can change
%	HELICS_DATA_TYPE_ANY: value:25262	open type that can be anything
%}
classdef HelicsDataTypes
	properties (Constant)
		HELICS_DATA_TYPE_UNKNOWN = int32(-1);
		HELICS_DATA_TYPE_STRING = int32(0);
		HELICS_DATA_TYPE_DOUBLE = int32(1);
		HELICS_DATA_TYPE_INT = int32(2);
		HELICS_DATA_TYPE_COMPLEX = int32(3);
		HELICS_DATA_TYPE_VECTOR = int32(4);
		HELICS_DATA_TYPE_COMPLEX_VECTOR = int32(5);
		HELICS_DATA_TYPE_NAMED_POINT = int32(6);
		HELICS_DATA_TYPE_BOOLEAN = int32(7);
		HELICS_DATA_TYPE_TIME = int32(8);
		HELICS_DATA_TYPE_CHAR = int32(9);
		HELICS_DATA_TYPE_RAW = int32(25);
		HELICS_DATA_TYPE_JSON = int32(30);
		HELICS_DATA_TYPE_MULTI = int32(33);
		HELICS_DATA_TYPE_ANY = int32(25262);
	end
end