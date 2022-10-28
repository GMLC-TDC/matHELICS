function varargout = helicsFederateGetPublication(varargin)
%  Get a publication object from a key.
% 
%	@param fed The value federate object to use to get the publication.
%	@param key The name of the publication.
% 
%
% 
%	@return A HelicsPublication object, the object will not be valid and err will contain an error code if no publication with the
%	specified key exists.
	[varargout{1:nargout}] = helicsMex('helicsFederateGetPublication', varargin{:});
end
