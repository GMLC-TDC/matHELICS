function varargout = helicsCreateCallbackFederate(varargin)
% Create a callback federate from a federate info object.
% 
% @details Callback federates are combination federates that run a series of callback for execution in a completely automated fashion.
% 
% @param fedName A string with the name of the federate, can be NULL or an empty string to pull the default name from fedInfo.
% @param fedInfo The federate info object that contains details on the federate.
% 
% 
% 
% @return An opaque value federate object; nullptr if the object creation failed.

	[varargout{1:nargout}] = helicsMex('helicsCreateCallbackFederate', varargin{:});
end
