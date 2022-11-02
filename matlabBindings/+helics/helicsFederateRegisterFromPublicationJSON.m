function varargout = helicsFederateRegisterFromPublicationJSON(varargin)
% Register the publications via JSON publication string.
% 
% @param fed The value federate object to use to register the publications.
% @param json The JSON publication string.
% 
% 
% 
% @details This would be the same JSON that would be used to publish data.

	[varargout{1:nargout}] = helicsMex('helicsFederateRegisterFromPublicationJSON', varargin{:});
end
