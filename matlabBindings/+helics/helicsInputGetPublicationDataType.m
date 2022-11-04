function varargout = helicsInputGetPublicationDataType(varargin)
% Get the type the publisher to an input is sending.
% 
% @param ipt The input to query.
% 
% @return An int containing the enumeration value of the publication type.

	[varargout{1:nargout}] = helicsMex('helicsInputGetPublicationDataType', varargin{:});
end
