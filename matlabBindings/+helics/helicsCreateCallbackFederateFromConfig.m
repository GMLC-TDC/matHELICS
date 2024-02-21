function varargout = helicsCreateCallbackFederateFromConfig(varargin)
% Create a callback federate from a JSON file or JSON string or TOML file.
% 
% @details Callback federates are combination federates that run a series of callbacks for execution in a completely automated fashion.
% The enterInitializingMode call transfers complete control of the federate to the Core and no further user interaction is expected.
% 
% @param configFile A JSON file or a JSON string or TOML file that contains setup and configuration information.
% 
% 
% 
% @return An opaque combination federate object.

	[varargout{1:nargout}] = helicsMex('helicsCreateCallbackFederateFromConfig', varargin{:});
end
