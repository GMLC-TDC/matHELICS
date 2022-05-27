%{
	Create a core object.

	@param type The type of the core to create.
	@param name The name of the core. It can be a nullptr or empty string to have a name automatically assigned.
	@param initString An initialization string to send to the core. The format is similar to command line arguments.
	                  Typical options include a broker name, the broker address, the number of federates, etc.

	@param[in,out] err An error object that will contain an error code and string if any error occurred during the execution of the function.


	@return A HelicsCore object.

	If the core is invalid, err will contain the corresponding error message and the returned object will be NULL.

%}
function varargout = helicsCreateCore(varargin)
	[varargout{1:nargout}] = helicsMex('helicsCreateCore', varargin{:});
end
