function varargout = helicsFederateGetInputByTarget(varargin)
% Get an input object from a target.
% 
% @param fed The value federate object to use to get the input.
% @param target The name of the publication that an input is targeting.
% 
% 
% 
% @return A HelicsInput object, the object will not be valid and err will contain an error code if no input with the specified
% key exists.

	[varargout{1:nargout}] = helicsMex('helicsFederateGetInputByTarget', varargin{:});
end
