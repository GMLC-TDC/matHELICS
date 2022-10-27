function varargout = helicsFederateRegisterInterfaces(varargin)
%  Load interfaces from a file.
% 
%	@param fed The federate to which to load interfaces.
%	@param file The name of a file to load the interfaces from either JSON, or TOML.
% 
	[varargout{1:nargout}] = helicsMex('helicsFederateRegisterInterfaces', varargin{:});
end
