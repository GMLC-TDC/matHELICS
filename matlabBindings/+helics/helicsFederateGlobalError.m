function varargout = helicsFederateGlobalError(varargin)
%  Generate a global error from a federate.
% 
%	@details A global error halts the co-simulation completely.
% 
%	@param fed The federate to create an error in.
%	@param errorCode The integer code for the error.
%	@param errorString A string describing the error.
	[varargout{1:nargout}] = helicsMex('helicsFederateGlobalError', varargin{:});
end
