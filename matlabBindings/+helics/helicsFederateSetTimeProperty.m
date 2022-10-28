function varargout = helicsFederateSetTimeProperty(varargin)
%  Set a time based property for a federate.
% 
%	@param fed The federate object to set the property for.
%	@param timeProperty A integer code for a time property.
%	@param time The requested value of the property.
% 
	[varargout{1:nargout}] = helicsMex('helicsFederateSetTimeProperty', varargin{:});
end
