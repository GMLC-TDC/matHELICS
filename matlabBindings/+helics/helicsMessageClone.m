function varargout = helicsMessageClone(varargin)
%  Clone a message object.
% 
%	@param message The message object to copy from.
% 
	[varargout{1:nargout}] = helicsMex('helicsMessageClone', varargin{:});
end
