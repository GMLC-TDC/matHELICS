function varargout = helicsAppFree(varargin)
% Free the HelicsApp object.
% @param app The app to free.

	[varargout{1:nargout}] = helicsMex('helicsAppFree', varargin{:});
end
