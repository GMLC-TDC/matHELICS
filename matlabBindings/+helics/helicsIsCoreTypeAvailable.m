function varargout = helicsIsCoreTypeAvailable(varargin)
% Returns true if core/broker type specified is available in current compilation.
% 
% @param type A string representing a core type.
% 
% @details Options include "zmq", "udp", "ipc", "interprocess", "tcp", "default", "mpi".

	[varargout{1:nargout}] = helicsMex('helicsIsCoreTypeAvailable', varargin{:});
end
