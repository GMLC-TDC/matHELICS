function varargout = helicsFederateGetIntegerProperty(varargin)
%  Get the current value of an integer property (such as a logging level).
% 
%	@param fed The federate to get the flag for.
%	@param intProperty A code for the property to set /ref helics_handle_options.
% 
%	@param[in,out] err A pointer to an error object for catching errors.
% 
%	@return The value of the property.
	[varargout{1:nargout}] = helicsMex('helicsFederateGetIntegerProperty', varargin{:});
end
