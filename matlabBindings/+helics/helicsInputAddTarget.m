function varargout = helicsInputAddTarget(varargin)
%  Add a publication to the list of data that an input subscribes to.
% 
%	@param ipt The named input to modify.
%	@param target The name of a publication that an input should subscribe to.
% 
%	@param[in,out] err A pointer to an error object for catching errors.

	[varargout{1:nargout}] = helicsMex('helicsInputAddTarget', varargin{:});
end
