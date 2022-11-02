function varargout = helicsCleanupLibrary(varargin)
% Function to do some housekeeping work.
% 
% @details This runs some cleanup routines and tries to close out any residual thread that haven't been shutdown yet.

	[varargout{1:nargout}] = helicsMex('helicsCleanupLibrary', varargin{:});
end
