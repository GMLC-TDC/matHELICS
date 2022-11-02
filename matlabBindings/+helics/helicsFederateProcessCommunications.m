function varargout = helicsFederateProcessCommunications(varargin)
% Tell helics to process internal communications for a period of time.
% 
% @param fed The federate to tell to process.
% 
% @param period The length of time to process communications and then return control.
%

	[varargout{1:nargout}] = helicsMex('helicsFederateProcessCommunications', varargin{:});
end
