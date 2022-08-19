%{
%	Get an input object from a key.

%	@param fed The value federate object to use to get the publication.
%	@param key The name of the input.

%	@param[in,out] err The error object to complete if there is an error.


%	@return A HelicsInput object, the object will not be valid and err will contain an error code if no input with the specified
%	key exists.
%}
function varargout = helicsFederateGetInput(varargin)
	[varargout{1:nargout}] = helicsMex('helicsFederateGetInput', varargin{:});
end
