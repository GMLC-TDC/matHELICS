function varargout = helicsFederateInfoSetTimeProperty(varargin)
%  Set the output delay for a federate.
% 
%	@param fi The federate info object to alter.
%	@param timeProperty An integer representation of the time based property to set see /ref helics_properties.
%	@param propertyValue The value of the property to set the timeProperty to.
% 
	[varargout{1:nargout}] = helicsMex('helicsFederateInfoSetTimeProperty', varargin{:});
end
