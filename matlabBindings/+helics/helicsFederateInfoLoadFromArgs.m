function varargout = helicsFederateInfoLoadFromArgs(varargin)
%	Load federate info from command line arguments.

%	@param fi A federateInfo object.
%	@param arguments A list of strings from the command line.
	[varargout{1:nargout}] = helicsMex('helicsFederateInfoLoadFromArgs', varargin{:});
end
