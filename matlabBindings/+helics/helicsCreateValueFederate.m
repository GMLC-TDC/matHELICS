function varargout = helicsCreateValueFederate(varargin)
% Create a value federate from a federate info object.
% 
% @details HelicsFederate objects can be used in all functions that take a HelicsFederate or HelicsFederate object as an argument.
% 
% @param fedName The name of the federate to create, can NULL or an empty string to use the default name from fedInfo or an assigned name.
% @param fedInfo The federate info object that contains details on the federate.
% 
% 
% 
% @return An opaque value federate object.

	[varargout{1:nargout}] = helicsMex('helicsCreateValueFederate', varargin{:});
end
