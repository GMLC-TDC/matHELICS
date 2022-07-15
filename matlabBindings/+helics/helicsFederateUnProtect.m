%{
	remove the protection of an existing federate

	@details this function allows a federate to be retrieved on demand, it must be explicitly close
 later otherwise it will be destroyed
	when the library is closed

	@param fed the name of an existing federate that should not be protected

	@param[in,out] err An error object that will contain an error code and string if the federate was not found.
%}
function varargout = helicsFederateUnProtect(varargin)
	[varargout{1:nargout}] = helicsMex('helicsFederateUnProtect', varargin{:});
end
