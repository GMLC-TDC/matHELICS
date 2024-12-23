function varargout = helicsAppDestroy(varargin)
% Disconnect and free a HelicsApp.
% @param app The app to destroy.

	[varargout{1:nargout}] = helicsMex('helicsAppDestroy', varargin{:});
end
