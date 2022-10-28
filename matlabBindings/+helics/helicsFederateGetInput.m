function varargout = helicsFederateGetInput(varargin)
%  Get an input object from a key.
% 
%	@param fed The value federate object to use to get the publication.
%	@param key The name of the input.
% 
%
% 
%	@return A HelicsInput object, the object will not be valid and err will contain an error code if no input with the specified
%	key exists.
	[varargout{1:nargout}] = helicsMex('helicsFederateGetInput', varargin{:});
end
