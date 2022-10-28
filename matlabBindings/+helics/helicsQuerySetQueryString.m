function varargout = helicsQuerySetQueryString(varargin)
%  Update the queryString of a query.
% 
%	@param query The query object to change the target of.
%	@param queryString the new queryString
% 
	[varargout{1:nargout}] = helicsMex('helicsQuerySetQueryString', varargin{:});
end
