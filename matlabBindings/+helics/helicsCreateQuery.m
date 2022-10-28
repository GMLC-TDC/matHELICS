function varargout = helicsCreateQuery(varargin)
%  Create a query object.
% 
%	@details A query object consists of a target and query string.
% 
%	@param target The name of the target to query.
%	@param query The query to make of the target.
	[varargout{1:nargout}] = helicsMex('helicsCreateQuery', varargin{:});
end
