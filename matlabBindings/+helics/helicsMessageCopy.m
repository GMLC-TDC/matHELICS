function varargout = helicsMessageCopy(varargin)
 Copy a message object.

%	@param src_message The message object to copy from.
%	@param dst_message The message object to copy to.

%	@param[in,out] err An error object to fill out in case of an error.
	[varargout{1:nargout}] = helicsMex('helicsMessageCopy', varargin{:});
end
