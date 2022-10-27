function varargout = helicsTranslatorAddInputTarget(varargin)
%  Add an input to send a translator output.
% 
%	@details All messages sent to a translator endpoint get translated and published to the translators target inputs.
%	This method adds an input to a translators which will receive translated messages.
%	@param trans The given translator to add a destination target to.
%	@param input The name of the input which will be receiving translated messages
% 

	[varargout{1:nargout}] = helicsMex('helicsTranslatorAddInputTarget', varargin{:});
end
