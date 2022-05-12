%{
	Call when done using the helics library.
	This function will ensure the threads are closed properly. If possible this should be the last call before exiting.
%}
function varargout = helicsCloseLibrary(varargin)
	[varargout{1:nargout}] = helicsMex("helicsCloseLibrary", varargin{:});
end
