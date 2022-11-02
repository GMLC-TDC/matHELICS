function varargout = helicsFederateInfoLoadFromString(varargin)
% Load federate info from command line arguments contained in a string.
% 
% @param fi A federateInfo object.
% @param args Command line arguments specified in a string.
% 
% @param[in,out] err An error object that will contain an error code and string if any error occurred during the execution of the function.

	[varargout{1:nargout}] = helicsMex('helicsFederateInfoLoadFromString', varargin{:});
end
