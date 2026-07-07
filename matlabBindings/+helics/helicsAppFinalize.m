function varargout = helicsAppFinalize(varargin)
% Finalize the HelicsApp.
% @param app The app to finalize.
% @param[in,out] err An error object that will contain an error code and string if any error occurred during the execution of the function.

	[varargout{1:nargout}] = helicsMex('helicsAppFinalize', varargin{:});
end
