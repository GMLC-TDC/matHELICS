%{
	Create a new reference to an existing core.

	@details This will create a new broker object that references the existing broker. The new broker object must be freed as well.

	@param core An existing HelicsCore.
	@param[in,out] err An error object that will contain an error code and string if any error occurred during the execution of the function.

	@return A new reference to the same broker.
%}
function varargout = helicsCoreClone(varargin)
	[varargout{1:nargout}] = helicsMex("helicsCoreClone", varargin{:});
end
