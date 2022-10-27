function varargout = helicsFederateGetFilterByIndex(varargin)
%  Get a filter by its index, typically already created via registerInterfaces file or something of that nature.
% 
%	@param fed The federate object in which to create a publication.
%	@param index The index of the publication to get.
% 
%
% 
%	@return A HelicsFilter, which will be NULL if an invalid index is given.
	[varargout{1:nargout}] = helicsMex('helicsFederateGetFilterByIndex', varargin{:});
end
