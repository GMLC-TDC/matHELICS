function varargout = helicsQuerySetTarget(varargin)
%  Update the target of a query.
% 
%	@param query The query object to change the target of.
%	@param target the name of the target to query
% 
% 
	[varargout{1:nargout}] = helicsMex('helicsQuerySetTarget', varargin{:});
end
