function varargout = helicsFederateLocalError(varargin)
% Generate a local error in a federate.
% 
% @details This will propagate through the co-simulation but not necessarily halt the co-simulation, it has a similar effect to finalize
% but does allow some interaction with a core for a brief time.
% @param fed The federate to create an error in.
% @param errorCode The integer code for the error.
% @param errorString A string describing the error.
% @param[in,out] err An error object that will contain an error code and string if any error occurred during the execution of the function.

	[varargout{1:nargout}] = helicsMex('helicsFederateLocalError', varargin{:});
end
