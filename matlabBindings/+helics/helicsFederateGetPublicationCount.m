function varargout = helicsFederateGetPublicationCount(varargin)
 Get the number of publications in a federate.

%	@return (-1) if fed was not a valid federate otherwise returns the number of publications.
	[varargout{1:nargout}] = helicsMex('helicsFederateGetPublicationCount', varargin{:});
end
