%{
%	Create a value federate from a federate info object.

%	@details HelicsFederate objects can be used in all functions that take a HelicsFederate or HelicsFederate object as an argument.

%	@param fedName The name of the federate to create, can NULL or an empty string to use the default name from fi or an assigned name.
%	@param fi The federate info object that contains details on the federate.

%	@param[in,out] err An error object that will contain an error code and string if any error occurred during the execution of the function.


%	@return An opaque value federate object.
%}
function varargout = helicsCreateValueFederate(varargin)
	[varargout{1:nargout}] = helicsMex('helicsCreateValueFederate', varargin{:});
end
