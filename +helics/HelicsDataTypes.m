%{
enumeration of allowable data types for publications and inputs

Attributes:
	HELICS_DATA_TYPE_UNKNOWN: value:-1	
	HELICS_DATA_TYPE_STRING: value:0	a sequence of characters
	HELICS_DATA_TYPE_DOUBLE: value:1	a double precision floating point number
	HELICS_DATA_TYPE_INT: value:2	a 64 bit integer
	HELICS_DATA_TYPE_COMPLEX: value:3	a pair of doubles representing a complex number
	HELICS_DATA_TYPE_VECTOR: value:4	an array of doubles
	HELICS_DATA_TYPE_COMPLEX_VECTOR: value:5	a complex vector object
	HELICS_DATA_TYPE_NAMED_POINT: value:6	a named point consisting of a string and a double
	HELICS_DATA_TYPE_BOOLEAN: value:7	a boolean data type
	HELICS_DATA_TYPE_TIME: value:8	time data type
	HELICS_DATA_TYPE_RAW: value:25	raw data type
	HELICS_DATA_TYPE_JSON: value:30	type converts to a valid json string
	HELICS_DATA_TYPE_MULTI: value:33	the data type can change
	HELICS_DATA_TYPE_ANY: value:25262	open type that can be anything
%}
HELICS_DATA_TYPE_UNKNOWN = -1;
HELICS_DATA_TYPE_STRING = 0;
HELICS_DATA_TYPE_DOUBLE = 1;
HELICS_DATA_TYPE_INT = 2;
HELICS_DATA_TYPE_COMPLEX = 3;
HELICS_DATA_TYPE_VECTOR = 4;
HELICS_DATA_TYPE_COMPLEX_VECTOR = 5;
HELICS_DATA_TYPE_NAMED_POINT = 6;
HELICS_DATA_TYPE_BOOLEAN = 7;
HELICS_DATA_TYPE_TIME = 8;
HELICS_DATA_TYPE_RAW = 25;
HELICS_DATA_TYPE_JSON = 30;
HELICS_DATA_TYPE_MULTI = 33;
HELICS_DATA_TYPE_ANY = 25262;