function varargout = helicsMessageGetBytes(varargin)
%	Get the raw data for a message object.

%	@param message A message object to get the data for.

%	@return Raw string data.
	[varargout{1:nargout}] = helicsMex('helicsMessageGetBytes', varargin{:});
end
