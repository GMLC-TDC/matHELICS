%{
%	Create a source Filter on the specified core.

%	@details Filters can be created through a federate or a core, linking through a federate allows
%	         a few extra features of name matching to function on the federate interface but otherwise equivalent behavior.

%	@param core The core to register through.
%	@param type The type of filter to create /ref HelicsFilterTypes.
%	@param name The name of the filter (can be NULL).

%	@param[in,out] err A pointer to an error object for catching errors.


%	@return A HelicsFilter object.
%}
function varargout = helicsCoreRegisterFilter(varargin)
	[varargout{1:nargout}] = helicsMex('helicsCoreRegisterFilter', varargin{:});
end
