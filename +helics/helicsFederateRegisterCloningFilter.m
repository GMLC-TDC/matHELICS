%{
	Create a cloning Filter on the specified federate.

	@details Cloning filters copy a message and send it to multiple locations, source and destination can be added
	         through other functions.

	@param fed The federate to register through.
	@param name The name of the filter (can be NULL).

	@param[in,out] err A pointer to an error object for catching errors.


	@return A HelicsFilter object.
%}
function varargout = helicsFederateRegisterCloningFilter(varargin)
	[varargout{1:nargout}] = helicsMex("helicsFederateRegisterCloningFilter", varargin{:});
end
