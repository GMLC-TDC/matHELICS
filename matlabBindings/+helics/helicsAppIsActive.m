function varargout = helicsAppIsActive(varargin)
% Check if the HelicsApp is active and ready to run.
% @param app The app to check.
% @return True if the app is active, otherwise false.

	[varargout{1:nargout}] = helicsMex('helicsAppIsActive', varargin{:});
end
