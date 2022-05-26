%{
enumeration of the predefined translator types

Attributes:
	HELICS_TRANSLATOR_TYPE_CUSTOM: value:0	a custom filter type that executes a user defined callback
	HELICS_TRANSLATOR_TYPE_JSON: value:11	a translator type that converts to and from JSON
	HELICS_TRANSLATOR_TYPE_BINARY: value:12	a translator type that just encodes the message again in binary
%}
HELICS_TRANSLATOR_TYPE_CUSTOM = 0;
HELICS_TRANSLATOR_TYPE_JSON = 11;
HELICS_TRANSLATOR_TYPE_BINARY = 12;