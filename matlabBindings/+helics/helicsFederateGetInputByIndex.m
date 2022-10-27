function varargout = helicsFederateGetInputByIndex(varargin)
%  Get an input by its index, typically already created via registerInterfaces file or something of that nature.
% 
%	@param fed The federate object in which to create a publication.
%	@param index The index of the publication to get.
% 
%
% 
%	@return A HelicsInput, which will be NULL if an invalid index.
	[varargout{1:nargout}] = helicsMex('helicsFederateGetInputByIndex', varargin{:});
end
