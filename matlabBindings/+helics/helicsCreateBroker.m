function varargout = helicsCreateBroker(varargin)
%  Create a broker object.
% 
%	@param type The type of the broker to create.
%	@param name The name of the broker. It can be a nullptr or empty string to have a name automatically assigned.
%	@param initString An initialization string to send to the core-the format is similar to command line arguments.
%	                  Typical options include a broker address such as --broker="XSSAF" if this is a subbroker, or the number of federates,
%	or the address.
% 
%	@param[in,out] err An error object that will contain an error code and string if any error occurred during the execution of the function.
%
% 
%	@return A HelicsBroker object.
% 
%	It will be NULL if there was an error indicated in the err object.

	[varargout{1:nargout}] = helicsMex('helicsCreateBroker', varargin{:});
end
