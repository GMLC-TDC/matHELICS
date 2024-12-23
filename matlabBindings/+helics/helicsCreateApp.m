function varargout = helicsCreateApp(varargin)
% Create a HelicsApp object.
% 
% @details Create a HelicsApp object.
% 
% @param appName A string with the name of the app, can be NULL or an empty string to pull the default name from fedInfo or the config file.
% @param appType The type of app to create.
% @param configFile Configuration file or string to pass into the app, can be NULL or empty.
% @param fedInfo The federate information to pass into the app, can be NULL.
% 
% 
% @return An opaque value app object, or nullptr if the object creation failed.

	[varargout{1:nargout}] = helicsMex('helicsCreateApp', varargin{:});
end
