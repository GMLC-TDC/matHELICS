function varargout = helicsCreateCombinationFederate(varargin)
% Create a combination federate from a federate info object.
% 
% @details Combination federates are both value federates and message federates, objects can be used in all functions
%                      that take a HelicsFederate, helics_message_federate or HelicsFederate object as an argument
% 
% @param fedName A string with the name of the federate, can be NULL or an empty string to pull the default name from fedInfo.
% @param fedInfo The federate info object that contains details on the federate.
% 
% 
% 
% @return An opaque value federate object nullptr if the object creation failed.

	[varargout{1:nargout}] = helicsMex('helicsCreateCombinationFederate', varargin{:});
end
