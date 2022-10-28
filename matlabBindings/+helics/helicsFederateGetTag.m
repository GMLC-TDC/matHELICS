function varargout = helicsFederateGetTag(varargin)
%  Get a federate tag value.
% 
%	@param fed The federate to get the tag for.
%	@param tagName The name of the tag to query.
% 
	[varargout{1:nargout}] = helicsMex('helicsFederateGetTag', varargin{:});
end
