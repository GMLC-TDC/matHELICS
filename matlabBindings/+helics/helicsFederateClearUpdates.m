%{
%	Clear all the update flags from a federates inputs.

%	@param fed The value federate object for which to clear update flags.
%}
function varargout = helicsFederateClearUpdates(varargin)
	[varargout{1:nargout}] = helicsMex('helicsFederateClearUpdates', varargin{:});
end
