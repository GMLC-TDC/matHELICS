function varargout = helicsGetSystemInfo(varargin)
% Get a json formatted system information string, containing version info.
% The string contains fields with system information like cpu, core count, operating system, and memory,
% as well as information about the HELICS build.  Used for debugging reports and gathering other information.

	[varargout{1:nargout}] = helicsMex('helicsGetSystemInfo', varargin{:});
end
