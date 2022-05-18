%{
	Load federate info from command line arguments.

	@param fi A federateInfo object.
	@param arguments A list of strings from the command line.
%}
function varargout = helicsFederateInfoLoadFromArgs(varargin)
	[varargout{1:nargout}] = helicsMex('helicsFederateInfoLoadFromArgs', varargin{:});
end
