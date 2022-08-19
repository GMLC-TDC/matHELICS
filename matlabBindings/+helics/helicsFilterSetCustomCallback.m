%{
%	Set a general callback for a custom filter.

%	@details Add a custom filter callback function for creating a custom filter operation in the c shared library.

%	@param filter The filter object in which to set the callback.
%	@param filtCall A function handle with the signature HelicsMessage(HelicsMessage message).
%}
function varargout = helicsFilterSetCustomCallback(varargin)
	[varargout{1:nargout}] = helicsMex('helicsFilterSetCustomCallback', varargin{:});
end
