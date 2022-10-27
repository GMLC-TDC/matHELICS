function varargout = helicsMessageClone(varargin)
%  Clone a message object.
% 
%	@param message The message object to copy from.
% 
%	@param[in,out] err An error object to fill out in case of an error.
	[varargout{1:nargout}] = helicsMex('helicsMessageClone', varargin{:});
end
