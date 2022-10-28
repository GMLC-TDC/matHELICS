function varargout = helicsFederateSetFlagOption(varargin)
%  Set a flag for the federate.
% 
%	@param fed The federate to alter a flag for.
%	@param flag The flag to change.
%	@param flagValue The new value of the flag. 0 for false, !=0 for true.
% 
	[varargout{1:nargout}] = helicsMex('helicsFederateSetFlagOption', varargin{:});
end
