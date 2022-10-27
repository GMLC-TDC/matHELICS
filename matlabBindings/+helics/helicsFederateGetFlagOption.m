function varargout = helicsFederateGetFlagOption(varargin)
%  Get a flag value for a federate.
% 
%	@param fed The federate to get the flag for.
%	@param flag The flag to query.
% 
%	@param[in,out] err A pointer to an error object for catching errors.
% 
%	@return The value of the flag.
	[varargout{1:nargout}] = helicsMex('helicsFederateGetFlagOption', varargin{:});
end
