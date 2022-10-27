function varargout = helicsFederateSetTag(varargin)
%  Set a federate tag value.
% 
%	@details This overwrites any previous value for this tag.
%	@param fed The federate to set the tag for.
%	@param tagName The name of the tag to set.
%	@param value The value of the tag.
% 
%	@param[in,out] err A pointer to an error object for catching errors.
	[varargout{1:nargout}] = helicsMex('helicsFederateSetTag', varargin{:});
end
