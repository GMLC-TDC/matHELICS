%{
enumeration of return values from the C interface functions

Attributes:
	HELICS_ERROR_FATAL: value:-404	global fatal error for federation
	HELICS_ERROR_EXTERNAL_TYPE: value:-203	an unknown non-helics error was produced
	HELICS_ERROR_OTHER: value:-101	the function produced a helics error of some other type
	HELICS_USER_EXCEPTION: value:-29	user code generated exception
	HELICS_ERROR_USER_ABORT: value:-27	user system abort
	HELICS_ERROR_INSUFFICIENT_SPACE: value:-18	insufficient space is available to store requested data
	HELICS_ERROR_EXECUTION_FAILURE: value:-14	the function execution has failed
	HELICS_ERROR_INVALID_FUNCTION_CALL: value:-10	the call made was invalid in the present state of the calling object
	HELICS_ERROR_INVALID_STATE_TRANSITION: value:-9	error issued when an invalid state transition occurred
	HELICS_WARNING: value:-8	the function issued a warning of some kind
	HELICS_ERROR_SYSTEM_FAILURE: value:-6	the federate has terminated unexpectedly and the call cannot be completed
	HELICS_ERROR_DISCARD: value:-5	the input was discarded and not used for some reason
	HELICS_ERROR_INVALID_ARGUMENT: value:-4	the parameter passed was invalid and unable to be used
	HELICS_ERROR_INVALID_OBJECT: value:-3	indicator that the object used was not a valid object
	HELICS_ERROR_CONNECTION_FAILURE: value:-2	the operation to connect has failed
	HELICS_ERROR_REGISTRATION_FAILURE: value:-1	registration has failed
	HELICS_OK: value:0	the function executed successfully
%}
HELICS_ERROR_FATAL = -404;
HELICS_ERROR_EXTERNAL_TYPE = -203;
HELICS_ERROR_OTHER = -101;
HELICS_USER_EXCEPTION = -29;
HELICS_ERROR_USER_ABORT = -27;
HELICS_ERROR_INSUFFICIENT_SPACE = -18;
HELICS_ERROR_EXECUTION_FAILURE = -14;
HELICS_ERROR_INVALID_FUNCTION_CALL = -10;
HELICS_ERROR_INVALID_STATE_TRANSITION = -9;
HELICS_WARNING = -8;
HELICS_ERROR_SYSTEM_FAILURE = -6;
HELICS_ERROR_DISCARD = -5;
HELICS_ERROR_INVALID_ARGUMENT = -4;
HELICS_ERROR_INVALID_OBJECT = -3;
HELICS_ERROR_CONNECTION_FAILURE = -2;
HELICS_ERROR_REGISTRATION_FAILURE = -1;
HELICS_OK = 0;