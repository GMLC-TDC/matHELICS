function varargout = helicsCoreIsValid(varargin)
%  Check if a core object is a valid object.
% 
%	@param core The HelicsCore object to test.
	[varargout{1:nargout}] = helicsMex('helicsCoreIsValid', varargin{:});
end
