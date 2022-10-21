#include "helics/helics.h"
#include <mex.h>
#include <stdexcept>
#include <string>
#include <unordered_map>

static int mexFunctionCalled = 0;

static void throwHelicsMatlabError(HelicsError *err) {
	mexUnlock();
	switch (err->error_code)
	{
	case HELICS_OK:
		return;
	case HELICS_ERROR_REGISTRATION_FAILURE:
		mexErrMsgIdAndTxt("helics:registration_failure", err->message);
		break;
	case HELICS_ERROR_CONNECTION_FAILURE:
		mexErrMsgIdAndTxt("helics:connection_failure", err->message);
		break;
	case HELICS_ERROR_INVALID_OBJECT:
		mexErrMsgIdAndTxt("helics:invalid_object", err->message);
		break;
	case HELICS_ERROR_INVALID_ARGUMENT:
		mexErrMsgIdAndTxt("helics:invalid_argument", err->message);
		break;
	case HELICS_ERROR_DISCARD:
		mexErrMsgIdAndTxt("helics:discard", err->message);
		break;
	case HELICS_ERROR_SYSTEM_FAILURE:
		mexErrMsgIdAndTxt("helics:system_failure", err->message);
		break;
	case HELICS_ERROR_INVALID_STATE_TRANSITION:
		mexErrMsgIdAndTxt("helics:invalid_state_transition", err->message);
		break;
	case HELICS_ERROR_INVALID_FUNCTION_CALL:
		mexErrMsgIdAndTxt("helics:invalid_function_call", err->message);
		break;
	case HELICS_ERROR_EXECUTION_FAILURE:
		mexErrMsgIdAndTxt("helics:execution_failure", err->message);
		break;
	case HELICS_ERROR_INSUFFICIENT_SPACE:
		mexErrMsgIdAndTxt("helics:insufficient_space", err->message);
		break;
	case HELICS_ERROR_OTHER:
	case HELICS_ERROR_EXTERNAL_TYPE:
	default:
		mexErrMsgIdAndTxt("helics:error", err->message);
		break;
	}
}

static const std::unordered_map<std::string,int> wrapperFunctionMap{
	{"helicsCreateDataBuffer",65},
	{"helicsDataBufferIsValid",66},
	{"helicsWrapDataInBuffer",67},
	{"helicsDataBufferFree",68},
	{"helicsDataBufferSize",69},
	{"helicsDataBufferCapacity",70},
	{"helicsDataBufferData",71},
	{"helicsDataBufferReserve",72},
	{"helicsDataBufferClone",73},
	{"helicsIntegerToBytes",74},
	{"helicsDoubleToBytes",75},
	{"helicsStringToBytes",76},
	{"helicsRawStringToBytes",77},
	{"helicsBooleanToBytes",78},
	{"helicsCharToBytes",79},
	{"helicsTimeToBytes",80},
	{"helicsComplexToBytes",81},
	{"helicsComplexObjectToBytes",82},
	{"helicsVectorToBytes",83},
	{"helicsNamedPointToBytes",84},
	{"helicsComplexVectorToBytes",85},
	{"helicsDataBufferType",86},
	{"helicsDataBufferToInteger",87},
	{"helicsDataBufferToDouble",88},
	{"helicsDataBufferToBoolean",89},
	{"helicsDataBufferToChar",90},
	{"helicsDataBufferStringSize",91},
	{"helicsDataBufferToString",92},
	{"helicsDataBufferToRawString",93},
	{"helicsDataBufferToTime",94},
	{"helicsDataBufferToComplexObject",95},
	{"helicsDataBufferToComplex",96},
	{"helicsDataBufferVectorSize",97},
	{"helicsDataBufferToVector",98},
	{"helicsDataBufferToComplexVector",99},
	{"helicsDataBufferToNamedPoint",100},
	{"helicsDataBufferConvertToType",101},
	{"helicsGetVersion",102},
	{"helicsGetBuildFlags",103},
	{"helicsGetCompilerVersion",104},
	{"helicsGetSystemInfo",105},
	{"helicsLoadSignalHandler",108},
	{"helicsLoadThreadedSignalHandler",109},
	{"helicsClearSignalHandler",110},
	{"helicsLoadSignalHandlerCallback",111},
	{"helicsLoadSignalHandlerCallbackNoExit",112},
	{"helicsAbort",113},
	{"helicsIsCoreTypeAvailable",114},
	{"helicsCreateCore",115},
	{"helicsCreateCoreFromArgs",116},
	{"helicsCoreClone",117},
	{"helicsCoreIsValid",118},
	{"helicsCreateBroker",119},
	{"helicsCreateBrokerFromArgs",120},
	{"helicsBrokerClone",121},
	{"helicsBrokerIsValid",122},
	{"helicsBrokerIsConnected",123},
	{"helicsBrokerDataLink",124},
	{"helicsBrokerAddSourceFilterToEndpoint",125},
	{"helicsBrokerAddDestinationFilterToEndpoint",126},
	{"helicsBrokerMakeConnections",127},
	{"helicsCoreWaitForDisconnect",128},
	{"helicsBrokerWaitForDisconnect",129},
	{"helicsCoreIsConnected",130},
	{"helicsCoreDataLink",131},
	{"helicsCoreAddSourceFilterToEndpoint",132},
	{"helicsCoreAddDestinationFilterToEndpoint",133},
	{"helicsCoreMakeConnections",134},
	{"helicsBrokerGetIdentifier",135},
	{"helicsCoreGetIdentifier",136},
	{"helicsBrokerGetAddress",137},
	{"helicsCoreGetAddress",138},
	{"helicsCoreSetReadyToInit",139},
	{"helicsCoreConnect",140},
	{"helicsCoreDisconnect",141},
	{"helicsGetFederateByName",142},
	{"helicsBrokerDisconnect",143},
	{"helicsFederateDestroy",144},
	{"helicsBrokerDestroy",145},
	{"helicsCoreDestroy",146},
	{"helicsCoreFree",147},
	{"helicsBrokerFree",148},
	{"helicsCreateValueFederate",149},
	{"helicsCreateValueFederateFromConfig",150},
	{"helicsCreateMessageFederate",151},
	{"helicsCreateMessageFederateFromConfig",152},
	{"helicsCreateCombinationFederate",153},
	{"helicsCreateCombinationFederateFromConfig",154},
	{"helicsCreateCallbackFederate",155},
	{"helicsCreateCallbackFederateFromConfig",156},
	{"helicsFederateClone",157},
	{"helicsFederateProtect",158},
	{"helicsFederateUnProtect",159},
	{"helicsFederateIsProtected",160},
	{"helicsCreateFederateInfo",161},
	{"helicsFederateInfoClone",162},
	{"helicsFederateInfoLoadFromArgs",163},
	{"helicsFederateInfoLoadFromString",164},
	{"helicsFederateInfoFree",165},
	{"helicsFederateIsValid",166},
	{"helicsFederateInfoSetCoreName",167},
	{"helicsFederateInfoSetCoreInitString",168},
	{"helicsFederateInfoSetBrokerInitString",169},
	{"helicsFederateInfoSetCoreType",170},
	{"helicsFederateInfoSetCoreTypeFromString",171},
	{"helicsFederateInfoSetBroker",172},
	{"helicsFederateInfoSetBrokerKey",173},
	{"helicsFederateInfoSetBrokerPort",174},
	{"helicsFederateInfoSetLocalPort",175},
	{"helicsGetPropertyIndex",176},
	{"helicsGetFlagIndex",177},
	{"helicsGetOptionIndex",178},
	{"helicsGetOptionValue",179},
	{"helicsGetDataType",180},
	{"helicsFederateInfoSetFlagOption",181},
	{"helicsFederateInfoSetSeparator",182},
	{"helicsFederateInfoSetTimeProperty",183},
	{"helicsFederateInfoSetIntegerProperty",184},
	{"helicsFederateRegisterInterfaces",185},
	{"helicsFederateGlobalError",186},
	{"helicsFederateLocalError",187},
	{"helicsFederateFinalize",188},
	{"helicsFederateFinalizeAsync",189},
	{"helicsFederateFinalizeComplete",190},
	{"helicsFederateDisconnect",191},
	{"helicsFederateDisconnectAsync",192},
	{"helicsFederateDisconnectComplete",193},
	{"helicsFederateFree",194},
	{"helicsCloseLibrary",195},
	{"helicsFederateEnterInitializingMode",196},
	{"helicsFederateEnterInitializingModeAsync",197},
	{"helicsFederateIsAsyncOperationCompleted",198},
	{"helicsFederateEnterInitializingModeComplete",199},
	{"helicsFederateEnterExecutingMode",200},
	{"helicsFederateEnterExecutingModeAsync",201},
	{"helicsFederateEnterExecutingModeComplete",202},
	{"helicsFederateEnterExecutingModeIterative",203},
	{"helicsFederateEnterExecutingModeIterativeAsync",204},
	{"helicsFederateEnterExecutingModeIterativeComplete",205},
	{"helicsFederateGetState",206},
	{"helicsFederateGetCore",207},
	{"helicsFederateRequestTime",208},
	{"helicsFederateRequestTimeAdvance",209},
	{"helicsFederateRequestNextStep",210},
	{"helicsFederateRequestTimeIterative",211},
	{"helicsFederateRequestTimeAsync",212},
	{"helicsFederateRequestTimeComplete",213},
	{"helicsFederateRequestTimeIterativeAsync",214},
	{"helicsFederateRequestTimeIterativeComplete",215},
	{"helicsFederateProcessCommunications",216},
	{"helicsFederateGetName",217},
	{"helicsFederateSetTimeProperty",218},
	{"helicsFederateSetFlagOption",219},
	{"helicsFederateSetSeparator",220},
	{"helicsFederateSetIntegerProperty",221},
	{"helicsFederateGetTimeProperty",222},
	{"helicsFederateGetFlagOption",223},
	{"helicsFederateGetIntegerProperty",224},
	{"helicsFederateGetCurrentTime",225},
	{"helicsFederateSetGlobal",226},
	{"helicsFederateSetTag",227},
	{"helicsFederateGetTag",228},
	{"helicsFederateAddDependency",229},
	{"helicsFederateSetLogFile",230},
	{"helicsFederateLogErrorMessage",231},
	{"helicsFederateLogWarningMessage",232},
	{"helicsFederateLogInfoMessage",233},
	{"helicsFederateLogDebugMessage",234},
	{"helicsFederateLogLevelMessage",235},
	{"helicsFederateSendCommand",236},
	{"helicsFederateGetCommand",237},
	{"helicsFederateGetCommandSource",238},
	{"helicsFederateWaitCommand",239},
	{"helicsCoreSetGlobal",240},
	{"helicsBrokerSetGlobal",241},
	{"helicsCoreSendCommand",242},
	{"helicsCoreSendOrderedCommand",243},
	{"helicsBrokerSendCommand",244},
	{"helicsBrokerSendOrderedCommand",245},
	{"helicsCoreSetLogFile",246},
	{"helicsBrokerSetLogFile",247},
	{"helicsBrokerSetTimeBarrier",248},
	{"helicsBrokerClearTimeBarrier",249},
	{"helicsBrokerGlobalError",250},
	{"helicsCoreGlobalError",251},
	{"helicsCreateQuery",252},
	{"helicsQueryExecute",253},
	{"helicsQueryCoreExecute",254},
	{"helicsQueryBrokerExecute",255},
	{"helicsQueryExecuteAsync",256},
	{"helicsQueryExecuteComplete",257},
	{"helicsQueryIsCompleted",258},
	{"helicsQuerySetTarget",259},
	{"helicsQuerySetQueryString",260},
	{"helicsQuerySetOrdering",261},
	{"helicsQueryFree",262},
	{"helicsCleanupLibrary",263},
	{"helicsFederateRegisterSubscription",264},
	{"helicsFederateRegisterPublication",265},
	{"helicsFederateRegisterTypePublication",266},
	{"helicsFederateRegisterGlobalPublication",267},
	{"helicsFederateRegisterGlobalTypePublication",268},
	{"helicsFederateRegisterInput",269},
	{"helicsFederateRegisterTypeInput",270},
	{"helicsFederateRegisterGlobalInput",271},
	{"helicsFederateRegisterGlobalTypeInput",272},
	{"helicsFederateGetPublication",273},
	{"helicsFederateGetPublicationByIndex",274},
	{"helicsFederateGetInput",275},
	{"helicsFederateGetInputByIndex",276},
	{"helicsFederateGetSubscription",277},
	{"helicsFederateClearUpdates",278},
	{"helicsFederateRegisterFromPublicationJSON",279},
	{"helicsFederatePublishJSON",280},
	{"helicsPublicationIsValid",281},
	{"helicsPublicationPublishBytes",282},
	{"helicsPublicationPublishString",283},
	{"helicsPublicationPublishInteger",284},
	{"helicsPublicationPublishBoolean",285},
	{"helicsPublicationPublishDouble",286},
	{"helicsPublicationPublishTime",287},
	{"helicsPublicationPublishChar",288},
	{"helicsPublicationPublishComplex",289},
	{"helicsPublicationPublishVector",290},
	{"helicsPublicationPublishComplexVector",291},
	{"helicsPublicationPublishNamedPoint",292},
	{"helicsPublicationAddTarget",293},
	{"helicsInputIsValid",294},
	{"helicsInputAddTarget",295},
	{"helicsInputGetByteCount",296},
	{"helicsInputGetBytes",297},
	{"helicsInputGetStringSize",298},
	{"helicsInputGetString",299},
	{"helicsInputGetInteger",300},
	{"helicsInputGetBoolean",301},
	{"helicsInputGetDouble",302},
	{"helicsInputGetTime",303},
	{"helicsInputGetChar",304},
	{"helicsInputGetComplexObject",305},
	{"helicsInputGetComplex",306},
	{"helicsInputGetVectorSize",307},
	{"helicsInputGetVector",308},
	{"helicsInputGetComplexVector",309},
	{"helicsInputGetNamedPoint",310},
	{"helicsInputSetDefaultBytes",311},
	{"helicsInputSetDefaultString",312},
	{"helicsInputSetDefaultInteger",313},
	{"helicsInputSetDefaultBoolean",314},
	{"helicsInputSetDefaultTime",315},
	{"helicsInputSetDefaultChar",316},
	{"helicsInputSetDefaultDouble",317},
	{"helicsInputSetDefaultComplex",318},
	{"helicsInputSetDefaultVector",319},
	{"helicsInputSetDefaultComplexVector",320},
	{"helicsInputSetDefaultNamedPoint",321},
	{"helicsInputGetType",322},
	{"helicsInputGetPublicationType",323},
	{"helicsInputGetPublicationDataType",324},
	{"helicsPublicationGetType",325},
	{"helicsInputGetName",326},
	{"helicsSubscriptionGetTarget",327},
	{"helicsPublicationGetName",328},
	{"helicsInputGetUnits",329},
	{"helicsInputGetInjectionUnits",330},
	{"helicsInputGetExtractionUnits",331},
	{"helicsPublicationGetUnits",332},
	{"helicsInputGetInfo",333},
	{"helicsInputSetInfo",334},
	{"helicsInputGetTag",335},
	{"helicsInputSetTag",336},
	{"helicsPublicationGetInfo",337},
	{"helicsPublicationSetInfo",338},
	{"helicsPublicationGetTag",339},
	{"helicsPublicationSetTag",340},
	{"helicsInputGetOption",341},
	{"helicsInputSetOption",342},
	{"helicsPublicationGetOption",343},
	{"helicsPublicationSetOption",344},
	{"helicsPublicationSetMinimumChange",345},
	{"helicsInputSetMinimumChange",346},
	{"helicsInputIsUpdated",347},
	{"helicsInputLastUpdateTime",348},
	{"helicsInputClearUpdate",349},
	{"helicsFederateGetPublicationCount",350},
	{"helicsFederateGetInputCount",351},
	{"helicsFederateRegisterEndpoint",352},
	{"helicsFederateRegisterGlobalEndpoint",353},
	{"helicsFederateRegisterTargetedEndpoint",354},
	{"helicsFederateRegisterGlobalTargetedEndpoint",355},
	{"helicsFederateGetEndpoint",356},
	{"helicsFederateGetEndpointByIndex",357},
	{"helicsEndpointIsValid",358},
	{"helicsEndpointSetDefaultDestination",359},
	{"helicsEndpointGetDefaultDestination",360},
	{"helicsEndpointSendBytes",361},
	{"helicsEndpointSendBytesTo",362},
	{"helicsEndpointSendBytesToAt",363},
	{"helicsEndpointSendBytesAt",364},
	{"helicsEndpointSendMessage",365},
	{"helicsEndpointSendMessageZeroCopy",366},
	{"helicsEndpointSubscribe",367},
	{"helicsFederateHasMessage",368},
	{"helicsEndpointHasMessage",369},
	{"helicsFederatePendingMessageCount",370},
	{"helicsEndpointPendingMessageCount",371},
	{"helicsEndpointGetMessage",372},
	{"helicsEndpointCreateMessage",373},
	{"helicsEndpointClearMessages",374},
	{"helicsFederateGetMessage",375},
	{"helicsFederateCreateMessage",376},
	{"helicsFederateClearMessages",377},
	{"helicsEndpointGetType",378},
	{"helicsEndpointGetName",379},
	{"helicsFederateGetEndpointCount",380},
	{"helicsEndpointGetInfo",381},
	{"helicsEndpointSetInfo",382},
	{"helicsEndpointGetTag",383},
	{"helicsEndpointSetTag",384},
	{"helicsEndpointSetOption",385},
	{"helicsEndpointGetOption",386},
	{"helicsEndpointAddSourceTarget",387},
	{"helicsEndpointAddDestinationTarget",388},
	{"helicsEndpointRemoveTarget",389},
	{"helicsEndpointAddSourceFilter",390},
	{"helicsEndpointAddDestinationFilter",391},
	{"helicsMessageGetSource",392},
	{"helicsMessageGetDestination",393},
	{"helicsMessageGetOriginalSource",394},
	{"helicsMessageGetOriginalDestination",395},
	{"helicsMessageGetTime",396},
	{"helicsMessageGetString",397},
	{"helicsMessageGetMessageID",398},
	{"helicsMessageGetFlagOption",399},
	{"helicsMessageGetByteCount",400},
	{"helicsMessageGetBytes",401},
	{"helicsMessageGetBytesPointer",402},
	{"helicsMessageIsValid",403},
	{"helicsMessageSetSource",404},
	{"helicsMessageSetDestination",405},
	{"helicsMessageSetOriginalSource",406},
	{"helicsMessageSetOriginalDestination",407},
	{"helicsMessageSetTime",408},
	{"helicsMessageResize",409},
	{"helicsMessageReserve",410},
	{"helicsMessageSetMessageID",411},
	{"helicsMessageClearFlags",412},
	{"helicsMessageSetFlagOption",413},
	{"helicsMessageSetString",414},
	{"helicsMessageSetData",415},
	{"helicsMessageAppendData",416},
	{"helicsMessageCopy",417},
	{"helicsMessageClone",418},
	{"helicsMessageFree",419},
	{"helicsMessageClear",420},
	{"helicsFederateRegisterFilter",421},
	{"helicsFederateRegisterGlobalFilter",422},
	{"helicsFederateRegisterCloningFilter",423},
	{"helicsFederateRegisterGlobalCloningFilter",424},
	{"helicsCoreRegisterFilter",425},
	{"helicsCoreRegisterCloningFilter",426},
	{"helicsFederateGetFilterCount",427},
	{"helicsFederateGetFilter",428},
	{"helicsFederateGetFilterByIndex",429},
	{"helicsFilterIsValid",430},
	{"helicsFilterGetName",431},
	{"helicsFilterSet",432},
	{"helicsFilterSetString",433},
	{"helicsFilterAddDestinationTarget",434},
	{"helicsFilterAddSourceTarget",435},
	{"helicsFilterAddDeliveryEndpoint",436},
	{"helicsFilterRemoveTarget",437},
	{"helicsFilterRemoveDeliveryEndpoint",438},
	{"helicsFilterGetInfo",439},
	{"helicsFilterSetInfo",440},
	{"helicsFilterGetTag",441},
	{"helicsFilterSetTag",442},
	{"helicsFilterSetOption",443},
	{"helicsFilterGetOption",444},
	{"helicsFederateRegisterTranslator",445},
	{"helicsFederateRegisterGlobalTranslator",446},
	{"helicsCoreRegisterTranslator",447},
	{"helicsFederateGetTranslatorCount",448},
	{"helicsFederateGetTranslator",449},
	{"helicsFederateGetTranslatorByIndex",450},
	{"helicsTranslatorIsValid",451},
	{"helicsTranslatorGetName",452},
	{"helicsTranslatorSet",453},
	{"helicsTranslatorSetString",454},
	{"helicsTranslatorAddInputTarget",455},
	{"helicsTranslatorAddPublicationTarget",456},
	{"helicsTranslatorAddSourceEndpoint",457},
	{"helicsTranslatorAddDestinationEndpoint",458},
	{"helicsTranslatorRemoveTarget",459},
	{"helicsTranslatorGetInfo",460},
	{"helicsTranslatorSetInfo",461},
	{"helicsTranslatorGetTag",462},
	{"helicsTranslatorSetTag",463},
	{"helicsTranslatorSetOption",464},
	{"helicsTranslatorGetOption",465},
	{"helicsBrokerSetLoggingCallback",466},
	{"helicsCoreSetLoggingCallback",467},
	{"helicsFederateSetLoggingCallback",468},
	{"helicsFilterSetCustomCallback",469},
	{"helicsTranslatorSetCustomCallback",470},
	{"helicsFederateSetQueryCallback",471},
	{"helicsFederateSetTimeRequestEntryCallback",472},
	{"helicsFederateSetTimeUpdateCallback",473},
	{"helicsFederateSetStateChangeCallback",474},
	{"helicsFederateSetTimeRequestReturnCallback",475},
	{"helicsFederateInitializingEntryCallback",476},
	{"helicsFederateExecutingEntryCallback",477},
	{"helicsFederateCosimulationTerminationCallback",478},
	{"helicsFederateErrorHandlerCallback",479},
	{"helicsCallbackFederateNextTimeCallback",480},
	{"helicsCallbackFederateNextTimeIterativeCallback",481},
	{"helicsCallbackFederateInitializeCallback",482},
	{"helicsQueryBufferFill",483}
};

void _wrap_helicsCreateDataBuffer(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsCreateDataBuffer:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxINT32_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsCreateDataBuffer:TypeError","Argument 1 must be of type int32.");
	}
mxInt32 *pinitialCapacity = mxGetInt32s(argv[0]);
	int32_t initialCapacity = static_cast<int32_t>(pinitialCapacity[0]);

	HelicsDataBuffer result = helicsCreateDataBuffer(initialCapacity);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	mxUint64 *rv = mxGetUint64s(_out);
	rv[0] = reinterpret_cast<mxUint64>(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsDataBufferIsValid(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsDataBufferIsValid:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsDataBufferIsValid:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsDataBuffer data = *(static_cast<HelicsDataBuffer*>(mxGetData(argv[0])));

	HelicsBool result = helicsDataBufferIsValid(data);

	mxArray *_out = mxCreateLogicalMatrix(1, 1);
	mxLogical *rv = mxGetLogicals(_out);
	if(result == HELICS_TRUE){
		rv[0] = true;
	}else{
		rv[0] = false;
	}

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsWrapDataInBuffer(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 3){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsWrapDataInBuffer:rhs","This function requires 3 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsWrapDataInBuffer:TypeError","Argument 1 must be of type uint64.");
	}
		void *data = mxGetData(argv[0]);

	if(!mxIsNumeric(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsWrapDataInBuffer:TypeError","Argument 2 must be of type integer.");
	}
		int dataSize = static_cast<int>(mxGetScalar(argv[1]));

	if(!mxIsNumeric(argv[2])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsWrapDataInBuffer:TypeError","Argument 3 must be of type integer.");
	}
		int dataCapacity = static_cast<int>(mxGetScalar(argv[2]));

	HelicsDataBuffer result = helicsWrapDataInBuffer(data, dataSize, dataCapacity);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	mxUint64 *rv = mxGetUint64s(_out);
	rv[0] = reinterpret_cast<mxUint64>(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsDataBufferFree(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsDataBufferFree:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsDataBufferFree:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsDataBuffer data = *(static_cast<HelicsDataBuffer*>(mxGetData(argv[0])));

	helicsDataBufferFree(data);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsDataBufferSize(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsDataBufferSize:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsDataBufferSize:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsDataBuffer data = *(static_cast<HelicsDataBuffer*>(mxGetData(argv[0])));

	int32_t result = helicsDataBufferSize(data);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT32_CLASS, mxREAL);
	mxInt32 *rv = mxGetInt32s(_out);
	rv[0] = static_cast<mxInt32>(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsDataBufferCapacity(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsDataBufferCapacity:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsDataBufferCapacity:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsDataBuffer data = *(static_cast<HelicsDataBuffer*>(mxGetData(argv[0])));

	int32_t result = helicsDataBufferCapacity(data);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT32_CLASS, mxREAL);
	mxInt32 *rv = mxGetInt32s(_out);
	rv[0] = static_cast<mxInt32>(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsDataBufferData(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsDataBufferData:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsDataBufferData:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsDataBuffer data = *(static_cast<HelicsDataBuffer*>(mxGetData(argv[0])));

	void *result = helicsDataBufferData(data);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	mxUint64 *rv = mxGetUint64s(_out);
	rv[0] = reinterpret_cast<mxUint64>(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsDataBufferReserve(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsDataBufferReserve:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsDataBufferReserve:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsDataBuffer data = *(static_cast<HelicsDataBuffer*>(mxGetData(argv[0])));

	if(mxGetClassID(argv[1]) != mxINT32_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsDataBufferReserve:TypeError","Argument 2 must be of type int32.");
	}
mxInt32 *pnewCapacity = mxGetInt32s(argv[1]);
	int32_t newCapacity = static_cast<int32_t>(pnewCapacity[0]);

	HelicsBool result = helicsDataBufferReserve(data, newCapacity);

	mxArray *_out = mxCreateLogicalMatrix(1, 1);
	mxLogical *rv = mxGetLogicals(_out);
	if(result == HELICS_TRUE){
		rv[0] = true;
	}else{
		rv[0] = false;
	}

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsDataBufferClone(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsDataBufferClone:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsDataBufferClone:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsDataBuffer data = *(static_cast<HelicsDataBuffer*>(mxGetData(argv[0])));

	HelicsDataBuffer result = helicsDataBufferClone(data);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	mxUint64 *rv = mxGetUint64s(_out);
	rv[0] = reinterpret_cast<mxUint64>(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsIntegerToBytes(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsIntegerToBytes:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsIntegerToBytes:TypeError","Argument 1 must be of type int64.");
	}
mxInt64 *pvalue = mxGetInt64s(argv[0]);
	int64_t value = static_cast<int64_t>(pvalue[0]);

	HelicsDataBuffer data = helicsCreateDataBuffer(sizeof(int));

	int32_t result = helicsIntegerToBytes(value, data);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	mxUint64 *rv = mxGetUint64s(_out);
	rv[0] = reinterpret_cast<mxUint64>(data);
	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsDoubleToBytes(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsDoubleToBytes:rhs","This function requires 1 arguments.");
	}

	if(!mxIsNumeric(argv[0])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsDoubleToBytes:TypeError","Argument 1 must be of type double.");
	}
	double value = mxGetScalar(argv[0]);

	HelicsDataBuffer data = helicsCreateDataBuffer(sizeof(double));

	int32_t result = helicsDoubleToBytes(value, data);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	mxUint64 *rv = mxGetUint64s(_out);
	rv[0] = reinterpret_cast<mxUint64>(data);
	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsStringToBytes(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsStringToBytes:rhs","This function requires 1 arguments.");
	}

	if(!mxIsChar(argv[0])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsStringToBytes:TypeError","Argument 1 must be a string.");
	}
	char *value = nullptr;
	size_t valueLength = 0;
	int valueStatus = 0;
	if(argc > 0){
		valueLength = mxGetN(argv[0]) + 1;
		value = static_cast<char *>(malloc(valueLength));
		valueStatus = mxGetString(argv[0], value, valueLength);

	}
	HelicsDataBuffer data = helicsCreateDataBuffer(static_cast<int32_t>(valueLength));

	int32_t result = helicsStringToBytes(value, data);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	mxUint64 *rv = mxGetUint64s(_out);
	rv[0] = reinterpret_cast<mxUint64>(data);
	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsRawStringToBytes(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsRawStringToBytes:rhs","This function requires 1 arguments.");
	}

	if(!mxIsChar(argv[0])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsRawStringToBytes:TypeError","Argument 1 must be a string.");
	}
	char *str = nullptr;
	size_t strLength = 0;
	int strStatus = 0;
	if(argc > 0){
		strLength = mxGetN(argv[0]) + 1;
		str = static_cast<char *>(malloc(strLength));
		strStatus = mxGetString(argv[0], str, strLength);

	}
	HelicsDataBuffer data = helicsCreateDataBuffer(static_cast<int32_t>(strLength));

	int32_t result = helicsRawStringToBytes(str, (int)strLength, data);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	mxUint64 *rv = mxGetUint64s(_out);
	rv[0] = reinterpret_cast<mxUint64>(data);
	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsBooleanToBytes(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsBooleanToBytes:rhs","This function requires 1 arguments.");
	}

	if(!mxIsLogical(argv[0])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsBooleanToBytes:TypeError","Argument 1 must be a 0 or 1.");
	}
	mxLogical *pvalue = mxGetLogicals(argv[0]);
	HelicsBool value = HELICS_FALSE;
	if(pvalue[0]){
		value = HELICS_TRUE;
	}

	HelicsDataBuffer data = helicsCreateDataBuffer(sizeof(HelicsBool));

	int32_t result = helicsBooleanToBytes(value, data);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	mxUint64 *rv = mxGetUint64s(_out);
	rv[0] = reinterpret_cast<mxUint64>(data);
	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsCharToBytes(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsCharToBytes:rhs","This function requires 1 arguments.");
	}

	if(!mxIsChar(argv[0])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsCharToBytes:TypeError","Argument 1 must be a string.");
	}
	char *value = nullptr;
	size_t valueLength = 0;
	int valueStatus = 0;
	if(argc > 0){
		valueLength = mxGetN(argv[0]) + 1;
		value = static_cast<char *>(malloc(valueLength));
		valueStatus = mxGetString(argv[0], value, valueLength);

	}
	HelicsDataBuffer data = helicsCreateDataBuffer(sizeof(char));

	int32_t result = helicsCharToBytes(*value, data);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	mxUint64 *rv = mxGetUint64s(_out);
	rv[0] = reinterpret_cast<mxUint64>(data);
	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsTimeToBytes(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsTimeToBytes:rhs","This function requires 1 arguments.");
	}

	if(!mxIsNumeric(argv[0])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsTimeToBytes:TypeError","Argument 1 must be of type double.");
	}
	HelicsTime value = (HelicsTime)(mxGetScalar(argv[0]));

	HelicsDataBuffer data = helicsCreateDataBuffer(sizeof(HelicsTime));

	int32_t result = helicsTimeToBytes(value, data);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	mxUint64 *rv = mxGetUint64s(_out);
	rv[0] = reinterpret_cast<mxUint64>(data);
	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsComplexToBytes(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsComplexToBytes:rhs","This function requires 1 arguments.");
	}

	if(!mxIsComplex(argv[0])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsComplexToBytes:TypeError","Argument 1 must be of type complex.");
	}

	mxComplexDouble *value = mxGetComplexDoubles(argv[0]);

	HelicsDataBuffer data = helicsCreateDataBuffer(2*sizeof(double));

	int32_t result = helicsComplexToBytes(value->real, value->imag, data);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	mxUint64 *rv = mxGetUint64s(_out);
	rv[0] = reinterpret_cast<mxUint64>(data);
	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsComplexObjectToBytes(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsComplexObjectToBytes:rhs","This function requires 1 arguments.");
	}

	if(!mxIsComplex(argv[0])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsComplexObjectToBytes:TypeError","Argument 1 must be of type complex.");
	}

	mxComplexDouble *value = mxGetComplexDoubles(argv[0]);

	HelicsDataBuffer data = helicsCreateDataBuffer(sizeof(HelicsComplex));

	HelicsComplex vObj = {value->real, value->imag};

	int32_t result = helicsComplexObjectToBytes(vObj, data);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	mxUint64 *rv = mxGetUint64s(_out);
	rv[0] = reinterpret_cast<mxUint64>(data);
	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsVectorToBytes(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsVectorToBytes:rhs","This function requires 1 arguments.");
	}

	if(!mxIsNumeric(argv[0])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsVectorToBytes:TypeError","Argument 1 must be an array of doubles.");
	}
	int dataSize =  static_cast<int>(mxGetNumberOfElements(argv[0]));

	double *value =  static_cast<double *>(mxGetDoubles(argv[0]));

	HelicsDataBuffer data = helicsCreateDataBuffer(dataSize*sizeof(double));

	int32_t result = helicsVectorToBytes(static_cast<const double *>(value), dataSize, data);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	mxUint64 *rv = mxGetUint64s(_out);
	rv[0] = reinterpret_cast<mxUint64>(data);
	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsNamedPointToBytes(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsNamedPointToBytes:rhs","This function requires 2 arguments.");
	}

	if(!mxIsChar(argv[0])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsNamedPointToBytes:TypeError","Argument 1 must be a string.");
	}
	char *name = nullptr;
	size_t nameLength = 0;
	int nameStatus = 0;
	if(argc > 0){
		nameLength = mxGetN(argv[0]) + 1;
		name = static_cast<char *>(malloc(nameLength));
		nameStatus = mxGetString(argv[0], name, nameLength);

	}
	if(!mxIsNumeric(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsNamedPointToBytes:TypeError","Argument 2 must be of type double.");
	}
	double value = mxGetScalar(argv[1]);

	HelicsDataBuffer data = helicsCreateDataBuffer(sizeof(double));

	int32_t result = helicsNamedPointToBytes(name, value, data);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	mxUint64 *rv = mxGetUint64s(_out);
	rv[0] = reinterpret_cast<mxUint64>(data);
	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsComplexVectorToBytes(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsComplexVectorToBytes:rhs","This function requires 1 arguments.");
	}

	if(!mxIsComplex(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsComplexVectorToBytes:TypeError","Argument 1 must be of an array of type complex.");
	}
	int dataSize =  static_cast<int>(mxGetN(argv[0])*2);

	double *value = static_cast<double *>(malloc(dataSize * sizeof(double)));
	mxComplexDouble *vals = mxGetComplexDoubles(argv[0]);
	for(int i=0; i<dataSize/2; ++i){
		value[2*i] = vals[i].real;
		value[2*i + 1] = vals[i].imag;
	}

	HelicsDataBuffer data = helicsCreateDataBuffer(dataSize*sizeof(double));

	int32_t result = helicsComplexVectorToBytes(static_cast<const double *>(value), dataSize, data);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	mxUint64 *rv = mxGetUint64s(_out);
	rv[0] = reinterpret_cast<mxUint64>(data);
	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsDataBufferType(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsDataBufferType:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsDataBufferType:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsDataBuffer data = *(static_cast<HelicsDataBuffer*>(mxGetData(argv[0])));

	int result = helicsDataBufferType(data);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
	mxInt64 *rv = mxGetInt64s(_out);
	rv[0] = static_cast<mxInt64>(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsDataBufferToInteger(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsDataBufferToInteger:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsDataBufferToInteger:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsDataBuffer data = *(static_cast<HelicsDataBuffer*>(mxGetData(argv[0])));

	int64_t result = helicsDataBufferToInteger(data);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
	mxInt64 *rv = mxGetInt64s(_out);
	rv[0] = static_cast<mxInt64>(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsDataBufferToDouble(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsDataBufferToDouble:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsDataBufferToDouble:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsDataBuffer data = *(static_cast<HelicsDataBuffer*>(mxGetData(argv[0])));

	double result = helicsDataBufferToDouble(data);

	mxArray *_out = mxCreateDoubleScalar(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsDataBufferToBoolean(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsDataBufferToBoolean:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsDataBufferToBoolean:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsDataBuffer data = *(static_cast<HelicsDataBuffer*>(mxGetData(argv[0])));

	HelicsBool result = helicsDataBufferToBoolean(data);

	mxArray *_out = mxCreateLogicalMatrix(1, 1);
	mxLogical *rv = mxGetLogicals(_out);
	if(result == HELICS_TRUE){
		rv[0] = true;
	}else{
		rv[0] = false;
	}

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsDataBufferToChar(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsDataBufferToChar:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsDataBufferToChar:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsDataBuffer data = *(static_cast<HelicsDataBuffer*>(mxGetData(argv[0])));

	char result = helicsDataBufferToChar(data);

	mxArray *_out = mxCreateString((const char *)(&result));

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsDataBufferStringSize(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsDataBufferStringSize:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsDataBufferStringSize:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsDataBuffer data = *(static_cast<HelicsDataBuffer*>(mxGetData(argv[0])));

	int result = helicsDataBufferStringSize(data);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
	mxInt64 *rv = mxGetInt64s(_out);
	rv[0] = static_cast<mxInt64>(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsDataBufferToString(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsDataBufferToString:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsDataBufferToString:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsDataBuffer data = *(static_cast<HelicsDataBuffer*>(mxGetData(argv[0])));

	int maxStringLen = helicsDataBufferStringSize(data) + 2;

	char *outputString = static_cast<char *>(malloc(maxStringLen));

	int actualLength = 0;

	helicsDataBufferToString(data, outputString, maxStringLen, &actualLength);

	mwSize dims[2] = {1, static_cast<mwSize>(actualLength)};
	mxArray *_out = mxCreateCharArray(2, dims);
	mxChar *out_data = static_cast<mxChar *>(mxGetData(_out));
	for(int i=0; i<(actualLength); ++i){
		out_data[i] = outputString[i];
	}

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(outputString);

}


void _wrap_helicsDataBufferToRawString(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsDataBufferToRawString:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsDataBufferToRawString:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsDataBuffer data = *(static_cast<HelicsDataBuffer*>(mxGetData(argv[0])));

	int maxStringLen = helicsDataBufferStringSize(data) + 2;

	char *outputString = static_cast<char *>(malloc(maxStringLen));

	int actualLength = 0;

	helicsDataBufferToRawString(data, outputString, maxStringLen, &actualLength);

	mwSize dims[2] = {1, static_cast<mwSize>(actualLength) - 1};
	mxArray *_out = mxCreateCharArray(2, dims);
	mxChar *out_data = static_cast<mxChar *>(mxGetData(_out));
	for(int i=0; i<(actualLength - 1); ++i){
		out_data[i] = outputString[i];
	}

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(outputString);

}


void _wrap_helicsDataBufferToTime(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsDataBufferToTime:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsDataBufferToTime:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsDataBuffer data = *(static_cast<HelicsDataBuffer*>(mxGetData(argv[0])));

	HelicsTime result = helicsDataBufferToTime(data);

	mxArray *_out = mxCreateDoubleScalar(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsDataBufferToComplexObject(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsDataBufferToComplexObject:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsDataBufferToComplexObject:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsDataBuffer data = *(static_cast<HelicsDataBuffer*>(mxGetData(argv[0])));

	HelicsComplex result = helicsDataBufferToComplexObject(data);

	mxComplexDouble complex_result;
	complex_result.real = result.real;
	complex_result.imag = result.imag;
	mxArray *_out = mxCreateDoubleMatrix(1,1,mxCOMPLEX);
	int status = mxSetComplexDoubles(_out, &complex_result);

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsDataBufferToComplex(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsDataBufferToComplex:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsDataBufferToComplex:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsDataBuffer data = *(static_cast<HelicsDataBuffer*>(mxGetData(argv[0])));

	double values[2];

	helicsDataBufferToComplex(data, &(values[0]), &(values[1]));

	mxArray *_out = mxCreateDoubleMatrix(1,1,mxCOMPLEX);
	mxComplexDouble *complex_result = mxGetComplexDoubles(_out);
	complex_result->real = values[0];
	complex_result->imag = values[1];
	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsDataBufferVectorSize(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsDataBufferVectorSize:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsDataBufferVectorSize:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsDataBuffer data = *(static_cast<HelicsDataBuffer*>(mxGetData(argv[0])));

	int result = helicsDataBufferVectorSize(data);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
	mxInt64 *rv = mxGetInt64s(_out);
	rv[0] = static_cast<mxInt64>(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsDataBufferToVector(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsDataBufferToVector:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsDataBufferToVector:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsDataBuffer data = *(static_cast<HelicsDataBuffer*>(mxGetData(argv[0])));

	int maxLen = helicsDataBufferVectorSize(data);

	double *values = static_cast<double *>(malloc(maxLen * sizeof(double)));

	int actualSize = 0;

	helicsDataBufferToVector(data, values, maxLen, &actualSize);

	mxDouble *result_data = static_cast<mxDouble *>(mxMalloc(actualSize * sizeof(mxDouble)));
	for(int i=0; i<actualSize; ++i){
		result_data[i] = static_cast<mxDouble>(values[i]);
	}
	mxArray *_out = mxCreateDoubleMatrix(actualSize, 1, mxREAL);
	int status = mxSetDoubles(_out, &(result_data[0]));

	if(_out){
		--resc;
		*resv++ = _out;
	}

}


void _wrap_helicsDataBufferToComplexVector(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsDataBufferToComplexVector:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsDataBufferToComplexVector:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsDataBuffer data = *(static_cast<HelicsDataBuffer*>(mxGetData(argv[0])));

	int maxLen = helicsDataBufferVectorSize(data);

	double *values = static_cast<double *>(malloc(maxLen * sizeof(double)));

	int actualSize = 0;

	helicsDataBufferToComplexVector(data, values, maxLen, &actualSize);

	mxComplexDouble *result_data = static_cast<mxComplexDouble *>(mxMalloc((actualSize/2)*sizeof(mxComplexDouble)));
	for(int i=0; i<(actualSize/2); ++i){
		result_data[i].real = values[2*(i)];
		result_data[i].imag = values[2*(i) + 1];
	}
	mxArray *_out = mxCreateDoubleMatrix(actualSize/2, 1, mxCOMPLEX);
	int status = mxSetComplexDoubles(_out, &(result_data[0]));

	if(_out){
		--resc;
		*resv++ = _out;
	}

}


void _wrap_helicsDataBufferToNamedPoint(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsDataBufferToNamedPoint:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsDataBufferToNamedPoint:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsDataBuffer data = *(static_cast<HelicsDataBuffer*>(mxGetData(argv[0])));

	int maxStringLen = helicsInputGetStringSize(data) + 2;

	char *outputString = static_cast<char *>(malloc(maxStringLen));

	int actualLength = 0;

	double val = 0;

	helicsDataBufferToNamedPoint(data, outputString, maxStringLen, &actualLength, &val);

	mwSize dims[2] = {1, static_cast<mwSize>(actualLength)-1};
	mxArray *_out = mxCreateCharArray(2, dims);
	mxChar *out_data = static_cast<mxChar *>(mxGetData(_out));
	for(int i=0; i<(actualLength-1); ++i){
		out_data[i] = outputString[i];
	}

	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(--resc>=0){
		mxArray *_out1 = mxCreateDoubleScalar(val);
		*resv++ = _out1;
	}

	free(outputString);
}


void _wrap_helicsDataBufferConvertToType(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsDataBufferConvertToType:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsDataBufferConvertToType:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsDataBuffer data = *(static_cast<HelicsDataBuffer*>(mxGetData(argv[0])));

	if(!mxIsNumeric(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsDataBufferConvertToType:TypeError","Argument 2 must be of type integer.");
	}
		int newDataType = static_cast<int>(mxGetScalar(argv[1]));

	HelicsBool result = helicsDataBufferConvertToType(data, newDataType);

	mxArray *_out = mxCreateLogicalMatrix(1, 1);
	mxLogical *rv = mxGetLogicals(_out);
	if(result == HELICS_TRUE){
		rv[0] = true;
	}else{
		rv[0] = false;
	}

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsGetVersion(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 0){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsGetVersion:rhs","This function requires 0 arguments.");
	}

	const char *result = helicsGetVersion();

	mxArray *_out = mxCreateString(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsGetBuildFlags(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 0){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsGetBuildFlags:rhs","This function requires 0 arguments.");
	}

	const char *result = helicsGetBuildFlags();

	mxArray *_out = mxCreateString(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsGetCompilerVersion(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 0){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsGetCompilerVersion:rhs","This function requires 0 arguments.");
	}

	const char *result = helicsGetCompilerVersion();

	mxArray *_out = mxCreateString(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsGetSystemInfo(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 0){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsGetSystemInfo:rhs","This function requires 0 arguments.");
	}

	const char *result = helicsGetSystemInfo();

	mxArray *_out = mxCreateString(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsLoadSignalHandler(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 0){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsLoadSignalHandler:rhs","This function requires 0 arguments.");
	}

	helicsLoadSignalHandler();

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsLoadThreadedSignalHandler(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 0){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsLoadThreadedSignalHandler:rhs","This function requires 0 arguments.");
	}

	helicsLoadThreadedSignalHandler();

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsClearSignalHandler(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 0){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsClearSignalHandler:rhs","This function requires 0 arguments.");
	}

	helicsClearSignalHandler();

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsAbort(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc < 1 || argc > 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsAbort:rhs","This function requires at least 1 arguments and at most 2 arguments.");
	}

	if(!mxIsNumeric(argv[0])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsAbort:TypeError","Argument 1 must be of type integer.");
	}
		int errorCode = static_cast<int>(mxGetScalar(argv[0]));

	if(argc > 1){
		if(!mxIsChar(argv[1])){
			mexUnlock();
			mexErrMsgIdAndTxt("MATLAB:helicsAbort:TypeError","Argument 2 must be a string.");
		}
	}
	char *errorString = nullptr;
	size_t errorStringLength = 0;
	int errorStringStatus = 0;
	if(argc > 1){
		errorStringLength = mxGetN(argv[1]) + 1;
		errorString = static_cast<char *>(malloc(errorStringLength));
		errorStringStatus = mxGetString(argv[1], errorString, errorStringLength);

	}
	helicsAbort(errorCode, static_cast<const char *>(errorString));

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(errorString);
}


void _wrap_helicsIsCoreTypeAvailable(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc < 0 || argc > 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsIsCoreTypeAvailable:rhs","This function requires at least 0 arguments and at most 1 arguments.");
	}

	if(argc > 0){
		if(!mxIsChar(argv[0])){
			mexUnlock();
			mexErrMsgIdAndTxt("MATLAB:helicsIsCoreTypeAvailable:TypeError","Argument 1 must be a string.");
		}
	}
	char *type = nullptr;
	size_t typeLength = 0;
	int typeStatus = 0;
	if(argc > 0){
		typeLength = mxGetN(argv[0]) + 1;
		type = static_cast<char *>(malloc(typeLength));
		typeStatus = mxGetString(argv[0], type, typeLength);

	}
	HelicsBool result = helicsIsCoreTypeAvailable(static_cast<const char *>(type));

	mxArray *_out = mxCreateLogicalMatrix(1, 1);
	mxLogical *rv = mxGetLogicals(_out);
	if(result == HELICS_TRUE){
		rv[0] = true;
	}else{
		rv[0] = false;
	}

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(type);
}


void _wrap_helicsCreateCore(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 3){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsCreateCore:rhs","This function requires 3 arguments.");
	}

	if(!mxIsChar(argv[0])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsCreateCore:TypeError","Argument 1 must be a string.");
	}
	char *type = nullptr;
	size_t typeLength = 0;
	int typeStatus = 0;
	if(argc > 0){
		typeLength = mxGetN(argv[0]) + 1;
		type = static_cast<char *>(malloc(typeLength));
		typeStatus = mxGetString(argv[0], type, typeLength);

	}
	if(!mxIsChar(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsCreateCore:TypeError","Argument 2 must be a string.");
	}
	char *name = nullptr;
	size_t nameLength = 0;
	int nameStatus = 0;
	if(argc > 1){
		nameLength = mxGetN(argv[1]) + 1;
		name = static_cast<char *>(malloc(nameLength));
		nameStatus = mxGetString(argv[1], name, nameLength);

	}
	if(!mxIsChar(argv[2])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsCreateCore:TypeError","Argument 3 must be a string.");
	}
	char *initString = nullptr;
	size_t initStringLength = 0;
	int initStringStatus = 0;
	if(argc > 2){
		initStringLength = mxGetN(argv[2]) + 1;
		initString = static_cast<char *>(malloc(initStringLength));
		initStringStatus = mxGetString(argv[2], initString, initStringLength);

	}
	HelicsError err = helicsErrorInitialize();

	HelicsCore result = helicsCreateCore(static_cast<const char *>(type), static_cast<const char *>(name), static_cast<const char *>(initString), &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	mxUint64 *rv = mxGetUint64s(_out);
	rv[0] = reinterpret_cast<mxUint64>(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(type);

	free(name);

	free(initString);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsCreateCoreFromArgs(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 3){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsCreateCoreFromArgs:rhs","This function requires 3 arguments.");
	}

	if(!mxIsChar(argv[0])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsCreateCoreFromArgs:TypeError","Argument 1 must be a string.");
	}
	char *type = nullptr;
	size_t typeLength = 0;
	int typeStatus = 0;
	if(argc > 0){
		typeLength = mxGetN(argv[0]) + 1;
		type = static_cast<char *>(malloc(typeLength));
		typeStatus = mxGetString(argv[0], type, typeLength);

	}
	if(!mxIsChar(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsCreateCoreFromArgs:TypeError","Argument 2 must be a string.");
	}
	char *name = nullptr;
	size_t nameLength = 0;
	int nameStatus = 0;
	if(argc > 1){
		nameLength = mxGetN(argv[1]) + 1;
		name = static_cast<char *>(malloc(nameLength));
		nameStatus = mxGetString(argv[1], name, nameLength);

	}
	int arg2 = 0;
	char **arg3;
	int ii;
	if(!mxIsCell(argv[2])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsCreateCoreFromArgs:TypeError","Argument 3 must be a cell array of strings.");
	}
	arg2 = static_cast<int>(mxGetNumberOfElements(argv[2]));
	arg3 = static_cast<char **>(malloc((arg2)*sizeof(char *)));
	for (ii=0;ii<arg2;ii++){
		mxArray *cellElement=mxGetCell(argv[2], ii);
		size_t len = mxGetN(cellElement) + 1;
		arg3[ii] = static_cast<char *>(malloc(static_cast<int>(len)));
		int flag = mxGetString(cellElement, arg3[ii], static_cast<int>(len));
	}

	HelicsError err = helicsErrorInitialize();

	HelicsCore result = helicsCreateCoreFromArgs(type, name, arg2, arg3, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	mxUint64 *rv = mxGetUint64s(_out);
	rv[0] = reinterpret_cast<mxUint64>(result);	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(type);

	free(name);

	free(arg3);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsCoreClone(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsCoreClone:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsCoreClone:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsCore core = *(static_cast<HelicsCore*>(mxGetData(argv[0])));

	HelicsError err = helicsErrorInitialize();

	HelicsCore result = helicsCoreClone(core, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	mxUint64 *rv = mxGetUint64s(_out);
	rv[0] = reinterpret_cast<mxUint64>(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsCoreIsValid(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsCoreIsValid:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsCoreIsValid:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsCore core = *(static_cast<HelicsCore*>(mxGetData(argv[0])));

	HelicsBool result = helicsCoreIsValid(core);

	mxArray *_out = mxCreateLogicalMatrix(1, 1);
	mxLogical *rv = mxGetLogicals(_out);
	if(result == HELICS_TRUE){
		rv[0] = true;
	}else{
		rv[0] = false;
	}

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsCreateBroker(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 3){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsCreateBroker:rhs","This function requires 3 arguments.");
	}

	if(!mxIsChar(argv[0])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsCreateBroker:TypeError","Argument 1 must be a string.");
	}
	char *type = nullptr;
	size_t typeLength = 0;
	int typeStatus = 0;
	if(argc > 0){
		typeLength = mxGetN(argv[0]) + 1;
		type = static_cast<char *>(malloc(typeLength));
		typeStatus = mxGetString(argv[0], type, typeLength);

	}
	if(!mxIsChar(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsCreateBroker:TypeError","Argument 2 must be a string.");
	}
	char *name = nullptr;
	size_t nameLength = 0;
	int nameStatus = 0;
	if(argc > 1){
		nameLength = mxGetN(argv[1]) + 1;
		name = static_cast<char *>(malloc(nameLength));
		nameStatus = mxGetString(argv[1], name, nameLength);

	}
	if(!mxIsChar(argv[2])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsCreateBroker:TypeError","Argument 3 must be a string.");
	}
	char *initString = nullptr;
	size_t initStringLength = 0;
	int initStringStatus = 0;
	if(argc > 2){
		initStringLength = mxGetN(argv[2]) + 1;
		initString = static_cast<char *>(malloc(initStringLength));
		initStringStatus = mxGetString(argv[2], initString, initStringLength);

	}
	HelicsError err = helicsErrorInitialize();

	HelicsBroker result = helicsCreateBroker(static_cast<const char *>(type), static_cast<const char *>(name), static_cast<const char *>(initString), &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	mxUint64 *rv = mxGetUint64s(_out);
	rv[0] = reinterpret_cast<mxUint64>(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(type);

	free(name);

	free(initString);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsCreateBrokerFromArgs(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 3){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsCreateBrokerFromArgs:rhs","This function requires 3 arguments.");
	}

	if(!mxIsChar(argv[0])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsCreateBrokerFromArgs:TypeError","Argument 1 must be a string.");
	}
	char *type = nullptr;
	size_t typeLength = 0;
	int typeStatus = 0;
	if(argc > 0){
		typeLength = mxGetN(argv[0]) + 1;
		type = static_cast<char *>(malloc(typeLength));
		typeStatus = mxGetString(argv[0], type, typeLength);

	}
	if(!mxIsChar(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsCreateBrokerFromArgs:TypeError","Argument 2 must be a string.");
	}
	char *name = nullptr;
	size_t nameLength = 0;
	int nameStatus = 0;
	if(argc > 1){
		nameLength = mxGetN(argv[1]) + 1;
		name = static_cast<char *>(malloc(nameLength));
		nameStatus = mxGetString(argv[1], name, nameLength);

	}
	if(!mxIsCell(argv[2])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsCreateBrokerFromArgs:TypeError","Argument 3 must be a cell array of strings.");
	}
	int arg2 = 0;
	char **arg3;
	int ii;
	arg2 = static_cast<int>(mxGetNumberOfElements(argv[2]));
	arg3 = static_cast<char **>(malloc((arg2)*sizeof(char *)));
	for (ii=0;ii<arg2;ii++){
		mxArray *cellElement=mxGetCell(argv[2], ii);
		size_t len = mxGetN(cellElement) + 1;
		arg3[ii] = static_cast<char *>(malloc(static_cast<int>(len)));
		int flag = mxGetString(cellElement, arg3[ii], static_cast<int>(len));
	}

	HelicsError err = helicsErrorInitialize();

	HelicsBroker result = helicsCreateBrokerFromArgs(type, name, arg2, arg3, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	mxUint64 *rv = mxGetUint64s(_out);
	rv[0] = reinterpret_cast<mxUint64>(result);	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(type);

	free(name);

	free(arg3);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsBrokerClone(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsBrokerClone:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsBrokerClone:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsBroker broker = *(static_cast<HelicsBroker*>(mxGetData(argv[0])));

	HelicsError err = helicsErrorInitialize();

	HelicsBroker result = helicsBrokerClone(broker, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	mxUint64 *rv = mxGetUint64s(_out);
	rv[0] = reinterpret_cast<mxUint64>(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsBrokerIsValid(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsBrokerIsValid:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsBrokerIsValid:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsBroker broker = *(static_cast<HelicsBroker*>(mxGetData(argv[0])));

	HelicsBool result = helicsBrokerIsValid(broker);

	mxArray *_out = mxCreateLogicalMatrix(1, 1);
	mxLogical *rv = mxGetLogicals(_out);
	if(result == HELICS_TRUE){
		rv[0] = true;
	}else{
		rv[0] = false;
	}

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsBrokerIsConnected(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsBrokerIsConnected:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsBrokerIsConnected:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsBroker broker = *(static_cast<HelicsBroker*>(mxGetData(argv[0])));

	HelicsBool result = helicsBrokerIsConnected(broker);

	mxArray *_out = mxCreateLogicalMatrix(1, 1);
	mxLogical *rv = mxGetLogicals(_out);
	if(result == HELICS_TRUE){
		rv[0] = true;
	}else{
		rv[0] = false;
	}

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsBrokerDataLink(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 3){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsBrokerDataLink:rhs","This function requires 3 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsBrokerDataLink:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsBroker broker = *(static_cast<HelicsBroker*>(mxGetData(argv[0])));

	if(!mxIsChar(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsBrokerDataLink:TypeError","Argument 2 must be a string.");
	}
	char *source = nullptr;
	size_t sourceLength = 0;
	int sourceStatus = 0;
	if(argc > 1){
		sourceLength = mxGetN(argv[1]) + 1;
		source = static_cast<char *>(malloc(sourceLength));
		sourceStatus = mxGetString(argv[1], source, sourceLength);

	}
	if(!mxIsChar(argv[2])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsBrokerDataLink:TypeError","Argument 3 must be a string.");
	}
	char *target = nullptr;
	size_t targetLength = 0;
	int targetStatus = 0;
	if(argc > 2){
		targetLength = mxGetN(argv[2]) + 1;
		target = static_cast<char *>(malloc(targetLength));
		targetStatus = mxGetString(argv[2], target, targetLength);

	}
	HelicsError err = helicsErrorInitialize();

	helicsBrokerDataLink(broker, static_cast<const char *>(source), static_cast<const char *>(target), &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(source);

	free(target);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsBrokerAddSourceFilterToEndpoint(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 3){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsBrokerAddSourceFilterToEndpoint:rhs","This function requires 3 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsBrokerAddSourceFilterToEndpoint:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsBroker broker = *(static_cast<HelicsBroker*>(mxGetData(argv[0])));

	if(!mxIsChar(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsBrokerAddSourceFilterToEndpoint:TypeError","Argument 2 must be a string.");
	}
	char *filter = nullptr;
	size_t filterLength = 0;
	int filterStatus = 0;
	if(argc > 1){
		filterLength = mxGetN(argv[1]) + 1;
		filter = static_cast<char *>(malloc(filterLength));
		filterStatus = mxGetString(argv[1], filter, filterLength);

	}
	if(!mxIsChar(argv[2])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsBrokerAddSourceFilterToEndpoint:TypeError","Argument 3 must be a string.");
	}
	char *endpoint = nullptr;
	size_t endpointLength = 0;
	int endpointStatus = 0;
	if(argc > 2){
		endpointLength = mxGetN(argv[2]) + 1;
		endpoint = static_cast<char *>(malloc(endpointLength));
		endpointStatus = mxGetString(argv[2], endpoint, endpointLength);

	}
	HelicsError err = helicsErrorInitialize();

	helicsBrokerAddSourceFilterToEndpoint(broker, static_cast<const char *>(filter), static_cast<const char *>(endpoint), &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(filter);

	free(endpoint);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsBrokerAddDestinationFilterToEndpoint(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 3){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsBrokerAddDestinationFilterToEndpoint:rhs","This function requires 3 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsBrokerAddDestinationFilterToEndpoint:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsBroker broker = *(static_cast<HelicsBroker*>(mxGetData(argv[0])));

	if(!mxIsChar(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsBrokerAddDestinationFilterToEndpoint:TypeError","Argument 2 must be a string.");
	}
	char *filter = nullptr;
	size_t filterLength = 0;
	int filterStatus = 0;
	if(argc > 1){
		filterLength = mxGetN(argv[1]) + 1;
		filter = static_cast<char *>(malloc(filterLength));
		filterStatus = mxGetString(argv[1], filter, filterLength);

	}
	if(!mxIsChar(argv[2])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsBrokerAddDestinationFilterToEndpoint:TypeError","Argument 3 must be a string.");
	}
	char *endpoint = nullptr;
	size_t endpointLength = 0;
	int endpointStatus = 0;
	if(argc > 2){
		endpointLength = mxGetN(argv[2]) + 1;
		endpoint = static_cast<char *>(malloc(endpointLength));
		endpointStatus = mxGetString(argv[2], endpoint, endpointLength);

	}
	HelicsError err = helicsErrorInitialize();

	helicsBrokerAddDestinationFilterToEndpoint(broker, static_cast<const char *>(filter), static_cast<const char *>(endpoint), &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(filter);

	free(endpoint);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsBrokerMakeConnections(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsBrokerMakeConnections:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsBrokerMakeConnections:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsBroker broker = *(static_cast<HelicsBroker*>(mxGetData(argv[0])));

	if(!mxIsChar(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsBrokerMakeConnections:TypeError","Argument 2 must be a string.");
	}
	char *file = nullptr;
	size_t fileLength = 0;
	int fileStatus = 0;
	if(argc > 1){
		fileLength = mxGetN(argv[1]) + 1;
		file = static_cast<char *>(malloc(fileLength));
		fileStatus = mxGetString(argv[1], file, fileLength);

	}
	HelicsError err = helicsErrorInitialize();

	helicsBrokerMakeConnections(broker, static_cast<const char *>(file), &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(file);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsCoreWaitForDisconnect(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsCoreWaitForDisconnect:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsCoreWaitForDisconnect:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsCore core = *(static_cast<HelicsCore*>(mxGetData(argv[0])));

	if(!mxIsNumeric(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsCoreWaitForDisconnect:TypeError","Argument 2 must be of type integer.");
	}
		int msToWait = static_cast<int>(mxGetScalar(argv[1]));

	HelicsError err = helicsErrorInitialize();

	HelicsBool result = helicsCoreWaitForDisconnect(core, msToWait, &err);

	mxArray *_out = mxCreateLogicalMatrix(1, 1);
	mxLogical *rv = mxGetLogicals(_out);
	if(result == HELICS_TRUE){
		rv[0] = true;
	}else{
		rv[0] = false;
	}

	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsBrokerWaitForDisconnect(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsBrokerWaitForDisconnect:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsBrokerWaitForDisconnect:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsBroker broker = *(static_cast<HelicsBroker*>(mxGetData(argv[0])));

	if(!mxIsNumeric(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsBrokerWaitForDisconnect:TypeError","Argument 2 must be of type integer.");
	}
		int msToWait = static_cast<int>(mxGetScalar(argv[1]));

	HelicsError err = helicsErrorInitialize();

	HelicsBool result = helicsBrokerWaitForDisconnect(broker, msToWait, &err);

	mxArray *_out = mxCreateLogicalMatrix(1, 1);
	mxLogical *rv = mxGetLogicals(_out);
	if(result == HELICS_TRUE){
		rv[0] = true;
	}else{
		rv[0] = false;
	}

	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsCoreIsConnected(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsCoreIsConnected:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsCoreIsConnected:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsCore core = *(static_cast<HelicsCore*>(mxGetData(argv[0])));

	HelicsBool result = helicsCoreIsConnected(core);

	mxArray *_out = mxCreateLogicalMatrix(1, 1);
	mxLogical *rv = mxGetLogicals(_out);
	if(result == HELICS_TRUE){
		rv[0] = true;
	}else{
		rv[0] = false;
	}

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsCoreDataLink(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 3){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsCoreDataLink:rhs","This function requires 3 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsCoreDataLink:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsCore core = *(static_cast<HelicsCore*>(mxGetData(argv[0])));

	if(!mxIsChar(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsCoreDataLink:TypeError","Argument 2 must be a string.");
	}
	char *source = nullptr;
	size_t sourceLength = 0;
	int sourceStatus = 0;
	if(argc > 1){
		sourceLength = mxGetN(argv[1]) + 1;
		source = static_cast<char *>(malloc(sourceLength));
		sourceStatus = mxGetString(argv[1], source, sourceLength);

	}
	if(!mxIsChar(argv[2])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsCoreDataLink:TypeError","Argument 3 must be a string.");
	}
	char *target = nullptr;
	size_t targetLength = 0;
	int targetStatus = 0;
	if(argc > 2){
		targetLength = mxGetN(argv[2]) + 1;
		target = static_cast<char *>(malloc(targetLength));
		targetStatus = mxGetString(argv[2], target, targetLength);

	}
	HelicsError err = helicsErrorInitialize();

	helicsCoreDataLink(core, static_cast<const char *>(source), static_cast<const char *>(target), &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(source);

	free(target);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsCoreAddSourceFilterToEndpoint(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 3){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsCoreAddSourceFilterToEndpoint:rhs","This function requires 3 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsCoreAddSourceFilterToEndpoint:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsCore core = *(static_cast<HelicsCore*>(mxGetData(argv[0])));

	if(!mxIsChar(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsCoreAddSourceFilterToEndpoint:TypeError","Argument 2 must be a string.");
	}
	char *filter = nullptr;
	size_t filterLength = 0;
	int filterStatus = 0;
	if(argc > 1){
		filterLength = mxGetN(argv[1]) + 1;
		filter = static_cast<char *>(malloc(filterLength));
		filterStatus = mxGetString(argv[1], filter, filterLength);

	}
	if(!mxIsChar(argv[2])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsCoreAddSourceFilterToEndpoint:TypeError","Argument 3 must be a string.");
	}
	char *endpoint = nullptr;
	size_t endpointLength = 0;
	int endpointStatus = 0;
	if(argc > 2){
		endpointLength = mxGetN(argv[2]) + 1;
		endpoint = static_cast<char *>(malloc(endpointLength));
		endpointStatus = mxGetString(argv[2], endpoint, endpointLength);

	}
	HelicsError err = helicsErrorInitialize();

	helicsCoreAddSourceFilterToEndpoint(core, static_cast<const char *>(filter), static_cast<const char *>(endpoint), &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(filter);

	free(endpoint);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsCoreAddDestinationFilterToEndpoint(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 3){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsCoreAddDestinationFilterToEndpoint:rhs","This function requires 3 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsCoreAddDestinationFilterToEndpoint:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsCore core = *(static_cast<HelicsCore*>(mxGetData(argv[0])));

	if(!mxIsChar(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsCoreAddDestinationFilterToEndpoint:TypeError","Argument 2 must be a string.");
	}
	char *filter = nullptr;
	size_t filterLength = 0;
	int filterStatus = 0;
	if(argc > 1){
		filterLength = mxGetN(argv[1]) + 1;
		filter = static_cast<char *>(malloc(filterLength));
		filterStatus = mxGetString(argv[1], filter, filterLength);

	}
	if(!mxIsChar(argv[2])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsCoreAddDestinationFilterToEndpoint:TypeError","Argument 3 must be a string.");
	}
	char *endpoint = nullptr;
	size_t endpointLength = 0;
	int endpointStatus = 0;
	if(argc > 2){
		endpointLength = mxGetN(argv[2]) + 1;
		endpoint = static_cast<char *>(malloc(endpointLength));
		endpointStatus = mxGetString(argv[2], endpoint, endpointLength);

	}
	HelicsError err = helicsErrorInitialize();

	helicsCoreAddDestinationFilterToEndpoint(core, static_cast<const char *>(filter), static_cast<const char *>(endpoint), &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(filter);

	free(endpoint);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsCoreMakeConnections(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsCoreMakeConnections:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsCoreMakeConnections:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsCore core = *(static_cast<HelicsCore*>(mxGetData(argv[0])));

	if(!mxIsChar(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsCoreMakeConnections:TypeError","Argument 2 must be a string.");
	}
	char *file = nullptr;
	size_t fileLength = 0;
	int fileStatus = 0;
	if(argc > 1){
		fileLength = mxGetN(argv[1]) + 1;
		file = static_cast<char *>(malloc(fileLength));
		fileStatus = mxGetString(argv[1], file, fileLength);

	}
	HelicsError err = helicsErrorInitialize();

	helicsCoreMakeConnections(core, static_cast<const char *>(file), &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(file);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsBrokerGetIdentifier(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsBrokerGetIdentifier:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsBrokerGetIdentifier:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsBroker broker = *(static_cast<HelicsBroker*>(mxGetData(argv[0])));

	const char *result = helicsBrokerGetIdentifier(broker);

	mxArray *_out = mxCreateString(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsCoreGetIdentifier(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsCoreGetIdentifier:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsCoreGetIdentifier:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsCore core = *(static_cast<HelicsCore*>(mxGetData(argv[0])));

	const char *result = helicsCoreGetIdentifier(core);

	mxArray *_out = mxCreateString(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsBrokerGetAddress(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsBrokerGetAddress:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsBrokerGetAddress:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsBroker broker = *(static_cast<HelicsBroker*>(mxGetData(argv[0])));

	const char *result = helicsBrokerGetAddress(broker);

	mxArray *_out = mxCreateString(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsCoreGetAddress(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsCoreGetAddress:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsCoreGetAddress:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsCore core = *(static_cast<HelicsCore*>(mxGetData(argv[0])));

	const char *result = helicsCoreGetAddress(core);

	mxArray *_out = mxCreateString(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsCoreSetReadyToInit(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsCoreSetReadyToInit:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsCoreSetReadyToInit:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsCore core = *(static_cast<HelicsCore*>(mxGetData(argv[0])));

	HelicsError err = helicsErrorInitialize();

	helicsCoreSetReadyToInit(core, &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsCoreConnect(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsCoreConnect:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsCoreConnect:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsCore core = *(static_cast<HelicsCore*>(mxGetData(argv[0])));

	HelicsError err = helicsErrorInitialize();

	HelicsBool result = helicsCoreConnect(core, &err);

	mxArray *_out = mxCreateLogicalMatrix(1, 1);
	mxLogical *rv = mxGetLogicals(_out);
	if(result == HELICS_TRUE){
		rv[0] = true;
	}else{
		rv[0] = false;
	}

	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsCoreDisconnect(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsCoreDisconnect:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsCoreDisconnect:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsCore core = *(static_cast<HelicsCore*>(mxGetData(argv[0])));

	HelicsError err = helicsErrorInitialize();

	helicsCoreDisconnect(core, &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsGetFederateByName(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsGetFederateByName:rhs","This function requires 1 arguments.");
	}

	if(!mxIsChar(argv[0])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsGetFederateByName:TypeError","Argument 1 must be a string.");
	}
	char *fedName = nullptr;
	size_t fedNameLength = 0;
	int fedNameStatus = 0;
	if(argc > 0){
		fedNameLength = mxGetN(argv[0]) + 1;
		fedName = static_cast<char *>(malloc(fedNameLength));
		fedNameStatus = mxGetString(argv[0], fedName, fedNameLength);

	}
	HelicsError err = helicsErrorInitialize();

	HelicsFederate result = helicsGetFederateByName(static_cast<const char *>(fedName), &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	mxUint64 *rv = mxGetUint64s(_out);
	rv[0] = reinterpret_cast<mxUint64>(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(fedName);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsBrokerDisconnect(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsBrokerDisconnect:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsBrokerDisconnect:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsBroker broker = *(static_cast<HelicsBroker*>(mxGetData(argv[0])));

	HelicsError err = helicsErrorInitialize();

	helicsBrokerDisconnect(broker, &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateDestroy(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateDestroy:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateDestroy:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFederate fed = *(static_cast<HelicsFederate*>(mxGetData(argv[0])));

	helicsFederateDestroy(fed);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsBrokerDestroy(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsBrokerDestroy:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsBrokerDestroy:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsBroker broker = *(static_cast<HelicsBroker*>(mxGetData(argv[0])));

	helicsBrokerDestroy(broker);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsCoreDestroy(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsCoreDestroy:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsCoreDestroy:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsCore core = *(static_cast<HelicsCore*>(mxGetData(argv[0])));

	helicsCoreDestroy(core);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsCoreFree(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsCoreFree:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsCoreFree:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsCore core = *(static_cast<HelicsCore*>(mxGetData(argv[0])));

	helicsCoreFree(core);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsBrokerFree(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsBrokerFree:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsBrokerFree:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsBroker broker = *(static_cast<HelicsBroker*>(mxGetData(argv[0])));

	helicsBrokerFree(broker);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsCreateValueFederate(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsCreateValueFederate:rhs","This function requires 2 arguments.");
	}

	if(!mxIsChar(argv[0])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsCreateValueFederate:TypeError","Argument 1 must be a string.");
	}
	char *fedName = nullptr;
	size_t fedNameLength = 0;
	int fedNameStatus = 0;
	if(argc > 0){
		fedNameLength = mxGetN(argv[0]) + 1;
		fedName = static_cast<char *>(malloc(fedNameLength));
		fedNameStatus = mxGetString(argv[0], fedName, fedNameLength);

	}
	if(mxGetClassID(argv[1]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsCreateValueFederate:TypeError","Argument 2 must be of type uint64.");
	}
	HelicsFederateInfo fi = *(static_cast<HelicsFederateInfo*>(mxGetData(argv[1])));

	HelicsError err = helicsErrorInitialize();

	HelicsFederate result = helicsCreateValueFederate(static_cast<const char *>(fedName), fi, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	mxUint64 *rv = mxGetUint64s(_out);
	rv[0] = reinterpret_cast<mxUint64>(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(fedName);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsCreateValueFederateFromConfig(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsCreateValueFederateFromConfig:rhs","This function requires 1 arguments.");
	}

	if(!mxIsChar(argv[0])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsCreateValueFederateFromConfig:TypeError","Argument 1 must be a string.");
	}
	char *configFile = nullptr;
	size_t configFileLength = 0;
	int configFileStatus = 0;
	if(argc > 0){
		configFileLength = mxGetN(argv[0]) + 1;
		configFile = static_cast<char *>(malloc(configFileLength));
		configFileStatus = mxGetString(argv[0], configFile, configFileLength);

	}
	HelicsError err = helicsErrorInitialize();

	HelicsFederate result = helicsCreateValueFederateFromConfig(static_cast<const char *>(configFile), &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	mxUint64 *rv = mxGetUint64s(_out);
	rv[0] = reinterpret_cast<mxUint64>(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(configFile);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsCreateMessageFederate(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsCreateMessageFederate:rhs","This function requires 2 arguments.");
	}

	if(!mxIsChar(argv[0])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsCreateMessageFederate:TypeError","Argument 1 must be a string.");
	}
	char *fedName = nullptr;
	size_t fedNameLength = 0;
	int fedNameStatus = 0;
	if(argc > 0){
		fedNameLength = mxGetN(argv[0]) + 1;
		fedName = static_cast<char *>(malloc(fedNameLength));
		fedNameStatus = mxGetString(argv[0], fedName, fedNameLength);

	}
	if(mxGetClassID(argv[1]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsCreateMessageFederate:TypeError","Argument 2 must be of type uint64.");
	}
	HelicsFederateInfo fi = *(static_cast<HelicsFederateInfo*>(mxGetData(argv[1])));

	HelicsError err = helicsErrorInitialize();

	HelicsFederate result = helicsCreateMessageFederate(static_cast<const char *>(fedName), fi, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	mxUint64 *rv = mxGetUint64s(_out);
	rv[0] = reinterpret_cast<mxUint64>(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(fedName);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsCreateMessageFederateFromConfig(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsCreateMessageFederateFromConfig:rhs","This function requires 1 arguments.");
	}

	if(!mxIsChar(argv[0])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsCreateMessageFederateFromConfig:TypeError","Argument 1 must be a string.");
	}
	char *configFile = nullptr;
	size_t configFileLength = 0;
	int configFileStatus = 0;
	if(argc > 0){
		configFileLength = mxGetN(argv[0]) + 1;
		configFile = static_cast<char *>(malloc(configFileLength));
		configFileStatus = mxGetString(argv[0], configFile, configFileLength);

	}
	HelicsError err = helicsErrorInitialize();

	HelicsFederate result = helicsCreateMessageFederateFromConfig(static_cast<const char *>(configFile), &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	mxUint64 *rv = mxGetUint64s(_out);
	rv[0] = reinterpret_cast<mxUint64>(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(configFile);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsCreateCombinationFederate(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsCreateCombinationFederate:rhs","This function requires 2 arguments.");
	}

	if(!mxIsChar(argv[0])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsCreateCombinationFederate:TypeError","Argument 1 must be a string.");
	}
	char *fedName = nullptr;
	size_t fedNameLength = 0;
	int fedNameStatus = 0;
	if(argc > 0){
		fedNameLength = mxGetN(argv[0]) + 1;
		fedName = static_cast<char *>(malloc(fedNameLength));
		fedNameStatus = mxGetString(argv[0], fedName, fedNameLength);

	}
	if(mxGetClassID(argv[1]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsCreateCombinationFederate:TypeError","Argument 2 must be of type uint64.");
	}
	HelicsFederateInfo fi = *(static_cast<HelicsFederateInfo*>(mxGetData(argv[1])));

	HelicsError err = helicsErrorInitialize();

	HelicsFederate result = helicsCreateCombinationFederate(static_cast<const char *>(fedName), fi, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	mxUint64 *rv = mxGetUint64s(_out);
	rv[0] = reinterpret_cast<mxUint64>(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(fedName);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsCreateCombinationFederateFromConfig(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsCreateCombinationFederateFromConfig:rhs","This function requires 1 arguments.");
	}

	if(!mxIsChar(argv[0])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsCreateCombinationFederateFromConfig:TypeError","Argument 1 must be a string.");
	}
	char *configFile = nullptr;
	size_t configFileLength = 0;
	int configFileStatus = 0;
	if(argc > 0){
		configFileLength = mxGetN(argv[0]) + 1;
		configFile = static_cast<char *>(malloc(configFileLength));
		configFileStatus = mxGetString(argv[0], configFile, configFileLength);

	}
	HelicsError err = helicsErrorInitialize();

	HelicsFederate result = helicsCreateCombinationFederateFromConfig(static_cast<const char *>(configFile), &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	mxUint64 *rv = mxGetUint64s(_out);
	rv[0] = reinterpret_cast<mxUint64>(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(configFile);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsCreateCallbackFederate(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsCreateCallbackFederate:rhs","This function requires 2 arguments.");
	}

	if(!mxIsChar(argv[0])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsCreateCallbackFederate:TypeError","Argument 1 must be a string.");
	}
	char *fedName = nullptr;
	size_t fedNameLength = 0;
	int fedNameStatus = 0;
	if(argc > 0){
		fedNameLength = mxGetN(argv[0]) + 1;
		fedName = static_cast<char *>(malloc(fedNameLength));
		fedNameStatus = mxGetString(argv[0], fedName, fedNameLength);

	}
	if(mxGetClassID(argv[1]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsCreateCallbackFederate:TypeError","Argument 2 must be of type uint64.");
	}
	HelicsFederateInfo fi = *(static_cast<HelicsFederateInfo*>(mxGetData(argv[1])));

	HelicsError err = helicsErrorInitialize();

	HelicsFederate result = helicsCreateCallbackFederate(static_cast<const char *>(fedName), fi, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	mxUint64 *rv = mxGetUint64s(_out);
	rv[0] = reinterpret_cast<mxUint64>(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(fedName);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsCreateCallbackFederateFromConfig(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsCreateCallbackFederateFromConfig:rhs","This function requires 1 arguments.");
	}

	if(!mxIsChar(argv[0])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsCreateCallbackFederateFromConfig:TypeError","Argument 1 must be a string.");
	}
	char *configFile = nullptr;
	size_t configFileLength = 0;
	int configFileStatus = 0;
	if(argc > 0){
		configFileLength = mxGetN(argv[0]) + 1;
		configFile = static_cast<char *>(malloc(configFileLength));
		configFileStatus = mxGetString(argv[0], configFile, configFileLength);

	}
	HelicsError err = helicsErrorInitialize();

	HelicsFederate result = helicsCreateCallbackFederateFromConfig(static_cast<const char *>(configFile), &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	mxUint64 *rv = mxGetUint64s(_out);
	rv[0] = reinterpret_cast<mxUint64>(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(configFile);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateClone(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateClone:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateClone:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFederate fed = *(static_cast<HelicsFederate*>(mxGetData(argv[0])));

	HelicsError err = helicsErrorInitialize();

	HelicsFederate result = helicsFederateClone(fed, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	mxUint64 *rv = mxGetUint64s(_out);
	rv[0] = reinterpret_cast<mxUint64>(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateProtect(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateProtect:rhs","This function requires 1 arguments.");
	}

	if(!mxIsChar(argv[0])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateProtect:TypeError","Argument 1 must be a string.");
	}
	char *fedName = nullptr;
	size_t fedNameLength = 0;
	int fedNameStatus = 0;
	if(argc > 0){
		fedNameLength = mxGetN(argv[0]) + 1;
		fedName = static_cast<char *>(malloc(fedNameLength));
		fedNameStatus = mxGetString(argv[0], fedName, fedNameLength);

	}
	HelicsError err = helicsErrorInitialize();

	helicsFederateProtect(static_cast<const char *>(fedName), &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(fedName);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateUnProtect(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateUnProtect:rhs","This function requires 1 arguments.");
	}

	if(!mxIsChar(argv[0])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateUnProtect:TypeError","Argument 1 must be a string.");
	}
	char *fedName = nullptr;
	size_t fedNameLength = 0;
	int fedNameStatus = 0;
	if(argc > 0){
		fedNameLength = mxGetN(argv[0]) + 1;
		fedName = static_cast<char *>(malloc(fedNameLength));
		fedNameStatus = mxGetString(argv[0], fedName, fedNameLength);

	}
	HelicsError err = helicsErrorInitialize();

	helicsFederateUnProtect(static_cast<const char *>(fedName), &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(fedName);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateIsProtected(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateIsProtected:rhs","This function requires 1 arguments.");
	}

	if(!mxIsChar(argv[0])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateIsProtected:TypeError","Argument 1 must be a string.");
	}
	char *fedName = nullptr;
	size_t fedNameLength = 0;
	int fedNameStatus = 0;
	if(argc > 0){
		fedNameLength = mxGetN(argv[0]) + 1;
		fedName = static_cast<char *>(malloc(fedNameLength));
		fedNameStatus = mxGetString(argv[0], fedName, fedNameLength);

	}
	HelicsError err = helicsErrorInitialize();

	HelicsBool result = helicsFederateIsProtected(static_cast<const char *>(fedName), &err);

	mxArray *_out = mxCreateLogicalMatrix(1, 1);
	mxLogical *rv = mxGetLogicals(_out);
	if(result == HELICS_TRUE){
		rv[0] = true;
	}else{
		rv[0] = false;
	}

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(fedName);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsCreateFederateInfo(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 0){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsCreateFederateInfo:rhs","This function requires 0 arguments.");
	}

	HelicsFederateInfo result = helicsCreateFederateInfo();

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	mxUint64 *rv = mxGetUint64s(_out);
	rv[0] = reinterpret_cast<mxUint64>(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsFederateInfoClone(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateInfoClone:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateInfoClone:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFederateInfo fi = *(static_cast<HelicsFederateInfo*>(mxGetData(argv[0])));

	HelicsError err = helicsErrorInitialize();

	HelicsFederateInfo result = helicsFederateInfoClone(fi, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	mxUint64 *rv = mxGetUint64s(_out);
	rv[0] = reinterpret_cast<mxUint64>(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateInfoLoadFromArgs(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateInfoLoadFromArgs:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateInfoLoadFromArgs:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFederateInfo fi = *(static_cast<HelicsFederateInfo*>(mxGetData(argv[0])));

	if(!mxIsCell(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateInfoLoadFromArgs:TypeError","Argument 2 must be of cell array of strings.");
	}
	int arg1 = 0;
	char **arg2;
	int ii;
	arg1 = static_cast<int>(mxGetNumberOfElements(argv[1]));
	arg2 = static_cast<char **>(malloc((arg1)*sizeof(char *)));
	for (ii=0;ii<arg1;ii++){
		mxArray *cellElement=mxGetCell(argv[1], ii);
		size_t len = mxGetN(cellElement) + 1;
		arg2[ii] = static_cast<char *>(malloc(static_cast<int>(len)));
		int flag = mxGetString(cellElement, arg2[ii], static_cast<int>(len));
	}

	HelicsError err = helicsErrorInitialize();

	helicsFederateInfoLoadFromArgs(fi, arg1, arg2, &err);

	mxArray *_out = nullptr;
	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(arg2);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateInfoLoadFromString(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateInfoLoadFromString:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateInfoLoadFromString:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFederateInfo fi = *(static_cast<HelicsFederateInfo*>(mxGetData(argv[0])));

	if(!mxIsChar(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateInfoLoadFromString:TypeError","Argument 2 must be a string.");
	}
	char *args = nullptr;
	size_t argsLength = 0;
	int argsStatus = 0;
	if(argc > 1){
		argsLength = mxGetN(argv[1]) + 1;
		args = static_cast<char *>(malloc(argsLength));
		argsStatus = mxGetString(argv[1], args, argsLength);

	}
	HelicsError err = helicsErrorInitialize();

	helicsFederateInfoLoadFromString(fi, static_cast<const char *>(args), &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(args);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateInfoFree(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateInfoFree:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateInfoFree:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFederateInfo fi = *(static_cast<HelicsFederateInfo*>(mxGetData(argv[0])));

	helicsFederateInfoFree(fi);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsFederateIsValid(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateIsValid:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateIsValid:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFederate fed = *(static_cast<HelicsFederate*>(mxGetData(argv[0])));

	HelicsBool result = helicsFederateIsValid(fed);

	mxArray *_out = mxCreateLogicalMatrix(1, 1);
	mxLogical *rv = mxGetLogicals(_out);
	if(result == HELICS_TRUE){
		rv[0] = true;
	}else{
		rv[0] = false;
	}

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsFederateInfoSetCoreName(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateInfoSetCoreName:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateInfoSetCoreName:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFederateInfo fi = *(static_cast<HelicsFederateInfo*>(mxGetData(argv[0])));

	if(!mxIsChar(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateInfoSetCoreName:TypeError","Argument 2 must be a string.");
	}
	char *corename = nullptr;
	size_t corenameLength = 0;
	int corenameStatus = 0;
	if(argc > 1){
		corenameLength = mxGetN(argv[1]) + 1;
		corename = static_cast<char *>(malloc(corenameLength));
		corenameStatus = mxGetString(argv[1], corename, corenameLength);

	}
	HelicsError err = helicsErrorInitialize();

	helicsFederateInfoSetCoreName(fi, static_cast<const char *>(corename), &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(corename);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateInfoSetCoreInitString(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateInfoSetCoreInitString:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateInfoSetCoreInitString:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFederateInfo fi = *(static_cast<HelicsFederateInfo*>(mxGetData(argv[0])));

	if(!mxIsChar(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateInfoSetCoreInitString:TypeError","Argument 2 must be a string.");
	}
	char *coreInit = nullptr;
	size_t coreInitLength = 0;
	int coreInitStatus = 0;
	if(argc > 1){
		coreInitLength = mxGetN(argv[1]) + 1;
		coreInit = static_cast<char *>(malloc(coreInitLength));
		coreInitStatus = mxGetString(argv[1], coreInit, coreInitLength);

	}
	HelicsError err = helicsErrorInitialize();

	helicsFederateInfoSetCoreInitString(fi, static_cast<const char *>(coreInit), &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(coreInit);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateInfoSetBrokerInitString(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateInfoSetBrokerInitString:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateInfoSetBrokerInitString:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFederateInfo fi = *(static_cast<HelicsFederateInfo*>(mxGetData(argv[0])));

	if(!mxIsChar(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateInfoSetBrokerInitString:TypeError","Argument 2 must be a string.");
	}
	char *brokerInit = nullptr;
	size_t brokerInitLength = 0;
	int brokerInitStatus = 0;
	if(argc > 1){
		brokerInitLength = mxGetN(argv[1]) + 1;
		brokerInit = static_cast<char *>(malloc(brokerInitLength));
		brokerInitStatus = mxGetString(argv[1], brokerInit, brokerInitLength);

	}
	HelicsError err = helicsErrorInitialize();

	helicsFederateInfoSetBrokerInitString(fi, static_cast<const char *>(brokerInit), &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(brokerInit);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateInfoSetCoreType(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateInfoSetCoreType:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateInfoSetCoreType:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFederateInfo fi = *(static_cast<HelicsFederateInfo*>(mxGetData(argv[0])));

	if(!mxIsNumeric(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateInfoSetCoreType:TypeError","Argument 2 must be of type integer.");
	}
		int coretype = static_cast<int>(mxGetScalar(argv[1]));

	HelicsError err = helicsErrorInitialize();

	helicsFederateInfoSetCoreType(fi, coretype, &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateInfoSetCoreTypeFromString(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateInfoSetCoreTypeFromString:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateInfoSetCoreTypeFromString:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFederateInfo fi = *(static_cast<HelicsFederateInfo*>(mxGetData(argv[0])));

	if(!mxIsChar(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateInfoSetCoreTypeFromString:TypeError","Argument 2 must be a string.");
	}
	char *coretype = nullptr;
	size_t coretypeLength = 0;
	int coretypeStatus = 0;
	if(argc > 1){
		coretypeLength = mxGetN(argv[1]) + 1;
		coretype = static_cast<char *>(malloc(coretypeLength));
		coretypeStatus = mxGetString(argv[1], coretype, coretypeLength);

	}
	HelicsError err = helicsErrorInitialize();

	helicsFederateInfoSetCoreTypeFromString(fi, static_cast<const char *>(coretype), &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(coretype);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateInfoSetBroker(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateInfoSetBroker:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateInfoSetBroker:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFederateInfo fi = *(static_cast<HelicsFederateInfo*>(mxGetData(argv[0])));

	if(!mxIsChar(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateInfoSetBroker:TypeError","Argument 2 must be a string.");
	}
	char *broker = nullptr;
	size_t brokerLength = 0;
	int brokerStatus = 0;
	if(argc > 1){
		brokerLength = mxGetN(argv[1]) + 1;
		broker = static_cast<char *>(malloc(brokerLength));
		brokerStatus = mxGetString(argv[1], broker, brokerLength);

	}
	HelicsError err = helicsErrorInitialize();

	helicsFederateInfoSetBroker(fi, static_cast<const char *>(broker), &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(broker);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateInfoSetBrokerKey(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateInfoSetBrokerKey:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateInfoSetBrokerKey:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFederateInfo fi = *(static_cast<HelicsFederateInfo*>(mxGetData(argv[0])));

	if(!mxIsChar(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateInfoSetBrokerKey:TypeError","Argument 2 must be a string.");
	}
	char *brokerkey = nullptr;
	size_t brokerkeyLength = 0;
	int brokerkeyStatus = 0;
	if(argc > 1){
		brokerkeyLength = mxGetN(argv[1]) + 1;
		brokerkey = static_cast<char *>(malloc(brokerkeyLength));
		brokerkeyStatus = mxGetString(argv[1], brokerkey, brokerkeyLength);

	}
	HelicsError err = helicsErrorInitialize();

	helicsFederateInfoSetBrokerKey(fi, static_cast<const char *>(brokerkey), &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(brokerkey);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateInfoSetBrokerPort(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateInfoSetBrokerPort:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateInfoSetBrokerPort:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFederateInfo fi = *(static_cast<HelicsFederateInfo*>(mxGetData(argv[0])));

	if(!mxIsNumeric(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateInfoSetBrokerPort:TypeError","Argument 2 must be of type integer.");
	}
		int brokerPort = static_cast<int>(mxGetScalar(argv[1]));

	HelicsError err = helicsErrorInitialize();

	helicsFederateInfoSetBrokerPort(fi, brokerPort, &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateInfoSetLocalPort(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateInfoSetLocalPort:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateInfoSetLocalPort:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFederateInfo fi = *(static_cast<HelicsFederateInfo*>(mxGetData(argv[0])));

	if(!mxIsChar(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateInfoSetLocalPort:TypeError","Argument 2 must be a string.");
	}
	char *localPort = nullptr;
	size_t localPortLength = 0;
	int localPortStatus = 0;
	if(argc > 1){
		localPortLength = mxGetN(argv[1]) + 1;
		localPort = static_cast<char *>(malloc(localPortLength));
		localPortStatus = mxGetString(argv[1], localPort, localPortLength);

	}
	HelicsError err = helicsErrorInitialize();

	helicsFederateInfoSetLocalPort(fi, static_cast<const char *>(localPort), &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(localPort);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsGetPropertyIndex(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc < 0 || argc > 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsGetPropertyIndex:rhs","This function requires at least 0 arguments and at most 1 arguments.");
	}

	if(argc > 0){
		if(!mxIsChar(argv[0])){
			mexUnlock();
			mexErrMsgIdAndTxt("MATLAB:helicsGetPropertyIndex:TypeError","Argument 1 must be a string.");
		}
	}
	char *val = nullptr;
	size_t valLength = 0;
	int valStatus = 0;
	if(argc > 0){
		valLength = mxGetN(argv[0]) + 1;
		val = static_cast<char *>(malloc(valLength));
		valStatus = mxGetString(argv[0], val, valLength);

	}
	int result = helicsGetPropertyIndex(static_cast<const char *>(val));

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
	mxInt64 *rv = mxGetInt64s(_out);
	rv[0] = static_cast<mxInt64>(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(val);
}


void _wrap_helicsGetFlagIndex(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc < 0 || argc > 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsGetFlagIndex:rhs","This function requires at least 0 arguments and at most 1 arguments.");
	}

	if(argc > 0){
		if(!mxIsChar(argv[0])){
			mexUnlock();
			mexErrMsgIdAndTxt("MATLAB:helicsGetFlagIndex:TypeError","Argument 1 must be a string.");
		}
	}
	char *val = nullptr;
	size_t valLength = 0;
	int valStatus = 0;
	if(argc > 0){
		valLength = mxGetN(argv[0]) + 1;
		val = static_cast<char *>(malloc(valLength));
		valStatus = mxGetString(argv[0], val, valLength);

	}
	int result = helicsGetFlagIndex(static_cast<const char *>(val));

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
	mxInt64 *rv = mxGetInt64s(_out);
	rv[0] = static_cast<mxInt64>(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(val);
}


void _wrap_helicsGetOptionIndex(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc < 0 || argc > 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsGetOptionIndex:rhs","This function requires at least 0 arguments and at most 1 arguments.");
	}

	if(argc > 0){
		if(!mxIsChar(argv[0])){
			mexUnlock();
			mexErrMsgIdAndTxt("MATLAB:helicsGetOptionIndex:TypeError","Argument 1 must be a string.");
		}
	}
	char *val = nullptr;
	size_t valLength = 0;
	int valStatus = 0;
	if(argc > 0){
		valLength = mxGetN(argv[0]) + 1;
		val = static_cast<char *>(malloc(valLength));
		valStatus = mxGetString(argv[0], val, valLength);

	}
	int result = helicsGetOptionIndex(static_cast<const char *>(val));

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
	mxInt64 *rv = mxGetInt64s(_out);
	rv[0] = static_cast<mxInt64>(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(val);
}


void _wrap_helicsGetOptionValue(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc < 0 || argc > 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsGetOptionValue:rhs","This function requires at least 0 arguments and at most 1 arguments.");
	}

	if(argc > 0){
		if(!mxIsChar(argv[0])){
			mexUnlock();
			mexErrMsgIdAndTxt("MATLAB:helicsGetOptionValue:TypeError","Argument 1 must be a string.");
		}
	}
	char *val = nullptr;
	size_t valLength = 0;
	int valStatus = 0;
	if(argc > 0){
		valLength = mxGetN(argv[0]) + 1;
		val = static_cast<char *>(malloc(valLength));
		valStatus = mxGetString(argv[0], val, valLength);

	}
	int result = helicsGetOptionValue(static_cast<const char *>(val));

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
	mxInt64 *rv = mxGetInt64s(_out);
	rv[0] = static_cast<mxInt64>(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(val);
}


void _wrap_helicsGetDataType(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc < 0 || argc > 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsGetDataType:rhs","This function requires at least 0 arguments and at most 1 arguments.");
	}

	if(argc > 0){
		if(!mxIsChar(argv[0])){
			mexUnlock();
			mexErrMsgIdAndTxt("MATLAB:helicsGetDataType:TypeError","Argument 1 must be a string.");
		}
	}
	char *val = nullptr;
	size_t valLength = 0;
	int valStatus = 0;
	if(argc > 0){
		valLength = mxGetN(argv[0]) + 1;
		val = static_cast<char *>(malloc(valLength));
		valStatus = mxGetString(argv[0], val, valLength);

	}
	int result = helicsGetDataType(static_cast<const char *>(val));

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
	mxInt64 *rv = mxGetInt64s(_out);
	rv[0] = static_cast<mxInt64>(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(val);
}


void _wrap_helicsFederateInfoSetFlagOption(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 3){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateInfoSetFlagOption:rhs","This function requires 3 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateInfoSetFlagOption:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFederateInfo fi = *(static_cast<HelicsFederateInfo*>(mxGetData(argv[0])));

	if(!mxIsNumeric(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateInfoSetFlagOption:TypeError","Argument 2 must be of type integer.");
	}
		int flag = static_cast<int>(mxGetScalar(argv[1]));

	if(!mxIsLogical(argv[2])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateInfoSetFlagOption:TypeError","Argument 3 must be a 0 or 1.");
	}
	mxLogical *pvalue = mxGetLogicals(argv[2]);
	HelicsBool value = HELICS_FALSE;
	if(pvalue[0]){
		value = HELICS_TRUE;
	}

	HelicsError err = helicsErrorInitialize();

	helicsFederateInfoSetFlagOption(fi, flag, value, &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateInfoSetSeparator(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateInfoSetSeparator:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateInfoSetSeparator:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFederateInfo fi = *(static_cast<HelicsFederateInfo*>(mxGetData(argv[0])));

	if(!mxIsChar(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateInfoSetSeparator:TypeError","Argument 2 must be a string.");
	}
	char *separator = nullptr;
	size_t separatorLength = 0;
	int separatorStatus = 0;
	if(argc > 1){
		separatorLength = mxGetN(argv[1]) + 1;
		separator = static_cast<char *>(malloc(separatorLength));
		separatorStatus = mxGetString(argv[1], separator, separatorLength);

	}
	HelicsError err = helicsErrorInitialize();

	helicsFederateInfoSetSeparator(fi, separator[0], &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(separator);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateInfoSetTimeProperty(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 3){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateInfoSetTimeProperty:rhs","This function requires 3 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateInfoSetTimeProperty:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFederateInfo fi = *(static_cast<HelicsFederateInfo*>(mxGetData(argv[0])));

	if(!mxIsNumeric(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateInfoSetTimeProperty:TypeError","Argument 2 must be of type integer.");
	}
		int timeProperty = static_cast<int>(mxGetScalar(argv[1]));

	if(!mxIsNumeric(argv[2])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateInfoSetTimeProperty:TypeError","Argument 3 must be of type double.");
	}
	HelicsTime propertyValue = (HelicsTime)(mxGetScalar(argv[2]));

	HelicsError err = helicsErrorInitialize();

	helicsFederateInfoSetTimeProperty(fi, timeProperty, propertyValue, &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateInfoSetIntegerProperty(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 3){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateInfoSetIntegerProperty:rhs","This function requires 3 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateInfoSetIntegerProperty:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFederateInfo fi = *(static_cast<HelicsFederateInfo*>(mxGetData(argv[0])));

	if(!mxIsNumeric(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateInfoSetIntegerProperty:TypeError","Argument 2 must be of type integer.");
	}
		int intProperty = static_cast<int>(mxGetScalar(argv[1]));

	if(!mxIsNumeric(argv[2])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateInfoSetIntegerProperty:TypeError","Argument 3 must be of type integer.");
	}
		int propertyValue = static_cast<int>(mxGetScalar(argv[2]));

	HelicsError err = helicsErrorInitialize();

	helicsFederateInfoSetIntegerProperty(fi, intProperty, propertyValue, &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateRegisterInterfaces(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateRegisterInterfaces:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateRegisterInterfaces:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFederate fed = *(static_cast<HelicsFederate*>(mxGetData(argv[0])));

	if(!mxIsChar(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateRegisterInterfaces:TypeError","Argument 2 must be a string.");
	}
	char *file = nullptr;
	size_t fileLength = 0;
	int fileStatus = 0;
	if(argc > 1){
		fileLength = mxGetN(argv[1]) + 1;
		file = static_cast<char *>(malloc(fileLength));
		fileStatus = mxGetString(argv[1], file, fileLength);

	}
	HelicsError err = helicsErrorInitialize();

	helicsFederateRegisterInterfaces(fed, static_cast<const char *>(file), &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(file);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateGlobalError(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 3){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateGlobalError:rhs","This function requires 3 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateGlobalError:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFederate fed = *(static_cast<HelicsFederate*>(mxGetData(argv[0])));

	if(!mxIsNumeric(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateGlobalError:TypeError","Argument 2 must be of type integer.");
	}
		int errorCode = static_cast<int>(mxGetScalar(argv[1]));

	if(!mxIsChar(argv[2])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateGlobalError:TypeError","Argument 3 must be a string.");
	}
	char *errorString = nullptr;
	size_t errorStringLength = 0;
	int errorStringStatus = 0;
	if(argc > 2){
		errorStringLength = mxGetN(argv[2]) + 1;
		errorString = static_cast<char *>(malloc(errorStringLength));
		errorStringStatus = mxGetString(argv[2], errorString, errorStringLength);

	}
	HelicsError err = helicsErrorInitialize();

	helicsFederateGlobalError(fed, errorCode, static_cast<const char *>(errorString), &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(errorString);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateLocalError(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 3){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateLocalError:rhs","This function requires 3 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateLocalError:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFederate fed = *(static_cast<HelicsFederate*>(mxGetData(argv[0])));

	if(!mxIsNumeric(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateLocalError:TypeError","Argument 2 must be of type integer.");
	}
		int errorCode = static_cast<int>(mxGetScalar(argv[1]));

	if(!mxIsChar(argv[2])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateLocalError:TypeError","Argument 3 must be a string.");
	}
	char *errorString = nullptr;
	size_t errorStringLength = 0;
	int errorStringStatus = 0;
	if(argc > 2){
		errorStringLength = mxGetN(argv[2]) + 1;
		errorString = static_cast<char *>(malloc(errorStringLength));
		errorStringStatus = mxGetString(argv[2], errorString, errorStringLength);

	}
	HelicsError err = helicsErrorInitialize();

	helicsFederateLocalError(fed, errorCode, static_cast<const char *>(errorString), &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(errorString);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateFinalize(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateFinalize:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateFinalize:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFederate fed = *(static_cast<HelicsFederate*>(mxGetData(argv[0])));

	HelicsError err = helicsErrorInitialize();

	helicsFederateFinalize(fed, &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateFinalizeAsync(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateFinalizeAsync:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateFinalizeAsync:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFederate fed = *(static_cast<HelicsFederate*>(mxGetData(argv[0])));

	HelicsError err = helicsErrorInitialize();

	helicsFederateFinalizeAsync(fed, &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateFinalizeComplete(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateFinalizeComplete:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateFinalizeComplete:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFederate fed = *(static_cast<HelicsFederate*>(mxGetData(argv[0])));

	HelicsError err = helicsErrorInitialize();

	helicsFederateFinalizeComplete(fed, &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateDisconnect(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateDisconnect:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateDisconnect:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFederate fed = *(static_cast<HelicsFederate*>(mxGetData(argv[0])));

	HelicsError err = helicsErrorInitialize();

	helicsFederateDisconnect(fed, &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateDisconnectAsync(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateDisconnectAsync:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateDisconnectAsync:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFederate fed = *(static_cast<HelicsFederate*>(mxGetData(argv[0])));

	HelicsError err = helicsErrorInitialize();

	helicsFederateDisconnectAsync(fed, &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateDisconnectComplete(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateDisconnectComplete:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateDisconnectComplete:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFederate fed = *(static_cast<HelicsFederate*>(mxGetData(argv[0])));

	HelicsError err = helicsErrorInitialize();

	helicsFederateDisconnectComplete(fed, &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateFree(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateFree:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateFree:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFederate fed = *(static_cast<HelicsFederate*>(mxGetData(argv[0])));

	helicsFederateFree(fed);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsCloseLibrary(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 0){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsCloseLibrary:rhs","This function requires 0 arguments.");
	}

	helicsCloseLibrary();

	mxArray *_out = nullptr;
	if(_out){
		--resc;
		*resv++ = _out;
	}
	mexUnlock();
}


void _wrap_helicsFederateEnterInitializingMode(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateEnterInitializingMode:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateEnterInitializingMode:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFederate fed = *(static_cast<HelicsFederate*>(mxGetData(argv[0])));

	HelicsError err = helicsErrorInitialize();

	helicsFederateEnterInitializingMode(fed, &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateEnterInitializingModeAsync(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateEnterInitializingModeAsync:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateEnterInitializingModeAsync:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFederate fed = *(static_cast<HelicsFederate*>(mxGetData(argv[0])));

	HelicsError err = helicsErrorInitialize();

	helicsFederateEnterInitializingModeAsync(fed, &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateIsAsyncOperationCompleted(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateIsAsyncOperationCompleted:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateIsAsyncOperationCompleted:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFederate fed = *(static_cast<HelicsFederate*>(mxGetData(argv[0])));

	HelicsError err = helicsErrorInitialize();

	HelicsBool result = helicsFederateIsAsyncOperationCompleted(fed, &err);

	mxArray *_out = mxCreateLogicalMatrix(1, 1);
	mxLogical *rv = mxGetLogicals(_out);
	if(result == HELICS_TRUE){
		rv[0] = true;
	}else{
		rv[0] = false;
	}

	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateEnterInitializingModeComplete(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateEnterInitializingModeComplete:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateEnterInitializingModeComplete:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFederate fed = *(static_cast<HelicsFederate*>(mxGetData(argv[0])));

	HelicsError err = helicsErrorInitialize();

	helicsFederateEnterInitializingModeComplete(fed, &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateEnterExecutingMode(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateEnterExecutingMode:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateEnterExecutingMode:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFederate fed = *(static_cast<HelicsFederate*>(mxGetData(argv[0])));

	HelicsError err = helicsErrorInitialize();

	helicsFederateEnterExecutingMode(fed, &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateEnterExecutingModeAsync(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateEnterExecutingModeAsync:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateEnterExecutingModeAsync:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFederate fed = *(static_cast<HelicsFederate*>(mxGetData(argv[0])));

	HelicsError err = helicsErrorInitialize();

	helicsFederateEnterExecutingModeAsync(fed, &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateEnterExecutingModeComplete(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateEnterExecutingModeComplete:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateEnterExecutingModeComplete:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFederate fed = *(static_cast<HelicsFederate*>(mxGetData(argv[0])));

	HelicsError err = helicsErrorInitialize();

	helicsFederateEnterExecutingModeComplete(fed, &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateEnterExecutingModeIterative(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateEnterExecutingModeIterative:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateEnterExecutingModeIterative:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFederate fed = *(static_cast<HelicsFederate*>(mxGetData(argv[0])));

	if(!mxIsNumeric(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateEnterExecutingModeIterative:TypeError","Argument 2 must be of type int32.");
	}
	int iterateInt = static_cast<int>(mxGetScalar(argv[1]));
	HelicsIterationRequest iterate = static_cast<HelicsIterationRequest>(iterateInt);

	HelicsError err = helicsErrorInitialize();

	HelicsIterationResult result = helicsFederateEnterExecutingModeIterative(fed, iterate, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT32_CLASS, mxREAL);
	mxInt32 *rv = mxGetInt32s(_out);
	rv[0] = static_cast<mxInt32>(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateEnterExecutingModeIterativeAsync(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateEnterExecutingModeIterativeAsync:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateEnterExecutingModeIterativeAsync:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFederate fed = *(static_cast<HelicsFederate*>(mxGetData(argv[0])));

	if(!mxIsNumeric(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateEnterExecutingModeIterativeAsync:TypeError","Argument 2 must be of type int32.");
	}
	int iterateInt = static_cast<int>(mxGetScalar(argv[1]));
	HelicsIterationRequest iterate = static_cast<HelicsIterationRequest>(iterateInt);

	HelicsError err = helicsErrorInitialize();

	helicsFederateEnterExecutingModeIterativeAsync(fed, iterate, &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateEnterExecutingModeIterativeComplete(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateEnterExecutingModeIterativeComplete:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateEnterExecutingModeIterativeComplete:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFederate fed = *(static_cast<HelicsFederate*>(mxGetData(argv[0])));

	HelicsError err = helicsErrorInitialize();

	HelicsIterationResult result = helicsFederateEnterExecutingModeIterativeComplete(fed, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT32_CLASS, mxREAL);
	mxInt32 *rv = mxGetInt32s(_out);
	rv[0] = static_cast<mxInt32>(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateGetState(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateGetState:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateGetState:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFederate fed = *(static_cast<HelicsFederate*>(mxGetData(argv[0])));

	HelicsError err = helicsErrorInitialize();

	HelicsFederateState result = helicsFederateGetState(fed, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT32_CLASS, mxREAL);
	mxInt32 *rv = mxGetInt32s(_out);
	rv[0] = static_cast<mxInt32>(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateGetCore(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateGetCore:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateGetCore:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFederate fed = *(static_cast<HelicsFederate*>(mxGetData(argv[0])));

	HelicsError err = helicsErrorInitialize();

	HelicsCore result = helicsFederateGetCore(fed, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	mxUint64 *rv = mxGetUint64s(_out);
	rv[0] = reinterpret_cast<mxUint64>(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateRequestTime(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateRequestTime:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateRequestTime:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFederate fed = *(static_cast<HelicsFederate*>(mxGetData(argv[0])));

	if(!mxIsNumeric(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateRequestTime:TypeError","Argument 2 must be of type double.");
	}
	HelicsTime requestTime = (HelicsTime)(mxGetScalar(argv[1]));

	HelicsError err = helicsErrorInitialize();

	HelicsTime result = helicsFederateRequestTime(fed, requestTime, &err);

	mxArray *_out = mxCreateDoubleScalar(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateRequestTimeAdvance(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateRequestTimeAdvance:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateRequestTimeAdvance:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFederate fed = *(static_cast<HelicsFederate*>(mxGetData(argv[0])));

	if(!mxIsNumeric(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateRequestTimeAdvance:TypeError","Argument 2 must be of type double.");
	}
	HelicsTime timeDelta = (HelicsTime)(mxGetScalar(argv[1]));

	HelicsError err = helicsErrorInitialize();

	HelicsTime result = helicsFederateRequestTimeAdvance(fed, timeDelta, &err);

	mxArray *_out = mxCreateDoubleScalar(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateRequestNextStep(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateRequestNextStep:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateRequestNextStep:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFederate fed = *(static_cast<HelicsFederate*>(mxGetData(argv[0])));

	HelicsError err = helicsErrorInitialize();

	HelicsTime result = helicsFederateRequestNextStep(fed, &err);

	mxArray *_out = mxCreateDoubleScalar(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateRequestTimeIterative(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 3){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateRequestTimeIterative:rhs","This function requires 3 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateRequestTimeIterative:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFederate fed = *(static_cast<HelicsFederate*>(mxGetData(argv[0])));

	if(!mxIsNumeric(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateRequestTimeIterative:TypeError","Argument 2 must be of type double.");
	}
	HelicsTime requestTime = (HelicsTime)(mxGetScalar(argv[1]));

	if(!mxIsNumeric(argv[2])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateRequestTimeIterative:TypeError","Argument 3 must be of type int32.");
	}
	int iterateInt = static_cast<int>(mxGetScalar(argv[2]));
	HelicsIterationRequest iterate = static_cast<HelicsIterationRequest>(iterateInt);

	HelicsIterationResult outIteration = HELICS_ITERATION_RESULT_ERROR;

	HelicsError err = helicsErrorInitialize();

	HelicsTime result = helicsFederateRequestTimeIterative(fed, requestTime, iterate, &outIteration, &err);

	mxArray *_out = mxCreateDoubleScalar(result);	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(--resc>=0){
		mxArray *_out1 = mxCreateNumericMatrix(1,1,mxINT32_CLASS,mxREAL);
		mxInt32 *rv = mxGetInt32s(_out1);
		rv[0] = static_cast<mxInt32>(outIteration);
		*resv++ = _out1;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateRequestTimeAsync(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateRequestTimeAsync:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateRequestTimeAsync:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFederate fed = *(static_cast<HelicsFederate*>(mxGetData(argv[0])));

	if(!mxIsNumeric(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateRequestTimeAsync:TypeError","Argument 2 must be of type double.");
	}
	HelicsTime requestTime = (HelicsTime)(mxGetScalar(argv[1]));

	HelicsError err = helicsErrorInitialize();

	helicsFederateRequestTimeAsync(fed, requestTime, &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateRequestTimeComplete(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateRequestTimeComplete:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateRequestTimeComplete:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFederate fed = *(static_cast<HelicsFederate*>(mxGetData(argv[0])));

	HelicsError err = helicsErrorInitialize();

	HelicsTime result = helicsFederateRequestTimeComplete(fed, &err);

	mxArray *_out = mxCreateDoubleScalar(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateRequestTimeIterativeAsync(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 3){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateRequestTimeIterativeAsync:rhs","This function requires 3 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateRequestTimeIterativeAsync:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFederate fed = *(static_cast<HelicsFederate*>(mxGetData(argv[0])));

	if(!mxIsNumeric(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateRequestTimeIterativeAsync:TypeError","Argument 2 must be of type double.");
	}
	HelicsTime requestTime = (HelicsTime)(mxGetScalar(argv[1]));

	if(!mxIsNumeric(argv[2])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateRequestTimeIterativeAsync:TypeError","Argument 3 must be of type int32.");
	}
	int iterateInt = static_cast<int>(mxGetScalar(argv[2]));
	HelicsIterationRequest iterate = static_cast<HelicsIterationRequest>(iterateInt);

	HelicsError err = helicsErrorInitialize();

	helicsFederateRequestTimeIterativeAsync(fed, requestTime, iterate, &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateRequestTimeIterativeComplete(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateRequestTimeIterativeComplete:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateRequestTimeIterativeComplete:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFederate fed = *(static_cast<HelicsFederate*>(mxGetData(argv[0])));

	HelicsIterationResult outIteration = HELICS_ITERATION_RESULT_ERROR;

	HelicsError err = helicsErrorInitialize();

	HelicsTime result = helicsFederateRequestTimeIterativeComplete(fed, &outIteration, &err);

	mxArray *_out = mxCreateDoubleScalar(result);	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(--resc>=0){
		mxArray *_out1 = mxCreateNumericMatrix(1,1,mxINT32_CLASS,mxREAL);
		mxInt32 *rv = mxGetInt32s(_out1);
		rv[0] = static_cast<mxInt32>(outIteration);
		*resv++ = _out1;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateProcessCommunications(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateProcessCommunications:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateProcessCommunications:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFederate fed = *(static_cast<HelicsFederate*>(mxGetData(argv[0])));

	if(!mxIsNumeric(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateProcessCommunications:TypeError","Argument 2 must be of type double.");
	}
	HelicsTime period = (HelicsTime)(mxGetScalar(argv[1]));

	HelicsError err = helicsErrorInitialize();

	helicsFederateProcessCommunications(fed, period, &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateGetName(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateGetName:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateGetName:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFederate fed = *(static_cast<HelicsFederate*>(mxGetData(argv[0])));

	const char *result = helicsFederateGetName(fed);

	mxArray *_out = mxCreateString(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsFederateSetTimeProperty(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 3){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateSetTimeProperty:rhs","This function requires 3 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateSetTimeProperty:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFederate fed = *(static_cast<HelicsFederate*>(mxGetData(argv[0])));

	if(!mxIsNumeric(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateSetTimeProperty:TypeError","Argument 2 must be of type integer.");
	}
		int timeProperty = static_cast<int>(mxGetScalar(argv[1]));

	if(!mxIsNumeric(argv[2])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateSetTimeProperty:TypeError","Argument 3 must be of type double.");
	}
	HelicsTime time = (HelicsTime)(mxGetScalar(argv[2]));

	HelicsError err = helicsErrorInitialize();

	helicsFederateSetTimeProperty(fed, timeProperty, time, &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateSetFlagOption(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 3){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateSetFlagOption:rhs","This function requires 3 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateSetFlagOption:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFederate fed = *(static_cast<HelicsFederate*>(mxGetData(argv[0])));

	if(!mxIsNumeric(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateSetFlagOption:TypeError","Argument 2 must be of type integer.");
	}
		int flag = static_cast<int>(mxGetScalar(argv[1]));

	if(!mxIsLogical(argv[2])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateSetFlagOption:TypeError","Argument 3 must be a 0 or 1.");
	}
	mxLogical *pflagValue = mxGetLogicals(argv[2]);
	HelicsBool flagValue = HELICS_FALSE;
	if(pflagValue[0]){
		flagValue = HELICS_TRUE;
	}

	HelicsError err = helicsErrorInitialize();

	helicsFederateSetFlagOption(fed, flag, flagValue, &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateSetSeparator(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateSetSeparator:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateSetSeparator:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFederate fed = *(static_cast<HelicsFederate*>(mxGetData(argv[0])));

	if(!mxIsChar(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateSetSeparator:TypeError","Argument 2 must be a string.");
	}
	char *separator = nullptr;
	size_t separatorLength = 0;
	int separatorStatus = 0;
	if(argc > 1){
		separatorLength = mxGetN(argv[1]) + 1;
		separator = static_cast<char *>(malloc(separatorLength));
		separatorStatus = mxGetString(argv[1], separator, separatorLength);

	}
	HelicsError err = helicsErrorInitialize();

	helicsFederateSetSeparator(fed, separator[0], &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(separator);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateSetIntegerProperty(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 3){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateSetIntegerProperty:rhs","This function requires 3 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateSetIntegerProperty:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFederate fed = *(static_cast<HelicsFederate*>(mxGetData(argv[0])));

	if(!mxIsNumeric(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateSetIntegerProperty:TypeError","Argument 2 must be of type integer.");
	}
		int intProperty = static_cast<int>(mxGetScalar(argv[1]));

	if(!mxIsNumeric(argv[2])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateSetIntegerProperty:TypeError","Argument 3 must be of type integer.");
	}
		int propertyVal = static_cast<int>(mxGetScalar(argv[2]));

	HelicsError err = helicsErrorInitialize();

	helicsFederateSetIntegerProperty(fed, intProperty, propertyVal, &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateGetTimeProperty(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateGetTimeProperty:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateGetTimeProperty:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFederate fed = *(static_cast<HelicsFederate*>(mxGetData(argv[0])));

	if(!mxIsNumeric(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateGetTimeProperty:TypeError","Argument 2 must be of type integer.");
	}
		int timeProperty = static_cast<int>(mxGetScalar(argv[1]));

	HelicsError err = helicsErrorInitialize();

	HelicsTime result = helicsFederateGetTimeProperty(fed, timeProperty, &err);

	mxArray *_out = mxCreateDoubleScalar(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateGetFlagOption(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateGetFlagOption:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateGetFlagOption:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFederate fed = *(static_cast<HelicsFederate*>(mxGetData(argv[0])));

	if(!mxIsNumeric(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateGetFlagOption:TypeError","Argument 2 must be of type integer.");
	}
		int flag = static_cast<int>(mxGetScalar(argv[1]));

	HelicsError err = helicsErrorInitialize();

	HelicsBool result = helicsFederateGetFlagOption(fed, flag, &err);

	mxArray *_out = mxCreateLogicalMatrix(1, 1);
	mxLogical *rv = mxGetLogicals(_out);
	if(result == HELICS_TRUE){
		rv[0] = true;
	}else{
		rv[0] = false;
	}

	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateGetIntegerProperty(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateGetIntegerProperty:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateGetIntegerProperty:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFederate fed = *(static_cast<HelicsFederate*>(mxGetData(argv[0])));

	if(!mxIsNumeric(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateGetIntegerProperty:TypeError","Argument 2 must be of type integer.");
	}
		int intProperty = static_cast<int>(mxGetScalar(argv[1]));

	HelicsError err = helicsErrorInitialize();

	int result = helicsFederateGetIntegerProperty(fed, intProperty, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
	mxInt64 *rv = mxGetInt64s(_out);
	rv[0] = static_cast<mxInt64>(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateGetCurrentTime(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateGetCurrentTime:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateGetCurrentTime:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFederate fed = *(static_cast<HelicsFederate*>(mxGetData(argv[0])));

	HelicsError err = helicsErrorInitialize();

	HelicsTime result = helicsFederateGetCurrentTime(fed, &err);

	mxArray *_out = mxCreateDoubleScalar(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateSetGlobal(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 3){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateSetGlobal:rhs","This function requires 3 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateSetGlobal:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFederate fed = *(static_cast<HelicsFederate*>(mxGetData(argv[0])));

	if(!mxIsChar(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateSetGlobal:TypeError","Argument 2 must be a string.");
	}
	char *valueName = nullptr;
	size_t valueNameLength = 0;
	int valueNameStatus = 0;
	if(argc > 1){
		valueNameLength = mxGetN(argv[1]) + 1;
		valueName = static_cast<char *>(malloc(valueNameLength));
		valueNameStatus = mxGetString(argv[1], valueName, valueNameLength);

	}
	if(!mxIsChar(argv[2])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateSetGlobal:TypeError","Argument 3 must be a string.");
	}
	char *value = nullptr;
	size_t valueLength = 0;
	int valueStatus = 0;
	if(argc > 2){
		valueLength = mxGetN(argv[2]) + 1;
		value = static_cast<char *>(malloc(valueLength));
		valueStatus = mxGetString(argv[2], value, valueLength);

	}
	HelicsError err = helicsErrorInitialize();

	helicsFederateSetGlobal(fed, static_cast<const char *>(valueName), static_cast<const char *>(value), &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(valueName);

	free(value);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateSetTag(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 3){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateSetTag:rhs","This function requires 3 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateSetTag:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFederate fed = *(static_cast<HelicsFederate*>(mxGetData(argv[0])));

	if(!mxIsChar(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateSetTag:TypeError","Argument 2 must be a string.");
	}
	char *tagName = nullptr;
	size_t tagNameLength = 0;
	int tagNameStatus = 0;
	if(argc > 1){
		tagNameLength = mxGetN(argv[1]) + 1;
		tagName = static_cast<char *>(malloc(tagNameLength));
		tagNameStatus = mxGetString(argv[1], tagName, tagNameLength);

	}
	if(!mxIsChar(argv[2])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateSetTag:TypeError","Argument 3 must be a string.");
	}
	char *value = nullptr;
	size_t valueLength = 0;
	int valueStatus = 0;
	if(argc > 2){
		valueLength = mxGetN(argv[2]) + 1;
		value = static_cast<char *>(malloc(valueLength));
		valueStatus = mxGetString(argv[2], value, valueLength);

	}
	HelicsError err = helicsErrorInitialize();

	helicsFederateSetTag(fed, static_cast<const char *>(tagName), static_cast<const char *>(value), &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(tagName);

	free(value);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateGetTag(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateGetTag:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateGetTag:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFederate fed = *(static_cast<HelicsFederate*>(mxGetData(argv[0])));

	if(!mxIsChar(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateGetTag:TypeError","Argument 2 must be a string.");
	}
	char *tagName = nullptr;
	size_t tagNameLength = 0;
	int tagNameStatus = 0;
	if(argc > 1){
		tagNameLength = mxGetN(argv[1]) + 1;
		tagName = static_cast<char *>(malloc(tagNameLength));
		tagNameStatus = mxGetString(argv[1], tagName, tagNameLength);

	}
	HelicsError err = helicsErrorInitialize();

	const char *result = helicsFederateGetTag(fed, static_cast<const char *>(tagName), &err);

	mxArray *_out = mxCreateString(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(tagName);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateAddDependency(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateAddDependency:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateAddDependency:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFederate fed = *(static_cast<HelicsFederate*>(mxGetData(argv[0])));

	if(!mxIsChar(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateAddDependency:TypeError","Argument 2 must be a string.");
	}
	char *fedName = nullptr;
	size_t fedNameLength = 0;
	int fedNameStatus = 0;
	if(argc > 1){
		fedNameLength = mxGetN(argv[1]) + 1;
		fedName = static_cast<char *>(malloc(fedNameLength));
		fedNameStatus = mxGetString(argv[1], fedName, fedNameLength);

	}
	HelicsError err = helicsErrorInitialize();

	helicsFederateAddDependency(fed, static_cast<const char *>(fedName), &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(fedName);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateSetLogFile(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateSetLogFile:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateSetLogFile:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFederate fed = *(static_cast<HelicsFederate*>(mxGetData(argv[0])));

	if(!mxIsChar(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateSetLogFile:TypeError","Argument 2 must be a string.");
	}
	char *logFile = nullptr;
	size_t logFileLength = 0;
	int logFileStatus = 0;
	if(argc > 1){
		logFileLength = mxGetN(argv[1]) + 1;
		logFile = static_cast<char *>(malloc(logFileLength));
		logFileStatus = mxGetString(argv[1], logFile, logFileLength);

	}
	HelicsError err = helicsErrorInitialize();

	helicsFederateSetLogFile(fed, static_cast<const char *>(logFile), &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(logFile);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateLogErrorMessage(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateLogErrorMessage:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateLogErrorMessage:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFederate fed = *(static_cast<HelicsFederate*>(mxGetData(argv[0])));

	if(!mxIsChar(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateLogErrorMessage:TypeError","Argument 2 must be a string.");
	}
	char *logmessage = nullptr;
	size_t logmessageLength = 0;
	int logmessageStatus = 0;
	if(argc > 1){
		logmessageLength = mxGetN(argv[1]) + 1;
		logmessage = static_cast<char *>(malloc(logmessageLength));
		logmessageStatus = mxGetString(argv[1], logmessage, logmessageLength);

	}
	HelicsError err = helicsErrorInitialize();

	helicsFederateLogErrorMessage(fed, static_cast<const char *>(logmessage), &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(logmessage);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateLogWarningMessage(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateLogWarningMessage:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateLogWarningMessage:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFederate fed = *(static_cast<HelicsFederate*>(mxGetData(argv[0])));

	if(!mxIsChar(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateLogWarningMessage:TypeError","Argument 2 must be a string.");
	}
	char *logmessage = nullptr;
	size_t logmessageLength = 0;
	int logmessageStatus = 0;
	if(argc > 1){
		logmessageLength = mxGetN(argv[1]) + 1;
		logmessage = static_cast<char *>(malloc(logmessageLength));
		logmessageStatus = mxGetString(argv[1], logmessage, logmessageLength);

	}
	HelicsError err = helicsErrorInitialize();

	helicsFederateLogWarningMessage(fed, static_cast<const char *>(logmessage), &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(logmessage);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateLogInfoMessage(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateLogInfoMessage:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateLogInfoMessage:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFederate fed = *(static_cast<HelicsFederate*>(mxGetData(argv[0])));

	if(!mxIsChar(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateLogInfoMessage:TypeError","Argument 2 must be a string.");
	}
	char *logmessage = nullptr;
	size_t logmessageLength = 0;
	int logmessageStatus = 0;
	if(argc > 1){
		logmessageLength = mxGetN(argv[1]) + 1;
		logmessage = static_cast<char *>(malloc(logmessageLength));
		logmessageStatus = mxGetString(argv[1], logmessage, logmessageLength);

	}
	HelicsError err = helicsErrorInitialize();

	helicsFederateLogInfoMessage(fed, static_cast<const char *>(logmessage), &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(logmessage);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateLogDebugMessage(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateLogDebugMessage:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateLogDebugMessage:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFederate fed = *(static_cast<HelicsFederate*>(mxGetData(argv[0])));

	if(!mxIsChar(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateLogDebugMessage:TypeError","Argument 2 must be a string.");
	}
	char *logmessage = nullptr;
	size_t logmessageLength = 0;
	int logmessageStatus = 0;
	if(argc > 1){
		logmessageLength = mxGetN(argv[1]) + 1;
		logmessage = static_cast<char *>(malloc(logmessageLength));
		logmessageStatus = mxGetString(argv[1], logmessage, logmessageLength);

	}
	HelicsError err = helicsErrorInitialize();

	helicsFederateLogDebugMessage(fed, static_cast<const char *>(logmessage), &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(logmessage);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateLogLevelMessage(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 3){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateLogLevelMessage:rhs","This function requires 3 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateLogLevelMessage:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFederate fed = *(static_cast<HelicsFederate*>(mxGetData(argv[0])));

	if(!mxIsNumeric(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateLogLevelMessage:TypeError","Argument 2 must be of type integer.");
	}
		int loglevel = static_cast<int>(mxGetScalar(argv[1]));

	if(!mxIsChar(argv[2])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateLogLevelMessage:TypeError","Argument 3 must be a string.");
	}
	char *logmessage = nullptr;
	size_t logmessageLength = 0;
	int logmessageStatus = 0;
	if(argc > 2){
		logmessageLength = mxGetN(argv[2]) + 1;
		logmessage = static_cast<char *>(malloc(logmessageLength));
		logmessageStatus = mxGetString(argv[2], logmessage, logmessageLength);

	}
	HelicsError err = helicsErrorInitialize();

	helicsFederateLogLevelMessage(fed, loglevel, static_cast<const char *>(logmessage), &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(logmessage);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateSendCommand(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 3){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateSendCommand:rhs","This function requires 3 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateSendCommand:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFederate fed = *(static_cast<HelicsFederate*>(mxGetData(argv[0])));

	if(!mxIsChar(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateSendCommand:TypeError","Argument 2 must be a string.");
	}
	char *target = nullptr;
	size_t targetLength = 0;
	int targetStatus = 0;
	if(argc > 1){
		targetLength = mxGetN(argv[1]) + 1;
		target = static_cast<char *>(malloc(targetLength));
		targetStatus = mxGetString(argv[1], target, targetLength);

	}
	if(!mxIsChar(argv[2])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateSendCommand:TypeError","Argument 3 must be a string.");
	}
	char *command = nullptr;
	size_t commandLength = 0;
	int commandStatus = 0;
	if(argc > 2){
		commandLength = mxGetN(argv[2]) + 1;
		command = static_cast<char *>(malloc(commandLength));
		commandStatus = mxGetString(argv[2], command, commandLength);

	}
	HelicsError err = helicsErrorInitialize();

	helicsFederateSendCommand(fed, static_cast<const char *>(target), static_cast<const char *>(command), &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(target);

	free(command);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateGetCommand(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateGetCommand:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateGetCommand:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFederate fed = *(static_cast<HelicsFederate*>(mxGetData(argv[0])));

	HelicsError err = helicsErrorInitialize();

	const char *result = helicsFederateGetCommand(fed, &err);

	mxArray *_out = mxCreateString(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateGetCommandSource(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateGetCommandSource:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateGetCommandSource:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFederate fed = *(static_cast<HelicsFederate*>(mxGetData(argv[0])));

	HelicsError err = helicsErrorInitialize();

	const char *result = helicsFederateGetCommandSource(fed, &err);

	mxArray *_out = mxCreateString(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateWaitCommand(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateWaitCommand:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateWaitCommand:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFederate fed = *(static_cast<HelicsFederate*>(mxGetData(argv[0])));

	HelicsError err = helicsErrorInitialize();

	const char *result = helicsFederateWaitCommand(fed, &err);

	mxArray *_out = mxCreateString(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsCoreSetGlobal(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 3){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsCoreSetGlobal:rhs","This function requires 3 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsCoreSetGlobal:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsCore core = *(static_cast<HelicsCore*>(mxGetData(argv[0])));

	if(!mxIsChar(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsCoreSetGlobal:TypeError","Argument 2 must be a string.");
	}
	char *valueName = nullptr;
	size_t valueNameLength = 0;
	int valueNameStatus = 0;
	if(argc > 1){
		valueNameLength = mxGetN(argv[1]) + 1;
		valueName = static_cast<char *>(malloc(valueNameLength));
		valueNameStatus = mxGetString(argv[1], valueName, valueNameLength);

	}
	if(!mxIsChar(argv[2])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsCoreSetGlobal:TypeError","Argument 3 must be a string.");
	}
	char *value = nullptr;
	size_t valueLength = 0;
	int valueStatus = 0;
	if(argc > 2){
		valueLength = mxGetN(argv[2]) + 1;
		value = static_cast<char *>(malloc(valueLength));
		valueStatus = mxGetString(argv[2], value, valueLength);

	}
	HelicsError err = helicsErrorInitialize();

	helicsCoreSetGlobal(core, static_cast<const char *>(valueName), static_cast<const char *>(value), &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(valueName);

	free(value);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsBrokerSetGlobal(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 3){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsBrokerSetGlobal:rhs","This function requires 3 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsBrokerSetGlobal:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsBroker broker = *(static_cast<HelicsBroker*>(mxGetData(argv[0])));

	if(!mxIsChar(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsBrokerSetGlobal:TypeError","Argument 2 must be a string.");
	}
	char *valueName = nullptr;
	size_t valueNameLength = 0;
	int valueNameStatus = 0;
	if(argc > 1){
		valueNameLength = mxGetN(argv[1]) + 1;
		valueName = static_cast<char *>(malloc(valueNameLength));
		valueNameStatus = mxGetString(argv[1], valueName, valueNameLength);

	}
	if(!mxIsChar(argv[2])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsBrokerSetGlobal:TypeError","Argument 3 must be a string.");
	}
	char *value = nullptr;
	size_t valueLength = 0;
	int valueStatus = 0;
	if(argc > 2){
		valueLength = mxGetN(argv[2]) + 1;
		value = static_cast<char *>(malloc(valueLength));
		valueStatus = mxGetString(argv[2], value, valueLength);

	}
	HelicsError err = helicsErrorInitialize();

	helicsBrokerSetGlobal(broker, static_cast<const char *>(valueName), static_cast<const char *>(value), &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(valueName);

	free(value);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsCoreSendCommand(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 3){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsCoreSendCommand:rhs","This function requires 3 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsCoreSendCommand:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsCore core = *(static_cast<HelicsCore*>(mxGetData(argv[0])));

	if(!mxIsChar(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsCoreSendCommand:TypeError","Argument 2 must be a string.");
	}
	char *target = nullptr;
	size_t targetLength = 0;
	int targetStatus = 0;
	if(argc > 1){
		targetLength = mxGetN(argv[1]) + 1;
		target = static_cast<char *>(malloc(targetLength));
		targetStatus = mxGetString(argv[1], target, targetLength);

	}
	if(!mxIsChar(argv[2])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsCoreSendCommand:TypeError","Argument 3 must be a string.");
	}
	char *command = nullptr;
	size_t commandLength = 0;
	int commandStatus = 0;
	if(argc > 2){
		commandLength = mxGetN(argv[2]) + 1;
		command = static_cast<char *>(malloc(commandLength));
		commandStatus = mxGetString(argv[2], command, commandLength);

	}
	HelicsError err = helicsErrorInitialize();

	helicsCoreSendCommand(core, static_cast<const char *>(target), static_cast<const char *>(command), &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(target);

	free(command);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsCoreSendOrderedCommand(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 3){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsCoreSendOrderedCommand:rhs","This function requires 3 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsCoreSendOrderedCommand:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsCore core = *(static_cast<HelicsCore*>(mxGetData(argv[0])));

	if(!mxIsChar(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsCoreSendOrderedCommand:TypeError","Argument 2 must be a string.");
	}
	char *target = nullptr;
	size_t targetLength = 0;
	int targetStatus = 0;
	if(argc > 1){
		targetLength = mxGetN(argv[1]) + 1;
		target = static_cast<char *>(malloc(targetLength));
		targetStatus = mxGetString(argv[1], target, targetLength);

	}
	if(!mxIsChar(argv[2])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsCoreSendOrderedCommand:TypeError","Argument 3 must be a string.");
	}
	char *command = nullptr;
	size_t commandLength = 0;
	int commandStatus = 0;
	if(argc > 2){
		commandLength = mxGetN(argv[2]) + 1;
		command = static_cast<char *>(malloc(commandLength));
		commandStatus = mxGetString(argv[2], command, commandLength);

	}
	HelicsError err = helicsErrorInitialize();

	helicsCoreSendOrderedCommand(core, static_cast<const char *>(target), static_cast<const char *>(command), &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(target);

	free(command);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsBrokerSendCommand(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 3){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsBrokerSendCommand:rhs","This function requires 3 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsBrokerSendCommand:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsBroker broker = *(static_cast<HelicsBroker*>(mxGetData(argv[0])));

	if(!mxIsChar(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsBrokerSendCommand:TypeError","Argument 2 must be a string.");
	}
	char *target = nullptr;
	size_t targetLength = 0;
	int targetStatus = 0;
	if(argc > 1){
		targetLength = mxGetN(argv[1]) + 1;
		target = static_cast<char *>(malloc(targetLength));
		targetStatus = mxGetString(argv[1], target, targetLength);

	}
	if(!mxIsChar(argv[2])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsBrokerSendCommand:TypeError","Argument 3 must be a string.");
	}
	char *command = nullptr;
	size_t commandLength = 0;
	int commandStatus = 0;
	if(argc > 2){
		commandLength = mxGetN(argv[2]) + 1;
		command = static_cast<char *>(malloc(commandLength));
		commandStatus = mxGetString(argv[2], command, commandLength);

	}
	HelicsError err = helicsErrorInitialize();

	helicsBrokerSendCommand(broker, static_cast<const char *>(target), static_cast<const char *>(command), &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(target);

	free(command);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsBrokerSendOrderedCommand(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 3){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsBrokerSendOrderedCommand:rhs","This function requires 3 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsBrokerSendOrderedCommand:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsBroker broker = *(static_cast<HelicsBroker*>(mxGetData(argv[0])));

	if(!mxIsChar(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsBrokerSendOrderedCommand:TypeError","Argument 2 must be a string.");
	}
	char *target = nullptr;
	size_t targetLength = 0;
	int targetStatus = 0;
	if(argc > 1){
		targetLength = mxGetN(argv[1]) + 1;
		target = static_cast<char *>(malloc(targetLength));
		targetStatus = mxGetString(argv[1], target, targetLength);

	}
	if(!mxIsChar(argv[2])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsBrokerSendOrderedCommand:TypeError","Argument 3 must be a string.");
	}
	char *command = nullptr;
	size_t commandLength = 0;
	int commandStatus = 0;
	if(argc > 2){
		commandLength = mxGetN(argv[2]) + 1;
		command = static_cast<char *>(malloc(commandLength));
		commandStatus = mxGetString(argv[2], command, commandLength);

	}
	HelicsError err = helicsErrorInitialize();

	helicsBrokerSendOrderedCommand(broker, static_cast<const char *>(target), static_cast<const char *>(command), &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(target);

	free(command);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsCoreSetLogFile(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsCoreSetLogFile:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsCoreSetLogFile:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsCore core = *(static_cast<HelicsCore*>(mxGetData(argv[0])));

	if(!mxIsChar(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsCoreSetLogFile:TypeError","Argument 2 must be a string.");
	}
	char *logFileName = nullptr;
	size_t logFileNameLength = 0;
	int logFileNameStatus = 0;
	if(argc > 1){
		logFileNameLength = mxGetN(argv[1]) + 1;
		logFileName = static_cast<char *>(malloc(logFileNameLength));
		logFileNameStatus = mxGetString(argv[1], logFileName, logFileNameLength);

	}
	HelicsError err = helicsErrorInitialize();

	helicsCoreSetLogFile(core, static_cast<const char *>(logFileName), &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(logFileName);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsBrokerSetLogFile(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsBrokerSetLogFile:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsBrokerSetLogFile:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsBroker broker = *(static_cast<HelicsBroker*>(mxGetData(argv[0])));

	if(!mxIsChar(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsBrokerSetLogFile:TypeError","Argument 2 must be a string.");
	}
	char *logFileName = nullptr;
	size_t logFileNameLength = 0;
	int logFileNameStatus = 0;
	if(argc > 1){
		logFileNameLength = mxGetN(argv[1]) + 1;
		logFileName = static_cast<char *>(malloc(logFileNameLength));
		logFileNameStatus = mxGetString(argv[1], logFileName, logFileNameLength);

	}
	HelicsError err = helicsErrorInitialize();

	helicsBrokerSetLogFile(broker, static_cast<const char *>(logFileName), &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(logFileName);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsBrokerSetTimeBarrier(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsBrokerSetTimeBarrier:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsBrokerSetTimeBarrier:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsBroker broker = *(static_cast<HelicsBroker*>(mxGetData(argv[0])));

	if(!mxIsNumeric(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsBrokerSetTimeBarrier:TypeError","Argument 2 must be of type double.");
	}
	HelicsTime barrierTime = (HelicsTime)(mxGetScalar(argv[1]));

	HelicsError err = helicsErrorInitialize();

	helicsBrokerSetTimeBarrier(broker, barrierTime, &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsBrokerClearTimeBarrier(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsBrokerClearTimeBarrier:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsBrokerClearTimeBarrier:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsBroker broker = *(static_cast<HelicsBroker*>(mxGetData(argv[0])));

	helicsBrokerClearTimeBarrier(broker);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsBrokerGlobalError(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 3){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsBrokerGlobalError:rhs","This function requires 3 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsBrokerGlobalError:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsBroker broker = *(static_cast<HelicsBroker*>(mxGetData(argv[0])));

	if(!mxIsNumeric(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsBrokerGlobalError:TypeError","Argument 2 must be of type integer.");
	}
		int errorCode = static_cast<int>(mxGetScalar(argv[1]));

	if(!mxIsChar(argv[2])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsBrokerGlobalError:TypeError","Argument 3 must be a string.");
	}
	char *errorString = nullptr;
	size_t errorStringLength = 0;
	int errorStringStatus = 0;
	if(argc > 2){
		errorStringLength = mxGetN(argv[2]) + 1;
		errorString = static_cast<char *>(malloc(errorStringLength));
		errorStringStatus = mxGetString(argv[2], errorString, errorStringLength);

	}
	HelicsError err = helicsErrorInitialize();

	helicsBrokerGlobalError(broker, errorCode, static_cast<const char *>(errorString), &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(errorString);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsCoreGlobalError(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 3){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsCoreGlobalError:rhs","This function requires 3 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsCoreGlobalError:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsCore core = *(static_cast<HelicsCore*>(mxGetData(argv[0])));

	if(!mxIsNumeric(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsCoreGlobalError:TypeError","Argument 2 must be of type integer.");
	}
		int errorCode = static_cast<int>(mxGetScalar(argv[1]));

	if(!mxIsChar(argv[2])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsCoreGlobalError:TypeError","Argument 3 must be a string.");
	}
	char *errorString = nullptr;
	size_t errorStringLength = 0;
	int errorStringStatus = 0;
	if(argc > 2){
		errorStringLength = mxGetN(argv[2]) + 1;
		errorString = static_cast<char *>(malloc(errorStringLength));
		errorStringStatus = mxGetString(argv[2], errorString, errorStringLength);

	}
	HelicsError err = helicsErrorInitialize();

	helicsCoreGlobalError(core, errorCode, static_cast<const char *>(errorString), &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(errorString);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsCreateQuery(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc < 1 || argc > 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsCreateQuery:rhs","This function requires at least 1 arguments and at most 2 arguments.");
	}

	if(!mxIsChar(argv[0])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsCreateQuery:TypeError","Argument 1 must be a string.");
	}
	char *target = nullptr;
	size_t targetLength = 0;
	int targetStatus = 0;
	if(argc > 0){
		targetLength = mxGetN(argv[0]) + 1;
		target = static_cast<char *>(malloc(targetLength));
		targetStatus = mxGetString(argv[0], target, targetLength);

	}
	if(argc > 1){
		if(!mxIsChar(argv[1])){
			mexUnlock();
			mexErrMsgIdAndTxt("MATLAB:helicsCreateQuery:TypeError","Argument 2 must be a string.");
		}
	}
	char *query = nullptr;
	size_t queryLength = 0;
	int queryStatus = 0;
	if(argc > 1){
		queryLength = mxGetN(argv[1]) + 1;
		query = static_cast<char *>(malloc(queryLength));
		queryStatus = mxGetString(argv[1], query, queryLength);

	}
	HelicsQuery result = helicsCreateQuery(static_cast<const char *>(target), static_cast<const char *>(query));

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	mxUint64 *rv = mxGetUint64s(_out);
	rv[0] = reinterpret_cast<mxUint64>(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(target);

	free(query);
}


void _wrap_helicsQueryExecute(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsQueryExecute:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsQueryExecute:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsQuery query = *(static_cast<HelicsQuery*>(mxGetData(argv[0])));

	if(mxGetClassID(argv[1]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsQueryExecute:TypeError","Argument 2 must be of type uint64.");
	}
	HelicsFederate fed = *(static_cast<HelicsFederate*>(mxGetData(argv[1])));

	HelicsError err = helicsErrorInitialize();

	const char *result = helicsQueryExecute(query, fed, &err);

	mxArray *_out = mxCreateString(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsQueryCoreExecute(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsQueryCoreExecute:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsQueryCoreExecute:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsQuery query = *(static_cast<HelicsQuery*>(mxGetData(argv[0])));

	if(mxGetClassID(argv[1]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsQueryCoreExecute:TypeError","Argument 2 must be of type uint64.");
	}
	HelicsCore core = *(static_cast<HelicsCore*>(mxGetData(argv[1])));

	HelicsError err = helicsErrorInitialize();

	const char *result = helicsQueryCoreExecute(query, core, &err);

	mxArray *_out = mxCreateString(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsQueryBrokerExecute(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsQueryBrokerExecute:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsQueryBrokerExecute:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsQuery query = *(static_cast<HelicsQuery*>(mxGetData(argv[0])));

	if(mxGetClassID(argv[1]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsQueryBrokerExecute:TypeError","Argument 2 must be of type uint64.");
	}
	HelicsBroker broker = *(static_cast<HelicsBroker*>(mxGetData(argv[1])));

	HelicsError err = helicsErrorInitialize();

	const char *result = helicsQueryBrokerExecute(query, broker, &err);

	mxArray *_out = mxCreateString(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsQueryExecuteAsync(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsQueryExecuteAsync:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsQueryExecuteAsync:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsQuery query = *(static_cast<HelicsQuery*>(mxGetData(argv[0])));

	if(mxGetClassID(argv[1]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsQueryExecuteAsync:TypeError","Argument 2 must be of type uint64.");
	}
	HelicsFederate fed = *(static_cast<HelicsFederate*>(mxGetData(argv[1])));

	HelicsError err = helicsErrorInitialize();

	helicsQueryExecuteAsync(query, fed, &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsQueryExecuteComplete(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsQueryExecuteComplete:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsQueryExecuteComplete:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsQuery query = *(static_cast<HelicsQuery*>(mxGetData(argv[0])));

	HelicsError err = helicsErrorInitialize();

	const char *result = helicsQueryExecuteComplete(query, &err);

	mxArray *_out = mxCreateString(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsQueryIsCompleted(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsQueryIsCompleted:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsQueryIsCompleted:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsQuery query = *(static_cast<HelicsQuery*>(mxGetData(argv[0])));

	HelicsBool result = helicsQueryIsCompleted(query);

	mxArray *_out = mxCreateLogicalMatrix(1, 1);
	mxLogical *rv = mxGetLogicals(_out);
	if(result == HELICS_TRUE){
		rv[0] = true;
	}else{
		rv[0] = false;
	}

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsQuerySetTarget(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsQuerySetTarget:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsQuerySetTarget:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsQuery query = *(static_cast<HelicsQuery*>(mxGetData(argv[0])));

	if(!mxIsChar(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsQuerySetTarget:TypeError","Argument 2 must be a string.");
	}
	char *target = nullptr;
	size_t targetLength = 0;
	int targetStatus = 0;
	if(argc > 1){
		targetLength = mxGetN(argv[1]) + 1;
		target = static_cast<char *>(malloc(targetLength));
		targetStatus = mxGetString(argv[1], target, targetLength);

	}
	HelicsError err = helicsErrorInitialize();

	helicsQuerySetTarget(query, static_cast<const char *>(target), &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(target);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsQuerySetQueryString(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsQuerySetQueryString:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsQuerySetQueryString:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsQuery query = *(static_cast<HelicsQuery*>(mxGetData(argv[0])));

	if(!mxIsChar(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsQuerySetQueryString:TypeError","Argument 2 must be a string.");
	}
	char *queryString = nullptr;
	size_t queryStringLength = 0;
	int queryStringStatus = 0;
	if(argc > 1){
		queryStringLength = mxGetN(argv[1]) + 1;
		queryString = static_cast<char *>(malloc(queryStringLength));
		queryStringStatus = mxGetString(argv[1], queryString, queryStringLength);

	}
	HelicsError err = helicsErrorInitialize();

	helicsQuerySetQueryString(query, static_cast<const char *>(queryString), &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(queryString);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsQuerySetOrdering(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsQuerySetOrdering:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsQuerySetOrdering:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsQuery query = *(static_cast<HelicsQuery*>(mxGetData(argv[0])));

	if(mxGetClassID(argv[1]) != mxINT32_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsQuerySetOrdering:TypeError","Argument 2 must be of type int32.");
	}
mxInt32 *pmode = mxGetInt32s(argv[1]);
	int32_t mode = static_cast<int32_t>(pmode[0]);

	HelicsError err = helicsErrorInitialize();

	helicsQuerySetOrdering(query, mode, &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsQueryFree(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsQueryFree:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsQueryFree:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsQuery query = *(static_cast<HelicsQuery*>(mxGetData(argv[0])));

	helicsQueryFree(query);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsCleanupLibrary(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 0){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsCleanupLibrary:rhs","This function requires 0 arguments.");
	}

	helicsCleanupLibrary();

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsFederateRegisterSubscription(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 3){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateRegisterSubscription:rhs","This function requires 3 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateRegisterSubscription:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFederate fed = *(static_cast<HelicsFederate*>(mxGetData(argv[0])));

	if(!mxIsChar(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateRegisterSubscription:TypeError","Argument 2 must be a string.");
	}
	char *key = nullptr;
	size_t keyLength = 0;
	int keyStatus = 0;
	if(argc > 1){
		keyLength = mxGetN(argv[1]) + 1;
		key = static_cast<char *>(malloc(keyLength));
		keyStatus = mxGetString(argv[1], key, keyLength);

	}
	if(!mxIsChar(argv[2])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateRegisterSubscription:TypeError","Argument 3 must be a string.");
	}
	char *units = nullptr;
	size_t unitsLength = 0;
	int unitsStatus = 0;
	if(argc > 2){
		unitsLength = mxGetN(argv[2]) + 1;
		units = static_cast<char *>(malloc(unitsLength));
		unitsStatus = mxGetString(argv[2], units, unitsLength);

	}
	HelicsError err = helicsErrorInitialize();

	HelicsInput result = helicsFederateRegisterSubscription(fed, static_cast<const char *>(key), static_cast<const char *>(units), &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	mxUint64 *rv = mxGetUint64s(_out);
	rv[0] = reinterpret_cast<mxUint64>(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(key);

	free(units);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateRegisterPublication(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 4){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateRegisterPublication:rhs","This function requires 4 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateRegisterPublication:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFederate fed = *(static_cast<HelicsFederate*>(mxGetData(argv[0])));

	if(!mxIsChar(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateRegisterPublication:TypeError","Argument 2 must be a string.");
	}
	char *key = nullptr;
	size_t keyLength = 0;
	int keyStatus = 0;
	if(argc > 1){
		keyLength = mxGetN(argv[1]) + 1;
		key = static_cast<char *>(malloc(keyLength));
		keyStatus = mxGetString(argv[1], key, keyLength);

	}
	if(!mxIsNumeric(argv[2])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateRegisterPublication:TypeError","Argument 3 must be of type int32.");
	}
	int typeInt = static_cast<int>(mxGetScalar(argv[2]));
	HelicsDataTypes type = static_cast<HelicsDataTypes>(typeInt);

	if(!mxIsChar(argv[3])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateRegisterPublication:TypeError","Argument 4 must be a string.");
	}
	char *units = nullptr;
	size_t unitsLength = 0;
	int unitsStatus = 0;
	if(argc > 3){
		unitsLength = mxGetN(argv[3]) + 1;
		units = static_cast<char *>(malloc(unitsLength));
		unitsStatus = mxGetString(argv[3], units, unitsLength);

	}
	HelicsError err = helicsErrorInitialize();

	HelicsPublication result = helicsFederateRegisterPublication(fed, static_cast<const char *>(key), type, static_cast<const char *>(units), &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	mxUint64 *rv = mxGetUint64s(_out);
	rv[0] = reinterpret_cast<mxUint64>(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(key);

	free(units);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateRegisterTypePublication(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 4){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateRegisterTypePublication:rhs","This function requires 4 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateRegisterTypePublication:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFederate fed = *(static_cast<HelicsFederate*>(mxGetData(argv[0])));

	if(!mxIsChar(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateRegisterTypePublication:TypeError","Argument 2 must be a string.");
	}
	char *key = nullptr;
	size_t keyLength = 0;
	int keyStatus = 0;
	if(argc > 1){
		keyLength = mxGetN(argv[1]) + 1;
		key = static_cast<char *>(malloc(keyLength));
		keyStatus = mxGetString(argv[1], key, keyLength);

	}
	if(!mxIsChar(argv[2])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateRegisterTypePublication:TypeError","Argument 3 must be a string.");
	}
	char *type = nullptr;
	size_t typeLength = 0;
	int typeStatus = 0;
	if(argc > 2){
		typeLength = mxGetN(argv[2]) + 1;
		type = static_cast<char *>(malloc(typeLength));
		typeStatus = mxGetString(argv[2], type, typeLength);

	}
	if(!mxIsChar(argv[3])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateRegisterTypePublication:TypeError","Argument 4 must be a string.");
	}
	char *units = nullptr;
	size_t unitsLength = 0;
	int unitsStatus = 0;
	if(argc > 3){
		unitsLength = mxGetN(argv[3]) + 1;
		units = static_cast<char *>(malloc(unitsLength));
		unitsStatus = mxGetString(argv[3], units, unitsLength);

	}
	HelicsError err = helicsErrorInitialize();

	HelicsPublication result = helicsFederateRegisterTypePublication(fed, static_cast<const char *>(key), static_cast<const char *>(type), static_cast<const char *>(units), &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	mxUint64 *rv = mxGetUint64s(_out);
	rv[0] = reinterpret_cast<mxUint64>(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(key);

	free(type);

	free(units);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateRegisterGlobalPublication(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 4){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateRegisterGlobalPublication:rhs","This function requires 4 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateRegisterGlobalPublication:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFederate fed = *(static_cast<HelicsFederate*>(mxGetData(argv[0])));

	if(!mxIsChar(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateRegisterGlobalPublication:TypeError","Argument 2 must be a string.");
	}
	char *key = nullptr;
	size_t keyLength = 0;
	int keyStatus = 0;
	if(argc > 1){
		keyLength = mxGetN(argv[1]) + 1;
		key = static_cast<char *>(malloc(keyLength));
		keyStatus = mxGetString(argv[1], key, keyLength);

	}
	if(!mxIsNumeric(argv[2])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateRegisterGlobalPublication:TypeError","Argument 3 must be of type int32.");
	}
	int typeInt = static_cast<int>(mxGetScalar(argv[2]));
	HelicsDataTypes type = static_cast<HelicsDataTypes>(typeInt);

	if(!mxIsChar(argv[3])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateRegisterGlobalPublication:TypeError","Argument 4 must be a string.");
	}
	char *units = nullptr;
	size_t unitsLength = 0;
	int unitsStatus = 0;
	if(argc > 3){
		unitsLength = mxGetN(argv[3]) + 1;
		units = static_cast<char *>(malloc(unitsLength));
		unitsStatus = mxGetString(argv[3], units, unitsLength);

	}
	HelicsError err = helicsErrorInitialize();

	HelicsPublication result = helicsFederateRegisterGlobalPublication(fed, static_cast<const char *>(key), type, static_cast<const char *>(units), &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	mxUint64 *rv = mxGetUint64s(_out);
	rv[0] = reinterpret_cast<mxUint64>(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(key);

	free(units);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateRegisterGlobalTypePublication(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 4){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateRegisterGlobalTypePublication:rhs","This function requires 4 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateRegisterGlobalTypePublication:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFederate fed = *(static_cast<HelicsFederate*>(mxGetData(argv[0])));

	if(!mxIsChar(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateRegisterGlobalTypePublication:TypeError","Argument 2 must be a string.");
	}
	char *key = nullptr;
	size_t keyLength = 0;
	int keyStatus = 0;
	if(argc > 1){
		keyLength = mxGetN(argv[1]) + 1;
		key = static_cast<char *>(malloc(keyLength));
		keyStatus = mxGetString(argv[1], key, keyLength);

	}
	if(!mxIsChar(argv[2])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateRegisterGlobalTypePublication:TypeError","Argument 3 must be a string.");
	}
	char *type = nullptr;
	size_t typeLength = 0;
	int typeStatus = 0;
	if(argc > 2){
		typeLength = mxGetN(argv[2]) + 1;
		type = static_cast<char *>(malloc(typeLength));
		typeStatus = mxGetString(argv[2], type, typeLength);

	}
	if(!mxIsChar(argv[3])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateRegisterGlobalTypePublication:TypeError","Argument 4 must be a string.");
	}
	char *units = nullptr;
	size_t unitsLength = 0;
	int unitsStatus = 0;
	if(argc > 3){
		unitsLength = mxGetN(argv[3]) + 1;
		units = static_cast<char *>(malloc(unitsLength));
		unitsStatus = mxGetString(argv[3], units, unitsLength);

	}
	HelicsError err = helicsErrorInitialize();

	HelicsPublication result = helicsFederateRegisterGlobalTypePublication(fed, static_cast<const char *>(key), static_cast<const char *>(type), static_cast<const char *>(units), &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	mxUint64 *rv = mxGetUint64s(_out);
	rv[0] = reinterpret_cast<mxUint64>(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(key);

	free(type);

	free(units);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateRegisterInput(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 4){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateRegisterInput:rhs","This function requires 4 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateRegisterInput:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFederate fed = *(static_cast<HelicsFederate*>(mxGetData(argv[0])));

	if(!mxIsChar(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateRegisterInput:TypeError","Argument 2 must be a string.");
	}
	char *key = nullptr;
	size_t keyLength = 0;
	int keyStatus = 0;
	if(argc > 1){
		keyLength = mxGetN(argv[1]) + 1;
		key = static_cast<char *>(malloc(keyLength));
		keyStatus = mxGetString(argv[1], key, keyLength);

	}
	if(!mxIsNumeric(argv[2])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateRegisterInput:TypeError","Argument 3 must be of type int32.");
	}
	int typeInt = static_cast<int>(mxGetScalar(argv[2]));
	HelicsDataTypes type = static_cast<HelicsDataTypes>(typeInt);

	if(!mxIsChar(argv[3])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateRegisterInput:TypeError","Argument 4 must be a string.");
	}
	char *units = nullptr;
	size_t unitsLength = 0;
	int unitsStatus = 0;
	if(argc > 3){
		unitsLength = mxGetN(argv[3]) + 1;
		units = static_cast<char *>(malloc(unitsLength));
		unitsStatus = mxGetString(argv[3], units, unitsLength);

	}
	HelicsError err = helicsErrorInitialize();

	HelicsInput result = helicsFederateRegisterInput(fed, static_cast<const char *>(key), type, static_cast<const char *>(units), &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	mxUint64 *rv = mxGetUint64s(_out);
	rv[0] = reinterpret_cast<mxUint64>(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(key);

	free(units);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateRegisterTypeInput(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 4){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateRegisterTypeInput:rhs","This function requires 4 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateRegisterTypeInput:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFederate fed = *(static_cast<HelicsFederate*>(mxGetData(argv[0])));

	if(!mxIsChar(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateRegisterTypeInput:TypeError","Argument 2 must be a string.");
	}
	char *key = nullptr;
	size_t keyLength = 0;
	int keyStatus = 0;
	if(argc > 1){
		keyLength = mxGetN(argv[1]) + 1;
		key = static_cast<char *>(malloc(keyLength));
		keyStatus = mxGetString(argv[1], key, keyLength);

	}
	if(!mxIsChar(argv[2])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateRegisterTypeInput:TypeError","Argument 3 must be a string.");
	}
	char *type = nullptr;
	size_t typeLength = 0;
	int typeStatus = 0;
	if(argc > 2){
		typeLength = mxGetN(argv[2]) + 1;
		type = static_cast<char *>(malloc(typeLength));
		typeStatus = mxGetString(argv[2], type, typeLength);

	}
	if(!mxIsChar(argv[3])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateRegisterTypeInput:TypeError","Argument 4 must be a string.");
	}
	char *units = nullptr;
	size_t unitsLength = 0;
	int unitsStatus = 0;
	if(argc > 3){
		unitsLength = mxGetN(argv[3]) + 1;
		units = static_cast<char *>(malloc(unitsLength));
		unitsStatus = mxGetString(argv[3], units, unitsLength);

	}
	HelicsError err = helicsErrorInitialize();

	HelicsInput result = helicsFederateRegisterTypeInput(fed, static_cast<const char *>(key), static_cast<const char *>(type), static_cast<const char *>(units), &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	mxUint64 *rv = mxGetUint64s(_out);
	rv[0] = reinterpret_cast<mxUint64>(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(key);

	free(type);

	free(units);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateRegisterGlobalInput(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 4){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateRegisterGlobalInput:rhs","This function requires 4 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateRegisterGlobalInput:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFederate fed = *(static_cast<HelicsFederate*>(mxGetData(argv[0])));

	if(!mxIsChar(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateRegisterGlobalInput:TypeError","Argument 2 must be a string.");
	}
	char *key = nullptr;
	size_t keyLength = 0;
	int keyStatus = 0;
	if(argc > 1){
		keyLength = mxGetN(argv[1]) + 1;
		key = static_cast<char *>(malloc(keyLength));
		keyStatus = mxGetString(argv[1], key, keyLength);

	}
	if(!mxIsNumeric(argv[2])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateRegisterGlobalInput:TypeError","Argument 3 must be of type int32.");
	}
	int typeInt = static_cast<int>(mxGetScalar(argv[2]));
	HelicsDataTypes type = static_cast<HelicsDataTypes>(typeInt);

	if(!mxIsChar(argv[3])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateRegisterGlobalInput:TypeError","Argument 4 must be a string.");
	}
	char *units = nullptr;
	size_t unitsLength = 0;
	int unitsStatus = 0;
	if(argc > 3){
		unitsLength = mxGetN(argv[3]) + 1;
		units = static_cast<char *>(malloc(unitsLength));
		unitsStatus = mxGetString(argv[3], units, unitsLength);

	}
	HelicsError err = helicsErrorInitialize();

	HelicsPublication result = helicsFederateRegisterGlobalInput(fed, static_cast<const char *>(key), type, static_cast<const char *>(units), &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	mxUint64 *rv = mxGetUint64s(_out);
	rv[0] = reinterpret_cast<mxUint64>(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(key);

	free(units);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateRegisterGlobalTypeInput(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 4){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateRegisterGlobalTypeInput:rhs","This function requires 4 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateRegisterGlobalTypeInput:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFederate fed = *(static_cast<HelicsFederate*>(mxGetData(argv[0])));

	if(!mxIsChar(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateRegisterGlobalTypeInput:TypeError","Argument 2 must be a string.");
	}
	char *key = nullptr;
	size_t keyLength = 0;
	int keyStatus = 0;
	if(argc > 1){
		keyLength = mxGetN(argv[1]) + 1;
		key = static_cast<char *>(malloc(keyLength));
		keyStatus = mxGetString(argv[1], key, keyLength);

	}
	if(!mxIsChar(argv[2])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateRegisterGlobalTypeInput:TypeError","Argument 3 must be a string.");
	}
	char *type = nullptr;
	size_t typeLength = 0;
	int typeStatus = 0;
	if(argc > 2){
		typeLength = mxGetN(argv[2]) + 1;
		type = static_cast<char *>(malloc(typeLength));
		typeStatus = mxGetString(argv[2], type, typeLength);

	}
	if(!mxIsChar(argv[3])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateRegisterGlobalTypeInput:TypeError","Argument 4 must be a string.");
	}
	char *units = nullptr;
	size_t unitsLength = 0;
	int unitsStatus = 0;
	if(argc > 3){
		unitsLength = mxGetN(argv[3]) + 1;
		units = static_cast<char *>(malloc(unitsLength));
		unitsStatus = mxGetString(argv[3], units, unitsLength);

	}
	HelicsError err = helicsErrorInitialize();

	HelicsPublication result = helicsFederateRegisterGlobalTypeInput(fed, static_cast<const char *>(key), static_cast<const char *>(type), static_cast<const char *>(units), &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	mxUint64 *rv = mxGetUint64s(_out);
	rv[0] = reinterpret_cast<mxUint64>(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(key);

	free(type);

	free(units);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateGetPublication(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateGetPublication:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateGetPublication:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFederate fed = *(static_cast<HelicsFederate*>(mxGetData(argv[0])));

	if(!mxIsChar(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateGetPublication:TypeError","Argument 2 must be a string.");
	}
	char *key = nullptr;
	size_t keyLength = 0;
	int keyStatus = 0;
	if(argc > 1){
		keyLength = mxGetN(argv[1]) + 1;
		key = static_cast<char *>(malloc(keyLength));
		keyStatus = mxGetString(argv[1], key, keyLength);

	}
	HelicsError err = helicsErrorInitialize();

	HelicsPublication result = helicsFederateGetPublication(fed, static_cast<const char *>(key), &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	mxUint64 *rv = mxGetUint64s(_out);
	rv[0] = reinterpret_cast<mxUint64>(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(key);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateGetPublicationByIndex(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateGetPublicationByIndex:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateGetPublicationByIndex:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFederate fed = *(static_cast<HelicsFederate*>(mxGetData(argv[0])));

	if(!mxIsNumeric(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateGetPublicationByIndex:TypeError","Argument 2 must be of type integer.");
	}
		int index = static_cast<int>(mxGetScalar(argv[1]));

	HelicsError err = helicsErrorInitialize();

	HelicsPublication result = helicsFederateGetPublicationByIndex(fed, index, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	mxUint64 *rv = mxGetUint64s(_out);
	rv[0] = reinterpret_cast<mxUint64>(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateGetInput(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateGetInput:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateGetInput:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFederate fed = *(static_cast<HelicsFederate*>(mxGetData(argv[0])));

	if(!mxIsChar(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateGetInput:TypeError","Argument 2 must be a string.");
	}
	char *key = nullptr;
	size_t keyLength = 0;
	int keyStatus = 0;
	if(argc > 1){
		keyLength = mxGetN(argv[1]) + 1;
		key = static_cast<char *>(malloc(keyLength));
		keyStatus = mxGetString(argv[1], key, keyLength);

	}
	HelicsError err = helicsErrorInitialize();

	HelicsInput result = helicsFederateGetInput(fed, static_cast<const char *>(key), &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	mxUint64 *rv = mxGetUint64s(_out);
	rv[0] = reinterpret_cast<mxUint64>(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(key);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateGetInputByIndex(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateGetInputByIndex:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateGetInputByIndex:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFederate fed = *(static_cast<HelicsFederate*>(mxGetData(argv[0])));

	if(!mxIsNumeric(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateGetInputByIndex:TypeError","Argument 2 must be of type integer.");
	}
		int index = static_cast<int>(mxGetScalar(argv[1]));

	HelicsError err = helicsErrorInitialize();

	HelicsInput result = helicsFederateGetInputByIndex(fed, index, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	mxUint64 *rv = mxGetUint64s(_out);
	rv[0] = reinterpret_cast<mxUint64>(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateGetSubscription(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateGetSubscription:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateGetSubscription:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFederate fed = *(static_cast<HelicsFederate*>(mxGetData(argv[0])));

	if(!mxIsChar(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateGetSubscription:TypeError","Argument 2 must be a string.");
	}
	char *key = nullptr;
	size_t keyLength = 0;
	int keyStatus = 0;
	if(argc > 1){
		keyLength = mxGetN(argv[1]) + 1;
		key = static_cast<char *>(malloc(keyLength));
		keyStatus = mxGetString(argv[1], key, keyLength);

	}
	HelicsError err = helicsErrorInitialize();

	HelicsInput result = helicsFederateGetSubscription(fed, static_cast<const char *>(key), &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	mxUint64 *rv = mxGetUint64s(_out);
	rv[0] = reinterpret_cast<mxUint64>(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(key);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateClearUpdates(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateClearUpdates:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateClearUpdates:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFederate fed = *(static_cast<HelicsFederate*>(mxGetData(argv[0])));

	helicsFederateClearUpdates(fed);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsFederateRegisterFromPublicationJSON(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateRegisterFromPublicationJSON:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateRegisterFromPublicationJSON:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFederate fed = *(static_cast<HelicsFederate*>(mxGetData(argv[0])));

	if(!mxIsChar(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateRegisterFromPublicationJSON:TypeError","Argument 2 must be a string.");
	}
	char *json = nullptr;
	size_t jsonLength = 0;
	int jsonStatus = 0;
	if(argc > 1){
		jsonLength = mxGetN(argv[1]) + 1;
		json = static_cast<char *>(malloc(jsonLength));
		jsonStatus = mxGetString(argv[1], json, jsonLength);

	}
	HelicsError err = helicsErrorInitialize();

	helicsFederateRegisterFromPublicationJSON(fed, static_cast<const char *>(json), &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(json);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederatePublishJSON(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederatePublishJSON:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederatePublishJSON:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFederate fed = *(static_cast<HelicsFederate*>(mxGetData(argv[0])));

	if(!mxIsChar(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederatePublishJSON:TypeError","Argument 2 must be a string.");
	}
	char *json = nullptr;
	size_t jsonLength = 0;
	int jsonStatus = 0;
	if(argc > 1){
		jsonLength = mxGetN(argv[1]) + 1;
		json = static_cast<char *>(malloc(jsonLength));
		jsonStatus = mxGetString(argv[1], json, jsonLength);

	}
	HelicsError err = helicsErrorInitialize();

	helicsFederatePublishJSON(fed, static_cast<const char *>(json), &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(json);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsPublicationIsValid(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsPublicationIsValid:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsPublicationIsValid:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsPublication pub = *(static_cast<HelicsPublication*>(mxGetData(argv[0])));

	HelicsBool result = helicsPublicationIsValid(pub);

	mxArray *_out = mxCreateLogicalMatrix(1, 1);
	mxLogical *rv = mxGetLogicals(_out);
	if(result == HELICS_TRUE){
		rv[0] = true;
	}else{
		rv[0] = false;
	}

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsPublicationPublishBytes(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsPublicationPublishBytes:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsPublicationPublishBytes:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsPublication pub = *(static_cast<HelicsPublication*>(mxGetData(argv[0])));

	if(!mxIsChar(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsPublicationPublishBytes:TypeError","Argument 2 must be a string.");
	}
	char *data = nullptr;
	size_t dataLength = 0;
	int dataStatus = 0;
	if(argc > 1){
		dataLength = mxGetN(argv[1]) + 1;
		data = static_cast<char *>(malloc(dataLength));
		dataStatus = mxGetString(argv[1], data, dataLength);

	}
	int inputDataLength = static_cast<int>(dataLength) - 1;

	HelicsError err = helicsErrorInitialize();

	helicsPublicationPublishBytes(pub, static_cast<void *>(data), inputDataLength, &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsPublicationPublishString(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsPublicationPublishString:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsPublicationPublishString:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsPublication pub = *(static_cast<HelicsPublication*>(mxGetData(argv[0])));

	if(!mxIsChar(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsPublicationPublishString:TypeError","Argument 2 must be a string.");
	}
	char *val = nullptr;
	size_t valLength = 0;
	int valStatus = 0;
	if(argc > 1){
		valLength = mxGetN(argv[1]) + 1;
		val = static_cast<char *>(malloc(valLength));
		valStatus = mxGetString(argv[1], val, valLength);

	}
	HelicsError err = helicsErrorInitialize();

	helicsPublicationPublishString(pub, static_cast<const char *>(val), &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(val);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsPublicationPublishInteger(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsPublicationPublishInteger:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsPublicationPublishInteger:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsPublication pub = *(static_cast<HelicsPublication*>(mxGetData(argv[0])));

	if(mxGetClassID(argv[1]) != mxINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsPublicationPublishInteger:TypeError","Argument 2 must be of type int64.");
	}
mxInt64 *pval = mxGetInt64s(argv[1]);
	int64_t val = static_cast<int64_t>(pval[0]);

	HelicsError err = helicsErrorInitialize();

	helicsPublicationPublishInteger(pub, val, &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsPublicationPublishBoolean(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsPublicationPublishBoolean:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsPublicationPublishBoolean:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsPublication pub = *(static_cast<HelicsPublication*>(mxGetData(argv[0])));

	if(!mxIsLogical(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsPublicationPublishBoolean:TypeError","Argument 2 must be a 0 or 1.");
	}
	mxLogical *pval = mxGetLogicals(argv[1]);
	HelicsBool val = HELICS_FALSE;
	if(pval[0]){
		val = HELICS_TRUE;
	}

	HelicsError err = helicsErrorInitialize();

	helicsPublicationPublishBoolean(pub, val, &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsPublicationPublishDouble(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsPublicationPublishDouble:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsPublicationPublishDouble:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsPublication pub = *(static_cast<HelicsPublication*>(mxGetData(argv[0])));

	if(!mxIsNumeric(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsPublicationPublishDouble:TypeError","Argument 2 must be of type double.");
	}
	double val = mxGetScalar(argv[1]);

	HelicsError err = helicsErrorInitialize();

	helicsPublicationPublishDouble(pub, val, &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsPublicationPublishTime(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsPublicationPublishTime:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsPublicationPublishTime:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsPublication pub = *(static_cast<HelicsPublication*>(mxGetData(argv[0])));

	if(!mxIsNumeric(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsPublicationPublishTime:TypeError","Argument 2 must be of type double.");
	}
	HelicsTime val = (HelicsTime)(mxGetScalar(argv[1]));

	HelicsError err = helicsErrorInitialize();

	helicsPublicationPublishTime(pub, val, &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsPublicationPublishChar(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsPublicationPublishChar:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsPublicationPublishChar:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsPublication pub = *(static_cast<HelicsPublication*>(mxGetData(argv[0])));

	if(!mxIsChar(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsPublicationPublishChar:TypeError","Argument 2 must be a string.");
	}
	char *val = nullptr;
	size_t valLength = 0;
	int valStatus = 0;
	if(argc > 1){
		valLength = mxGetN(argv[1]) + 1;
		val = static_cast<char *>(malloc(valLength));
		valStatus = mxGetString(argv[1], val, valLength);

	}
	HelicsError err = helicsErrorInitialize();

	helicsPublicationPublishChar(pub, val[0], &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(val);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsPublicationPublishComplex(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsPublicationPublishComplex:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsPublicationPublishComplex:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsPublication pub = *(static_cast<HelicsPublication*>(mxGetData(argv[0])));

	if(!mxIsComplex(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsPublicationPublishComplex:TypeError","Argument 2 must be of type complex.");
	}
	mxComplexDouble *complexValue = mxGetComplexDoubles(argv[1]);
	double value[2] = {complexValue[0].real, complexValue[0].imag};

	HelicsError err = helicsErrorInitialize();

	helicsPublicationPublishComplex(pub, value[0], value[1], &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsPublicationPublishVector(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsPublicationPublishVector:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsPublicationPublishVector:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsPublication pub = *(static_cast<HelicsPublication*>(mxGetData(argv[0])));

	if(!mxIsNumeric(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsPublicationPublishVector:TypeError","Argument 2 must be an array of type double.");
	}
	int vectorLength =  static_cast<int>(mxGetNumberOfElements(argv[1]));

	double *vectorInput =  static_cast<double *>(mxGetDoubles(argv[1]));

	HelicsError err = helicsErrorInitialize();

	helicsPublicationPublishVector(pub, static_cast<const double *>(vectorInput), vectorLength, &err);

	mxArray *_out = nullptr;
	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsPublicationPublishComplexVector(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsPublicationPublishComplexVector:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsPublicationPublishComplexVector:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsPublication pub = *(static_cast<HelicsPublication*>(mxGetData(argv[0])));

	if(!mxIsComplex(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsPublicationPublishComplexVector:TypeError","Argument 2 must be an array of type complex.");
	}
	int vectorLength =  static_cast<int>(mxGetN(argv[1])*2);

	double *vectorInput = static_cast<double *>(malloc(vectorLength * sizeof(double)));
	mxComplexDouble *vals = mxGetComplexDoubles(argv[1]);
	for(int i=0; i<vectorLength/2; ++i){
		vectorInput[2*i] = vals[i].real;
		vectorInput[2*i + 1] = vals[i].imag;
	}

	HelicsError err = helicsErrorInitialize();

	helicsPublicationPublishComplexVector(pub, static_cast<const double *>(vectorInput), vectorLength, &err);

	mxArray *_out = nullptr;
	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsPublicationPublishNamedPoint(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 3){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsPublicationPublishNamedPoint:rhs","This function requires 3 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsPublicationPublishNamedPoint:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsPublication pub = *(static_cast<HelicsPublication*>(mxGetData(argv[0])));

	if(!mxIsChar(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsPublicationPublishNamedPoint:TypeError","Argument 2 must be a string.");
	}
	char *field = nullptr;
	size_t fieldLength = 0;
	int fieldStatus = 0;
	if(argc > 1){
		fieldLength = mxGetN(argv[1]) + 1;
		field = static_cast<char *>(malloc(fieldLength));
		fieldStatus = mxGetString(argv[1], field, fieldLength);

	}
	if(!mxIsNumeric(argv[2])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsPublicationPublishNamedPoint:TypeError","Argument 3 must be of type double.");
	}
	double val = mxGetScalar(argv[2]);

	HelicsError err = helicsErrorInitialize();

	helicsPublicationPublishNamedPoint(pub, static_cast<const char *>(field), val, &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(field);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsPublicationAddTarget(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsPublicationAddTarget:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsPublicationAddTarget:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsPublication pub = *(static_cast<HelicsPublication*>(mxGetData(argv[0])));

	if(!mxIsChar(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsPublicationAddTarget:TypeError","Argument 2 must be a string.");
	}
	char *target = nullptr;
	size_t targetLength = 0;
	int targetStatus = 0;
	if(argc > 1){
		targetLength = mxGetN(argv[1]) + 1;
		target = static_cast<char *>(malloc(targetLength));
		targetStatus = mxGetString(argv[1], target, targetLength);

	}
	HelicsError err = helicsErrorInitialize();

	helicsPublicationAddTarget(pub, static_cast<const char *>(target), &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(target);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsInputIsValid(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsInputIsValid:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsInputIsValid:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsInput ipt = *(static_cast<HelicsInput*>(mxGetData(argv[0])));

	HelicsBool result = helicsInputIsValid(ipt);

	mxArray *_out = mxCreateLogicalMatrix(1, 1);
	mxLogical *rv = mxGetLogicals(_out);
	if(result == HELICS_TRUE){
		rv[0] = true;
	}else{
		rv[0] = false;
	}

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsInputAddTarget(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsInputAddTarget:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsInputAddTarget:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsInput ipt = *(static_cast<HelicsInput*>(mxGetData(argv[0])));

	if(!mxIsChar(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsInputAddTarget:TypeError","Argument 2 must be a string.");
	}
	char *target = nullptr;
	size_t targetLength = 0;
	int targetStatus = 0;
	if(argc > 1){
		targetLength = mxGetN(argv[1]) + 1;
		target = static_cast<char *>(malloc(targetLength));
		targetStatus = mxGetString(argv[1], target, targetLength);

	}
	HelicsError err = helicsErrorInitialize();

	helicsInputAddTarget(ipt, static_cast<const char *>(target), &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(target);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsInputGetByteCount(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsInputGetByteCount:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsInputGetByteCount:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsInput ipt = *(static_cast<HelicsInput*>(mxGetData(argv[0])));

	int result = helicsInputGetByteCount(ipt);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
	mxInt64 *rv = mxGetInt64s(_out);
	rv[0] = static_cast<mxInt64>(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsInputGetBytes(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsInputGetBytes:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsInputGetBytes:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsInput ipt = *(static_cast<HelicsInput*>(mxGetData(argv[0])));

	int maxDataLen = helicsInputGetByteCount(ipt) + 2;

	void *data = malloc(maxDataLen);

	int actualSize = 0;

	HelicsError err = helicsErrorInitialize();

	helicsInputGetBytes(ipt, data, maxDataLen, &actualSize, &err);

	mxChar *dataChar = static_cast<mxChar *>(data);
	mwSize dims[2] = {1, static_cast<mwSize>(actualSize)};
	mxArray *_out = mxCreateCharArray(2, dims);
	mxChar *out_data = static_cast<mxChar *>(mxGetData(_out));
	for(int i=0; i<actualSize; ++i){
		out_data[i] = dataChar[i];
	}

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(data);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsInputGetStringSize(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsInputGetStringSize:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsInputGetStringSize:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsInput ipt = *(static_cast<HelicsInput*>(mxGetData(argv[0])));

	int result = helicsInputGetStringSize(ipt);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
	mxInt64 *rv = mxGetInt64s(_out);
	rv[0] = static_cast<mxInt64>(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsInputGetString(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsInputGetString:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsInputGetString:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsInput ipt = *(static_cast<HelicsInput*>(mxGetData(argv[0])));

	int maxStringLen = helicsInputGetStringSize(ipt) + 2;

	char *outputString = static_cast<char *>(malloc(maxStringLen));

	int actualLength = 0;

	HelicsError err = helicsErrorInitialize();

	helicsInputGetString(ipt, outputString, maxStringLen, &actualLength, &err);

	mwSize dims[2] = {1, static_cast<mwSize>(actualLength) - 1};
	mxArray *_out = mxCreateCharArray(2, dims);
	mxChar *out_data = static_cast<mxChar *>(mxGetData(_out));
	for(int i=0; i<(actualLength-1); ++i){
		out_data[i] = outputString[i];
	}

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(outputString);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsInputGetInteger(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsInputGetInteger:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsInputGetInteger:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsInput ipt = *(static_cast<HelicsInput*>(mxGetData(argv[0])));

	HelicsError err = helicsErrorInitialize();

	int64_t result = helicsInputGetInteger(ipt, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
	mxInt64 *rv = mxGetInt64s(_out);
	rv[0] = static_cast<mxInt64>(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsInputGetBoolean(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsInputGetBoolean:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsInputGetBoolean:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsInput ipt = *(static_cast<HelicsInput*>(mxGetData(argv[0])));

	HelicsError err = helicsErrorInitialize();

	HelicsBool result = helicsInputGetBoolean(ipt, &err);

	mxArray *_out = mxCreateLogicalMatrix(1, 1);
	mxLogical *rv = mxGetLogicals(_out);
	if(result == HELICS_TRUE){
		rv[0] = true;
	}else{
		rv[0] = false;
	}

	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsInputGetDouble(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsInputGetDouble:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsInputGetDouble:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsInput ipt = *(static_cast<HelicsInput*>(mxGetData(argv[0])));

	HelicsError err = helicsErrorInitialize();

	double result = helicsInputGetDouble(ipt, &err);

	mxArray *_out = mxCreateDoubleScalar(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsInputGetTime(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsInputGetTime:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsInputGetTime:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsInput ipt = *(static_cast<HelicsInput*>(mxGetData(argv[0])));

	HelicsError err = helicsErrorInitialize();

	HelicsTime result = helicsInputGetTime(ipt, &err);

	mxArray *_out = mxCreateDoubleScalar(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsInputGetChar(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsInputGetChar:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsInputGetChar:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsInput ipt = *(static_cast<HelicsInput*>(mxGetData(argv[0])));

	HelicsError err = helicsErrorInitialize();

	char result = helicsInputGetChar(ipt, &err);

	mxArray *_out = mxCreateString((const char *)(&result));

	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsInputGetComplexObject(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsInputGetComplexObject:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsInputGetComplexObject:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsInput ipt = *(static_cast<HelicsInput*>(mxGetData(argv[0])));

	HelicsError err = helicsErrorInitialize();

	HelicsComplex result = helicsInputGetComplexObject(ipt, &err);

	mxComplexDouble complex_result;
	complex_result.real = result.real;
	complex_result.imag = result.imag;
	mxArray *_out = mxCreateDoubleMatrix(1,1,mxCOMPLEX);
	int status = mxSetComplexDoubles(_out, &complex_result);

	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsInputGetComplex(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsInputGetComplex:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsInputGetComplex:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsInput ipt = *(static_cast<HelicsInput*>(mxGetData(argv[0])));

	double values[2];

	HelicsError err = helicsErrorInitialize();

	helicsInputGetComplex(ipt, &(values[0]), &(values[1]), &err);

	mxArray *_out = mxCreateDoubleMatrix(1,1,mxCOMPLEX);
	mxComplexDouble *complex_result = mxGetComplexDoubles(_out);
	complex_result->real = values[0];
	complex_result->imag = values[1];
	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsInputGetVectorSize(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsInputGetVectorSize:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsInputGetVectorSize:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsInput ipt = *(static_cast<HelicsInput*>(mxGetData(argv[0])));

	int result = helicsInputGetVectorSize(ipt);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
	mxInt64 *rv = mxGetInt64s(_out);
	rv[0] = static_cast<mxInt64>(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsInputGetVector(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsInputGetVector:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsInputGetVector:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsInput ipt = *(static_cast<HelicsInput*>(mxGetData(argv[0])));

	int maxLength = helicsInputGetVectorSize(ipt);

	double *data = static_cast<double *>(malloc(maxLength * sizeof(double)));

	int actualSize = 0;

	HelicsError err = helicsErrorInitialize();

	helicsInputGetVector(ipt, data, maxLength, &actualSize, &err);

	mxDouble *result_data = static_cast<mxDouble *>(mxMalloc(actualSize * sizeof(mxDouble)));
	for(int i=0; i<actualSize; ++i){
		result_data[i] = static_cast<mxDouble>(data[i]);
	}
	mxArray *_out = mxCreateDoubleMatrix(actualSize, 1, mxREAL);
	int status = mxSetDoubles(_out, &(result_data[0]));

	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsInputGetComplexVector(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsInputGetComplexVector:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsInputGetComplexVector:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsInput ipt = *(static_cast<HelicsInput*>(mxGetData(argv[0])));

	int maxLength = helicsInputGetVectorSize(ipt);

	double *data = static_cast<double *>(malloc(maxLength * sizeof(double)));

	int actualSize = 0;

	HelicsError err = helicsErrorInitialize();

	helicsInputGetComplexVector(ipt, data, maxLength, &actualSize, &err);

	mxComplexDouble *result_data = static_cast<mxComplexDouble *>(mxMalloc((actualSize/2)*sizeof(mxComplexDouble)));
	for(int i=0; i<(actualSize/2); ++i){
		result_data[i].real = data[2*(i)];
		result_data[i].imag = data[2*(i) + 1];
	}
	mxArray *_out = mxCreateDoubleMatrix(actualSize/2, 1, mxCOMPLEX);
	int status = mxSetComplexDoubles(_out, &(result_data[0]));

	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsInputGetNamedPoint(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsInputGetNamedPoint:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsInputGetNamedPoint:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsInput ipt = *(static_cast<HelicsInput*>(mxGetData(argv[0])));

	int maxStringLen = helicsInputGetStringSize(ipt) + 2;

	char *outputString = static_cast<char *>(malloc(maxStringLen));

	int actualLength = 0;

	double val = 0;

	HelicsError err = helicsErrorInitialize();

	helicsInputGetNamedPoint(ipt, outputString, maxStringLen, &actualLength, &val, &err);

	mwSize dims[2] = {1, static_cast<mwSize>(actualLength)-1};
	mxArray *_out = mxCreateCharArray(2, dims);
	mxChar *out_data = static_cast<mxChar *>(mxGetData(_out));
	for(int i=0; i<(actualLength-1); ++i){
		out_data[i] = outputString[i];
	}

	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(--resc>=0){
		mxArray *_out1 = mxCreateDoubleScalar(val);
		*resv++ = _out1;
	}

	free(outputString);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsInputSetDefaultBytes(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsInputSetDefaultBytes:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsInputSetDefaultBytes:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsInput ipt = *(static_cast<HelicsInput*>(mxGetData(argv[0])));

	if(!mxIsChar(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsInputSetDefaultBytes:TypeError","Argument 2 must be a string.");
	}
	char *data = nullptr;
	size_t dataLength = 0;
	int dataStatus = 0;
	if(argc > 1){
		dataLength = mxGetN(argv[1]) + 1;
		data = static_cast<char *>(malloc(dataLength));
		dataStatus = mxGetString(argv[1], data, dataLength);

	}
	int inputDataLength = static_cast<int>(dataLength) - 1;

	HelicsError err = helicsErrorInitialize();

	helicsInputSetDefaultBytes(ipt, static_cast<void *>(data), inputDataLength, &err);

	mxArray *_out = nullptr;
	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(data);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsInputSetDefaultString(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsInputSetDefaultString:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsInputSetDefaultString:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsInput ipt = *(static_cast<HelicsInput*>(mxGetData(argv[0])));

	if(!mxIsChar(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsInputSetDefaultString:TypeError","Argument 2 must be a string.");
	}
	char *defaultString = nullptr;
	size_t defaultStringLength = 0;
	int defaultStringStatus = 0;
	if(argc > 1){
		defaultStringLength = mxGetN(argv[1]) + 1;
		defaultString = static_cast<char *>(malloc(defaultStringLength));
		defaultStringStatus = mxGetString(argv[1], defaultString, defaultStringLength);

	}
	HelicsError err = helicsErrorInitialize();

	helicsInputSetDefaultString(ipt, static_cast<const char *>(defaultString), &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(defaultString);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsInputSetDefaultInteger(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsInputSetDefaultInteger:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsInputSetDefaultInteger:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsInput ipt = *(static_cast<HelicsInput*>(mxGetData(argv[0])));

	if(mxGetClassID(argv[1]) != mxINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsInputSetDefaultInteger:TypeError","Argument 2 must be of type int64.");
	}
mxInt64 *pval = mxGetInt64s(argv[1]);
	int64_t val = static_cast<int64_t>(pval[0]);

	HelicsError err = helicsErrorInitialize();

	helicsInputSetDefaultInteger(ipt, val, &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsInputSetDefaultBoolean(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsInputSetDefaultBoolean:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsInputSetDefaultBoolean:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsInput ipt = *(static_cast<HelicsInput*>(mxGetData(argv[0])));

	if(!mxIsLogical(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsInputSetDefaultBoolean:TypeError","Argument 2 must be a 0 or 1.");
	}
	mxLogical *pval = mxGetLogicals(argv[1]);
	HelicsBool val = HELICS_FALSE;
	if(pval[0]){
		val = HELICS_TRUE;
	}

	HelicsError err = helicsErrorInitialize();

	helicsInputSetDefaultBoolean(ipt, val, &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsInputSetDefaultTime(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsInputSetDefaultTime:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsInputSetDefaultTime:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsInput ipt = *(static_cast<HelicsInput*>(mxGetData(argv[0])));

	if(!mxIsNumeric(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsInputSetDefaultTime:TypeError","Argument 2 must be of type double.");
	}
	HelicsTime val = (HelicsTime)(mxGetScalar(argv[1]));

	HelicsError err = helicsErrorInitialize();

	helicsInputSetDefaultTime(ipt, val, &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsInputSetDefaultChar(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsInputSetDefaultChar:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsInputSetDefaultChar:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsInput ipt = *(static_cast<HelicsInput*>(mxGetData(argv[0])));

	if(!mxIsChar(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsInputSetDefaultChar:TypeError","Argument 2 must be a string.");
	}
	char *val = nullptr;
	size_t valLength = 0;
	int valStatus = 0;
	if(argc > 1){
		valLength = mxGetN(argv[1]) + 1;
		val = static_cast<char *>(malloc(valLength));
		valStatus = mxGetString(argv[1], val, valLength);

	}
	HelicsError err = helicsErrorInitialize();

	helicsInputSetDefaultChar(ipt, val[0], &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(val);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsInputSetDefaultDouble(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsInputSetDefaultDouble:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsInputSetDefaultDouble:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsInput ipt = *(static_cast<HelicsInput*>(mxGetData(argv[0])));

	if(!mxIsNumeric(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsInputSetDefaultDouble:TypeError","Argument 2 must be of type double.");
	}
	double val = mxGetScalar(argv[1]);

	HelicsError err = helicsErrorInitialize();

	helicsInputSetDefaultDouble(ipt, val, &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsInputSetDefaultComplex(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsInputSetDefaultComplex:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsInputSetDefaultComplex:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsInput ipt = *(static_cast<HelicsInput*>(mxGetData(argv[0])));

	if(!mxIsComplex(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsInputSetDefaultComplex:TypeError","Argument 2 must be of type complex.");
	}
	mxComplexDouble *value = mxGetComplexDoubles(argv[1]);

	HelicsError err = helicsErrorInitialize();

	helicsInputSetDefaultComplex(ipt, static_cast<double>(value->real), static_cast<double>(value->imag), &err);

	mxArray *_out = nullptr;
	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsInputSetDefaultVector(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsInputSetDefaultVector:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsInputSetDefaultVector:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsInput ipt = *(static_cast<HelicsInput*>(mxGetData(argv[0])));

	if(!mxIsNumeric(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsInputSetDefaultVector:TypeError","Argument 2 must be an array of doubles.");
	}
	int vectorLength =  static_cast<int>(mxGetNumberOfElements(argv[1]));

	double *vectorInput =  static_cast<double *>(mxGetDoubles(argv[1]));

	HelicsError err = helicsErrorInitialize();

	helicsInputSetDefaultVector(ipt, static_cast<const double *>(vectorInput), vectorLength, &err);

	mxArray *_out = nullptr;
	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsInputSetDefaultComplexVector(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsInputSetDefaultComplexVector:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsInputSetDefaultComplexVector:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsInput ipt = *(static_cast<HelicsInput*>(mxGetData(argv[0])));

	if(!mxIsComplex(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsInputSetDefaultComplexVector:TypeError","Argument 2 must be of an array of type complex.");
	}
	int vectorLength =  static_cast<int>(mxGetN(argv[1])*2);

	double *vectorInput = static_cast<double *>(malloc(vectorLength * sizeof(double)));
	mxComplexDouble *vals = mxGetComplexDoubles(argv[1]);
	for(int i=0; i<vectorLength/2; ++i){
		vectorInput[2*i] = vals[i].real;
		vectorInput[2*i + 1] = vals[i].imag;
	}

	HelicsError err = helicsErrorInitialize();

	helicsInputSetDefaultComplexVector(ipt, static_cast<const double *>(vectorInput), vectorLength, &err);

	mxArray *_out = nullptr;
	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsInputSetDefaultNamedPoint(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 3){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsInputSetDefaultNamedPoint:rhs","This function requires 3 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsInputSetDefaultNamedPoint:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsInput ipt = *(static_cast<HelicsInput*>(mxGetData(argv[0])));

	if(!mxIsChar(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsInputSetDefaultNamedPoint:TypeError","Argument 2 must be a string.");
	}
	char *defaultName = nullptr;
	size_t defaultNameLength = 0;
	int defaultNameStatus = 0;
	if(argc > 1){
		defaultNameLength = mxGetN(argv[1]) + 1;
		defaultName = static_cast<char *>(malloc(defaultNameLength));
		defaultNameStatus = mxGetString(argv[1], defaultName, defaultNameLength);

	}
	if(!mxIsNumeric(argv[2])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsInputSetDefaultNamedPoint:TypeError","Argument 3 must be of type double.");
	}
	double val = mxGetScalar(argv[2]);

	HelicsError err = helicsErrorInitialize();

	helicsInputSetDefaultNamedPoint(ipt, static_cast<const char *>(defaultName), val, &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(defaultName);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsInputGetType(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsInputGetType:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsInputGetType:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsInput ipt = *(static_cast<HelicsInput*>(mxGetData(argv[0])));

	const char *result = helicsInputGetType(ipt);

	mxArray *_out = mxCreateString(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsInputGetPublicationType(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsInputGetPublicationType:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsInputGetPublicationType:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsInput ipt = *(static_cast<HelicsInput*>(mxGetData(argv[0])));

	const char *result = helicsInputGetPublicationType(ipt);

	mxArray *_out = mxCreateString(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsInputGetPublicationDataType(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsInputGetPublicationDataType:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsInputGetPublicationDataType:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsInput ipt = *(static_cast<HelicsInput*>(mxGetData(argv[0])));

	int result = helicsInputGetPublicationDataType(ipt);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
	mxInt64 *rv = mxGetInt64s(_out);
	rv[0] = static_cast<mxInt64>(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsPublicationGetType(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsPublicationGetType:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsPublicationGetType:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsPublication pub = *(static_cast<HelicsPublication*>(mxGetData(argv[0])));

	const char *result = helicsPublicationGetType(pub);

	mxArray *_out = mxCreateString(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsInputGetName(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsInputGetName:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsInputGetName:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsInput ipt = *(static_cast<HelicsInput*>(mxGetData(argv[0])));

	const char *result = helicsInputGetName(ipt);

	mxArray *_out = mxCreateString(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsSubscriptionGetTarget(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsSubscriptionGetTarget:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsSubscriptionGetTarget:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsInput ipt = *(static_cast<HelicsInput*>(mxGetData(argv[0])));

	const char *result = helicsSubscriptionGetTarget(ipt);

	mxArray *_out = mxCreateString(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsPublicationGetName(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsPublicationGetName:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsPublicationGetName:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsPublication pub = *(static_cast<HelicsPublication*>(mxGetData(argv[0])));

	const char *result = helicsPublicationGetName(pub);

	mxArray *_out = mxCreateString(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsInputGetUnits(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsInputGetUnits:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsInputGetUnits:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsInput ipt = *(static_cast<HelicsInput*>(mxGetData(argv[0])));

	const char *result = helicsInputGetUnits(ipt);

	mxArray *_out = mxCreateString(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsInputGetInjectionUnits(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsInputGetInjectionUnits:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsInputGetInjectionUnits:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsInput ipt = *(static_cast<HelicsInput*>(mxGetData(argv[0])));

	const char *result = helicsInputGetInjectionUnits(ipt);

	mxArray *_out = mxCreateString(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsInputGetExtractionUnits(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsInputGetExtractionUnits:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsInputGetExtractionUnits:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsInput ipt = *(static_cast<HelicsInput*>(mxGetData(argv[0])));

	const char *result = helicsInputGetExtractionUnits(ipt);

	mxArray *_out = mxCreateString(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsPublicationGetUnits(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsPublicationGetUnits:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsPublicationGetUnits:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsPublication pub = *(static_cast<HelicsPublication*>(mxGetData(argv[0])));

	const char *result = helicsPublicationGetUnits(pub);

	mxArray *_out = mxCreateString(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsInputGetInfo(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsInputGetInfo:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsInputGetInfo:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsInput inp = *(static_cast<HelicsInput*>(mxGetData(argv[0])));

	const char *result = helicsInputGetInfo(inp);

	mxArray *_out = mxCreateString(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsInputSetInfo(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsInputSetInfo:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsInputSetInfo:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsInput inp = *(static_cast<HelicsInput*>(mxGetData(argv[0])));

	if(!mxIsChar(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsInputSetInfo:TypeError","Argument 2 must be a string.");
	}
	char *info = nullptr;
	size_t infoLength = 0;
	int infoStatus = 0;
	if(argc > 1){
		infoLength = mxGetN(argv[1]) + 1;
		info = static_cast<char *>(malloc(infoLength));
		infoStatus = mxGetString(argv[1], info, infoLength);

	}
	HelicsError err = helicsErrorInitialize();

	helicsInputSetInfo(inp, static_cast<const char *>(info), &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(info);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsInputGetTag(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc < 1 || argc > 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsInputGetTag:rhs","This function requires at least 1 arguments and at most 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsInputGetTag:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsInput inp = *(static_cast<HelicsInput*>(mxGetData(argv[0])));

	if(argc > 1){
		if(!mxIsChar(argv[1])){
			mexUnlock();
			mexErrMsgIdAndTxt("MATLAB:helicsInputGetTag:TypeError","Argument 2 must be a string.");
		}
	}
	char *tagname = nullptr;
	size_t tagnameLength = 0;
	int tagnameStatus = 0;
	if(argc > 1){
		tagnameLength = mxGetN(argv[1]) + 1;
		tagname = static_cast<char *>(malloc(tagnameLength));
		tagnameStatus = mxGetString(argv[1], tagname, tagnameLength);

	}
	const char *result = helicsInputGetTag(inp, static_cast<const char *>(tagname));

	mxArray *_out = mxCreateString(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(tagname);
}


void _wrap_helicsInputSetTag(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 3){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsInputSetTag:rhs","This function requires 3 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsInputSetTag:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsInput inp = *(static_cast<HelicsInput*>(mxGetData(argv[0])));

	if(!mxIsChar(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsInputSetTag:TypeError","Argument 2 must be a string.");
	}
	char *tagname = nullptr;
	size_t tagnameLength = 0;
	int tagnameStatus = 0;
	if(argc > 1){
		tagnameLength = mxGetN(argv[1]) + 1;
		tagname = static_cast<char *>(malloc(tagnameLength));
		tagnameStatus = mxGetString(argv[1], tagname, tagnameLength);

	}
	if(!mxIsChar(argv[2])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsInputSetTag:TypeError","Argument 3 must be a string.");
	}
	char *tagvalue = nullptr;
	size_t tagvalueLength = 0;
	int tagvalueStatus = 0;
	if(argc > 2){
		tagvalueLength = mxGetN(argv[2]) + 1;
		tagvalue = static_cast<char *>(malloc(tagvalueLength));
		tagvalueStatus = mxGetString(argv[2], tagvalue, tagvalueLength);

	}
	HelicsError err = helicsErrorInitialize();

	helicsInputSetTag(inp, static_cast<const char *>(tagname), static_cast<const char *>(tagvalue), &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(tagname);

	free(tagvalue);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsPublicationGetInfo(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsPublicationGetInfo:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsPublicationGetInfo:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsPublication pub = *(static_cast<HelicsPublication*>(mxGetData(argv[0])));

	const char *result = helicsPublicationGetInfo(pub);

	mxArray *_out = mxCreateString(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsPublicationSetInfo(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsPublicationSetInfo:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsPublicationSetInfo:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsPublication pub = *(static_cast<HelicsPublication*>(mxGetData(argv[0])));

	if(!mxIsChar(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsPublicationSetInfo:TypeError","Argument 2 must be a string.");
	}
	char *info = nullptr;
	size_t infoLength = 0;
	int infoStatus = 0;
	if(argc > 1){
		infoLength = mxGetN(argv[1]) + 1;
		info = static_cast<char *>(malloc(infoLength));
		infoStatus = mxGetString(argv[1], info, infoLength);

	}
	HelicsError err = helicsErrorInitialize();

	helicsPublicationSetInfo(pub, static_cast<const char *>(info), &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(info);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsPublicationGetTag(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc < 1 || argc > 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsPublicationGetTag:rhs","This function requires at least 1 arguments and at most 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsPublicationGetTag:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsPublication pub = *(static_cast<HelicsPublication*>(mxGetData(argv[0])));

	if(argc > 1){
		if(!mxIsChar(argv[1])){
			mexUnlock();
			mexErrMsgIdAndTxt("MATLAB:helicsPublicationGetTag:TypeError","Argument 2 must be a string.");
		}
	}
	char *tagname = nullptr;
	size_t tagnameLength = 0;
	int tagnameStatus = 0;
	if(argc > 1){
		tagnameLength = mxGetN(argv[1]) + 1;
		tagname = static_cast<char *>(malloc(tagnameLength));
		tagnameStatus = mxGetString(argv[1], tagname, tagnameLength);

	}
	const char *result = helicsPublicationGetTag(pub, static_cast<const char *>(tagname));

	mxArray *_out = mxCreateString(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(tagname);
}


void _wrap_helicsPublicationSetTag(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 3){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsPublicationSetTag:rhs","This function requires 3 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsPublicationSetTag:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsPublication pub = *(static_cast<HelicsPublication*>(mxGetData(argv[0])));

	if(!mxIsChar(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsPublicationSetTag:TypeError","Argument 2 must be a string.");
	}
	char *tagname = nullptr;
	size_t tagnameLength = 0;
	int tagnameStatus = 0;
	if(argc > 1){
		tagnameLength = mxGetN(argv[1]) + 1;
		tagname = static_cast<char *>(malloc(tagnameLength));
		tagnameStatus = mxGetString(argv[1], tagname, tagnameLength);

	}
	if(!mxIsChar(argv[2])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsPublicationSetTag:TypeError","Argument 3 must be a string.");
	}
	char *tagvalue = nullptr;
	size_t tagvalueLength = 0;
	int tagvalueStatus = 0;
	if(argc > 2){
		tagvalueLength = mxGetN(argv[2]) + 1;
		tagvalue = static_cast<char *>(malloc(tagvalueLength));
		tagvalueStatus = mxGetString(argv[2], tagvalue, tagvalueLength);

	}
	HelicsError err = helicsErrorInitialize();

	helicsPublicationSetTag(pub, static_cast<const char *>(tagname), static_cast<const char *>(tagvalue), &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(tagname);

	free(tagvalue);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsInputGetOption(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsInputGetOption:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsInputGetOption:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsInput inp = *(static_cast<HelicsInput*>(mxGetData(argv[0])));

	if(!mxIsNumeric(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsInputGetOption:TypeError","Argument 2 must be of type integer.");
	}
		int option = static_cast<int>(mxGetScalar(argv[1]));

	int result = helicsInputGetOption(inp, option);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
	mxInt64 *rv = mxGetInt64s(_out);
	rv[0] = static_cast<mxInt64>(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsInputSetOption(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 3){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsInputSetOption:rhs","This function requires 3 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsInputSetOption:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsInput inp = *(static_cast<HelicsInput*>(mxGetData(argv[0])));

	if(!mxIsNumeric(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsInputSetOption:TypeError","Argument 2 must be of type integer.");
	}
		int option = static_cast<int>(mxGetScalar(argv[1]));

	if(!mxIsNumeric(argv[2])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsInputSetOption:TypeError","Argument 3 must be of type integer.");
	}
		int value = static_cast<int>(mxGetScalar(argv[2]));

	HelicsError err = helicsErrorInitialize();

	helicsInputSetOption(inp, option, value, &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsPublicationGetOption(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsPublicationGetOption:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsPublicationGetOption:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsPublication pub = *(static_cast<HelicsPublication*>(mxGetData(argv[0])));

	if(!mxIsNumeric(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsPublicationGetOption:TypeError","Argument 2 must be of type integer.");
	}
		int option = static_cast<int>(mxGetScalar(argv[1]));

	int result = helicsPublicationGetOption(pub, option);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
	mxInt64 *rv = mxGetInt64s(_out);
	rv[0] = static_cast<mxInt64>(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsPublicationSetOption(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 3){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsPublicationSetOption:rhs","This function requires 3 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsPublicationSetOption:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsPublication pub = *(static_cast<HelicsPublication*>(mxGetData(argv[0])));

	if(!mxIsNumeric(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsPublicationSetOption:TypeError","Argument 2 must be of type integer.");
	}
		int option = static_cast<int>(mxGetScalar(argv[1]));

	if(!mxIsNumeric(argv[2])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsPublicationSetOption:TypeError","Argument 3 must be of type integer.");
	}
		int val = static_cast<int>(mxGetScalar(argv[2]));

	HelicsError err = helicsErrorInitialize();

	helicsPublicationSetOption(pub, option, val, &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsPublicationSetMinimumChange(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsPublicationSetMinimumChange:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsPublicationSetMinimumChange:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsPublication pub = *(static_cast<HelicsPublication*>(mxGetData(argv[0])));

	if(!mxIsNumeric(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsPublicationSetMinimumChange:TypeError","Argument 2 must be of type double.");
	}
	double tolerance = mxGetScalar(argv[1]);

	HelicsError err = helicsErrorInitialize();

	helicsPublicationSetMinimumChange(pub, tolerance, &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsInputSetMinimumChange(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsInputSetMinimumChange:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsInputSetMinimumChange:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsInput inp = *(static_cast<HelicsInput*>(mxGetData(argv[0])));

	if(!mxIsNumeric(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsInputSetMinimumChange:TypeError","Argument 2 must be of type double.");
	}
	double tolerance = mxGetScalar(argv[1]);

	HelicsError err = helicsErrorInitialize();

	helicsInputSetMinimumChange(inp, tolerance, &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsInputIsUpdated(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsInputIsUpdated:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsInputIsUpdated:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsInput ipt = *(static_cast<HelicsInput*>(mxGetData(argv[0])));

	HelicsBool result = helicsInputIsUpdated(ipt);

	mxArray *_out = mxCreateLogicalMatrix(1, 1);
	mxLogical *rv = mxGetLogicals(_out);
	if(result == HELICS_TRUE){
		rv[0] = true;
	}else{
		rv[0] = false;
	}

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsInputLastUpdateTime(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsInputLastUpdateTime:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsInputLastUpdateTime:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsInput ipt = *(static_cast<HelicsInput*>(mxGetData(argv[0])));

	HelicsTime result = helicsInputLastUpdateTime(ipt);

	mxArray *_out = mxCreateDoubleScalar(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsInputClearUpdate(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsInputClearUpdate:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsInputClearUpdate:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsInput ipt = *(static_cast<HelicsInput*>(mxGetData(argv[0])));

	helicsInputClearUpdate(ipt);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsFederateGetPublicationCount(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateGetPublicationCount:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateGetPublicationCount:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFederate fed = *(static_cast<HelicsFederate*>(mxGetData(argv[0])));

	int result = helicsFederateGetPublicationCount(fed);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
	mxInt64 *rv = mxGetInt64s(_out);
	rv[0] = static_cast<mxInt64>(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsFederateGetInputCount(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateGetInputCount:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateGetInputCount:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFederate fed = *(static_cast<HelicsFederate*>(mxGetData(argv[0])));

	int result = helicsFederateGetInputCount(fed);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
	mxInt64 *rv = mxGetInt64s(_out);
	rv[0] = static_cast<mxInt64>(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsFederateRegisterEndpoint(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 3){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateRegisterEndpoint:rhs","This function requires 3 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateRegisterEndpoint:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFederate fed = *(static_cast<HelicsFederate*>(mxGetData(argv[0])));

	if(!mxIsChar(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateRegisterEndpoint:TypeError","Argument 2 must be a string.");
	}
	char *name = nullptr;
	size_t nameLength = 0;
	int nameStatus = 0;
	if(argc > 1){
		nameLength = mxGetN(argv[1]) + 1;
		name = static_cast<char *>(malloc(nameLength));
		nameStatus = mxGetString(argv[1], name, nameLength);

	}
	if(!mxIsChar(argv[2])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateRegisterEndpoint:TypeError","Argument 3 must be a string.");
	}
	char *type = nullptr;
	size_t typeLength = 0;
	int typeStatus = 0;
	if(argc > 2){
		typeLength = mxGetN(argv[2]) + 1;
		type = static_cast<char *>(malloc(typeLength));
		typeStatus = mxGetString(argv[2], type, typeLength);

	}
	HelicsError err = helicsErrorInitialize();

	HelicsEndpoint result = helicsFederateRegisterEndpoint(fed, static_cast<const char *>(name), static_cast<const char *>(type), &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	mxUint64 *rv = mxGetUint64s(_out);
	rv[0] = reinterpret_cast<mxUint64>(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(name);

	free(type);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateRegisterGlobalEndpoint(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 3){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateRegisterGlobalEndpoint:rhs","This function requires 3 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateRegisterGlobalEndpoint:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFederate fed = *(static_cast<HelicsFederate*>(mxGetData(argv[0])));

	if(!mxIsChar(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateRegisterGlobalEndpoint:TypeError","Argument 2 must be a string.");
	}
	char *name = nullptr;
	size_t nameLength = 0;
	int nameStatus = 0;
	if(argc > 1){
		nameLength = mxGetN(argv[1]) + 1;
		name = static_cast<char *>(malloc(nameLength));
		nameStatus = mxGetString(argv[1], name, nameLength);

	}
	if(!mxIsChar(argv[2])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateRegisterGlobalEndpoint:TypeError","Argument 3 must be a string.");
	}
	char *type = nullptr;
	size_t typeLength = 0;
	int typeStatus = 0;
	if(argc > 2){
		typeLength = mxGetN(argv[2]) + 1;
		type = static_cast<char *>(malloc(typeLength));
		typeStatus = mxGetString(argv[2], type, typeLength);

	}
	HelicsError err = helicsErrorInitialize();

	HelicsEndpoint result = helicsFederateRegisterGlobalEndpoint(fed, static_cast<const char *>(name), static_cast<const char *>(type), &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	mxUint64 *rv = mxGetUint64s(_out);
	rv[0] = reinterpret_cast<mxUint64>(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(name);

	free(type);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateRegisterTargetedEndpoint(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 3){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateRegisterTargetedEndpoint:rhs","This function requires 3 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateRegisterTargetedEndpoint:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFederate fed = *(static_cast<HelicsFederate*>(mxGetData(argv[0])));

	if(!mxIsChar(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateRegisterTargetedEndpoint:TypeError","Argument 2 must be a string.");
	}
	char *name = nullptr;
	size_t nameLength = 0;
	int nameStatus = 0;
	if(argc > 1){
		nameLength = mxGetN(argv[1]) + 1;
		name = static_cast<char *>(malloc(nameLength));
		nameStatus = mxGetString(argv[1], name, nameLength);

	}
	if(!mxIsChar(argv[2])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateRegisterTargetedEndpoint:TypeError","Argument 3 must be a string.");
	}
	char *type = nullptr;
	size_t typeLength = 0;
	int typeStatus = 0;
	if(argc > 2){
		typeLength = mxGetN(argv[2]) + 1;
		type = static_cast<char *>(malloc(typeLength));
		typeStatus = mxGetString(argv[2], type, typeLength);

	}
	HelicsError err = helicsErrorInitialize();

	HelicsEndpoint result = helicsFederateRegisterTargetedEndpoint(fed, static_cast<const char *>(name), static_cast<const char *>(type), &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	mxUint64 *rv = mxGetUint64s(_out);
	rv[0] = reinterpret_cast<mxUint64>(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(name);

	free(type);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateRegisterGlobalTargetedEndpoint(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 3){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateRegisterGlobalTargetedEndpoint:rhs","This function requires 3 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateRegisterGlobalTargetedEndpoint:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFederate fed = *(static_cast<HelicsFederate*>(mxGetData(argv[0])));

	if(!mxIsChar(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateRegisterGlobalTargetedEndpoint:TypeError","Argument 2 must be a string.");
	}
	char *name = nullptr;
	size_t nameLength = 0;
	int nameStatus = 0;
	if(argc > 1){
		nameLength = mxGetN(argv[1]) + 1;
		name = static_cast<char *>(malloc(nameLength));
		nameStatus = mxGetString(argv[1], name, nameLength);

	}
	if(!mxIsChar(argv[2])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateRegisterGlobalTargetedEndpoint:TypeError","Argument 3 must be a string.");
	}
	char *type = nullptr;
	size_t typeLength = 0;
	int typeStatus = 0;
	if(argc > 2){
		typeLength = mxGetN(argv[2]) + 1;
		type = static_cast<char *>(malloc(typeLength));
		typeStatus = mxGetString(argv[2], type, typeLength);

	}
	HelicsError err = helicsErrorInitialize();

	HelicsEndpoint result = helicsFederateRegisterGlobalTargetedEndpoint(fed, static_cast<const char *>(name), static_cast<const char *>(type), &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	mxUint64 *rv = mxGetUint64s(_out);
	rv[0] = reinterpret_cast<mxUint64>(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(name);

	free(type);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateGetEndpoint(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateGetEndpoint:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateGetEndpoint:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFederate fed = *(static_cast<HelicsFederate*>(mxGetData(argv[0])));

	if(!mxIsChar(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateGetEndpoint:TypeError","Argument 2 must be a string.");
	}
	char *name = nullptr;
	size_t nameLength = 0;
	int nameStatus = 0;
	if(argc > 1){
		nameLength = mxGetN(argv[1]) + 1;
		name = static_cast<char *>(malloc(nameLength));
		nameStatus = mxGetString(argv[1], name, nameLength);

	}
	HelicsError err = helicsErrorInitialize();

	HelicsEndpoint result = helicsFederateGetEndpoint(fed, static_cast<const char *>(name), &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	mxUint64 *rv = mxGetUint64s(_out);
	rv[0] = reinterpret_cast<mxUint64>(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(name);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateGetEndpointByIndex(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateGetEndpointByIndex:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateGetEndpointByIndex:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFederate fed = *(static_cast<HelicsFederate*>(mxGetData(argv[0])));

	if(!mxIsNumeric(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateGetEndpointByIndex:TypeError","Argument 2 must be of type integer.");
	}
		int index = static_cast<int>(mxGetScalar(argv[1]));

	HelicsError err = helicsErrorInitialize();

	HelicsEndpoint result = helicsFederateGetEndpointByIndex(fed, index, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	mxUint64 *rv = mxGetUint64s(_out);
	rv[0] = reinterpret_cast<mxUint64>(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsEndpointIsValid(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsEndpointIsValid:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsEndpointIsValid:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsEndpoint endpoint = *(static_cast<HelicsEndpoint*>(mxGetData(argv[0])));

	HelicsBool result = helicsEndpointIsValid(endpoint);

	mxArray *_out = mxCreateLogicalMatrix(1, 1);
	mxLogical *rv = mxGetLogicals(_out);
	if(result == HELICS_TRUE){
		rv[0] = true;
	}else{
		rv[0] = false;
	}

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsEndpointSetDefaultDestination(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsEndpointSetDefaultDestination:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsEndpointSetDefaultDestination:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsEndpoint endpoint = *(static_cast<HelicsEndpoint*>(mxGetData(argv[0])));

	if(!mxIsChar(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsEndpointSetDefaultDestination:TypeError","Argument 2 must be a string.");
	}
	char *dst = nullptr;
	size_t dstLength = 0;
	int dstStatus = 0;
	if(argc > 1){
		dstLength = mxGetN(argv[1]) + 1;
		dst = static_cast<char *>(malloc(dstLength));
		dstStatus = mxGetString(argv[1], dst, dstLength);

	}
	HelicsError err = helicsErrorInitialize();

	helicsEndpointSetDefaultDestination(endpoint, static_cast<const char *>(dst), &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(dst);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsEndpointGetDefaultDestination(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsEndpointGetDefaultDestination:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsEndpointGetDefaultDestination:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsEndpoint endpoint = *(static_cast<HelicsEndpoint*>(mxGetData(argv[0])));

	const char *result = helicsEndpointGetDefaultDestination(endpoint);

	mxArray *_out = mxCreateString(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsEndpointSendBytes(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsEndpointSendBytes:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsEndpointSendBytes:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsEndpoint endpoint = *(static_cast<HelicsEndpoint*>(mxGetData(argv[0])));

	if(!mxIsChar(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsEndpointSendBytes:TypeError","Argument 2 must be a string.");
	}
	char *data = nullptr;
	size_t dataLength = 0;
	int dataStatus = 0;
	if(argc > 1){
		dataLength = mxGetN(argv[1]) + 1;
		data = static_cast<char *>(malloc(dataLength));
		dataStatus = mxGetString(argv[1], data, dataLength);

	}
	int inputDataLength = (int)(dataLength - 1);

	HelicsError err = helicsErrorInitialize();

	helicsEndpointSendBytes(endpoint, (void *)data, inputDataLength, &err);

	mxArray *_out = nullptr;
	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(data);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsEndpointSendBytesTo(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 3){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsEndpointSendBytesTo:rhs","This function requires 3 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsEndpointSendBytesTo:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsEndpoint endpoint = *(static_cast<HelicsEndpoint*>(mxGetData(argv[0])));

	if(!mxIsChar(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsEndpointSendBytesTo:TypeError","Argument 2 must be a string.");
	}
	char *data = nullptr;
	size_t dataLength = 0;
	int dataStatus = 0;
	if(argc > 1){
		dataLength = mxGetN(argv[1]) + 1;
		data = static_cast<char *>(malloc(dataLength));
		dataStatus = mxGetString(argv[1], data, dataLength);

	}
	int inputDataLength = (int)(dataLength - 1);

	if(!mxIsChar(argv[2])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsEndpointSendBytesTo:TypeError","Argument 3 must be a string.");
	}
	char *dst = nullptr;
	size_t dstLength = 0;
	int dstStatus = 0;
	if(argc > 2){
		dstLength = mxGetN(argv[2]) + 1;
		dst = static_cast<char *>(malloc(dstLength));
		dstStatus = mxGetString(argv[2], dst, dstLength);

	}
	HelicsError err = helicsErrorInitialize();

	helicsEndpointSendBytesTo(endpoint, (void *)data, inputDataLength, dst, &err);

	mxArray *_out = nullptr;
	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(data);

	free(dst);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsEndpointSendBytesToAt(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 4){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsEndpointSendBytesToAt:rhs","This function requires 4 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsEndpointSendBytesToAt:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsEndpoint endpoint = *(static_cast<HelicsEndpoint*>(mxGetData(argv[0])));

	if(!mxIsChar(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsEndpointSendBytesToAt:TypeError","Argument 2 must be a string.");
	}
	char *data = nullptr;
	size_t dataLength = 0;
	int dataStatus = 0;
	if(argc > 1){
		dataLength = mxGetN(argv[1]) + 1;
		data = static_cast<char *>(malloc(dataLength));
		dataStatus = mxGetString(argv[1], data, dataLength);

	}
	int inputDataLength = (int)(dataLength - 1);

	if(!mxIsChar(argv[2])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsEndpointSendBytesToAt:TypeError","Argument 3 must be a string.");
	}
	char *dst = nullptr;
	size_t dstLength = 0;
	int dstStatus = 0;
	if(argc > 2){
		dstLength = mxGetN(argv[2]) + 1;
		dst = static_cast<char *>(malloc(dstLength));
		dstStatus = mxGetString(argv[2], dst, dstLength);

	}
	if(!mxIsNumeric(argv[3])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsEndpointSendBytesToAt:TypeError","Argument 4 must be of type double.");
	}
	HelicsTime time = (HelicsTime)(mxGetScalar(argv[3]));

	HelicsError err = helicsErrorInitialize();

	helicsEndpointSendBytesToAt(endpoint, (void *)data, inputDataLength, dst, time, &err);

	mxArray *_out = nullptr;
	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(data);

	free(dst);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsEndpointSendBytesAt(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 3){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsEndpointSendBytesAt:rhs","This function requires 3 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsEndpointSendBytesAt:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsEndpoint endpoint = *(static_cast<HelicsEndpoint*>(mxGetData(argv[0])));

	if(!mxIsChar(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsEndpointSendBytesAt:TypeError","Argument 2 must be a string.");
	}
	char *data = nullptr;
	size_t dataLength = 0;
	int dataStatus = 0;
	if(argc > 1){
		dataLength = mxGetN(argv[1]) + 1;
		data = static_cast<char *>(malloc(dataLength));
		dataStatus = mxGetString(argv[1], data, dataLength);

	}
	int inputDataLength = (int)(dataLength - 1);

	if(!mxIsNumeric(argv[2])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsEndpointSendBytesAt:TypeError","Argument 3 must be of type double.");
	}
	HelicsTime time = (HelicsTime)(mxGetScalar(argv[2]));

	HelicsError err = helicsErrorInitialize();

	helicsEndpointSendBytesAt(endpoint, (void *)data, inputDataLength, time, &err);

	mxArray *_out = nullptr;
	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(data);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsEndpointSendMessage(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsEndpointSendMessage:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsEndpointSendMessage:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsEndpoint endpoint = *(static_cast<HelicsEndpoint*>(mxGetData(argv[0])));

	if(mxGetClassID(argv[1]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsEndpointSendMessage:TypeError","Argument 2 must be of type uint64.");
	}
	HelicsMessage message = *(static_cast<HelicsMessage*>(mxGetData(argv[1])));

	HelicsError err = helicsErrorInitialize();

	helicsEndpointSendMessage(endpoint, message, &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsEndpointSendMessageZeroCopy(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsEndpointSendMessageZeroCopy:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsEndpointSendMessageZeroCopy:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsEndpoint endpoint = *(static_cast<HelicsEndpoint*>(mxGetData(argv[0])));

	if(mxGetClassID(argv[1]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsEndpointSendMessageZeroCopy:TypeError","Argument 2 must be of type uint64.");
	}
	HelicsMessage message = *(static_cast<HelicsMessage*>(mxGetData(argv[1])));

	HelicsError err = helicsErrorInitialize();

	helicsEndpointSendMessageZeroCopy(endpoint, message, &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsEndpointSubscribe(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsEndpointSubscribe:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsEndpointSubscribe:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsEndpoint endpoint = *(static_cast<HelicsEndpoint*>(mxGetData(argv[0])));

	if(!mxIsChar(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsEndpointSubscribe:TypeError","Argument 2 must be a string.");
	}
	char *key = nullptr;
	size_t keyLength = 0;
	int keyStatus = 0;
	if(argc > 1){
		keyLength = mxGetN(argv[1]) + 1;
		key = static_cast<char *>(malloc(keyLength));
		keyStatus = mxGetString(argv[1], key, keyLength);

	}
	HelicsError err = helicsErrorInitialize();

	helicsEndpointSubscribe(endpoint, static_cast<const char *>(key), &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(key);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateHasMessage(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateHasMessage:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateHasMessage:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFederate fed = *(static_cast<HelicsFederate*>(mxGetData(argv[0])));

	HelicsBool result = helicsFederateHasMessage(fed);

	mxArray *_out = mxCreateLogicalMatrix(1, 1);
	mxLogical *rv = mxGetLogicals(_out);
	if(result == HELICS_TRUE){
		rv[0] = true;
	}else{
		rv[0] = false;
	}

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsEndpointHasMessage(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsEndpointHasMessage:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsEndpointHasMessage:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsEndpoint endpoint = *(static_cast<HelicsEndpoint*>(mxGetData(argv[0])));

	HelicsBool result = helicsEndpointHasMessage(endpoint);

	mxArray *_out = mxCreateLogicalMatrix(1, 1);
	mxLogical *rv = mxGetLogicals(_out);
	if(result == HELICS_TRUE){
		rv[0] = true;
	}else{
		rv[0] = false;
	}

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsFederatePendingMessageCount(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederatePendingMessageCount:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederatePendingMessageCount:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFederate fed = *(static_cast<HelicsFederate*>(mxGetData(argv[0])));

	int result = helicsFederatePendingMessageCount(fed);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
	mxInt64 *rv = mxGetInt64s(_out);
	rv[0] = static_cast<mxInt64>(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsEndpointPendingMessageCount(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsEndpointPendingMessageCount:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsEndpointPendingMessageCount:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsEndpoint endpoint = *(static_cast<HelicsEndpoint*>(mxGetData(argv[0])));

	int result = helicsEndpointPendingMessageCount(endpoint);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
	mxInt64 *rv = mxGetInt64s(_out);
	rv[0] = static_cast<mxInt64>(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsEndpointGetMessage(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsEndpointGetMessage:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsEndpointGetMessage:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsEndpoint endpoint = *(static_cast<HelicsEndpoint*>(mxGetData(argv[0])));

	HelicsMessage result = helicsEndpointGetMessage(endpoint);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	mxUint64 *rv = mxGetUint64s(_out);
	rv[0] = reinterpret_cast<mxUint64>(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsEndpointCreateMessage(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsEndpointCreateMessage:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsEndpointCreateMessage:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsEndpoint endpoint = *(static_cast<HelicsEndpoint*>(mxGetData(argv[0])));

	HelicsError err = helicsErrorInitialize();

	HelicsMessage result = helicsEndpointCreateMessage(endpoint, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	mxUint64 *rv = mxGetUint64s(_out);
	rv[0] = reinterpret_cast<mxUint64>(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsEndpointClearMessages(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsEndpointClearMessages:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsEndpointClearMessages:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsEndpoint endpoint = *(static_cast<HelicsEndpoint*>(mxGetData(argv[0])));

	helicsEndpointClearMessages(endpoint);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsFederateGetMessage(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateGetMessage:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateGetMessage:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFederate fed = *(static_cast<HelicsFederate*>(mxGetData(argv[0])));

	HelicsMessage result = helicsFederateGetMessage(fed);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	mxUint64 *rv = mxGetUint64s(_out);
	rv[0] = reinterpret_cast<mxUint64>(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsFederateCreateMessage(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateCreateMessage:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateCreateMessage:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFederate fed = *(static_cast<HelicsFederate*>(mxGetData(argv[0])));

	HelicsError err = helicsErrorInitialize();

	HelicsMessage result = helicsFederateCreateMessage(fed, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	mxUint64 *rv = mxGetUint64s(_out);
	rv[0] = reinterpret_cast<mxUint64>(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateClearMessages(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateClearMessages:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateClearMessages:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFederate fed = *(static_cast<HelicsFederate*>(mxGetData(argv[0])));

	helicsFederateClearMessages(fed);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsEndpointGetType(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsEndpointGetType:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsEndpointGetType:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsEndpoint endpoint = *(static_cast<HelicsEndpoint*>(mxGetData(argv[0])));

	const char *result = helicsEndpointGetType(endpoint);

	mxArray *_out = mxCreateString(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsEndpointGetName(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsEndpointGetName:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsEndpointGetName:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsEndpoint endpoint = *(static_cast<HelicsEndpoint*>(mxGetData(argv[0])));

	const char *result = helicsEndpointGetName(endpoint);

	mxArray *_out = mxCreateString(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsFederateGetEndpointCount(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateGetEndpointCount:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateGetEndpointCount:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFederate fed = *(static_cast<HelicsFederate*>(mxGetData(argv[0])));

	int result = helicsFederateGetEndpointCount(fed);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
	mxInt64 *rv = mxGetInt64s(_out);
	rv[0] = static_cast<mxInt64>(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsEndpointGetInfo(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsEndpointGetInfo:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsEndpointGetInfo:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsEndpoint end = *(static_cast<HelicsEndpoint*>(mxGetData(argv[0])));

	const char *result = helicsEndpointGetInfo(end);

	mxArray *_out = mxCreateString(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsEndpointSetInfo(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsEndpointSetInfo:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsEndpointSetInfo:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsEndpoint endpoint = *(static_cast<HelicsEndpoint*>(mxGetData(argv[0])));

	if(!mxIsChar(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsEndpointSetInfo:TypeError","Argument 2 must be a string.");
	}
	char *info = nullptr;
	size_t infoLength = 0;
	int infoStatus = 0;
	if(argc > 1){
		infoLength = mxGetN(argv[1]) + 1;
		info = static_cast<char *>(malloc(infoLength));
		infoStatus = mxGetString(argv[1], info, infoLength);

	}
	HelicsError err = helicsErrorInitialize();

	helicsEndpointSetInfo(endpoint, static_cast<const char *>(info), &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(info);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsEndpointGetTag(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc < 1 || argc > 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsEndpointGetTag:rhs","This function requires at least 1 arguments and at most 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsEndpointGetTag:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsEndpoint endpoint = *(static_cast<HelicsEndpoint*>(mxGetData(argv[0])));

	if(argc > 1){
		if(!mxIsChar(argv[1])){
			mexUnlock();
			mexErrMsgIdAndTxt("MATLAB:helicsEndpointGetTag:TypeError","Argument 2 must be a string.");
		}
	}
	char *tagname = nullptr;
	size_t tagnameLength = 0;
	int tagnameStatus = 0;
	if(argc > 1){
		tagnameLength = mxGetN(argv[1]) + 1;
		tagname = static_cast<char *>(malloc(tagnameLength));
		tagnameStatus = mxGetString(argv[1], tagname, tagnameLength);

	}
	const char *result = helicsEndpointGetTag(endpoint, static_cast<const char *>(tagname));

	mxArray *_out = mxCreateString(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(tagname);
}


void _wrap_helicsEndpointSetTag(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 3){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsEndpointSetTag:rhs","This function requires 3 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsEndpointSetTag:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsEndpoint endpoint = *(static_cast<HelicsEndpoint*>(mxGetData(argv[0])));

	if(!mxIsChar(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsEndpointSetTag:TypeError","Argument 2 must be a string.");
	}
	char *tagname = nullptr;
	size_t tagnameLength = 0;
	int tagnameStatus = 0;
	if(argc > 1){
		tagnameLength = mxGetN(argv[1]) + 1;
		tagname = static_cast<char *>(malloc(tagnameLength));
		tagnameStatus = mxGetString(argv[1], tagname, tagnameLength);

	}
	if(!mxIsChar(argv[2])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsEndpointSetTag:TypeError","Argument 3 must be a string.");
	}
	char *tagvalue = nullptr;
	size_t tagvalueLength = 0;
	int tagvalueStatus = 0;
	if(argc > 2){
		tagvalueLength = mxGetN(argv[2]) + 1;
		tagvalue = static_cast<char *>(malloc(tagvalueLength));
		tagvalueStatus = mxGetString(argv[2], tagvalue, tagvalueLength);

	}
	HelicsError err = helicsErrorInitialize();

	helicsEndpointSetTag(endpoint, static_cast<const char *>(tagname), static_cast<const char *>(tagvalue), &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(tagname);

	free(tagvalue);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsEndpointSetOption(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 3){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsEndpointSetOption:rhs","This function requires 3 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsEndpointSetOption:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsEndpoint endpoint = *(static_cast<HelicsEndpoint*>(mxGetData(argv[0])));

	if(!mxIsNumeric(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsEndpointSetOption:TypeError","Argument 2 must be of type integer.");
	}
		int option = static_cast<int>(mxGetScalar(argv[1]));

	if(!mxIsNumeric(argv[2])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsEndpointSetOption:TypeError","Argument 3 must be of type integer.");
	}
		int value = static_cast<int>(mxGetScalar(argv[2]));

	HelicsError err = helicsErrorInitialize();

	helicsEndpointSetOption(endpoint, option, value, &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsEndpointGetOption(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsEndpointGetOption:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsEndpointGetOption:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsEndpoint endpoint = *(static_cast<HelicsEndpoint*>(mxGetData(argv[0])));

	if(!mxIsNumeric(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsEndpointGetOption:TypeError","Argument 2 must be of type integer.");
	}
		int option = static_cast<int>(mxGetScalar(argv[1]));

	int result = helicsEndpointGetOption(endpoint, option);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
	mxInt64 *rv = mxGetInt64s(_out);
	rv[0] = static_cast<mxInt64>(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsEndpointAddSourceTarget(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsEndpointAddSourceTarget:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsEndpointAddSourceTarget:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsEndpoint endpoint = *(static_cast<HelicsEndpoint*>(mxGetData(argv[0])));

	if(!mxIsChar(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsEndpointAddSourceTarget:TypeError","Argument 2 must be a string.");
	}
	char *targetEndpoint = nullptr;
	size_t targetEndpointLength = 0;
	int targetEndpointStatus = 0;
	if(argc > 1){
		targetEndpointLength = mxGetN(argv[1]) + 1;
		targetEndpoint = static_cast<char *>(malloc(targetEndpointLength));
		targetEndpointStatus = mxGetString(argv[1], targetEndpoint, targetEndpointLength);

	}
	HelicsError err = helicsErrorInitialize();

	helicsEndpointAddSourceTarget(endpoint, static_cast<const char *>(targetEndpoint), &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(targetEndpoint);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsEndpointAddDestinationTarget(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsEndpointAddDestinationTarget:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsEndpointAddDestinationTarget:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsEndpoint endpoint = *(static_cast<HelicsEndpoint*>(mxGetData(argv[0])));

	if(!mxIsChar(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsEndpointAddDestinationTarget:TypeError","Argument 2 must be a string.");
	}
	char *targetEndpoint = nullptr;
	size_t targetEndpointLength = 0;
	int targetEndpointStatus = 0;
	if(argc > 1){
		targetEndpointLength = mxGetN(argv[1]) + 1;
		targetEndpoint = static_cast<char *>(malloc(targetEndpointLength));
		targetEndpointStatus = mxGetString(argv[1], targetEndpoint, targetEndpointLength);

	}
	HelicsError err = helicsErrorInitialize();

	helicsEndpointAddDestinationTarget(endpoint, static_cast<const char *>(targetEndpoint), &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(targetEndpoint);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsEndpointRemoveTarget(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsEndpointRemoveTarget:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsEndpointRemoveTarget:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsEndpoint endpoint = *(static_cast<HelicsEndpoint*>(mxGetData(argv[0])));

	if(!mxIsChar(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsEndpointRemoveTarget:TypeError","Argument 2 must be a string.");
	}
	char *targetEndpoint = nullptr;
	size_t targetEndpointLength = 0;
	int targetEndpointStatus = 0;
	if(argc > 1){
		targetEndpointLength = mxGetN(argv[1]) + 1;
		targetEndpoint = static_cast<char *>(malloc(targetEndpointLength));
		targetEndpointStatus = mxGetString(argv[1], targetEndpoint, targetEndpointLength);

	}
	HelicsError err = helicsErrorInitialize();

	helicsEndpointRemoveTarget(endpoint, static_cast<const char *>(targetEndpoint), &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(targetEndpoint);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsEndpointAddSourceFilter(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsEndpointAddSourceFilter:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsEndpointAddSourceFilter:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsEndpoint endpoint = *(static_cast<HelicsEndpoint*>(mxGetData(argv[0])));

	if(!mxIsChar(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsEndpointAddSourceFilter:TypeError","Argument 2 must be a string.");
	}
	char *filterName = nullptr;
	size_t filterNameLength = 0;
	int filterNameStatus = 0;
	if(argc > 1){
		filterNameLength = mxGetN(argv[1]) + 1;
		filterName = static_cast<char *>(malloc(filterNameLength));
		filterNameStatus = mxGetString(argv[1], filterName, filterNameLength);

	}
	HelicsError err = helicsErrorInitialize();

	helicsEndpointAddSourceFilter(endpoint, static_cast<const char *>(filterName), &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(filterName);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsEndpointAddDestinationFilter(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsEndpointAddDestinationFilter:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsEndpointAddDestinationFilter:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsEndpoint endpoint = *(static_cast<HelicsEndpoint*>(mxGetData(argv[0])));

	if(!mxIsChar(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsEndpointAddDestinationFilter:TypeError","Argument 2 must be a string.");
	}
	char *filterName = nullptr;
	size_t filterNameLength = 0;
	int filterNameStatus = 0;
	if(argc > 1){
		filterNameLength = mxGetN(argv[1]) + 1;
		filterName = static_cast<char *>(malloc(filterNameLength));
		filterNameStatus = mxGetString(argv[1], filterName, filterNameLength);

	}
	HelicsError err = helicsErrorInitialize();

	helicsEndpointAddDestinationFilter(endpoint, static_cast<const char *>(filterName), &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(filterName);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsMessageGetSource(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsMessageGetSource:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsMessageGetSource:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsMessage message = *(static_cast<HelicsMessage*>(mxGetData(argv[0])));

	const char *result = helicsMessageGetSource(message);

	mxArray *_out = mxCreateString(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsMessageGetDestination(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsMessageGetDestination:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsMessageGetDestination:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsMessage message = *(static_cast<HelicsMessage*>(mxGetData(argv[0])));

	const char *result = helicsMessageGetDestination(message);

	mxArray *_out = mxCreateString(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsMessageGetOriginalSource(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsMessageGetOriginalSource:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsMessageGetOriginalSource:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsMessage message = *(static_cast<HelicsMessage*>(mxGetData(argv[0])));

	const char *result = helicsMessageGetOriginalSource(message);

	mxArray *_out = mxCreateString(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsMessageGetOriginalDestination(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsMessageGetOriginalDestination:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsMessageGetOriginalDestination:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsMessage message = *(static_cast<HelicsMessage*>(mxGetData(argv[0])));

	const char *result = helicsMessageGetOriginalDestination(message);

	mxArray *_out = mxCreateString(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsMessageGetTime(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsMessageGetTime:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsMessageGetTime:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsMessage message = *(static_cast<HelicsMessage*>(mxGetData(argv[0])));

	HelicsTime result = helicsMessageGetTime(message);

	mxArray *_out = mxCreateDoubleScalar(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsMessageGetString(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsMessageGetString:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsMessageGetString:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsMessage message = *(static_cast<HelicsMessage*>(mxGetData(argv[0])));

	const char *result = helicsMessageGetString(message);

	mxArray *_out = mxCreateString(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsMessageGetMessageID(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsMessageGetMessageID:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsMessageGetMessageID:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsMessage message = *(static_cast<HelicsMessage*>(mxGetData(argv[0])));

	int result = helicsMessageGetMessageID(message);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
	mxInt64 *rv = mxGetInt64s(_out);
	rv[0] = static_cast<mxInt64>(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsMessageGetFlagOption(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsMessageGetFlagOption:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsMessageGetFlagOption:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsMessage message = *(static_cast<HelicsMessage*>(mxGetData(argv[0])));

	if(!mxIsNumeric(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsMessageGetFlagOption:TypeError","Argument 2 must be of type integer.");
	}
		int flag = static_cast<int>(mxGetScalar(argv[1]));

	HelicsBool result = helicsMessageGetFlagOption(message, flag);

	mxArray *_out = mxCreateLogicalMatrix(1, 1);
	mxLogical *rv = mxGetLogicals(_out);
	if(result == HELICS_TRUE){
		rv[0] = true;
	}else{
		rv[0] = false;
	}

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsMessageGetByteCount(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsMessageGetByteCount:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsMessageGetByteCount:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsMessage message = *(static_cast<HelicsMessage*>(mxGetData(argv[0])));

	int result = helicsMessageGetByteCount(message);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
	mxInt64 *rv = mxGetInt64s(_out);
	rv[0] = static_cast<mxInt64>(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsMessageGetBytes(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsMessageGetBytes:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsMessageGetBytes:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsMessage message = *(static_cast<HelicsMessage*>(mxGetData(argv[0])));

	int maxMessageLength = helicsMessageGetByteCount(message) + 2;

	char *data = static_cast<char *>(malloc(maxMessageLength));

	int actualSize = 0;

	HelicsError err = helicsErrorInitialize();

	helicsMessageGetBytes(message, static_cast<void *>(data), maxMessageLength, &actualSize, &err);

	mwSize dims[2] = {1,static_cast<mwSize>(actualSize)};
	mxArray *_out = mxCreateCharArray(2,dims);
	mxChar *out_data = static_cast<mxChar*>(mxGetData(_out));
	for(int i=0; i<actualSize; ++i){
		out_data[i] = data[i];
	}

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(data);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsMessageGetBytesPointer(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsMessageGetBytesPointer:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsMessageGetBytesPointer:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsMessage message = *(static_cast<HelicsMessage*>(mxGetData(argv[0])));

	void *result = helicsMessageGetBytesPointer(message);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	mxUint64 *rv = mxGetUint64s(_out);
	rv[0] = reinterpret_cast<mxUint64>(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsMessageIsValid(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsMessageIsValid:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsMessageIsValid:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsMessage message = *(static_cast<HelicsMessage*>(mxGetData(argv[0])));

	HelicsBool result = helicsMessageIsValid(message);

	mxArray *_out = mxCreateLogicalMatrix(1, 1);
	mxLogical *rv = mxGetLogicals(_out);
	if(result == HELICS_TRUE){
		rv[0] = true;
	}else{
		rv[0] = false;
	}

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsMessageSetSource(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsMessageSetSource:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsMessageSetSource:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsMessage message = *(static_cast<HelicsMessage*>(mxGetData(argv[0])));

	if(!mxIsChar(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsMessageSetSource:TypeError","Argument 2 must be a string.");
	}
	char *src = nullptr;
	size_t srcLength = 0;
	int srcStatus = 0;
	if(argc > 1){
		srcLength = mxGetN(argv[1]) + 1;
		src = static_cast<char *>(malloc(srcLength));
		srcStatus = mxGetString(argv[1], src, srcLength);

	}
	HelicsError err = helicsErrorInitialize();

	helicsMessageSetSource(message, static_cast<const char *>(src), &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(src);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsMessageSetDestination(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsMessageSetDestination:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsMessageSetDestination:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsMessage message = *(static_cast<HelicsMessage*>(mxGetData(argv[0])));

	if(!mxIsChar(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsMessageSetDestination:TypeError","Argument 2 must be a string.");
	}
	char *dst = nullptr;
	size_t dstLength = 0;
	int dstStatus = 0;
	if(argc > 1){
		dstLength = mxGetN(argv[1]) + 1;
		dst = static_cast<char *>(malloc(dstLength));
		dstStatus = mxGetString(argv[1], dst, dstLength);

	}
	HelicsError err = helicsErrorInitialize();

	helicsMessageSetDestination(message, static_cast<const char *>(dst), &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(dst);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsMessageSetOriginalSource(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsMessageSetOriginalSource:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsMessageSetOriginalSource:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsMessage message = *(static_cast<HelicsMessage*>(mxGetData(argv[0])));

	if(!mxIsChar(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsMessageSetOriginalSource:TypeError","Argument 2 must be a string.");
	}
	char *src = nullptr;
	size_t srcLength = 0;
	int srcStatus = 0;
	if(argc > 1){
		srcLength = mxGetN(argv[1]) + 1;
		src = static_cast<char *>(malloc(srcLength));
		srcStatus = mxGetString(argv[1], src, srcLength);

	}
	HelicsError err = helicsErrorInitialize();

	helicsMessageSetOriginalSource(message, static_cast<const char *>(src), &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(src);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsMessageSetOriginalDestination(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsMessageSetOriginalDestination:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsMessageSetOriginalDestination:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsMessage message = *(static_cast<HelicsMessage*>(mxGetData(argv[0])));

	if(!mxIsChar(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsMessageSetOriginalDestination:TypeError","Argument 2 must be a string.");
	}
	char *dst = nullptr;
	size_t dstLength = 0;
	int dstStatus = 0;
	if(argc > 1){
		dstLength = mxGetN(argv[1]) + 1;
		dst = static_cast<char *>(malloc(dstLength));
		dstStatus = mxGetString(argv[1], dst, dstLength);

	}
	HelicsError err = helicsErrorInitialize();

	helicsMessageSetOriginalDestination(message, static_cast<const char *>(dst), &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(dst);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsMessageSetTime(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsMessageSetTime:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsMessageSetTime:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsMessage message = *(static_cast<HelicsMessage*>(mxGetData(argv[0])));

	if(!mxIsNumeric(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsMessageSetTime:TypeError","Argument 2 must be of type double.");
	}
	HelicsTime time = (HelicsTime)(mxGetScalar(argv[1]));

	HelicsError err = helicsErrorInitialize();

	helicsMessageSetTime(message, time, &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsMessageResize(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsMessageResize:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsMessageResize:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsMessage message = *(static_cast<HelicsMessage*>(mxGetData(argv[0])));

	if(!mxIsNumeric(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsMessageResize:TypeError","Argument 2 must be of type integer.");
	}
		int newSize = static_cast<int>(mxGetScalar(argv[1]));

	HelicsError err = helicsErrorInitialize();

	helicsMessageResize(message, newSize, &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsMessageReserve(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsMessageReserve:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsMessageReserve:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsMessage message = *(static_cast<HelicsMessage*>(mxGetData(argv[0])));

	if(!mxIsNumeric(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsMessageReserve:TypeError","Argument 2 must be of type integer.");
	}
		int reserveSize = static_cast<int>(mxGetScalar(argv[1]));

	HelicsError err = helicsErrorInitialize();

	helicsMessageReserve(message, reserveSize, &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsMessageSetMessageID(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsMessageSetMessageID:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsMessageSetMessageID:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsMessage message = *(static_cast<HelicsMessage*>(mxGetData(argv[0])));

	if(mxGetClassID(argv[1]) != mxINT32_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsMessageSetMessageID:TypeError","Argument 2 must be of type int32.");
	}
mxInt32 *pmessageID = mxGetInt32s(argv[1]);
	int32_t messageID = static_cast<int32_t>(pmessageID[0]);

	HelicsError err = helicsErrorInitialize();

	helicsMessageSetMessageID(message, messageID, &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsMessageClearFlags(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsMessageClearFlags:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsMessageClearFlags:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsMessage message = *(static_cast<HelicsMessage*>(mxGetData(argv[0])));

	helicsMessageClearFlags(message);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsMessageSetFlagOption(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 3){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsMessageSetFlagOption:rhs","This function requires 3 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsMessageSetFlagOption:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsMessage message = *(static_cast<HelicsMessage*>(mxGetData(argv[0])));

	if(!mxIsNumeric(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsMessageSetFlagOption:TypeError","Argument 2 must be of type integer.");
	}
		int flag = static_cast<int>(mxGetScalar(argv[1]));

	if(!mxIsLogical(argv[2])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsMessageSetFlagOption:TypeError","Argument 3 must be a 0 or 1.");
	}
	mxLogical *pflagValue = mxGetLogicals(argv[2]);
	HelicsBool flagValue = HELICS_FALSE;
	if(pflagValue[0]){
		flagValue = HELICS_TRUE;
	}

	HelicsError err = helicsErrorInitialize();

	helicsMessageSetFlagOption(message, flag, flagValue, &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsMessageSetString(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsMessageSetString:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsMessageSetString:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsMessage message = *(static_cast<HelicsMessage*>(mxGetData(argv[0])));

	if(!mxIsChar(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsMessageSetString:TypeError","Argument 2 must be a string.");
	}
	char *data = nullptr;
	size_t dataLength = 0;
	int dataStatus = 0;
	if(argc > 1){
		dataLength = mxGetN(argv[1]) + 1;
		data = static_cast<char *>(malloc(dataLength));
		dataStatus = mxGetString(argv[1], data, dataLength);

	}
	HelicsError err = helicsErrorInitialize();

	helicsMessageSetString(message, static_cast<const char *>(data), &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(data);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsMessageSetData(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsMessageSetData:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsMessageSetData:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsMessage message = *(static_cast<HelicsMessage*>(mxGetData(argv[0])));

	if(!mxIsChar(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsMessageSetData:TypeError","Argument 2 must be a string.");
	}
	char *data = nullptr;
	size_t dataLength = 0;
	int dataStatus = 0;
	if(argc > 1){
		dataLength = mxGetN(argv[1]) + 1;
		data = static_cast<char *>(malloc(dataLength));
		dataStatus = mxGetString(argv[1], data, dataLength);

	}
	int inputDataLength = static_cast<int>(dataLength) - 1;

	HelicsError err = helicsErrorInitialize();

	helicsMessageSetData(message, static_cast<void *>(data), inputDataLength, &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsMessageAppendData(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsMessageAppendData:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsMessageAppendData:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsMessage message = *(static_cast<HelicsMessage*>(mxGetData(argv[0])));

	if(!mxIsChar(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsMessageAppendData:TypeError","Argument 2 must be a string.");
	}
	char *data = nullptr;
	size_t dataLength = 0;
	int dataStatus = 0;
	if(argc > 1){
		dataLength = mxGetN(argv[1]) + 1;
		data = static_cast<char *>(malloc(dataLength));
		dataStatus = mxGetString(argv[1], data, dataLength);

	}
	int inputDataLength = static_cast<int>(dataLength) - 1;

	HelicsError err = helicsErrorInitialize();

	helicsMessageAppendData(message, static_cast<void *>(data), inputDataLength, &err);

	mxArray *_out = nullptr;
	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsMessageCopy(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsMessageCopy:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsMessageCopy:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsMessage src_message = *(static_cast<HelicsMessage*>(mxGetData(argv[0])));

	if(mxGetClassID(argv[1]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsMessageCopy:TypeError","Argument 2 must be of type uint64.");
	}
	HelicsMessage dst_message = *(static_cast<HelicsMessage*>(mxGetData(argv[1])));

	HelicsError err = helicsErrorInitialize();

	helicsMessageCopy(src_message, dst_message, &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsMessageClone(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsMessageClone:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsMessageClone:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsMessage message = *(static_cast<HelicsMessage*>(mxGetData(argv[0])));

	HelicsError err = helicsErrorInitialize();

	HelicsMessage result = helicsMessageClone(message, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	mxUint64 *rv = mxGetUint64s(_out);
	rv[0] = reinterpret_cast<mxUint64>(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsMessageFree(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsMessageFree:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsMessageFree:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsMessage message = *(static_cast<HelicsMessage*>(mxGetData(argv[0])));

	helicsMessageFree(message);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsMessageClear(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsMessageClear:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsMessageClear:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsMessage message = *(static_cast<HelicsMessage*>(mxGetData(argv[0])));

	HelicsError err = helicsErrorInitialize();

	helicsMessageClear(message, &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateRegisterFilter(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 3){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateRegisterFilter:rhs","This function requires 3 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateRegisterFilter:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFederate fed = *(static_cast<HelicsFederate*>(mxGetData(argv[0])));

	if(!mxIsNumeric(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateRegisterFilter:TypeError","Argument 2 must be of type int32.");
	}
	int typeInt = static_cast<int>(mxGetScalar(argv[1]));
	HelicsFilterTypes type = static_cast<HelicsFilterTypes>(typeInt);

	if(!mxIsChar(argv[2])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateRegisterFilter:TypeError","Argument 3 must be a string.");
	}
	char *name = nullptr;
	size_t nameLength = 0;
	int nameStatus = 0;
	if(argc > 2){
		nameLength = mxGetN(argv[2]) + 1;
		name = static_cast<char *>(malloc(nameLength));
		nameStatus = mxGetString(argv[2], name, nameLength);

	}
	HelicsError err = helicsErrorInitialize();

	HelicsFilter result = helicsFederateRegisterFilter(fed, type, static_cast<const char *>(name), &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	mxUint64 *rv = mxGetUint64s(_out);
	rv[0] = reinterpret_cast<mxUint64>(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(name);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateRegisterGlobalFilter(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 3){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateRegisterGlobalFilter:rhs","This function requires 3 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateRegisterGlobalFilter:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFederate fed = *(static_cast<HelicsFederate*>(mxGetData(argv[0])));

	if(!mxIsNumeric(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateRegisterGlobalFilter:TypeError","Argument 2 must be of type int32.");
	}
	int typeInt = static_cast<int>(mxGetScalar(argv[1]));
	HelicsFilterTypes type = static_cast<HelicsFilterTypes>(typeInt);

	if(!mxIsChar(argv[2])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateRegisterGlobalFilter:TypeError","Argument 3 must be a string.");
	}
	char *name = nullptr;
	size_t nameLength = 0;
	int nameStatus = 0;
	if(argc > 2){
		nameLength = mxGetN(argv[2]) + 1;
		name = static_cast<char *>(malloc(nameLength));
		nameStatus = mxGetString(argv[2], name, nameLength);

	}
	HelicsError err = helicsErrorInitialize();

	HelicsFilter result = helicsFederateRegisterGlobalFilter(fed, type, static_cast<const char *>(name), &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	mxUint64 *rv = mxGetUint64s(_out);
	rv[0] = reinterpret_cast<mxUint64>(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(name);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateRegisterCloningFilter(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateRegisterCloningFilter:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateRegisterCloningFilter:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFederate fed = *(static_cast<HelicsFederate*>(mxGetData(argv[0])));

	if(!mxIsChar(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateRegisterCloningFilter:TypeError","Argument 2 must be a string.");
	}
	char *name = nullptr;
	size_t nameLength = 0;
	int nameStatus = 0;
	if(argc > 1){
		nameLength = mxGetN(argv[1]) + 1;
		name = static_cast<char *>(malloc(nameLength));
		nameStatus = mxGetString(argv[1], name, nameLength);

	}
	HelicsError err = helicsErrorInitialize();

	HelicsFilter result = helicsFederateRegisterCloningFilter(fed, static_cast<const char *>(name), &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	mxUint64 *rv = mxGetUint64s(_out);
	rv[0] = reinterpret_cast<mxUint64>(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(name);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateRegisterGlobalCloningFilter(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateRegisterGlobalCloningFilter:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateRegisterGlobalCloningFilter:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFederate fed = *(static_cast<HelicsFederate*>(mxGetData(argv[0])));

	if(!mxIsChar(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateRegisterGlobalCloningFilter:TypeError","Argument 2 must be a string.");
	}
	char *name = nullptr;
	size_t nameLength = 0;
	int nameStatus = 0;
	if(argc > 1){
		nameLength = mxGetN(argv[1]) + 1;
		name = static_cast<char *>(malloc(nameLength));
		nameStatus = mxGetString(argv[1], name, nameLength);

	}
	HelicsError err = helicsErrorInitialize();

	HelicsFilter result = helicsFederateRegisterGlobalCloningFilter(fed, static_cast<const char *>(name), &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	mxUint64 *rv = mxGetUint64s(_out);
	rv[0] = reinterpret_cast<mxUint64>(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(name);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsCoreRegisterFilter(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 3){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsCoreRegisterFilter:rhs","This function requires 3 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsCoreRegisterFilter:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsCore core = *(static_cast<HelicsCore*>(mxGetData(argv[0])));

	if(!mxIsNumeric(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsCoreRegisterFilter:TypeError","Argument 2 must be of type int32.");
	}
	int typeInt = static_cast<int>(mxGetScalar(argv[1]));
	HelicsFilterTypes type = static_cast<HelicsFilterTypes>(typeInt);

	if(!mxIsChar(argv[2])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsCoreRegisterFilter:TypeError","Argument 3 must be a string.");
	}
	char *name = nullptr;
	size_t nameLength = 0;
	int nameStatus = 0;
	if(argc > 2){
		nameLength = mxGetN(argv[2]) + 1;
		name = static_cast<char *>(malloc(nameLength));
		nameStatus = mxGetString(argv[2], name, nameLength);

	}
	HelicsError err = helicsErrorInitialize();

	HelicsFilter result = helicsCoreRegisterFilter(core, type, static_cast<const char *>(name), &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	mxUint64 *rv = mxGetUint64s(_out);
	rv[0] = reinterpret_cast<mxUint64>(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(name);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsCoreRegisterCloningFilter(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsCoreRegisterCloningFilter:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsCoreRegisterCloningFilter:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsCore core = *(static_cast<HelicsCore*>(mxGetData(argv[0])));

	if(!mxIsChar(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsCoreRegisterCloningFilter:TypeError","Argument 2 must be a string.");
	}
	char *name = nullptr;
	size_t nameLength = 0;
	int nameStatus = 0;
	if(argc > 1){
		nameLength = mxGetN(argv[1]) + 1;
		name = static_cast<char *>(malloc(nameLength));
		nameStatus = mxGetString(argv[1], name, nameLength);

	}
	HelicsError err = helicsErrorInitialize();

	HelicsFilter result = helicsCoreRegisterCloningFilter(core, static_cast<const char *>(name), &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	mxUint64 *rv = mxGetUint64s(_out);
	rv[0] = reinterpret_cast<mxUint64>(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(name);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateGetFilterCount(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateGetFilterCount:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateGetFilterCount:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFederate fed = *(static_cast<HelicsFederate*>(mxGetData(argv[0])));

	int result = helicsFederateGetFilterCount(fed);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
	mxInt64 *rv = mxGetInt64s(_out);
	rv[0] = static_cast<mxInt64>(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsFederateGetFilter(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateGetFilter:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateGetFilter:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFederate fed = *(static_cast<HelicsFederate*>(mxGetData(argv[0])));

	if(!mxIsChar(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateGetFilter:TypeError","Argument 2 must be a string.");
	}
	char *name = nullptr;
	size_t nameLength = 0;
	int nameStatus = 0;
	if(argc > 1){
		nameLength = mxGetN(argv[1]) + 1;
		name = static_cast<char *>(malloc(nameLength));
		nameStatus = mxGetString(argv[1], name, nameLength);

	}
	HelicsError err = helicsErrorInitialize();

	HelicsFilter result = helicsFederateGetFilter(fed, static_cast<const char *>(name), &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	mxUint64 *rv = mxGetUint64s(_out);
	rv[0] = reinterpret_cast<mxUint64>(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(name);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateGetFilterByIndex(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateGetFilterByIndex:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateGetFilterByIndex:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFederate fed = *(static_cast<HelicsFederate*>(mxGetData(argv[0])));

	if(!mxIsNumeric(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateGetFilterByIndex:TypeError","Argument 2 must be of type integer.");
	}
		int index = static_cast<int>(mxGetScalar(argv[1]));

	HelicsError err = helicsErrorInitialize();

	HelicsFilter result = helicsFederateGetFilterByIndex(fed, index, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	mxUint64 *rv = mxGetUint64s(_out);
	rv[0] = reinterpret_cast<mxUint64>(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFilterIsValid(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFilterIsValid:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFilterIsValid:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFilter filt = *(static_cast<HelicsFilter*>(mxGetData(argv[0])));

	HelicsBool result = helicsFilterIsValid(filt);

	mxArray *_out = mxCreateLogicalMatrix(1, 1);
	mxLogical *rv = mxGetLogicals(_out);
	if(result == HELICS_TRUE){
		rv[0] = true;
	}else{
		rv[0] = false;
	}

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsFilterGetName(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFilterGetName:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFilterGetName:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFilter filt = *(static_cast<HelicsFilter*>(mxGetData(argv[0])));

	const char *result = helicsFilterGetName(filt);

	mxArray *_out = mxCreateString(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsFilterSet(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 3){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFilterSet:rhs","This function requires 3 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFilterSet:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFilter filt = *(static_cast<HelicsFilter*>(mxGetData(argv[0])));

	if(!mxIsChar(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFilterSet:TypeError","Argument 2 must be a string.");
	}
	char *prop = nullptr;
	size_t propLength = 0;
	int propStatus = 0;
	if(argc > 1){
		propLength = mxGetN(argv[1]) + 1;
		prop = static_cast<char *>(malloc(propLength));
		propStatus = mxGetString(argv[1], prop, propLength);

	}
	if(!mxIsNumeric(argv[2])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFilterSet:TypeError","Argument 3 must be of type double.");
	}
	double val = mxGetScalar(argv[2]);

	HelicsError err = helicsErrorInitialize();

	helicsFilterSet(filt, static_cast<const char *>(prop), val, &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(prop);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFilterSetString(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 3){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFilterSetString:rhs","This function requires 3 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFilterSetString:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFilter filt = *(static_cast<HelicsFilter*>(mxGetData(argv[0])));

	if(!mxIsChar(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFilterSetString:TypeError","Argument 2 must be a string.");
	}
	char *prop = nullptr;
	size_t propLength = 0;
	int propStatus = 0;
	if(argc > 1){
		propLength = mxGetN(argv[1]) + 1;
		prop = static_cast<char *>(malloc(propLength));
		propStatus = mxGetString(argv[1], prop, propLength);

	}
	if(!mxIsChar(argv[2])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFilterSetString:TypeError","Argument 3 must be a string.");
	}
	char *val = nullptr;
	size_t valLength = 0;
	int valStatus = 0;
	if(argc > 2){
		valLength = mxGetN(argv[2]) + 1;
		val = static_cast<char *>(malloc(valLength));
		valStatus = mxGetString(argv[2], val, valLength);

	}
	HelicsError err = helicsErrorInitialize();

	helicsFilterSetString(filt, static_cast<const char *>(prop), static_cast<const char *>(val), &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(prop);

	free(val);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFilterAddDestinationTarget(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFilterAddDestinationTarget:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFilterAddDestinationTarget:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFilter filt = *(static_cast<HelicsFilter*>(mxGetData(argv[0])));

	if(!mxIsChar(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFilterAddDestinationTarget:TypeError","Argument 2 must be a string.");
	}
	char *dst = nullptr;
	size_t dstLength = 0;
	int dstStatus = 0;
	if(argc > 1){
		dstLength = mxGetN(argv[1]) + 1;
		dst = static_cast<char *>(malloc(dstLength));
		dstStatus = mxGetString(argv[1], dst, dstLength);

	}
	HelicsError err = helicsErrorInitialize();

	helicsFilterAddDestinationTarget(filt, static_cast<const char *>(dst), &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(dst);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFilterAddSourceTarget(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFilterAddSourceTarget:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFilterAddSourceTarget:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFilter filt = *(static_cast<HelicsFilter*>(mxGetData(argv[0])));

	if(!mxIsChar(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFilterAddSourceTarget:TypeError","Argument 2 must be a string.");
	}
	char *source = nullptr;
	size_t sourceLength = 0;
	int sourceStatus = 0;
	if(argc > 1){
		sourceLength = mxGetN(argv[1]) + 1;
		source = static_cast<char *>(malloc(sourceLength));
		sourceStatus = mxGetString(argv[1], source, sourceLength);

	}
	HelicsError err = helicsErrorInitialize();

	helicsFilterAddSourceTarget(filt, static_cast<const char *>(source), &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(source);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFilterAddDeliveryEndpoint(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFilterAddDeliveryEndpoint:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFilterAddDeliveryEndpoint:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFilter filt = *(static_cast<HelicsFilter*>(mxGetData(argv[0])));

	if(!mxIsChar(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFilterAddDeliveryEndpoint:TypeError","Argument 2 must be a string.");
	}
	char *deliveryEndpoint = nullptr;
	size_t deliveryEndpointLength = 0;
	int deliveryEndpointStatus = 0;
	if(argc > 1){
		deliveryEndpointLength = mxGetN(argv[1]) + 1;
		deliveryEndpoint = static_cast<char *>(malloc(deliveryEndpointLength));
		deliveryEndpointStatus = mxGetString(argv[1], deliveryEndpoint, deliveryEndpointLength);

	}
	HelicsError err = helicsErrorInitialize();

	helicsFilterAddDeliveryEndpoint(filt, static_cast<const char *>(deliveryEndpoint), &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(deliveryEndpoint);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFilterRemoveTarget(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFilterRemoveTarget:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFilterRemoveTarget:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFilter filt = *(static_cast<HelicsFilter*>(mxGetData(argv[0])));

	if(!mxIsChar(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFilterRemoveTarget:TypeError","Argument 2 must be a string.");
	}
	char *target = nullptr;
	size_t targetLength = 0;
	int targetStatus = 0;
	if(argc > 1){
		targetLength = mxGetN(argv[1]) + 1;
		target = static_cast<char *>(malloc(targetLength));
		targetStatus = mxGetString(argv[1], target, targetLength);

	}
	HelicsError err = helicsErrorInitialize();

	helicsFilterRemoveTarget(filt, static_cast<const char *>(target), &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(target);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFilterRemoveDeliveryEndpoint(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFilterRemoveDeliveryEndpoint:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFilterRemoveDeliveryEndpoint:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFilter filt = *(static_cast<HelicsFilter*>(mxGetData(argv[0])));

	if(!mxIsChar(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFilterRemoveDeliveryEndpoint:TypeError","Argument 2 must be a string.");
	}
	char *deliveryEndpoint = nullptr;
	size_t deliveryEndpointLength = 0;
	int deliveryEndpointStatus = 0;
	if(argc > 1){
		deliveryEndpointLength = mxGetN(argv[1]) + 1;
		deliveryEndpoint = static_cast<char *>(malloc(deliveryEndpointLength));
		deliveryEndpointStatus = mxGetString(argv[1], deliveryEndpoint, deliveryEndpointLength);

	}
	HelicsError err = helicsErrorInitialize();

	helicsFilterRemoveDeliveryEndpoint(filt, static_cast<const char *>(deliveryEndpoint), &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(deliveryEndpoint);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFilterGetInfo(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFilterGetInfo:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFilterGetInfo:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFilter filt = *(static_cast<HelicsFilter*>(mxGetData(argv[0])));

	const char *result = helicsFilterGetInfo(filt);

	mxArray *_out = mxCreateString(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsFilterSetInfo(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFilterSetInfo:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFilterSetInfo:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFilter filt = *(static_cast<HelicsFilter*>(mxGetData(argv[0])));

	if(!mxIsChar(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFilterSetInfo:TypeError","Argument 2 must be a string.");
	}
	char *info = nullptr;
	size_t infoLength = 0;
	int infoStatus = 0;
	if(argc > 1){
		infoLength = mxGetN(argv[1]) + 1;
		info = static_cast<char *>(malloc(infoLength));
		infoStatus = mxGetString(argv[1], info, infoLength);

	}
	HelicsError err = helicsErrorInitialize();

	helicsFilterSetInfo(filt, static_cast<const char *>(info), &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(info);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFilterGetTag(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc < 1 || argc > 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFilterGetTag:rhs","This function requires at least 1 arguments and at most 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFilterGetTag:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFilter filt = *(static_cast<HelicsFilter*>(mxGetData(argv[0])));

	if(argc > 1){
		if(!mxIsChar(argv[1])){
			mexUnlock();
			mexErrMsgIdAndTxt("MATLAB:helicsFilterGetTag:TypeError","Argument 2 must be a string.");
		}
	}
	char *tagname = nullptr;
	size_t tagnameLength = 0;
	int tagnameStatus = 0;
	if(argc > 1){
		tagnameLength = mxGetN(argv[1]) + 1;
		tagname = static_cast<char *>(malloc(tagnameLength));
		tagnameStatus = mxGetString(argv[1], tagname, tagnameLength);

	}
	const char *result = helicsFilterGetTag(filt, static_cast<const char *>(tagname));

	mxArray *_out = mxCreateString(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(tagname);
}


void _wrap_helicsFilterSetTag(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 3){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFilterSetTag:rhs","This function requires 3 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFilterSetTag:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFilter filt = *(static_cast<HelicsFilter*>(mxGetData(argv[0])));

	if(!mxIsChar(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFilterSetTag:TypeError","Argument 2 must be a string.");
	}
	char *tagname = nullptr;
	size_t tagnameLength = 0;
	int tagnameStatus = 0;
	if(argc > 1){
		tagnameLength = mxGetN(argv[1]) + 1;
		tagname = static_cast<char *>(malloc(tagnameLength));
		tagnameStatus = mxGetString(argv[1], tagname, tagnameLength);

	}
	if(!mxIsChar(argv[2])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFilterSetTag:TypeError","Argument 3 must be a string.");
	}
	char *tagvalue = nullptr;
	size_t tagvalueLength = 0;
	int tagvalueStatus = 0;
	if(argc > 2){
		tagvalueLength = mxGetN(argv[2]) + 1;
		tagvalue = static_cast<char *>(malloc(tagvalueLength));
		tagvalueStatus = mxGetString(argv[2], tagvalue, tagvalueLength);

	}
	HelicsError err = helicsErrorInitialize();

	helicsFilterSetTag(filt, static_cast<const char *>(tagname), static_cast<const char *>(tagvalue), &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(tagname);

	free(tagvalue);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFilterSetOption(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 3){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFilterSetOption:rhs","This function requires 3 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFilterSetOption:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFilter filt = *(static_cast<HelicsFilter*>(mxGetData(argv[0])));

	if(!mxIsNumeric(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFilterSetOption:TypeError","Argument 2 must be of type integer.");
	}
		int option = static_cast<int>(mxGetScalar(argv[1]));

	if(!mxIsNumeric(argv[2])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFilterSetOption:TypeError","Argument 3 must be of type integer.");
	}
		int value = static_cast<int>(mxGetScalar(argv[2]));

	HelicsError err = helicsErrorInitialize();

	helicsFilterSetOption(filt, option, value, &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFilterGetOption(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFilterGetOption:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFilterGetOption:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFilter filt = *(static_cast<HelicsFilter*>(mxGetData(argv[0])));

	if(!mxIsNumeric(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFilterGetOption:TypeError","Argument 2 must be of type integer.");
	}
		int option = static_cast<int>(mxGetScalar(argv[1]));

	int result = helicsFilterGetOption(filt, option);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
	mxInt64 *rv = mxGetInt64s(_out);
	rv[0] = static_cast<mxInt64>(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsFederateRegisterTranslator(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 3){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateRegisterTranslator:rhs","This function requires 3 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateRegisterTranslator:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFederate fed = *(static_cast<HelicsFederate*>(mxGetData(argv[0])));

	if(!mxIsNumeric(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateRegisterTranslator:TypeError","Argument 2 must be of type int32.");
	}
	int typeInt = static_cast<int>(mxGetScalar(argv[1]));
	HelicsTranslatorTypes type = static_cast<HelicsTranslatorTypes>(typeInt);

	if(!mxIsChar(argv[2])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateRegisterTranslator:TypeError","Argument 3 must be a string.");
	}
	char *name = nullptr;
	size_t nameLength = 0;
	int nameStatus = 0;
	if(argc > 2){
		nameLength = mxGetN(argv[2]) + 1;
		name = static_cast<char *>(malloc(nameLength));
		nameStatus = mxGetString(argv[2], name, nameLength);

	}
	HelicsError err = helicsErrorInitialize();

	HelicsTranslator result = helicsFederateRegisterTranslator(fed, type, static_cast<const char *>(name), &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	mxUint64 *rv = mxGetUint64s(_out);
	rv[0] = reinterpret_cast<mxUint64>(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(name);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateRegisterGlobalTranslator(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 3){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateRegisterGlobalTranslator:rhs","This function requires 3 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateRegisterGlobalTranslator:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFederate fed = *(static_cast<HelicsFederate*>(mxGetData(argv[0])));

	if(!mxIsNumeric(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateRegisterGlobalTranslator:TypeError","Argument 2 must be of type int32.");
	}
	int typeInt = static_cast<int>(mxGetScalar(argv[1]));
	HelicsTranslatorTypes type = static_cast<HelicsTranslatorTypes>(typeInt);

	if(!mxIsChar(argv[2])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateRegisterGlobalTranslator:TypeError","Argument 3 must be a string.");
	}
	char *name = nullptr;
	size_t nameLength = 0;
	int nameStatus = 0;
	if(argc > 2){
		nameLength = mxGetN(argv[2]) + 1;
		name = static_cast<char *>(malloc(nameLength));
		nameStatus = mxGetString(argv[2], name, nameLength);

	}
	HelicsError err = helicsErrorInitialize();

	HelicsTranslator result = helicsFederateRegisterGlobalTranslator(fed, type, static_cast<const char *>(name), &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	mxUint64 *rv = mxGetUint64s(_out);
	rv[0] = reinterpret_cast<mxUint64>(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(name);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsCoreRegisterTranslator(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 3){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsCoreRegisterTranslator:rhs","This function requires 3 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsCoreRegisterTranslator:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsCore core = *(static_cast<HelicsCore*>(mxGetData(argv[0])));

	if(!mxIsNumeric(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsCoreRegisterTranslator:TypeError","Argument 2 must be of type int32.");
	}
	int typeInt = static_cast<int>(mxGetScalar(argv[1]));
	HelicsTranslatorTypes type = static_cast<HelicsTranslatorTypes>(typeInt);

	if(!mxIsChar(argv[2])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsCoreRegisterTranslator:TypeError","Argument 3 must be a string.");
	}
	char *name = nullptr;
	size_t nameLength = 0;
	int nameStatus = 0;
	if(argc > 2){
		nameLength = mxGetN(argv[2]) + 1;
		name = static_cast<char *>(malloc(nameLength));
		nameStatus = mxGetString(argv[2], name, nameLength);

	}
	HelicsError err = helicsErrorInitialize();

	HelicsTranslator result = helicsCoreRegisterTranslator(core, type, static_cast<const char *>(name), &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	mxUint64 *rv = mxGetUint64s(_out);
	rv[0] = reinterpret_cast<mxUint64>(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(name);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateGetTranslatorCount(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateGetTranslatorCount:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateGetTranslatorCount:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFederate fed = *(static_cast<HelicsFederate*>(mxGetData(argv[0])));

	int result = helicsFederateGetTranslatorCount(fed);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
	mxInt64 *rv = mxGetInt64s(_out);
	rv[0] = static_cast<mxInt64>(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsFederateGetTranslator(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateGetTranslator:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateGetTranslator:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFederate fed = *(static_cast<HelicsFederate*>(mxGetData(argv[0])));

	if(!mxIsChar(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateGetTranslator:TypeError","Argument 2 must be a string.");
	}
	char *name = nullptr;
	size_t nameLength = 0;
	int nameStatus = 0;
	if(argc > 1){
		nameLength = mxGetN(argv[1]) + 1;
		name = static_cast<char *>(malloc(nameLength));
		nameStatus = mxGetString(argv[1], name, nameLength);

	}
	HelicsError err = helicsErrorInitialize();

	HelicsTranslator result = helicsFederateGetTranslator(fed, static_cast<const char *>(name), &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	mxUint64 *rv = mxGetUint64s(_out);
	rv[0] = reinterpret_cast<mxUint64>(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(name);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateGetTranslatorByIndex(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateGetTranslatorByIndex:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateGetTranslatorByIndex:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFederate fed = *(static_cast<HelicsFederate*>(mxGetData(argv[0])));

	if(!mxIsNumeric(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateGetTranslatorByIndex:TypeError","Argument 2 must be of type integer.");
	}
		int index = static_cast<int>(mxGetScalar(argv[1]));

	HelicsError err = helicsErrorInitialize();

	HelicsTranslator result = helicsFederateGetTranslatorByIndex(fed, index, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	mxUint64 *rv = mxGetUint64s(_out);
	rv[0] = reinterpret_cast<mxUint64>(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsTranslatorIsValid(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsTranslatorIsValid:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsTranslatorIsValid:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsTranslator trans = *(static_cast<HelicsTranslator*>(mxGetData(argv[0])));

	HelicsBool result = helicsTranslatorIsValid(trans);

	mxArray *_out = mxCreateLogicalMatrix(1, 1);
	mxLogical *rv = mxGetLogicals(_out);
	if(result == HELICS_TRUE){
		rv[0] = true;
	}else{
		rv[0] = false;
	}

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsTranslatorGetName(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsTranslatorGetName:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsTranslatorGetName:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsTranslator trans = *(static_cast<HelicsTranslator*>(mxGetData(argv[0])));

	const char *result = helicsTranslatorGetName(trans);

	mxArray *_out = mxCreateString(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsTranslatorSet(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 3){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsTranslatorSet:rhs","This function requires 3 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsTranslatorSet:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsTranslator trans = *(static_cast<HelicsTranslator*>(mxGetData(argv[0])));

	if(!mxIsChar(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsTranslatorSet:TypeError","Argument 2 must be a string.");
	}
	char *prop = nullptr;
	size_t propLength = 0;
	int propStatus = 0;
	if(argc > 1){
		propLength = mxGetN(argv[1]) + 1;
		prop = static_cast<char *>(malloc(propLength));
		propStatus = mxGetString(argv[1], prop, propLength);

	}
	if(!mxIsNumeric(argv[2])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsTranslatorSet:TypeError","Argument 3 must be of type double.");
	}
	double val = mxGetScalar(argv[2]);

	HelicsError err = helicsErrorInitialize();

	helicsTranslatorSet(trans, static_cast<const char *>(prop), val, &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(prop);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsTranslatorSetString(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 3){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsTranslatorSetString:rhs","This function requires 3 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsTranslatorSetString:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsTranslator trans = *(static_cast<HelicsTranslator*>(mxGetData(argv[0])));

	if(!mxIsChar(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsTranslatorSetString:TypeError","Argument 2 must be a string.");
	}
	char *prop = nullptr;
	size_t propLength = 0;
	int propStatus = 0;
	if(argc > 1){
		propLength = mxGetN(argv[1]) + 1;
		prop = static_cast<char *>(malloc(propLength));
		propStatus = mxGetString(argv[1], prop, propLength);

	}
	if(!mxIsChar(argv[2])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsTranslatorSetString:TypeError","Argument 3 must be a string.");
	}
	char *val = nullptr;
	size_t valLength = 0;
	int valStatus = 0;
	if(argc > 2){
		valLength = mxGetN(argv[2]) + 1;
		val = static_cast<char *>(malloc(valLength));
		valStatus = mxGetString(argv[2], val, valLength);

	}
	HelicsError err = helicsErrorInitialize();

	helicsTranslatorSetString(trans, static_cast<const char *>(prop), static_cast<const char *>(val), &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(prop);

	free(val);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsTranslatorAddInputTarget(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsTranslatorAddInputTarget:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsTranslatorAddInputTarget:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsTranslator trans = *(static_cast<HelicsTranslator*>(mxGetData(argv[0])));

	if(!mxIsChar(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsTranslatorAddInputTarget:TypeError","Argument 2 must be a string.");
	}
	char *input = nullptr;
	size_t inputLength = 0;
	int inputStatus = 0;
	if(argc > 1){
		inputLength = mxGetN(argv[1]) + 1;
		input = static_cast<char *>(malloc(inputLength));
		inputStatus = mxGetString(argv[1], input, inputLength);

	}
	HelicsError err = helicsErrorInitialize();

	helicsTranslatorAddInputTarget(trans, static_cast<const char *>(input), &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(input);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsTranslatorAddPublicationTarget(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsTranslatorAddPublicationTarget:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsTranslatorAddPublicationTarget:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsTranslator trans = *(static_cast<HelicsTranslator*>(mxGetData(argv[0])));

	if(!mxIsChar(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsTranslatorAddPublicationTarget:TypeError","Argument 2 must be a string.");
	}
	char *pub = nullptr;
	size_t pubLength = 0;
	int pubStatus = 0;
	if(argc > 1){
		pubLength = mxGetN(argv[1]) + 1;
		pub = static_cast<char *>(malloc(pubLength));
		pubStatus = mxGetString(argv[1], pub, pubLength);

	}
	HelicsError err = helicsErrorInitialize();

	helicsTranslatorAddPublicationTarget(trans, static_cast<const char *>(pub), &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(pub);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsTranslatorAddSourceEndpoint(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsTranslatorAddSourceEndpoint:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsTranslatorAddSourceEndpoint:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsTranslator trans = *(static_cast<HelicsTranslator*>(mxGetData(argv[0])));

	if(!mxIsChar(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsTranslatorAddSourceEndpoint:TypeError","Argument 2 must be a string.");
	}
	char *ept = nullptr;
	size_t eptLength = 0;
	int eptStatus = 0;
	if(argc > 1){
		eptLength = mxGetN(argv[1]) + 1;
		ept = static_cast<char *>(malloc(eptLength));
		eptStatus = mxGetString(argv[1], ept, eptLength);

	}
	HelicsError err = helicsErrorInitialize();

	helicsTranslatorAddSourceEndpoint(trans, static_cast<const char *>(ept), &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(ept);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsTranslatorAddDestinationEndpoint(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsTranslatorAddDestinationEndpoint:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsTranslatorAddDestinationEndpoint:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsTranslator trans = *(static_cast<HelicsTranslator*>(mxGetData(argv[0])));

	if(!mxIsChar(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsTranslatorAddDestinationEndpoint:TypeError","Argument 2 must be a string.");
	}
	char *ept = nullptr;
	size_t eptLength = 0;
	int eptStatus = 0;
	if(argc > 1){
		eptLength = mxGetN(argv[1]) + 1;
		ept = static_cast<char *>(malloc(eptLength));
		eptStatus = mxGetString(argv[1], ept, eptLength);

	}
	HelicsError err = helicsErrorInitialize();

	helicsTranslatorAddDestinationEndpoint(trans, static_cast<const char *>(ept), &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(ept);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsTranslatorRemoveTarget(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsTranslatorRemoveTarget:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsTranslatorRemoveTarget:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsTranslator trans = *(static_cast<HelicsTranslator*>(mxGetData(argv[0])));

	if(!mxIsChar(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsTranslatorRemoveTarget:TypeError","Argument 2 must be a string.");
	}
	char *target = nullptr;
	size_t targetLength = 0;
	int targetStatus = 0;
	if(argc > 1){
		targetLength = mxGetN(argv[1]) + 1;
		target = static_cast<char *>(malloc(targetLength));
		targetStatus = mxGetString(argv[1], target, targetLength);

	}
	HelicsError err = helicsErrorInitialize();

	helicsTranslatorRemoveTarget(trans, static_cast<const char *>(target), &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(target);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsTranslatorGetInfo(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 1){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsTranslatorGetInfo:rhs","This function requires 1 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsTranslatorGetInfo:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsTranslator trans = *(static_cast<HelicsTranslator*>(mxGetData(argv[0])));

	const char *result = helicsTranslatorGetInfo(trans);

	mxArray *_out = mxCreateString(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsTranslatorSetInfo(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsTranslatorSetInfo:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsTranslatorSetInfo:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsTranslator trans = *(static_cast<HelicsTranslator*>(mxGetData(argv[0])));

	if(!mxIsChar(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsTranslatorSetInfo:TypeError","Argument 2 must be a string.");
	}
	char *info = nullptr;
	size_t infoLength = 0;
	int infoStatus = 0;
	if(argc > 1){
		infoLength = mxGetN(argv[1]) + 1;
		info = static_cast<char *>(malloc(infoLength));
		infoStatus = mxGetString(argv[1], info, infoLength);

	}
	HelicsError err = helicsErrorInitialize();

	helicsTranslatorSetInfo(trans, static_cast<const char *>(info), &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(info);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsTranslatorGetTag(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc < 1 || argc > 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsTranslatorGetTag:rhs","This function requires at least 1 arguments and at most 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsTranslatorGetTag:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsTranslator trans = *(static_cast<HelicsTranslator*>(mxGetData(argv[0])));

	if(argc > 1){
		if(!mxIsChar(argv[1])){
			mexUnlock();
			mexErrMsgIdAndTxt("MATLAB:helicsTranslatorGetTag:TypeError","Argument 2 must be a string.");
		}
	}
	char *tagname = nullptr;
	size_t tagnameLength = 0;
	int tagnameStatus = 0;
	if(argc > 1){
		tagnameLength = mxGetN(argv[1]) + 1;
		tagname = static_cast<char *>(malloc(tagnameLength));
		tagnameStatus = mxGetString(argv[1], tagname, tagnameLength);

	}
	const char *result = helicsTranslatorGetTag(trans, static_cast<const char *>(tagname));

	mxArray *_out = mxCreateString(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(tagname);
}


void _wrap_helicsTranslatorSetTag(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 3){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsTranslatorSetTag:rhs","This function requires 3 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsTranslatorSetTag:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsTranslator trans = *(static_cast<HelicsTranslator*>(mxGetData(argv[0])));

	if(!mxIsChar(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsTranslatorSetTag:TypeError","Argument 2 must be a string.");
	}
	char *tagname = nullptr;
	size_t tagnameLength = 0;
	int tagnameStatus = 0;
	if(argc > 1){
		tagnameLength = mxGetN(argv[1]) + 1;
		tagname = static_cast<char *>(malloc(tagnameLength));
		tagnameStatus = mxGetString(argv[1], tagname, tagnameLength);

	}
	if(!mxIsChar(argv[2])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsTranslatorSetTag:TypeError","Argument 3 must be a string.");
	}
	char *tagvalue = nullptr;
	size_t tagvalueLength = 0;
	int tagvalueStatus = 0;
	if(argc > 2){
		tagvalueLength = mxGetN(argv[2]) + 1;
		tagvalue = static_cast<char *>(malloc(tagvalueLength));
		tagvalueStatus = mxGetString(argv[2], tagvalue, tagvalueLength);

	}
	HelicsError err = helicsErrorInitialize();

	helicsTranslatorSetTag(trans, static_cast<const char *>(tagname), static_cast<const char *>(tagvalue), &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(tagname);

	free(tagvalue);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsTranslatorSetOption(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 3){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsTranslatorSetOption:rhs","This function requires 3 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsTranslatorSetOption:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsTranslator trans = *(static_cast<HelicsTranslator*>(mxGetData(argv[0])));

	if(!mxIsNumeric(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsTranslatorSetOption:TypeError","Argument 2 must be of type integer.");
	}
		int option = static_cast<int>(mxGetScalar(argv[1]));

	if(!mxIsNumeric(argv[2])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsTranslatorSetOption:TypeError","Argument 3 must be of type integer.");
	}
		int value = static_cast<int>(mxGetScalar(argv[2]));

	HelicsError err = helicsErrorInitialize();

	helicsTranslatorSetOption(trans, option, value, &err);

	mxArray *_out = nullptr;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsTranslatorGetOption(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsTranslatorGetOption:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsTranslatorGetOption:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsTranslator trans = *(static_cast<HelicsTranslator*>(mxGetData(argv[0])));

	if(!mxIsNumeric(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsTranslatorGetOption:TypeError","Argument 2 must be of type integer.");
	}
		int option = static_cast<int>(mxGetScalar(argv[1]));

	int result = helicsTranslatorGetOption(trans, option);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
	mxInt64 *rv = mxGetInt64s(_out);
	rv[0] = static_cast<mxInt64>(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void matlabBrokerLoggingCallback(int loglevel, const char* identifier, const char* message, void *userData){
	mxArray *lhs;
	mxArray *rhs[4];
	rhs[0] = reinterpret_cast<mxArray *>(userData);
	rhs[1] = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
	mxInt64 *rhs1Ptr = mxGetInt64s(rhs[1]);
	rhs1Ptr[0] = static_cast<mxInt64>(loglevel);
	rhs[2] = mxCreateString(identifier);
	rhs[3] = mxCreateString(message);
	int status = mexCallMATLAB(0,&lhs,4,rhs,"feval");
	mxDestroyArray(lhs);
	mxDestroyArray(rhs[1]);
	mxDestroyArray(rhs[2]);
	mxDestroyArray(rhs[3]);
}

void _wrap_helicsBrokerSetLoggingCallback(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsBrokerSetLoggingCallback:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsBrokerSetLoggingCallback:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsBroker broker = *(static_cast<HelicsBroker*>(mxGetData(argv[0])));

	void *userData = mxGetData(argv[1]);
	HelicsError err = helicsErrorInitialize();

	helicsBrokerSetLoggingCallback(broker, &matlabBrokerLoggingCallback, userData, &err);

	mxArray *_out = nullptr;
	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void matlabCoreLoggingCallback(int loglevel, const char* identifier, const char* message, void *userData){
	mxArray *lhs;
	mxArray *rhs[4];
	rhs[0] = reinterpret_cast<mxArray *>(userData);
	rhs[1] = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
	mxInt64 *rhs1Ptr = mxGetInt64s(rhs[1]);
	rhs1Ptr[0] = static_cast<mxInt64>(loglevel);
	rhs[2] = mxCreateString(identifier);
	rhs[3] = mxCreateString(message);
	int status = mexCallMATLAB(0,&lhs,4,rhs,"feval");
	mxDestroyArray(lhs);
	mxDestroyArray(rhs[1]);
	mxDestroyArray(rhs[2]);
	mxDestroyArray(rhs[3]);
}

void _wrap_helicsCoreSetLoggingCallback(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsCoreSetLoggingCallback:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsCoreSetLoggingCallback:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsCore core = *(static_cast<HelicsCore*>(mxGetData(argv[0])));

	void *userData = mxGetData(argv[1]);
	HelicsError err = helicsErrorInitialize();

	helicsCoreSetLoggingCallback(core, &matlabCoreLoggingCallback, userData, &err);

	mxArray *_out = nullptr;
	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void matlabFederateLoggingCallback(int loglevel, const char* identifier, const char* message, void *userData){
	mxArray *lhs;
	mxArray *rhs[4];
	rhs[0] = reinterpret_cast<mxArray *>(userData);
	rhs[1] = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
	mxInt64 *rhs1Ptr = mxGetInt64s(rhs[1]);
	rhs1Ptr[0] = static_cast<mxInt64>(loglevel);
	rhs[2] = mxCreateString(identifier);
	rhs[3] = mxCreateString(message);
	int status = mexCallMATLAB(0,&lhs,4,rhs,"feval");
	mxDestroyArray(lhs);
	mxDestroyArray(rhs[1]);
	mxDestroyArray(rhs[2]);
	mxDestroyArray(rhs[3]);
}

void _wrap_helicsFederateSetLoggingCallback(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateSetLoggingCallback:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateSetLoggingCallback:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFederate fed = *(static_cast<HelicsFederate*>(mxGetData(argv[0])));

	void *userData = mxGetData(argv[1]);
	HelicsError err = helicsErrorInitialize();

	helicsFederateSetLoggingCallback(fed, &matlabFederateLoggingCallback, userData, &err);

	mxArray *_out = nullptr;
	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


HelicsMessage matlabFilterCustomCallback(HelicsMessage message, void *userData){
	mxArray *lhs[1];
	mxArray *rhs[2];
	rhs[0] = reinterpret_cast<mxArray *>(userData);
	rhs[1] = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	mxUint64 *rhs1Ptr = mxGetUint64s(rhs[1]);
	rhs1Ptr[0] = reinterpret_cast<mxUint64>(message);
	int status = mexCallMATLAB(1,lhs,2,rhs,"feval");
	HelicsMessage rv = static_cast<HelicsMessage>(mxGetData(lhs[0]));
	mxDestroyArray(rhs[1]);
	return rv;
}

void _wrap_helicsFilterSetCustomCallback(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFilterSetCustomCallback:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFilterSetCustomCallback:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFilter filter = *(static_cast<HelicsFilter*>(mxGetData(argv[0])));

	void *userData = mxGetData(argv[1]);
	HelicsError err = helicsErrorInitialize();

	helicsFilterSetCustomCallback(filter, &matlabFilterCustomCallback, userData, &err);

	mxArray *_out = nullptr;
	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void matlabToMessageCallCallback(HelicsDataBuffer value, HelicsMessage message, void *userData){
	mxArray *lhs;
	mxArray *rhs[3];
	rhs[0] = static_cast<mxArray **>(userData)[0];
	rhs[1] = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	mxUint64 *rhs1Ptr = mxGetUint64s(rhs[1]);
	rhs1Ptr[0] = reinterpret_cast<mxUint64>(value);
	rhs[2] = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	mxUint64 *rhs2Ptr = mxGetUint64s(rhs[1]);
	rhs2Ptr[0] = reinterpret_cast<mxUint64>(message);
	int status = mexCallMATLAB(0,&lhs,3,rhs,"feval");
	mxDestroyArray(lhs);
	mxDestroyArray(rhs[1]);
	mxDestroyArray(rhs[2]);
}

void matlabToValueCallCallback(HelicsMessage message, HelicsDataBuffer value, void *userData){
	mxArray *lhs;
	mxArray *rhs[3];
	rhs[0] = static_cast<mxArray **>(userData)[1];
	rhs[1] = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	mxUint64 *rhs1Ptr = mxGetUint64s(rhs[1]);
	rhs1Ptr[0] = reinterpret_cast<mxUint64>(message);
	rhs[2] = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	mxUint64 *rhs2Ptr = mxGetUint64s(rhs[1]);
	rhs2Ptr[0] = reinterpret_cast<mxUint64>(value);
	int status = mexCallMATLAB(0,&lhs,3,rhs,"feval");
	mxDestroyArray(lhs);
	mxDestroyArray(rhs[1]);
	mxDestroyArray(rhs[2]);
}

void _wrap_helicsTranslatorSetCustomCallback(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 3){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsTranslatorSetCustomCallback:rhs","This function requires 3 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsTranslatorSetCustomCallback:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsTranslator translator = *(static_cast<HelicsTranslator*>(mxGetData(argv[0])));

	mxArray *callbacks[2];
	callbacks[0] = const_cast<mxArray *>(argv[1]);
	callbacks[1] = const_cast<mxArray *>(argv[2]);
	void *userData = reinterpret_cast<void *>(callbacks);
	HelicsError err = helicsErrorInitialize();

	helicsTranslatorSetCustomCallback(translator, &matlabToMessageCallCallback, &matlabToValueCallCallback, userData, &err);

	mxArray *_out = nullptr;
	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void matlabFederateQueryCallback(const char* query, int querySize, HelicsQueryBuffer buffer, void *userData){
	mxArray *lhs;
	mxArray *rhs[4];
	mwSize dims[2] = {1, static_cast<mwSize>(querySize)};
	rhs[0] = reinterpret_cast<mxArray *>(userData);
	rhs[1] = mxCreateCharArray(2, dims);
	mxChar *pQuery = static_cast<mxChar *>(mxGetData(rhs[1]));
	for(int i=0; i<querySize; ++i){
		pQuery[i] = query[i];
	}
	rhs[2] = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
	mxInt64 *rhs2Ptr = mxGetInt64s(rhs[2]);
	rhs2Ptr[0] = static_cast<mxInt64>(querySize);
	rhs[3] = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	mxUint64 *rhs3Ptr = mxGetUint64s(rhs[3]);
	rhs3Ptr[0] = reinterpret_cast<mxUint64>(buffer);
	int status = mexCallMATLAB(0,&lhs,4,rhs,"feval");
	mxDestroyArray(rhs[1]);
	mxDestroyArray(rhs[2]);
	mxDestroyArray(rhs[3]);
}

void _wrap_helicsFederateSetQueryCallback(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateSetQueryCallback:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateSetQueryCallback:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFilter filter = *(static_cast<HelicsFilter*>(mxGetData(argv[0])));

	void *userData = mxGetData(argv[1]);
	HelicsError err = helicsErrorInitialize();

	helicsFederateSetQueryCallback(filter, &matlabFederateQueryCallback, userData, &err);

	mxArray *_out = nullptr;
	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void matlabFederateSetTimeRequestEntryCallback(HelicsTime currentTime, HelicsTime requestTime, HelicsBool iterating, void *userData){
	mxArray *lhs;
	mxArray *rhs[4];
	rhs[0] = reinterpret_cast<mxArray *>(userData);
	rhs[1] = mxCreateDoubleScalar(currentTime);
	rhs[2] = mxCreateDoubleScalar(requestTime);
	rhs[3] = mxCreateLogicalMatrix(1, 1);
	mxLogical *rhs3Ptr = mxGetLogicals(rhs[3]);
	rhs3Ptr[0] = false;
	if(iterating == HELICS_TRUE){
		rhs3Ptr[0] = true;
	}
	int status = mexCallMATLAB(0,&lhs,4,rhs,"feval");
	mxDestroyArray(lhs);
	mxDestroyArray(rhs[1]);
	mxDestroyArray(rhs[2]);
	mxDestroyArray(rhs[3]);
}

void _wrap_helicsFederateSetTimeRequestEntryCallback(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateSetTimeRequestEntryCallback:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateSetTimeRequestEntryCallback:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFederate fed = *(static_cast<HelicsFederate*>(mxGetData(argv[0])));

	void *userData = mxGetData(argv[1]);
	HelicsError err = helicsErrorInitialize();

	helicsFederateSetTimeRequestEntryCallback(fed, &matlabFederateSetTimeRequestEntryCallback, userData, &err);

	mxArray *_out = nullptr;
	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void matlabFederateTimeUpdateCallback(HelicsTime newTime, HelicsBool iterating, void *userData){
	mxArray *lhs;
	mxArray *rhs[3];
	rhs[0] = reinterpret_cast<mxArray *>(userData);
	rhs[1] = mxCreateDoubleScalar(static_cast<double>(newTime));
	rhs[2] = mxCreateLogicalMatrix(1, 1);
	mxLogical *rhs2Ptr = mxGetLogicals(rhs[2]);
	rhs2Ptr[0] = false;
	if(iterating == HELICS_TRUE){
		rhs2Ptr[0] = true;
	}
	int status = mexCallMATLAB(0,&lhs,3,rhs,"feval");
	mxDestroyArray(lhs);
	mxDestroyArray(rhs[1]);
	mxDestroyArray(rhs[2]);
}

void _wrap_helicsFederateSetTimeUpdateCallback(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateSetTimeUpdateCallback:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateSetTimeUpdateCallback:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFederate fed = *(static_cast<HelicsFederate*>(mxGetData(argv[0])));

	void *userData = mxGetData(argv[1]);
	HelicsError err = helicsErrorInitialize();

	helicsFederateSetTimeUpdateCallback(fed, &matlabFederateTimeUpdateCallback, userData, &err);

	mxArray *_out = nullptr;
	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void matlabFederateSetStateChangeCallback(HelicsFederateState newState, HelicsFederateState oldState, void *userData){
	mxArray *lhs;
	mxArray *rhs[3];
	rhs[0] = reinterpret_cast<mxArray *>(userData);
	rhs[1] = mxCreateNumericMatrix(1, 1, mxINT32_CLASS, mxREAL);
	mxInt32 *rhs1Ptr = mxGetInt32s(rhs[1]);
	rhs1Ptr[0] = static_cast<mxInt32>(newState);
	rhs[2] = mxCreateNumericMatrix(1, 1, mxINT32_CLASS, mxREAL);
	mxInt32 *rhs2Ptr = mxGetInt32s(rhs[2]);
	rhs2Ptr[0] = static_cast<mxInt32>(oldState);
	int status = mexCallMATLAB(0,&lhs,3,rhs,"feval");
	mxDestroyArray(lhs);
	mxDestroyArray(rhs[1]);
	mxDestroyArray(rhs[2]);
}

void _wrap_helicsFederateSetStateChangeCallback(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateSetStateChangeCallback:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateSetStateChangeCallback:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFederate fed = *(static_cast<HelicsFederate*>(mxGetData(argv[0])));

	void *userData = mxGetData(argv[1]);
	HelicsError err = helicsErrorInitialize();

	helicsFederateSetStateChangeCallback(fed, &matlabFederateSetStateChangeCallback, userData, &err);

	mxArray *_out = nullptr;
	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void matlabFederateSetTimeRequestReturnCallback(HelicsTime newTime, HelicsBool iterating, void *userData){
	mxArray *lhs;
	mxArray *rhs[3];
	rhs[0] = reinterpret_cast<mxArray *>(userData);
	rhs[1] = mxCreateDoubleScalar(newTime);
	rhs[2] = mxCreateLogicalMatrix(1, 1);
	mxLogical *rhs2Ptr = mxGetLogicals(rhs[2]);
	rhs2Ptr[0] = false;
	if(iterating == HELICS_TRUE){
		rhs2Ptr[0] = true;
	}
	int status = mexCallMATLAB(0,&lhs,3,rhs,"feval");
	mxDestroyArray(lhs);
	mxDestroyArray(rhs[1]);
	mxDestroyArray(rhs[2]);
}

void _wrap_helicsFederateSetTimeRequestReturnCallback(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateSetTimeRequestReturnCallback:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateSetTimeRequestReturnCallback:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFederate fed = *(static_cast<HelicsFederate*>(mxGetData(argv[0])));

	void *userData = mxGetData(argv[1]);
	HelicsError err = helicsErrorInitialize();

	helicsFederateSetTimeRequestReturnCallback(fed, &matlabFederateSetTimeRequestReturnCallback, userData, &err);

	mxArray *_out = nullptr;
	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void matlabFederateInitializingEntryCallback(HelicsBool iterating, void *userData){
	mxArray *lhs;
	mxArray *rhs[2];
	rhs[0] = reinterpret_cast<mxArray *>(userData);
	rhs[1] = mxCreateLogicalMatrix(1, 1);
	mxLogical *rhs1Ptr = mxGetLogicals(rhs[1]);
	rhs1Ptr[0] = false;
	if(iterating == HELICS_TRUE){
		rhs1Ptr[0] = true;
	}
	int status = mexCallMATLAB(0,&lhs,2,rhs,"feval");
	mxDestroyArray(lhs);
	mxDestroyArray(rhs[1]);
}

void _wrap_helicsFederateInitializingEntryCallback(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateInitializingEntryCallback:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateInitializingEntryCallback:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFederate fed = *(static_cast<HelicsFederate*>(mxGetData(argv[0])));

	void *userData = mxGetData(argv[1]);
	HelicsError err = helicsErrorInitialize();

	helicsFederateInitializingEntryCallback(fed, &matlabFederateInitializingEntryCallback, userData, &err);

	mxArray *_out = nullptr;
	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void matlabFederateExecutingEntryCallback(void *userData){
	mxArray *lhs;
	mxArray *rhs[1];
	rhs[0] = reinterpret_cast<mxArray *>(userData);
	int status = mexCallMATLAB(0,&lhs,1,rhs,"feval");
	mxDestroyArray(lhs);
}

void _wrap_helicsFederateExecutingEntryCallback(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateExecutingEntryCallback:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateExecutingEntryCallback:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFederate fed = *(static_cast<HelicsFederate*>(mxGetData(argv[0])));

	void *userData = mxGetData(argv[1]);
	HelicsError err = helicsErrorInitialize();

	helicsFederateExecutingEntryCallback(fed, &matlabFederateExecutingEntryCallback, userData, &err);

	mxArray *_out = nullptr;
	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void matlabFederateCosimulationTerminationCallback(void *userData){
	mxArray *lhs;
	mxArray *rhs[1];
	rhs[0] = reinterpret_cast<mxArray *>(userData);
	int status = mexCallMATLAB(0,&lhs,1,rhs,"feval");
	mxDestroyArray(lhs);
}

void _wrap_helicsFederateCosimulationTerminationCallback(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateCosimulationTerminationCallback:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateCosimulationTerminationCallback:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFederate fed = *(static_cast<HelicsFederate*>(mxGetData(argv[0])));

	void *userData = mxGetData(argv[1]);
	HelicsError err = helicsErrorInitialize();

	helicsFederateCosimulationTerminationCallback(fed, &matlabFederateCosimulationTerminationCallback, userData, &err);

	mxArray *_out = nullptr;
	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void matlabFederateErrorHandlerCallback(int errorCode, const char* errorString, void *userData){
	mxArray *lhs;
	mxArray *rhs[3];
	rhs[0] = reinterpret_cast<mxArray *>(userData);
	rhs[1] = mxCreateNumericMatrix(1, 1, mxINT32_CLASS, mxREAL);
	mxInt32 *rhs1Ptr = mxGetInt32s(rhs[1]);
	rhs1Ptr[0] = static_cast<mxInt32>(errorCode);
	rhs[2] = mxCreateString(errorString);
	int status = mexCallMATLAB(0,&lhs,3,rhs,"feval");
	mxDestroyArray(lhs);
	mxDestroyArray(rhs[1]);
	mxDestroyArray(rhs[2]);
}

void _wrap_helicsFederateErrorHandlerCallback(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateErrorHandlerCallback:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsFederateErrorHandlerCallback:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFederate fed = *(static_cast<HelicsFederate*>(mxGetData(argv[0])));

	void *userData = mxGetData(argv[1]);
	HelicsError err = helicsErrorInitialize();

	helicsFederateErrorHandlerCallback(fed, &matlabFederateErrorHandlerCallback, userData, &err);

	mxArray *_out = nullptr;
	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


HelicsTime matlabCallbackFederateNextTimeCallback(HelicsTime time, void *userData){
	mxArray *lhs[1];
	mxArray *rhs[2];
	rhs[0] = reinterpret_cast<mxArray *>(userData);
	rhs[1] = mxCreateDoubleScalar(time);
	int status = mexCallMATLAB(1,lhs,2,rhs,"feval");
	if(!mxIsNumeric(lhs[0])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsCallbackFederateNextTimeCallback:TypeError","return type must be of type double.");
	}
	HelicsTime rv = (HelicsTime)(mxGetScalar(lhs[0]));

	mxDestroyArray(lhs[0]);
	mxDestroyArray(rhs[1]);
	return rv;
}

void _wrap_helicsCallbackFederateNextTimeCallback(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:matlabCallbackFederateNextTimeCallback:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsCallbackFederateNextTimeCallback:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFederate fed = *(static_cast<HelicsFederate*>(mxGetData(argv[0])));

	void *userData = mxGetData(argv[1]);
	HelicsError err = helicsErrorInitialize();

	helicsCallbackFederateNextTimeCallback(fed, &matlabCallbackFederateNextTimeCallback, userData, &err);

	mxArray *_out = nullptr;
	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


HelicsTime matlabCallbackFederateNextTimeIterativeCallback(HelicsTime time, HelicsIterationResult iterationResult, HelicsIterationRequest *iteration, void *userData){
	mxArray *lhs[2];
	mxArray *rhs[4];
	rhs[0] = reinterpret_cast<mxArray *>(userData);
	rhs[1] = mxCreateDoubleScalar(time);
	rhs[2] = mxCreateNumericMatrix(1, 1, mxINT32_CLASS, mxREAL);
	mxInt32 *pRhs2 = mxGetInt32s(rhs[2]);
	pRhs2[0] = static_cast<mxInt32>(iterationResult);
	rhs[3] = mxCreateNumericMatrix(1, 1, mxINT32_CLASS, mxREAL);
	mxInt32 *pRhs3 = mxGetInt32s(rhs[3]);
	pRhs3[0] = static_cast<mxInt32>(*iteration);
	int status = mexCallMATLAB(2,lhs,4,rhs,"feval");
	if(!mxIsNumeric(lhs[0])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:matlabCallbackFederateNextTimeIterativeCallback:TypeError","first type returned must be of type double.");
	}
	HelicsTime rv = (HelicsTime)(mxGetScalar(lhs[0]));

	if(mxGetClassID(lhs[1]) != mxINT32_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:matlabCallbackFederateNextTimeIterativeCallback:TypeError","second type returned must be of type int32.");
	}
	mxInt32 *pIterationRequest = mxGetInt32s(lhs[1]);
	*iteration = static_cast<HelicsIterationRequest>(pIterationRequest[0]);

	mxDestroyArray(lhs[0]);
	mxDestroyArray(lhs[1]);
	mxDestroyArray(rhs[1]);
	mxDestroyArray(rhs[2]);
	mxDestroyArray(rhs[3]);
	return rv;
}

void _wrap_helicsCallbackFederateNextTimeIterativeCallback(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsCallbackFederateNextTimeIterativeCallback:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsCallbackFederateNextTimeIterativeCallback:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFederate fed = *(static_cast<HelicsFederate*>(mxGetData(argv[0])));

	void *userData = mxGetData(argv[1]);
	HelicsError err = helicsErrorInitialize();

	helicsCallbackFederateNextTimeIterativeCallback(fed, &matlabCallbackFederateNextTimeIterativeCallback, userData, &err);

	mxArray *_out = nullptr;
	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


HelicsIterationRequest matlabCallbackFederateInitializeCallback(void *userData){
	mxArray *lhs[1];
	mxArray *rhs[1];
	rhs[0] = reinterpret_cast<mxArray *>(userData);
	int status = mexCallMATLAB(1,lhs,1,rhs,"feval");
	if(mxGetClassID(lhs[0]) != mxINT32_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:matlabCallbackFederateInitializeCallback:TypeError","return type must be of type int32.");
	}
	mxInt32 *pIterationRequest = mxGetInt32s(lhs[0]);
	HelicsIterationRequest rv = static_cast<HelicsIterationRequest>(pIterationRequest[0]);

	mxDestroyArray(lhs[0]);
	return rv;
}

void _wrap_helicsCallbackFederateInitializeCallback(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsCallbackFederateInitializeCallback:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsCallbackFederateInitializeCallback:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsFederate fed = *(static_cast<HelicsFederate*>(mxGetData(argv[0])));

	void *userData = mxGetData(argv[1]);
	HelicsError err = helicsErrorInitialize();

	helicsCallbackFederateInitializeCallback(fed, &matlabCallbackFederateInitializeCallback, userData, &err);

	mxArray *_out = nullptr;
	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsQueryBufferFill(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	if(argc != 2){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsQueryBufferFill:rhs","This function requires 2 arguments.");
	}

	if(mxGetClassID(argv[0]) != mxUINT64_CLASS){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsQueryBufferFill:TypeError","Argument 1 must be of type uint64.");
	}
	HelicsQueryBuffer buffer = *(static_cast<HelicsQueryBuffer*>(mxGetData(argv[0])));

	if(!mxIsChar(argv[1])){
		mexUnlock();
		mexErrMsgIdAndTxt("MATLAB:helicsQueryBufferFill:TypeError","Argument 2 must be a string.");
	}
	char *queryResult = nullptr;
	size_t queryResultLength = 0;
	int queryResultStatus = 0;
	if(argc > 1){
		queryResultLength = mxGetN(argv[1]) + 1;
		queryResult = static_cast<char *>(malloc(queryResultLength));
		queryResultStatus = mxGetString(argv[1], queryResult, queryResultLength);

	}
	int strSize = static_cast<int>(queryResultLength) - 1;

	HelicsError err = helicsErrorInitialize();

	helicsQueryBufferFill(buffer, queryResult, strSize, &err);

	mxArray *_out = nullptr;
	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void mexFunction(int resc, mxArray *resv[], int argc, const mxArray *argv[]) {
	if(--argc < 0 || !mxIsChar(*argv)){
		mexErrMsgTxt("This mex file should only be called from inside the .m files. First input should be the function ID.");
	}
	int functionId;
	try {
		functionId = wrapperFunctionMap.at(std::string(mxArrayToString(*argv++)));
	} catch (const std::out_of_range& e) {
		mexErrMsgTxt("unrecognized function id.");
	}
	int flag=0;
	/* Prevent unloading this file until MATLAB exits */
	if(!mexFunctionCalled) {
		mexFunctionCalled = 1;
		mexLock();
	}
	switch (functionId) {
	case 65:
		_wrap_helicsCreateDataBuffer(resc, resv, argc, argv);
		break;
	case 66:
		_wrap_helicsDataBufferIsValid(resc, resv, argc, argv);
		break;
	case 67:
		_wrap_helicsWrapDataInBuffer(resc, resv, argc, argv);
		break;
	case 68:
		_wrap_helicsDataBufferFree(resc, resv, argc, argv);
		break;
	case 69:
		_wrap_helicsDataBufferSize(resc, resv, argc, argv);
		break;
	case 70:
		_wrap_helicsDataBufferCapacity(resc, resv, argc, argv);
		break;
	case 71:
		_wrap_helicsDataBufferData(resc, resv, argc, argv);
		break;
	case 72:
		_wrap_helicsDataBufferReserve(resc, resv, argc, argv);
		break;
	case 73:
		_wrap_helicsDataBufferClone(resc, resv, argc, argv);
		break;
	case 74:
		_wrap_helicsIntegerToBytes(resc, resv, argc, argv);
		break;
	case 75:
		_wrap_helicsDoubleToBytes(resc, resv, argc, argv);
		break;
	case 76:
		_wrap_helicsStringToBytes(resc, resv, argc, argv);
		break;
	case 77:
		_wrap_helicsRawStringToBytes(resc, resv, argc, argv);
		break;
	case 78:
		_wrap_helicsBooleanToBytes(resc, resv, argc, argv);
		break;
	case 79:
		_wrap_helicsCharToBytes(resc, resv, argc, argv);
		break;
	case 80:
		_wrap_helicsTimeToBytes(resc, resv, argc, argv);
		break;
	case 81:
		_wrap_helicsComplexToBytes(resc, resv, argc, argv);
		break;
	case 82:
		_wrap_helicsComplexObjectToBytes(resc, resv, argc, argv);
		break;
	case 83:
		_wrap_helicsVectorToBytes(resc, resv, argc, argv);
		break;
	case 84:
		_wrap_helicsNamedPointToBytes(resc, resv, argc, argv);
		break;
	case 85:
		_wrap_helicsComplexVectorToBytes(resc, resv, argc, argv);
		break;
	case 86:
		_wrap_helicsDataBufferType(resc, resv, argc, argv);
		break;
	case 87:
		_wrap_helicsDataBufferToInteger(resc, resv, argc, argv);
		break;
	case 88:
		_wrap_helicsDataBufferToDouble(resc, resv, argc, argv);
		break;
	case 89:
		_wrap_helicsDataBufferToBoolean(resc, resv, argc, argv);
		break;
	case 90:
		_wrap_helicsDataBufferToChar(resc, resv, argc, argv);
		break;
	case 91:
		_wrap_helicsDataBufferStringSize(resc, resv, argc, argv);
		break;
	case 92:
		_wrap_helicsDataBufferToString(resc, resv, argc, argv);
		break;
	case 93:
		_wrap_helicsDataBufferToRawString(resc, resv, argc, argv);
		break;
	case 94:
		_wrap_helicsDataBufferToTime(resc, resv, argc, argv);
		break;
	case 95:
		_wrap_helicsDataBufferToComplexObject(resc, resv, argc, argv);
		break;
	case 96:
		_wrap_helicsDataBufferToComplex(resc, resv, argc, argv);
		break;
	case 97:
		_wrap_helicsDataBufferVectorSize(resc, resv, argc, argv);
		break;
	case 98:
		_wrap_helicsDataBufferToVector(resc, resv, argc, argv);
		break;
	case 99:
		_wrap_helicsDataBufferToComplexVector(resc, resv, argc, argv);
		break;
	case 100:
		_wrap_helicsDataBufferToNamedPoint(resc, resv, argc, argv);
		break;
	case 101:
		_wrap_helicsDataBufferConvertToType(resc, resv, argc, argv);
		break;
	case 102:
		_wrap_helicsGetVersion(resc, resv, argc, argv);
		break;
	case 103:
		_wrap_helicsGetBuildFlags(resc, resv, argc, argv);
		break;
	case 104:
		_wrap_helicsGetCompilerVersion(resc, resv, argc, argv);
		break;
	case 105:
		_wrap_helicsGetSystemInfo(resc, resv, argc, argv);
		break;
	case 108:
		_wrap_helicsLoadSignalHandler(resc, resv, argc, argv);
		break;
	case 109:
		_wrap_helicsLoadThreadedSignalHandler(resc, resv, argc, argv);
		break;
	case 110:
		_wrap_helicsClearSignalHandler(resc, resv, argc, argv);
		break;
	case 113:
		_wrap_helicsAbort(resc, resv, argc, argv);
		break;
	case 114:
		_wrap_helicsIsCoreTypeAvailable(resc, resv, argc, argv);
		break;
	case 115:
		_wrap_helicsCreateCore(resc, resv, argc, argv);
		break;
	case 116:
		_wrap_helicsCreateCoreFromArgs(resc, resv, argc, argv);
		break;
	case 117:
		_wrap_helicsCoreClone(resc, resv, argc, argv);
		break;
	case 118:
		_wrap_helicsCoreIsValid(resc, resv, argc, argv);
		break;
	case 119:
		_wrap_helicsCreateBroker(resc, resv, argc, argv);
		break;
	case 120:
		_wrap_helicsCreateBrokerFromArgs(resc, resv, argc, argv);
		break;
	case 121:
		_wrap_helicsBrokerClone(resc, resv, argc, argv);
		break;
	case 122:
		_wrap_helicsBrokerIsValid(resc, resv, argc, argv);
		break;
	case 123:
		_wrap_helicsBrokerIsConnected(resc, resv, argc, argv);
		break;
	case 124:
		_wrap_helicsBrokerDataLink(resc, resv, argc, argv);
		break;
	case 125:
		_wrap_helicsBrokerAddSourceFilterToEndpoint(resc, resv, argc, argv);
		break;
	case 126:
		_wrap_helicsBrokerAddDestinationFilterToEndpoint(resc, resv, argc, argv);
		break;
	case 127:
		_wrap_helicsBrokerMakeConnections(resc, resv, argc, argv);
		break;
	case 128:
		_wrap_helicsCoreWaitForDisconnect(resc, resv, argc, argv);
		break;
	case 129:
		_wrap_helicsBrokerWaitForDisconnect(resc, resv, argc, argv);
		break;
	case 130:
		_wrap_helicsCoreIsConnected(resc, resv, argc, argv);
		break;
	case 131:
		_wrap_helicsCoreDataLink(resc, resv, argc, argv);
		break;
	case 132:
		_wrap_helicsCoreAddSourceFilterToEndpoint(resc, resv, argc, argv);
		break;
	case 133:
		_wrap_helicsCoreAddDestinationFilterToEndpoint(resc, resv, argc, argv);
		break;
	case 134:
		_wrap_helicsCoreMakeConnections(resc, resv, argc, argv);
		break;
	case 135:
		_wrap_helicsBrokerGetIdentifier(resc, resv, argc, argv);
		break;
	case 136:
		_wrap_helicsCoreGetIdentifier(resc, resv, argc, argv);
		break;
	case 137:
		_wrap_helicsBrokerGetAddress(resc, resv, argc, argv);
		break;
	case 138:
		_wrap_helicsCoreGetAddress(resc, resv, argc, argv);
		break;
	case 139:
		_wrap_helicsCoreSetReadyToInit(resc, resv, argc, argv);
		break;
	case 140:
		_wrap_helicsCoreConnect(resc, resv, argc, argv);
		break;
	case 141:
		_wrap_helicsCoreDisconnect(resc, resv, argc, argv);
		break;
	case 142:
		_wrap_helicsGetFederateByName(resc, resv, argc, argv);
		break;
	case 143:
		_wrap_helicsBrokerDisconnect(resc, resv, argc, argv);
		break;
	case 144:
		_wrap_helicsFederateDestroy(resc, resv, argc, argv);
		break;
	case 145:
		_wrap_helicsBrokerDestroy(resc, resv, argc, argv);
		break;
	case 146:
		_wrap_helicsCoreDestroy(resc, resv, argc, argv);
		break;
	case 147:
		_wrap_helicsCoreFree(resc, resv, argc, argv);
		break;
	case 148:
		_wrap_helicsBrokerFree(resc, resv, argc, argv);
		break;
	case 149:
		_wrap_helicsCreateValueFederate(resc, resv, argc, argv);
		break;
	case 150:
		_wrap_helicsCreateValueFederateFromConfig(resc, resv, argc, argv);
		break;
	case 151:
		_wrap_helicsCreateMessageFederate(resc, resv, argc, argv);
		break;
	case 152:
		_wrap_helicsCreateMessageFederateFromConfig(resc, resv, argc, argv);
		break;
	case 153:
		_wrap_helicsCreateCombinationFederate(resc, resv, argc, argv);
		break;
	case 154:
		_wrap_helicsCreateCombinationFederateFromConfig(resc, resv, argc, argv);
		break;
	case 155:
		_wrap_helicsCreateCallbackFederate(resc, resv, argc, argv);
		break;
	case 156:
		_wrap_helicsCreateCallbackFederateFromConfig(resc, resv, argc, argv);
		break;
	case 157:
		_wrap_helicsFederateClone(resc, resv, argc, argv);
		break;
	case 158:
		_wrap_helicsFederateProtect(resc, resv, argc, argv);
		break;
	case 159:
		_wrap_helicsFederateUnProtect(resc, resv, argc, argv);
		break;
	case 160:
		_wrap_helicsFederateIsProtected(resc, resv, argc, argv);
		break;
	case 161:
		_wrap_helicsCreateFederateInfo(resc, resv, argc, argv);
		break;
	case 162:
		_wrap_helicsFederateInfoClone(resc, resv, argc, argv);
		break;
	case 163:
		_wrap_helicsFederateInfoLoadFromArgs(resc, resv, argc, argv);
		break;
	case 164:
		_wrap_helicsFederateInfoLoadFromString(resc, resv, argc, argv);
		break;
	case 165:
		_wrap_helicsFederateInfoFree(resc, resv, argc, argv);
		break;
	case 166:
		_wrap_helicsFederateIsValid(resc, resv, argc, argv);
		break;
	case 167:
		_wrap_helicsFederateInfoSetCoreName(resc, resv, argc, argv);
		break;
	case 168:
		_wrap_helicsFederateInfoSetCoreInitString(resc, resv, argc, argv);
		break;
	case 169:
		_wrap_helicsFederateInfoSetBrokerInitString(resc, resv, argc, argv);
		break;
	case 170:
		_wrap_helicsFederateInfoSetCoreType(resc, resv, argc, argv);
		break;
	case 171:
		_wrap_helicsFederateInfoSetCoreTypeFromString(resc, resv, argc, argv);
		break;
	case 172:
		_wrap_helicsFederateInfoSetBroker(resc, resv, argc, argv);
		break;
	case 173:
		_wrap_helicsFederateInfoSetBrokerKey(resc, resv, argc, argv);
		break;
	case 174:
		_wrap_helicsFederateInfoSetBrokerPort(resc, resv, argc, argv);
		break;
	case 175:
		_wrap_helicsFederateInfoSetLocalPort(resc, resv, argc, argv);
		break;
	case 176:
		_wrap_helicsGetPropertyIndex(resc, resv, argc, argv);
		break;
	case 177:
		_wrap_helicsGetFlagIndex(resc, resv, argc, argv);
		break;
	case 178:
		_wrap_helicsGetOptionIndex(resc, resv, argc, argv);
		break;
	case 179:
		_wrap_helicsGetOptionValue(resc, resv, argc, argv);
		break;
	case 180:
		_wrap_helicsGetDataType(resc, resv, argc, argv);
		break;
	case 181:
		_wrap_helicsFederateInfoSetFlagOption(resc, resv, argc, argv);
		break;
	case 182:
		_wrap_helicsFederateInfoSetSeparator(resc, resv, argc, argv);
		break;
	case 183:
		_wrap_helicsFederateInfoSetTimeProperty(resc, resv, argc, argv);
		break;
	case 184:
		_wrap_helicsFederateInfoSetIntegerProperty(resc, resv, argc, argv);
		break;
	case 185:
		_wrap_helicsFederateRegisterInterfaces(resc, resv, argc, argv);
		break;
	case 186:
		_wrap_helicsFederateGlobalError(resc, resv, argc, argv);
		break;
	case 187:
		_wrap_helicsFederateLocalError(resc, resv, argc, argv);
		break;
	case 188:
		_wrap_helicsFederateFinalize(resc, resv, argc, argv);
		break;
	case 189:
		_wrap_helicsFederateFinalizeAsync(resc, resv, argc, argv);
		break;
	case 190:
		_wrap_helicsFederateFinalizeComplete(resc, resv, argc, argv);
		break;
	case 191:
		_wrap_helicsFederateDisconnect(resc, resv, argc, argv);
		break;
	case 192:
		_wrap_helicsFederateDisconnectAsync(resc, resv, argc, argv);
		break;
	case 193:
		_wrap_helicsFederateDisconnectComplete(resc, resv, argc, argv);
		break;
	case 194:
		_wrap_helicsFederateFree(resc, resv, argc, argv);
		break;
	case 195:
		_wrap_helicsCloseLibrary(resc, resv, argc, argv);
		break;
	case 196:
		_wrap_helicsFederateEnterInitializingMode(resc, resv, argc, argv);
		break;
	case 197:
		_wrap_helicsFederateEnterInitializingModeAsync(resc, resv, argc, argv);
		break;
	case 198:
		_wrap_helicsFederateIsAsyncOperationCompleted(resc, resv, argc, argv);
		break;
	case 199:
		_wrap_helicsFederateEnterInitializingModeComplete(resc, resv, argc, argv);
		break;
	case 200:
		_wrap_helicsFederateEnterExecutingMode(resc, resv, argc, argv);
		break;
	case 201:
		_wrap_helicsFederateEnterExecutingModeAsync(resc, resv, argc, argv);
		break;
	case 202:
		_wrap_helicsFederateEnterExecutingModeComplete(resc, resv, argc, argv);
		break;
	case 203:
		_wrap_helicsFederateEnterExecutingModeIterative(resc, resv, argc, argv);
		break;
	case 204:
		_wrap_helicsFederateEnterExecutingModeIterativeAsync(resc, resv, argc, argv);
		break;
	case 205:
		_wrap_helicsFederateEnterExecutingModeIterativeComplete(resc, resv, argc, argv);
		break;
	case 206:
		_wrap_helicsFederateGetState(resc, resv, argc, argv);
		break;
	case 207:
		_wrap_helicsFederateGetCore(resc, resv, argc, argv);
		break;
	case 208:
		_wrap_helicsFederateRequestTime(resc, resv, argc, argv);
		break;
	case 209:
		_wrap_helicsFederateRequestTimeAdvance(resc, resv, argc, argv);
		break;
	case 210:
		_wrap_helicsFederateRequestNextStep(resc, resv, argc, argv);
		break;
	case 211:
		_wrap_helicsFederateRequestTimeIterative(resc, resv, argc, argv);
		break;
	case 212:
		_wrap_helicsFederateRequestTimeAsync(resc, resv, argc, argv);
		break;
	case 213:
		_wrap_helicsFederateRequestTimeComplete(resc, resv, argc, argv);
		break;
	case 214:
		_wrap_helicsFederateRequestTimeIterativeAsync(resc, resv, argc, argv);
		break;
	case 215:
		_wrap_helicsFederateRequestTimeIterativeComplete(resc, resv, argc, argv);
		break;
	case 216:
		_wrap_helicsFederateProcessCommunications(resc, resv, argc, argv);
		break;
	case 217:
		_wrap_helicsFederateGetName(resc, resv, argc, argv);
		break;
	case 218:
		_wrap_helicsFederateSetTimeProperty(resc, resv, argc, argv);
		break;
	case 219:
		_wrap_helicsFederateSetFlagOption(resc, resv, argc, argv);
		break;
	case 220:
		_wrap_helicsFederateSetSeparator(resc, resv, argc, argv);
		break;
	case 221:
		_wrap_helicsFederateSetIntegerProperty(resc, resv, argc, argv);
		break;
	case 222:
		_wrap_helicsFederateGetTimeProperty(resc, resv, argc, argv);
		break;
	case 223:
		_wrap_helicsFederateGetFlagOption(resc, resv, argc, argv);
		break;
	case 224:
		_wrap_helicsFederateGetIntegerProperty(resc, resv, argc, argv);
		break;
	case 225:
		_wrap_helicsFederateGetCurrentTime(resc, resv, argc, argv);
		break;
	case 226:
		_wrap_helicsFederateSetGlobal(resc, resv, argc, argv);
		break;
	case 227:
		_wrap_helicsFederateSetTag(resc, resv, argc, argv);
		break;
	case 228:
		_wrap_helicsFederateGetTag(resc, resv, argc, argv);
		break;
	case 229:
		_wrap_helicsFederateAddDependency(resc, resv, argc, argv);
		break;
	case 230:
		_wrap_helicsFederateSetLogFile(resc, resv, argc, argv);
		break;
	case 231:
		_wrap_helicsFederateLogErrorMessage(resc, resv, argc, argv);
		break;
	case 232:
		_wrap_helicsFederateLogWarningMessage(resc, resv, argc, argv);
		break;
	case 233:
		_wrap_helicsFederateLogInfoMessage(resc, resv, argc, argv);
		break;
	case 234:
		_wrap_helicsFederateLogDebugMessage(resc, resv, argc, argv);
		break;
	case 235:
		_wrap_helicsFederateLogLevelMessage(resc, resv, argc, argv);
		break;
	case 236:
		_wrap_helicsFederateSendCommand(resc, resv, argc, argv);
		break;
	case 237:
		_wrap_helicsFederateGetCommand(resc, resv, argc, argv);
		break;
	case 238:
		_wrap_helicsFederateGetCommandSource(resc, resv, argc, argv);
		break;
	case 239:
		_wrap_helicsFederateWaitCommand(resc, resv, argc, argv);
		break;
	case 240:
		_wrap_helicsCoreSetGlobal(resc, resv, argc, argv);
		break;
	case 241:
		_wrap_helicsBrokerSetGlobal(resc, resv, argc, argv);
		break;
	case 242:
		_wrap_helicsCoreSendCommand(resc, resv, argc, argv);
		break;
	case 243:
		_wrap_helicsCoreSendOrderedCommand(resc, resv, argc, argv);
		break;
	case 244:
		_wrap_helicsBrokerSendCommand(resc, resv, argc, argv);
		break;
	case 245:
		_wrap_helicsBrokerSendOrderedCommand(resc, resv, argc, argv);
		break;
	case 246:
		_wrap_helicsCoreSetLogFile(resc, resv, argc, argv);
		break;
	case 247:
		_wrap_helicsBrokerSetLogFile(resc, resv, argc, argv);
		break;
	case 248:
		_wrap_helicsBrokerSetTimeBarrier(resc, resv, argc, argv);
		break;
	case 249:
		_wrap_helicsBrokerClearTimeBarrier(resc, resv, argc, argv);
		break;
	case 250:
		_wrap_helicsBrokerGlobalError(resc, resv, argc, argv);
		break;
	case 251:
		_wrap_helicsCoreGlobalError(resc, resv, argc, argv);
		break;
	case 252:
		_wrap_helicsCreateQuery(resc, resv, argc, argv);
		break;
	case 253:
		_wrap_helicsQueryExecute(resc, resv, argc, argv);
		break;
	case 254:
		_wrap_helicsQueryCoreExecute(resc, resv, argc, argv);
		break;
	case 255:
		_wrap_helicsQueryBrokerExecute(resc, resv, argc, argv);
		break;
	case 256:
		_wrap_helicsQueryExecuteAsync(resc, resv, argc, argv);
		break;
	case 257:
		_wrap_helicsQueryExecuteComplete(resc, resv, argc, argv);
		break;
	case 258:
		_wrap_helicsQueryIsCompleted(resc, resv, argc, argv);
		break;
	case 259:
		_wrap_helicsQuerySetTarget(resc, resv, argc, argv);
		break;
	case 260:
		_wrap_helicsQuerySetQueryString(resc, resv, argc, argv);
		break;
	case 261:
		_wrap_helicsQuerySetOrdering(resc, resv, argc, argv);
		break;
	case 262:
		_wrap_helicsQueryFree(resc, resv, argc, argv);
		break;
	case 263:
		_wrap_helicsCleanupLibrary(resc, resv, argc, argv);
		break;
	case 264:
		_wrap_helicsFederateRegisterSubscription(resc, resv, argc, argv);
		break;
	case 265:
		_wrap_helicsFederateRegisterPublication(resc, resv, argc, argv);
		break;
	case 266:
		_wrap_helicsFederateRegisterTypePublication(resc, resv, argc, argv);
		break;
	case 267:
		_wrap_helicsFederateRegisterGlobalPublication(resc, resv, argc, argv);
		break;
	case 268:
		_wrap_helicsFederateRegisterGlobalTypePublication(resc, resv, argc, argv);
		break;
	case 269:
		_wrap_helicsFederateRegisterInput(resc, resv, argc, argv);
		break;
	case 270:
		_wrap_helicsFederateRegisterTypeInput(resc, resv, argc, argv);
		break;
	case 271:
		_wrap_helicsFederateRegisterGlobalInput(resc, resv, argc, argv);
		break;
	case 272:
		_wrap_helicsFederateRegisterGlobalTypeInput(resc, resv, argc, argv);
		break;
	case 273:
		_wrap_helicsFederateGetPublication(resc, resv, argc, argv);
		break;
	case 274:
		_wrap_helicsFederateGetPublicationByIndex(resc, resv, argc, argv);
		break;
	case 275:
		_wrap_helicsFederateGetInput(resc, resv, argc, argv);
		break;
	case 276:
		_wrap_helicsFederateGetInputByIndex(resc, resv, argc, argv);
		break;
	case 277:
		_wrap_helicsFederateGetSubscription(resc, resv, argc, argv);
		break;
	case 278:
		_wrap_helicsFederateClearUpdates(resc, resv, argc, argv);
		break;
	case 279:
		_wrap_helicsFederateRegisterFromPublicationJSON(resc, resv, argc, argv);
		break;
	case 280:
		_wrap_helicsFederatePublishJSON(resc, resv, argc, argv);
		break;
	case 281:
		_wrap_helicsPublicationIsValid(resc, resv, argc, argv);
		break;
	case 282:
		_wrap_helicsPublicationPublishBytes(resc, resv, argc, argv);
		break;
	case 283:
		_wrap_helicsPublicationPublishString(resc, resv, argc, argv);
		break;
	case 284:
		_wrap_helicsPublicationPublishInteger(resc, resv, argc, argv);
		break;
	case 285:
		_wrap_helicsPublicationPublishBoolean(resc, resv, argc, argv);
		break;
	case 286:
		_wrap_helicsPublicationPublishDouble(resc, resv, argc, argv);
		break;
	case 287:
		_wrap_helicsPublicationPublishTime(resc, resv, argc, argv);
		break;
	case 288:
		_wrap_helicsPublicationPublishChar(resc, resv, argc, argv);
		break;
	case 289:
		_wrap_helicsPublicationPublishComplex(resc, resv, argc, argv);
		break;
	case 290:
		_wrap_helicsPublicationPublishVector(resc, resv, argc, argv);
		break;
	case 291:
		_wrap_helicsPublicationPublishComplexVector(resc, resv, argc, argv);
		break;
	case 292:
		_wrap_helicsPublicationPublishNamedPoint(resc, resv, argc, argv);
		break;
	case 293:
		_wrap_helicsPublicationAddTarget(resc, resv, argc, argv);
		break;
	case 294:
		_wrap_helicsInputIsValid(resc, resv, argc, argv);
		break;
	case 295:
		_wrap_helicsInputAddTarget(resc, resv, argc, argv);
		break;
	case 296:
		_wrap_helicsInputGetByteCount(resc, resv, argc, argv);
		break;
	case 297:
		_wrap_helicsInputGetBytes(resc, resv, argc, argv);
		break;
	case 298:
		_wrap_helicsInputGetStringSize(resc, resv, argc, argv);
		break;
	case 299:
		_wrap_helicsInputGetString(resc, resv, argc, argv);
		break;
	case 300:
		_wrap_helicsInputGetInteger(resc, resv, argc, argv);
		break;
	case 301:
		_wrap_helicsInputGetBoolean(resc, resv, argc, argv);
		break;
	case 302:
		_wrap_helicsInputGetDouble(resc, resv, argc, argv);
		break;
	case 303:
		_wrap_helicsInputGetTime(resc, resv, argc, argv);
		break;
	case 304:
		_wrap_helicsInputGetChar(resc, resv, argc, argv);
		break;
	case 305:
		_wrap_helicsInputGetComplexObject(resc, resv, argc, argv);
		break;
	case 306:
		_wrap_helicsInputGetComplex(resc, resv, argc, argv);
		break;
	case 307:
		_wrap_helicsInputGetVectorSize(resc, resv, argc, argv);
		break;
	case 308:
		_wrap_helicsInputGetVector(resc, resv, argc, argv);
		break;
	case 309:
		_wrap_helicsInputGetComplexVector(resc, resv, argc, argv);
		break;
	case 310:
		_wrap_helicsInputGetNamedPoint(resc, resv, argc, argv);
		break;
	case 311:
		_wrap_helicsInputSetDefaultBytes(resc, resv, argc, argv);
		break;
	case 312:
		_wrap_helicsInputSetDefaultString(resc, resv, argc, argv);
		break;
	case 313:
		_wrap_helicsInputSetDefaultInteger(resc, resv, argc, argv);
		break;
	case 314:
		_wrap_helicsInputSetDefaultBoolean(resc, resv, argc, argv);
		break;
	case 315:
		_wrap_helicsInputSetDefaultTime(resc, resv, argc, argv);
		break;
	case 316:
		_wrap_helicsInputSetDefaultChar(resc, resv, argc, argv);
		break;
	case 317:
		_wrap_helicsInputSetDefaultDouble(resc, resv, argc, argv);
		break;
	case 318:
		_wrap_helicsInputSetDefaultComplex(resc, resv, argc, argv);
		break;
	case 319:
		_wrap_helicsInputSetDefaultVector(resc, resv, argc, argv);
		break;
	case 320:
		_wrap_helicsInputSetDefaultComplexVector(resc, resv, argc, argv);
		break;
	case 321:
		_wrap_helicsInputSetDefaultNamedPoint(resc, resv, argc, argv);
		break;
	case 322:
		_wrap_helicsInputGetType(resc, resv, argc, argv);
		break;
	case 323:
		_wrap_helicsInputGetPublicationType(resc, resv, argc, argv);
		break;
	case 324:
		_wrap_helicsInputGetPublicationDataType(resc, resv, argc, argv);
		break;
	case 325:
		_wrap_helicsPublicationGetType(resc, resv, argc, argv);
		break;
	case 326:
		_wrap_helicsInputGetName(resc, resv, argc, argv);
		break;
	case 327:
		_wrap_helicsSubscriptionGetTarget(resc, resv, argc, argv);
		break;
	case 328:
		_wrap_helicsPublicationGetName(resc, resv, argc, argv);
		break;
	case 329:
		_wrap_helicsInputGetUnits(resc, resv, argc, argv);
		break;
	case 330:
		_wrap_helicsInputGetInjectionUnits(resc, resv, argc, argv);
		break;
	case 331:
		_wrap_helicsInputGetExtractionUnits(resc, resv, argc, argv);
		break;
	case 332:
		_wrap_helicsPublicationGetUnits(resc, resv, argc, argv);
		break;
	case 333:
		_wrap_helicsInputGetInfo(resc, resv, argc, argv);
		break;
	case 334:
		_wrap_helicsInputSetInfo(resc, resv, argc, argv);
		break;
	case 335:
		_wrap_helicsInputGetTag(resc, resv, argc, argv);
		break;
	case 336:
		_wrap_helicsInputSetTag(resc, resv, argc, argv);
		break;
	case 337:
		_wrap_helicsPublicationGetInfo(resc, resv, argc, argv);
		break;
	case 338:
		_wrap_helicsPublicationSetInfo(resc, resv, argc, argv);
		break;
	case 339:
		_wrap_helicsPublicationGetTag(resc, resv, argc, argv);
		break;
	case 340:
		_wrap_helicsPublicationSetTag(resc, resv, argc, argv);
		break;
	case 341:
		_wrap_helicsInputGetOption(resc, resv, argc, argv);
		break;
	case 342:
		_wrap_helicsInputSetOption(resc, resv, argc, argv);
		break;
	case 343:
		_wrap_helicsPublicationGetOption(resc, resv, argc, argv);
		break;
	case 344:
		_wrap_helicsPublicationSetOption(resc, resv, argc, argv);
		break;
	case 345:
		_wrap_helicsPublicationSetMinimumChange(resc, resv, argc, argv);
		break;
	case 346:
		_wrap_helicsInputSetMinimumChange(resc, resv, argc, argv);
		break;
	case 347:
		_wrap_helicsInputIsUpdated(resc, resv, argc, argv);
		break;
	case 348:
		_wrap_helicsInputLastUpdateTime(resc, resv, argc, argv);
		break;
	case 349:
		_wrap_helicsInputClearUpdate(resc, resv, argc, argv);
		break;
	case 350:
		_wrap_helicsFederateGetPublicationCount(resc, resv, argc, argv);
		break;
	case 351:
		_wrap_helicsFederateGetInputCount(resc, resv, argc, argv);
		break;
	case 352:
		_wrap_helicsFederateRegisterEndpoint(resc, resv, argc, argv);
		break;
	case 353:
		_wrap_helicsFederateRegisterGlobalEndpoint(resc, resv, argc, argv);
		break;
	case 354:
		_wrap_helicsFederateRegisterTargetedEndpoint(resc, resv, argc, argv);
		break;
	case 355:
		_wrap_helicsFederateRegisterGlobalTargetedEndpoint(resc, resv, argc, argv);
		break;
	case 356:
		_wrap_helicsFederateGetEndpoint(resc, resv, argc, argv);
		break;
	case 357:
		_wrap_helicsFederateGetEndpointByIndex(resc, resv, argc, argv);
		break;
	case 358:
		_wrap_helicsEndpointIsValid(resc, resv, argc, argv);
		break;
	case 359:
		_wrap_helicsEndpointSetDefaultDestination(resc, resv, argc, argv);
		break;
	case 360:
		_wrap_helicsEndpointGetDefaultDestination(resc, resv, argc, argv);
		break;
	case 361:
		_wrap_helicsEndpointSendBytes(resc, resv, argc, argv);
		break;
	case 362:
		_wrap_helicsEndpointSendBytesTo(resc, resv, argc, argv);
		break;
	case 363:
		_wrap_helicsEndpointSendBytesToAt(resc, resv, argc, argv);
		break;
	case 364:
		_wrap_helicsEndpointSendBytesAt(resc, resv, argc, argv);
		break;
	case 365:
		_wrap_helicsEndpointSendMessage(resc, resv, argc, argv);
		break;
	case 366:
		_wrap_helicsEndpointSendMessageZeroCopy(resc, resv, argc, argv);
		break;
	case 367:
		_wrap_helicsEndpointSubscribe(resc, resv, argc, argv);
		break;
	case 368:
		_wrap_helicsFederateHasMessage(resc, resv, argc, argv);
		break;
	case 369:
		_wrap_helicsEndpointHasMessage(resc, resv, argc, argv);
		break;
	case 370:
		_wrap_helicsFederatePendingMessageCount(resc, resv, argc, argv);
		break;
	case 371:
		_wrap_helicsEndpointPendingMessageCount(resc, resv, argc, argv);
		break;
	case 372:
		_wrap_helicsEndpointGetMessage(resc, resv, argc, argv);
		break;
	case 373:
		_wrap_helicsEndpointCreateMessage(resc, resv, argc, argv);
		break;
	case 374:
		_wrap_helicsEndpointClearMessages(resc, resv, argc, argv);
		break;
	case 375:
		_wrap_helicsFederateGetMessage(resc, resv, argc, argv);
		break;
	case 376:
		_wrap_helicsFederateCreateMessage(resc, resv, argc, argv);
		break;
	case 377:
		_wrap_helicsFederateClearMessages(resc, resv, argc, argv);
		break;
	case 378:
		_wrap_helicsEndpointGetType(resc, resv, argc, argv);
		break;
	case 379:
		_wrap_helicsEndpointGetName(resc, resv, argc, argv);
		break;
	case 380:
		_wrap_helicsFederateGetEndpointCount(resc, resv, argc, argv);
		break;
	case 381:
		_wrap_helicsEndpointGetInfo(resc, resv, argc, argv);
		break;
	case 382:
		_wrap_helicsEndpointSetInfo(resc, resv, argc, argv);
		break;
	case 383:
		_wrap_helicsEndpointGetTag(resc, resv, argc, argv);
		break;
	case 384:
		_wrap_helicsEndpointSetTag(resc, resv, argc, argv);
		break;
	case 385:
		_wrap_helicsEndpointSetOption(resc, resv, argc, argv);
		break;
	case 386:
		_wrap_helicsEndpointGetOption(resc, resv, argc, argv);
		break;
	case 387:
		_wrap_helicsEndpointAddSourceTarget(resc, resv, argc, argv);
		break;
	case 388:
		_wrap_helicsEndpointAddDestinationTarget(resc, resv, argc, argv);
		break;
	case 389:
		_wrap_helicsEndpointRemoveTarget(resc, resv, argc, argv);
		break;
	case 390:
		_wrap_helicsEndpointAddSourceFilter(resc, resv, argc, argv);
		break;
	case 391:
		_wrap_helicsEndpointAddDestinationFilter(resc, resv, argc, argv);
		break;
	case 392:
		_wrap_helicsMessageGetSource(resc, resv, argc, argv);
		break;
	case 393:
		_wrap_helicsMessageGetDestination(resc, resv, argc, argv);
		break;
	case 394:
		_wrap_helicsMessageGetOriginalSource(resc, resv, argc, argv);
		break;
	case 395:
		_wrap_helicsMessageGetOriginalDestination(resc, resv, argc, argv);
		break;
	case 396:
		_wrap_helicsMessageGetTime(resc, resv, argc, argv);
		break;
	case 397:
		_wrap_helicsMessageGetString(resc, resv, argc, argv);
		break;
	case 398:
		_wrap_helicsMessageGetMessageID(resc, resv, argc, argv);
		break;
	case 399:
		_wrap_helicsMessageGetFlagOption(resc, resv, argc, argv);
		break;
	case 400:
		_wrap_helicsMessageGetByteCount(resc, resv, argc, argv);
		break;
	case 401:
		_wrap_helicsMessageGetBytes(resc, resv, argc, argv);
		break;
	case 402:
		_wrap_helicsMessageGetBytesPointer(resc, resv, argc, argv);
		break;
	case 403:
		_wrap_helicsMessageIsValid(resc, resv, argc, argv);
		break;
	case 404:
		_wrap_helicsMessageSetSource(resc, resv, argc, argv);
		break;
	case 405:
		_wrap_helicsMessageSetDestination(resc, resv, argc, argv);
		break;
	case 406:
		_wrap_helicsMessageSetOriginalSource(resc, resv, argc, argv);
		break;
	case 407:
		_wrap_helicsMessageSetOriginalDestination(resc, resv, argc, argv);
		break;
	case 408:
		_wrap_helicsMessageSetTime(resc, resv, argc, argv);
		break;
	case 409:
		_wrap_helicsMessageResize(resc, resv, argc, argv);
		break;
	case 410:
		_wrap_helicsMessageReserve(resc, resv, argc, argv);
		break;
	case 411:
		_wrap_helicsMessageSetMessageID(resc, resv, argc, argv);
		break;
	case 412:
		_wrap_helicsMessageClearFlags(resc, resv, argc, argv);
		break;
	case 413:
		_wrap_helicsMessageSetFlagOption(resc, resv, argc, argv);
		break;
	case 414:
		_wrap_helicsMessageSetString(resc, resv, argc, argv);
		break;
	case 415:
		_wrap_helicsMessageSetData(resc, resv, argc, argv);
		break;
	case 416:
		_wrap_helicsMessageAppendData(resc, resv, argc, argv);
		break;
	case 417:
		_wrap_helicsMessageCopy(resc, resv, argc, argv);
		break;
	case 418:
		_wrap_helicsMessageClone(resc, resv, argc, argv);
		break;
	case 419:
		_wrap_helicsMessageFree(resc, resv, argc, argv);
		break;
	case 420:
		_wrap_helicsMessageClear(resc, resv, argc, argv);
		break;
	case 421:
		_wrap_helicsFederateRegisterFilter(resc, resv, argc, argv);
		break;
	case 422:
		_wrap_helicsFederateRegisterGlobalFilter(resc, resv, argc, argv);
		break;
	case 423:
		_wrap_helicsFederateRegisterCloningFilter(resc, resv, argc, argv);
		break;
	case 424:
		_wrap_helicsFederateRegisterGlobalCloningFilter(resc, resv, argc, argv);
		break;
	case 425:
		_wrap_helicsCoreRegisterFilter(resc, resv, argc, argv);
		break;
	case 426:
		_wrap_helicsCoreRegisterCloningFilter(resc, resv, argc, argv);
		break;
	case 427:
		_wrap_helicsFederateGetFilterCount(resc, resv, argc, argv);
		break;
	case 428:
		_wrap_helicsFederateGetFilter(resc, resv, argc, argv);
		break;
	case 429:
		_wrap_helicsFederateGetFilterByIndex(resc, resv, argc, argv);
		break;
	case 430:
		_wrap_helicsFilterIsValid(resc, resv, argc, argv);
		break;
	case 431:
		_wrap_helicsFilterGetName(resc, resv, argc, argv);
		break;
	case 432:
		_wrap_helicsFilterSet(resc, resv, argc, argv);
		break;
	case 433:
		_wrap_helicsFilterSetString(resc, resv, argc, argv);
		break;
	case 434:
		_wrap_helicsFilterAddDestinationTarget(resc, resv, argc, argv);
		break;
	case 435:
		_wrap_helicsFilterAddSourceTarget(resc, resv, argc, argv);
		break;
	case 436:
		_wrap_helicsFilterAddDeliveryEndpoint(resc, resv, argc, argv);
		break;
	case 437:
		_wrap_helicsFilterRemoveTarget(resc, resv, argc, argv);
		break;
	case 438:
		_wrap_helicsFilterRemoveDeliveryEndpoint(resc, resv, argc, argv);
		break;
	case 439:
		_wrap_helicsFilterGetInfo(resc, resv, argc, argv);
		break;
	case 440:
		_wrap_helicsFilterSetInfo(resc, resv, argc, argv);
		break;
	case 441:
		_wrap_helicsFilterGetTag(resc, resv, argc, argv);
		break;
	case 442:
		_wrap_helicsFilterSetTag(resc, resv, argc, argv);
		break;
	case 443:
		_wrap_helicsFilterSetOption(resc, resv, argc, argv);
		break;
	case 444:
		_wrap_helicsFilterGetOption(resc, resv, argc, argv);
		break;
	case 445:
		_wrap_helicsFederateRegisterTranslator(resc, resv, argc, argv);
		break;
	case 446:
		_wrap_helicsFederateRegisterGlobalTranslator(resc, resv, argc, argv);
		break;
	case 447:
		_wrap_helicsCoreRegisterTranslator(resc, resv, argc, argv);
		break;
	case 448:
		_wrap_helicsFederateGetTranslatorCount(resc, resv, argc, argv);
		break;
	case 449:
		_wrap_helicsFederateGetTranslator(resc, resv, argc, argv);
		break;
	case 450:
		_wrap_helicsFederateGetTranslatorByIndex(resc, resv, argc, argv);
		break;
	case 451:
		_wrap_helicsTranslatorIsValid(resc, resv, argc, argv);
		break;
	case 452:
		_wrap_helicsTranslatorGetName(resc, resv, argc, argv);
		break;
	case 453:
		_wrap_helicsTranslatorSet(resc, resv, argc, argv);
		break;
	case 454:
		_wrap_helicsTranslatorSetString(resc, resv, argc, argv);
		break;
	case 455:
		_wrap_helicsTranslatorAddInputTarget(resc, resv, argc, argv);
		break;
	case 456:
		_wrap_helicsTranslatorAddPublicationTarget(resc, resv, argc, argv);
		break;
	case 457:
		_wrap_helicsTranslatorAddSourceEndpoint(resc, resv, argc, argv);
		break;
	case 458:
		_wrap_helicsTranslatorAddDestinationEndpoint(resc, resv, argc, argv);
		break;
	case 459:
		_wrap_helicsTranslatorRemoveTarget(resc, resv, argc, argv);
		break;
	case 460:
		_wrap_helicsTranslatorGetInfo(resc, resv, argc, argv);
		break;
	case 461:
		_wrap_helicsTranslatorSetInfo(resc, resv, argc, argv);
		break;
	case 462:
		_wrap_helicsTranslatorGetTag(resc, resv, argc, argv);
		break;
	case 463:
		_wrap_helicsTranslatorSetTag(resc, resv, argc, argv);
		break;
	case 464:
		_wrap_helicsTranslatorSetOption(resc, resv, argc, argv);
		break;
	case 465:
		_wrap_helicsTranslatorGetOption(resc, resv, argc, argv);
		break;
	case 466:
		_wrap_helicsBrokerSetLoggingCallback(resc, resv, argc, argv);
		break;
	case 467:
		_wrap_helicsCoreSetLoggingCallback(resc, resv, argc, argv);
		break;
	case 468:
		_wrap_helicsFederateSetLoggingCallback(resc, resv, argc, argv);
		break;
	case 469:
		_wrap_helicsFilterSetCustomCallback(resc, resv, argc, argv);
		break;
	case 470:
		_wrap_helicsTranslatorSetCustomCallback(resc, resv, argc, argv);
		break;
	case 471:
		_wrap_helicsFederateSetQueryCallback(resc, resv, argc, argv);
		break;
	case 472:
		_wrap_helicsFederateSetTimeRequestEntryCallback(resc, resv, argc, argv);
		break;
	case 473:
		_wrap_helicsFederateSetTimeUpdateCallback(resc, resv, argc, argv);
		break;
	case 474:
		_wrap_helicsFederateSetStateChangeCallback(resc, resv, argc, argv);
		break;
	case 475:
		_wrap_helicsFederateSetTimeRequestReturnCallback(resc, resv, argc, argv);
		break;
	case 476:
		_wrap_helicsFederateInitializingEntryCallback(resc, resv, argc, argv);
		break;
	case 477:
		_wrap_helicsFederateExecutingEntryCallback(resc, resv, argc, argv);
		break;
	case 478:
		_wrap_helicsFederateCosimulationTerminationCallback(resc, resv, argc, argv);
		break;
	case 479:
		_wrap_helicsFederateErrorHandlerCallback(resc, resv, argc, argv);
		break;
	case 480:
		_wrap_helicsCallbackFederateNextTimeCallback(resc, resv, argc, argv);
		break;
	case 481:
		_wrap_helicsCallbackFederateNextTimeIterativeCallback(resc, resv, argc, argv);
		break;
	case 482:
		_wrap_helicsCallbackFederateInitializeCallback(resc, resv, argc, argv);
		break;
	case 483:
		_wrap_helicsQueryBufferFill(resc, resv, argc, argv);
		break;
	default:
		mexErrMsgIdAndTxt("helics:mexFunction","An unknown function id was encountered. Call the mex function with a valid function id.");
	}
}

