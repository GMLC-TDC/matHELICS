function varargout = helicsPublicationAddTarget(varargin)
%  Add a named input to the list of targets a publication publishes to.
% 
%	@param pub The publication to add the target for.
%	@param target The name of an input that the data should be sent to.
% 
%	@param[in,out] err A pointer to an error object for catching errors.

	[varargout{1:nargout}] = helicsMex('helicsPublicationAddTarget', varargin{:});
end
