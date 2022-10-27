function varargout = helicsGetCompilerVersion(varargin)
%  Get the compiler version used to compile HELICS.
	[varargout{1:nargout}] = helicsMex('helicsGetCompilerVersion', varargin{:});
end
