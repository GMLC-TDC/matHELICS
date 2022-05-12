%{
	Create a combination federate from a JSON file or JSON string or TOML file.

	@details Combination federates are both value federates and message federates, objects can be used in all functions
	         that take a HelicsFederate, helics_message_federate or HelicsFederate object as an argument

	@param configFile A JSON file or a JSON string or TOML file that contains setup and configuration information.

	@param[in,out] err An error object that will contain an error code and string if any error occurred during the execution of the function.


	@return An opaque combination federate object.
%}
function varargout = helicsCreateCombinationFederateFromConfig(varargin)
	[varargout{1:nargout}] = helicsMex("helicsCreateCombinationFederateFromConfig", varargin{:});
end
