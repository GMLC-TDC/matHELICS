function varargout = helicsPublicationAddTarget(varargin)
%  Add a named input to the list of targets a publication publishes to.
% 
%	@param pub The publication to add the target for.
%	@param target The name of an input that the data should be sent to.
% 

	[varargout{1:nargout}] = helicsMex('helicsPublicationAddTarget', varargin{:});
end
