function varargout = helicsFederateInfoLoadFromString(varargin)
%  Load federate info from command line arguments contained in a string.
% 
%	@param fi A federateInfo object.
%	@param args Command line arguments specified in a string.
% 

	[varargout{1:nargout}] = helicsMex('helicsFederateInfoLoadFromString', varargin{:});
end
