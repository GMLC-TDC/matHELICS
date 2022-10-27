function varargout = helicsFederateGetName(varargin)
%  Get the name of the federate.
% 
%	@param fed The federate object to query.
% 
%	@return A pointer to a string with the name.
	[varargout{1:nargout}] = helicsMex('helicsFederateGetName', varargin{:});
end
