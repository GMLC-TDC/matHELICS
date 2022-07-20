#include "helics/helics.h"
#include <mex.h>
#include <stdexcept>
#include <string>
#include <unordered_map>

static int mexFunctionCalled = 0;

static void throwHelicsMatlabError(HelicsError *err) {
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
	{"HELICS_DATA_TYPE_CHAR",2},
	{"helicsCreateDataBuffer",66},
	{"helicsDataBufferIsValid",67},
	{"helicsWrapDataInBuffer",68},
	{"helicsDataBufferFree",69},
	{"helicsDataBufferSize",70},
	{"helicsDataBufferCapacity",71},
	{"helicsDataBufferData",72},
	{"helicsDataBufferReserve",73},
	{"helicsDataBufferClone",74},
	{"helicsIntegerToBytes",75},
	{"helicsDoubleToBytes",76},
	{"helicsStringToBytes",77},
	{"helicsRawStringToBytes",78},
	{"helicsBooleanToBytes",79},
	{"helicsCharToBytes",80},
	{"helicsTimeToBytes",81},
	{"helicsComplexToBytes",82},
	{"helicsComplexObjectToBytes",83},
	{"helicsVectorToBytes",84},
	{"helicsNamedPointToBytes",85},
	{"helicsComplexVectorToBytes",86},
	{"helicsDataBufferType",87},
	{"helicsDataBufferToInteger",88},
	{"helicsDataBufferToDouble",89},
	{"helicsDataBufferToBoolean",90},
	{"helicsDataBufferToChar",91},
	{"helicsDataBufferStringSize",92},
	{"helicsDataBufferToString",93},
	{"helicsDataBufferToRawString",94},
	{"helicsDataBufferToTime",95},
	{"helicsDataBufferToComplexObject",96},
	{"helicsDataBufferToComplex",97},
	{"helicsDataBufferVectorSize",98},
	{"helicsDataBufferToVector",99},
	{"helicsDataBufferToComplexVector",100},
	{"helicsDataBufferToNamedPoint",101},
	{"helicsDataBufferConvertToType",102},
	{"helicsGetVersion",103},
	{"helicsGetBuildFlags",104},
	{"helicsGetCompilerVersion",105},
	{"helicsGetSystemInfo",106},
	{"helicsLoadSignalHandler",109},
	{"helicsLoadThreadedSignalHandler",110},
	{"helicsClearSignalHandler",111},
	{"helicsLoadSignalHandlerCallback",112},
	{"helicsLoadSignalHandlerCallbackNoExit",113},
	{"helicsAbort",114},
	{"helicsIsCoreTypeAvailable",115},
	{"helicsCreateCore",116},
	{"helicsCreateCoreFromArgs",117},
	{"helicsCoreClone",118},
	{"helicsCoreIsValid",119},
	{"helicsCreateBroker",120},
	{"helicsCreateBrokerFromArgs",121},
	{"helicsBrokerClone",122},
	{"helicsBrokerIsValid",123},
	{"helicsBrokerIsConnected",124},
	{"helicsBrokerDataLink",125},
	{"helicsBrokerAddSourceFilterToEndpoint",126},
	{"helicsBrokerAddDestinationFilterToEndpoint",127},
	{"helicsBrokerMakeConnections",128},
	{"helicsCoreWaitForDisconnect",129},
	{"helicsBrokerWaitForDisconnect",130},
	{"helicsCoreIsConnected",131},
	{"helicsCoreDataLink",132},
	{"helicsCoreAddSourceFilterToEndpoint",133},
	{"helicsCoreAddDestinationFilterToEndpoint",134},
	{"helicsCoreMakeConnections",135},
	{"helicsBrokerGetIdentifier",136},
	{"helicsCoreGetIdentifier",137},
	{"helicsBrokerGetAddress",138},
	{"helicsCoreGetAddress",139},
	{"helicsCoreSetReadyToInit",140},
	{"helicsCoreConnect",141},
	{"helicsCoreDisconnect",142},
	{"helicsGetFederateByName",143},
	{"helicsBrokerDisconnect",144},
	{"helicsFederateDestroy",145},
	{"helicsBrokerDestroy",146},
	{"helicsCoreDestroy",147},
	{"helicsCoreFree",148},
	{"helicsBrokerFree",149},
	{"helicsCreateValueFederate",150},
	{"helicsCreateValueFederateFromConfig",151},
	{"helicsCreateMessageFederate",152},
	{"helicsCreateMessageFederateFromConfig",153},
	{"helicsCreateCombinationFederate",154},
	{"helicsCreateCombinationFederateFromConfig",155},
	{"helicsFederateClone",156},
	{"helicsFederateProtect",157},
	{"helicsFederateUnProtect",158},
	{"helicsFederateIsProtected",159},
	{"helicsCreateFederateInfo",160},
	{"helicsFederateInfoClone",161},
	{"helicsFederateInfoLoadFromArgs",162},
	{"helicsFederateInfoLoadFromString",163},
	{"helicsFederateInfoFree",164},
	{"helicsFederateIsValid",165},
	{"helicsFederateInfoSetCoreName",166},
	{"helicsFederateInfoSetCoreInitString",167},
	{"helicsFederateInfoSetBrokerInitString",168},
	{"helicsFederateInfoSetCoreType",169},
	{"helicsFederateInfoSetCoreTypeFromString",170},
	{"helicsFederateInfoSetBroker",171},
	{"helicsFederateInfoSetBrokerKey",172},
	{"helicsFederateInfoSetBrokerPort",173},
	{"helicsFederateInfoSetLocalPort",174},
	{"helicsGetPropertyIndex",175},
	{"helicsGetFlagIndex",176},
	{"helicsGetOptionIndex",177},
	{"helicsGetOptionValue",178},
	{"helicsGetDataType",179},
	{"helicsFederateInfoSetFlagOption",180},
	{"helicsFederateInfoSetSeparator",181},
	{"helicsFederateInfoSetTimeProperty",182},
	{"helicsFederateInfoSetIntegerProperty",183},
	{"helicsFederateRegisterInterfaces",184},
	{"helicsFederateGlobalError",185},
	{"helicsFederateLocalError",186},
	{"helicsFederateFinalize",187},
	{"helicsFederateFinalizeAsync",188},
	{"helicsFederateFinalizeComplete",189},
	{"helicsFederateDisconnect",190},
	{"helicsFederateDisconnectAsync",191},
	{"helicsFederateDisconnectComplete",192},
	{"helicsFederateFree",193},
	{"helicsCloseLibrary",194},
	{"helicsFederateEnterInitializingMode",195},
	{"helicsFederateEnterInitializingModeAsync",196},
	{"helicsFederateIsAsyncOperationCompleted",197},
	{"helicsFederateEnterInitializingModeComplete",198},
	{"helicsFederateEnterExecutingMode",199},
	{"helicsFederateEnterExecutingModeAsync",200},
	{"helicsFederateEnterExecutingModeComplete",201},
	{"helicsFederateEnterExecutingModeIterative",202},
	{"helicsFederateEnterExecutingModeIterativeAsync",203},
	{"helicsFederateEnterExecutingModeIterativeComplete",204},
	{"helicsFederateGetState",205},
	{"helicsFederateGetCore",206},
	{"helicsFederateRequestTime",207},
	{"helicsFederateRequestTimeAdvance",208},
	{"helicsFederateRequestNextStep",209},
	{"helicsFederateRequestTimeIterative",210},
	{"helicsFederateRequestTimeAsync",211},
	{"helicsFederateRequestTimeComplete",212},
	{"helicsFederateRequestTimeIterativeAsync",213},
	{"helicsFederateRequestTimeIterativeComplete",214},
	{"helicsFederateProcessCommunications",215},
	{"helicsFederateGetName",216},
	{"helicsFederateSetTimeProperty",217},
	{"helicsFederateSetFlagOption",218},
	{"helicsFederateSetSeparator",219},
	{"helicsFederateSetIntegerProperty",220},
	{"helicsFederateGetTimeProperty",221},
	{"helicsFederateGetFlagOption",222},
	{"helicsFederateGetIntegerProperty",223},
	{"helicsFederateGetCurrentTime",224},
	{"helicsFederateSetGlobal",225},
	{"helicsFederateSetTag",226},
	{"helicsFederateGetTag",227},
	{"helicsFederateAddDependency",228},
	{"helicsFederateSetLogFile",229},
	{"helicsFederateLogErrorMessage",230},
	{"helicsFederateLogWarningMessage",231},
	{"helicsFederateLogInfoMessage",232},
	{"helicsFederateLogDebugMessage",233},
	{"helicsFederateLogLevelMessage",234},
	{"helicsFederateSendCommand",235},
	{"helicsFederateGetCommand",236},
	{"helicsFederateGetCommandSource",237},
	{"helicsFederateWaitCommand",238},
	{"helicsCoreSetGlobal",239},
	{"helicsBrokerSetGlobal",240},
	{"helicsCoreSendCommand",241},
	{"helicsCoreSendOrderedCommand",242},
	{"helicsBrokerSendCommand",243},
	{"helicsBrokerSendOrderedCommand",244},
	{"helicsCoreSetLogFile",245},
	{"helicsBrokerSetLogFile",246},
	{"helicsBrokerSetTimeBarrier",247},
	{"helicsBrokerClearTimeBarrier",248},
	{"helicsBrokerGlobalError",249},
	{"helicsCoreGlobalError",250},
	{"helicsCreateQuery",251},
	{"helicsQueryExecute",252},
	{"helicsQueryCoreExecute",253},
	{"helicsQueryBrokerExecute",254},
	{"helicsQueryExecuteAsync",255},
	{"helicsQueryExecuteComplete",256},
	{"helicsQueryIsCompleted",257},
	{"helicsQuerySetTarget",258},
	{"helicsQuerySetQueryString",259},
	{"helicsQuerySetOrdering",260},
	{"helicsQueryFree",261},
	{"helicsCleanupLibrary",262},
	{"helicsFederateRegisterSubscription",263},
	{"helicsFederateRegisterPublication",264},
	{"helicsFederateRegisterTypePublication",265},
	{"helicsFederateRegisterGlobalPublication",266},
	{"helicsFederateRegisterGlobalTypePublication",267},
	{"helicsFederateRegisterInput",268},
	{"helicsFederateRegisterTypeInput",269},
	{"helicsFederateRegisterGlobalInput",270},
	{"helicsFederateRegisterGlobalTypeInput",271},
	{"helicsFederateGetPublication",272},
	{"helicsFederateGetPublicationByIndex",273},
	{"helicsFederateGetInput",274},
	{"helicsFederateGetInputByIndex",275},
	{"helicsFederateGetSubscription",276},
	{"helicsFederateClearUpdates",277},
	{"helicsFederateRegisterFromPublicationJSON",278},
	{"helicsFederatePublishJSON",279},
	{"helicsPublicationIsValid",280},
	{"helicsPublicationPublishBytes",281},
	{"helicsPublicationPublishString",282},
	{"helicsPublicationPublishInteger",283},
	{"helicsPublicationPublishBoolean",284},
	{"helicsPublicationPublishDouble",285},
	{"helicsPublicationPublishTime",286},
	{"helicsPublicationPublishChar",287},
	{"helicsPublicationPublishComplex",288},
	{"helicsPublicationPublishVector",289},
	{"helicsPublicationPublishComplexVector",290},
	{"helicsPublicationPublishNamedPoint",291},
	{"helicsPublicationAddTarget",292},
	{"helicsInputIsValid",293},
	{"helicsInputAddTarget",294},
	{"helicsInputGetByteCount",295},
	{"helicsInputGetBytes",296},
	{"helicsInputGetStringSize",297},
	{"helicsInputGetString",298},
	{"helicsInputGetInteger",299},
	{"helicsInputGetBoolean",300},
	{"helicsInputGetDouble",301},
	{"helicsInputGetTime",302},
	{"helicsInputGetChar",303},
	{"helicsInputGetComplexObject",304},
	{"helicsInputGetComplex",305},
	{"helicsInputGetVectorSize",306},
	{"helicsInputGetVector",307},
	{"helicsInputGetComplexVector",308},
	{"helicsInputGetNamedPoint",309},
	{"helicsInputSetDefaultBytes",310},
	{"helicsInputSetDefaultString",311},
	{"helicsInputSetDefaultInteger",312},
	{"helicsInputSetDefaultBoolean",313},
	{"helicsInputSetDefaultTime",314},
	{"helicsInputSetDefaultChar",315},
	{"helicsInputSetDefaultDouble",316},
	{"helicsInputSetDefaultComplex",317},
	{"helicsInputSetDefaultVector",318},
	{"helicsInputSetDefaultComplexVector",319},
	{"helicsInputSetDefaultNamedPoint",320},
	{"helicsInputGetType",321},
	{"helicsInputGetPublicationType",322},
	{"helicsInputGetPublicationDataType",323},
	{"helicsPublicationGetType",324},
	{"helicsInputGetName",325},
	{"helicsSubscriptionGetTarget",326},
	{"helicsPublicationGetName",327},
	{"helicsInputGetUnits",328},
	{"helicsInputGetInjectionUnits",329},
	{"helicsInputGetExtractionUnits",330},
	{"helicsPublicationGetUnits",331},
	{"helicsInputGetInfo",332},
	{"helicsInputSetInfo",333},
	{"helicsInputGetTag",334},
	{"helicsInputSetTag",335},
	{"helicsPublicationGetInfo",336},
	{"helicsPublicationSetInfo",337},
	{"helicsPublicationGetTag",338},
	{"helicsPublicationSetTag",339},
	{"helicsInputGetOption",340},
	{"helicsInputSetOption",341},
	{"helicsPublicationGetOption",342},
	{"helicsPublicationSetOption",343},
	{"helicsPublicationSetMinimumChange",344},
	{"helicsInputSetMinimumChange",345},
	{"helicsInputIsUpdated",346},
	{"helicsInputLastUpdateTime",347},
	{"helicsInputClearUpdate",348},
	{"helicsFederateGetPublicationCount",349},
	{"helicsFederateGetInputCount",350},
	{"helicsFederateRegisterEndpoint",351},
	{"helicsFederateRegisterGlobalEndpoint",352},
	{"helicsFederateRegisterTargetedEndpoint",353},
	{"helicsFederateRegisterGlobalTargetedEndpoint",354},
	{"helicsFederateGetEndpoint",355},
	{"helicsFederateGetEndpointByIndex",356},
	{"helicsEndpointIsValid",357},
	{"helicsEndpointSetDefaultDestination",358},
	{"helicsEndpointGetDefaultDestination",359},
	{"helicsEndpointSendBytes",360},
	{"helicsEndpointSendBytesTo",361},
	{"helicsEndpointSendBytesToAt",362},
	{"helicsEndpointSendBytesAt",363},
	{"helicsEndpointSendMessage",364},
	{"helicsEndpointSendMessageZeroCopy",365},
	{"helicsEndpointSubscribe",366},
	{"helicsFederateHasMessage",367},
	{"helicsEndpointHasMessage",368},
	{"helicsFederatePendingMessageCount",369},
	{"helicsEndpointPendingMessageCount",370},
	{"helicsEndpointGetMessage",371},
	{"helicsEndpointCreateMessage",372},
	{"helicsEndpointClearMessages",373},
	{"helicsFederateGetMessage",374},
	{"helicsFederateCreateMessage",375},
	{"helicsFederateClearMessages",376},
	{"helicsEndpointGetType",377},
	{"helicsEndpointGetName",378},
	{"helicsFederateGetEndpointCount",379},
	{"helicsEndpointGetInfo",380},
	{"helicsEndpointSetInfo",381},
	{"helicsEndpointGetTag",382},
	{"helicsEndpointSetTag",383},
	{"helicsEndpointSetOption",384},
	{"helicsEndpointGetOption",385},
	{"helicsEndpointAddSourceTarget",386},
	{"helicsEndpointAddDestinationTarget",387},
	{"helicsEndpointRemoveTarget",388},
	{"helicsEndpointAddSourceFilter",389},
	{"helicsEndpointAddDestinationFilter",390},
	{"helicsMessageGetSource",391},
	{"helicsMessageGetDestination",392},
	{"helicsMessageGetOriginalSource",393},
	{"helicsMessageGetOriginalDestination",394},
	{"helicsMessageGetTime",395},
	{"helicsMessageGetString",396},
	{"helicsMessageGetMessageID",397},
	{"helicsMessageGetFlagOption",398},
	{"helicsMessageGetByteCount",399},
	{"helicsMessageGetBytes",400},
	{"helicsMessageGetBytesPointer",401},
	{"helicsMessageIsValid",402},
	{"helicsMessageSetSource",403},
	{"helicsMessageSetDestination",404},
	{"helicsMessageSetOriginalSource",405},
	{"helicsMessageSetOriginalDestination",406},
	{"helicsMessageSetTime",407},
	{"helicsMessageResize",408},
	{"helicsMessageReserve",409},
	{"helicsMessageSetMessageID",410},
	{"helicsMessageClearFlags",411},
	{"helicsMessageSetFlagOption",412},
	{"helicsMessageSetString",413},
	{"helicsMessageSetData",414},
	{"helicsMessageAppendData",415},
	{"helicsMessageCopy",416},
	{"helicsMessageClone",417},
	{"helicsMessageFree",418},
	{"helicsMessageClear",419},
	{"helicsFederateRegisterFilter",420},
	{"helicsFederateRegisterGlobalFilter",421},
	{"helicsFederateRegisterCloningFilter",422},
	{"helicsFederateRegisterGlobalCloningFilter",423},
	{"helicsCoreRegisterFilter",424},
	{"helicsCoreRegisterCloningFilter",425},
	{"helicsFederateGetFilterCount",426},
	{"helicsFederateGetFilter",427},
	{"helicsFederateGetFilterByIndex",428},
	{"helicsFilterIsValid",429},
	{"helicsFilterGetName",430},
	{"helicsFilterSet",431},
	{"helicsFilterSetString",432},
	{"helicsFilterAddDestinationTarget",433},
	{"helicsFilterAddSourceTarget",434},
	{"helicsFilterAddDeliveryEndpoint",435},
	{"helicsFilterRemoveTarget",436},
	{"helicsFilterRemoveDeliveryEndpoint",437},
	{"helicsFilterGetInfo",438},
	{"helicsFilterSetInfo",439},
	{"helicsFilterGetTag",440},
	{"helicsFilterSetTag",441},
	{"helicsFilterSetOption",442},
	{"helicsFilterGetOption",443},
	{"helicsFederateRegisterTranslator",444},
	{"helicsFederateRegisterGlobalTranslator",445},
	{"helicsCoreRegisterTranslator",446},
	{"helicsFederateGetTranslatorCount",447},
	{"helicsFederateGetTranslator",448},
	{"helicsFederateGetTranslatorByIndex",449},
	{"helicsTranslatorIsValid",450},
	{"helicsTranslatorGetName",451},
	{"helicsTranslatorSet",452},
	{"helicsTranslatorSetString",453},
	{"helicsTranslatorAddInputTarget",454},
	{"helicsTranslatorAddPublicationTarget",455},
	{"helicsTranslatorAddSourceEndpoint",456},
	{"helicsTranslatorAddDestinationEndpoint",457},
	{"helicsTranslatorRemoveTarget",458},
	{"helicsTranslatorGetInfo",459},
	{"helicsTranslatorSetInfo",460},
	{"helicsTranslatorGetTag",461},
	{"helicsTranslatorSetTag",462},
	{"helicsTranslatorSetOption",463},
	{"helicsTranslatorGetOption",464},
	{"helicsBrokerSetLoggingCallback",465},
	{"helicsCoreSetLoggingCallback",466},
	{"helicsFederateSetLoggingCallback",467},
	{"helicsFilterSetCustomCallback",468},
	{"helicsTranslatorSetCustomCallback",469},
	{"helicsFederateSetQueryCallback",470},
	{"helicsFederateSetTimeRequestEntryCallback",471},
	{"helicsFederateSetTimeUpdateCallback",472},
	{"helicsFederateSetStateChangeCallback",473},
	{"helicsFederateSetTimeRequestReturnCallback",474},
	{"helicsQueryBufferFill",475}
};

void _wrap_HELICS_DATA_TYPE_CHAR(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	mxArray *_out = mxCreateNumericMatrix(1,1,mxINT64_CLASS,mxREAL);
	*((int64_t*)mxGetData(_out)) = (int64_t)HELICS_DATA_TYPE_CHAR;
	resv[0] = _out;
}

void _wrap_helicsCreateDataBuffer(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	int32_t initialCapacity = *((int32_t *)mxGetData(argv[0]));

	HelicsDataBuffer result = helicsCreateDataBuffer(initialCapacity);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_t*)mxGetData(_out)) = (uint64_t)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsDataBufferIsValid(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsDataBuffer data = *(HelicsDataBuffer*)(mxGetData(argv[0]));

	HelicsBool result = helicsDataBufferIsValid(data);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT32_CLASS, mxREAL);
	*((int32_t*)mxGetData(_out)) = (int32_t)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsWrapDataInBuffer(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	void *data = mxGetData(argv[0]);

	int dataSize = (int)(mxGetScalar(argv[1]));

	int dataCapacity = (int)(mxGetScalar(argv[2]));

	HelicsDataBuffer result = helicsWrapDataInBuffer(data, dataSize, dataCapacity);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_t*)mxGetData(_out)) = (uint64_t)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}






}


void _wrap_helicsDataBufferFree(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsDataBuffer data = *(HelicsDataBuffer*)(mxGetData(argv[0]));

	helicsDataBufferFree(data);

	mxArray *_out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsDataBufferSize(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsDataBuffer data = *(HelicsDataBuffer*)(mxGetData(argv[0]));

	int32_t result = helicsDataBufferSize(data);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT32_CLASS, mxREAL);
	*((int32_t*)mxGetData(_out)) = (int32_t)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsDataBufferCapacity(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsDataBuffer data = *(HelicsDataBuffer*)(mxGetData(argv[0]));

	int32_t result = helicsDataBufferCapacity(data);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT32_CLASS, mxREAL);
	*((int32_t*)mxGetData(_out)) = (int32_t)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsDataBufferData(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsDataBuffer data = *(HelicsDataBuffer*)(mxGetData(argv[0]));

	void *result = helicsDataBufferData(data);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_t*)mxGetData(_out)) = (uint64_t)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsDataBufferReserve(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsDataBuffer data = *(HelicsDataBuffer*)(mxGetData(argv[0]));

	int32_t newCapacity = *((int32_t *)mxGetData(argv[1]));

	HelicsBool result = helicsDataBufferReserve(data, newCapacity);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT32_CLASS, mxREAL);
	*((int32_t*)mxGetData(_out)) = (int32_t)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}




}


void _wrap_helicsDataBufferClone(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsDataBuffer data = *(HelicsDataBuffer*)(mxGetData(argv[0]));

	HelicsDataBuffer result = helicsDataBufferClone(data);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_t*)mxGetData(_out)) = (uint64_t)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsIntegerToBytes(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	int64_t value = *((int64_t *)mxGetData(argv[0]));

	HelicsDataBuffer data = helicsCreateDataBuffer(sizeof(int));

	int32_t result = helicsIntegerToBytes(value, data);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_t*)mxGetData(_out)) = (uint64_t)data;

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsDoubleToBytes(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	double value = mxGetScalar(argv[0]);

	HelicsDataBuffer data = helicsCreateDataBuffer(sizeof(double));

	int32_t result = helicsDoubleToBytes(value, data);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_t*)mxGetData(_out)) = (uint64_t)data;

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsStringToBytes(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	char *value;
	size_t valueLength;
	int valueStatus;
	valueLength = mxGetN(argv[0]) + 1;
	value = (char *)malloc(valueLength);
	valueStatus = mxGetString(argv[0], value, valueLength);

	HelicsDataBuffer data = helicsCreateDataBuffer(static_cast<int32_t>(valueLength));

	int32_t result = helicsStringToBytes(value, data);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_t*)mxGetData(_out)) = (uint64_t)data;

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsRawStringToBytes(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	char *str;
	size_t strLength;
	int strStatus;
	strLength = mxGetN(argv[0]) + 1;
	str = (char *)malloc(strLength);
	strStatus = mxGetString(argv[0], str, strLength);

	HelicsDataBuffer data = helicsCreateDataBuffer(static_cast<int32_t>(strLength));

	int32_t result = helicsRawStringToBytes(str, (int)strLength, data);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_t*)mxGetData(_out)) = (uint64_t)data;

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsBooleanToBytes(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsBool value = (HelicsBool)(mxGetScalar(argv[0]));

	HelicsDataBuffer data = helicsCreateDataBuffer(sizeof(HelicsBool));

	int32_t result = helicsBooleanToBytes(value, data);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_t*)mxGetData(_out)) = (uint64_t)data;

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsCharToBytes(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	char *value;
	size_t valueLength;
	int valueStatus;
	valueLength = mxGetN(argv[0]) + 1;
	value = (char *)malloc(valueLength);
	valueStatus = mxGetString(argv[0], value, valueLength);

	HelicsDataBuffer data = helicsCreateDataBuffer(sizeof(char));

	int32_t result = helicsCharToBytes(*value, data);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_t*)mxGetData(_out)) = (uint64_t)data;

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsTimeToBytes(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsTime value = (HelicsTime)(mxGetScalar(argv[0]));

	HelicsDataBuffer data = helicsCreateDataBuffer(sizeof(HelicsTime));

	int32_t result = helicsTimeToBytes(value, data);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_t*)mxGetData(_out)) = (uint64_t)data;

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsComplexToBytes(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	mxComplexDouble *value = mxGetComplexDoubles(argv[1]);

	HelicsDataBuffer data = helicsCreateDataBuffer(2*sizeof(double));

	int32_t result = helicsComplexToBytes(value->real, value->imag, data);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_t*)mxGetData(_out)) = (uint64_t)data;

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsComplexObjectToBytes(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	mxComplexDouble *value = mxGetComplexDoubles(argv[1]);

	HelicsDataBuffer data = helicsCreateDataBuffer(sizeof(HelicsComplex));

	HelicsComplex vObj = {value->real, value->imag};

	int32_t result = helicsComplexObjectToBytes(vObj, data);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_t*)mxGetData(_out)) = (uint64_t)data;

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsVectorToBytes(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	int dataSize =  (int)mxGetNumberOfElements(argv[0]);

	double *value =  (double *)mxGetDoubles(argv[0]);

	HelicsDataBuffer data = helicsCreateDataBuffer(dataSize*sizeof(double));

	int32_t result = helicsVectorToBytes((const double *)value, dataSize, data);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_t*)mxGetData(_out)) = (uint64_t)data;

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsNamedPointToBytes(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	char *name;
	size_t nameLength;
	int nameStatus;
	nameLength = mxGetN(argv[0]) + 1;
	name = (char *)malloc(nameLength);
	nameStatus = mxGetString(argv[0], name, nameLength);

	double value = mxGetScalar(argv[1]);

	HelicsDataBuffer data = helicsCreateDataBuffer(sizeof(double));

	int32_t result = helicsNamedPointToBytes(name, value, data);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_t*)mxGetData(_out)) = (uint64_t)data;

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsComplexVectorToBytes(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	int dataSize =  (int)mxGetN(argv[0])*2;

	double *value = (double *)malloc(dataSize * sizeof(double));
	mxComplexDouble *vals = mxGetComplexDoubles(argv[0]);
	for(int i=0; i<dataSize/2; ++i){
		value[2*i] = vals[i].real;
		value[2*i + 1] = vals[i].imag;
	}

	HelicsDataBuffer data = helicsCreateDataBuffer(dataSize*sizeof(double));

	int32_t result = helicsComplexVectorToBytes((const double *)value, dataSize, data);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_t*)mxGetData(_out)) = (uint64_t)data;

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsDataBufferType(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsDataBuffer data = *(HelicsDataBuffer*)(mxGetData(argv[0]));

	int result = helicsDataBufferType(data);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
	*((int64_t*)mxGetData(_out)) = (int64_t)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsDataBufferToInteger(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsDataBuffer data = *(HelicsDataBuffer*)(mxGetData(argv[0]));

	int64_t result = helicsDataBufferToInteger(data);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
	*((int64_t*)mxGetData(_out)) = (int64_t)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsDataBufferToDouble(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsDataBuffer data = *(HelicsDataBuffer*)(mxGetData(argv[0]));

	double result = helicsDataBufferToDouble(data);

	mxArray *_out = mxCreateDoubleScalar(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsDataBufferToBoolean(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsDataBuffer data = *(HelicsDataBuffer*)(mxGetData(argv[0]));

	HelicsBool result = helicsDataBufferToBoolean(data);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT32_CLASS, mxREAL);
	*((int32_t*)mxGetData(_out)) = (int32_t)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsDataBufferToChar(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsDataBuffer data = *(HelicsDataBuffer*)(mxGetData(argv[0]));

	char result = helicsDataBufferToChar(data);

	mxArray *_out = mxCreateString((const char *)(&result));

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsDataBufferStringSize(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsDataBuffer data = *(HelicsDataBuffer*)(mxGetData(argv[0]));

	int result = helicsDataBufferStringSize(data);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
	*((int64_t*)mxGetData(_out)) = (int64_t)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsDataBufferToString(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsDataBuffer data = *(HelicsDataBuffer*)(mxGetData(argv[0]));

	int maxStringLen = helicsDataBufferStringSize(data) + 2;

	char *outputString = (char *)malloc(maxStringLen);

	int actualLength = 0;

	helicsDataBufferToString(data, outputString, maxStringLen, &actualLength);

	mwSize dims[2] = {1, static_cast<mwSize>(actualLength)};
	mxArray *_out = mxCreateCharArray(2, dims);
	mxChar *out_data = (mxChar *)mxGetData(_out);
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
	HelicsDataBuffer data = *(HelicsDataBuffer*)(mxGetData(argv[0]));

	int maxStringLen = helicsDataBufferStringSize(data) + 2;

	char *outputString = (char *)malloc(maxStringLen);

	int actualLength = 0;

	helicsDataBufferToRawString(data, outputString, maxStringLen, &actualLength);

	mwSize dims[2] = {1, static_cast<mwSize>(actualLength) - 1};
	mxArray *_out = mxCreateCharArray(2, dims);
	mxChar *out_data = (mxChar *)mxGetData(_out);
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
	HelicsDataBuffer data = *(HelicsDataBuffer*)(mxGetData(argv[0]));

	HelicsTime result = helicsDataBufferToTime(data);

	mxArray *_out = mxCreateDoubleScalar(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsDataBufferToComplexObject(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsDataBuffer data = *(HelicsDataBuffer*)(mxGetData(argv[0]));

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
	HelicsDataBuffer data = *(HelicsDataBuffer*)(mxGetData(argv[0]));

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
	HelicsDataBuffer data = *(HelicsDataBuffer*)(mxGetData(argv[0]));

	int result = helicsDataBufferVectorSize(data);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
	*((int64_t*)mxGetData(_out)) = (int64_t)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsDataBufferToVector(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsDataBuffer data = *(HelicsDataBuffer*)(mxGetData(argv[0]));

	int maxLen = helicsDataBufferVectorSize(data);

	double *values = (double *)malloc(maxLen * sizeof(double));

	int actualSize = 0;

	helicsDataBufferToVector(data, values, maxLen, &actualSize);

	mxDouble *result_data = (mxDouble *)mxMalloc(actualSize * sizeof(mxDouble));
	for(int i=0; i<actualSize; ++i){
		result_data[i] = (mxDouble)values[i];
	}
	mxArray *_out = mxCreateDoubleMatrix(actualSize, 1, mxREAL);
	int status = mxSetDoubles(_out, &(result_data[0]));

	if(_out){
		--resc;
		*resv++ = _out;
	}

}


void _wrap_helicsDataBufferToComplexVector(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsDataBuffer data = *(HelicsDataBuffer*)(mxGetData(argv[0]));

	int maxLen = helicsDataBufferVectorSize(data);

	double *values = (double *)malloc(maxLen * sizeof(double));

	int actualSize = 0;

	helicsDataBufferToComplexVector(data, values, maxLen, &actualSize);

	mxComplexDouble *result_data = (mxComplexDouble *)mxMalloc((actualSize/2)*sizeof(mxComplexDouble));
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
	HelicsDataBuffer data = *(HelicsDataBuffer*)(mxGetData(argv[0]));

	int maxStringLen = helicsInputGetStringSize(data) + 2;

	char *outputString = (char *)malloc(maxStringLen);

	int actualLength = 0;

	double val = 0;

	helicsDataBufferToNamedPoint(data, outputString, maxStringLen, &actualLength, &val);

	mwSize dims[2] = {1, static_cast<mwSize>(actualLength)-1};
	mxArray *_out = mxCreateCharArray(2, dims);
	mxChar *out_data = (mxChar *)mxGetData(_out);
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
	HelicsDataBuffer data = *(HelicsDataBuffer*)(mxGetData(argv[0]));

	int newDataType = (int)(mxGetScalar(argv[1]));

	HelicsBool result = helicsDataBufferConvertToType(data, newDataType);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT32_CLASS, mxREAL);
	*((int32_t*)mxGetData(_out)) = (int32_t)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}




}


void _wrap_helicsGetVersion(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	const char *result = helicsGetVersion();

	mxArray *_out = mxCreateString(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsGetBuildFlags(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	const char *result = helicsGetBuildFlags();

	mxArray *_out = mxCreateString(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsGetCompilerVersion(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	const char *result = helicsGetCompilerVersion();

	mxArray *_out = mxCreateString(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsGetSystemInfo(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	const char *result = helicsGetSystemInfo();

	mxArray *_out = mxCreateString(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsLoadSignalHandler(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	helicsLoadSignalHandler();

	mxArray *_out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsLoadThreadedSignalHandler(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	helicsLoadThreadedSignalHandler();

	mxArray *_out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsClearSignalHandler(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	helicsClearSignalHandler();

	mxArray *_out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsAbort(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	int errorCode = (int)(mxGetScalar(argv[0]));

	char *errorString;
	size_t errorStringLength;
	int errorStringStatus;
	errorStringLength = mxGetN(argv[1]) + 1;
	errorString = (char *)malloc(errorStringLength);
	errorStringStatus = mxGetString(argv[1], errorString, errorStringLength);

	helicsAbort(errorCode, (char const *)errorString);

	mxArray *_out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	free(errorString);
}


void _wrap_helicsIsCoreTypeAvailable(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	char *type;
	size_t typeLength;
	int typeStatus;
	typeLength = mxGetN(argv[0]) + 1;
	type = (char *)malloc(typeLength);
	typeStatus = mxGetString(argv[0], type, typeLength);

	HelicsBool result = helicsIsCoreTypeAvailable((char const *)type);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT32_CLASS, mxREAL);
	*((int32_t*)mxGetData(_out)) = (int32_t)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(type);
}


void _wrap_helicsCreateCore(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	char *type;
	size_t typeLength;
	int typeStatus;
	typeLength = mxGetN(argv[0]) + 1;
	type = (char *)malloc(typeLength);
	typeStatus = mxGetString(argv[0], type, typeLength);

	char *name;
	size_t nameLength;
	int nameStatus;
	nameLength = mxGetN(argv[1]) + 1;
	name = (char *)malloc(nameLength);
	nameStatus = mxGetString(argv[1], name, nameLength);

	char *initString;
	size_t initStringLength;
	int initStringStatus;
	initStringLength = mxGetN(argv[2]) + 1;
	initString = (char *)malloc(initStringLength);
	initStringStatus = mxGetString(argv[2], initString, initStringLength);

	HelicsError err = helicsErrorInitialize();

	HelicsCore result = helicsCreateCore((char const *)type, (char const *)name, (char const *)initString, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_t*)mxGetData(_out)) = (uint64_t)result;

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
	char *type;
	size_t typeLength;
	int typeStatus;
	typeLength = mxGetN(argv[0]) + 1;
	type = (char *)malloc(typeLength);
	typeStatus = mxGetString(argv[0], type, typeLength);

	char *name;
	size_t nameLength;
	int nameStatus;
	nameLength = mxGetN(argv[1]) + 1;
	name = (char *)malloc(nameLength);
	nameStatus = mxGetString(argv[1], name, nameLength);

	int arg2 = 0;
	char **arg3;
	int ii;
	arg2 = static_cast<int>(mxGetNumberOfElements(argv[2]));
	arg3 = (char **)malloc((arg2)*sizeof(char *));
	for (ii=0;ii<arg2;ii++){
		mxArray *cellElement=mxGetCell(argv[2], ii);
		size_t len = mxGetN(cellElement) + 1;
		arg3[ii] = (char *)malloc(static_cast<int>(len));
		int flag = mxGetString(cellElement, arg3[ii], static_cast<int>(len));
	}

	HelicsError err = helicsErrorInitialize();

	HelicsCore result = helicsCreateCoreFromArgs(type, name, arg2, arg3, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_t*)mxGetData(_out)) = (uint64_t)result;

	if(_out){
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
	HelicsCore core = *(HelicsCore*)(mxGetData(argv[0]));

	HelicsError err = helicsErrorInitialize();

	HelicsCore result = helicsCoreClone(core, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_t*)mxGetData(_out)) = (uint64_t)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsCoreIsValid(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsCore core = *(HelicsCore*)(mxGetData(argv[0]));

	HelicsBool result = helicsCoreIsValid(core);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT32_CLASS, mxREAL);
	*((int32_t*)mxGetData(_out)) = (int32_t)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsCreateBroker(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	char *type;
	size_t typeLength;
	int typeStatus;
	typeLength = mxGetN(argv[0]) + 1;
	type = (char *)malloc(typeLength);
	typeStatus = mxGetString(argv[0], type, typeLength);

	char *name;
	size_t nameLength;
	int nameStatus;
	nameLength = mxGetN(argv[1]) + 1;
	name = (char *)malloc(nameLength);
	nameStatus = mxGetString(argv[1], name, nameLength);

	char *initString;
	size_t initStringLength;
	int initStringStatus;
	initStringLength = mxGetN(argv[2]) + 1;
	initString = (char *)malloc(initStringLength);
	initStringStatus = mxGetString(argv[2], initString, initStringLength);

	HelicsError err = helicsErrorInitialize();

	HelicsBroker result = helicsCreateBroker((char const *)type, (char const *)name, (char const *)initString, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_t*)mxGetData(_out)) = (uint64_t)result;

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
	char *type;
	size_t typeLength;
	int typeStatus;
	typeLength = mxGetN(argv[0]) + 1;
	type = (char *)malloc(typeLength);
	typeStatus = mxGetString(argv[0], type, typeLength);

	char *name;
	size_t nameLength;
	int nameStatus;
	nameLength = mxGetN(argv[1]) + 1;
	name = (char *)malloc(nameLength);
	nameStatus = mxGetString(argv[1], name, nameLength);

	int arg2 = 0;
	char **arg3;
	int ii;
	arg2 = static_cast<int>(mxGetNumberOfElements(argv[2]));
	arg3 = (char **)malloc((arg2)*sizeof(char *));
	for (ii=0;ii<arg2;ii++){
		mxArray *cellElement=mxGetCell(argv[2], ii);
		size_t len = mxGetN(cellElement) + 1;
		arg3[ii] = (char *)malloc(static_cast<int>(len));
		int flag = mxGetString(cellElement, arg3[ii], static_cast<int>(len));
	}

	HelicsError err = helicsErrorInitialize();

	HelicsBroker result = helicsCreateBrokerFromArgs(type, name, arg2, arg3, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_t*)mxGetData(_out)) = (uint64_t)result;

	if(_out){
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
	HelicsBroker broker = *(HelicsBroker*)(mxGetData(argv[0]));

	HelicsError err = helicsErrorInitialize();

	HelicsBroker result = helicsBrokerClone(broker, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_t*)mxGetData(_out)) = (uint64_t)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsBrokerIsValid(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsBroker broker = *(HelicsBroker*)(mxGetData(argv[0]));

	HelicsBool result = helicsBrokerIsValid(broker);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT32_CLASS, mxREAL);
	*((int32_t*)mxGetData(_out)) = (int32_t)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsBrokerIsConnected(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsBroker broker = *(HelicsBroker*)(mxGetData(argv[0]));

	HelicsBool result = helicsBrokerIsConnected(broker);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT32_CLASS, mxREAL);
	*((int32_t*)mxGetData(_out)) = (int32_t)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsBrokerDataLink(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsBroker broker = *(HelicsBroker*)(mxGetData(argv[0]));

	char *source;
	size_t sourceLength;
	int sourceStatus;
	sourceLength = mxGetN(argv[1]) + 1;
	source = (char *)malloc(sourceLength);
	sourceStatus = mxGetString(argv[1], source, sourceLength);

	char *target;
	size_t targetLength;
	int targetStatus;
	targetLength = mxGetN(argv[2]) + 1;
	target = (char *)malloc(targetLength);
	targetStatus = mxGetString(argv[2], target, targetLength);

	HelicsError err = helicsErrorInitialize();

	helicsBrokerDataLink(broker, (char const *)source, (char const *)target, &err);

	mxArray *_out = (mxArray *)0;

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
	HelicsBroker broker = *(HelicsBroker*)(mxGetData(argv[0]));

	char *filter;
	size_t filterLength;
	int filterStatus;
	filterLength = mxGetN(argv[1]) + 1;
	filter = (char *)malloc(filterLength);
	filterStatus = mxGetString(argv[1], filter, filterLength);

	char *endpoint;
	size_t endpointLength;
	int endpointStatus;
	endpointLength = mxGetN(argv[2]) + 1;
	endpoint = (char *)malloc(endpointLength);
	endpointStatus = mxGetString(argv[2], endpoint, endpointLength);

	HelicsError err = helicsErrorInitialize();

	helicsBrokerAddSourceFilterToEndpoint(broker, (char const *)filter, (char const *)endpoint, &err);

	mxArray *_out = (mxArray *)0;

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
	HelicsBroker broker = *(HelicsBroker*)(mxGetData(argv[0]));

	char *filter;
	size_t filterLength;
	int filterStatus;
	filterLength = mxGetN(argv[1]) + 1;
	filter = (char *)malloc(filterLength);
	filterStatus = mxGetString(argv[1], filter, filterLength);

	char *endpoint;
	size_t endpointLength;
	int endpointStatus;
	endpointLength = mxGetN(argv[2]) + 1;
	endpoint = (char *)malloc(endpointLength);
	endpointStatus = mxGetString(argv[2], endpoint, endpointLength);

	HelicsError err = helicsErrorInitialize();

	helicsBrokerAddDestinationFilterToEndpoint(broker, (char const *)filter, (char const *)endpoint, &err);

	mxArray *_out = (mxArray *)0;

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
	HelicsBroker broker = *(HelicsBroker*)(mxGetData(argv[0]));

	char *file;
	size_t fileLength;
	int fileStatus;
	fileLength = mxGetN(argv[1]) + 1;
	file = (char *)malloc(fileLength);
	fileStatus = mxGetString(argv[1], file, fileLength);

	HelicsError err = helicsErrorInitialize();

	helicsBrokerMakeConnections(broker, (char const *)file, &err);

	mxArray *_out = (mxArray *)0;

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
	HelicsCore core = *(HelicsCore*)(mxGetData(argv[0]));

	int msToWait = (int)(mxGetScalar(argv[1]));

	HelicsError err = helicsErrorInitialize();

	HelicsBool result = helicsCoreWaitForDisconnect(core, msToWait, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT32_CLASS, mxREAL);
	*((int32_t*)mxGetData(_out)) = (int32_t)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}





	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsBrokerWaitForDisconnect(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsBroker broker = *(HelicsBroker*)(mxGetData(argv[0]));

	int msToWait = (int)(mxGetScalar(argv[1]));

	HelicsError err = helicsErrorInitialize();

	HelicsBool result = helicsBrokerWaitForDisconnect(broker, msToWait, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT32_CLASS, mxREAL);
	*((int32_t*)mxGetData(_out)) = (int32_t)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}





	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsCoreIsConnected(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsCore core = *(HelicsCore*)(mxGetData(argv[0]));

	HelicsBool result = helicsCoreIsConnected(core);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT32_CLASS, mxREAL);
	*((int32_t*)mxGetData(_out)) = (int32_t)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsCoreDataLink(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsCore core = *(HelicsCore*)(mxGetData(argv[0]));

	char *source;
	size_t sourceLength;
	int sourceStatus;
	sourceLength = mxGetN(argv[1]) + 1;
	source = (char *)malloc(sourceLength);
	sourceStatus = mxGetString(argv[1], source, sourceLength);

	char *target;
	size_t targetLength;
	int targetStatus;
	targetLength = mxGetN(argv[2]) + 1;
	target = (char *)malloc(targetLength);
	targetStatus = mxGetString(argv[2], target, targetLength);

	HelicsError err = helicsErrorInitialize();

	helicsCoreDataLink(core, (char const *)source, (char const *)target, &err);

	mxArray *_out = (mxArray *)0;

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
	HelicsCore core = *(HelicsCore*)(mxGetData(argv[0]));

	char *filter;
	size_t filterLength;
	int filterStatus;
	filterLength = mxGetN(argv[1]) + 1;
	filter = (char *)malloc(filterLength);
	filterStatus = mxGetString(argv[1], filter, filterLength);

	char *endpoint;
	size_t endpointLength;
	int endpointStatus;
	endpointLength = mxGetN(argv[2]) + 1;
	endpoint = (char *)malloc(endpointLength);
	endpointStatus = mxGetString(argv[2], endpoint, endpointLength);

	HelicsError err = helicsErrorInitialize();

	helicsCoreAddSourceFilterToEndpoint(core, (char const *)filter, (char const *)endpoint, &err);

	mxArray *_out = (mxArray *)0;

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
	HelicsCore core = *(HelicsCore*)(mxGetData(argv[0]));

	char *filter;
	size_t filterLength;
	int filterStatus;
	filterLength = mxGetN(argv[1]) + 1;
	filter = (char *)malloc(filterLength);
	filterStatus = mxGetString(argv[1], filter, filterLength);

	char *endpoint;
	size_t endpointLength;
	int endpointStatus;
	endpointLength = mxGetN(argv[2]) + 1;
	endpoint = (char *)malloc(endpointLength);
	endpointStatus = mxGetString(argv[2], endpoint, endpointLength);

	HelicsError err = helicsErrorInitialize();

	helicsCoreAddDestinationFilterToEndpoint(core, (char const *)filter, (char const *)endpoint, &err);

	mxArray *_out = (mxArray *)0;

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
	HelicsCore core = *(HelicsCore*)(mxGetData(argv[0]));

	char *file;
	size_t fileLength;
	int fileStatus;
	fileLength = mxGetN(argv[1]) + 1;
	file = (char *)malloc(fileLength);
	fileStatus = mxGetString(argv[1], file, fileLength);

	HelicsError err = helicsErrorInitialize();

	helicsCoreMakeConnections(core, (char const *)file, &err);

	mxArray *_out = (mxArray *)0;

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
	HelicsBroker broker = *(HelicsBroker*)(mxGetData(argv[0]));

	const char *result = helicsBrokerGetIdentifier(broker);

	mxArray *_out = mxCreateString(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsCoreGetIdentifier(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsCore core = *(HelicsCore*)(mxGetData(argv[0]));

	const char *result = helicsCoreGetIdentifier(core);

	mxArray *_out = mxCreateString(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsBrokerGetAddress(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsBroker broker = *(HelicsBroker*)(mxGetData(argv[0]));

	const char *result = helicsBrokerGetAddress(broker);

	mxArray *_out = mxCreateString(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsCoreGetAddress(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsCore core = *(HelicsCore*)(mxGetData(argv[0]));

	const char *result = helicsCoreGetAddress(core);

	mxArray *_out = mxCreateString(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsCoreSetReadyToInit(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsCore core = *(HelicsCore*)(mxGetData(argv[0]));

	HelicsError err = helicsErrorInitialize();

	helicsCoreSetReadyToInit(core, &err);

	mxArray *_out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsCoreConnect(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsCore core = *(HelicsCore*)(mxGetData(argv[0]));

	HelicsError err = helicsErrorInitialize();

	HelicsBool result = helicsCoreConnect(core, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT32_CLASS, mxREAL);
	*((int32_t*)mxGetData(_out)) = (int32_t)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsCoreDisconnect(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsCore core = *(HelicsCore*)(mxGetData(argv[0]));

	HelicsError err = helicsErrorInitialize();

	helicsCoreDisconnect(core, &err);

	mxArray *_out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsGetFederateByName(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	char *fedName;
	size_t fedNameLength;
	int fedNameStatus;
	fedNameLength = mxGetN(argv[0]) + 1;
	fedName = (char *)malloc(fedNameLength);
	fedNameStatus = mxGetString(argv[0], fedName, fedNameLength);

	HelicsError err = helicsErrorInitialize();

	HelicsFederate result = helicsGetFederateByName((char const *)fedName, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_t*)mxGetData(_out)) = (uint64_t)result;

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
	HelicsBroker broker = *(HelicsBroker*)(mxGetData(argv[0]));

	HelicsError err = helicsErrorInitialize();

	helicsBrokerDisconnect(broker, &err);

	mxArray *_out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateDestroy(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsFederate fed = *(HelicsFederate*)(mxGetData(argv[0]));

	helicsFederateDestroy(fed);

	mxArray *_out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsBrokerDestroy(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsBroker broker = *(HelicsBroker*)(mxGetData(argv[0]));

	helicsBrokerDestroy(broker);

	mxArray *_out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsCoreDestroy(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsCore core = *(HelicsCore*)(mxGetData(argv[0]));

	helicsCoreDestroy(core);

	mxArray *_out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsCoreFree(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsCore core = *(HelicsCore*)(mxGetData(argv[0]));

	helicsCoreFree(core);

	mxArray *_out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsBrokerFree(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsBroker broker = *(HelicsBroker*)(mxGetData(argv[0]));

	helicsBrokerFree(broker);

	mxArray *_out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsCreateValueFederate(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	char *fedName;
	size_t fedNameLength;
	int fedNameStatus;
	fedNameLength = mxGetN(argv[0]) + 1;
	fedName = (char *)malloc(fedNameLength);
	fedNameStatus = mxGetString(argv[0], fedName, fedNameLength);

	HelicsFederateInfo fi = *(HelicsFederateInfo*)(mxGetData(argv[1]));

	HelicsError err = helicsErrorInitialize();

	HelicsFederate result = helicsCreateValueFederate((char const *)fedName, fi, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_t*)mxGetData(_out)) = (uint64_t)result;

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
	char *configFile;
	size_t configFileLength;
	int configFileStatus;
	configFileLength = mxGetN(argv[0]) + 1;
	configFile = (char *)malloc(configFileLength);
	configFileStatus = mxGetString(argv[0], configFile, configFileLength);

	HelicsError err = helicsErrorInitialize();

	HelicsFederate result = helicsCreateValueFederateFromConfig((char const *)configFile, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_t*)mxGetData(_out)) = (uint64_t)result;

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
	char *fedName;
	size_t fedNameLength;
	int fedNameStatus;
	fedNameLength = mxGetN(argv[0]) + 1;
	fedName = (char *)malloc(fedNameLength);
	fedNameStatus = mxGetString(argv[0], fedName, fedNameLength);

	HelicsFederateInfo fi = *(HelicsFederateInfo*)(mxGetData(argv[1]));

	HelicsError err = helicsErrorInitialize();

	HelicsFederate result = helicsCreateMessageFederate((char const *)fedName, fi, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_t*)mxGetData(_out)) = (uint64_t)result;

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
	char *configFile;
	size_t configFileLength;
	int configFileStatus;
	configFileLength = mxGetN(argv[0]) + 1;
	configFile = (char *)malloc(configFileLength);
	configFileStatus = mxGetString(argv[0], configFile, configFileLength);

	HelicsError err = helicsErrorInitialize();

	HelicsFederate result = helicsCreateMessageFederateFromConfig((char const *)configFile, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_t*)mxGetData(_out)) = (uint64_t)result;

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
	char *fedName;
	size_t fedNameLength;
	int fedNameStatus;
	fedNameLength = mxGetN(argv[0]) + 1;
	fedName = (char *)malloc(fedNameLength);
	fedNameStatus = mxGetString(argv[0], fedName, fedNameLength);

	HelicsFederateInfo fi = *(HelicsFederateInfo*)(mxGetData(argv[1]));

	HelicsError err = helicsErrorInitialize();

	HelicsFederate result = helicsCreateCombinationFederate((char const *)fedName, fi, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_t*)mxGetData(_out)) = (uint64_t)result;

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
	char *configFile;
	size_t configFileLength;
	int configFileStatus;
	configFileLength = mxGetN(argv[0]) + 1;
	configFile = (char *)malloc(configFileLength);
	configFileStatus = mxGetString(argv[0], configFile, configFileLength);

	HelicsError err = helicsErrorInitialize();

	HelicsFederate result = helicsCreateCombinationFederateFromConfig((char const *)configFile, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_t*)mxGetData(_out)) = (uint64_t)result;

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
	HelicsFederate fed = *(HelicsFederate*)(mxGetData(argv[0]));

	HelicsError err = helicsErrorInitialize();

	HelicsFederate result = helicsFederateClone(fed, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_t*)mxGetData(_out)) = (uint64_t)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateProtect(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	char *fedName;
	size_t fedNameLength;
	int fedNameStatus;
	fedNameLength = mxGetN(argv[0]) + 1;
	fedName = (char *)malloc(fedNameLength);
	fedNameStatus = mxGetString(argv[0], fedName, fedNameLength);

	HelicsError err = helicsErrorInitialize();

	helicsFederateProtect((char const *)fedName, &err);

	mxArray *_out = (mxArray *)0;

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
	char *fedName;
	size_t fedNameLength;
	int fedNameStatus;
	fedNameLength = mxGetN(argv[0]) + 1;
	fedName = (char *)malloc(fedNameLength);
	fedNameStatus = mxGetString(argv[0], fedName, fedNameLength);

	HelicsError err = helicsErrorInitialize();

	helicsFederateUnProtect((char const *)fedName, &err);

	mxArray *_out = (mxArray *)0;

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
	char *fedName;
	size_t fedNameLength;
	int fedNameStatus;
	fedNameLength = mxGetN(argv[0]) + 1;
	fedName = (char *)malloc(fedNameLength);
	fedNameStatus = mxGetString(argv[0], fedName, fedNameLength);

	HelicsError err = helicsErrorInitialize();

	HelicsBool result = helicsFederateIsProtected((char const *)fedName, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT32_CLASS, mxREAL);
	*((int32_t*)mxGetData(_out)) = (int32_t)result;

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
	HelicsFederateInfo result = helicsCreateFederateInfo();

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_t*)mxGetData(_out)) = (uint64_t)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsFederateInfoClone(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsFederateInfo fi = *(HelicsFederateInfo*)(mxGetData(argv[0]));

	HelicsError err = helicsErrorInitialize();

	HelicsFederateInfo result = helicsFederateInfoClone(fi, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_t*)mxGetData(_out)) = (uint64_t)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateInfoLoadFromArgs(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsFederateInfo fi = *(HelicsFederateInfo*)(mxGetData(argv[0]));

	int arg1 = 0;
	char **arg2;
	int ii;
	arg1 = static_cast<int>(mxGetNumberOfElements(argv[1]));
	arg2 = (char **)malloc((arg1)*sizeof(char *));
	for (ii=0;ii<arg1;ii++){
		mxArray *cellElement=mxGetCell(argv[1], ii);
		size_t len = mxGetN(cellElement) + 1;
		arg2[ii] = (char *)malloc(static_cast<int>(len));
		int flag = mxGetString(cellElement, arg2[ii], static_cast<int>(len));
	}

	HelicsError err = helicsErrorInitialize();

	helicsFederateInfoLoadFromArgs(fi, arg1, arg2, &err);

	mxArray *_out = (mxArray *)0;
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
	HelicsFederateInfo fi = *(HelicsFederateInfo*)(mxGetData(argv[0]));

	char *args;
	size_t argsLength;
	int argsStatus;
	argsLength = mxGetN(argv[1]) + 1;
	args = (char *)malloc(argsLength);
	argsStatus = mxGetString(argv[1], args, argsLength);

	HelicsError err = helicsErrorInitialize();

	helicsFederateInfoLoadFromString(fi, (char const *)args, &err);

	mxArray *_out = (mxArray *)0;

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
	HelicsFederateInfo fi = *(HelicsFederateInfo*)(mxGetData(argv[0]));

	helicsFederateInfoFree(fi);

	mxArray *_out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsFederateIsValid(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsFederate fed = *(HelicsFederate*)(mxGetData(argv[0]));

	HelicsBool result = helicsFederateIsValid(fed);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT32_CLASS, mxREAL);
	*((int32_t*)mxGetData(_out)) = (int32_t)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsFederateInfoSetCoreName(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsFederateInfo fi = *(HelicsFederateInfo*)(mxGetData(argv[0]));

	char *corename;
	size_t corenameLength;
	int corenameStatus;
	corenameLength = mxGetN(argv[1]) + 1;
	corename = (char *)malloc(corenameLength);
	corenameStatus = mxGetString(argv[1], corename, corenameLength);

	HelicsError err = helicsErrorInitialize();

	helicsFederateInfoSetCoreName(fi, (char const *)corename, &err);

	mxArray *_out = (mxArray *)0;

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
	HelicsFederateInfo fi = *(HelicsFederateInfo*)(mxGetData(argv[0]));

	char *coreInit;
	size_t coreInitLength;
	int coreInitStatus;
	coreInitLength = mxGetN(argv[1]) + 1;
	coreInit = (char *)malloc(coreInitLength);
	coreInitStatus = mxGetString(argv[1], coreInit, coreInitLength);

	HelicsError err = helicsErrorInitialize();

	helicsFederateInfoSetCoreInitString(fi, (char const *)coreInit, &err);

	mxArray *_out = (mxArray *)0;

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
	HelicsFederateInfo fi = *(HelicsFederateInfo*)(mxGetData(argv[0]));

	char *brokerInit;
	size_t brokerInitLength;
	int brokerInitStatus;
	brokerInitLength = mxGetN(argv[1]) + 1;
	brokerInit = (char *)malloc(brokerInitLength);
	brokerInitStatus = mxGetString(argv[1], brokerInit, brokerInitLength);

	HelicsError err = helicsErrorInitialize();

	helicsFederateInfoSetBrokerInitString(fi, (char const *)brokerInit, &err);

	mxArray *_out = (mxArray *)0;

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
	HelicsFederateInfo fi = *(HelicsFederateInfo*)(mxGetData(argv[0]));

	int coretype = (int)(mxGetScalar(argv[1]));

	HelicsError err = helicsErrorInitialize();

	helicsFederateInfoSetCoreType(fi, coretype, &err);

	mxArray *_out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}





	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateInfoSetCoreTypeFromString(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsFederateInfo fi = *(HelicsFederateInfo*)(mxGetData(argv[0]));

	char *coretype;
	size_t coretypeLength;
	int coretypeStatus;
	coretypeLength = mxGetN(argv[1]) + 1;
	coretype = (char *)malloc(coretypeLength);
	coretypeStatus = mxGetString(argv[1], coretype, coretypeLength);

	HelicsError err = helicsErrorInitialize();

	helicsFederateInfoSetCoreTypeFromString(fi, (char const *)coretype, &err);

	mxArray *_out = (mxArray *)0;

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
	HelicsFederateInfo fi = *(HelicsFederateInfo*)(mxGetData(argv[0]));

	char *broker;
	size_t brokerLength;
	int brokerStatus;
	brokerLength = mxGetN(argv[1]) + 1;
	broker = (char *)malloc(brokerLength);
	brokerStatus = mxGetString(argv[1], broker, brokerLength);

	HelicsError err = helicsErrorInitialize();

	helicsFederateInfoSetBroker(fi, (char const *)broker, &err);

	mxArray *_out = (mxArray *)0;

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
	HelicsFederateInfo fi = *(HelicsFederateInfo*)(mxGetData(argv[0]));

	char *brokerkey;
	size_t brokerkeyLength;
	int brokerkeyStatus;
	brokerkeyLength = mxGetN(argv[1]) + 1;
	brokerkey = (char *)malloc(brokerkeyLength);
	brokerkeyStatus = mxGetString(argv[1], brokerkey, brokerkeyLength);

	HelicsError err = helicsErrorInitialize();

	helicsFederateInfoSetBrokerKey(fi, (char const *)brokerkey, &err);

	mxArray *_out = (mxArray *)0;

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
	HelicsFederateInfo fi = *(HelicsFederateInfo*)(mxGetData(argv[0]));

	int brokerPort = (int)(mxGetScalar(argv[1]));

	HelicsError err = helicsErrorInitialize();

	helicsFederateInfoSetBrokerPort(fi, brokerPort, &err);

	mxArray *_out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}





	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateInfoSetLocalPort(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsFederateInfo fi = *(HelicsFederateInfo*)(mxGetData(argv[0]));

	char *localPort;
	size_t localPortLength;
	int localPortStatus;
	localPortLength = mxGetN(argv[1]) + 1;
	localPort = (char *)malloc(localPortLength);
	localPortStatus = mxGetString(argv[1], localPort, localPortLength);

	HelicsError err = helicsErrorInitialize();

	helicsFederateInfoSetLocalPort(fi, (char const *)localPort, &err);

	mxArray *_out = (mxArray *)0;

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
	char *val;
	size_t valLength;
	int valStatus;
	valLength = mxGetN(argv[0]) + 1;
	val = (char *)malloc(valLength);
	valStatus = mxGetString(argv[0], val, valLength);

	int result = helicsGetPropertyIndex((char const *)val);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
	*((int64_t*)mxGetData(_out)) = (int64_t)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(val);
}


void _wrap_helicsGetFlagIndex(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	char *val;
	size_t valLength;
	int valStatus;
	valLength = mxGetN(argv[0]) + 1;
	val = (char *)malloc(valLength);
	valStatus = mxGetString(argv[0], val, valLength);

	int result = helicsGetFlagIndex((char const *)val);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
	*((int64_t*)mxGetData(_out)) = (int64_t)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(val);
}


void _wrap_helicsGetOptionIndex(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	char *val;
	size_t valLength;
	int valStatus;
	valLength = mxGetN(argv[0]) + 1;
	val = (char *)malloc(valLength);
	valStatus = mxGetString(argv[0], val, valLength);

	int result = helicsGetOptionIndex((char const *)val);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
	*((int64_t*)mxGetData(_out)) = (int64_t)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(val);
}


void _wrap_helicsGetOptionValue(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	char *val;
	size_t valLength;
	int valStatus;
	valLength = mxGetN(argv[0]) + 1;
	val = (char *)malloc(valLength);
	valStatus = mxGetString(argv[0], val, valLength);

	int result = helicsGetOptionValue((char const *)val);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
	*((int64_t*)mxGetData(_out)) = (int64_t)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(val);
}


void _wrap_helicsGetDataType(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	char *val;
	size_t valLength;
	int valStatus;
	valLength = mxGetN(argv[0]) + 1;
	val = (char *)malloc(valLength);
	valStatus = mxGetString(argv[0], val, valLength);

	int result = helicsGetDataType((char const *)val);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
	*((int64_t*)mxGetData(_out)) = (int64_t)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(val);
}


void _wrap_helicsFederateInfoSetFlagOption(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsFederateInfo fi = *(HelicsFederateInfo*)(mxGetData(argv[0]));

	int flag = (int)(mxGetScalar(argv[1]));

	HelicsBool value = (HelicsBool)(mxGetScalar(argv[2]));

	HelicsError err = helicsErrorInitialize();

	helicsFederateInfoSetFlagOption(fi, flag, value, &err);

	mxArray *_out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}







	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateInfoSetSeparator(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsFederateInfo fi = *(HelicsFederateInfo*)(mxGetData(argv[0]));

	char *separator;
	size_t separatorLength;
	int separatorStatus;
	separatorLength = mxGetN(argv[1]) + 1;
	separator = (char *)malloc(separatorLength);
	separatorStatus = mxGetString(argv[1], separator, separatorLength);

	HelicsError err = helicsErrorInitialize();

	helicsFederateInfoSetSeparator(fi, separator[0], &err);

	mxArray *_out = (mxArray *)0;

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
	HelicsFederateInfo fi = *(HelicsFederateInfo*)(mxGetData(argv[0]));

	int timeProperty = (int)(mxGetScalar(argv[1]));

	HelicsTime propertyValue = (HelicsTime)(mxGetScalar(argv[2]));

	HelicsError err = helicsErrorInitialize();

	helicsFederateInfoSetTimeProperty(fi, timeProperty, propertyValue, &err);

	mxArray *_out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}







	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateInfoSetIntegerProperty(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsFederateInfo fi = *(HelicsFederateInfo*)(mxGetData(argv[0]));

	int intProperty = (int)(mxGetScalar(argv[1]));

	int propertyValue = (int)(mxGetScalar(argv[2]));

	HelicsError err = helicsErrorInitialize();

	helicsFederateInfoSetIntegerProperty(fi, intProperty, propertyValue, &err);

	mxArray *_out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}







	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateRegisterInterfaces(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsFederate fed = *(HelicsFederate*)(mxGetData(argv[0]));

	char *file;
	size_t fileLength;
	int fileStatus;
	fileLength = mxGetN(argv[1]) + 1;
	file = (char *)malloc(fileLength);
	fileStatus = mxGetString(argv[1], file, fileLength);

	HelicsError err = helicsErrorInitialize();

	helicsFederateRegisterInterfaces(fed, (char const *)file, &err);

	mxArray *_out = (mxArray *)0;

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
	HelicsFederate fed = *(HelicsFederate*)(mxGetData(argv[0]));

	int errorCode = (int)(mxGetScalar(argv[1]));

	char *errorString;
	size_t errorStringLength;
	int errorStringStatus;
	errorStringLength = mxGetN(argv[2]) + 1;
	errorString = (char *)malloc(errorStringLength);
	errorStringStatus = mxGetString(argv[2], errorString, errorStringLength);

	HelicsError err = helicsErrorInitialize();

	helicsFederateGlobalError(fed, errorCode, (char const *)errorString, &err);

	mxArray *_out = (mxArray *)0;

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
	HelicsFederate fed = *(HelicsFederate*)(mxGetData(argv[0]));

	int errorCode = (int)(mxGetScalar(argv[1]));

	char *errorString;
	size_t errorStringLength;
	int errorStringStatus;
	errorStringLength = mxGetN(argv[2]) + 1;
	errorString = (char *)malloc(errorStringLength);
	errorStringStatus = mxGetString(argv[2], errorString, errorStringLength);

	HelicsError err = helicsErrorInitialize();

	helicsFederateLocalError(fed, errorCode, (char const *)errorString, &err);

	mxArray *_out = (mxArray *)0;

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
	HelicsFederate fed = *(HelicsFederate*)(mxGetData(argv[0]));

	HelicsError err = helicsErrorInitialize();

	helicsFederateFinalize(fed, &err);

	mxArray *_out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateFinalizeAsync(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsFederate fed = *(HelicsFederate*)(mxGetData(argv[0]));

	HelicsError err = helicsErrorInitialize();

	helicsFederateFinalizeAsync(fed, &err);

	mxArray *_out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateFinalizeComplete(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsFederate fed = *(HelicsFederate*)(mxGetData(argv[0]));

	HelicsError err = helicsErrorInitialize();

	helicsFederateFinalizeComplete(fed, &err);

	mxArray *_out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateDisconnect(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsFederate fed = *(HelicsFederate*)(mxGetData(argv[0]));

	HelicsError err = helicsErrorInitialize();

	helicsFederateDisconnect(fed, &err);

	mxArray *_out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateDisconnectAsync(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsFederate fed = *(HelicsFederate*)(mxGetData(argv[0]));

	HelicsError err = helicsErrorInitialize();

	helicsFederateDisconnectAsync(fed, &err);

	mxArray *_out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateDisconnectComplete(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsFederate fed = *(HelicsFederate*)(mxGetData(argv[0]));

	HelicsError err = helicsErrorInitialize();

	helicsFederateDisconnectComplete(fed, &err);

	mxArray *_out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateFree(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsFederate fed = *(HelicsFederate*)(mxGetData(argv[0]));

	helicsFederateFree(fed);

	mxArray *_out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsCloseLibrary(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	helicsCloseLibrary();

	mxArray *_out = (mxArray *)0;
	if(_out){
		--resc;
		*resv++ = _out;
	}
	mexUnlock();
}


void _wrap_helicsFederateEnterInitializingMode(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsFederate fed = *(HelicsFederate*)(mxGetData(argv[0]));

	HelicsError err = helicsErrorInitialize();

	helicsFederateEnterInitializingMode(fed, &err);

	mxArray *_out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateEnterInitializingModeAsync(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsFederate fed = *(HelicsFederate*)(mxGetData(argv[0]));

	HelicsError err = helicsErrorInitialize();

	helicsFederateEnterInitializingModeAsync(fed, &err);

	mxArray *_out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateIsAsyncOperationCompleted(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsFederate fed = *(HelicsFederate*)(mxGetData(argv[0]));

	HelicsError err = helicsErrorInitialize();

	HelicsBool result = helicsFederateIsAsyncOperationCompleted(fed, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT32_CLASS, mxREAL);
	*((int32_t*)mxGetData(_out)) = (int32_t)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateEnterInitializingModeComplete(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsFederate fed = *(HelicsFederate*)(mxGetData(argv[0]));

	HelicsError err = helicsErrorInitialize();

	helicsFederateEnterInitializingModeComplete(fed, &err);

	mxArray *_out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateEnterExecutingMode(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsFederate fed = *(HelicsFederate*)(mxGetData(argv[0]));

	HelicsError err = helicsErrorInitialize();

	helicsFederateEnterExecutingMode(fed, &err);

	mxArray *_out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateEnterExecutingModeAsync(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsFederate fed = *(HelicsFederate*)(mxGetData(argv[0]));

	HelicsError err = helicsErrorInitialize();

	helicsFederateEnterExecutingModeAsync(fed, &err);

	mxArray *_out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateEnterExecutingModeComplete(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsFederate fed = *(HelicsFederate*)(mxGetData(argv[0]));

	HelicsError err = helicsErrorInitialize();

	helicsFederateEnterExecutingModeComplete(fed, &err);

	mxArray *_out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateEnterExecutingModeIterative(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsFederate fed = *(HelicsFederate*)(mxGetData(argv[0]));

	int iterateInt = static_cast<int>(mxGetScalar(argv[1]));
	HelicsIterationRequest iterate = static_cast<HelicsIterationRequest>(iterateInt);

	HelicsError err = helicsErrorInitialize();

	HelicsIterationResult result = helicsFederateEnterExecutingModeIterative(fed, iterate, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT32_CLASS, mxREAL);
	*((int32_t*)mxGetData(_out)) = (int32_t)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}





	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateEnterExecutingModeIterativeAsync(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsFederate fed = *(HelicsFederate*)(mxGetData(argv[0]));

	int iterateInt = static_cast<int>(mxGetScalar(argv[1]));
	HelicsIterationRequest iterate = static_cast<HelicsIterationRequest>(iterateInt);

	HelicsError err = helicsErrorInitialize();

	helicsFederateEnterExecutingModeIterativeAsync(fed, iterate, &err);

	mxArray *_out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}





	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateEnterExecutingModeIterativeComplete(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsFederate fed = *(HelicsFederate*)(mxGetData(argv[0]));

	HelicsError err = helicsErrorInitialize();

	HelicsIterationResult result = helicsFederateEnterExecutingModeIterativeComplete(fed, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT32_CLASS, mxREAL);
	*((int32_t*)mxGetData(_out)) = (int32_t)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateGetState(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsFederate fed = *(HelicsFederate*)(mxGetData(argv[0]));

	HelicsError err = helicsErrorInitialize();

	HelicsFederateState result = helicsFederateGetState(fed, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT32_CLASS, mxREAL);
	*((int32_t*)mxGetData(_out)) = (int32_t)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateGetCore(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsFederate fed = *(HelicsFederate*)(mxGetData(argv[0]));

	HelicsError err = helicsErrorInitialize();

	HelicsCore result = helicsFederateGetCore(fed, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_t*)mxGetData(_out)) = (uint64_t)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateRequestTime(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsFederate fed = *(HelicsFederate*)(mxGetData(argv[0]));

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
	HelicsFederate fed = *(HelicsFederate*)(mxGetData(argv[0]));

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
	HelicsFederate fed = *(HelicsFederate*)(mxGetData(argv[0]));

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
	HelicsFederate fed = *(HelicsFederate*)(mxGetData(argv[0]));

	HelicsTime requestTime = (HelicsTime)(mxGetScalar(argv[1]));

	int iterateInt = static_cast<int>(mxGetScalar(argv[2]));
	HelicsIterationRequest iterate = static_cast<HelicsIterationRequest>(iterateInt);

	HelicsIterationResult outIteration = HELICS_ITERATION_RESULT_ERROR;

	HelicsError err = helicsErrorInitialize();

	HelicsTime result = helicsFederateRequestTimeIterative(fed, requestTime, iterate, &outIteration, &err);

	mxArray *_out = mxCreateDoubleScalar(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(--resc>=0){
		mxArray *_out1 = mxCreateNumericMatrix(1,1,mxINT32_CLASS,mxREAL);
		*((int32_t*)mxGetData(_out1)) = (int32_t)outIteration;
		*resv++ = _out1;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateRequestTimeAsync(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsFederate fed = *(HelicsFederate*)(mxGetData(argv[0]));

	HelicsTime requestTime = (HelicsTime)(mxGetScalar(argv[1]));

	HelicsError err = helicsErrorInitialize();

	helicsFederateRequestTimeAsync(fed, requestTime, &err);

	mxArray *_out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}





	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateRequestTimeComplete(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsFederate fed = *(HelicsFederate*)(mxGetData(argv[0]));

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
	HelicsFederate fed = *(HelicsFederate*)(mxGetData(argv[0]));

	HelicsTime requestTime = (HelicsTime)(mxGetScalar(argv[1]));

	int iterateInt = static_cast<int>(mxGetScalar(argv[2]));
	HelicsIterationRequest iterate = static_cast<HelicsIterationRequest>(iterateInt);

	HelicsError err = helicsErrorInitialize();

	helicsFederateRequestTimeIterativeAsync(fed, requestTime, iterate, &err);

	mxArray *_out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}







	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateRequestTimeIterativeComplete(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsFederate fed = *(HelicsFederate*)(mxGetData(argv[0]));

	HelicsIterationResult outIteration = HELICS_ITERATION_RESULT_ERROR;

	HelicsError err = helicsErrorInitialize();

	HelicsTime result = helicsFederateRequestTimeIterativeComplete(fed, &outIteration, &err);

	mxArray *_out = mxCreateDoubleScalar(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(--resc>=0){
		mxArray *_out1 = mxCreateNumericMatrix(1,1,mxINT32_CLASS,mxREAL);
		*((int32_t*)mxGetData(_out1)) = (int32_t)outIteration;
		*resv++ = _out1;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateProcessCommunications(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsFederate fed = *(HelicsFederate*)(mxGetData(argv[0]));

	HelicsTime period = (HelicsTime)(mxGetScalar(argv[1]));

	HelicsError err = helicsErrorInitialize();

	helicsFederateProcessCommunications(fed, period, &err);

	mxArray *_out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}





	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateGetName(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsFederate fed = *(HelicsFederate*)(mxGetData(argv[0]));

	const char *result = helicsFederateGetName(fed);

	mxArray *_out = mxCreateString(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsFederateSetTimeProperty(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsFederate fed = *(HelicsFederate*)(mxGetData(argv[0]));

	int timeProperty = (int)(mxGetScalar(argv[1]));

	HelicsTime time = (HelicsTime)(mxGetScalar(argv[2]));

	HelicsError err = helicsErrorInitialize();

	helicsFederateSetTimeProperty(fed, timeProperty, time, &err);

	mxArray *_out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}







	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateSetFlagOption(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsFederate fed = *(HelicsFederate*)(mxGetData(argv[0]));

	int flag = (int)(mxGetScalar(argv[1]));

	HelicsBool flagValue = (HelicsBool)(mxGetScalar(argv[2]));

	HelicsError err = helicsErrorInitialize();

	helicsFederateSetFlagOption(fed, flag, flagValue, &err);

	mxArray *_out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}







	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateSetSeparator(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsFederate fed = *(HelicsFederate*)(mxGetData(argv[0]));

	char *separator;
	size_t separatorLength;
	int separatorStatus;
	separatorLength = mxGetN(argv[1]) + 1;
	separator = (char *)malloc(separatorLength);
	separatorStatus = mxGetString(argv[1], separator, separatorLength);

	HelicsError err = helicsErrorInitialize();

	helicsFederateSetSeparator(fed, separator[0], &err);

	mxArray *_out = (mxArray *)0;

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
	HelicsFederate fed = *(HelicsFederate*)(mxGetData(argv[0]));

	int intProperty = (int)(mxGetScalar(argv[1]));

	int propertyVal = (int)(mxGetScalar(argv[2]));

	HelicsError err = helicsErrorInitialize();

	helicsFederateSetIntegerProperty(fed, intProperty, propertyVal, &err);

	mxArray *_out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}







	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateGetTimeProperty(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsFederate fed = *(HelicsFederate*)(mxGetData(argv[0]));

	int timeProperty = (int)(mxGetScalar(argv[1]));

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
	HelicsFederate fed = *(HelicsFederate*)(mxGetData(argv[0]));

	int flag = (int)(mxGetScalar(argv[1]));

	HelicsError err = helicsErrorInitialize();

	HelicsBool result = helicsFederateGetFlagOption(fed, flag, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT32_CLASS, mxREAL);
	*((int32_t*)mxGetData(_out)) = (int32_t)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}





	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateGetIntegerProperty(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsFederate fed = *(HelicsFederate*)(mxGetData(argv[0]));

	int intProperty = (int)(mxGetScalar(argv[1]));

	HelicsError err = helicsErrorInitialize();

	int result = helicsFederateGetIntegerProperty(fed, intProperty, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
	*((int64_t*)mxGetData(_out)) = (int64_t)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}





	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateGetCurrentTime(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsFederate fed = *(HelicsFederate*)(mxGetData(argv[0]));

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
	HelicsFederate fed = *(HelicsFederate*)(mxGetData(argv[0]));

	char *valueName;
	size_t valueNameLength;
	int valueNameStatus;
	valueNameLength = mxGetN(argv[1]) + 1;
	valueName = (char *)malloc(valueNameLength);
	valueNameStatus = mxGetString(argv[1], valueName, valueNameLength);

	char *value;
	size_t valueLength;
	int valueStatus;
	valueLength = mxGetN(argv[2]) + 1;
	value = (char *)malloc(valueLength);
	valueStatus = mxGetString(argv[2], value, valueLength);

	HelicsError err = helicsErrorInitialize();

	helicsFederateSetGlobal(fed, (char const *)valueName, (char const *)value, &err);

	mxArray *_out = (mxArray *)0;

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
	HelicsFederate fed = *(HelicsFederate*)(mxGetData(argv[0]));

	char *tagName;
	size_t tagNameLength;
	int tagNameStatus;
	tagNameLength = mxGetN(argv[1]) + 1;
	tagName = (char *)malloc(tagNameLength);
	tagNameStatus = mxGetString(argv[1], tagName, tagNameLength);

	char *value;
	size_t valueLength;
	int valueStatus;
	valueLength = mxGetN(argv[2]) + 1;
	value = (char *)malloc(valueLength);
	valueStatus = mxGetString(argv[2], value, valueLength);

	HelicsError err = helicsErrorInitialize();

	helicsFederateSetTag(fed, (char const *)tagName, (char const *)value, &err);

	mxArray *_out = (mxArray *)0;

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
	HelicsFederate fed = *(HelicsFederate*)(mxGetData(argv[0]));

	char *tagName;
	size_t tagNameLength;
	int tagNameStatus;
	tagNameLength = mxGetN(argv[1]) + 1;
	tagName = (char *)malloc(tagNameLength);
	tagNameStatus = mxGetString(argv[1], tagName, tagNameLength);

	HelicsError err = helicsErrorInitialize();

	const char *result = helicsFederateGetTag(fed, (char const *)tagName, &err);

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
	HelicsFederate fed = *(HelicsFederate*)(mxGetData(argv[0]));

	char *fedName;
	size_t fedNameLength;
	int fedNameStatus;
	fedNameLength = mxGetN(argv[1]) + 1;
	fedName = (char *)malloc(fedNameLength);
	fedNameStatus = mxGetString(argv[1], fedName, fedNameLength);

	HelicsError err = helicsErrorInitialize();

	helicsFederateAddDependency(fed, (char const *)fedName, &err);

	mxArray *_out = (mxArray *)0;

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
	HelicsFederate fed = *(HelicsFederate*)(mxGetData(argv[0]));

	char *logFile;
	size_t logFileLength;
	int logFileStatus;
	logFileLength = mxGetN(argv[1]) + 1;
	logFile = (char *)malloc(logFileLength);
	logFileStatus = mxGetString(argv[1], logFile, logFileLength);

	HelicsError err = helicsErrorInitialize();

	helicsFederateSetLogFile(fed, (char const *)logFile, &err);

	mxArray *_out = (mxArray *)0;

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
	HelicsFederate fed = *(HelicsFederate*)(mxGetData(argv[0]));

	char *logmessage;
	size_t logmessageLength;
	int logmessageStatus;
	logmessageLength = mxGetN(argv[1]) + 1;
	logmessage = (char *)malloc(logmessageLength);
	logmessageStatus = mxGetString(argv[1], logmessage, logmessageLength);

	HelicsError err = helicsErrorInitialize();

	helicsFederateLogErrorMessage(fed, (char const *)logmessage, &err);

	mxArray *_out = (mxArray *)0;

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
	HelicsFederate fed = *(HelicsFederate*)(mxGetData(argv[0]));

	char *logmessage;
	size_t logmessageLength;
	int logmessageStatus;
	logmessageLength = mxGetN(argv[1]) + 1;
	logmessage = (char *)malloc(logmessageLength);
	logmessageStatus = mxGetString(argv[1], logmessage, logmessageLength);

	HelicsError err = helicsErrorInitialize();

	helicsFederateLogWarningMessage(fed, (char const *)logmessage, &err);

	mxArray *_out = (mxArray *)0;

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
	HelicsFederate fed = *(HelicsFederate*)(mxGetData(argv[0]));

	char *logmessage;
	size_t logmessageLength;
	int logmessageStatus;
	logmessageLength = mxGetN(argv[1]) + 1;
	logmessage = (char *)malloc(logmessageLength);
	logmessageStatus = mxGetString(argv[1], logmessage, logmessageLength);

	HelicsError err = helicsErrorInitialize();

	helicsFederateLogInfoMessage(fed, (char const *)logmessage, &err);

	mxArray *_out = (mxArray *)0;

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
	HelicsFederate fed = *(HelicsFederate*)(mxGetData(argv[0]));

	char *logmessage;
	size_t logmessageLength;
	int logmessageStatus;
	logmessageLength = mxGetN(argv[1]) + 1;
	logmessage = (char *)malloc(logmessageLength);
	logmessageStatus = mxGetString(argv[1], logmessage, logmessageLength);

	HelicsError err = helicsErrorInitialize();

	helicsFederateLogDebugMessage(fed, (char const *)logmessage, &err);

	mxArray *_out = (mxArray *)0;

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
	HelicsFederate fed = *(HelicsFederate*)(mxGetData(argv[0]));

	int loglevel = (int)(mxGetScalar(argv[1]));

	char *logmessage;
	size_t logmessageLength;
	int logmessageStatus;
	logmessageLength = mxGetN(argv[2]) + 1;
	logmessage = (char *)malloc(logmessageLength);
	logmessageStatus = mxGetString(argv[2], logmessage, logmessageLength);

	HelicsError err = helicsErrorInitialize();

	helicsFederateLogLevelMessage(fed, loglevel, (char const *)logmessage, &err);

	mxArray *_out = (mxArray *)0;

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
	HelicsFederate fed = *(HelicsFederate*)(mxGetData(argv[0]));

	char *target;
	size_t targetLength;
	int targetStatus;
	targetLength = mxGetN(argv[1]) + 1;
	target = (char *)malloc(targetLength);
	targetStatus = mxGetString(argv[1], target, targetLength);

	char *command;
	size_t commandLength;
	int commandStatus;
	commandLength = mxGetN(argv[2]) + 1;
	command = (char *)malloc(commandLength);
	commandStatus = mxGetString(argv[2], command, commandLength);

	HelicsError err = helicsErrorInitialize();

	helicsFederateSendCommand(fed, (char const *)target, (char const *)command, &err);

	mxArray *_out = (mxArray *)0;

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
	HelicsFederate fed = *(HelicsFederate*)(mxGetData(argv[0]));

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
	HelicsFederate fed = *(HelicsFederate*)(mxGetData(argv[0]));

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
	HelicsFederate fed = *(HelicsFederate*)(mxGetData(argv[0]));

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
	HelicsCore core = *(HelicsCore*)(mxGetData(argv[0]));

	char *valueName;
	size_t valueNameLength;
	int valueNameStatus;
	valueNameLength = mxGetN(argv[1]) + 1;
	valueName = (char *)malloc(valueNameLength);
	valueNameStatus = mxGetString(argv[1], valueName, valueNameLength);

	char *value;
	size_t valueLength;
	int valueStatus;
	valueLength = mxGetN(argv[2]) + 1;
	value = (char *)malloc(valueLength);
	valueStatus = mxGetString(argv[2], value, valueLength);

	HelicsError err = helicsErrorInitialize();

	helicsCoreSetGlobal(core, (char const *)valueName, (char const *)value, &err);

	mxArray *_out = (mxArray *)0;

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
	HelicsBroker broker = *(HelicsBroker*)(mxGetData(argv[0]));

	char *valueName;
	size_t valueNameLength;
	int valueNameStatus;
	valueNameLength = mxGetN(argv[1]) + 1;
	valueName = (char *)malloc(valueNameLength);
	valueNameStatus = mxGetString(argv[1], valueName, valueNameLength);

	char *value;
	size_t valueLength;
	int valueStatus;
	valueLength = mxGetN(argv[2]) + 1;
	value = (char *)malloc(valueLength);
	valueStatus = mxGetString(argv[2], value, valueLength);

	HelicsError err = helicsErrorInitialize();

	helicsBrokerSetGlobal(broker, (char const *)valueName, (char const *)value, &err);

	mxArray *_out = (mxArray *)0;

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
	HelicsCore core = *(HelicsCore*)(mxGetData(argv[0]));

	char *target;
	size_t targetLength;
	int targetStatus;
	targetLength = mxGetN(argv[1]) + 1;
	target = (char *)malloc(targetLength);
	targetStatus = mxGetString(argv[1], target, targetLength);

	char *command;
	size_t commandLength;
	int commandStatus;
	commandLength = mxGetN(argv[2]) + 1;
	command = (char *)malloc(commandLength);
	commandStatus = mxGetString(argv[2], command, commandLength);

	HelicsError err = helicsErrorInitialize();

	helicsCoreSendCommand(core, (char const *)target, (char const *)command, &err);

	mxArray *_out = (mxArray *)0;

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
	HelicsCore core = *(HelicsCore*)(mxGetData(argv[0]));

	char *target;
	size_t targetLength;
	int targetStatus;
	targetLength = mxGetN(argv[1]) + 1;
	target = (char *)malloc(targetLength);
	targetStatus = mxGetString(argv[1], target, targetLength);

	char *command;
	size_t commandLength;
	int commandStatus;
	commandLength = mxGetN(argv[2]) + 1;
	command = (char *)malloc(commandLength);
	commandStatus = mxGetString(argv[2], command, commandLength);

	HelicsError err = helicsErrorInitialize();

	helicsCoreSendOrderedCommand(core, (char const *)target, (char const *)command, &err);

	mxArray *_out = (mxArray *)0;

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
	HelicsBroker broker = *(HelicsBroker*)(mxGetData(argv[0]));

	char *target;
	size_t targetLength;
	int targetStatus;
	targetLength = mxGetN(argv[1]) + 1;
	target = (char *)malloc(targetLength);
	targetStatus = mxGetString(argv[1], target, targetLength);

	char *command;
	size_t commandLength;
	int commandStatus;
	commandLength = mxGetN(argv[2]) + 1;
	command = (char *)malloc(commandLength);
	commandStatus = mxGetString(argv[2], command, commandLength);

	HelicsError err = helicsErrorInitialize();

	helicsBrokerSendCommand(broker, (char const *)target, (char const *)command, &err);

	mxArray *_out = (mxArray *)0;

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
	HelicsBroker broker = *(HelicsBroker*)(mxGetData(argv[0]));

	char *target;
	size_t targetLength;
	int targetStatus;
	targetLength = mxGetN(argv[1]) + 1;
	target = (char *)malloc(targetLength);
	targetStatus = mxGetString(argv[1], target, targetLength);

	char *command;
	size_t commandLength;
	int commandStatus;
	commandLength = mxGetN(argv[2]) + 1;
	command = (char *)malloc(commandLength);
	commandStatus = mxGetString(argv[2], command, commandLength);

	HelicsError err = helicsErrorInitialize();

	helicsBrokerSendOrderedCommand(broker, (char const *)target, (char const *)command, &err);

	mxArray *_out = (mxArray *)0;

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
	HelicsCore core = *(HelicsCore*)(mxGetData(argv[0]));

	char *logFileName;
	size_t logFileNameLength;
	int logFileNameStatus;
	logFileNameLength = mxGetN(argv[1]) + 1;
	logFileName = (char *)malloc(logFileNameLength);
	logFileNameStatus = mxGetString(argv[1], logFileName, logFileNameLength);

	HelicsError err = helicsErrorInitialize();

	helicsCoreSetLogFile(core, (char const *)logFileName, &err);

	mxArray *_out = (mxArray *)0;

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
	HelicsBroker broker = *(HelicsBroker*)(mxGetData(argv[0]));

	char *logFileName;
	size_t logFileNameLength;
	int logFileNameStatus;
	logFileNameLength = mxGetN(argv[1]) + 1;
	logFileName = (char *)malloc(logFileNameLength);
	logFileNameStatus = mxGetString(argv[1], logFileName, logFileNameLength);

	HelicsError err = helicsErrorInitialize();

	helicsBrokerSetLogFile(broker, (char const *)logFileName, &err);

	mxArray *_out = (mxArray *)0;

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
	HelicsBroker broker = *(HelicsBroker*)(mxGetData(argv[0]));

	HelicsTime barrierTime = (HelicsTime)(mxGetScalar(argv[1]));

	HelicsError err = helicsErrorInitialize();

	helicsBrokerSetTimeBarrier(broker, barrierTime, &err);

	mxArray *_out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}





	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsBrokerClearTimeBarrier(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsBroker broker = *(HelicsBroker*)(mxGetData(argv[0]));

	helicsBrokerClearTimeBarrier(broker);

	mxArray *_out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsBrokerGlobalError(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsBroker broker = *(HelicsBroker*)(mxGetData(argv[0]));

	int errorCode = (int)(mxGetScalar(argv[1]));

	char *errorString;
	size_t errorStringLength;
	int errorStringStatus;
	errorStringLength = mxGetN(argv[2]) + 1;
	errorString = (char *)malloc(errorStringLength);
	errorStringStatus = mxGetString(argv[2], errorString, errorStringLength);

	HelicsError err = helicsErrorInitialize();

	helicsBrokerGlobalError(broker, errorCode, (char const *)errorString, &err);

	mxArray *_out = (mxArray *)0;

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
	HelicsCore core = *(HelicsCore*)(mxGetData(argv[0]));

	int errorCode = (int)(mxGetScalar(argv[1]));

	char *errorString;
	size_t errorStringLength;
	int errorStringStatus;
	errorStringLength = mxGetN(argv[2]) + 1;
	errorString = (char *)malloc(errorStringLength);
	errorStringStatus = mxGetString(argv[2], errorString, errorStringLength);

	HelicsError err = helicsErrorInitialize();

	helicsCoreGlobalError(core, errorCode, (char const *)errorString, &err);

	mxArray *_out = (mxArray *)0;

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
	char *target;
	size_t targetLength;
	int targetStatus;
	targetLength = mxGetN(argv[0]) + 1;
	target = (char *)malloc(targetLength);
	targetStatus = mxGetString(argv[0], target, targetLength);

	char *query;
	size_t queryLength;
	int queryStatus;
	queryLength = mxGetN(argv[1]) + 1;
	query = (char *)malloc(queryLength);
	queryStatus = mxGetString(argv[1], query, queryLength);

	HelicsQuery result = helicsCreateQuery((char const *)target, (char const *)query);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_t*)mxGetData(_out)) = (uint64_t)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	free(target);

	free(query);
}


void _wrap_helicsQueryExecute(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsQuery query = *(HelicsQuery*)(mxGetData(argv[0]));

	HelicsFederate fed = *(HelicsFederate*)(mxGetData(argv[1]));

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
	HelicsQuery query = *(HelicsQuery*)(mxGetData(argv[0]));

	HelicsCore core = *(HelicsCore*)(mxGetData(argv[1]));

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
	HelicsQuery query = *(HelicsQuery*)(mxGetData(argv[0]));

	HelicsBroker broker = *(HelicsBroker*)(mxGetData(argv[1]));

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
	HelicsQuery query = *(HelicsQuery*)(mxGetData(argv[0]));

	HelicsFederate fed = *(HelicsFederate*)(mxGetData(argv[1]));

	HelicsError err = helicsErrorInitialize();

	helicsQueryExecuteAsync(query, fed, &err);

	mxArray *_out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}





	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsQueryExecuteComplete(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsQuery query = *(HelicsQuery*)(mxGetData(argv[0]));

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
	HelicsQuery query = *(HelicsQuery*)(mxGetData(argv[0]));

	HelicsBool result = helicsQueryIsCompleted(query);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT32_CLASS, mxREAL);
	*((int32_t*)mxGetData(_out)) = (int32_t)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsQuerySetTarget(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsQuery query = *(HelicsQuery*)(mxGetData(argv[0]));

	char *target;
	size_t targetLength;
	int targetStatus;
	targetLength = mxGetN(argv[1]) + 1;
	target = (char *)malloc(targetLength);
	targetStatus = mxGetString(argv[1], target, targetLength);

	HelicsError err = helicsErrorInitialize();

	helicsQuerySetTarget(query, (char const *)target, &err);

	mxArray *_out = (mxArray *)0;

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
	HelicsQuery query = *(HelicsQuery*)(mxGetData(argv[0]));

	char *queryString;
	size_t queryStringLength;
	int queryStringStatus;
	queryStringLength = mxGetN(argv[1]) + 1;
	queryString = (char *)malloc(queryStringLength);
	queryStringStatus = mxGetString(argv[1], queryString, queryStringLength);

	HelicsError err = helicsErrorInitialize();

	helicsQuerySetQueryString(query, (char const *)queryString, &err);

	mxArray *_out = (mxArray *)0;

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
	HelicsQuery query = *(HelicsQuery*)(mxGetData(argv[0]));

	int32_t mode = *((int32_t *)mxGetData(argv[1]));

	HelicsError err = helicsErrorInitialize();

	helicsQuerySetOrdering(query, mode, &err);

	mxArray *_out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}





	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsQueryFree(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsQuery query = *(HelicsQuery*)(mxGetData(argv[0]));

	helicsQueryFree(query);

	mxArray *_out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsCleanupLibrary(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	helicsCleanupLibrary();

	mxArray *_out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsFederateRegisterSubscription(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsFederate fed = *(HelicsFederate*)(mxGetData(argv[0]));

	char *key;
	size_t keyLength;
	int keyStatus;
	keyLength = mxGetN(argv[1]) + 1;
	key = (char *)malloc(keyLength);
	keyStatus = mxGetString(argv[1], key, keyLength);

	char *units;
	size_t unitsLength;
	int unitsStatus;
	unitsLength = mxGetN(argv[2]) + 1;
	units = (char *)malloc(unitsLength);
	unitsStatus = mxGetString(argv[2], units, unitsLength);

	HelicsError err = helicsErrorInitialize();

	HelicsInput result = helicsFederateRegisterSubscription(fed, (char const *)key, (char const *)units, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_t*)mxGetData(_out)) = (uint64_t)result;

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
	HelicsFederate fed = *(HelicsFederate*)(mxGetData(argv[0]));

	char *key;
	size_t keyLength;
	int keyStatus;
	keyLength = mxGetN(argv[1]) + 1;
	key = (char *)malloc(keyLength);
	keyStatus = mxGetString(argv[1], key, keyLength);

	int typeInt = static_cast<int>(mxGetScalar(argv[2]));
	HelicsDataTypes type = static_cast<HelicsDataTypes>(typeInt);

	char *units;
	size_t unitsLength;
	int unitsStatus;
	unitsLength = mxGetN(argv[3]) + 1;
	units = (char *)malloc(unitsLength);
	unitsStatus = mxGetString(argv[3], units, unitsLength);

	HelicsError err = helicsErrorInitialize();

	HelicsPublication result = helicsFederateRegisterPublication(fed, (char const *)key, type, (char const *)units, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_t*)mxGetData(_out)) = (uint64_t)result;

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
	HelicsFederate fed = *(HelicsFederate*)(mxGetData(argv[0]));

	char *key;
	size_t keyLength;
	int keyStatus;
	keyLength = mxGetN(argv[1]) + 1;
	key = (char *)malloc(keyLength);
	keyStatus = mxGetString(argv[1], key, keyLength);

	char *type;
	size_t typeLength;
	int typeStatus;
	typeLength = mxGetN(argv[2]) + 1;
	type = (char *)malloc(typeLength);
	typeStatus = mxGetString(argv[2], type, typeLength);

	char *units;
	size_t unitsLength;
	int unitsStatus;
	unitsLength = mxGetN(argv[3]) + 1;
	units = (char *)malloc(unitsLength);
	unitsStatus = mxGetString(argv[3], units, unitsLength);

	HelicsError err = helicsErrorInitialize();

	HelicsPublication result = helicsFederateRegisterTypePublication(fed, (char const *)key, (char const *)type, (char const *)units, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_t*)mxGetData(_out)) = (uint64_t)result;

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
	HelicsFederate fed = *(HelicsFederate*)(mxGetData(argv[0]));

	char *key;
	size_t keyLength;
	int keyStatus;
	keyLength = mxGetN(argv[1]) + 1;
	key = (char *)malloc(keyLength);
	keyStatus = mxGetString(argv[1], key, keyLength);

	int typeInt = static_cast<int>(mxGetScalar(argv[2]));
	HelicsDataTypes type = static_cast<HelicsDataTypes>(typeInt);

	char *units;
	size_t unitsLength;
	int unitsStatus;
	unitsLength = mxGetN(argv[3]) + 1;
	units = (char *)malloc(unitsLength);
	unitsStatus = mxGetString(argv[3], units, unitsLength);

	HelicsError err = helicsErrorInitialize();

	HelicsPublication result = helicsFederateRegisterGlobalPublication(fed, (char const *)key, type, (char const *)units, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_t*)mxGetData(_out)) = (uint64_t)result;

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
	HelicsFederate fed = *(HelicsFederate*)(mxGetData(argv[0]));

	char *key;
	size_t keyLength;
	int keyStatus;
	keyLength = mxGetN(argv[1]) + 1;
	key = (char *)malloc(keyLength);
	keyStatus = mxGetString(argv[1], key, keyLength);

	char *type;
	size_t typeLength;
	int typeStatus;
	typeLength = mxGetN(argv[2]) + 1;
	type = (char *)malloc(typeLength);
	typeStatus = mxGetString(argv[2], type, typeLength);

	char *units;
	size_t unitsLength;
	int unitsStatus;
	unitsLength = mxGetN(argv[3]) + 1;
	units = (char *)malloc(unitsLength);
	unitsStatus = mxGetString(argv[3], units, unitsLength);

	HelicsError err = helicsErrorInitialize();

	HelicsPublication result = helicsFederateRegisterGlobalTypePublication(fed, (char const *)key, (char const *)type, (char const *)units, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_t*)mxGetData(_out)) = (uint64_t)result;

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
	HelicsFederate fed = *(HelicsFederate*)(mxGetData(argv[0]));

	char *key;
	size_t keyLength;
	int keyStatus;
	keyLength = mxGetN(argv[1]) + 1;
	key = (char *)malloc(keyLength);
	keyStatus = mxGetString(argv[1], key, keyLength);

	int typeInt = static_cast<int>(mxGetScalar(argv[2]));
	HelicsDataTypes type = static_cast<HelicsDataTypes>(typeInt);

	char *units;
	size_t unitsLength;
	int unitsStatus;
	unitsLength = mxGetN(argv[3]) + 1;
	units = (char *)malloc(unitsLength);
	unitsStatus = mxGetString(argv[3], units, unitsLength);

	HelicsError err = helicsErrorInitialize();

	HelicsInput result = helicsFederateRegisterInput(fed, (char const *)key, type, (char const *)units, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_t*)mxGetData(_out)) = (uint64_t)result;

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
	HelicsFederate fed = *(HelicsFederate*)(mxGetData(argv[0]));

	char *key;
	size_t keyLength;
	int keyStatus;
	keyLength = mxGetN(argv[1]) + 1;
	key = (char *)malloc(keyLength);
	keyStatus = mxGetString(argv[1], key, keyLength);

	char *type;
	size_t typeLength;
	int typeStatus;
	typeLength = mxGetN(argv[2]) + 1;
	type = (char *)malloc(typeLength);
	typeStatus = mxGetString(argv[2], type, typeLength);

	char *units;
	size_t unitsLength;
	int unitsStatus;
	unitsLength = mxGetN(argv[3]) + 1;
	units = (char *)malloc(unitsLength);
	unitsStatus = mxGetString(argv[3], units, unitsLength);

	HelicsError err = helicsErrorInitialize();

	HelicsInput result = helicsFederateRegisterTypeInput(fed, (char const *)key, (char const *)type, (char const *)units, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_t*)mxGetData(_out)) = (uint64_t)result;

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
	HelicsFederate fed = *(HelicsFederate*)(mxGetData(argv[0]));

	char *key;
	size_t keyLength;
	int keyStatus;
	keyLength = mxGetN(argv[1]) + 1;
	key = (char *)malloc(keyLength);
	keyStatus = mxGetString(argv[1], key, keyLength);

	int typeInt = static_cast<int>(mxGetScalar(argv[2]));
	HelicsDataTypes type = static_cast<HelicsDataTypes>(typeInt);

	char *units;
	size_t unitsLength;
	int unitsStatus;
	unitsLength = mxGetN(argv[3]) + 1;
	units = (char *)malloc(unitsLength);
	unitsStatus = mxGetString(argv[3], units, unitsLength);

	HelicsError err = helicsErrorInitialize();

	HelicsPublication result = helicsFederateRegisterGlobalInput(fed, (char const *)key, type, (char const *)units, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_t*)mxGetData(_out)) = (uint64_t)result;

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
	HelicsFederate fed = *(HelicsFederate*)(mxGetData(argv[0]));

	char *key;
	size_t keyLength;
	int keyStatus;
	keyLength = mxGetN(argv[1]) + 1;
	key = (char *)malloc(keyLength);
	keyStatus = mxGetString(argv[1], key, keyLength);

	char *type;
	size_t typeLength;
	int typeStatus;
	typeLength = mxGetN(argv[2]) + 1;
	type = (char *)malloc(typeLength);
	typeStatus = mxGetString(argv[2], type, typeLength);

	char *units;
	size_t unitsLength;
	int unitsStatus;
	unitsLength = mxGetN(argv[3]) + 1;
	units = (char *)malloc(unitsLength);
	unitsStatus = mxGetString(argv[3], units, unitsLength);

	HelicsError err = helicsErrorInitialize();

	HelicsPublication result = helicsFederateRegisterGlobalTypeInput(fed, (char const *)key, (char const *)type, (char const *)units, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_t*)mxGetData(_out)) = (uint64_t)result;

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
	HelicsFederate fed = *(HelicsFederate*)(mxGetData(argv[0]));

	char *key;
	size_t keyLength;
	int keyStatus;
	keyLength = mxGetN(argv[1]) + 1;
	key = (char *)malloc(keyLength);
	keyStatus = mxGetString(argv[1], key, keyLength);

	HelicsError err = helicsErrorInitialize();

	HelicsPublication result = helicsFederateGetPublication(fed, (char const *)key, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_t*)mxGetData(_out)) = (uint64_t)result;

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
	HelicsFederate fed = *(HelicsFederate*)(mxGetData(argv[0]));

	int index = (int)(mxGetScalar(argv[1]));

	HelicsError err = helicsErrorInitialize();

	HelicsPublication result = helicsFederateGetPublicationByIndex(fed, index, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_t*)mxGetData(_out)) = (uint64_t)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}





	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateGetInput(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsFederate fed = *(HelicsFederate*)(mxGetData(argv[0]));

	char *key;
	size_t keyLength;
	int keyStatus;
	keyLength = mxGetN(argv[1]) + 1;
	key = (char *)malloc(keyLength);
	keyStatus = mxGetString(argv[1], key, keyLength);

	HelicsError err = helicsErrorInitialize();

	HelicsInput result = helicsFederateGetInput(fed, (char const *)key, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_t*)mxGetData(_out)) = (uint64_t)result;

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
	HelicsFederate fed = *(HelicsFederate*)(mxGetData(argv[0]));

	int index = (int)(mxGetScalar(argv[1]));

	HelicsError err = helicsErrorInitialize();

	HelicsInput result = helicsFederateGetInputByIndex(fed, index, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_t*)mxGetData(_out)) = (uint64_t)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}





	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateGetSubscription(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsFederate fed = *(HelicsFederate*)(mxGetData(argv[0]));

	char *key;
	size_t keyLength;
	int keyStatus;
	keyLength = mxGetN(argv[1]) + 1;
	key = (char *)malloc(keyLength);
	keyStatus = mxGetString(argv[1], key, keyLength);

	HelicsError err = helicsErrorInitialize();

	HelicsInput result = helicsFederateGetSubscription(fed, (char const *)key, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_t*)mxGetData(_out)) = (uint64_t)result;

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
	HelicsFederate fed = *(HelicsFederate*)(mxGetData(argv[0]));

	helicsFederateClearUpdates(fed);

	mxArray *_out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsFederateRegisterFromPublicationJSON(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsFederate fed = *(HelicsFederate*)(mxGetData(argv[0]));

	char *json;
	size_t jsonLength;
	int jsonStatus;
	jsonLength = mxGetN(argv[1]) + 1;
	json = (char *)malloc(jsonLength);
	jsonStatus = mxGetString(argv[1], json, jsonLength);

	HelicsError err = helicsErrorInitialize();

	helicsFederateRegisterFromPublicationJSON(fed, (char const *)json, &err);

	mxArray *_out = (mxArray *)0;

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
	HelicsFederate fed = *(HelicsFederate*)(mxGetData(argv[0]));

	char *json;
	size_t jsonLength;
	int jsonStatus;
	jsonLength = mxGetN(argv[1]) + 1;
	json = (char *)malloc(jsonLength);
	jsonStatus = mxGetString(argv[1], json, jsonLength);

	HelicsError err = helicsErrorInitialize();

	helicsFederatePublishJSON(fed, (char const *)json, &err);

	mxArray *_out = (mxArray *)0;

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
	HelicsPublication pub = *(HelicsPublication*)(mxGetData(argv[0]));

	HelicsBool result = helicsPublicationIsValid(pub);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT32_CLASS, mxREAL);
	*((int32_t*)mxGetData(_out)) = (int32_t)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsPublicationPublishBytes(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsPublication pub = *(HelicsPublication*)(mxGetData(argv[0]));

	char *data;
	size_t dataLength;
	int dataStatus;
	dataLength = mxGetN(argv[1]) + 1;
	data = (char *)malloc(dataLength);
	dataStatus = mxGetString(argv[1], data, dataLength);

	int inputDataLength = static_cast<int>(dataLength) - 1;

	HelicsError err = helicsErrorInitialize();

	helicsPublicationPublishBytes(pub, (void *)data, inputDataLength, &err);

	mxArray *_out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsPublicationPublishString(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsPublication pub = *(HelicsPublication*)(mxGetData(argv[0]));

	char *val;
	size_t valLength;
	int valStatus;
	valLength = mxGetN(argv[1]) + 1;
	val = (char *)malloc(valLength);
	valStatus = mxGetString(argv[1], val, valLength);

	HelicsError err = helicsErrorInitialize();

	helicsPublicationPublishString(pub, (char const *)val, &err);

	mxArray *_out = (mxArray *)0;

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
	HelicsPublication pub = *(HelicsPublication*)(mxGetData(argv[0]));

	int64_t val = *((int64_t *)mxGetData(argv[1]));

	HelicsError err = helicsErrorInitialize();

	helicsPublicationPublishInteger(pub, val, &err);

	mxArray *_out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}





	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsPublicationPublishBoolean(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsPublication pub = *(HelicsPublication*)(mxGetData(argv[0]));

	HelicsBool val = (HelicsBool)(mxGetScalar(argv[1]));

	HelicsError err = helicsErrorInitialize();

	helicsPublicationPublishBoolean(pub, val, &err);

	mxArray *_out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}





	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsPublicationPublishDouble(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsPublication pub = *(HelicsPublication*)(mxGetData(argv[0]));

	double val = mxGetScalar(argv[1]);

	HelicsError err = helicsErrorInitialize();

	helicsPublicationPublishDouble(pub, val, &err);

	mxArray *_out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}





	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsPublicationPublishTime(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsPublication pub = *(HelicsPublication*)(mxGetData(argv[0]));

	HelicsTime val = (HelicsTime)(mxGetScalar(argv[1]));

	HelicsError err = helicsErrorInitialize();

	helicsPublicationPublishTime(pub, val, &err);

	mxArray *_out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}





	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsPublicationPublishChar(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsPublication pub = *(HelicsPublication*)(mxGetData(argv[0]));

	char *val;
	size_t valLength;
	int valStatus;
	valLength = mxGetN(argv[1]) + 1;
	val = (char *)malloc(valLength);
	valStatus = mxGetString(argv[1], val, valLength);

	HelicsError err = helicsErrorInitialize();

	helicsPublicationPublishChar(pub, val[0], &err);

	mxArray *_out = (mxArray *)0;

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
	HelicsPublication pub = *(HelicsPublication*)(mxGetData(argv[0]));

	mxComplexDouble *complexValue = mxGetComplexDoubles(argv[1]);
	double value[2] = {complexValue[0].real, complexValue[0].imag};

	HelicsError err = helicsErrorInitialize();

	helicsPublicationPublishComplex(pub, value[0], value[1], &err);

	mxArray *_out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsPublicationPublishVector(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsPublication pub = *(HelicsPublication*)(mxGetData(argv[0]));

	int vectorLength =  (int)mxGetNumberOfElements(argv[1]);

	double *vectorInput =  (double *)mxGetDoubles(argv[1]);

	HelicsError err = helicsErrorInitialize();

	helicsPublicationPublishVector(pub, (const double *)vectorInput, vectorLength, &err);

	mxArray *_out = (mxArray *)0;
	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsPublicationPublishComplexVector(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsPublication pub = *(HelicsPublication*)(mxGetData(argv[0]));

	int vectorLength =  (int)mxGetN(argv[1])*2;

	double *vectorInput = (double *)malloc(vectorLength * sizeof(double));
	mxComplexDouble *vals = mxGetComplexDoubles(argv[1]);
	for(int i=0; i<vectorLength/2; ++i){
		vectorInput[2*i] = vals[i].real;
		vectorInput[2*i + 1] = vals[i].imag;
	}

	HelicsError err = helicsErrorInitialize();

	helicsPublicationPublishComplexVector(pub, (const double *)vectorInput, vectorLength, &err);

	mxArray *_out = (mxArray *)0;
	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsPublicationPublishNamedPoint(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsPublication pub = *(HelicsPublication*)(mxGetData(argv[0]));

	char *field;
	size_t fieldLength;
	int fieldStatus;
	fieldLength = mxGetN(argv[1]) + 1;
	field = (char *)malloc(fieldLength);
	fieldStatus = mxGetString(argv[1], field, fieldLength);

	double val = mxGetScalar(argv[2]);

	HelicsError err = helicsErrorInitialize();

	helicsPublicationPublishNamedPoint(pub, (char const *)field, val, &err);

	mxArray *_out = (mxArray *)0;

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
	HelicsPublication pub = *(HelicsPublication*)(mxGetData(argv[0]));

	char *target;
	size_t targetLength;
	int targetStatus;
	targetLength = mxGetN(argv[1]) + 1;
	target = (char *)malloc(targetLength);
	targetStatus = mxGetString(argv[1], target, targetLength);

	HelicsError err = helicsErrorInitialize();

	helicsPublicationAddTarget(pub, (char const *)target, &err);

	mxArray *_out = (mxArray *)0;

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
	HelicsInput ipt = *(HelicsInput*)(mxGetData(argv[0]));

	HelicsBool result = helicsInputIsValid(ipt);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT32_CLASS, mxREAL);
	*((int32_t*)mxGetData(_out)) = (int32_t)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsInputAddTarget(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsInput ipt = *(HelicsInput*)(mxGetData(argv[0]));

	char *target;
	size_t targetLength;
	int targetStatus;
	targetLength = mxGetN(argv[1]) + 1;
	target = (char *)malloc(targetLength);
	targetStatus = mxGetString(argv[1], target, targetLength);

	HelicsError err = helicsErrorInitialize();

	helicsInputAddTarget(ipt, (char const *)target, &err);

	mxArray *_out = (mxArray *)0;

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
	HelicsInput ipt = *(HelicsInput*)(mxGetData(argv[0]));

	int result = helicsInputGetByteCount(ipt);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
	*((int64_t*)mxGetData(_out)) = (int64_t)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsInputGetBytes(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsInput ipt = *(HelicsInput*)(mxGetData(argv[0]));

	int maxDataLen = helicsInputGetByteCount(ipt) + 2;

	void *data = malloc(maxDataLen);

	int actualSize = 0;

	HelicsError err = helicsErrorInitialize();

	helicsInputGetBytes(ipt, data, maxDataLen, &actualSize, &err);

	mxChar *dataChar = (mxChar *)data;
	mwSize dims[2] = {1, static_cast<mwSize>(actualSize)};
	mxArray *_out = mxCreateCharArray(2, dims);
	mxChar *out_data = (mxChar *)mxGetData(_out);
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
	HelicsInput ipt = *(HelicsInput*)(mxGetData(argv[0]));

	int result = helicsInputGetStringSize(ipt);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
	*((int64_t*)mxGetData(_out)) = (int64_t)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsInputGetString(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsInput ipt = *(HelicsInput*)(mxGetData(argv[0]));

	int maxStringLen = helicsInputGetStringSize(ipt) + 2;

	char *outputString = (char *)malloc(maxStringLen);

	int actualLength = 0;

	HelicsError err = helicsErrorInitialize();

	helicsInputGetString(ipt, outputString, maxStringLen, &actualLength, &err);

	mwSize dims[2] = {1, static_cast<mwSize>(actualLength) - 1};
	mxArray *_out = mxCreateCharArray(2, dims);
	mxChar *out_data = (mxChar *)mxGetData(_out);
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
	HelicsInput ipt = *(HelicsInput*)(mxGetData(argv[0]));

	HelicsError err = helicsErrorInitialize();

	int64_t result = helicsInputGetInteger(ipt, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
	*((int64_t*)mxGetData(_out)) = (int64_t)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsInputGetBoolean(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsInput ipt = *(HelicsInput*)(mxGetData(argv[0]));

	HelicsError err = helicsErrorInitialize();

	HelicsBool result = helicsInputGetBoolean(ipt, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT32_CLASS, mxREAL);
	*((int32_t*)mxGetData(_out)) = (int32_t)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsInputGetDouble(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsInput ipt = *(HelicsInput*)(mxGetData(argv[0]));

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
	HelicsInput ipt = *(HelicsInput*)(mxGetData(argv[0]));

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
	HelicsInput ipt = *(HelicsInput*)(mxGetData(argv[0]));

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
	HelicsInput ipt = *(HelicsInput*)(mxGetData(argv[0]));

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
	HelicsInput ipt = *(HelicsInput*)(mxGetData(argv[0]));

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
	HelicsInput ipt = *(HelicsInput*)(mxGetData(argv[0]));

	int result = helicsInputGetVectorSize(ipt);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
	*((int64_t*)mxGetData(_out)) = (int64_t)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsInputGetVector(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsInput ipt = *(HelicsInput*)(mxGetData(argv[0]));

	int maxLength = helicsInputGetVectorSize(ipt);

	double *data = (double *)malloc(maxLength * sizeof(double));

	int actualSize = 0;

	HelicsError err = helicsErrorInitialize();

	helicsInputGetVector(ipt, data, maxLength, &actualSize, &err);

	mxDouble *result_data = (mxDouble *)mxMalloc(actualSize * sizeof(mxDouble));
	for(int i=0; i<actualSize; ++i){
		result_data[i] = (mxDouble)data[i];
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
	HelicsInput ipt = *(HelicsInput*)(mxGetData(argv[0]));

	int maxLength = helicsInputGetVectorSize(ipt);

	double *data = (double *)malloc(maxLength * sizeof(double));

	int actualSize = 0;

	HelicsError err = helicsErrorInitialize();

	helicsInputGetComplexVector(ipt, data, maxLength, &actualSize, &err);

	mxComplexDouble *result_data = (mxComplexDouble *)mxMalloc((actualSize/2)*sizeof(mxComplexDouble));
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
	HelicsInput ipt = *(HelicsInput*)(mxGetData(argv[0]));

	int maxStringLen = helicsInputGetStringSize(ipt) + 2;

	char *outputString = (char *)malloc(maxStringLen);

	int actualLength = 0;

	double val = 0;

	HelicsError err = helicsErrorInitialize();

	helicsInputGetNamedPoint(ipt, outputString, maxStringLen, &actualLength, &val, &err);

	mwSize dims[2] = {1, static_cast<mwSize>(actualLength)-1};
	mxArray *_out = mxCreateCharArray(2, dims);
	mxChar *out_data = (mxChar *)mxGetData(_out);
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
	HelicsInput ipt = *(HelicsInput*)(mxGetData(argv[0]));

	char *data;
	size_t dataLength;
	int dataStatus;
	dataLength = mxGetN(argv[1]) + 1;
	data = (char *)malloc(dataLength);
	dataStatus = mxGetString(argv[1], data, dataLength);

	int inputDataLength = static_cast<int>(dataLength) - 1;

	HelicsError err = helicsErrorInitialize();

	helicsInputSetDefaultBytes(ipt, (void *)data, inputDataLength, &err);

	mxArray *_out = (mxArray *)0;
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
	HelicsInput ipt = *(HelicsInput*)(mxGetData(argv[0]));

	char *defaultString;
	size_t defaultStringLength;
	int defaultStringStatus;
	defaultStringLength = mxGetN(argv[1]) + 1;
	defaultString = (char *)malloc(defaultStringLength);
	defaultStringStatus = mxGetString(argv[1], defaultString, defaultStringLength);

	HelicsError err = helicsErrorInitialize();

	helicsInputSetDefaultString(ipt, (char const *)defaultString, &err);

	mxArray *_out = (mxArray *)0;

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
	HelicsInput ipt = *(HelicsInput*)(mxGetData(argv[0]));

	int64_t val = *((int64_t *)mxGetData(argv[1]));

	HelicsError err = helicsErrorInitialize();

	helicsInputSetDefaultInteger(ipt, val, &err);

	mxArray *_out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}





	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsInputSetDefaultBoolean(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsInput ipt = *(HelicsInput*)(mxGetData(argv[0]));

	HelicsBool val = (HelicsBool)(mxGetScalar(argv[1]));

	HelicsError err = helicsErrorInitialize();

	helicsInputSetDefaultBoolean(ipt, val, &err);

	mxArray *_out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}





	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsInputSetDefaultTime(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsInput ipt = *(HelicsInput*)(mxGetData(argv[0]));

	HelicsTime val = (HelicsTime)(mxGetScalar(argv[1]));

	HelicsError err = helicsErrorInitialize();

	helicsInputSetDefaultTime(ipt, val, &err);

	mxArray *_out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}





	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsInputSetDefaultChar(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsInput ipt = *(HelicsInput*)(mxGetData(argv[0]));

	char *val;
	size_t valLength;
	int valStatus;
	valLength = mxGetN(argv[1]) + 1;
	val = (char *)malloc(valLength);
	valStatus = mxGetString(argv[1], val, valLength);

	HelicsError err = helicsErrorInitialize();

	helicsInputSetDefaultChar(ipt, val[0], &err);

	mxArray *_out = (mxArray *)0;

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
	HelicsInput ipt = *(HelicsInput*)(mxGetData(argv[0]));

	double val = mxGetScalar(argv[1]);

	HelicsError err = helicsErrorInitialize();

	helicsInputSetDefaultDouble(ipt, val, &err);

	mxArray *_out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}





	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsInputSetDefaultComplex(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsInput ipt = *(HelicsInput*)(mxGetData(argv[0]));

	mxComplexDouble *value = mxGetComplexDoubles(argv[1]);

	HelicsError err = helicsErrorInitialize();

	helicsInputSetDefaultComplex(ipt, (double)value->real, (double)value->imag, &err);

	mxArray *_out = (mxArray *)0;
	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsInputSetDefaultVector(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsInput ipt = *(HelicsInput*)(mxGetData(argv[0]));

	int vectorLength =  (int)mxGetNumberOfElements(argv[1]);

	double *vectorInput =  (double *)mxGetDoubles(argv[1]);

	HelicsError err = helicsErrorInitialize();

	helicsInputSetDefaultVector(ipt, (const double *)vectorInput, vectorLength, &err);

	mxArray *_out = (mxArray *)0;
	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsInputSetDefaultComplexVector(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsInput ipt = *(HelicsInput*)(mxGetData(argv[0]));

	int vectorLength =  (int)mxGetN(argv[1])*2;

	double *vectorInput = (double *)malloc(vectorLength * sizeof(double));
	mxComplexDouble *vals = mxGetComplexDoubles(argv[1]);
	for(int i=0; i<vectorLength/2; ++i){
		vectorInput[2*i] = vals[i].real;
		vectorInput[2*i + 1] = vals[i].imag;
	}

	HelicsError err = helicsErrorInitialize();

	helicsInputSetDefaultComplexVector(ipt, (const double *)vectorInput, vectorLength, &err);

	mxArray *_out = (mxArray *)0;
	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsInputSetDefaultNamedPoint(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsInput ipt = *(HelicsInput*)(mxGetData(argv[0]));

	char *defaultName;
	size_t defaultNameLength;
	int defaultNameStatus;
	defaultNameLength = mxGetN(argv[1]) + 1;
	defaultName = (char *)malloc(defaultNameLength);
	defaultNameStatus = mxGetString(argv[1], defaultName, defaultNameLength);

	double val = mxGetScalar(argv[2]);

	HelicsError err = helicsErrorInitialize();

	helicsInputSetDefaultNamedPoint(ipt, (char const *)defaultName, val, &err);

	mxArray *_out = (mxArray *)0;

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
	HelicsInput ipt = *(HelicsInput*)(mxGetData(argv[0]));

	const char *result = helicsInputGetType(ipt);

	mxArray *_out = mxCreateString(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsInputGetPublicationType(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsInput ipt = *(HelicsInput*)(mxGetData(argv[0]));

	const char *result = helicsInputGetPublicationType(ipt);

	mxArray *_out = mxCreateString(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsInputGetPublicationDataType(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsInput ipt = *(HelicsInput*)(mxGetData(argv[0]));

	int result = helicsInputGetPublicationDataType(ipt);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
	*((int64_t*)mxGetData(_out)) = (int64_t)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsPublicationGetType(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsPublication pub = *(HelicsPublication*)(mxGetData(argv[0]));

	const char *result = helicsPublicationGetType(pub);

	mxArray *_out = mxCreateString(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsInputGetName(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsInput ipt = *(HelicsInput*)(mxGetData(argv[0]));

	const char *result = helicsInputGetName(ipt);

	mxArray *_out = mxCreateString(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsSubscriptionGetTarget(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsInput ipt = *(HelicsInput*)(mxGetData(argv[0]));

	const char *result = helicsSubscriptionGetTarget(ipt);

	mxArray *_out = mxCreateString(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsPublicationGetName(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsPublication pub = *(HelicsPublication*)(mxGetData(argv[0]));

	const char *result = helicsPublicationGetName(pub);

	mxArray *_out = mxCreateString(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsInputGetUnits(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsInput ipt = *(HelicsInput*)(mxGetData(argv[0]));

	const char *result = helicsInputGetUnits(ipt);

	mxArray *_out = mxCreateString(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsInputGetInjectionUnits(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsInput ipt = *(HelicsInput*)(mxGetData(argv[0]));

	const char *result = helicsInputGetInjectionUnits(ipt);

	mxArray *_out = mxCreateString(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsInputGetExtractionUnits(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsInput ipt = *(HelicsInput*)(mxGetData(argv[0]));

	const char *result = helicsInputGetExtractionUnits(ipt);

	mxArray *_out = mxCreateString(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsPublicationGetUnits(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsPublication pub = *(HelicsPublication*)(mxGetData(argv[0]));

	const char *result = helicsPublicationGetUnits(pub);

	mxArray *_out = mxCreateString(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsInputGetInfo(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsInput inp = *(HelicsInput*)(mxGetData(argv[0]));

	const char *result = helicsInputGetInfo(inp);

	mxArray *_out = mxCreateString(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsInputSetInfo(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsInput inp = *(HelicsInput*)(mxGetData(argv[0]));

	char *info;
	size_t infoLength;
	int infoStatus;
	infoLength = mxGetN(argv[1]) + 1;
	info = (char *)malloc(infoLength);
	infoStatus = mxGetString(argv[1], info, infoLength);

	HelicsError err = helicsErrorInitialize();

	helicsInputSetInfo(inp, (char const *)info, &err);

	mxArray *_out = (mxArray *)0;

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
	HelicsInput inp = *(HelicsInput*)(mxGetData(argv[0]));

	char *tagname;
	size_t tagnameLength;
	int tagnameStatus;
	tagnameLength = mxGetN(argv[1]) + 1;
	tagname = (char *)malloc(tagnameLength);
	tagnameStatus = mxGetString(argv[1], tagname, tagnameLength);

	const char *result = helicsInputGetTag(inp, (char const *)tagname);

	mxArray *_out = mxCreateString(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}



	free(tagname);
}


void _wrap_helicsInputSetTag(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsInput inp = *(HelicsInput*)(mxGetData(argv[0]));

	char *tagname;
	size_t tagnameLength;
	int tagnameStatus;
	tagnameLength = mxGetN(argv[1]) + 1;
	tagname = (char *)malloc(tagnameLength);
	tagnameStatus = mxGetString(argv[1], tagname, tagnameLength);

	char *tagvalue;
	size_t tagvalueLength;
	int tagvalueStatus;
	tagvalueLength = mxGetN(argv[2]) + 1;
	tagvalue = (char *)malloc(tagvalueLength);
	tagvalueStatus = mxGetString(argv[2], tagvalue, tagvalueLength);

	HelicsError err = helicsErrorInitialize();

	helicsInputSetTag(inp, (char const *)tagname, (char const *)tagvalue, &err);

	mxArray *_out = (mxArray *)0;

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
	HelicsPublication pub = *(HelicsPublication*)(mxGetData(argv[0]));

	const char *result = helicsPublicationGetInfo(pub);

	mxArray *_out = mxCreateString(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsPublicationSetInfo(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsPublication pub = *(HelicsPublication*)(mxGetData(argv[0]));

	char *info;
	size_t infoLength;
	int infoStatus;
	infoLength = mxGetN(argv[1]) + 1;
	info = (char *)malloc(infoLength);
	infoStatus = mxGetString(argv[1], info, infoLength);

	HelicsError err = helicsErrorInitialize();

	helicsPublicationSetInfo(pub, (char const *)info, &err);

	mxArray *_out = (mxArray *)0;

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
	HelicsPublication pub = *(HelicsPublication*)(mxGetData(argv[0]));

	char *tagname;
	size_t tagnameLength;
	int tagnameStatus;
	tagnameLength = mxGetN(argv[1]) + 1;
	tagname = (char *)malloc(tagnameLength);
	tagnameStatus = mxGetString(argv[1], tagname, tagnameLength);

	const char *result = helicsPublicationGetTag(pub, (char const *)tagname);

	mxArray *_out = mxCreateString(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}



	free(tagname);
}


void _wrap_helicsPublicationSetTag(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsPublication pub = *(HelicsPublication*)(mxGetData(argv[0]));

	char *tagname;
	size_t tagnameLength;
	int tagnameStatus;
	tagnameLength = mxGetN(argv[1]) + 1;
	tagname = (char *)malloc(tagnameLength);
	tagnameStatus = mxGetString(argv[1], tagname, tagnameLength);

	char *tagvalue;
	size_t tagvalueLength;
	int tagvalueStatus;
	tagvalueLength = mxGetN(argv[2]) + 1;
	tagvalue = (char *)malloc(tagvalueLength);
	tagvalueStatus = mxGetString(argv[2], tagvalue, tagvalueLength);

	HelicsError err = helicsErrorInitialize();

	helicsPublicationSetTag(pub, (char const *)tagname, (char const *)tagvalue, &err);

	mxArray *_out = (mxArray *)0;

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
	HelicsInput inp = *(HelicsInput*)(mxGetData(argv[0]));

	int option = (int)(mxGetScalar(argv[1]));

	int result = helicsInputGetOption(inp, option);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
	*((int64_t*)mxGetData(_out)) = (int64_t)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}




}


void _wrap_helicsInputSetOption(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsInput inp = *(HelicsInput*)(mxGetData(argv[0]));

	int option = (int)(mxGetScalar(argv[1]));

	int value = (int)(mxGetScalar(argv[2]));

	HelicsError err = helicsErrorInitialize();

	helicsInputSetOption(inp, option, value, &err);

	mxArray *_out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}







	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsPublicationGetOption(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsPublication pub = *(HelicsPublication*)(mxGetData(argv[0]));

	int option = (int)(mxGetScalar(argv[1]));

	int result = helicsPublicationGetOption(pub, option);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
	*((int64_t*)mxGetData(_out)) = (int64_t)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}




}


void _wrap_helicsPublicationSetOption(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsPublication pub = *(HelicsPublication*)(mxGetData(argv[0]));

	int option = (int)(mxGetScalar(argv[1]));

	int val = (int)(mxGetScalar(argv[2]));

	HelicsError err = helicsErrorInitialize();

	helicsPublicationSetOption(pub, option, val, &err);

	mxArray *_out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}







	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsPublicationSetMinimumChange(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsPublication pub = *(HelicsPublication*)(mxGetData(argv[0]));

	double tolerance = mxGetScalar(argv[1]);

	HelicsError err = helicsErrorInitialize();

	helicsPublicationSetMinimumChange(pub, tolerance, &err);

	mxArray *_out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}





	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsInputSetMinimumChange(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsInput inp = *(HelicsInput*)(mxGetData(argv[0]));

	double tolerance = mxGetScalar(argv[1]);

	HelicsError err = helicsErrorInitialize();

	helicsInputSetMinimumChange(inp, tolerance, &err);

	mxArray *_out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}





	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsInputIsUpdated(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsInput ipt = *(HelicsInput*)(mxGetData(argv[0]));

	HelicsBool result = helicsInputIsUpdated(ipt);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT32_CLASS, mxREAL);
	*((int32_t*)mxGetData(_out)) = (int32_t)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsInputLastUpdateTime(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsInput ipt = *(HelicsInput*)(mxGetData(argv[0]));

	HelicsTime result = helicsInputLastUpdateTime(ipt);

	mxArray *_out = mxCreateDoubleScalar(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsInputClearUpdate(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsInput ipt = *(HelicsInput*)(mxGetData(argv[0]));

	helicsInputClearUpdate(ipt);

	mxArray *_out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsFederateGetPublicationCount(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsFederate fed = *(HelicsFederate*)(mxGetData(argv[0]));

	int result = helicsFederateGetPublicationCount(fed);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
	*((int64_t*)mxGetData(_out)) = (int64_t)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsFederateGetInputCount(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsFederate fed = *(HelicsFederate*)(mxGetData(argv[0]));

	int result = helicsFederateGetInputCount(fed);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
	*((int64_t*)mxGetData(_out)) = (int64_t)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsFederateRegisterEndpoint(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsFederate fed = *(HelicsFederate*)(mxGetData(argv[0]));

	char *name;
	size_t nameLength;
	int nameStatus;
	nameLength = mxGetN(argv[1]) + 1;
	name = (char *)malloc(nameLength);
	nameStatus = mxGetString(argv[1], name, nameLength);

	char *type;
	size_t typeLength;
	int typeStatus;
	typeLength = mxGetN(argv[2]) + 1;
	type = (char *)malloc(typeLength);
	typeStatus = mxGetString(argv[2], type, typeLength);

	HelicsError err = helicsErrorInitialize();

	HelicsEndpoint result = helicsFederateRegisterEndpoint(fed, (char const *)name, (char const *)type, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_t*)mxGetData(_out)) = (uint64_t)result;

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
	HelicsFederate fed = *(HelicsFederate*)(mxGetData(argv[0]));

	char *name;
	size_t nameLength;
	int nameStatus;
	nameLength = mxGetN(argv[1]) + 1;
	name = (char *)malloc(nameLength);
	nameStatus = mxGetString(argv[1], name, nameLength);

	char *type;
	size_t typeLength;
	int typeStatus;
	typeLength = mxGetN(argv[2]) + 1;
	type = (char *)malloc(typeLength);
	typeStatus = mxGetString(argv[2], type, typeLength);

	HelicsError err = helicsErrorInitialize();

	HelicsEndpoint result = helicsFederateRegisterGlobalEndpoint(fed, (char const *)name, (char const *)type, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_t*)mxGetData(_out)) = (uint64_t)result;

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
	HelicsFederate fed = *(HelicsFederate*)(mxGetData(argv[0]));

	char *name;
	size_t nameLength;
	int nameStatus;
	nameLength = mxGetN(argv[1]) + 1;
	name = (char *)malloc(nameLength);
	nameStatus = mxGetString(argv[1], name, nameLength);

	char *type;
	size_t typeLength;
	int typeStatus;
	typeLength = mxGetN(argv[2]) + 1;
	type = (char *)malloc(typeLength);
	typeStatus = mxGetString(argv[2], type, typeLength);

	HelicsError err = helicsErrorInitialize();

	HelicsEndpoint result = helicsFederateRegisterTargetedEndpoint(fed, (char const *)name, (char const *)type, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_t*)mxGetData(_out)) = (uint64_t)result;

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
	HelicsFederate fed = *(HelicsFederate*)(mxGetData(argv[0]));

	char *name;
	size_t nameLength;
	int nameStatus;
	nameLength = mxGetN(argv[1]) + 1;
	name = (char *)malloc(nameLength);
	nameStatus = mxGetString(argv[1], name, nameLength);

	char *type;
	size_t typeLength;
	int typeStatus;
	typeLength = mxGetN(argv[2]) + 1;
	type = (char *)malloc(typeLength);
	typeStatus = mxGetString(argv[2], type, typeLength);

	HelicsError err = helicsErrorInitialize();

	HelicsEndpoint result = helicsFederateRegisterGlobalTargetedEndpoint(fed, (char const *)name, (char const *)type, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_t*)mxGetData(_out)) = (uint64_t)result;

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
	HelicsFederate fed = *(HelicsFederate*)(mxGetData(argv[0]));

	char *name;
	size_t nameLength;
	int nameStatus;
	nameLength = mxGetN(argv[1]) + 1;
	name = (char *)malloc(nameLength);
	nameStatus = mxGetString(argv[1], name, nameLength);

	HelicsError err = helicsErrorInitialize();

	HelicsEndpoint result = helicsFederateGetEndpoint(fed, (char const *)name, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_t*)mxGetData(_out)) = (uint64_t)result;

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
	HelicsFederate fed = *(HelicsFederate*)(mxGetData(argv[0]));

	int index = (int)(mxGetScalar(argv[1]));

	HelicsError err = helicsErrorInitialize();

	HelicsEndpoint result = helicsFederateGetEndpointByIndex(fed, index, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_t*)mxGetData(_out)) = (uint64_t)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}





	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsEndpointIsValid(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsEndpoint endpoint = *(HelicsEndpoint*)(mxGetData(argv[0]));

	HelicsBool result = helicsEndpointIsValid(endpoint);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT32_CLASS, mxREAL);
	*((int32_t*)mxGetData(_out)) = (int32_t)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsEndpointSetDefaultDestination(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsEndpoint endpoint = *(HelicsEndpoint*)(mxGetData(argv[0]));

	char *dst;
	size_t dstLength;
	int dstStatus;
	dstLength = mxGetN(argv[1]) + 1;
	dst = (char *)malloc(dstLength);
	dstStatus = mxGetString(argv[1], dst, dstLength);

	HelicsError err = helicsErrorInitialize();

	helicsEndpointSetDefaultDestination(endpoint, (char const *)dst, &err);

	mxArray *_out = (mxArray *)0;

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
	HelicsEndpoint endpoint = *(HelicsEndpoint*)(mxGetData(argv[0]));

	const char *result = helicsEndpointGetDefaultDestination(endpoint);

	mxArray *_out = mxCreateString(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsEndpointSendBytes(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsEndpoint endpoint = *(HelicsEndpoint*)(mxGetData(argv[0]));

	char *data;
	size_t dataLength;
	int dataStatus;
	dataLength = mxGetN(argv[1]) + 1;
	data = (char *)malloc(dataLength);
	dataStatus = mxGetString(argv[1], data, dataLength);

	int inputDataLength = (int)(dataLength - 1);

	HelicsError err = helicsErrorInitialize();

	helicsEndpointSendBytes(endpoint, (void *)data, inputDataLength, &err);

	mxArray *_out = (mxArray *)0;
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
	HelicsEndpoint endpoint = *(HelicsEndpoint*)(mxGetData(argv[0]));

	char *data;
	size_t dataLength;
	int dataStatus;
	dataLength = mxGetN(argv[1]) + 1;
	data = (char *)malloc(dataLength);
	dataStatus = mxGetString(argv[1], data, dataLength);

	int inputDataLength = (int)(dataLength - 1);

	char *dst;
	size_t dstLength;
	int dstStatus;
	dstLength = mxGetN(argv[2]) + 1;
	dst = (char *)malloc(dstLength);
	dstStatus = mxGetString(argv[2], dst, dstLength);

	HelicsError err = helicsErrorInitialize();

	helicsEndpointSendBytesTo(endpoint, (void *)data, inputDataLength, dst, &err);

	mxArray *_out = (mxArray *)0;
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
	HelicsEndpoint endpoint = *(HelicsEndpoint*)(mxGetData(argv[0]));

	char *data;
	size_t dataLength;
	int dataStatus;
	dataLength = mxGetN(argv[1]) + 1;
	data = (char *)malloc(dataLength);
	dataStatus = mxGetString(argv[1], data, dataLength);

	int inputDataLength = (int)(dataLength - 1);

	char *dst;
	size_t dstLength;
	int dstStatus;
	dstLength = mxGetN(argv[2]) + 1;
	dst = (char *)malloc(dstLength);
	dstStatus = mxGetString(argv[2], dst, dstLength);

	HelicsTime time = (HelicsTime)(mxGetScalar(argv[3]));

	HelicsError err = helicsErrorInitialize();

	helicsEndpointSendBytesToAt(endpoint, (void *)data, inputDataLength, dst, time, &err);

	mxArray *_out = (mxArray *)0;
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
	HelicsEndpoint endpoint = *(HelicsEndpoint*)(mxGetData(argv[0]));

	char *data;
	size_t dataLength;
	int dataStatus;
	dataLength = mxGetN(argv[1]) + 1;
	data = (char *)malloc(dataLength);
	dataStatus = mxGetString(argv[1], data, dataLength);

	int inputDataLength = (int)(dataLength - 1);

	HelicsTime time = (HelicsTime)(mxGetScalar(argv[2]));

	HelicsError err = helicsErrorInitialize();

	helicsEndpointSendBytesAt(endpoint, (void *)data, inputDataLength, time, &err);

	mxArray *_out = (mxArray *)0;
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
	HelicsEndpoint endpoint = *(HelicsEndpoint*)(mxGetData(argv[0]));

	HelicsMessage message = *(HelicsMessage*)(mxGetData(argv[1]));

	HelicsError err = helicsErrorInitialize();

	helicsEndpointSendMessage(endpoint, message, &err);

	mxArray *_out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}





	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsEndpointSendMessageZeroCopy(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsEndpoint endpoint = *(HelicsEndpoint*)(mxGetData(argv[0]));

	HelicsMessage message = *(HelicsMessage*)(mxGetData(argv[1]));

	HelicsError err = helicsErrorInitialize();

	helicsEndpointSendMessageZeroCopy(endpoint, message, &err);

	mxArray *_out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}





	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsEndpointSubscribe(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsEndpoint endpoint = *(HelicsEndpoint*)(mxGetData(argv[0]));

	char *key;
	size_t keyLength;
	int keyStatus;
	keyLength = mxGetN(argv[1]) + 1;
	key = (char *)malloc(keyLength);
	keyStatus = mxGetString(argv[1], key, keyLength);

	HelicsError err = helicsErrorInitialize();

	helicsEndpointSubscribe(endpoint, (char const *)key, &err);

	mxArray *_out = (mxArray *)0;

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
	HelicsFederate fed = *(HelicsFederate*)(mxGetData(argv[0]));

	HelicsBool result = helicsFederateHasMessage(fed);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT32_CLASS, mxREAL);
	*((int32_t*)mxGetData(_out)) = (int32_t)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsEndpointHasMessage(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsEndpoint endpoint = *(HelicsEndpoint*)(mxGetData(argv[0]));

	HelicsBool result = helicsEndpointHasMessage(endpoint);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT32_CLASS, mxREAL);
	*((int32_t*)mxGetData(_out)) = (int32_t)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsFederatePendingMessageCount(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsFederate fed = *(HelicsFederate*)(mxGetData(argv[0]));

	int result = helicsFederatePendingMessageCount(fed);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
	*((int64_t*)mxGetData(_out)) = (int64_t)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsEndpointPendingMessageCount(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsEndpoint endpoint = *(HelicsEndpoint*)(mxGetData(argv[0]));

	int result = helicsEndpointPendingMessageCount(endpoint);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
	*((int64_t*)mxGetData(_out)) = (int64_t)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsEndpointGetMessage(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsEndpoint endpoint = *(HelicsEndpoint*)(mxGetData(argv[0]));

	HelicsMessage result = helicsEndpointGetMessage(endpoint);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_t*)mxGetData(_out)) = (uint64_t)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsEndpointCreateMessage(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsEndpoint endpoint = *(HelicsEndpoint*)(mxGetData(argv[0]));

	HelicsError err = helicsErrorInitialize();

	HelicsMessage result = helicsEndpointCreateMessage(endpoint, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_t*)mxGetData(_out)) = (uint64_t)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsEndpointClearMessages(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsEndpoint endpoint = *(HelicsEndpoint*)(mxGetData(argv[0]));

	helicsEndpointClearMessages(endpoint);

	mxArray *_out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsFederateGetMessage(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsFederate fed = *(HelicsFederate*)(mxGetData(argv[0]));

	HelicsMessage result = helicsFederateGetMessage(fed);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_t*)mxGetData(_out)) = (uint64_t)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsFederateCreateMessage(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsFederate fed = *(HelicsFederate*)(mxGetData(argv[0]));

	HelicsError err = helicsErrorInitialize();

	HelicsMessage result = helicsFederateCreateMessage(fed, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_t*)mxGetData(_out)) = (uint64_t)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateClearMessages(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsFederate fed = *(HelicsFederate*)(mxGetData(argv[0]));

	helicsFederateClearMessages(fed);

	mxArray *_out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsEndpointGetType(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsEndpoint endpoint = *(HelicsEndpoint*)(mxGetData(argv[0]));

	const char *result = helicsEndpointGetType(endpoint);

	mxArray *_out = mxCreateString(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsEndpointGetName(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsEndpoint endpoint = *(HelicsEndpoint*)(mxGetData(argv[0]));

	const char *result = helicsEndpointGetName(endpoint);

	mxArray *_out = mxCreateString(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsFederateGetEndpointCount(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsFederate fed = *(HelicsFederate*)(mxGetData(argv[0]));

	int result = helicsFederateGetEndpointCount(fed);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
	*((int64_t*)mxGetData(_out)) = (int64_t)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsEndpointGetInfo(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsEndpoint end = *(HelicsEndpoint*)(mxGetData(argv[0]));

	const char *result = helicsEndpointGetInfo(end);

	mxArray *_out = mxCreateString(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsEndpointSetInfo(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsEndpoint endpoint = *(HelicsEndpoint*)(mxGetData(argv[0]));

	char *info;
	size_t infoLength;
	int infoStatus;
	infoLength = mxGetN(argv[1]) + 1;
	info = (char *)malloc(infoLength);
	infoStatus = mxGetString(argv[1], info, infoLength);

	HelicsError err = helicsErrorInitialize();

	helicsEndpointSetInfo(endpoint, (char const *)info, &err);

	mxArray *_out = (mxArray *)0;

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
	HelicsEndpoint endpoint = *(HelicsEndpoint*)(mxGetData(argv[0]));

	char *tagname;
	size_t tagnameLength;
	int tagnameStatus;
	tagnameLength = mxGetN(argv[1]) + 1;
	tagname = (char *)malloc(tagnameLength);
	tagnameStatus = mxGetString(argv[1], tagname, tagnameLength);

	const char *result = helicsEndpointGetTag(endpoint, (char const *)tagname);

	mxArray *_out = mxCreateString(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}



	free(tagname);
}


void _wrap_helicsEndpointSetTag(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsEndpoint endpoint = *(HelicsEndpoint*)(mxGetData(argv[0]));

	char *tagname;
	size_t tagnameLength;
	int tagnameStatus;
	tagnameLength = mxGetN(argv[1]) + 1;
	tagname = (char *)malloc(tagnameLength);
	tagnameStatus = mxGetString(argv[1], tagname, tagnameLength);

	char *tagvalue;
	size_t tagvalueLength;
	int tagvalueStatus;
	tagvalueLength = mxGetN(argv[2]) + 1;
	tagvalue = (char *)malloc(tagvalueLength);
	tagvalueStatus = mxGetString(argv[2], tagvalue, tagvalueLength);

	HelicsError err = helicsErrorInitialize();

	helicsEndpointSetTag(endpoint, (char const *)tagname, (char const *)tagvalue, &err);

	mxArray *_out = (mxArray *)0;

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
	HelicsEndpoint endpoint = *(HelicsEndpoint*)(mxGetData(argv[0]));

	int option = (int)(mxGetScalar(argv[1]));

	int value = (int)(mxGetScalar(argv[2]));

	HelicsError err = helicsErrorInitialize();

	helicsEndpointSetOption(endpoint, option, value, &err);

	mxArray *_out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}







	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsEndpointGetOption(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsEndpoint endpoint = *(HelicsEndpoint*)(mxGetData(argv[0]));

	int option = (int)(mxGetScalar(argv[1]));

	int result = helicsEndpointGetOption(endpoint, option);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
	*((int64_t*)mxGetData(_out)) = (int64_t)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}




}


void _wrap_helicsEndpointAddSourceTarget(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsEndpoint endpoint = *(HelicsEndpoint*)(mxGetData(argv[0]));

	char *targetEndpoint;
	size_t targetEndpointLength;
	int targetEndpointStatus;
	targetEndpointLength = mxGetN(argv[1]) + 1;
	targetEndpoint = (char *)malloc(targetEndpointLength);
	targetEndpointStatus = mxGetString(argv[1], targetEndpoint, targetEndpointLength);

	HelicsError err = helicsErrorInitialize();

	helicsEndpointAddSourceTarget(endpoint, (char const *)targetEndpoint, &err);

	mxArray *_out = (mxArray *)0;

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
	HelicsEndpoint endpoint = *(HelicsEndpoint*)(mxGetData(argv[0]));

	char *targetEndpoint;
	size_t targetEndpointLength;
	int targetEndpointStatus;
	targetEndpointLength = mxGetN(argv[1]) + 1;
	targetEndpoint = (char *)malloc(targetEndpointLength);
	targetEndpointStatus = mxGetString(argv[1], targetEndpoint, targetEndpointLength);

	HelicsError err = helicsErrorInitialize();

	helicsEndpointAddDestinationTarget(endpoint, (char const *)targetEndpoint, &err);

	mxArray *_out = (mxArray *)0;

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
	HelicsEndpoint endpoint = *(HelicsEndpoint*)(mxGetData(argv[0]));

	char *targetEndpoint;
	size_t targetEndpointLength;
	int targetEndpointStatus;
	targetEndpointLength = mxGetN(argv[1]) + 1;
	targetEndpoint = (char *)malloc(targetEndpointLength);
	targetEndpointStatus = mxGetString(argv[1], targetEndpoint, targetEndpointLength);

	HelicsError err = helicsErrorInitialize();

	helicsEndpointRemoveTarget(endpoint, (char const *)targetEndpoint, &err);

	mxArray *_out = (mxArray *)0;

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
	HelicsEndpoint endpoint = *(HelicsEndpoint*)(mxGetData(argv[0]));

	char *filterName;
	size_t filterNameLength;
	int filterNameStatus;
	filterNameLength = mxGetN(argv[1]) + 1;
	filterName = (char *)malloc(filterNameLength);
	filterNameStatus = mxGetString(argv[1], filterName, filterNameLength);

	HelicsError err = helicsErrorInitialize();

	helicsEndpointAddSourceFilter(endpoint, (char const *)filterName, &err);

	mxArray *_out = (mxArray *)0;

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
	HelicsEndpoint endpoint = *(HelicsEndpoint*)(mxGetData(argv[0]));

	char *filterName;
	size_t filterNameLength;
	int filterNameStatus;
	filterNameLength = mxGetN(argv[1]) + 1;
	filterName = (char *)malloc(filterNameLength);
	filterNameStatus = mxGetString(argv[1], filterName, filterNameLength);

	HelicsError err = helicsErrorInitialize();

	helicsEndpointAddDestinationFilter(endpoint, (char const *)filterName, &err);

	mxArray *_out = (mxArray *)0;

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
	HelicsMessage message = *(HelicsMessage*)(mxGetData(argv[0]));

	const char *result = helicsMessageGetSource(message);

	mxArray *_out = mxCreateString(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsMessageGetDestination(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsMessage message = *(HelicsMessage*)(mxGetData(argv[0]));

	const char *result = helicsMessageGetDestination(message);

	mxArray *_out = mxCreateString(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsMessageGetOriginalSource(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsMessage message = *(HelicsMessage*)(mxGetData(argv[0]));

	const char *result = helicsMessageGetOriginalSource(message);

	mxArray *_out = mxCreateString(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsMessageGetOriginalDestination(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsMessage message = *(HelicsMessage*)(mxGetData(argv[0]));

	const char *result = helicsMessageGetOriginalDestination(message);

	mxArray *_out = mxCreateString(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsMessageGetTime(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsMessage message = *(HelicsMessage*)(mxGetData(argv[0]));

	HelicsTime result = helicsMessageGetTime(message);

	mxArray *_out = mxCreateDoubleScalar(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsMessageGetString(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsMessage message = *(HelicsMessage*)(mxGetData(argv[0]));

	const char *result = helicsMessageGetString(message);

	mxArray *_out = mxCreateString(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsMessageGetMessageID(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsMessage message = *(HelicsMessage*)(mxGetData(argv[0]));

	int result = helicsMessageGetMessageID(message);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
	*((int64_t*)mxGetData(_out)) = (int64_t)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsMessageGetFlagOption(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsMessage message = *(HelicsMessage*)(mxGetData(argv[0]));

	int flag = (int)(mxGetScalar(argv[1]));

	HelicsBool result = helicsMessageGetFlagOption(message, flag);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT32_CLASS, mxREAL);
	*((int32_t*)mxGetData(_out)) = (int32_t)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}




}


void _wrap_helicsMessageGetByteCount(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsMessage message = *(HelicsMessage*)(mxGetData(argv[0]));

	int result = helicsMessageGetByteCount(message);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
	*((int64_t*)mxGetData(_out)) = (int64_t)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsMessageGetBytes(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsMessage message = *(HelicsMessage*)(mxGetData(argv[0]));

	int maxMessageLength = helicsMessageGetByteCount(message) + 2;

	char *data = (char *)malloc(maxMessageLength);

	int actualSize = 0;

	HelicsError err = helicsErrorInitialize();

	helicsMessageGetBytes(message, (void *)data, maxMessageLength, &actualSize, &err);

	mwSize dims[2] = {1,(mwSize)actualSize};
	mxArray *_out = mxCreateCharArray(2,dims);
	mxChar *out_data = (mxChar *)mxGetData(_out);
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
	HelicsMessage message = *(HelicsMessage*)(mxGetData(argv[0]));

	void *result = helicsMessageGetBytesPointer(message);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_t*)mxGetData(_out)) = (uint64_t)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsMessageIsValid(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsMessage message = *(HelicsMessage*)(mxGetData(argv[0]));

	HelicsBool result = helicsMessageIsValid(message);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT32_CLASS, mxREAL);
	*((int32_t*)mxGetData(_out)) = (int32_t)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsMessageSetSource(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsMessage message = *(HelicsMessage*)(mxGetData(argv[0]));

	char *src;
	size_t srcLength;
	int srcStatus;
	srcLength = mxGetN(argv[1]) + 1;
	src = (char *)malloc(srcLength);
	srcStatus = mxGetString(argv[1], src, srcLength);

	HelicsError err = helicsErrorInitialize();

	helicsMessageSetSource(message, (char const *)src, &err);

	mxArray *_out = (mxArray *)0;

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
	HelicsMessage message = *(HelicsMessage*)(mxGetData(argv[0]));

	char *dst;
	size_t dstLength;
	int dstStatus;
	dstLength = mxGetN(argv[1]) + 1;
	dst = (char *)malloc(dstLength);
	dstStatus = mxGetString(argv[1], dst, dstLength);

	HelicsError err = helicsErrorInitialize();

	helicsMessageSetDestination(message, (char const *)dst, &err);

	mxArray *_out = (mxArray *)0;

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
	HelicsMessage message = *(HelicsMessage*)(mxGetData(argv[0]));

	char *src;
	size_t srcLength;
	int srcStatus;
	srcLength = mxGetN(argv[1]) + 1;
	src = (char *)malloc(srcLength);
	srcStatus = mxGetString(argv[1], src, srcLength);

	HelicsError err = helicsErrorInitialize();

	helicsMessageSetOriginalSource(message, (char const *)src, &err);

	mxArray *_out = (mxArray *)0;

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
	HelicsMessage message = *(HelicsMessage*)(mxGetData(argv[0]));

	char *dst;
	size_t dstLength;
	int dstStatus;
	dstLength = mxGetN(argv[1]) + 1;
	dst = (char *)malloc(dstLength);
	dstStatus = mxGetString(argv[1], dst, dstLength);

	HelicsError err = helicsErrorInitialize();

	helicsMessageSetOriginalDestination(message, (char const *)dst, &err);

	mxArray *_out = (mxArray *)0;

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
	HelicsMessage message = *(HelicsMessage*)(mxGetData(argv[0]));

	HelicsTime time = (HelicsTime)(mxGetScalar(argv[1]));

	HelicsError err = helicsErrorInitialize();

	helicsMessageSetTime(message, time, &err);

	mxArray *_out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}





	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsMessageResize(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsMessage message = *(HelicsMessage*)(mxGetData(argv[0]));

	int newSize = (int)(mxGetScalar(argv[1]));

	HelicsError err = helicsErrorInitialize();

	helicsMessageResize(message, newSize, &err);

	mxArray *_out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}





	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsMessageReserve(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsMessage message = *(HelicsMessage*)(mxGetData(argv[0]));

	int reserveSize = (int)(mxGetScalar(argv[1]));

	HelicsError err = helicsErrorInitialize();

	helicsMessageReserve(message, reserveSize, &err);

	mxArray *_out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}





	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsMessageSetMessageID(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsMessage message = *(HelicsMessage*)(mxGetData(argv[0]));

	int32_t messageID = *((int32_t *)mxGetData(argv[1]));

	HelicsError err = helicsErrorInitialize();

	helicsMessageSetMessageID(message, messageID, &err);

	mxArray *_out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}





	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsMessageClearFlags(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsMessage message = *(HelicsMessage*)(mxGetData(argv[0]));

	helicsMessageClearFlags(message);

	mxArray *_out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsMessageSetFlagOption(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsMessage message = *(HelicsMessage*)(mxGetData(argv[0]));

	int flag = (int)(mxGetScalar(argv[1]));

	HelicsBool flagValue = (HelicsBool)(mxGetScalar(argv[2]));

	HelicsError err = helicsErrorInitialize();

	helicsMessageSetFlagOption(message, flag, flagValue, &err);

	mxArray *_out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}







	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsMessageSetString(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsMessage message = *(HelicsMessage*)(mxGetData(argv[0]));

	char *data;
	size_t dataLength;
	int dataStatus;
	dataLength = mxGetN(argv[1]) + 1;
	data = (char *)malloc(dataLength);
	dataStatus = mxGetString(argv[1], data, dataLength);

	HelicsError err = helicsErrorInitialize();

	helicsMessageSetString(message, (char const *)data, &err);

	mxArray *_out = (mxArray *)0;

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
	HelicsMessage message = *(HelicsMessage*)(mxGetData(argv[0]));

	char *data;
	size_t dataLength;
	int dataStatus;
	dataLength = mxGetN(argv[1]) + 1;
	data = (char *)malloc(dataLength);
	dataStatus = mxGetString(argv[1], data, dataLength);

	int inputDataLength = static_cast<int>(dataLength) - 1;

	HelicsError err = helicsErrorInitialize();

	helicsMessageSetData(message, (void *)data, inputDataLength, &err);

	mxArray *_out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsMessageAppendData(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsMessage message = *(HelicsMessage*)(mxGetData(argv[0]));

	char *data;
	size_t dataLength;
	int dataStatus;
	dataLength = mxGetN(argv[1]) + 1;
	data = (char *)malloc(dataLength);
	dataStatus = mxGetString(argv[1], data, dataLength);

	int inputDataLength = static_cast<int>(dataLength) - 1;

	HelicsError err = helicsErrorInitialize();

	helicsMessageAppendData(message, (void *)data, inputDataLength, &err);

	mxArray *_out = (mxArray *)0;
	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsMessageCopy(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsMessage src_message = *(HelicsMessage*)(mxGetData(argv[0]));

	HelicsMessage dst_message = *(HelicsMessage*)(mxGetData(argv[1]));

	HelicsError err = helicsErrorInitialize();

	helicsMessageCopy(src_message, dst_message, &err);

	mxArray *_out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}





	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsMessageClone(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsMessage message = *(HelicsMessage*)(mxGetData(argv[0]));

	HelicsError err = helicsErrorInitialize();

	HelicsMessage result = helicsMessageClone(message, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_t*)mxGetData(_out)) = (uint64_t)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsMessageFree(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsMessage message = *(HelicsMessage*)(mxGetData(argv[0]));

	helicsMessageFree(message);

	mxArray *_out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsMessageClear(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsMessage message = *(HelicsMessage*)(mxGetData(argv[0]));

	HelicsError err = helicsErrorInitialize();

	helicsMessageClear(message, &err);

	mxArray *_out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateRegisterFilter(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsFederate fed = *(HelicsFederate*)(mxGetData(argv[0]));

	int typeInt = static_cast<int>(mxGetScalar(argv[1]));
	HelicsFilterTypes type = static_cast<HelicsFilterTypes>(typeInt);

	char *name;
	size_t nameLength;
	int nameStatus;
	nameLength = mxGetN(argv[2]) + 1;
	name = (char *)malloc(nameLength);
	nameStatus = mxGetString(argv[2], name, nameLength);

	HelicsError err = helicsErrorInitialize();

	HelicsFilter result = helicsFederateRegisterFilter(fed, type, (char const *)name, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_t*)mxGetData(_out)) = (uint64_t)result;

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
	HelicsFederate fed = *(HelicsFederate*)(mxGetData(argv[0]));

	int typeInt = static_cast<int>(mxGetScalar(argv[1]));
	HelicsFilterTypes type = static_cast<HelicsFilterTypes>(typeInt);

	char *name;
	size_t nameLength;
	int nameStatus;
	nameLength = mxGetN(argv[2]) + 1;
	name = (char *)malloc(nameLength);
	nameStatus = mxGetString(argv[2], name, nameLength);

	HelicsError err = helicsErrorInitialize();

	HelicsFilter result = helicsFederateRegisterGlobalFilter(fed, type, (char const *)name, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_t*)mxGetData(_out)) = (uint64_t)result;

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
	HelicsFederate fed = *(HelicsFederate*)(mxGetData(argv[0]));

	char *name;
	size_t nameLength;
	int nameStatus;
	nameLength = mxGetN(argv[1]) + 1;
	name = (char *)malloc(nameLength);
	nameStatus = mxGetString(argv[1], name, nameLength);

	HelicsError err = helicsErrorInitialize();

	HelicsFilter result = helicsFederateRegisterCloningFilter(fed, (char const *)name, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_t*)mxGetData(_out)) = (uint64_t)result;

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
	HelicsFederate fed = *(HelicsFederate*)(mxGetData(argv[0]));

	char *name;
	size_t nameLength;
	int nameStatus;
	nameLength = mxGetN(argv[1]) + 1;
	name = (char *)malloc(nameLength);
	nameStatus = mxGetString(argv[1], name, nameLength);

	HelicsError err = helicsErrorInitialize();

	HelicsFilter result = helicsFederateRegisterGlobalCloningFilter(fed, (char const *)name, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_t*)mxGetData(_out)) = (uint64_t)result;

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
	HelicsCore core = *(HelicsCore*)(mxGetData(argv[0]));

	int typeInt = static_cast<int>(mxGetScalar(argv[1]));
	HelicsFilterTypes type = static_cast<HelicsFilterTypes>(typeInt);

	char *name;
	size_t nameLength;
	int nameStatus;
	nameLength = mxGetN(argv[2]) + 1;
	name = (char *)malloc(nameLength);
	nameStatus = mxGetString(argv[2], name, nameLength);

	HelicsError err = helicsErrorInitialize();

	HelicsFilter result = helicsCoreRegisterFilter(core, type, (char const *)name, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_t*)mxGetData(_out)) = (uint64_t)result;

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
	HelicsCore core = *(HelicsCore*)(mxGetData(argv[0]));

	char *name;
	size_t nameLength;
	int nameStatus;
	nameLength = mxGetN(argv[1]) + 1;
	name = (char *)malloc(nameLength);
	nameStatus = mxGetString(argv[1], name, nameLength);

	HelicsError err = helicsErrorInitialize();

	HelicsFilter result = helicsCoreRegisterCloningFilter(core, (char const *)name, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_t*)mxGetData(_out)) = (uint64_t)result;

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
	HelicsFederate fed = *(HelicsFederate*)(mxGetData(argv[0]));

	int result = helicsFederateGetFilterCount(fed);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
	*((int64_t*)mxGetData(_out)) = (int64_t)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsFederateGetFilter(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsFederate fed = *(HelicsFederate*)(mxGetData(argv[0]));

	char *name;
	size_t nameLength;
	int nameStatus;
	nameLength = mxGetN(argv[1]) + 1;
	name = (char *)malloc(nameLength);
	nameStatus = mxGetString(argv[1], name, nameLength);

	HelicsError err = helicsErrorInitialize();

	HelicsFilter result = helicsFederateGetFilter(fed, (char const *)name, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_t*)mxGetData(_out)) = (uint64_t)result;

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
	HelicsFederate fed = *(HelicsFederate*)(mxGetData(argv[0]));

	int index = (int)(mxGetScalar(argv[1]));

	HelicsError err = helicsErrorInitialize();

	HelicsFilter result = helicsFederateGetFilterByIndex(fed, index, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_t*)mxGetData(_out)) = (uint64_t)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}





	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFilterIsValid(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsFilter filt = *(HelicsFilter*)(mxGetData(argv[0]));

	HelicsBool result = helicsFilterIsValid(filt);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT32_CLASS, mxREAL);
	*((int32_t*)mxGetData(_out)) = (int32_t)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsFilterGetName(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsFilter filt = *(HelicsFilter*)(mxGetData(argv[0]));

	const char *result = helicsFilterGetName(filt);

	mxArray *_out = mxCreateString(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsFilterSet(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsFilter filt = *(HelicsFilter*)(mxGetData(argv[0]));

	char *prop;
	size_t propLength;
	int propStatus;
	propLength = mxGetN(argv[1]) + 1;
	prop = (char *)malloc(propLength);
	propStatus = mxGetString(argv[1], prop, propLength);

	double val = mxGetScalar(argv[2]);

	HelicsError err = helicsErrorInitialize();

	helicsFilterSet(filt, (char const *)prop, val, &err);

	mxArray *_out = (mxArray *)0;

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
	HelicsFilter filt = *(HelicsFilter*)(mxGetData(argv[0]));

	char *prop;
	size_t propLength;
	int propStatus;
	propLength = mxGetN(argv[1]) + 1;
	prop = (char *)malloc(propLength);
	propStatus = mxGetString(argv[1], prop, propLength);

	char *val;
	size_t valLength;
	int valStatus;
	valLength = mxGetN(argv[2]) + 1;
	val = (char *)malloc(valLength);
	valStatus = mxGetString(argv[2], val, valLength);

	HelicsError err = helicsErrorInitialize();

	helicsFilterSetString(filt, (char const *)prop, (char const *)val, &err);

	mxArray *_out = (mxArray *)0;

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
	HelicsFilter filt = *(HelicsFilter*)(mxGetData(argv[0]));

	char *dst;
	size_t dstLength;
	int dstStatus;
	dstLength = mxGetN(argv[1]) + 1;
	dst = (char *)malloc(dstLength);
	dstStatus = mxGetString(argv[1], dst, dstLength);

	HelicsError err = helicsErrorInitialize();

	helicsFilterAddDestinationTarget(filt, (char const *)dst, &err);

	mxArray *_out = (mxArray *)0;

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
	HelicsFilter filt = *(HelicsFilter*)(mxGetData(argv[0]));

	char *source;
	size_t sourceLength;
	int sourceStatus;
	sourceLength = mxGetN(argv[1]) + 1;
	source = (char *)malloc(sourceLength);
	sourceStatus = mxGetString(argv[1], source, sourceLength);

	HelicsError err = helicsErrorInitialize();

	helicsFilterAddSourceTarget(filt, (char const *)source, &err);

	mxArray *_out = (mxArray *)0;

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
	HelicsFilter filt = *(HelicsFilter*)(mxGetData(argv[0]));

	char *deliveryEndpoint;
	size_t deliveryEndpointLength;
	int deliveryEndpointStatus;
	deliveryEndpointLength = mxGetN(argv[1]) + 1;
	deliveryEndpoint = (char *)malloc(deliveryEndpointLength);
	deliveryEndpointStatus = mxGetString(argv[1], deliveryEndpoint, deliveryEndpointLength);

	HelicsError err = helicsErrorInitialize();

	helicsFilterAddDeliveryEndpoint(filt, (char const *)deliveryEndpoint, &err);

	mxArray *_out = (mxArray *)0;

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
	HelicsFilter filt = *(HelicsFilter*)(mxGetData(argv[0]));

	char *target;
	size_t targetLength;
	int targetStatus;
	targetLength = mxGetN(argv[1]) + 1;
	target = (char *)malloc(targetLength);
	targetStatus = mxGetString(argv[1], target, targetLength);

	HelicsError err = helicsErrorInitialize();

	helicsFilterRemoveTarget(filt, (char const *)target, &err);

	mxArray *_out = (mxArray *)0;

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
	HelicsFilter filt = *(HelicsFilter*)(mxGetData(argv[0]));

	char *deliveryEndpoint;
	size_t deliveryEndpointLength;
	int deliveryEndpointStatus;
	deliveryEndpointLength = mxGetN(argv[1]) + 1;
	deliveryEndpoint = (char *)malloc(deliveryEndpointLength);
	deliveryEndpointStatus = mxGetString(argv[1], deliveryEndpoint, deliveryEndpointLength);

	HelicsError err = helicsErrorInitialize();

	helicsFilterRemoveDeliveryEndpoint(filt, (char const *)deliveryEndpoint, &err);

	mxArray *_out = (mxArray *)0;

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
	HelicsFilter filt = *(HelicsFilter*)(mxGetData(argv[0]));

	const char *result = helicsFilterGetInfo(filt);

	mxArray *_out = mxCreateString(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsFilterSetInfo(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsFilter filt = *(HelicsFilter*)(mxGetData(argv[0]));

	char *info;
	size_t infoLength;
	int infoStatus;
	infoLength = mxGetN(argv[1]) + 1;
	info = (char *)malloc(infoLength);
	infoStatus = mxGetString(argv[1], info, infoLength);

	HelicsError err = helicsErrorInitialize();

	helicsFilterSetInfo(filt, (char const *)info, &err);

	mxArray *_out = (mxArray *)0;

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
	HelicsFilter filt = *(HelicsFilter*)(mxGetData(argv[0]));

	char *tagname;
	size_t tagnameLength;
	int tagnameStatus;
	tagnameLength = mxGetN(argv[1]) + 1;
	tagname = (char *)malloc(tagnameLength);
	tagnameStatus = mxGetString(argv[1], tagname, tagnameLength);

	const char *result = helicsFilterGetTag(filt, (char const *)tagname);

	mxArray *_out = mxCreateString(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}



	free(tagname);
}


void _wrap_helicsFilterSetTag(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsFilter filt = *(HelicsFilter*)(mxGetData(argv[0]));

	char *tagname;
	size_t tagnameLength;
	int tagnameStatus;
	tagnameLength = mxGetN(argv[1]) + 1;
	tagname = (char *)malloc(tagnameLength);
	tagnameStatus = mxGetString(argv[1], tagname, tagnameLength);

	char *tagvalue;
	size_t tagvalueLength;
	int tagvalueStatus;
	tagvalueLength = mxGetN(argv[2]) + 1;
	tagvalue = (char *)malloc(tagvalueLength);
	tagvalueStatus = mxGetString(argv[2], tagvalue, tagvalueLength);

	HelicsError err = helicsErrorInitialize();

	helicsFilterSetTag(filt, (char const *)tagname, (char const *)tagvalue, &err);

	mxArray *_out = (mxArray *)0;

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
	HelicsFilter filt = *(HelicsFilter*)(mxGetData(argv[0]));

	int option = (int)(mxGetScalar(argv[1]));

	int value = (int)(mxGetScalar(argv[2]));

	HelicsError err = helicsErrorInitialize();

	helicsFilterSetOption(filt, option, value, &err);

	mxArray *_out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}







	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFilterGetOption(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsFilter filt = *(HelicsFilter*)(mxGetData(argv[0]));

	int option = (int)(mxGetScalar(argv[1]));

	int result = helicsFilterGetOption(filt, option);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
	*((int64_t*)mxGetData(_out)) = (int64_t)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}




}


void _wrap_helicsFederateRegisterTranslator(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsFederate fed = *(HelicsFederate*)(mxGetData(argv[0]));

	int typeInt = static_cast<int>(mxGetScalar(argv[1]));
	HelicsTranslatorTypes type = static_cast<HelicsTranslatorTypes>(typeInt);

	char *name;
	size_t nameLength;
	int nameStatus;
	nameLength = mxGetN(argv[2]) + 1;
	name = (char *)malloc(nameLength);
	nameStatus = mxGetString(argv[2], name, nameLength);

	HelicsError err = helicsErrorInitialize();

	HelicsTranslator result = helicsFederateRegisterTranslator(fed, type, (char const *)name, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_t*)mxGetData(_out)) = (uint64_t)result;

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
	HelicsFederate fed = *(HelicsFederate*)(mxGetData(argv[0]));

	int typeInt = static_cast<int>(mxGetScalar(argv[1]));
	HelicsTranslatorTypes type = static_cast<HelicsTranslatorTypes>(typeInt);

	char *name;
	size_t nameLength;
	int nameStatus;
	nameLength = mxGetN(argv[2]) + 1;
	name = (char *)malloc(nameLength);
	nameStatus = mxGetString(argv[2], name, nameLength);

	HelicsError err = helicsErrorInitialize();

	HelicsTranslator result = helicsFederateRegisterGlobalTranslator(fed, type, (char const *)name, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_t*)mxGetData(_out)) = (uint64_t)result;

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
	HelicsCore core = *(HelicsCore*)(mxGetData(argv[0]));

	int typeInt = static_cast<int>(mxGetScalar(argv[1]));
	HelicsTranslatorTypes type = static_cast<HelicsTranslatorTypes>(typeInt);

	char *name;
	size_t nameLength;
	int nameStatus;
	nameLength = mxGetN(argv[2]) + 1;
	name = (char *)malloc(nameLength);
	nameStatus = mxGetString(argv[2], name, nameLength);

	HelicsError err = helicsErrorInitialize();

	HelicsTranslator result = helicsCoreRegisterTranslator(core, type, (char const *)name, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_t*)mxGetData(_out)) = (uint64_t)result;

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
	HelicsFederate fed = *(HelicsFederate*)(mxGetData(argv[0]));

	int result = helicsFederateGetTranslatorCount(fed);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
	*((int64_t*)mxGetData(_out)) = (int64_t)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsFederateGetTranslator(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsFederate fed = *(HelicsFederate*)(mxGetData(argv[0]));

	char *name;
	size_t nameLength;
	int nameStatus;
	nameLength = mxGetN(argv[1]) + 1;
	name = (char *)malloc(nameLength);
	nameStatus = mxGetString(argv[1], name, nameLength);

	HelicsError err = helicsErrorInitialize();

	HelicsTranslator result = helicsFederateGetTranslator(fed, (char const *)name, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_t*)mxGetData(_out)) = (uint64_t)result;

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
	HelicsFederate fed = *(HelicsFederate*)(mxGetData(argv[0]));

	int index = (int)(mxGetScalar(argv[1]));

	HelicsError err = helicsErrorInitialize();

	HelicsTranslator result = helicsFederateGetTranslatorByIndex(fed, index, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_t*)mxGetData(_out)) = (uint64_t)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}





	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsTranslatorIsValid(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsTranslator trans = *(HelicsTranslator*)(mxGetData(argv[0]));

	HelicsBool result = helicsTranslatorIsValid(trans);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT32_CLASS, mxREAL);
	*((int32_t*)mxGetData(_out)) = (int32_t)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsTranslatorGetName(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsTranslator trans = *(HelicsTranslator*)(mxGetData(argv[0]));

	const char *result = helicsTranslatorGetName(trans);

	mxArray *_out = mxCreateString(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsTranslatorSet(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsTranslator trans = *(HelicsTranslator*)(mxGetData(argv[0]));

	char *prop;
	size_t propLength;
	int propStatus;
	propLength = mxGetN(argv[1]) + 1;
	prop = (char *)malloc(propLength);
	propStatus = mxGetString(argv[1], prop, propLength);

	double val = mxGetScalar(argv[2]);

	HelicsError err = helicsErrorInitialize();

	helicsTranslatorSet(trans, (char const *)prop, val, &err);

	mxArray *_out = (mxArray *)0;

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
	HelicsTranslator trans = *(HelicsTranslator*)(mxGetData(argv[0]));

	char *prop;
	size_t propLength;
	int propStatus;
	propLength = mxGetN(argv[1]) + 1;
	prop = (char *)malloc(propLength);
	propStatus = mxGetString(argv[1], prop, propLength);

	char *val;
	size_t valLength;
	int valStatus;
	valLength = mxGetN(argv[2]) + 1;
	val = (char *)malloc(valLength);
	valStatus = mxGetString(argv[2], val, valLength);

	HelicsError err = helicsErrorInitialize();

	helicsTranslatorSetString(trans, (char const *)prop, (char const *)val, &err);

	mxArray *_out = (mxArray *)0;

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
	HelicsTranslator trans = *(HelicsTranslator*)(mxGetData(argv[0]));

	char *input;
	size_t inputLength;
	int inputStatus;
	inputLength = mxGetN(argv[1]) + 1;
	input = (char *)malloc(inputLength);
	inputStatus = mxGetString(argv[1], input, inputLength);

	HelicsError err = helicsErrorInitialize();

	helicsTranslatorAddInputTarget(trans, (char const *)input, &err);

	mxArray *_out = (mxArray *)0;

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
	HelicsTranslator trans = *(HelicsTranslator*)(mxGetData(argv[0]));

	char *pub;
	size_t pubLength;
	int pubStatus;
	pubLength = mxGetN(argv[1]) + 1;
	pub = (char *)malloc(pubLength);
	pubStatus = mxGetString(argv[1], pub, pubLength);

	HelicsError err = helicsErrorInitialize();

	helicsTranslatorAddPublicationTarget(trans, (char const *)pub, &err);

	mxArray *_out = (mxArray *)0;

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
	HelicsTranslator trans = *(HelicsTranslator*)(mxGetData(argv[0]));

	char *ept;
	size_t eptLength;
	int eptStatus;
	eptLength = mxGetN(argv[1]) + 1;
	ept = (char *)malloc(eptLength);
	eptStatus = mxGetString(argv[1], ept, eptLength);

	HelicsError err = helicsErrorInitialize();

	helicsTranslatorAddSourceEndpoint(trans, (char const *)ept, &err);

	mxArray *_out = (mxArray *)0;

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
	HelicsTranslator trans = *(HelicsTranslator*)(mxGetData(argv[0]));

	char *ept;
	size_t eptLength;
	int eptStatus;
	eptLength = mxGetN(argv[1]) + 1;
	ept = (char *)malloc(eptLength);
	eptStatus = mxGetString(argv[1], ept, eptLength);

	HelicsError err = helicsErrorInitialize();

	helicsTranslatorAddDestinationEndpoint(trans, (char const *)ept, &err);

	mxArray *_out = (mxArray *)0;

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
	HelicsTranslator trans = *(HelicsTranslator*)(mxGetData(argv[0]));

	char *target;
	size_t targetLength;
	int targetStatus;
	targetLength = mxGetN(argv[1]) + 1;
	target = (char *)malloc(targetLength);
	targetStatus = mxGetString(argv[1], target, targetLength);

	HelicsError err = helicsErrorInitialize();

	helicsTranslatorRemoveTarget(trans, (char const *)target, &err);

	mxArray *_out = (mxArray *)0;

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
	HelicsTranslator trans = *(HelicsTranslator*)(mxGetData(argv[0]));

	const char *result = helicsTranslatorGetInfo(trans);

	mxArray *_out = mxCreateString(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsTranslatorSetInfo(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsTranslator trans = *(HelicsTranslator*)(mxGetData(argv[0]));

	char *info;
	size_t infoLength;
	int infoStatus;
	infoLength = mxGetN(argv[1]) + 1;
	info = (char *)malloc(infoLength);
	infoStatus = mxGetString(argv[1], info, infoLength);

	HelicsError err = helicsErrorInitialize();

	helicsTranslatorSetInfo(trans, (char const *)info, &err);

	mxArray *_out = (mxArray *)0;

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
	HelicsTranslator trans = *(HelicsTranslator*)(mxGetData(argv[0]));

	char *tagname;
	size_t tagnameLength;
	int tagnameStatus;
	tagnameLength = mxGetN(argv[1]) + 1;
	tagname = (char *)malloc(tagnameLength);
	tagnameStatus = mxGetString(argv[1], tagname, tagnameLength);

	const char *result = helicsTranslatorGetTag(trans, (char const *)tagname);

	mxArray *_out = mxCreateString(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}



	free(tagname);
}


void _wrap_helicsTranslatorSetTag(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsTranslator trans = *(HelicsTranslator*)(mxGetData(argv[0]));

	char *tagname;
	size_t tagnameLength;
	int tagnameStatus;
	tagnameLength = mxGetN(argv[1]) + 1;
	tagname = (char *)malloc(tagnameLength);
	tagnameStatus = mxGetString(argv[1], tagname, tagnameLength);

	char *tagvalue;
	size_t tagvalueLength;
	int tagvalueStatus;
	tagvalueLength = mxGetN(argv[2]) + 1;
	tagvalue = (char *)malloc(tagvalueLength);
	tagvalueStatus = mxGetString(argv[2], tagvalue, tagvalueLength);

	HelicsError err = helicsErrorInitialize();

	helicsTranslatorSetTag(trans, (char const *)tagname, (char const *)tagvalue, &err);

	mxArray *_out = (mxArray *)0;

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
	HelicsTranslator trans = *(HelicsTranslator*)(mxGetData(argv[0]));

	int option = (int)(mxGetScalar(argv[1]));

	int value = (int)(mxGetScalar(argv[2]));

	HelicsError err = helicsErrorInitialize();

	helicsTranslatorSetOption(trans, option, value, &err);

	mxArray *_out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}







	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsTranslatorGetOption(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsTranslator trans = *(HelicsTranslator*)(mxGetData(argv[0]));

	int option = (int)(mxGetScalar(argv[1]));

	int result = helicsTranslatorGetOption(trans, option);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
	*((int64_t*)mxGetData(_out)) = (int64_t)result;

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
	*((int64_t*)mxGetData(rhs[1])) = (int64_t)loglevel;
	rhs[2] = mxCreateString(identifier);
	rhs[3] = mxCreateString(message);
	int status = mexCallMATLAB(0,&lhs,4,rhs,"feval");
	mxDestroyArray(lhs);
	mxDestroyArray(rhs[1]);
	mxDestroyArray(rhs[2]);
	mxDestroyArray(rhs[3]);
}

void _wrap_helicsBrokerSetLoggingCallback(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsBroker broker = *(HelicsBroker*)(mxGetData(argv[0]));

	void *userData = mxGetData(argv[1]);
	HelicsError err = helicsErrorInitialize();

	helicsBrokerSetLoggingCallback(broker, &matlabBrokerLoggingCallback, userData, &err);

	mxArray *_out = (mxArray *)0;
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
	*((int64_t*)mxGetData(rhs[1])) = (int64_t)loglevel;
	rhs[2] = mxCreateString(identifier);
	rhs[3] = mxCreateString(message);
	int status = mexCallMATLAB(0,&lhs,4,rhs,"feval");
	mxDestroyArray(lhs);
	mxDestroyArray(rhs[1]);
	mxDestroyArray(rhs[2]);
	mxDestroyArray(rhs[3]);
}

void _wrap_helicsCoreSetLoggingCallback(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsCore core = *(HelicsCore*)(mxGetData(argv[0]));

	void *userData = mxGetData(argv[1]);
	HelicsError err = helicsErrorInitialize();

	helicsCoreSetLoggingCallback(core, &matlabCoreLoggingCallback, userData, &err);

	mxArray *_out = (mxArray *)0;
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
	*((int64_t*)mxGetData(rhs[1])) = (int64_t)loglevel;
	rhs[2] = mxCreateString(identifier);
	rhs[3] = mxCreateString(message);
	int status = mexCallMATLAB(0,&lhs,4,rhs,"feval");
	mxDestroyArray(lhs);
	mxDestroyArray(rhs[1]);
	mxDestroyArray(rhs[2]);
	mxDestroyArray(rhs[3]);
}

void _wrap_helicsFederateSetLoggingCallback(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsFederate fed = *(HelicsFederate*)(mxGetData(argv[0]));

	void *userData = mxGetData(argv[1]);
	HelicsError err = helicsErrorInitialize();

	helicsFederateSetLoggingCallback(fed, &matlabFederateLoggingCallback, userData, &err);

	mxArray *_out = (mxArray *)0;
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
	*((uint64_t*)mxGetData(rhs[1])) = (uint64_t)message;
	int status = mexCallMATLAB(1,lhs,2,rhs,"feval");
	HelicsMessage rv = (HelicsMessage)(mxGetData(lhs[0]));
	mxDestroyArray(rhs[1]);
	return rv;
}

void _wrap_helicsFilterSetCustomCallback(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsFilter filter = *(HelicsFilter*)(mxGetData(argv[0]));

	void *userData = mxGetData(argv[1]);
	HelicsError err = helicsErrorInitialize();

	helicsFilterSetCustomCallback(filter, &matlabFilterCustomCallback, userData, &err);

	mxArray *_out = (mxArray *)0;
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
	*((uint64_t*)mxGetData(rhs[1])) = (uint64_t)value;

	rhs[2] = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
	*((int64_t*)mxGetData(rhs[2])) =  (int64_t)message;
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
	*((uint64_t*)mxGetData(rhs[1])) = (uint64_t)message;

	rhs[2] = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
	*((int64_t*)mxGetData(rhs[2])) =  (int64_t)value;
	int status = mexCallMATLAB(0,&lhs,3,rhs,"feval");
	mxDestroyArray(lhs);
	mxDestroyArray(rhs[1]);
	mxDestroyArray(rhs[2]);
}

void _wrap_helicsTranslatorSetCustomCallback(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsTranslator translator = *(HelicsTranslator*)(mxGetData(argv[0]));

	mxArray *callbacks[2];
	callbacks[0] = const_cast<mxArray *>(argv[1]);
	callbacks[1] = const_cast<mxArray *>(argv[2]);
	void *userData = reinterpret_cast<void *>(callbacks);
	HelicsError err = helicsErrorInitialize();

	helicsTranslatorSetCustomCallback(translator, &matlabToMessageCallCallback, &matlabToValueCallCallback, userData, &err);

	mxArray *_out = (mxArray *)0;
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
	mxChar *pQuery = (mxChar *)mxGetData(rhs[1]);
	for(int i=0; i<querySize; ++i){
		pQuery[i] = query[i];
	}
	rhs[2] = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
	*((int64_t*)mxGetData(rhs[2])) = (int64_t)querySize;
	rhs[3] = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_t*)mxGetData(rhs[3])) = (uint64_t)buffer;
	int status = mexCallMATLAB(0,&lhs,4,rhs,"feval");
	mxDestroyArray(rhs[1]);
	mxDestroyArray(rhs[2]);
	mxDestroyArray(rhs[3]);
}

void _wrap_helicsFederateSetQueryCallback(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsFilter filter = *(HelicsFilter*)(mxGetData(argv[0]));

	void *userData = mxGetData(argv[1]);
	HelicsError err = helicsErrorInitialize();

	helicsFederateSetQueryCallback(filter, &matlabFederateQueryCallback, userData, &err);

	mxArray *_out = (mxArray *)0;
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
	rhs[3] = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
	*((int64_t*)mxGetData(rhs[2])) =  (int64_t)iterating;
	int status = mexCallMATLAB(0,&lhs,4,rhs,"feval");
	mxDestroyArray(lhs);
	mxDestroyArray(rhs[1]);
	mxDestroyArray(rhs[2]);
	mxDestroyArray(rhs[3]);
}

void _wrap_helicsFederateSetTimeRequestEntryCallback(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsFederate fed = *(HelicsFederate*)(mxGetData(argv[0]));

	void *userData = mxGetData(argv[1]);
	HelicsError err = helicsErrorInitialize();

	helicsFederateSetTimeRequestEntryCallback(fed, &matlabFederateSetTimeRequestEntryCallback, userData, &err);

	mxArray *_out = (mxArray *)0;
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
	rhs[1] = mxCreateDoubleScalar((double)newTime);
	rhs[2] = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
	*((int64_t*)mxGetData(rhs[2])) =  (int64_t)iterating;
	int status = mexCallMATLAB(0,&lhs,3,rhs,"feval");
	mxDestroyArray(lhs);
	mxDestroyArray(rhs[1]);
	mxDestroyArray(rhs[2]);
}

void _wrap_helicsFederateSetTimeUpdateCallback(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsFederate fed = *(HelicsFederate*)(mxGetData(argv[0]));

	void *userData = mxGetData(argv[1]);
	HelicsError err = helicsErrorInitialize();

	helicsFederateSetTimeUpdateCallback(fed, &matlabFederateTimeUpdateCallback, userData, &err);

	mxArray *_out = (mxArray *)0;
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
	rhs[1] = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
	*((int64_t*)mxGetData(rhs[2])) =  (int64_t)newState;
	rhs[2] = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
	*((int64_t*)mxGetData(rhs[2])) =  (int64_t)oldState;
	int status = mexCallMATLAB(0,&lhs,3,rhs,"feval");
	mxDestroyArray(lhs);
	mxDestroyArray(rhs[1]);
	mxDestroyArray(rhs[2]);
}

void _wrap_helicsFederateSetStateChangeCallback(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsFederate fed = *(HelicsFederate*)(mxGetData(argv[0]));

	void *userData = mxGetData(argv[1]);
	HelicsError err = helicsErrorInitialize();

	helicsFederateSetStateChangeCallback(fed, &matlabFederateSetStateChangeCallback, userData, &err);

	mxArray *_out = (mxArray *)0;
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
	rhs[2] = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
	*((int64_t*)mxGetData(rhs[2])) =  (int64_t)iterating;
	int status = mexCallMATLAB(0,&lhs,3,rhs,"feval");
	mxDestroyArray(lhs);
	mxDestroyArray(rhs[1]);
	mxDestroyArray(rhs[2]);
}

void _wrap_helicsFederateSetTimeRequestReturnCallback(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsFederate fed = *(HelicsFederate*)(mxGetData(argv[0]));

	void *userData = mxGetData(argv[1]);
	HelicsError err = helicsErrorInitialize();

	helicsFederateSetTimeRequestReturnCallback(fed, &matlabFederateSetTimeRequestReturnCallback, userData, &err);

	mxArray *_out = (mxArray *)0;
	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsQueryBufferFill(int resc, mxArray *resv[], int argc, const mxArray *argv[]){
	HelicsQueryBuffer buffer = *(HelicsQueryBuffer*)(mxGetData(argv[0]));

	char *queryResult;
	size_t queryResultLength;
	int queryResultStatus;
	queryResultLength = mxGetN(argv[1]) + 1;
	queryResult = (char *)malloc(queryResultLength);
	queryResultStatus = mxGetString(argv[1], queryResult, queryResultLength);

	int strSize = static_cast<int>(queryResultLength) - 1;

	HelicsError err = helicsErrorInitialize();

	helicsQueryBufferFill(buffer, queryResult, strSize, &err);

	mxArray *_out = (mxArray *)0;
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
	case 2:
		_wrap_HELICS_DATA_TYPE_CHAR(resc, resv, argc, argv);
		break;
	case 66:
		_wrap_helicsCreateDataBuffer(resc, resv, argc, argv);
		break;
	case 67:
		_wrap_helicsDataBufferIsValid(resc, resv, argc, argv);
		break;
	case 68:
		_wrap_helicsWrapDataInBuffer(resc, resv, argc, argv);
		break;
	case 69:
		_wrap_helicsDataBufferFree(resc, resv, argc, argv);
		break;
	case 70:
		_wrap_helicsDataBufferSize(resc, resv, argc, argv);
		break;
	case 71:
		_wrap_helicsDataBufferCapacity(resc, resv, argc, argv);
		break;
	case 72:
		_wrap_helicsDataBufferData(resc, resv, argc, argv);
		break;
	case 73:
		_wrap_helicsDataBufferReserve(resc, resv, argc, argv);
		break;
	case 74:
		_wrap_helicsDataBufferClone(resc, resv, argc, argv);
		break;
	case 75:
		_wrap_helicsIntegerToBytes(resc, resv, argc, argv);
		break;
	case 76:
		_wrap_helicsDoubleToBytes(resc, resv, argc, argv);
		break;
	case 77:
		_wrap_helicsStringToBytes(resc, resv, argc, argv);
		break;
	case 78:
		_wrap_helicsRawStringToBytes(resc, resv, argc, argv);
		break;
	case 79:
		_wrap_helicsBooleanToBytes(resc, resv, argc, argv);
		break;
	case 80:
		_wrap_helicsCharToBytes(resc, resv, argc, argv);
		break;
	case 81:
		_wrap_helicsTimeToBytes(resc, resv, argc, argv);
		break;
	case 82:
		_wrap_helicsComplexToBytes(resc, resv, argc, argv);
		break;
	case 83:
		_wrap_helicsComplexObjectToBytes(resc, resv, argc, argv);
		break;
	case 84:
		_wrap_helicsVectorToBytes(resc, resv, argc, argv);
		break;
	case 85:
		_wrap_helicsNamedPointToBytes(resc, resv, argc, argv);
		break;
	case 86:
		_wrap_helicsComplexVectorToBytes(resc, resv, argc, argv);
		break;
	case 87:
		_wrap_helicsDataBufferType(resc, resv, argc, argv);
		break;
	case 88:
		_wrap_helicsDataBufferToInteger(resc, resv, argc, argv);
		break;
	case 89:
		_wrap_helicsDataBufferToDouble(resc, resv, argc, argv);
		break;
	case 90:
		_wrap_helicsDataBufferToBoolean(resc, resv, argc, argv);
		break;
	case 91:
		_wrap_helicsDataBufferToChar(resc, resv, argc, argv);
		break;
	case 92:
		_wrap_helicsDataBufferStringSize(resc, resv, argc, argv);
		break;
	case 93:
		_wrap_helicsDataBufferToString(resc, resv, argc, argv);
		break;
	case 94:
		_wrap_helicsDataBufferToRawString(resc, resv, argc, argv);
		break;
	case 95:
		_wrap_helicsDataBufferToTime(resc, resv, argc, argv);
		break;
	case 96:
		_wrap_helicsDataBufferToComplexObject(resc, resv, argc, argv);
		break;
	case 97:
		_wrap_helicsDataBufferToComplex(resc, resv, argc, argv);
		break;
	case 98:
		_wrap_helicsDataBufferVectorSize(resc, resv, argc, argv);
		break;
	case 99:
		_wrap_helicsDataBufferToVector(resc, resv, argc, argv);
		break;
	case 100:
		_wrap_helicsDataBufferToComplexVector(resc, resv, argc, argv);
		break;
	case 101:
		_wrap_helicsDataBufferToNamedPoint(resc, resv, argc, argv);
		break;
	case 102:
		_wrap_helicsDataBufferConvertToType(resc, resv, argc, argv);
		break;
	case 103:
		_wrap_helicsGetVersion(resc, resv, argc, argv);
		break;
	case 104:
		_wrap_helicsGetBuildFlags(resc, resv, argc, argv);
		break;
	case 105:
		_wrap_helicsGetCompilerVersion(resc, resv, argc, argv);
		break;
	case 106:
		_wrap_helicsGetSystemInfo(resc, resv, argc, argv);
		break;
	case 109:
		_wrap_helicsLoadSignalHandler(resc, resv, argc, argv);
		break;
	case 110:
		_wrap_helicsLoadThreadedSignalHandler(resc, resv, argc, argv);
		break;
	case 111:
		_wrap_helicsClearSignalHandler(resc, resv, argc, argv);
		break;
	case 114:
		_wrap_helicsAbort(resc, resv, argc, argv);
		break;
	case 115:
		_wrap_helicsIsCoreTypeAvailable(resc, resv, argc, argv);
		break;
	case 116:
		_wrap_helicsCreateCore(resc, resv, argc, argv);
		break;
	case 117:
		_wrap_helicsCreateCoreFromArgs(resc, resv, argc, argv);
		break;
	case 118:
		_wrap_helicsCoreClone(resc, resv, argc, argv);
		break;
	case 119:
		_wrap_helicsCoreIsValid(resc, resv, argc, argv);
		break;
	case 120:
		_wrap_helicsCreateBroker(resc, resv, argc, argv);
		break;
	case 121:
		_wrap_helicsCreateBrokerFromArgs(resc, resv, argc, argv);
		break;
	case 122:
		_wrap_helicsBrokerClone(resc, resv, argc, argv);
		break;
	case 123:
		_wrap_helicsBrokerIsValid(resc, resv, argc, argv);
		break;
	case 124:
		_wrap_helicsBrokerIsConnected(resc, resv, argc, argv);
		break;
	case 125:
		_wrap_helicsBrokerDataLink(resc, resv, argc, argv);
		break;
	case 126:
		_wrap_helicsBrokerAddSourceFilterToEndpoint(resc, resv, argc, argv);
		break;
	case 127:
		_wrap_helicsBrokerAddDestinationFilterToEndpoint(resc, resv, argc, argv);
		break;
	case 128:
		_wrap_helicsBrokerMakeConnections(resc, resv, argc, argv);
		break;
	case 129:
		_wrap_helicsCoreWaitForDisconnect(resc, resv, argc, argv);
		break;
	case 130:
		_wrap_helicsBrokerWaitForDisconnect(resc, resv, argc, argv);
		break;
	case 131:
		_wrap_helicsCoreIsConnected(resc, resv, argc, argv);
		break;
	case 132:
		_wrap_helicsCoreDataLink(resc, resv, argc, argv);
		break;
	case 133:
		_wrap_helicsCoreAddSourceFilterToEndpoint(resc, resv, argc, argv);
		break;
	case 134:
		_wrap_helicsCoreAddDestinationFilterToEndpoint(resc, resv, argc, argv);
		break;
	case 135:
		_wrap_helicsCoreMakeConnections(resc, resv, argc, argv);
		break;
	case 136:
		_wrap_helicsBrokerGetIdentifier(resc, resv, argc, argv);
		break;
	case 137:
		_wrap_helicsCoreGetIdentifier(resc, resv, argc, argv);
		break;
	case 138:
		_wrap_helicsBrokerGetAddress(resc, resv, argc, argv);
		break;
	case 139:
		_wrap_helicsCoreGetAddress(resc, resv, argc, argv);
		break;
	case 140:
		_wrap_helicsCoreSetReadyToInit(resc, resv, argc, argv);
		break;
	case 141:
		_wrap_helicsCoreConnect(resc, resv, argc, argv);
		break;
	case 142:
		_wrap_helicsCoreDisconnect(resc, resv, argc, argv);
		break;
	case 143:
		_wrap_helicsGetFederateByName(resc, resv, argc, argv);
		break;
	case 144:
		_wrap_helicsBrokerDisconnect(resc, resv, argc, argv);
		break;
	case 145:
		_wrap_helicsFederateDestroy(resc, resv, argc, argv);
		break;
	case 146:
		_wrap_helicsBrokerDestroy(resc, resv, argc, argv);
		break;
	case 147:
		_wrap_helicsCoreDestroy(resc, resv, argc, argv);
		break;
	case 148:
		_wrap_helicsCoreFree(resc, resv, argc, argv);
		break;
	case 149:
		_wrap_helicsBrokerFree(resc, resv, argc, argv);
		break;
	case 150:
		_wrap_helicsCreateValueFederate(resc, resv, argc, argv);
		break;
	case 151:
		_wrap_helicsCreateValueFederateFromConfig(resc, resv, argc, argv);
		break;
	case 152:
		_wrap_helicsCreateMessageFederate(resc, resv, argc, argv);
		break;
	case 153:
		_wrap_helicsCreateMessageFederateFromConfig(resc, resv, argc, argv);
		break;
	case 154:
		_wrap_helicsCreateCombinationFederate(resc, resv, argc, argv);
		break;
	case 155:
		_wrap_helicsCreateCombinationFederateFromConfig(resc, resv, argc, argv);
		break;
	case 156:
		_wrap_helicsFederateClone(resc, resv, argc, argv);
		break;
	case 157:
		_wrap_helicsFederateProtect(resc, resv, argc, argv);
		break;
	case 158:
		_wrap_helicsFederateUnProtect(resc, resv, argc, argv);
		break;
	case 159:
		_wrap_helicsFederateIsProtected(resc, resv, argc, argv);
		break;
	case 160:
		_wrap_helicsCreateFederateInfo(resc, resv, argc, argv);
		break;
	case 161:
		_wrap_helicsFederateInfoClone(resc, resv, argc, argv);
		break;
	case 162:
		_wrap_helicsFederateInfoLoadFromArgs(resc, resv, argc, argv);
		break;
	case 163:
		_wrap_helicsFederateInfoLoadFromString(resc, resv, argc, argv);
		break;
	case 164:
		_wrap_helicsFederateInfoFree(resc, resv, argc, argv);
		break;
	case 165:
		_wrap_helicsFederateIsValid(resc, resv, argc, argv);
		break;
	case 166:
		_wrap_helicsFederateInfoSetCoreName(resc, resv, argc, argv);
		break;
	case 167:
		_wrap_helicsFederateInfoSetCoreInitString(resc, resv, argc, argv);
		break;
	case 168:
		_wrap_helicsFederateInfoSetBrokerInitString(resc, resv, argc, argv);
		break;
	case 169:
		_wrap_helicsFederateInfoSetCoreType(resc, resv, argc, argv);
		break;
	case 170:
		_wrap_helicsFederateInfoSetCoreTypeFromString(resc, resv, argc, argv);
		break;
	case 171:
		_wrap_helicsFederateInfoSetBroker(resc, resv, argc, argv);
		break;
	case 172:
		_wrap_helicsFederateInfoSetBrokerKey(resc, resv, argc, argv);
		break;
	case 173:
		_wrap_helicsFederateInfoSetBrokerPort(resc, resv, argc, argv);
		break;
	case 174:
		_wrap_helicsFederateInfoSetLocalPort(resc, resv, argc, argv);
		break;
	case 175:
		_wrap_helicsGetPropertyIndex(resc, resv, argc, argv);
		break;
	case 176:
		_wrap_helicsGetFlagIndex(resc, resv, argc, argv);
		break;
	case 177:
		_wrap_helicsGetOptionIndex(resc, resv, argc, argv);
		break;
	case 178:
		_wrap_helicsGetOptionValue(resc, resv, argc, argv);
		break;
	case 179:
		_wrap_helicsGetDataType(resc, resv, argc, argv);
		break;
	case 180:
		_wrap_helicsFederateInfoSetFlagOption(resc, resv, argc, argv);
		break;
	case 181:
		_wrap_helicsFederateInfoSetSeparator(resc, resv, argc, argv);
		break;
	case 182:
		_wrap_helicsFederateInfoSetTimeProperty(resc, resv, argc, argv);
		break;
	case 183:
		_wrap_helicsFederateInfoSetIntegerProperty(resc, resv, argc, argv);
		break;
	case 184:
		_wrap_helicsFederateRegisterInterfaces(resc, resv, argc, argv);
		break;
	case 185:
		_wrap_helicsFederateGlobalError(resc, resv, argc, argv);
		break;
	case 186:
		_wrap_helicsFederateLocalError(resc, resv, argc, argv);
		break;
	case 187:
		_wrap_helicsFederateFinalize(resc, resv, argc, argv);
		break;
	case 188:
		_wrap_helicsFederateFinalizeAsync(resc, resv, argc, argv);
		break;
	case 189:
		_wrap_helicsFederateFinalizeComplete(resc, resv, argc, argv);
		break;
	case 190:
		_wrap_helicsFederateDisconnect(resc, resv, argc, argv);
		break;
	case 191:
		_wrap_helicsFederateDisconnectAsync(resc, resv, argc, argv);
		break;
	case 192:
		_wrap_helicsFederateDisconnectComplete(resc, resv, argc, argv);
		break;
	case 193:
		_wrap_helicsFederateFree(resc, resv, argc, argv);
		break;
	case 194:
		_wrap_helicsCloseLibrary(resc, resv, argc, argv);
		break;
	case 195:
		_wrap_helicsFederateEnterInitializingMode(resc, resv, argc, argv);
		break;
	case 196:
		_wrap_helicsFederateEnterInitializingModeAsync(resc, resv, argc, argv);
		break;
	case 197:
		_wrap_helicsFederateIsAsyncOperationCompleted(resc, resv, argc, argv);
		break;
	case 198:
		_wrap_helicsFederateEnterInitializingModeComplete(resc, resv, argc, argv);
		break;
	case 199:
		_wrap_helicsFederateEnterExecutingMode(resc, resv, argc, argv);
		break;
	case 200:
		_wrap_helicsFederateEnterExecutingModeAsync(resc, resv, argc, argv);
		break;
	case 201:
		_wrap_helicsFederateEnterExecutingModeComplete(resc, resv, argc, argv);
		break;
	case 202:
		_wrap_helicsFederateEnterExecutingModeIterative(resc, resv, argc, argv);
		break;
	case 203:
		_wrap_helicsFederateEnterExecutingModeIterativeAsync(resc, resv, argc, argv);
		break;
	case 204:
		_wrap_helicsFederateEnterExecutingModeIterativeComplete(resc, resv, argc, argv);
		break;
	case 205:
		_wrap_helicsFederateGetState(resc, resv, argc, argv);
		break;
	case 206:
		_wrap_helicsFederateGetCore(resc, resv, argc, argv);
		break;
	case 207:
		_wrap_helicsFederateRequestTime(resc, resv, argc, argv);
		break;
	case 208:
		_wrap_helicsFederateRequestTimeAdvance(resc, resv, argc, argv);
		break;
	case 209:
		_wrap_helicsFederateRequestNextStep(resc, resv, argc, argv);
		break;
	case 210:
		_wrap_helicsFederateRequestTimeIterative(resc, resv, argc, argv);
		break;
	case 211:
		_wrap_helicsFederateRequestTimeAsync(resc, resv, argc, argv);
		break;
	case 212:
		_wrap_helicsFederateRequestTimeComplete(resc, resv, argc, argv);
		break;
	case 213:
		_wrap_helicsFederateRequestTimeIterativeAsync(resc, resv, argc, argv);
		break;
	case 214:
		_wrap_helicsFederateRequestTimeIterativeComplete(resc, resv, argc, argv);
		break;
	case 215:
		_wrap_helicsFederateProcessCommunications(resc, resv, argc, argv);
		break;
	case 216:
		_wrap_helicsFederateGetName(resc, resv, argc, argv);
		break;
	case 217:
		_wrap_helicsFederateSetTimeProperty(resc, resv, argc, argv);
		break;
	case 218:
		_wrap_helicsFederateSetFlagOption(resc, resv, argc, argv);
		break;
	case 219:
		_wrap_helicsFederateSetSeparator(resc, resv, argc, argv);
		break;
	case 220:
		_wrap_helicsFederateSetIntegerProperty(resc, resv, argc, argv);
		break;
	case 221:
		_wrap_helicsFederateGetTimeProperty(resc, resv, argc, argv);
		break;
	case 222:
		_wrap_helicsFederateGetFlagOption(resc, resv, argc, argv);
		break;
	case 223:
		_wrap_helicsFederateGetIntegerProperty(resc, resv, argc, argv);
		break;
	case 224:
		_wrap_helicsFederateGetCurrentTime(resc, resv, argc, argv);
		break;
	case 225:
		_wrap_helicsFederateSetGlobal(resc, resv, argc, argv);
		break;
	case 226:
		_wrap_helicsFederateSetTag(resc, resv, argc, argv);
		break;
	case 227:
		_wrap_helicsFederateGetTag(resc, resv, argc, argv);
		break;
	case 228:
		_wrap_helicsFederateAddDependency(resc, resv, argc, argv);
		break;
	case 229:
		_wrap_helicsFederateSetLogFile(resc, resv, argc, argv);
		break;
	case 230:
		_wrap_helicsFederateLogErrorMessage(resc, resv, argc, argv);
		break;
	case 231:
		_wrap_helicsFederateLogWarningMessage(resc, resv, argc, argv);
		break;
	case 232:
		_wrap_helicsFederateLogInfoMessage(resc, resv, argc, argv);
		break;
	case 233:
		_wrap_helicsFederateLogDebugMessage(resc, resv, argc, argv);
		break;
	case 234:
		_wrap_helicsFederateLogLevelMessage(resc, resv, argc, argv);
		break;
	case 235:
		_wrap_helicsFederateSendCommand(resc, resv, argc, argv);
		break;
	case 236:
		_wrap_helicsFederateGetCommand(resc, resv, argc, argv);
		break;
	case 237:
		_wrap_helicsFederateGetCommandSource(resc, resv, argc, argv);
		break;
	case 238:
		_wrap_helicsFederateWaitCommand(resc, resv, argc, argv);
		break;
	case 239:
		_wrap_helicsCoreSetGlobal(resc, resv, argc, argv);
		break;
	case 240:
		_wrap_helicsBrokerSetGlobal(resc, resv, argc, argv);
		break;
	case 241:
		_wrap_helicsCoreSendCommand(resc, resv, argc, argv);
		break;
	case 242:
		_wrap_helicsCoreSendOrderedCommand(resc, resv, argc, argv);
		break;
	case 243:
		_wrap_helicsBrokerSendCommand(resc, resv, argc, argv);
		break;
	case 244:
		_wrap_helicsBrokerSendOrderedCommand(resc, resv, argc, argv);
		break;
	case 245:
		_wrap_helicsCoreSetLogFile(resc, resv, argc, argv);
		break;
	case 246:
		_wrap_helicsBrokerSetLogFile(resc, resv, argc, argv);
		break;
	case 247:
		_wrap_helicsBrokerSetTimeBarrier(resc, resv, argc, argv);
		break;
	case 248:
		_wrap_helicsBrokerClearTimeBarrier(resc, resv, argc, argv);
		break;
	case 249:
		_wrap_helicsBrokerGlobalError(resc, resv, argc, argv);
		break;
	case 250:
		_wrap_helicsCoreGlobalError(resc, resv, argc, argv);
		break;
	case 251:
		_wrap_helicsCreateQuery(resc, resv, argc, argv);
		break;
	case 252:
		_wrap_helicsQueryExecute(resc, resv, argc, argv);
		break;
	case 253:
		_wrap_helicsQueryCoreExecute(resc, resv, argc, argv);
		break;
	case 254:
		_wrap_helicsQueryBrokerExecute(resc, resv, argc, argv);
		break;
	case 255:
		_wrap_helicsQueryExecuteAsync(resc, resv, argc, argv);
		break;
	case 256:
		_wrap_helicsQueryExecuteComplete(resc, resv, argc, argv);
		break;
	case 257:
		_wrap_helicsQueryIsCompleted(resc, resv, argc, argv);
		break;
	case 258:
		_wrap_helicsQuerySetTarget(resc, resv, argc, argv);
		break;
	case 259:
		_wrap_helicsQuerySetQueryString(resc, resv, argc, argv);
		break;
	case 260:
		_wrap_helicsQuerySetOrdering(resc, resv, argc, argv);
		break;
	case 261:
		_wrap_helicsQueryFree(resc, resv, argc, argv);
		break;
	case 262:
		_wrap_helicsCleanupLibrary(resc, resv, argc, argv);
		break;
	case 263:
		_wrap_helicsFederateRegisterSubscription(resc, resv, argc, argv);
		break;
	case 264:
		_wrap_helicsFederateRegisterPublication(resc, resv, argc, argv);
		break;
	case 265:
		_wrap_helicsFederateRegisterTypePublication(resc, resv, argc, argv);
		break;
	case 266:
		_wrap_helicsFederateRegisterGlobalPublication(resc, resv, argc, argv);
		break;
	case 267:
		_wrap_helicsFederateRegisterGlobalTypePublication(resc, resv, argc, argv);
		break;
	case 268:
		_wrap_helicsFederateRegisterInput(resc, resv, argc, argv);
		break;
	case 269:
		_wrap_helicsFederateRegisterTypeInput(resc, resv, argc, argv);
		break;
	case 270:
		_wrap_helicsFederateRegisterGlobalInput(resc, resv, argc, argv);
		break;
	case 271:
		_wrap_helicsFederateRegisterGlobalTypeInput(resc, resv, argc, argv);
		break;
	case 272:
		_wrap_helicsFederateGetPublication(resc, resv, argc, argv);
		break;
	case 273:
		_wrap_helicsFederateGetPublicationByIndex(resc, resv, argc, argv);
		break;
	case 274:
		_wrap_helicsFederateGetInput(resc, resv, argc, argv);
		break;
	case 275:
		_wrap_helicsFederateGetInputByIndex(resc, resv, argc, argv);
		break;
	case 276:
		_wrap_helicsFederateGetSubscription(resc, resv, argc, argv);
		break;
	case 277:
		_wrap_helicsFederateClearUpdates(resc, resv, argc, argv);
		break;
	case 278:
		_wrap_helicsFederateRegisterFromPublicationJSON(resc, resv, argc, argv);
		break;
	case 279:
		_wrap_helicsFederatePublishJSON(resc, resv, argc, argv);
		break;
	case 280:
		_wrap_helicsPublicationIsValid(resc, resv, argc, argv);
		break;
	case 281:
		_wrap_helicsPublicationPublishBytes(resc, resv, argc, argv);
		break;
	case 282:
		_wrap_helicsPublicationPublishString(resc, resv, argc, argv);
		break;
	case 283:
		_wrap_helicsPublicationPublishInteger(resc, resv, argc, argv);
		break;
	case 284:
		_wrap_helicsPublicationPublishBoolean(resc, resv, argc, argv);
		break;
	case 285:
		_wrap_helicsPublicationPublishDouble(resc, resv, argc, argv);
		break;
	case 286:
		_wrap_helicsPublicationPublishTime(resc, resv, argc, argv);
		break;
	case 287:
		_wrap_helicsPublicationPublishChar(resc, resv, argc, argv);
		break;
	case 288:
		_wrap_helicsPublicationPublishComplex(resc, resv, argc, argv);
		break;
	case 289:
		_wrap_helicsPublicationPublishVector(resc, resv, argc, argv);
		break;
	case 290:
		_wrap_helicsPublicationPublishComplexVector(resc, resv, argc, argv);
		break;
	case 291:
		_wrap_helicsPublicationPublishNamedPoint(resc, resv, argc, argv);
		break;
	case 292:
		_wrap_helicsPublicationAddTarget(resc, resv, argc, argv);
		break;
	case 293:
		_wrap_helicsInputIsValid(resc, resv, argc, argv);
		break;
	case 294:
		_wrap_helicsInputAddTarget(resc, resv, argc, argv);
		break;
	case 295:
		_wrap_helicsInputGetByteCount(resc, resv, argc, argv);
		break;
	case 296:
		_wrap_helicsInputGetBytes(resc, resv, argc, argv);
		break;
	case 297:
		_wrap_helicsInputGetStringSize(resc, resv, argc, argv);
		break;
	case 298:
		_wrap_helicsInputGetString(resc, resv, argc, argv);
		break;
	case 299:
		_wrap_helicsInputGetInteger(resc, resv, argc, argv);
		break;
	case 300:
		_wrap_helicsInputGetBoolean(resc, resv, argc, argv);
		break;
	case 301:
		_wrap_helicsInputGetDouble(resc, resv, argc, argv);
		break;
	case 302:
		_wrap_helicsInputGetTime(resc, resv, argc, argv);
		break;
	case 303:
		_wrap_helicsInputGetChar(resc, resv, argc, argv);
		break;
	case 304:
		_wrap_helicsInputGetComplexObject(resc, resv, argc, argv);
		break;
	case 305:
		_wrap_helicsInputGetComplex(resc, resv, argc, argv);
		break;
	case 306:
		_wrap_helicsInputGetVectorSize(resc, resv, argc, argv);
		break;
	case 307:
		_wrap_helicsInputGetVector(resc, resv, argc, argv);
		break;
	case 308:
		_wrap_helicsInputGetComplexVector(resc, resv, argc, argv);
		break;
	case 309:
		_wrap_helicsInputGetNamedPoint(resc, resv, argc, argv);
		break;
	case 310:
		_wrap_helicsInputSetDefaultBytes(resc, resv, argc, argv);
		break;
	case 311:
		_wrap_helicsInputSetDefaultString(resc, resv, argc, argv);
		break;
	case 312:
		_wrap_helicsInputSetDefaultInteger(resc, resv, argc, argv);
		break;
	case 313:
		_wrap_helicsInputSetDefaultBoolean(resc, resv, argc, argv);
		break;
	case 314:
		_wrap_helicsInputSetDefaultTime(resc, resv, argc, argv);
		break;
	case 315:
		_wrap_helicsInputSetDefaultChar(resc, resv, argc, argv);
		break;
	case 316:
		_wrap_helicsInputSetDefaultDouble(resc, resv, argc, argv);
		break;
	case 317:
		_wrap_helicsInputSetDefaultComplex(resc, resv, argc, argv);
		break;
	case 318:
		_wrap_helicsInputSetDefaultVector(resc, resv, argc, argv);
		break;
	case 319:
		_wrap_helicsInputSetDefaultComplexVector(resc, resv, argc, argv);
		break;
	case 320:
		_wrap_helicsInputSetDefaultNamedPoint(resc, resv, argc, argv);
		break;
	case 321:
		_wrap_helicsInputGetType(resc, resv, argc, argv);
		break;
	case 322:
		_wrap_helicsInputGetPublicationType(resc, resv, argc, argv);
		break;
	case 323:
		_wrap_helicsInputGetPublicationDataType(resc, resv, argc, argv);
		break;
	case 324:
		_wrap_helicsPublicationGetType(resc, resv, argc, argv);
		break;
	case 325:
		_wrap_helicsInputGetName(resc, resv, argc, argv);
		break;
	case 326:
		_wrap_helicsSubscriptionGetTarget(resc, resv, argc, argv);
		break;
	case 327:
		_wrap_helicsPublicationGetName(resc, resv, argc, argv);
		break;
	case 328:
		_wrap_helicsInputGetUnits(resc, resv, argc, argv);
		break;
	case 329:
		_wrap_helicsInputGetInjectionUnits(resc, resv, argc, argv);
		break;
	case 330:
		_wrap_helicsInputGetExtractionUnits(resc, resv, argc, argv);
		break;
	case 331:
		_wrap_helicsPublicationGetUnits(resc, resv, argc, argv);
		break;
	case 332:
		_wrap_helicsInputGetInfo(resc, resv, argc, argv);
		break;
	case 333:
		_wrap_helicsInputSetInfo(resc, resv, argc, argv);
		break;
	case 334:
		_wrap_helicsInputGetTag(resc, resv, argc, argv);
		break;
	case 335:
		_wrap_helicsInputSetTag(resc, resv, argc, argv);
		break;
	case 336:
		_wrap_helicsPublicationGetInfo(resc, resv, argc, argv);
		break;
	case 337:
		_wrap_helicsPublicationSetInfo(resc, resv, argc, argv);
		break;
	case 338:
		_wrap_helicsPublicationGetTag(resc, resv, argc, argv);
		break;
	case 339:
		_wrap_helicsPublicationSetTag(resc, resv, argc, argv);
		break;
	case 340:
		_wrap_helicsInputGetOption(resc, resv, argc, argv);
		break;
	case 341:
		_wrap_helicsInputSetOption(resc, resv, argc, argv);
		break;
	case 342:
		_wrap_helicsPublicationGetOption(resc, resv, argc, argv);
		break;
	case 343:
		_wrap_helicsPublicationSetOption(resc, resv, argc, argv);
		break;
	case 344:
		_wrap_helicsPublicationSetMinimumChange(resc, resv, argc, argv);
		break;
	case 345:
		_wrap_helicsInputSetMinimumChange(resc, resv, argc, argv);
		break;
	case 346:
		_wrap_helicsInputIsUpdated(resc, resv, argc, argv);
		break;
	case 347:
		_wrap_helicsInputLastUpdateTime(resc, resv, argc, argv);
		break;
	case 348:
		_wrap_helicsInputClearUpdate(resc, resv, argc, argv);
		break;
	case 349:
		_wrap_helicsFederateGetPublicationCount(resc, resv, argc, argv);
		break;
	case 350:
		_wrap_helicsFederateGetInputCount(resc, resv, argc, argv);
		break;
	case 351:
		_wrap_helicsFederateRegisterEndpoint(resc, resv, argc, argv);
		break;
	case 352:
		_wrap_helicsFederateRegisterGlobalEndpoint(resc, resv, argc, argv);
		break;
	case 353:
		_wrap_helicsFederateRegisterTargetedEndpoint(resc, resv, argc, argv);
		break;
	case 354:
		_wrap_helicsFederateRegisterGlobalTargetedEndpoint(resc, resv, argc, argv);
		break;
	case 355:
		_wrap_helicsFederateGetEndpoint(resc, resv, argc, argv);
		break;
	case 356:
		_wrap_helicsFederateGetEndpointByIndex(resc, resv, argc, argv);
		break;
	case 357:
		_wrap_helicsEndpointIsValid(resc, resv, argc, argv);
		break;
	case 358:
		_wrap_helicsEndpointSetDefaultDestination(resc, resv, argc, argv);
		break;
	case 359:
		_wrap_helicsEndpointGetDefaultDestination(resc, resv, argc, argv);
		break;
	case 360:
		_wrap_helicsEndpointSendBytes(resc, resv, argc, argv);
		break;
	case 361:
		_wrap_helicsEndpointSendBytesTo(resc, resv, argc, argv);
		break;
	case 362:
		_wrap_helicsEndpointSendBytesToAt(resc, resv, argc, argv);
		break;
	case 363:
		_wrap_helicsEndpointSendBytesAt(resc, resv, argc, argv);
		break;
	case 364:
		_wrap_helicsEndpointSendMessage(resc, resv, argc, argv);
		break;
	case 365:
		_wrap_helicsEndpointSendMessageZeroCopy(resc, resv, argc, argv);
		break;
	case 366:
		_wrap_helicsEndpointSubscribe(resc, resv, argc, argv);
		break;
	case 367:
		_wrap_helicsFederateHasMessage(resc, resv, argc, argv);
		break;
	case 368:
		_wrap_helicsEndpointHasMessage(resc, resv, argc, argv);
		break;
	case 369:
		_wrap_helicsFederatePendingMessageCount(resc, resv, argc, argv);
		break;
	case 370:
		_wrap_helicsEndpointPendingMessageCount(resc, resv, argc, argv);
		break;
	case 371:
		_wrap_helicsEndpointGetMessage(resc, resv, argc, argv);
		break;
	case 372:
		_wrap_helicsEndpointCreateMessage(resc, resv, argc, argv);
		break;
	case 373:
		_wrap_helicsEndpointClearMessages(resc, resv, argc, argv);
		break;
	case 374:
		_wrap_helicsFederateGetMessage(resc, resv, argc, argv);
		break;
	case 375:
		_wrap_helicsFederateCreateMessage(resc, resv, argc, argv);
		break;
	case 376:
		_wrap_helicsFederateClearMessages(resc, resv, argc, argv);
		break;
	case 377:
		_wrap_helicsEndpointGetType(resc, resv, argc, argv);
		break;
	case 378:
		_wrap_helicsEndpointGetName(resc, resv, argc, argv);
		break;
	case 379:
		_wrap_helicsFederateGetEndpointCount(resc, resv, argc, argv);
		break;
	case 380:
		_wrap_helicsEndpointGetInfo(resc, resv, argc, argv);
		break;
	case 381:
		_wrap_helicsEndpointSetInfo(resc, resv, argc, argv);
		break;
	case 382:
		_wrap_helicsEndpointGetTag(resc, resv, argc, argv);
		break;
	case 383:
		_wrap_helicsEndpointSetTag(resc, resv, argc, argv);
		break;
	case 384:
		_wrap_helicsEndpointSetOption(resc, resv, argc, argv);
		break;
	case 385:
		_wrap_helicsEndpointGetOption(resc, resv, argc, argv);
		break;
	case 386:
		_wrap_helicsEndpointAddSourceTarget(resc, resv, argc, argv);
		break;
	case 387:
		_wrap_helicsEndpointAddDestinationTarget(resc, resv, argc, argv);
		break;
	case 388:
		_wrap_helicsEndpointRemoveTarget(resc, resv, argc, argv);
		break;
	case 389:
		_wrap_helicsEndpointAddSourceFilter(resc, resv, argc, argv);
		break;
	case 390:
		_wrap_helicsEndpointAddDestinationFilter(resc, resv, argc, argv);
		break;
	case 391:
		_wrap_helicsMessageGetSource(resc, resv, argc, argv);
		break;
	case 392:
		_wrap_helicsMessageGetDestination(resc, resv, argc, argv);
		break;
	case 393:
		_wrap_helicsMessageGetOriginalSource(resc, resv, argc, argv);
		break;
	case 394:
		_wrap_helicsMessageGetOriginalDestination(resc, resv, argc, argv);
		break;
	case 395:
		_wrap_helicsMessageGetTime(resc, resv, argc, argv);
		break;
	case 396:
		_wrap_helicsMessageGetString(resc, resv, argc, argv);
		break;
	case 397:
		_wrap_helicsMessageGetMessageID(resc, resv, argc, argv);
		break;
	case 398:
		_wrap_helicsMessageGetFlagOption(resc, resv, argc, argv);
		break;
	case 399:
		_wrap_helicsMessageGetByteCount(resc, resv, argc, argv);
		break;
	case 400:
		_wrap_helicsMessageGetBytes(resc, resv, argc, argv);
		break;
	case 401:
		_wrap_helicsMessageGetBytesPointer(resc, resv, argc, argv);
		break;
	case 402:
		_wrap_helicsMessageIsValid(resc, resv, argc, argv);
		break;
	case 403:
		_wrap_helicsMessageSetSource(resc, resv, argc, argv);
		break;
	case 404:
		_wrap_helicsMessageSetDestination(resc, resv, argc, argv);
		break;
	case 405:
		_wrap_helicsMessageSetOriginalSource(resc, resv, argc, argv);
		break;
	case 406:
		_wrap_helicsMessageSetOriginalDestination(resc, resv, argc, argv);
		break;
	case 407:
		_wrap_helicsMessageSetTime(resc, resv, argc, argv);
		break;
	case 408:
		_wrap_helicsMessageResize(resc, resv, argc, argv);
		break;
	case 409:
		_wrap_helicsMessageReserve(resc, resv, argc, argv);
		break;
	case 410:
		_wrap_helicsMessageSetMessageID(resc, resv, argc, argv);
		break;
	case 411:
		_wrap_helicsMessageClearFlags(resc, resv, argc, argv);
		break;
	case 412:
		_wrap_helicsMessageSetFlagOption(resc, resv, argc, argv);
		break;
	case 413:
		_wrap_helicsMessageSetString(resc, resv, argc, argv);
		break;
	case 414:
		_wrap_helicsMessageSetData(resc, resv, argc, argv);
		break;
	case 415:
		_wrap_helicsMessageAppendData(resc, resv, argc, argv);
		break;
	case 416:
		_wrap_helicsMessageCopy(resc, resv, argc, argv);
		break;
	case 417:
		_wrap_helicsMessageClone(resc, resv, argc, argv);
		break;
	case 418:
		_wrap_helicsMessageFree(resc, resv, argc, argv);
		break;
	case 419:
		_wrap_helicsMessageClear(resc, resv, argc, argv);
		break;
	case 420:
		_wrap_helicsFederateRegisterFilter(resc, resv, argc, argv);
		break;
	case 421:
		_wrap_helicsFederateRegisterGlobalFilter(resc, resv, argc, argv);
		break;
	case 422:
		_wrap_helicsFederateRegisterCloningFilter(resc, resv, argc, argv);
		break;
	case 423:
		_wrap_helicsFederateRegisterGlobalCloningFilter(resc, resv, argc, argv);
		break;
	case 424:
		_wrap_helicsCoreRegisterFilter(resc, resv, argc, argv);
		break;
	case 425:
		_wrap_helicsCoreRegisterCloningFilter(resc, resv, argc, argv);
		break;
	case 426:
		_wrap_helicsFederateGetFilterCount(resc, resv, argc, argv);
		break;
	case 427:
		_wrap_helicsFederateGetFilter(resc, resv, argc, argv);
		break;
	case 428:
		_wrap_helicsFederateGetFilterByIndex(resc, resv, argc, argv);
		break;
	case 429:
		_wrap_helicsFilterIsValid(resc, resv, argc, argv);
		break;
	case 430:
		_wrap_helicsFilterGetName(resc, resv, argc, argv);
		break;
	case 431:
		_wrap_helicsFilterSet(resc, resv, argc, argv);
		break;
	case 432:
		_wrap_helicsFilterSetString(resc, resv, argc, argv);
		break;
	case 433:
		_wrap_helicsFilterAddDestinationTarget(resc, resv, argc, argv);
		break;
	case 434:
		_wrap_helicsFilterAddSourceTarget(resc, resv, argc, argv);
		break;
	case 435:
		_wrap_helicsFilterAddDeliveryEndpoint(resc, resv, argc, argv);
		break;
	case 436:
		_wrap_helicsFilterRemoveTarget(resc, resv, argc, argv);
		break;
	case 437:
		_wrap_helicsFilterRemoveDeliveryEndpoint(resc, resv, argc, argv);
		break;
	case 438:
		_wrap_helicsFilterGetInfo(resc, resv, argc, argv);
		break;
	case 439:
		_wrap_helicsFilterSetInfo(resc, resv, argc, argv);
		break;
	case 440:
		_wrap_helicsFilterGetTag(resc, resv, argc, argv);
		break;
	case 441:
		_wrap_helicsFilterSetTag(resc, resv, argc, argv);
		break;
	case 442:
		_wrap_helicsFilterSetOption(resc, resv, argc, argv);
		break;
	case 443:
		_wrap_helicsFilterGetOption(resc, resv, argc, argv);
		break;
	case 444:
		_wrap_helicsFederateRegisterTranslator(resc, resv, argc, argv);
		break;
	case 445:
		_wrap_helicsFederateRegisterGlobalTranslator(resc, resv, argc, argv);
		break;
	case 446:
		_wrap_helicsCoreRegisterTranslator(resc, resv, argc, argv);
		break;
	case 447:
		_wrap_helicsFederateGetTranslatorCount(resc, resv, argc, argv);
		break;
	case 448:
		_wrap_helicsFederateGetTranslator(resc, resv, argc, argv);
		break;
	case 449:
		_wrap_helicsFederateGetTranslatorByIndex(resc, resv, argc, argv);
		break;
	case 450:
		_wrap_helicsTranslatorIsValid(resc, resv, argc, argv);
		break;
	case 451:
		_wrap_helicsTranslatorGetName(resc, resv, argc, argv);
		break;
	case 452:
		_wrap_helicsTranslatorSet(resc, resv, argc, argv);
		break;
	case 453:
		_wrap_helicsTranslatorSetString(resc, resv, argc, argv);
		break;
	case 454:
		_wrap_helicsTranslatorAddInputTarget(resc, resv, argc, argv);
		break;
	case 455:
		_wrap_helicsTranslatorAddPublicationTarget(resc, resv, argc, argv);
		break;
	case 456:
		_wrap_helicsTranslatorAddSourceEndpoint(resc, resv, argc, argv);
		break;
	case 457:
		_wrap_helicsTranslatorAddDestinationEndpoint(resc, resv, argc, argv);
		break;
	case 458:
		_wrap_helicsTranslatorRemoveTarget(resc, resv, argc, argv);
		break;
	case 459:
		_wrap_helicsTranslatorGetInfo(resc, resv, argc, argv);
		break;
	case 460:
		_wrap_helicsTranslatorSetInfo(resc, resv, argc, argv);
		break;
	case 461:
		_wrap_helicsTranslatorGetTag(resc, resv, argc, argv);
		break;
	case 462:
		_wrap_helicsTranslatorSetTag(resc, resv, argc, argv);
		break;
	case 463:
		_wrap_helicsTranslatorSetOption(resc, resv, argc, argv);
		break;
	case 464:
		_wrap_helicsTranslatorGetOption(resc, resv, argc, argv);
		break;
	case 465:
		_wrap_helicsBrokerSetLoggingCallback(resc, resv, argc, argv);
		break;
	case 466:
		_wrap_helicsCoreSetLoggingCallback(resc, resv, argc, argv);
		break;
	case 467:
		_wrap_helicsFederateSetLoggingCallback(resc, resv, argc, argv);
		break;
	case 468:
		_wrap_helicsFilterSetCustomCallback(resc, resv, argc, argv);
		break;
	case 469:
		_wrap_helicsTranslatorSetCustomCallback(resc, resv, argc, argv);
		break;
	case 470:
		_wrap_helicsFederateSetQueryCallback(resc, resv, argc, argv);
		break;
	case 471:
		_wrap_helicsFederateSetTimeRequestEntryCallback(resc, resv, argc, argv);
		break;
	case 472:
		_wrap_helicsFederateSetTimeUpdateCallback(resc, resv, argc, argv);
		break;
	case 473:
		_wrap_helicsFederateSetStateChangeCallback(resc, resv, argc, argv);
		break;
	case 474:
		_wrap_helicsFederateSetTimeRequestReturnCallback(resc, resv, argc, argv);
		break;
	case 475:
		_wrap_helicsQueryBufferFill(resc, resv, argc, argv);
		break;
	default:
		mexErrMsgIdAndTxt("helics:mexFunction","An unknown function id was encountered. Call the mex function with a valid function id.");
	}
}

