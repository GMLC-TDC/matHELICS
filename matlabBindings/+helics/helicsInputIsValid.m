function varargout = helicsInputIsValid(varargin)
%  Check if an input is valid.
% 
%	@param ipt The input to check.
% 
%	@return HELICS_TRUE if the Input object represents a valid input.
	[varargout{1:nargout}] = helicsMex('helicsInputIsValid', varargin{:});
end
