function varargout = helicsMessageClear(varargin)
 Reset a message to empty state
%	@param message The message object to copy from.
%	@details The message after this function will be empty, with no source or destination

%	@param[in,out] err An error object to fill out in case of an error.
	[varargout{1:nargout}] = helicsMex('helicsMessageClear', varargin{:});
end
