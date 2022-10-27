function varargout = helicsFederateIsAsyncOperationCompleted(varargin)
%  Check if the current Asynchronous operation has completed.
% 
%	@param fed The federate to operate on.
% 
% 
%	@return HELICS_FALSE if not completed, HELICS_TRUE if completed.
	[varargout{1:nargout}] = helicsMex('helicsFederateIsAsyncOperationCompleted', varargin{:});
end
