function varargout = helicsInputGetPublicationType(varargin)
% Get the type the publisher to an input is sending.
% 
% @param ipt The input to query.
% 
% with the type name.

	[varargout{1:nargout}] = helicsMex('helicsInputGetPublicationType', varargin{:});
end
