function varargout = helicsAppLoadFile(varargin)
% Load a file to an App.
% 
% @details Loads a configuration file for an app.
% 
% @param appName A string with the name of the app, can be NULL or an empty string to pull the default name from fedInfo or the config
% file.
% @param app The app to load a file.
% @param configFile Configuration file or string to pass into the app, can be NULL or empty.
% @param[in,out] err An error object that will contain an error code and string if any error occurred during the execution of the function.

	[varargout{1:nargout}] = helicsMex('helicsAppLoadFile', varargin{:});
end