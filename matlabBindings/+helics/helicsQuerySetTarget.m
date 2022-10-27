function varargout = helicsQuerySetTarget(varargin)
%  Update the target of a query.
% 
%	@param query The query object to change the target of.
%	@param target the name of the target to query
% 
% 
%	@param[in,out] err An error object that will contain an error code and string if any error occurred during the execution of the function.
	[varargout{1:nargout}] = helicsMex('helicsQuerySetTarget', varargin{:});
end
