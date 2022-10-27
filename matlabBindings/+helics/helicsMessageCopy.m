function varargout = helicsMessageCopy(varargin)
%  Copy a message object.
% 
%	@param src_message The message object to copy from.
%	@param dst_message The message object to copy to.
% 
	[varargout{1:nargout}] = helicsMex('helicsMessageCopy', varargin{:});
end
