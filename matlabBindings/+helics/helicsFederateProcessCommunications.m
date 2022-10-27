function varargout = helicsFederateProcessCommunications(varargin)
 Tell helics to process internal communications for a period of time.

%	@param fed The federate to tell to process.

%	@param period The length of time to process communications and then return control.
%	@param[in,out] err An error object that will contain an error code and string if any error occurred during the execution of the function.

	[varargout{1:nargout}] = helicsMex('helicsFederateProcessCommunications', varargin{:});
end
