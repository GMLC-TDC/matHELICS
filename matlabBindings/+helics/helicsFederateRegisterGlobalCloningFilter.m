function varargout = helicsFederateRegisterGlobalCloningFilter(varargin)
%  Create a global cloning Filter on the specified federate.
% 
%	@details Cloning filters copy a message and send it to multiple locations, source and destination can be added
%	         through other functions.
% 
%	@param fed The federate to register through.
%	@param name The name of the filter (can be NULL).
% 
%
% 
%	@return A HelicsFilter object.
	[varargout{1:nargout}] = helicsMex('helicsFederateRegisterGlobalCloningFilter', varargin{:});
end
