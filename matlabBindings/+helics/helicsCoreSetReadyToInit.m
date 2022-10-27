function varargout = helicsCoreSetReadyToInit(varargin)
%  Set the core to ready for init.
% 
%	@details This function is used for cores that have filters but no federates so there needs to be
%	         a direct signal to the core to trigger the federation initialization.
% 
%	@param core The core object to enable init values for.
% 

	[varargout{1:nargout}] = helicsMex('helicsCoreSetReadyToInit', varargin{:});
end
