%{
	Set the minimum change detection tolerance.

	@param pub The publication to modify.
	@param tolerance The tolerance level for publication, values changing less than this value will not be published.

	@param[in,out] err An error object to fill out in case of an error.
%}
function varargout = helicsPublicationSetMinimumChange(varargin)
	[varargout{1:nargout}] = helicsMex("helicsPublicationSetMinimumChange", varargin{:});
end
