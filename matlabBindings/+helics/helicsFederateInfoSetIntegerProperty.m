function varargout = helicsFederateInfoSetIntegerProperty(varargin)
%  Set an integer property for a federate.
% 
%	@details Set known properties.
% 
%	@param fi The federateInfo object to alter.
%	@param intProperty An int identifying the property.
%	@param propertyValue The value to set the property to.
% 
	[varargout{1:nargout}] = helicsMex('helicsFederateInfoSetIntegerProperty', varargin{:});
end
