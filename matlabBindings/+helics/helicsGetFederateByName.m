%{
	Get an existing federate object from a core by name.

	@details The federate must have been created by one of the other functions and at least one of the objects referencing the created
	         federate must still be active in the process.

	@param fedName The name of the federate to retrieve.

	@param[in,out] err An error object that will contain an error code and string if any error occurred during the execution of the function.


	@return NULL if no fed is available by that name otherwise a HelicsFederate with that name.
%}
function varargout = helicsGetFederateByName(varargin)
	[varargout{1:nargout}] = helicsMex('helicsGetFederateByName', varargin{:});
end
