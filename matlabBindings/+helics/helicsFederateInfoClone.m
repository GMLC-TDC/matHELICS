function varargout = helicsFederateInfoClone(varargin)
%  Create a federate info object from an existing one and clone the information.
% 
%	@param fi A federateInfo object to duplicate.
% 
%
% 
%	 @return A HelicsFederateInfo object which is a reference to the created object.
	[varargout{1:nargout}] = helicsMex('helicsFederateInfoClone', varargin{:});
end
