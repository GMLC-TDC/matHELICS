function varargout = helicsFederateClone(varargin)
%  Create a new reference to an existing federate.
% 
%	@details This will create a new HelicsFederate object that references the existing federate. The new object must be freed as well.
% 
%	@param fed An existing HelicsFederate.
% 
%
% 
%	@return A new reference to the same federate.
	[varargout{1:nargout}] = helicsMex('helicsFederateClone', varargin{:});
end
