function varargout = helicsAppRun(varargin)
% Run the HelicsApp.
% @details Execute the app to completion.
% @param app The app to execute.
% @param[in,out] err An error object that will contain an error code and string if any error occurred during the execution of the function.

	[varargout{1:nargout}] = helicsMex('helicsAppRun', varargin{:});
end
