function varargout = helicsFederateRegisterTypeInput(varargin)
%  Register an input with a defined type.
% 
%	@details The input becomes part of the federate and is destroyed when the federate is freed so there are no separate free
%	functions for subscriptions, inputs, and publications.
% 
%	@param fed The federate object in which to create an input.
%	@param key The identifier for the input.
%	@param type A string describing the expected type of the input.
%	@param units A string listing the units of the input maybe NULL.
% 
%
% 
%	@return An object containing the publication.
	[varargout{1:nargout}] = helicsMex('helicsFederateRegisterTypeInput', varargin{:});
end
