function varargout = helicsInputSetInfo(varargin)
%  Set the data in the info field for an input.
% 
%	@param inp The input to query.
%	@param info The string to set.
% 
	[varargout{1:nargout}] = helicsMex('helicsInputSetInfo', varargin{:});
end
