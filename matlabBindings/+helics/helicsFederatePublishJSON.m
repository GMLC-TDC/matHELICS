function varargout = helicsFederatePublishJSON(varargin)
%  Publish data contained in a JSON file or string.
% 
%	@param fed The value federate object through which to publish the data.
%	@param json The publication file name or literal JSON data string.
% 

	[varargout{1:nargout}] = helicsMex('helicsFederatePublishJSON', varargin{:});
end
