%{
	Create a federate info object for specifying federate information when constructing a federate.

	@return A HelicsFederateInfo object which is a reference to the created object.
%}
function varargout = helicsCreateFederateInfo(varargin)
	[varargout{1:nargout}] = helicsMex('helicsCreateFederateInfo', varargin{:});
end
