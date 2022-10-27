function varargout = helicsFederateGetTimeProperty(varargin)
%  Get the current value of a time based property in a federate.
% 
%	@param fed The federate query.
%	@param timeProperty The property to query.
% 
	[varargout{1:nargout}] = helicsMex('helicsFederateGetTimeProperty', varargin{:});
end
