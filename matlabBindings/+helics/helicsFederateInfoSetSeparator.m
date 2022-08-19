%{
%	Set the separator character in the info structure.

%	@details The separator character is the separation character for local publications/endpoints in creating their global name.
%	For example if the separator character is '/'  then a local endpoint would have a globally reachable name of fedName/localName.
%	@param fi The federate info object to alter.
%	@param separator The character to use as a separator.

%	@param[in,out] err An error object that will contain an error code and string if any error occurred during the execution of the function.
%}
function varargout = helicsFederateInfoSetSeparator(varargin)
	[varargout{1:nargout}] = helicsMex('helicsFederateInfoSetSeparator', varargin{:});
end
