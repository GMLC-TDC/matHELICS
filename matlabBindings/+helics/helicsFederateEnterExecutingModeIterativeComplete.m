function varargout = helicsFederateEnterExecutingModeIterativeComplete(varargin)
%  Complete the asynchronous iterative call into ExecutionMode.
% 
%	@param fed The federate to make the request of.
% 
% 
%	@return An iteration object containing the iteration time and iteration_status.
	[varargout{1:nargout}] = helicsMex('helicsFederateEnterExecutingModeIterativeComplete', varargin{:});
end
