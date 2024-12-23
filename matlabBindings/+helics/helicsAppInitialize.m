function varargout = helicsAppInitialize(varargin)
% Initialize the HelicsApp federate.
% @details Generate all the interfaces and load data for the application.
% @param app The app to initialize.
% @param[in,out] err An error object that will contain an error code and string if any error occurred during the execution of the function.

	[varargout{1:nargout}] = helicsMex('helicsAppInitialize', varargin{:});
end
