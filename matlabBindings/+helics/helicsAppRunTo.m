function varargout = helicsAppRunTo(varargin)
% Run a HelicsApp to a specified stop time.
% @details It is possible to call this method repeatedly with different times.
% @param app The app to run.
% @param stopTime The desired stop time.
% @param[in,out] err An error object that will contain an error code and string if any error occurred during the execution of the function.

	[varargout{1:nargout}] = helicsMex('helicsAppRunTo', varargin{:});
end
