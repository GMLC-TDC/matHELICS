function varargout = helicsBrokerIsValid(varargin)
%  Check if a broker object is a valid object.
% 
%	@param broker The HelicsBroker object to test.
	[varargout{1:nargout}] = helicsMex('helicsBrokerIsValid', varargin{:});
end
