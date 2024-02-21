function varargout = helicsInputGetDataBuffer(varargin)
% Get a copy of the raw data in a HelicsDataBuffer
% 
% @param inp The input to get the data for.
% 
% @return A HelicsDataBuffer object containing the data

	[varargout{1:nargout}] = helicsMex('helicsInputGetDataBuffer', varargin{:});
end
