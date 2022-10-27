function varargout = helicsFederateSetIntegerProperty(varargin)
%  Set an integer based property of a federate.
% 
%	@param fed The federate to change the property for.
%	@param intProperty The property to set.
%	@param propertyVal The value of the property.
% 
	[varargout{1:nargout}] = helicsMex('helicsFederateSetIntegerProperty', varargin{:});
end
