function varargout = helicsGetFederateByName(varargin)
% Get an existing federate object from a core by name.
% 
% @details The federate must have been created by one of the other functions and at least one of the objects referencing the created
%          federate must still be active in the process.
% 
% @param fedName The name of the federate to retrieve.
% 
% 
% 
% @return NULL if no fed is available by that name otherwise a HelicsFederate with that name.

	[varargout{1:nargout}] = helicsMex('helicsGetFederateByName', varargin{:});
end
