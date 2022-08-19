%{
%	Register the publications via JSON publication string.

%	@param fed The value federate object to use to register the publications.
%	@param json The JSON publication string.

%	@param[in,out] err The error object to complete if there is an error.


%	@details This would be the same JSON that would be used to publish data.
%}
function varargout = helicsFederateRegisterFromPublicationJSON(varargin)
	[varargout{1:nargout}] = helicsMex('helicsFederateRegisterFromPublicationJSON', varargin{:});
end
