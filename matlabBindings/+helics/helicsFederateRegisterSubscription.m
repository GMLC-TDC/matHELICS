function varargout = helicsFederateRegisterSubscription(varargin)
% Create an input and add a publication target.
% 
% @details this method is a wrapper method to create and unnamed input and add a publication target to it
% 
% @param fed The federate object in which to create an input, must have been created with /ref helicsCreateValueFederate or
% /ref helicsCreateCombinationFederate.
% @param key The identifier matching a publication to add as an input target.
% @param units A string listing the units of the input (may be NULL).
% 
% 
% 
% @return An object containing the input.

	[varargout{1:nargout}] = helicsMex('helicsFederateRegisterSubscription', varargin{:});
end
