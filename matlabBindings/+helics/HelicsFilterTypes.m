%{
enumeration of the predefined filter types

Attributes:
	HELICS_FILTER_TYPE_CUSTOM: value:0	a custom filter type that executes a user defined callback
	HELICS_FILTER_TYPE_DELAY: value:1	a filter type that executes a fixed delay on a message
	HELICS_FILTER_TYPE_RANDOM_DELAY: value:2	a filter type that executes a random delay on the messages
	HELICS_FILTER_TYPE_RANDOM_DROP: value:3	a filter type that randomly drops messages
	HELICS_FILTER_TYPE_REROUTE: value:4	a filter type that reroutes a message to a different destination than originally specified
	HELICS_FILTER_TYPE_CLONE: value:5	a filter type that duplicates a message and sends the copy to a different destination
	HELICS_FILTER_TYPE_FIREWALL: value:6	a customizable filter type that can perform different actions on a message based on firewall like rules
%}
classdef HelicsFilterTypes
	properties (Constant)
		HELICS_FILTER_TYPE_CUSTOM = int32(0);
		HELICS_FILTER_TYPE_DELAY = int32(1);
		HELICS_FILTER_TYPE_RANDOM_DELAY = int32(2);
		HELICS_FILTER_TYPE_RANDOM_DROP = int32(3);
		HELICS_FILTER_TYPE_REROUTE = int32(4);
		HELICS_FILTER_TYPE_CLONE = int32(5);
		HELICS_FILTER_TYPE_FIREWALL = int32(6);
	end
end