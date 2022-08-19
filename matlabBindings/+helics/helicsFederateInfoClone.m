%{
%	Create a federate info object from an existing one and clone the information.

%	@param fi A federateInfo object to duplicate.

%	@param[in,out] err An error object that will contain an error code and string if any error occurred during the execution of the function.


%	 @return A HelicsFederateInfo object which is a reference to the created object.
%}
function varargout = helicsFederateInfoClone(varargin)
	[varargout{1:nargout}] = helicsMex('helicsFederateInfoClone', varargin{:});
end
