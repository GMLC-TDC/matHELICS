function varargout = helicsCreateMessageFederateFromConfig(varargin)
% Create a message federate from a JSON file or JSON string or TOML file.
% 
% @details helics_message_federate objects can be used in all functions that take a helics_message_federate or HelicsFederate object as an
% argument.
% 
% @param configFile A Config(JSON,TOML) file or a JSON string that contains setup and configuration information.
% 
% 
% 
% @return An opaque message federate object.

	[varargout{1:nargout}] = helicsMex('helicsCreateMessageFederateFromConfig', varargin{:});
end
