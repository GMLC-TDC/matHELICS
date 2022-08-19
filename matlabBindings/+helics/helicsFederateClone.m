%{
%	Create a new reference to an existing federate.

%	@details This will create a new HelicsFederate object that references the existing federate. The new object must be freed as well.

%	@param fed An existing HelicsFederate.

%	@param[in,out] err An error object that will contain an error code and string if any error occurred during the execution of the function.


%	@return A new reference to the same federate.
%}
function varargout = helicsFederateClone(varargin)
	[varargout{1:nargout}] = helicsMex('helicsFederateClone', varargin{:});
end
