%{
	Publish data contained in a JSON file or string.

	@param fed The value federate object through which to publish the data.
	@param json The publication file name or literal JSON data string.

	@param[in,out] err The error object to complete if there is an error.

%}
function varargout = helicsFederatePublishJSON(varargin)
	[varargout{1:nargout}] = helicsMex("helicsFederatePublishJSON", varargin{:});
end
