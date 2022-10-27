function varargout = helicsFederateUnProtect(varargin)
%  remove the protection of an existing federate
% 
%	@details this function allows a federate to be retrieved on demand, it must be explicitly close
 later otherwise it will be destroyed
%	when the library is closed
% 
%	@param fedName the name of an existing federate that should not be protected
% 
	[varargout{1:nargout}] = helicsMex('helicsFederateUnProtect', varargin{:});
end
