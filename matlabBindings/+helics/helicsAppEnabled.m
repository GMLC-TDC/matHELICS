function varargout = helicsAppEnabled(varargin)
% HELICS_TRUE if the app library is enabled

	[varargout{1:nargout}] = helicsMex('helicsAppEnabled', varargin{:});
end
