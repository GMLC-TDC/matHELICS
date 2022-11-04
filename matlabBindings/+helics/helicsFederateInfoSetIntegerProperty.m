function varargout = helicsFederateInfoSetIntegerProperty(varargin)
% Set an integer property for a federate.
% 
% @details Set known properties.
% 
% @param fi The federateInfo object to alter.
% @param intProperty An int identifying the property.
% @param propertyValue The value to set the property to.
% 
% @param[in,out] err An error object that will contain an error code and string if any error occurred during the execution of the function.

	[varargout{1:nargout}] = helicsMex('helicsFederateInfoSetIntegerProperty', varargin{:});
end
