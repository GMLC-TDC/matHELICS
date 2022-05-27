%{
	Get the number of publications in a federate.

	@return (-1) if fed was not a valid federate otherwise returns the number of publications.
%}
function varargout = helicsFederateGetPublicationCount(varargin)
	[varargout{1:nargout}] = helicsMex('helicsFederateGetPublicationCount', varargin{:});
end
