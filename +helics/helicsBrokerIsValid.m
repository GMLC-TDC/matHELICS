%{
	Check if a broker object is a valid object.

	@param broker The HelicsBroker object to test.
%}
function varargout = helicsBrokerIsValid(varargin)
	[varargout{1:nargout}] = helicsMex("helicsBrokerIsValid", varargin{:});
end
