%{
	Create a value federate from a JSON file, JSON string, or TOML file.

	@details HelicsFederate objects can be used in all functions that take a HelicsFederate or HelicsFederate object as an argument.

	@param configFile A JSON file or a JSON string or TOML file that contains setup and configuration information.

	@param[in,out] err An error object that will contain an error code and string if any error occurred during the execution of the function.


	@return An opaque value federate object.
%}
function varargout = helicsCreateValueFederateFromConfig(varargin)
	[varargout{1:nargout}] = helicsMex('helicsCreateValueFederateFromConfig', varargin{:});
end
