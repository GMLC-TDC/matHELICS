function varargout = helicsAppGetFederate(varargin)
% Run the HelicsApp.
% @details Execute the HelicsApp to completion
% @param app The app to execute.
% 
% @return An opaque value federate object that can be used in any of the federate methods, not recommended to use this object to advance
% time, the app will not likely function normally; other query, information calls, or modification calls on the federate are fine.

	[varargout{1:nargout}] = helicsMex('helicsAppGetFederate', varargin{:});
end
