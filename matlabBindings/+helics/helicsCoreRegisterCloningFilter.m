function varargout = helicsCoreRegisterCloningFilter(varargin)
%  Create a cloning Filter on the specified core.
% 
%	@details Cloning filters copy a message and send it to multiple locations, source and destination can be added
%	         through other functions.
% 
%	@param core The core to register through.
%	@param name The name of the filter (can be NULL).
% 
%
% 
%	@return A HelicsFilter object.
	[varargout{1:nargout}] = helicsMex('helicsCoreRegisterCloningFilter', varargin{:});
end
