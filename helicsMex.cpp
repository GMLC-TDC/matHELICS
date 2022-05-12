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
	{"helicsWrapDataInBuffer",67},
	{"helicsDataBufferFree",68},
	{"helicsDataBufferSize",69},
	{"helicsDataBufferCapacity",70},
	{"helicsDataBufferData",71},
	{"helicsDataBufferReserve",72},
	{"helicsIntToBytes",73},
	{"helicsDoubleToBytes",74},
	{"helicsStringToBytes",75},
	{"helicsBoolToBytes",76},
	{"helicsCharToBytes",77},
	{"helicsTimeToBytes",78},
	{"helicsComplexToBytes",79},
	{"helicsVectorToBytes",80},
	{"helicsDataBufferType",81},
	{"helicsDataBufferToInt",82},
	{"helicsDataBufferToDouble",83},
	{"helicsDataBufferToBool",84},
	{"helicsGetVersion",85},
	{"helicsGetBuildFlags",86},
	{"helicsGetCompilerVersion",87},
	{"helicsGetSystemInfo",88},
	{"helicsLoadSignalHandler",91},
	{"helicsLoadThreadedSignalHandler",92},
	{"helicsClearSignalHandler",93},
	{"helicsLoadSignalHandlerCallback",94},
	{"helicsLoadSignalHandlerCallbackNoExit",95},
	{"helicsAbort",96},
	{"helicsIsCoreTypeAvailable",97},
	{"helicsCreateCore",98},
	{"helicsCreateCoreFromArgs",99},
	{"helicsCoreClone",100},
	{"helicsCoreIsValid",101},
	{"helicsCreateBroker",102},
	{"helicsCreateBrokerFromArgs",103},
	{"helicsBrokerClone",104},
	{"helicsBrokerIsValid",105},
	{"helicsBrokerIsConnected",106},
	{"helicsBrokerDataLink",107},
	{"helicsBrokerAddSourceFilterToEndpoint",108},
	{"helicsBrokerAddDestinationFilterToEndpoint",109},
	{"helicsBrokerMakeConnections",110},
	{"helicsCoreWaitForDisconnect",111},
	{"helicsBrokerWaitForDisconnect",112},
	{"helicsCoreIsConnected",113},
	{"helicsCoreDataLink",114},
	{"helicsCoreAddSourceFilterToEndpoint",115},
	{"helicsCoreAddDestinationFilterToEndpoint",116},
	{"helicsCoreMakeConnections",117},
	{"helicsBrokerGetIdentifier",118},
	{"helicsCoreGetIdentifier",119},
	{"helicsBrokerGetAddress",120},
	{"helicsCoreGetAddress",121},
	{"helicsCoreSetReadyToInit",122},
	{"helicsCoreConnect",123},
	{"helicsCoreDisconnect",124},
	{"helicsGetFederateByName",125},
	{"helicsBrokerDisconnect",126},
	{"helicsFederateDestroy",127},
	{"helicsBrokerDestroy",128},
	{"helicsCoreDestroy",129},
	{"helicsCoreFree",130},
	{"helicsBrokerFree",131},
	{"helicsCreateValueFederate",132},
	{"helicsCreateValueFederateFromConfig",133},
	{"helicsCreateMessageFederate",134},
	{"helicsCreateMessageFederateFromConfig",135},
	{"helicsCreateCombinationFederate",136},
	{"helicsCreateCombinationFederateFromConfig",137},
	{"helicsFederateClone",138},
	{"helicsCreateFederateInfo",139},
	{"helicsFederateInfoClone",140},
	{"helicsFederateInfoLoadFromArgs",141},
	{"helicsFederateInfoLoadFromString",142},
	{"helicsFederateInfoFree",143},
	{"helicsFederateIsValid",144},
	{"helicsFederateInfoSetCoreName",145},
	{"helicsFederateInfoSetCoreInitString",146},
	{"helicsFederateInfoSetBrokerInitString",147},
	{"helicsFederateInfoSetCoreType",148},
	{"helicsFederateInfoSetCoreTypeFromString",149},
	{"helicsFederateInfoSetBroker",150},
	{"helicsFederateInfoSetBrokerKey",151},
	{"helicsFederateInfoSetBrokerPort",152},
	{"helicsFederateInfoSetLocalPort",153},
	{"helicsGetPropertyIndex",154},
	{"helicsGetFlagIndex",155},
	{"helicsGetOptionIndex",156},
	{"helicsGetOptionValue",157},
	{"helicsGetDataType",158},
	{"helicsFederateInfoSetFlagOption",159},
	{"helicsFederateInfoSetSeparator",160},
	{"helicsFederateInfoSetTimeProperty",161},
	{"helicsFederateInfoSetIntegerProperty",162},
	{"helicsFederateRegisterInterfaces",163},
	{"helicsFederateGlobalError",164},
	{"helicsFederateLocalError",165},
	{"helicsFederateFinalize",166},
	{"helicsFederateFinalizeAsync",167},
	{"helicsFederateFinalizeComplete",168},
	{"helicsFederateDisconnect",169},
	{"helicsFederateDisconnectAsync",170},
	{"helicsFederateDisconnectComplete",171},
	{"helicsFederateFree",172},
	{"helicsCloseLibrary",173},
	{"helicsFederateEnterInitializingMode",174},
	{"helicsFederateEnterInitializingModeAsync",175},
	{"helicsFederateIsAsyncOperationCompleted",176},
	{"helicsFederateEnterInitializingModeComplete",177},
	{"helicsFederateEnterExecutingMode",178},
	{"helicsFederateEnterExecutingModeAsync",179},
	{"helicsFederateEnterExecutingModeComplete",180},
	{"helicsFederateEnterExecutingModeIterative",181},
	{"helicsFederateEnterExecutingModeIterativeAsync",182},
	{"helicsFederateEnterExecutingModeIterativeComplete",183},
	{"helicsFederateGetState",184},
	{"helicsFederateGetCore",185},
	{"helicsFederateRequestTime",186},
	{"helicsFederateRequestTimeAdvance",187},
	{"helicsFederateRequestNextStep",188},
	{"helicsFederateRequestTimeIterative",189},
	{"helicsFederateRequestTimeAsync",190},
	{"helicsFederateRequestTimeComplete",191},
	{"helicsFederateRequestTimeIterativeAsync",192},
	{"helicsFederateRequestTimeIterativeComplete",193},
	{"helicsFederateProcessCommunications",194},
	{"helicsFederateGetName",195},
	{"helicsFederateSetTimeProperty",196},
	{"helicsFederateSetFlagOption",197},
	{"helicsFederateSetSeparator",198},
	{"helicsFederateSetIntegerProperty",199},
	{"helicsFederateGetTimeProperty",200},
	{"helicsFederateGetFlagOption",201},
	{"helicsFederateGetIntegerProperty",202},
	{"helicsFederateGetCurrentTime",203},
	{"helicsFederateSetGlobal",204},
	{"helicsFederateSetTag",205},
	{"helicsFederateGetTag",206},
	{"helicsFederateAddDependency",207},
	{"helicsFederateSetLogFile",208},
	{"helicsFederateLogErrorMessage",209},
	{"helicsFederateLogWarningMessage",210},
	{"helicsFederateLogInfoMessage",211},
	{"helicsFederateLogDebugMessage",212},
	{"helicsFederateLogLevelMessage",213},
	{"helicsFederateSendCommand",214},
	{"helicsFederateGetCommand",215},
	{"helicsFederateGetCommandSource",216},
	{"helicsFederateWaitCommand",217},
	{"helicsCoreSetGlobal",218},
	{"helicsBrokerSetGlobal",219},
	{"helicsCoreSendCommand",220},
	{"helicsBrokerSendCommand",221},
	{"helicsCoreSetLogFile",222},
	{"helicsBrokerSetLogFile",223},
	{"helicsBrokerSetTimeBarrier",224},
	{"helicsBrokerClearTimeBarrier",225},
	{"helicsBrokerGlobalError",226},
	{"helicsCoreGlobalError",227},
	{"helicsCreateQuery",228},
	{"helicsQueryExecute",229},
	{"helicsQueryCoreExecute",230},
	{"helicsQueryBrokerExecute",231},
	{"helicsQueryExecuteAsync",232},
	{"helicsQueryExecuteComplete",233},
	{"helicsQueryIsCompleted",234},
	{"helicsQuerySetTarget",235},
	{"helicsQuerySetQueryString",236},
	{"helicsQuerySetOrdering",237},
	{"helicsQueryFree",238},
	{"helicsCleanupLibrary",239},
	{"helicsFederateRegisterSubscription",240},
	{"helicsFederateRegisterPublication",241},
	{"helicsFederateRegisterTypePublication",242},
	{"helicsFederateRegisterGlobalPublication",243},
	{"helicsFederateRegisterGlobalTypePublication",244},
	{"helicsFederateRegisterInput",245},
	{"helicsFederateRegisterTypeInput",246},
	{"helicsFederateRegisterGlobalInput",247},
	{"helicsFederateRegisterGlobalTypeInput",248},
	{"helicsFederateGetPublication",249},
	{"helicsFederateGetPublicationByIndex",250},
	{"helicsFederateGetInput",251},
	{"helicsFederateGetInputByIndex",252},
	{"helicsFederateGetSubscription",253},
	{"helicsFederateClearUpdates",254},
	{"helicsFederateRegisterFromPublicationJSON",255},
	{"helicsFederatePublishJSON",256},
	{"helicsPublicationIsValid",257},
	{"helicsPublicationPublishBytes",258},
	{"helicsPublicationPublishString",259},
	{"helicsPublicationPublishInteger",260},
	{"helicsPublicationPublishBoolean",261},
	{"helicsPublicationPublishDouble",262},
	{"helicsPublicationPublishTime",263},
	{"helicsPublicationPublishChar",264},
	{"helicsPublicationPublishComplex",265},
	{"helicsPublicationPublishVector",266},
	{"helicsPublicationPublishComplexVector",267},
	{"helicsPublicationPublishNamedPoint",268},
	{"helicsPublicationAddTarget",269},
	{"helicsInputIsValid",270},
	{"helicsInputAddTarget",271},
	{"helicsInputGetByteCount",272},
	{"helicsInputGetBytes",273},
	{"helicsInputGetStringSize",274},
	{"helicsInputGetString",275},
	{"helicsInputGetInteger",276},
	{"helicsInputGetBoolean",277},
	{"helicsInputGetDouble",278},
	{"helicsInputGetTime",279},
	{"helicsInputGetChar",280},
	{"helicsInputGetComplexObject",281},
	{"helicsInputGetComplex",282},
	{"helicsInputGetVectorSize",283},
	{"helicsInputGetVector",284},
	{"helicsInputGetComplexVector",285},
	{"helicsInputGetNamedPoint",286},
	{"helicsInputSetDefaultBytes",287},
	{"helicsInputSetDefaultString",288},
	{"helicsInputSetDefaultInteger",289},
	{"helicsInputSetDefaultBoolean",290},
	{"helicsInputSetDefaultTime",291},
	{"helicsInputSetDefaultChar",292},
	{"helicsInputSetDefaultDouble",293},
	{"helicsInputSetDefaultComplex",294},
	{"helicsInputSetDefaultVector",295},
	{"helicsInputSetDefaultComplexVector",296},
	{"helicsInputSetDefaultNamedPoint",297},
	{"helicsInputGetType",298},
	{"helicsInputGetPublicationType",299},
	{"helicsInputGetPublicationDataType",300},
	{"helicsPublicationGetType",301},
	{"helicsInputGetName",302},
	{"helicsSubscriptionGetTarget",303},
	{"helicsPublicationGetName",304},
	{"helicsInputGetUnits",305},
	{"helicsInputGetInjectionUnits",306},
	{"helicsInputGetExtractionUnits",307},
	{"helicsPublicationGetUnits",308},
	{"helicsInputGetInfo",309},
	{"helicsInputSetInfo",310},
	{"helicsInputGetTag",311},
	{"helicsInputSetTag",312},
	{"helicsPublicationGetInfo",313},
	{"helicsPublicationSetInfo",314},
	{"helicsPublicationGetTag",315},
	{"helicsPublicationSetTag",316},
	{"helicsInputGetOption",317},
	{"helicsInputSetOption",318},
	{"helicsPublicationGetOption",319},
	{"helicsPublicationSetOption",320},
	{"helicsPublicationSetMinimumChange",321},
	{"helicsInputSetMinimumChange",322},
	{"helicsInputIsUpdated",323},
	{"helicsInputLastUpdateTime",324},
	{"helicsInputClearUpdate",325},
	{"helicsFederateGetPublicationCount",326},
	{"helicsFederateGetInputCount",327},
	{"helicsFederateRegisterEndpoint",328},
	{"helicsFederateRegisterGlobalEndpoint",329},
	{"helicsFederateRegisterTargetedEndpoint",330},
	{"helicsFederateRegisterGlobalTargetedEndpoint",331},
	{"helicsFederateGetEndpoint",332},
	{"helicsFederateGetEndpointByIndex",333},
	{"helicsEndpointIsValid",334},
	{"helicsEndpointSetDefaultDestination",335},
	{"helicsEndpointGetDefaultDestination",336},
	{"helicsEndpointSendBytes",337},
	{"helicsEndpointSendBytesTo",338},
	{"helicsEndpointSendBytesToAt",339},
	{"helicsEndpointSendBytesAt",340},
	{"helicsEndpointSendMessage",341},
	{"helicsEndpointSendMessageZeroCopy",342},
	{"helicsEndpointSubscribe",343},
	{"helicsFederateHasMessage",344},
	{"helicsEndpointHasMessage",345},
	{"helicsFederatePendingMessageCount",346},
	{"helicsEndpointPendingMessageCount",347},
	{"helicsEndpointGetMessage",348},
	{"helicsEndpointCreateMessage",349},
	{"helicsFederateGetMessage",350},
	{"helicsFederateCreateMessage",351},
	{"helicsFederateClearMessages",352},
	{"helicsEndpointGetType",353},
	{"helicsEndpointGetName",354},
	{"helicsFederateGetEndpointCount",355},
	{"helicsEndpointGetInfo",356},
	{"helicsEndpointSetInfo",357},
	{"helicsEndpointGetTag",358},
	{"helicsEndpointSetTag",359},
	{"helicsEndpointSetOption",360},
	{"helicsEndpointGetOption",361},
	{"helicsEndpointAddSourceTarget",362},
	{"helicsEndpointAddDestinationTarget",363},
	{"helicsEndpointRemoveTarget",364},
	{"helicsEndpointAddSourceFilter",365},
	{"helicsEndpointAddDestinationFilter",366},
	{"helicsMessageGetSource",367},
	{"helicsMessageGetDestination",368},
	{"helicsMessageGetOriginalSource",369},
	{"helicsMessageGetOriginalDestination",370},
	{"helicsMessageGetTime",371},
	{"helicsMessageGetString",372},
	{"helicsMessageGetMessageID",373},
	{"helicsMessageGetFlagOption",374},
	{"helicsMessageGetByteCount",375},
	{"helicsMessageGetBytes",376},
	{"helicsMessageGetBytesPointer",377},
	{"helicsMessageIsValid",378},
	{"helicsMessageSetSource",379},
	{"helicsMessageSetDestination",380},
	{"helicsMessageSetOriginalSource",381},
	{"helicsMessageSetOriginalDestination",382},
	{"helicsMessageSetTime",383},
	{"helicsMessageResize",384},
	{"helicsMessageReserve",385},
	{"helicsMessageSetMessageID",386},
	{"helicsMessageClearFlags",387},
	{"helicsMessageSetFlagOption",388},
	{"helicsMessageSetString",389},
	{"helicsMessageSetData",390},
	{"helicsMessageAppendData",391},
	{"helicsMessageCopy",392},
	{"helicsMessageClone",393},
	{"helicsMessageFree",394},
	{"helicsMessageClear",395},
	{"helicsFederateRegisterFilter",396},
	{"helicsFederateRegisterGlobalFilter",397},
	{"helicsFederateRegisterCloningFilter",398},
	{"helicsFederateRegisterGlobalCloningFilter",399},
	{"helicsCoreRegisterFilter",400},
	{"helicsCoreRegisterCloningFilter",401},
	{"helicsFederateGetFilterCount",402},
	{"helicsFederateGetFilter",403},
	{"helicsFederateGetFilterByIndex",404},
	{"helicsFilterIsValid",405},
	{"helicsFilterGetName",406},
	{"helicsFilterSet",407},
	{"helicsFilterSetString",408},
	{"helicsFilterAddDestinationTarget",409},
	{"helicsFilterAddSourceTarget",410},
	{"helicsFilterAddDeliveryEndpoint",411},
	{"helicsFilterRemoveTarget",412},
	{"helicsFilterRemoveDeliveryEndpoint",413},
	{"helicsFilterGetInfo",414},
	{"helicsFilterSetInfo",415},
	{"helicsFilterGetTag",416},
	{"helicsFilterSetTag",417},
	{"helicsFilterSetOption",418},
	{"helicsFilterGetOption",419},
	{"helicsFederateRegisterTranslator",420},
	{"helicsFederateRegisterGlobalTranslator",421},
	{"helicsCoreRegisterTranslator",422},
	{"helicsFederateGetTranslatorCount",423},
	{"helicsFederateGetTranslator",424},
	{"helicsFederateGetTranslatorByIndex",425},
	{"helicsTranslatorIsValid",426},
	{"helicsTranslatorGetName",427},
	{"helicsTranslatorSet",428},
	{"helicsTranslatorSetString",429},
	{"helicsTranslatorAddInputTarget",430},
	{"helicsTranslatorAddPublicationTarget",431},
	{"helicsTranslatorAddSourceEndpoint",432},
	{"helicsTranslatorAddDestinationEndpoint",433},
	{"helicsTranslatorRemoveTarget",434},
	{"helicsTranslatorGetInfo",435},
	{"helicsTranslatorSetInfo",436},
	{"helicsTranslatorGetTag",437},
	{"helicsTranslatorSetTag",438},
	{"helicsTranslatorSetOption",439},
	{"helicsTranslatorGetOption",440},
	{"helicsBrokerSetLoggingCallback",441},
	{"helicsCoreSetLoggingCallback",442},
	{"helicsFederateSetLoggingCallback",443},
	{"helicsFilterSetCustomCallback",444},
	{"helicsFederateSetQueryCallback",445},
	{"helicsFederateSetTimeRequestEntryCallback",446},
	{"helicsFederateSetTimeUpdateCallback",447},
	{"helicsFederateSetStateChangeCallback",448},
	{"helicsFederateSetTimeRequestReturnCallback",449},
	{"helicsQueryBufferFill",450}
};

void mexFunction(int resc, mxArray *resv[], int argc, const mxArray *argv[]) {
	if(--argc < 0 || !mxIsString(*argv)){
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
		_wrap_HELICS_DATA_TYPE_CHAR(resc, resv, argc, (mxArray**)(argv));
		break;
	case 66:
		_wrap_helicsCreateDataBuffer(resc, resv, argc, (mxArray**)(argv));
		break;
	case 67:
		_wrap_helicsWrapDataInBuffer(resc, resv, argc, (mxArray**)(argv));
		break;
	case 68:
		_wrap_helicsDataBufferFree(resc, resv, argc, (mxArray**)(argv));
		break;
	case 69:
		_wrap_helicsDataBufferSize(resc, resv, argc, (mxArray**)(argv));
		break;
	case 70:
		_wrap_helicsDataBufferCapacity(resc, resv, argc, (mxArray**)(argv));
		break;
	case 71:
		_wrap_helicsDataBufferData(resc, resv, argc, (mxArray**)(argv));
		break;
	case 72:
		_wrap_helicsDataBufferReserve(resc, resv, argc, (mxArray**)(argv));
		break;
	case 73:
		_wrap_helicsIntToBytes(resc, resv, argc, (mxArray**)(argv));
		break;
	case 74:
		_wrap_helicsDoubleToBytes(resc, resv, argc, (mxArray**)(argv));
		break;
	case 75:
		_wrap_helicsStringToBytes(resc, resv, argc, (mxArray**)(argv));
		break;
	case 76:
		_wrap_helicsBoolToBytes(resc, resv, argc, (mxArray**)(argv));
		break;
	case 77:
		_wrap_helicsCharToBytes(resc, resv, argc, (mxArray**)(argv));
		break;
	case 78:
		_wrap_helicsTimeToBytes(resc, resv, argc, (mxArray**)(argv));
		break;
	case 79:
		_wrap_helicsComplexToBytes(resc, resv, argc, (mxArray**)(argv));
		break;
	case 80:
		_wrap_helicsVectorToBytes(resc, resv, argc, (mxArray**)(argv));
		break;
	case 81:
		_wrap_helicsDataBufferType(resc, resv, argc, (mxArray**)(argv));
		break;
	case 82:
		_wrap_helicsDataBufferToInt(resc, resv, argc, (mxArray**)(argv));
		break;
	case 83:
		_wrap_helicsDataBufferToDouble(resc, resv, argc, (mxArray**)(argv));
		break;
	case 84:
		_wrap_helicsDataBufferToBool(resc, resv, argc, (mxArray**)(argv));
		break;
	case 85:
		_wrap_helicsGetVersion(resc, resv, argc, (mxArray**)(argv));
		break;
	case 86:
		_wrap_helicsGetBuildFlags(resc, resv, argc, (mxArray**)(argv));
		break;
	case 87:
		_wrap_helicsGetCompilerVersion(resc, resv, argc, (mxArray**)(argv));
		break;
	case 88:
		_wrap_helicsGetSystemInfo(resc, resv, argc, (mxArray**)(argv));
		break;
	case 91:
		_wrap_helicsLoadSignalHandler(resc, resv, argc, (mxArray**)(argv));
		break;
	case 92:
		_wrap_helicsLoadThreadedSignalHandler(resc, resv, argc, (mxArray**)(argv));
		break;
	case 93:
		_wrap_helicsClearSignalHandler(resc, resv, argc, (mxArray**)(argv));
		break;
	case 96:
		_wrap_helicsAbort(resc, resv, argc, (mxArray**)(argv));
		break;
	case 97:
		_wrap_helicsIsCoreTypeAvailable(resc, resv, argc, (mxArray**)(argv));
		break;
	case 98:
		_wrap_helicsCreateCore(resc, resv, argc, (mxArray**)(argv));
		break;
	case 99:
		_wrap_helicsCreateCoreFromArgs(resc, resv, argc, (mxArray**)(argv));
		break;
	case 100:
		_wrap_helicsCoreClone(resc, resv, argc, (mxArray**)(argv));
		break;
	case 101:
		_wrap_helicsCoreIsValid(resc, resv, argc, (mxArray**)(argv));
		break;
	case 102:
		_wrap_helicsCreateBroker(resc, resv, argc, (mxArray**)(argv));
		break;
	case 103:
		_wrap_helicsCreateBrokerFromArgs(resc, resv, argc, (mxArray**)(argv));
		break;
	case 104:
		_wrap_helicsBrokerClone(resc, resv, argc, (mxArray**)(argv));
		break;
	case 105:
		_wrap_helicsBrokerIsValid(resc, resv, argc, (mxArray**)(argv));
		break;
	case 106:
		_wrap_helicsBrokerIsConnected(resc, resv, argc, (mxArray**)(argv));
		break;
	case 107:
		_wrap_helicsBrokerDataLink(resc, resv, argc, (mxArray**)(argv));
		break;
	case 108:
		_wrap_helicsBrokerAddSourceFilterToEndpoint(resc, resv, argc, (mxArray**)(argv));
		break;
	case 109:
		_wrap_helicsBrokerAddDestinationFilterToEndpoint(resc, resv, argc, (mxArray**)(argv));
		break;
	case 110:
		_wrap_helicsBrokerMakeConnections(resc, resv, argc, (mxArray**)(argv));
		break;
	case 111:
		_wrap_helicsCoreWaitForDisconnect(resc, resv, argc, (mxArray**)(argv));
		break;
	case 112:
		_wrap_helicsBrokerWaitForDisconnect(resc, resv, argc, (mxArray**)(argv));
		break;
	case 113:
		_wrap_helicsCoreIsConnected(resc, resv, argc, (mxArray**)(argv));
		break;
	case 114:
		_wrap_helicsCoreDataLink(resc, resv, argc, (mxArray**)(argv));
		break;
	case 115:
		_wrap_helicsCoreAddSourceFilterToEndpoint(resc, resv, argc, (mxArray**)(argv));
		break;
	case 116:
		_wrap_helicsCoreAddDestinationFilterToEndpoint(resc, resv, argc, (mxArray**)(argv));
		break;
	case 117:
		_wrap_helicsCoreMakeConnections(resc, resv, argc, (mxArray**)(argv));
		break;
	case 118:
		_wrap_helicsBrokerGetIdentifier(resc, resv, argc, (mxArray**)(argv));
		break;
	case 119:
		_wrap_helicsCoreGetIdentifier(resc, resv, argc, (mxArray**)(argv));
		break;
	case 120:
		_wrap_helicsBrokerGetAddress(resc, resv, argc, (mxArray**)(argv));
		break;
	case 121:
		_wrap_helicsCoreGetAddress(resc, resv, argc, (mxArray**)(argv));
		break;
	case 122:
		_wrap_helicsCoreSetReadyToInit(resc, resv, argc, (mxArray**)(argv));
		break;
	case 123:
		_wrap_helicsCoreConnect(resc, resv, argc, (mxArray**)(argv));
		break;
	case 124:
		_wrap_helicsCoreDisconnect(resc, resv, argc, (mxArray**)(argv));
		break;
	case 125:
		_wrap_helicsGetFederateByName(resc, resv, argc, (mxArray**)(argv));
		break;
	case 126:
		_wrap_helicsBrokerDisconnect(resc, resv, argc, (mxArray**)(argv));
		break;
	case 127:
		_wrap_helicsFederateDestroy(resc, resv, argc, (mxArray**)(argv));
		break;
	case 128:
		_wrap_helicsBrokerDestroy(resc, resv, argc, (mxArray**)(argv));
		break;
	case 129:
		_wrap_helicsCoreDestroy(resc, resv, argc, (mxArray**)(argv));
		break;
	case 130:
		_wrap_helicsCoreFree(resc, resv, argc, (mxArray**)(argv));
		break;
	case 131:
		_wrap_helicsBrokerFree(resc, resv, argc, (mxArray**)(argv));
		break;
	case 132:
		_wrap_helicsCreateValueFederate(resc, resv, argc, (mxArray**)(argv));
		break;
	case 133:
		_wrap_helicsCreateValueFederateFromConfig(resc, resv, argc, (mxArray**)(argv));
		break;
	case 134:
		_wrap_helicsCreateMessageFederate(resc, resv, argc, (mxArray**)(argv));
		break;
	case 135:
		_wrap_helicsCreateMessageFederateFromConfig(resc, resv, argc, (mxArray**)(argv));
		break;
	case 136:
		_wrap_helicsCreateCombinationFederate(resc, resv, argc, (mxArray**)(argv));
		break;
	case 137:
		_wrap_helicsCreateCombinationFederateFromConfig(resc, resv, argc, (mxArray**)(argv));
		break;
	case 138:
		_wrap_helicsFederateClone(resc, resv, argc, (mxArray**)(argv));
		break;
	case 139:
		_wrap_helicsCreateFederateInfo(resc, resv, argc, (mxArray**)(argv));
		break;
	case 140:
		_wrap_helicsFederateInfoClone(resc, resv, argc, (mxArray**)(argv));
		break;
	case 141:
		_wrap_helicsFederateInfoLoadFromArgs(resc, resv, argc, (mxArray**)(argv));
		break;
	case 142:
		_wrap_helicsFederateInfoLoadFromString(resc, resv, argc, (mxArray**)(argv));
		break;
	case 143:
		_wrap_helicsFederateInfoFree(resc, resv, argc, (mxArray**)(argv));
		break;
	case 144:
		_wrap_helicsFederateIsValid(resc, resv, argc, (mxArray**)(argv));
		break;
	case 145:
		_wrap_helicsFederateInfoSetCoreName(resc, resv, argc, (mxArray**)(argv));
		break;
	case 146:
		_wrap_helicsFederateInfoSetCoreInitString(resc, resv, argc, (mxArray**)(argv));
		break;
	case 147:
		_wrap_helicsFederateInfoSetBrokerInitString(resc, resv, argc, (mxArray**)(argv));
		break;
	case 148:
		_wrap_helicsFederateInfoSetCoreType(resc, resv, argc, (mxArray**)(argv));
		break;
	case 149:
		_wrap_helicsFederateInfoSetCoreTypeFromString(resc, resv, argc, (mxArray**)(argv));
		break;
	case 150:
		_wrap_helicsFederateInfoSetBroker(resc, resv, argc, (mxArray**)(argv));
		break;
	case 151:
		_wrap_helicsFederateInfoSetBrokerKey(resc, resv, argc, (mxArray**)(argv));
		break;
	case 152:
		_wrap_helicsFederateInfoSetBrokerPort(resc, resv, argc, (mxArray**)(argv));
		break;
	case 153:
		_wrap_helicsFederateInfoSetLocalPort(resc, resv, argc, (mxArray**)(argv));
		break;
	case 154:
		_wrap_helicsGetPropertyIndex(resc, resv, argc, (mxArray**)(argv));
		break;
	case 155:
		_wrap_helicsGetFlagIndex(resc, resv, argc, (mxArray**)(argv));
		break;
	case 156:
		_wrap_helicsGetOptionIndex(resc, resv, argc, (mxArray**)(argv));
		break;
	case 157:
		_wrap_helicsGetOptionValue(resc, resv, argc, (mxArray**)(argv));
		break;
	case 158:
		_wrap_helicsGetDataType(resc, resv, argc, (mxArray**)(argv));
		break;
	case 159:
		_wrap_helicsFederateInfoSetFlagOption(resc, resv, argc, (mxArray**)(argv));
		break;
	case 160:
		_wrap_helicsFederateInfoSetSeparator(resc, resv, argc, (mxArray**)(argv));
		break;
	case 161:
		_wrap_helicsFederateInfoSetTimeProperty(resc, resv, argc, (mxArray**)(argv));
		break;
	case 162:
		_wrap_helicsFederateInfoSetIntegerProperty(resc, resv, argc, (mxArray**)(argv));
		break;
	case 163:
		_wrap_helicsFederateRegisterInterfaces(resc, resv, argc, (mxArray**)(argv));
		break;
	case 164:
		_wrap_helicsFederateGlobalError(resc, resv, argc, (mxArray**)(argv));
		break;
	case 165:
		_wrap_helicsFederateLocalError(resc, resv, argc, (mxArray**)(argv));
		break;
	case 166:
		_wrap_helicsFederateFinalize(resc, resv, argc, (mxArray**)(argv));
		break;
	case 167:
		_wrap_helicsFederateFinalizeAsync(resc, resv, argc, (mxArray**)(argv));
		break;
	case 168:
		_wrap_helicsFederateFinalizeComplete(resc, resv, argc, (mxArray**)(argv));
		break;
	case 169:
		_wrap_helicsFederateDisconnect(resc, resv, argc, (mxArray**)(argv));
		break;
	case 170:
		_wrap_helicsFederateDisconnectAsync(resc, resv, argc, (mxArray**)(argv));
		break;
	case 171:
		_wrap_helicsFederateDisconnectComplete(resc, resv, argc, (mxArray**)(argv));
		break;
	case 172:
		_wrap_helicsFederateFree(resc, resv, argc, (mxArray**)(argv));
		break;
	case 173:
		_wrap_helicsCloseLibrary(resc, resv, argc, (mxArray**)(argv));
		break;
	case 174:
		_wrap_helicsFederateEnterInitializingMode(resc, resv, argc, (mxArray**)(argv));
		break;
	case 175:
		_wrap_helicsFederateEnterInitializingModeAsync(resc, resv, argc, (mxArray**)(argv));
		break;
	case 176:
		_wrap_helicsFederateIsAsyncOperationCompleted(resc, resv, argc, (mxArray**)(argv));
		break;
	case 177:
		_wrap_helicsFederateEnterInitializingModeComplete(resc, resv, argc, (mxArray**)(argv));
		break;
	case 178:
		_wrap_helicsFederateEnterExecutingMode(resc, resv, argc, (mxArray**)(argv));
		break;
	case 179:
		_wrap_helicsFederateEnterExecutingModeAsync(resc, resv, argc, (mxArray**)(argv));
		break;
	case 180:
		_wrap_helicsFederateEnterExecutingModeComplete(resc, resv, argc, (mxArray**)(argv));
		break;
	case 181:
		_wrap_helicsFederateEnterExecutingModeIterative(resc, resv, argc, (mxArray**)(argv));
		break;
	case 182:
		_wrap_helicsFederateEnterExecutingModeIterativeAsync(resc, resv, argc, (mxArray**)(argv));
		break;
	case 183:
		_wrap_helicsFederateEnterExecutingModeIterativeComplete(resc, resv, argc, (mxArray**)(argv));
		break;
	case 184:
		_wrap_helicsFederateGetState(resc, resv, argc, (mxArray**)(argv));
		break;
	case 185:
		_wrap_helicsFederateGetCore(resc, resv, argc, (mxArray**)(argv));
		break;
	case 186:
		_wrap_helicsFederateRequestTime(resc, resv, argc, (mxArray**)(argv));
		break;
	case 187:
		_wrap_helicsFederateRequestTimeAdvance(resc, resv, argc, (mxArray**)(argv));
		break;
	case 188:
		_wrap_helicsFederateRequestNextStep(resc, resv, argc, (mxArray**)(argv));
		break;
	case 189:
		_wrap_helicsFederateRequestTimeIterative(resc, resv, argc, (mxArray**)(argv));
		break;
	case 190:
		_wrap_helicsFederateRequestTimeAsync(resc, resv, argc, (mxArray**)(argv));
		break;
	case 191:
		_wrap_helicsFederateRequestTimeComplete(resc, resv, argc, (mxArray**)(argv));
		break;
	case 192:
		_wrap_helicsFederateRequestTimeIterativeAsync(resc, resv, argc, (mxArray**)(argv));
		break;
	case 193:
		_wrap_helicsFederateRequestTimeIterativeComplete(resc, resv, argc, (mxArray**)(argv));
		break;
	case 194:
		_wrap_helicsFederateProcessCommunications(resc, resv, argc, (mxArray**)(argv));
		break;
	case 195:
		_wrap_helicsFederateGetName(resc, resv, argc, (mxArray**)(argv));
		break;
	case 196:
		_wrap_helicsFederateSetTimeProperty(resc, resv, argc, (mxArray**)(argv));
		break;
	case 197:
		_wrap_helicsFederateSetFlagOption(resc, resv, argc, (mxArray**)(argv));
		break;
	case 198:
		_wrap_helicsFederateSetSeparator(resc, resv, argc, (mxArray**)(argv));
		break;
	case 199:
		_wrap_helicsFederateSetIntegerProperty(resc, resv, argc, (mxArray**)(argv));
		break;
	case 200:
		_wrap_helicsFederateGetTimeProperty(resc, resv, argc, (mxArray**)(argv));
		break;
	case 201:
		_wrap_helicsFederateGetFlagOption(resc, resv, argc, (mxArray**)(argv));
		break;
	case 202:
		_wrap_helicsFederateGetIntegerProperty(resc, resv, argc, (mxArray**)(argv));
		break;
	case 203:
		_wrap_helicsFederateGetCurrentTime(resc, resv, argc, (mxArray**)(argv));
		break;
	case 204:
		_wrap_helicsFederateSetGlobal(resc, resv, argc, (mxArray**)(argv));
		break;
	case 205:
		_wrap_helicsFederateSetTag(resc, resv, argc, (mxArray**)(argv));
		break;
	case 206:
		_wrap_helicsFederateGetTag(resc, resv, argc, (mxArray**)(argv));
		break;
	case 207:
		_wrap_helicsFederateAddDependency(resc, resv, argc, (mxArray**)(argv));
		break;
	case 208:
		_wrap_helicsFederateSetLogFile(resc, resv, argc, (mxArray**)(argv));
		break;
	case 209:
		_wrap_helicsFederateLogErrorMessage(resc, resv, argc, (mxArray**)(argv));
		break;
	case 210:
		_wrap_helicsFederateLogWarningMessage(resc, resv, argc, (mxArray**)(argv));
		break;
	case 211:
		_wrap_helicsFederateLogInfoMessage(resc, resv, argc, (mxArray**)(argv));
		break;
	case 212:
		_wrap_helicsFederateLogDebugMessage(resc, resv, argc, (mxArray**)(argv));
		break;
	case 213:
		_wrap_helicsFederateLogLevelMessage(resc, resv, argc, (mxArray**)(argv));
		break;
	case 214:
		_wrap_helicsFederateSendCommand(resc, resv, argc, (mxArray**)(argv));
		break;
	case 215:
		_wrap_helicsFederateGetCommand(resc, resv, argc, (mxArray**)(argv));
		break;
	case 216:
		_wrap_helicsFederateGetCommandSource(resc, resv, argc, (mxArray**)(argv));
		break;
	case 217:
		_wrap_helicsFederateWaitCommand(resc, resv, argc, (mxArray**)(argv));
		break;
	case 218:
		_wrap_helicsCoreSetGlobal(resc, resv, argc, (mxArray**)(argv));
		break;
	case 219:
		_wrap_helicsBrokerSetGlobal(resc, resv, argc, (mxArray**)(argv));
		break;
	case 220:
		_wrap_helicsCoreSendCommand(resc, resv, argc, (mxArray**)(argv));
		break;
	case 221:
		_wrap_helicsBrokerSendCommand(resc, resv, argc, (mxArray**)(argv));
		break;
	case 222:
		_wrap_helicsCoreSetLogFile(resc, resv, argc, (mxArray**)(argv));
		break;
	case 223:
		_wrap_helicsBrokerSetLogFile(resc, resv, argc, (mxArray**)(argv));
		break;
	case 224:
		_wrap_helicsBrokerSetTimeBarrier(resc, resv, argc, (mxArray**)(argv));
		break;
	case 225:
		_wrap_helicsBrokerClearTimeBarrier(resc, resv, argc, (mxArray**)(argv));
		break;
	case 226:
		_wrap_helicsBrokerGlobalError(resc, resv, argc, (mxArray**)(argv));
		break;
	case 227:
		_wrap_helicsCoreGlobalError(resc, resv, argc, (mxArray**)(argv));
		break;
	case 228:
		_wrap_helicsCreateQuery(resc, resv, argc, (mxArray**)(argv));
		break;
	case 229:
		_wrap_helicsQueryExecute(resc, resv, argc, (mxArray**)(argv));
		break;
	case 230:
		_wrap_helicsQueryCoreExecute(resc, resv, argc, (mxArray**)(argv));
		break;
	case 231:
		_wrap_helicsQueryBrokerExecute(resc, resv, argc, (mxArray**)(argv));
		break;
	case 232:
		_wrap_helicsQueryExecuteAsync(resc, resv, argc, (mxArray**)(argv));
		break;
	case 233:
		_wrap_helicsQueryExecuteComplete(resc, resv, argc, (mxArray**)(argv));
		break;
	case 234:
		_wrap_helicsQueryIsCompleted(resc, resv, argc, (mxArray**)(argv));
		break;
	case 235:
		_wrap_helicsQuerySetTarget(resc, resv, argc, (mxArray**)(argv));
		break;
	case 236:
		_wrap_helicsQuerySetQueryString(resc, resv, argc, (mxArray**)(argv));
		break;
	case 237:
		_wrap_helicsQuerySetOrdering(resc, resv, argc, (mxArray**)(argv));
		break;
	case 238:
		_wrap_helicsQueryFree(resc, resv, argc, (mxArray**)(argv));
		break;
	case 239:
		_wrap_helicsCleanupLibrary(resc, resv, argc, (mxArray**)(argv));
		break;
	case 240:
		_wrap_helicsFederateRegisterSubscription(resc, resv, argc, (mxArray**)(argv));
		break;
	case 241:
		_wrap_helicsFederateRegisterPublication(resc, resv, argc, (mxArray**)(argv));
		break;
	case 242:
		_wrap_helicsFederateRegisterTypePublication(resc, resv, argc, (mxArray**)(argv));
		break;
	case 243:
		_wrap_helicsFederateRegisterGlobalPublication(resc, resv, argc, (mxArray**)(argv));
		break;
	case 244:
		_wrap_helicsFederateRegisterGlobalTypePublication(resc, resv, argc, (mxArray**)(argv));
		break;
	case 245:
		_wrap_helicsFederateRegisterInput(resc, resv, argc, (mxArray**)(argv));
		break;
	case 246:
		_wrap_helicsFederateRegisterTypeInput(resc, resv, argc, (mxArray**)(argv));
		break;
	case 247:
		_wrap_helicsFederateRegisterGlobalInput(resc, resv, argc, (mxArray**)(argv));
		break;
	case 248:
		_wrap_helicsFederateRegisterGlobalTypeInput(resc, resv, argc, (mxArray**)(argv));
		break;
	case 249:
		_wrap_helicsFederateGetPublication(resc, resv, argc, (mxArray**)(argv));
		break;
	case 250:
		_wrap_helicsFederateGetPublicationByIndex(resc, resv, argc, (mxArray**)(argv));
		break;
	case 251:
		_wrap_helicsFederateGetInput(resc, resv, argc, (mxArray**)(argv));
		break;
	case 252:
		_wrap_helicsFederateGetInputByIndex(resc, resv, argc, (mxArray**)(argv));
		break;
	case 253:
		_wrap_helicsFederateGetSubscription(resc, resv, argc, (mxArray**)(argv));
		break;
	case 254:
		_wrap_helicsFederateClearUpdates(resc, resv, argc, (mxArray**)(argv));
		break;
	case 255:
		_wrap_helicsFederateRegisterFromPublicationJSON(resc, resv, argc, (mxArray**)(argv));
		break;
	case 256:
		_wrap_helicsFederatePublishJSON(resc, resv, argc, (mxArray**)(argv));
		break;
	case 257:
		_wrap_helicsPublicationIsValid(resc, resv, argc, (mxArray**)(argv));
		break;
	case 258:
		_wrap_helicsPublicationPublishBytes(resc, resv, argc, (mxArray**)(argv));
		break;
	case 259:
		_wrap_helicsPublicationPublishString(resc, resv, argc, (mxArray**)(argv));
		break;
	case 260:
		_wrap_helicsPublicationPublishInteger(resc, resv, argc, (mxArray**)(argv));
		break;
	case 261:
		_wrap_helicsPublicationPublishBoolean(resc, resv, argc, (mxArray**)(argv));
		break;
	case 262:
		_wrap_helicsPublicationPublishDouble(resc, resv, argc, (mxArray**)(argv));
		break;
	case 263:
		_wrap_helicsPublicationPublishTime(resc, resv, argc, (mxArray**)(argv));
		break;
	case 264:
		_wrap_helicsPublicationPublishChar(resc, resv, argc, (mxArray**)(argv));
		break;
	case 265:
		_wrap_helicsPublicationPublishComplex(resc, resv, argc, (mxArray**)(argv));
		break;
	case 266:
		_wrap_helicsPublicationPublishVector(resc, resv, argc, (mxArray**)(argv));
		break;
	case 267:
		_wrap_helicsPublicationPublishComplexVector(resc, resv, argc, (mxArray**)(argv));
		break;
	case 268:
		_wrap_helicsPublicationPublishNamedPoint(resc, resv, argc, (mxArray**)(argv));
		break;
	case 269:
		_wrap_helicsPublicationAddTarget(resc, resv, argc, (mxArray**)(argv));
		break;
	case 270:
		_wrap_helicsInputIsValid(resc, resv, argc, (mxArray**)(argv));
		break;
	case 271:
		_wrap_helicsInputAddTarget(resc, resv, argc, (mxArray**)(argv));
		break;
	case 272:
		_wrap_helicsInputGetByteCount(resc, resv, argc, (mxArray**)(argv));
		break;
	case 273:
		_wrap_helicsInputGetBytes(resc, resv, argc, (mxArray**)(argv));
		break;
	case 274:
		_wrap_helicsInputGetStringSize(resc, resv, argc, (mxArray**)(argv));
		break;
	case 275:
		_wrap_helicsInputGetString(resc, resv, argc, (mxArray**)(argv));
		break;
	case 276:
		_wrap_helicsInputGetInteger(resc, resv, argc, (mxArray**)(argv));
		break;
	case 277:
		_wrap_helicsInputGetBoolean(resc, resv, argc, (mxArray**)(argv));
		break;
	case 278:
		_wrap_helicsInputGetDouble(resc, resv, argc, (mxArray**)(argv));
		break;
	case 279:
		_wrap_helicsInputGetTime(resc, resv, argc, (mxArray**)(argv));
		break;
	case 280:
		_wrap_helicsInputGetChar(resc, resv, argc, (mxArray**)(argv));
		break;
	case 281:
		_wrap_helicsInputGetComplexObject(resc, resv, argc, (mxArray**)(argv));
		break;
	case 282:
		_wrap_helicsInputGetComplex(resc, resv, argc, (mxArray**)(argv));
		break;
	case 283:
		_wrap_helicsInputGetVectorSize(resc, resv, argc, (mxArray**)(argv));
		break;
	case 284:
		_wrap_helicsInputGetVector(resc, resv, argc, (mxArray**)(argv));
		break;
	case 285:
		_wrap_helicsInputGetComplexVector(resc, resv, argc, (mxArray**)(argv));
		break;
	case 286:
		_wrap_helicsInputGetNamedPoint(resc, resv, argc, (mxArray**)(argv));
		break;
	case 287:
		_wrap_helicsInputSetDefaultBytes(resc, resv, argc, (mxArray**)(argv));
		break;
	case 288:
		_wrap_helicsInputSetDefaultString(resc, resv, argc, (mxArray**)(argv));
		break;
	case 289:
		_wrap_helicsInputSetDefaultInteger(resc, resv, argc, (mxArray**)(argv));
		break;
	case 290:
		_wrap_helicsInputSetDefaultBoolean(resc, resv, argc, (mxArray**)(argv));
		break;
	case 291:
		_wrap_helicsInputSetDefaultTime(resc, resv, argc, (mxArray**)(argv));
		break;
	case 292:
		_wrap_helicsInputSetDefaultChar(resc, resv, argc, (mxArray**)(argv));
		break;
	case 293:
		_wrap_helicsInputSetDefaultDouble(resc, resv, argc, (mxArray**)(argv));
		break;
	case 294:
		_wrap_helicsInputSetDefaultComplex(resc, resv, argc, (mxArray**)(argv));
		break;
	case 295:
		_wrap_helicsInputSetDefaultVector(resc, resv, argc, (mxArray**)(argv));
		break;
	case 296:
		_wrap_helicsInputSetDefaultComplexVector(resc, resv, argc, (mxArray**)(argv));
		break;
	case 297:
		_wrap_helicsInputSetDefaultNamedPoint(resc, resv, argc, (mxArray**)(argv));
		break;
	case 298:
		_wrap_helicsInputGetType(resc, resv, argc, (mxArray**)(argv));
		break;
	case 299:
		_wrap_helicsInputGetPublicationType(resc, resv, argc, (mxArray**)(argv));
		break;
	case 300:
		_wrap_helicsInputGetPublicationDataType(resc, resv, argc, (mxArray**)(argv));
		break;
	case 301:
		_wrap_helicsPublicationGetType(resc, resv, argc, (mxArray**)(argv));
		break;
	case 302:
		_wrap_helicsInputGetName(resc, resv, argc, (mxArray**)(argv));
		break;
	case 303:
		_wrap_helicsSubscriptionGetTarget(resc, resv, argc, (mxArray**)(argv));
		break;
	case 304:
		_wrap_helicsPublicationGetName(resc, resv, argc, (mxArray**)(argv));
		break;
	case 305:
		_wrap_helicsInputGetUnits(resc, resv, argc, (mxArray**)(argv));
		break;
	case 306:
		_wrap_helicsInputGetInjectionUnits(resc, resv, argc, (mxArray**)(argv));
		break;
	case 307:
		_wrap_helicsInputGetExtractionUnits(resc, resv, argc, (mxArray**)(argv));
		break;
	case 308:
		_wrap_helicsPublicationGetUnits(resc, resv, argc, (mxArray**)(argv));
		break;
	case 309:
		_wrap_helicsInputGetInfo(resc, resv, argc, (mxArray**)(argv));
		break;
	case 310:
		_wrap_helicsInputSetInfo(resc, resv, argc, (mxArray**)(argv));
		break;
	case 311:
		_wrap_helicsInputGetTag(resc, resv, argc, (mxArray**)(argv));
		break;
	case 312:
		_wrap_helicsInputSetTag(resc, resv, argc, (mxArray**)(argv));
		break;
	case 313:
		_wrap_helicsPublicationGetInfo(resc, resv, argc, (mxArray**)(argv));
		break;
	case 314:
		_wrap_helicsPublicationSetInfo(resc, resv, argc, (mxArray**)(argv));
		break;
	case 315:
		_wrap_helicsPublicationGetTag(resc, resv, argc, (mxArray**)(argv));
		break;
	case 316:
		_wrap_helicsPublicationSetTag(resc, resv, argc, (mxArray**)(argv));
		break;
	case 317:
		_wrap_helicsInputGetOption(resc, resv, argc, (mxArray**)(argv));
		break;
	case 318:
		_wrap_helicsInputSetOption(resc, resv, argc, (mxArray**)(argv));
		break;
	case 319:
		_wrap_helicsPublicationGetOption(resc, resv, argc, (mxArray**)(argv));
		break;
	case 320:
		_wrap_helicsPublicationSetOption(resc, resv, argc, (mxArray**)(argv));
		break;
	case 321:
		_wrap_helicsPublicationSetMinimumChange(resc, resv, argc, (mxArray**)(argv));
		break;
	case 322:
		_wrap_helicsInputSetMinimumChange(resc, resv, argc, (mxArray**)(argv));
		break;
	case 323:
		_wrap_helicsInputIsUpdated(resc, resv, argc, (mxArray**)(argv));
		break;
	case 324:
		_wrap_helicsInputLastUpdateTime(resc, resv, argc, (mxArray**)(argv));
		break;
	case 325:
		_wrap_helicsInputClearUpdate(resc, resv, argc, (mxArray**)(argv));
		break;
	case 326:
		_wrap_helicsFederateGetPublicationCount(resc, resv, argc, (mxArray**)(argv));
		break;
	case 327:
		_wrap_helicsFederateGetInputCount(resc, resv, argc, (mxArray**)(argv));
		break;
	case 328:
		_wrap_helicsFederateRegisterEndpoint(resc, resv, argc, (mxArray**)(argv));
		break;
	case 329:
		_wrap_helicsFederateRegisterGlobalEndpoint(resc, resv, argc, (mxArray**)(argv));
		break;
	case 330:
		_wrap_helicsFederateRegisterTargetedEndpoint(resc, resv, argc, (mxArray**)(argv));
		break;
	case 331:
		_wrap_helicsFederateRegisterGlobalTargetedEndpoint(resc, resv, argc, (mxArray**)(argv));
		break;
	case 332:
		_wrap_helicsFederateGetEndpoint(resc, resv, argc, (mxArray**)(argv));
		break;
	case 333:
		_wrap_helicsFederateGetEndpointByIndex(resc, resv, argc, (mxArray**)(argv));
		break;
	case 334:
		_wrap_helicsEndpointIsValid(resc, resv, argc, (mxArray**)(argv));
		break;
	case 335:
		_wrap_helicsEndpointSetDefaultDestination(resc, resv, argc, (mxArray**)(argv));
		break;
	case 336:
		_wrap_helicsEndpointGetDefaultDestination(resc, resv, argc, (mxArray**)(argv));
		break;
	case 337:
		_wrap_helicsEndpointSendBytes(resc, resv, argc, (mxArray**)(argv));
		break;
	case 338:
		_wrap_helicsEndpointSendBytesTo(resc, resv, argc, (mxArray**)(argv));
		break;
	case 339:
		_wrap_helicsEndpointSendBytesToAt(resc, resv, argc, (mxArray**)(argv));
		break;
	case 340:
		_wrap_helicsEndpointSendBytesAt(resc, resv, argc, (mxArray**)(argv));
		break;
	case 341:
		_wrap_helicsEndpointSendMessage(resc, resv, argc, (mxArray**)(argv));
		break;
	case 342:
		_wrap_helicsEndpointSendMessageZeroCopy(resc, resv, argc, (mxArray**)(argv));
		break;
	case 343:
		_wrap_helicsEndpointSubscribe(resc, resv, argc, (mxArray**)(argv));
		break;
	case 344:
		_wrap_helicsFederateHasMessage(resc, resv, argc, (mxArray**)(argv));
		break;
	case 345:
		_wrap_helicsEndpointHasMessage(resc, resv, argc, (mxArray**)(argv));
		break;
	case 346:
		_wrap_helicsFederatePendingMessageCount(resc, resv, argc, (mxArray**)(argv));
		break;
	case 347:
		_wrap_helicsEndpointPendingMessageCount(resc, resv, argc, (mxArray**)(argv));
		break;
	case 348:
		_wrap_helicsEndpointGetMessage(resc, resv, argc, (mxArray**)(argv));
		break;
	case 349:
		_wrap_helicsEndpointCreateMessage(resc, resv, argc, (mxArray**)(argv));
		break;
	case 350:
		_wrap_helicsFederateGetMessage(resc, resv, argc, (mxArray**)(argv));
		break;
	case 351:
		_wrap_helicsFederateCreateMessage(resc, resv, argc, (mxArray**)(argv));
		break;
	case 352:
		_wrap_helicsFederateClearMessages(resc, resv, argc, (mxArray**)(argv));
		break;
	case 353:
		_wrap_helicsEndpointGetType(resc, resv, argc, (mxArray**)(argv));
		break;
	case 354:
		_wrap_helicsEndpointGetName(resc, resv, argc, (mxArray**)(argv));
		break;
	case 355:
		_wrap_helicsFederateGetEndpointCount(resc, resv, argc, (mxArray**)(argv));
		break;
	case 356:
		_wrap_helicsEndpointGetInfo(resc, resv, argc, (mxArray**)(argv));
		break;
	case 357:
		_wrap_helicsEndpointSetInfo(resc, resv, argc, (mxArray**)(argv));
		break;
	case 358:
		_wrap_helicsEndpointGetTag(resc, resv, argc, (mxArray**)(argv));
		break;
	case 359:
		_wrap_helicsEndpointSetTag(resc, resv, argc, (mxArray**)(argv));
		break;
	case 360:
		_wrap_helicsEndpointSetOption(resc, resv, argc, (mxArray**)(argv));
		break;
	case 361:
		_wrap_helicsEndpointGetOption(resc, resv, argc, (mxArray**)(argv));
		break;
	case 362:
		_wrap_helicsEndpointAddSourceTarget(resc, resv, argc, (mxArray**)(argv));
		break;
	case 363:
		_wrap_helicsEndpointAddDestinationTarget(resc, resv, argc, (mxArray**)(argv));
		break;
	case 364:
		_wrap_helicsEndpointRemoveTarget(resc, resv, argc, (mxArray**)(argv));
		break;
	case 365:
		_wrap_helicsEndpointAddSourceFilter(resc, resv, argc, (mxArray**)(argv));
		break;
	case 366:
		_wrap_helicsEndpointAddDestinationFilter(resc, resv, argc, (mxArray**)(argv));
		break;
	case 367:
		_wrap_helicsMessageGetSource(resc, resv, argc, (mxArray**)(argv));
		break;
	case 368:
		_wrap_helicsMessageGetDestination(resc, resv, argc, (mxArray**)(argv));
		break;
	case 369:
		_wrap_helicsMessageGetOriginalSource(resc, resv, argc, (mxArray**)(argv));
		break;
	case 370:
		_wrap_helicsMessageGetOriginalDestination(resc, resv, argc, (mxArray**)(argv));
		break;
	case 371:
		_wrap_helicsMessageGetTime(resc, resv, argc, (mxArray**)(argv));
		break;
	case 372:
		_wrap_helicsMessageGetString(resc, resv, argc, (mxArray**)(argv));
		break;
	case 373:
		_wrap_helicsMessageGetMessageID(resc, resv, argc, (mxArray**)(argv));
		break;
	case 374:
		_wrap_helicsMessageGetFlagOption(resc, resv, argc, (mxArray**)(argv));
		break;
	case 375:
		_wrap_helicsMessageGetByteCount(resc, resv, argc, (mxArray**)(argv));
		break;
	case 376:
		_wrap_helicsMessageGetBytes(resc, resv, argc, (mxArray**)(argv));
		break;
	case 377:
		_wrap_helicsMessageGetBytesPointer(resc, resv, argc, (mxArray**)(argv));
		break;
	case 378:
		_wrap_helicsMessageIsValid(resc, resv, argc, (mxArray**)(argv));
		break;
	case 379:
		_wrap_helicsMessageSetSource(resc, resv, argc, (mxArray**)(argv));
		break;
	case 380:
		_wrap_helicsMessageSetDestination(resc, resv, argc, (mxArray**)(argv));
		break;
	case 381:
		_wrap_helicsMessageSetOriginalSource(resc, resv, argc, (mxArray**)(argv));
		break;
	case 382:
		_wrap_helicsMessageSetOriginalDestination(resc, resv, argc, (mxArray**)(argv));
		break;
	case 383:
		_wrap_helicsMessageSetTime(resc, resv, argc, (mxArray**)(argv));
		break;
	case 384:
		_wrap_helicsMessageResize(resc, resv, argc, (mxArray**)(argv));
		break;
	case 385:
		_wrap_helicsMessageReserve(resc, resv, argc, (mxArray**)(argv));
		break;
	case 386:
		_wrap_helicsMessageSetMessageID(resc, resv, argc, (mxArray**)(argv));
		break;
	case 387:
		_wrap_helicsMessageClearFlags(resc, resv, argc, (mxArray**)(argv));
		break;
	case 388:
		_wrap_helicsMessageSetFlagOption(resc, resv, argc, (mxArray**)(argv));
		break;
	case 389:
		_wrap_helicsMessageSetString(resc, resv, argc, (mxArray**)(argv));
		break;
	case 390:
		_wrap_helicsMessageSetData(resc, resv, argc, (mxArray**)(argv));
		break;
	case 391:
		_wrap_helicsMessageAppendData(resc, resv, argc, (mxArray**)(argv));
		break;
	case 392:
		_wrap_helicsMessageCopy(resc, resv, argc, (mxArray**)(argv));
		break;
	case 393:
		_wrap_helicsMessageClone(resc, resv, argc, (mxArray**)(argv));
		break;
	case 394:
		_wrap_helicsMessageFree(resc, resv, argc, (mxArray**)(argv));
		break;
	case 395:
		_wrap_helicsMessageClear(resc, resv, argc, (mxArray**)(argv));
		break;
	case 396:
		_wrap_helicsFederateRegisterFilter(resc, resv, argc, (mxArray**)(argv));
		break;
	case 397:
		_wrap_helicsFederateRegisterGlobalFilter(resc, resv, argc, (mxArray**)(argv));
		break;
	case 398:
		_wrap_helicsFederateRegisterCloningFilter(resc, resv, argc, (mxArray**)(argv));
		break;
	case 399:
		_wrap_helicsFederateRegisterGlobalCloningFilter(resc, resv, argc, (mxArray**)(argv));
		break;
	case 400:
		_wrap_helicsCoreRegisterFilter(resc, resv, argc, (mxArray**)(argv));
		break;
	case 401:
		_wrap_helicsCoreRegisterCloningFilter(resc, resv, argc, (mxArray**)(argv));
		break;
	case 402:
		_wrap_helicsFederateGetFilterCount(resc, resv, argc, (mxArray**)(argv));
		break;
	case 403:
		_wrap_helicsFederateGetFilter(resc, resv, argc, (mxArray**)(argv));
		break;
	case 404:
		_wrap_helicsFederateGetFilterByIndex(resc, resv, argc, (mxArray**)(argv));
		break;
	case 405:
		_wrap_helicsFilterIsValid(resc, resv, argc, (mxArray**)(argv));
		break;
	case 406:
		_wrap_helicsFilterGetName(resc, resv, argc, (mxArray**)(argv));
		break;
	case 407:
		_wrap_helicsFilterSet(resc, resv, argc, (mxArray**)(argv));
		break;
	case 408:
		_wrap_helicsFilterSetString(resc, resv, argc, (mxArray**)(argv));
		break;
	case 409:
		_wrap_helicsFilterAddDestinationTarget(resc, resv, argc, (mxArray**)(argv));
		break;
	case 410:
		_wrap_helicsFilterAddSourceTarget(resc, resv, argc, (mxArray**)(argv));
		break;
	case 411:
		_wrap_helicsFilterAddDeliveryEndpoint(resc, resv, argc, (mxArray**)(argv));
		break;
	case 412:
		_wrap_helicsFilterRemoveTarget(resc, resv, argc, (mxArray**)(argv));
		break;
	case 413:
		_wrap_helicsFilterRemoveDeliveryEndpoint(resc, resv, argc, (mxArray**)(argv));
		break;
	case 414:
		_wrap_helicsFilterGetInfo(resc, resv, argc, (mxArray**)(argv));
		break;
	case 415:
		_wrap_helicsFilterSetInfo(resc, resv, argc, (mxArray**)(argv));
		break;
	case 416:
		_wrap_helicsFilterGetTag(resc, resv, argc, (mxArray**)(argv));
		break;
	case 417:
		_wrap_helicsFilterSetTag(resc, resv, argc, (mxArray**)(argv));
		break;
	case 418:
		_wrap_helicsFilterSetOption(resc, resv, argc, (mxArray**)(argv));
		break;
	case 419:
		_wrap_helicsFilterGetOption(resc, resv, argc, (mxArray**)(argv));
		break;
	case 420:
		_wrap_helicsFederateRegisterTranslator(resc, resv, argc, (mxArray**)(argv));
		break;
	case 421:
		_wrap_helicsFederateRegisterGlobalTranslator(resc, resv, argc, (mxArray**)(argv));
		break;
	case 422:
		_wrap_helicsCoreRegisterTranslator(resc, resv, argc, (mxArray**)(argv));
		break;
	case 423:
		_wrap_helicsFederateGetTranslatorCount(resc, resv, argc, (mxArray**)(argv));
		break;
	case 424:
		_wrap_helicsFederateGetTranslator(resc, resv, argc, (mxArray**)(argv));
		break;
	case 425:
		_wrap_helicsFederateGetTranslatorByIndex(resc, resv, argc, (mxArray**)(argv));
		break;
	case 426:
		_wrap_helicsTranslatorIsValid(resc, resv, argc, (mxArray**)(argv));
		break;
	case 427:
		_wrap_helicsTranslatorGetName(resc, resv, argc, (mxArray**)(argv));
		break;
	case 428:
		_wrap_helicsTranslatorSet(resc, resv, argc, (mxArray**)(argv));
		break;
	case 429:
		_wrap_helicsTranslatorSetString(resc, resv, argc, (mxArray**)(argv));
		break;
	case 430:
		_wrap_helicsTranslatorAddInputTarget(resc, resv, argc, (mxArray**)(argv));
		break;
	case 431:
		_wrap_helicsTranslatorAddPublicationTarget(resc, resv, argc, (mxArray**)(argv));
		break;
	case 432:
		_wrap_helicsTranslatorAddSourceEndpoint(resc, resv, argc, (mxArray**)(argv));
		break;
	case 433:
		_wrap_helicsTranslatorAddDestinationEndpoint(resc, resv, argc, (mxArray**)(argv));
		break;
	case 434:
		_wrap_helicsTranslatorRemoveTarget(resc, resv, argc, (mxArray**)(argv));
		break;
	case 435:
		_wrap_helicsTranslatorGetInfo(resc, resv, argc, (mxArray**)(argv));
		break;
	case 436:
		_wrap_helicsTranslatorSetInfo(resc, resv, argc, (mxArray**)(argv));
		break;
	case 437:
		_wrap_helicsTranslatorGetTag(resc, resv, argc, (mxArray**)(argv));
		break;
	case 438:
		_wrap_helicsTranslatorSetTag(resc, resv, argc, (mxArray**)(argv));
		break;
	case 439:
		_wrap_helicsTranslatorSetOption(resc, resv, argc, (mxArray**)(argv));
		break;
	case 440:
		_wrap_helicsTranslatorGetOption(resc, resv, argc, (mxArray**)(argv));
		break;
	case 441:
		_wrap_helicsBrokerSetLoggingCallback(resc, resv, argc, (mxArray**)(argv));
		break;
	case 442:
		_wrap_helicsCoreSetLoggingCallback(resc, resv, argc, (mxArray**)(argv));
		break;
	case 443:
		_wrap_helicsFederateSetLoggingCallback(resc, resv, argc, (mxArray**)(argv));
		break;
	case 444:
		_wrap_helicsFilterSetCustomCallback(resc, resv, argc, (mxArray**)(argv));
		break;
	case 445:
		_wrap_helicsFederateSetQueryCallback(resc, resv, argc, (mxArray**)(argv));
		break;
	case 446:
		_wrap_helicsFederateSetTimeRequestEntryCallback(resc, resv, argc, (mxArray**)(argv));
		break;
	case 447:
		_wrap_helicsFederateSetTimeUpdateCallback(resc, resv, argc, (mxArray**)(argv));
		break;
	case 448:
		_wrap_helicsFederateSetStateChangeCallback(resc, resv, argc, (mxArray**)(argv));
		break;
	case 449:
		_wrap_helicsFederateSetTimeRequestReturnCallback(resc, resv, argc, (mxArray**)(argv));
		break;
	case 450:
		_wrap_helicsQueryBufferFill(resc, resv, argc, (mxArray**)(argv));
		break;
	default:
		mexErrMsgIdAndTxt("helics:mexFunction","An unknown function id was encountered. Call the mex function with a valid function id.");
	}
}

void _wrap_HELICS_DATA_TYPE_CHAR(int resc, mxArray *resv[], int argc, mxArray *argv[]){
	mxArray *out = mxCreateNumericMatrix(1,1,mxINT64_CLASS,mxREAL);
	*((int64_T*)mxGetData(out)) = (int64_t)HELICS_DATA_TYPE_CHAR;
	resv[0] = out;
}

void _wrap_helicsCreateDataBuffer(resc, resv, argc, (mxArray**)(argv)){
	int32_t initialCapacity = *((int32_t *)mxGetData(argv[0]));

\HelicsDataBuffer result = helicsCreateDataBuffer(initialCapacity);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_T*)mxGetData(_out)) = (uint64_T)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsWrapDataInBuffer(resc, resv, argc, (mxArray**)(argv)){
	void *data = mxGetData(argv[0]);

	int dataSize = (int)(mxGetScalar(argv[1]));

	int dataCapacity = (int)(mxGetScalar(argv[2]));

\HelicsDataBuffer result = helicsWrapDataInBuffer(data, dataSize, dataCapacity);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_T*)mxGetData(_out)) = (uint64_T)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}






}


void _wrap_helicsDataBufferFree(resc, resv, argc, (mxArray**)(argv)){
	HelicsDataBuffer data = (HelicsDataBuffer)(mxGetData(argv[0]));

	helicsDataBufferFree(data);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsDataBufferSize(resc, resv, argc, (mxArray**)(argv)){
	HelicsDataBuffer data = (HelicsDataBuffer)(mxGetData(argv[0]));

	int32_t result = helicsDataBufferSize(data);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
	*((int64_T*)mxGetData(out)) = (int64_T)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsDataBufferCapacity(resc, resv, argc, (mxArray**)(argv)){
	HelicsDataBuffer data = (HelicsDataBuffer)(mxGetData(argv[0]));

	int32_t result = helicsDataBufferCapacity(data);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
	*((int64_T*)mxGetData(out)) = (int64_T)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsDataBufferData(resc, resv, argc, (mxArray**)(argv)){
	HelicsDataBuffer data = (HelicsDataBuffer)(mxGetData(argv[0]));

	void *reault = helicsDataBufferData(data);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_T*)mxGetData(_out)) = (uint64_T)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsDataBufferReserve(resc, resv, argc, (mxArray**)(argv)){
	HelicsDataBuffer data = (HelicsDataBuffer)(mxGetData(argv[0]));

	int32_t newCapacity = *((int32_t *)mxGetData(argv[1]));

	HelicsBool result = helicsDataBufferReserve(data, newCapacity);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
	*((int64_T*)mxGetData(out)) = (int64_T)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}




}


void _wrap_helicsIntToBytes(resc, resv, argc, (mxArray**)(argv)){
	int64_t value = *((int64_t *)mxGetData(argv[0]));

	 HelicsDataBuffer data = helicsCreateDataBuffer(sizeof(int));

	int32_t result = helicsIntToBytes(value, data);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_T*)mxGetData(_out)) = (uint64_T)data;

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsDoubleToBytes(resc, resv, argc, (mxArray**)(argv)){
	double value = mxGetScalar(argv[0]);

	 HelicsDataBuffer data = helicsCreateDataBuffer(sizeof(double));

	int32_t result = helicsDoubleToBytes(value, data);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_T*)mxGetData(_out)) = (uint64_T)data;

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsStringToBytes(resc, resv, argc, (mxArray**)(argv)){
	char *str;
	size_t strLength;
	int strStatus;
	strLength = mxGetN(argv[0]) + 1;
	str = (char *)mxMalloc(strLength);
	strStatus = mxGetString(argv[0], str, strLength);

	 HelicsDataBuffer data = helicsCreateDataBuffer(strLength);

	int32_t result = helicsStringToBytes(str, data);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_T*)mxGetData(_out)) = (uint64_T)data;

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsBoolToBytes(resc, resv, argc, (mxArray**)(argv)){
	HelicsBool value = (HelicsBool)(mxGetScalar(argv[0]));

	 HelicsDataBuffer data = helicsCreateDataBuffer(sizeof(HelicsBool);

	int32_t result = helicsBoolToBytes(value, data);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_T*)mxGetData(_out)) = (uint64_T)data;

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsCharToBytes(resc, resv, argc, (mxArray**)(argv)){
	char *value;
	size_t valueLength;
	int valueStatus;
	valueLength = mxGetN(argv[0]) + 1;
	value = (char *)mxMalloc(valueLength);
	valueStatus = mxGetString(argv[0], value, valueLength);

	 HelicsDataBuffer data = helicsCreateDataBuffer(valueLength);

	int32_t result = helicsCharToBytes(value, data);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_T*)mxGetData(_out)) = (uint64_T)data;

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsTimeToBytes(resc, resv, argc, (mxArray**)(argv)){
	HelicsTime value = (HelicsTime)(mxGetScalar(argv[0]));

	 HelicsDataBuffer data = helicsCreateDataBuffer(sizeof(HelicsTime));

	int32_t result = helicsTimeToBytes(value, data);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_T*)mxGetData(_out)) = (uint64_T)data;

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsComplexToBytes(resc, resv, argc, (mxArray**)(argv)){
	mxComplexDouble *value = xmGetComplexDoubles(argv[1]);

	 HelicsDataBuffer data = helicsCreateDataBuffer(2*sizeof(double));

	int32_t result = helicsComplexToBytes(value->real, value->imag, data);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_T*)mxGetData(_out)) = (uint64_T)data;

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsVectorToBytes(resc, resv, argc, (mxArray**)(argv)){
	int dataSize =  (int)mxGetNumberOfElements(argv[0]);

	double *value =  (double *)mxGetDoubles(argv[0]);

	 HelicsDataBuffer data = helicsCreateDataBuffer(dataSize*sizeof(double));

	int32_t result = helicsVectorToBytes((const double *)value, dataSize, data);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_T*)mxGetData(_out)) = (uint64_T)data;

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsDataBufferType(resc, resv, argc, (mxArray**)(argv)){
	HelicsDataBuffer data = (HelicsDataBuffer)(mxGetData(argv[0]));

	int result = helicsDataBufferType(data);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
	*((int64_T*)mxGetData(out)) = (int64_T)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsDataBufferToInt(resc, resv, argc, (mxArray**)(argv)){
	HelicsDataBuffer data = (HelicsDataBuffer)(mxGetData(argv[0]));

	int64_t result = helicsDataBufferToInt(data);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
	*((int64_T*)mxGetData(out)) = (int64_T)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsDataBufferToDouble(resc, resv, argc, (mxArray**)(argv)){
	HelicsDataBuffer data = (HelicsDataBuffer)(mxGetData(argv[0]));

	double result = helicsDataBufferToDouble(data);

	mxArray *_out = mxCreateDoubleScalar(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsDataBufferToBool(resc, resv, argc, (mxArray**)(argv)){
	HelicsDataBuffer data = (HelicsDataBuffer)(mxGetData(argv[0]));

	HelicsBool result = helicsDataBufferToBool(data);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
	*((int64_T*)mxGetData(out)) = (int64_T)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsGetVersion(resc, resv, argc, (mxArray**)(argv)){
	char *result = helicsGetVersion();

	mxArray *_out = mxCreateString((const char *)result);

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsGetBuildFlags(resc, resv, argc, (mxArray**)(argv)){
	char *result = helicsGetBuildFlags();

	mxArray *_out = mxCreateString((const char *)result);

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsGetCompilerVersion(resc, resv, argc, (mxArray**)(argv)){
	char *result = helicsGetCompilerVersion();

	mxArray *_out = mxCreateString((const char *)result);

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsGetSystemInfo(resc, resv, argc, (mxArray**)(argv)){
	char *result = helicsGetSystemInfo();

	mxArray *_out = mxCreateString((const char *)result);

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsLoadSignalHandler(resc, resv, argc, (mxArray**)(argv)){
	helicsLoadSignalHandler();

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsLoadThreadedSignalHandler(resc, resv, argc, (mxArray**)(argv)){
	helicsLoadThreadedSignalHandler();

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsClearSignalHandler(resc, resv, argc, (mxArray**)(argv)){
	helicsClearSignalHandler();

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsAbort(resc, resv, argc, (mxArray**)(argv)){
	int errorCode = (int)(mxGetScalar(argv[0]));

	char *errorString;
	size_t errorStringLength;
	int errorStringStatus;
	errorStringLength = mxGetN(argv[1]) + 1;
	errorString = (char *)mxMalloc(errorStringLength);
	errorStringStatus = mxGetString(argv[1], errorString, errorStringLength);

	helicsAbort(errorCode, (char const *)errorString);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	mxFree(errorString);
}


void _wrap_helicsIsCoreTypeAvailable(resc, resv, argc, (mxArray**)(argv)){
	char *type;
	size_t typeLength;
	int typeStatus;
	typeLength = mxGetN(argv[0]) + 1;
	type = (char *)mxMalloc(typeLength);
	typeStatus = mxGetString(argv[0], type, typeLength);

	HelicsBool result = helicsIsCoreTypeAvailable((char const *)type);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
	*((int64_T*)mxGetData(out)) = (int64_T)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	mxFree(type);
}


void _wrap_helicsCreateCore(resc, resv, argc, (mxArray**)(argv)){
	char *type;
	size_t typeLength;
	int typeStatus;
	typeLength = mxGetN(argv[0]) + 1;
	type = (char *)mxMalloc(typeLength);
	typeStatus = mxGetString(argv[0], type, typeLength);

	char *name;
	size_t nameLength;
	int nameStatus;
	nameLength = mxGetN(argv[1]) + 1;
	name = (char *)mxMalloc(nameLength);
	nameStatus = mxGetString(argv[1], name, nameLength);

	char *initString;
	size_t initStringLength;
	int initStringStatus;
	initStringLength = mxGetN(argv[2]) + 1;
	initString = (char *)mxMalloc(initStringLength);
	initStringStatus = mxGetString(argv[2], initString, initStringLength);

	HelicsError err = helicsErrorInitialize();

	HelicsCore result = helicsCreateCore((char const *)type, (char const *)name, (char const *)initString, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_T*)mxGetData(_out)) = (uint64_T)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	mxFree(type);

	mxFree(name);

	mxFree(initString);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsCreateCoreFromArgs(resc, resv, argc, (mxArray**)(argv)){
	char *type;
	size_t typeLength;
	int typeStatus;
	typeLength = mxGetN(argv[0]) + 1;
	type = (char *)mxMalloc(typeLength);
	typeStatus = mxGetString(argv[0], type, typeLength);

	char *name;
	size_t nameLength;
	int nameStatus;
	nameLength = mxGetN(argv[1]) + 1;
	name = (char *)mxMalloc(nameLength);
	nameStatus = mxGetString(argv[1], name, nameLength);

	int arg3 = 0;
	char **arg4 = (char **)0;
	int ii;
	arg3 = static_cast<int>(mxGetNumberOfElements(argv[2]));
	arg4 = (char **)mxMalloc((arg3)*sizeof(char *));
	for (ii=0;ii<arg3;ii++){
		mxArray *cellElement=mxGetCell(argv[2], ii);
		int len = mxGetN(cellElement) + 1;
		arg4[ii] = (char *)mxMalloc(len);
		int flag = mxGetString(cellElement, arg4[ii], len);
	}

	HelicsError err = helicsErrorInitialize();

	HelicsCore result = helicsCreateCoreFromArgs(type, name, arg3, arg4, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_T*)mxGetData(_out)) = (uint64_T)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	mxFree(type);

	mxFree(name);

	mxFree(arg4);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsCoreClone(resc, resv, argc, (mxArray**)(argv)){
	HelicsCore core = (HelicsCore)(mxGetData(argv[0]));

	HelicsError err = helicsErrorInitialize();

	HelicsCore result = helicsCoreClone(core, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_T*)mxGetData(_out)) = (uint64_T)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsCoreIsValid(resc, resv, argc, (mxArray**)(argv)){
	HelicsCore core = (HelicsCore)(mxGetData(argv[0]));

	HelicsBool result = helicsCoreIsValid(core);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
	*((int64_T*)mxGetData(out)) = (int64_T)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsCreateBroker(resc, resv, argc, (mxArray**)(argv)){
	char *type;
	size_t typeLength;
	int typeStatus;
	typeLength = mxGetN(argv[0]) + 1;
	type = (char *)mxMalloc(typeLength);
	typeStatus = mxGetString(argv[0], type, typeLength);

	char *name;
	size_t nameLength;
	int nameStatus;
	nameLength = mxGetN(argv[1]) + 1;
	name = (char *)mxMalloc(nameLength);
	nameStatus = mxGetString(argv[1], name, nameLength);

	char *initString;
	size_t initStringLength;
	int initStringStatus;
	initStringLength = mxGetN(argv[2]) + 1;
	initString = (char *)mxMalloc(initStringLength);
	initStringStatus = mxGetString(argv[2], initString, initStringLength);

	HelicsError err = helicsErrorInitialize();

	HelicsBroker result = helicsCreateBroker((char const *)type, (char const *)name, (char const *)initString, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_T*)mxGetData(_out)) = (uint64_T)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	mxFree(type);

	mxFree(name);

	mxFree(initString);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsCreateBrokerFromArgs(resc, resv, argc, (mxArray**)(argv)){
	char *type;
	size_t typeLength;
	int typeStatus;
	typeLength = mxGetN(argv[0]) + 1;
	type = (char *)mxMalloc(typeLength);
	typeStatus = mxGetString(argv[0], type, typeLength);

	char *name;
	size_t nameLength;
	int nameStatus;
	nameLength = mxGetN(argv[1]) + 1;
	name = (char *)mxMalloc(nameLength);
	nameStatus = mxGetString(argv[1], name, nameLength);

	int arg3 = 0;
	char **arg4 = (char **)0;
	int ii;
	arg2 = static_cast<int>(mxGetNumberOfElements(argv[2]));
	arg3 = (char **)mxMalloc((arg2)*sizeof(char *));
	for (ii=0;ii<arg3;ii++){
		mxArray *cellElement=mxGetCell(argv[2], ii);
		int len = mxGetN(cellElement) + 1;
		arg4[ii] = (char *)mxMalloc(len);
		int flag = mxGetString(cellElement, arg4[ii], len);
	}

	HelicsError err = helicsErrorInitialize();

	HelicsBroker result = helicsCreateBrokerFromArgs(type, name, arg3, arg4, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_T*)mxGetData(_out)) = (uint64_T)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	mxFree(type);

	mxFree(name);

	mxFree(arg4);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsBrokerClone(resc, resv, argc, (mxArray**)(argv)){
	HelicsBroker broker = (HelicsBroker)(mxGetData(argv[0]));

	HelicsError err = helicsErrorInitialize();

	HelicsBroker result = helicsBrokerClone(broker, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_T*)mxGetData(_out)) = (uint64_T)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsBrokerIsValid(resc, resv, argc, (mxArray**)(argv)){
	HelicsBroker broker = (HelicsBroker)(mxGetData(argv[0]));

	HelicsBool result = helicsBrokerIsValid(broker);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
	*((int64_T*)mxGetData(out)) = (int64_T)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsBrokerIsConnected(resc, resv, argc, (mxArray**)(argv)){
	HelicsBroker broker = (HelicsBroker)(mxGetData(argv[0]));

	HelicsBool result = helicsBrokerIsConnected(broker);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
	*((int64_T*)mxGetData(out)) = (int64_T)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsBrokerDataLink(resc, resv, argc, (mxArray**)(argv)){
	HelicsBroker broker = (HelicsBroker)(mxGetData(argv[0]));

	char *source;
	size_t sourceLength;
	int sourceStatus;
	sourceLength = mxGetN(argv[1]) + 1;
	source = (char *)mxMalloc(sourceLength);
	sourceStatus = mxGetString(argv[1], source, sourceLength);

	char *target;
	size_t targetLength;
	int targetStatus;
	targetLength = mxGetN(argv[2]) + 1;
	target = (char *)mxMalloc(targetLength);
	targetStatus = mxGetString(argv[2], target, targetLength);

	HelicsError err = helicsErrorInitialize();

	helicsBrokerDataLink(broker, (char const *)source, (char const *)target, &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	mxFree(source);

	mxFree(target);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsBrokerAddSourceFilterToEndpoint(resc, resv, argc, (mxArray**)(argv)){
	HelicsBroker broker = (HelicsBroker)(mxGetData(argv[0]));

	char *filter;
	size_t filterLength;
	int filterStatus;
	filterLength = mxGetN(argv[1]) + 1;
	filter = (char *)mxMalloc(filterLength);
	filterStatus = mxGetString(argv[1], filter, filterLength);

	char *endpoint;
	size_t endpointLength;
	int endpointStatus;
	endpointLength = mxGetN(argv[2]) + 1;
	endpoint = (char *)mxMalloc(endpointLength);
	endpointStatus = mxGetString(argv[2], endpoint, endpointLength);

	HelicsError err = helicsErrorInitialize();

	helicsBrokerAddSourceFilterToEndpoint(broker, (char const *)filter, (char const *)endpoint, &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	mxFree(filter);

	mxFree(endpoint);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsBrokerAddDestinationFilterToEndpoint(resc, resv, argc, (mxArray**)(argv)){
	HelicsBroker broker = (HelicsBroker)(mxGetData(argv[0]));

	char *filter;
	size_t filterLength;
	int filterStatus;
	filterLength = mxGetN(argv[1]) + 1;
	filter = (char *)mxMalloc(filterLength);
	filterStatus = mxGetString(argv[1], filter, filterLength);

	char *endpoint;
	size_t endpointLength;
	int endpointStatus;
	endpointLength = mxGetN(argv[2]) + 1;
	endpoint = (char *)mxMalloc(endpointLength);
	endpointStatus = mxGetString(argv[2], endpoint, endpointLength);

	HelicsError err = helicsErrorInitialize();

	helicsBrokerAddDestinationFilterToEndpoint(broker, (char const *)filter, (char const *)endpoint, &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	mxFree(filter);

	mxFree(endpoint);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsBrokerMakeConnections(resc, resv, argc, (mxArray**)(argv)){
	HelicsBroker broker = (HelicsBroker)(mxGetData(argv[0]));

	char *file;
	size_t fileLength;
	int fileStatus;
	fileLength = mxGetN(argv[1]) + 1;
	file = (char *)mxMalloc(fileLength);
	fileStatus = mxGetString(argv[1], file, fileLength);

	HelicsError err = helicsErrorInitialize();

	helicsBrokerMakeConnections(broker, (char const *)file, &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	mxFree(file);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsCoreWaitForDisconnect(resc, resv, argc, (mxArray**)(argv)){
	HelicsCore core = (HelicsCore)(mxGetData(argv[0]));

	int msToWait = (int)(mxGetScalar(argv[1]));

	HelicsError err = helicsErrorInitialize();

	HelicsBool result = helicsCoreWaitForDisconnect(core, msToWait, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
	*((int64_T*)mxGetData(out)) = (int64_T)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}





	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsBrokerWaitForDisconnect(resc, resv, argc, (mxArray**)(argv)){
	HelicsBroker broker = (HelicsBroker)(mxGetData(argv[0]));

	int msToWait = (int)(mxGetScalar(argv[1]));

	HelicsError err = helicsErrorInitialize();

	HelicsBool result = helicsBrokerWaitForDisconnect(broker, msToWait, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
	*((int64_T*)mxGetData(out)) = (int64_T)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}





	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsCoreIsConnected(resc, resv, argc, (mxArray**)(argv)){
	HelicsCore core = (HelicsCore)(mxGetData(argv[0]));

	HelicsBool result = helicsCoreIsConnected(core);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
	*((int64_T*)mxGetData(out)) = (int64_T)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsCoreDataLink(resc, resv, argc, (mxArray**)(argv)){
	HelicsCore core = (HelicsCore)(mxGetData(argv[0]));

	char *source;
	size_t sourceLength;
	int sourceStatus;
	sourceLength = mxGetN(argv[1]) + 1;
	source = (char *)mxMalloc(sourceLength);
	sourceStatus = mxGetString(argv[1], source, sourceLength);

	char *target;
	size_t targetLength;
	int targetStatus;
	targetLength = mxGetN(argv[2]) + 1;
	target = (char *)mxMalloc(targetLength);
	targetStatus = mxGetString(argv[2], target, targetLength);

	HelicsError err = helicsErrorInitialize();

	helicsCoreDataLink(core, (char const *)source, (char const *)target, &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	mxFree(source);

	mxFree(target);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsCoreAddSourceFilterToEndpoint(resc, resv, argc, (mxArray**)(argv)){
	HelicsCore core = (HelicsCore)(mxGetData(argv[0]));

	char *filter;
	size_t filterLength;
	int filterStatus;
	filterLength = mxGetN(argv[1]) + 1;
	filter = (char *)mxMalloc(filterLength);
	filterStatus = mxGetString(argv[1], filter, filterLength);

	char *endpoint;
	size_t endpointLength;
	int endpointStatus;
	endpointLength = mxGetN(argv[2]) + 1;
	endpoint = (char *)mxMalloc(endpointLength);
	endpointStatus = mxGetString(argv[2], endpoint, endpointLength);

	HelicsError err = helicsErrorInitialize();

	helicsCoreAddSourceFilterToEndpoint(core, (char const *)filter, (char const *)endpoint, &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	mxFree(filter);

	mxFree(endpoint);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsCoreAddDestinationFilterToEndpoint(resc, resv, argc, (mxArray**)(argv)){
	HelicsCore core = (HelicsCore)(mxGetData(argv[0]));

	char *filter;
	size_t filterLength;
	int filterStatus;
	filterLength = mxGetN(argv[1]) + 1;
	filter = (char *)mxMalloc(filterLength);
	filterStatus = mxGetString(argv[1], filter, filterLength);

	char *endpoint;
	size_t endpointLength;
	int endpointStatus;
	endpointLength = mxGetN(argv[2]) + 1;
	endpoint = (char *)mxMalloc(endpointLength);
	endpointStatus = mxGetString(argv[2], endpoint, endpointLength);

	HelicsError err = helicsErrorInitialize();

	helicsCoreAddDestinationFilterToEndpoint(core, (char const *)filter, (char const *)endpoint, &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	mxFree(filter);

	mxFree(endpoint);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsCoreMakeConnections(resc, resv, argc, (mxArray**)(argv)){
	HelicsCore core = (HelicsCore)(mxGetData(argv[0]));

	char *file;
	size_t fileLength;
	int fileStatus;
	fileLength = mxGetN(argv[1]) + 1;
	file = (char *)mxMalloc(fileLength);
	fileStatus = mxGetString(argv[1], file, fileLength);

	HelicsError err = helicsErrorInitialize();

	helicsCoreMakeConnections(core, (char const *)file, &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	mxFree(file);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsBrokerGetIdentifier(resc, resv, argc, (mxArray**)(argv)){
	HelicsBroker broker = (HelicsBroker)(mxGetData(argv[0]));

	char *result = helicsBrokerGetIdentifier(broker);

	mxArray *_out = mxCreateString((const char *)result);

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsCoreGetIdentifier(resc, resv, argc, (mxArray**)(argv)){
	HelicsCore core = (HelicsCore)(mxGetData(argv[0]));

	char *result = helicsCoreGetIdentifier(core);

	mxArray *_out = mxCreateString((const char *)result);

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsBrokerGetAddress(resc, resv, argc, (mxArray**)(argv)){
	HelicsBroker broker = (HelicsBroker)(mxGetData(argv[0]));

	char *result = helicsBrokerGetAddress(broker);

	mxArray *_out = mxCreateString((const char *)result);

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsCoreGetAddress(resc, resv, argc, (mxArray**)(argv)){
	HelicsCore core = (HelicsCore)(mxGetData(argv[0]));

	char *result = helicsCoreGetAddress(core);

	mxArray *_out = mxCreateString((const char *)result);

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsCoreSetReadyToInit(resc, resv, argc, (mxArray**)(argv)){
	HelicsCore core = (HelicsCore)(mxGetData(argv[0]));

	HelicsError err = helicsErrorInitialize();

	helicsCoreSetReadyToInit(core, &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsCoreConnect(resc, resv, argc, (mxArray**)(argv)){
	HelicsCore core = (HelicsCore)(mxGetData(argv[0]));

	HelicsError err = helicsErrorInitialize();

	HelicsBool result = helicsCoreConnect(core, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
	*((int64_T*)mxGetData(out)) = (int64_T)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsCoreDisconnect(resc, resv, argc, (mxArray**)(argv)){
	HelicsCore core = (HelicsCore)(mxGetData(argv[0]));

	HelicsError err = helicsErrorInitialize();

	helicsCoreDisconnect(core, &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsGetFederateByName(resc, resv, argc, (mxArray**)(argv)){
	char *fedName;
	size_t fedNameLength;
	int fedNameStatus;
	fedNameLength = mxGetN(argv[0]) + 1;
	fedName = (char *)mxMalloc(fedNameLength);
	fedNameStatus = mxGetString(argv[0], fedName, fedNameLength);

	HelicsError err = helicsErrorInitialize();

	HelicsFederate result = helicsGetFederateByName((char const *)fedName, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_T*)mxGetData(_out)) = (uint64_T)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	mxFree(fedName);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsBrokerDisconnect(resc, resv, argc, (mxArray**)(argv)){
	HelicsBroker broker = (HelicsBroker)(mxGetData(argv[0]));

	HelicsError err = helicsErrorInitialize();

	helicsBrokerDisconnect(broker, &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateDestroy(resc, resv, argc, (mxArray**)(argv)){
	HelicsFederate fed = (HelicsFederate)(mxGetData(argv[0]));

	helicsFederateDestroy(fed);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsBrokerDestroy(resc, resv, argc, (mxArray**)(argv)){
	HelicsBroker broker = (HelicsBroker)(mxGetData(argv[0]));

	helicsBrokerDestroy(broker);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsCoreDestroy(resc, resv, argc, (mxArray**)(argv)){
	HelicsCore core = (HelicsCore)(mxGetData(argv[0]));

	helicsCoreDestroy(core);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsCoreFree(resc, resv, argc, (mxArray**)(argv)){
	HelicsCore core = (HelicsCore)(mxGetData(argv[0]));

	helicsCoreFree(core);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsBrokerFree(resc, resv, argc, (mxArray**)(argv)){
	HelicsBroker broker = (HelicsBroker)(mxGetData(argv[0]));

	helicsBrokerFree(broker);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsCreateValueFederate(resc, resv, argc, (mxArray**)(argv)){
	char *fedName;
	size_t fedNameLength;
	int fedNameStatus;
	fedNameLength = mxGetN(argv[0]) + 1;
	fedName = (char *)mxMalloc(fedNameLength);
	fedNameStatus = mxGetString(argv[0], fedName, fedNameLength);

	HelicsFederateInfo fi = (HelicsFederateInfo)(mxGetData(argv[1]));

	HelicsError err = helicsErrorInitialize();

	HelicsFederate result = helicsCreateValueFederate((char const *)fedName, fi, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_T*)mxGetData(_out)) = (uint64_T)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	mxFree(fedName);



	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsCreateValueFederateFromConfig(resc, resv, argc, (mxArray**)(argv)){
	char *configFile;
	size_t configFileLength;
	int configFileStatus;
	configFileLength = mxGetN(argv[0]) + 1;
	configFile = (char *)mxMalloc(configFileLength);
	configFileStatus = mxGetString(argv[0], configFile, configFileLength);

	HelicsError err = helicsErrorInitialize();

	HelicsFederate result = helicsCreateValueFederateFromConfig((char const *)configFile, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_T*)mxGetData(_out)) = (uint64_T)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	mxFree(configFile);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsCreateMessageFederate(resc, resv, argc, (mxArray**)(argv)){
	char *fedName;
	size_t fedNameLength;
	int fedNameStatus;
	fedNameLength = mxGetN(argv[0]) + 1;
	fedName = (char *)mxMalloc(fedNameLength);
	fedNameStatus = mxGetString(argv[0], fedName, fedNameLength);

	HelicsFederateInfo fi = (HelicsFederateInfo)(mxGetData(argv[1]));

	HelicsError err = helicsErrorInitialize();

	HelicsFederate result = helicsCreateMessageFederate((char const *)fedName, fi, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_T*)mxGetData(_out)) = (uint64_T)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	mxFree(fedName);



	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsCreateMessageFederateFromConfig(resc, resv, argc, (mxArray**)(argv)){
	char *configFile;
	size_t configFileLength;
	int configFileStatus;
	configFileLength = mxGetN(argv[0]) + 1;
	configFile = (char *)mxMalloc(configFileLength);
	configFileStatus = mxGetString(argv[0], configFile, configFileLength);

	HelicsError err = helicsErrorInitialize();

	HelicsFederate result = helicsCreateMessageFederateFromConfig((char const *)configFile, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_T*)mxGetData(_out)) = (uint64_T)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	mxFree(configFile);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsCreateCombinationFederate(resc, resv, argc, (mxArray**)(argv)){
	char *fedName;
	size_t fedNameLength;
	int fedNameStatus;
	fedNameLength = mxGetN(argv[0]) + 1;
	fedName = (char *)mxMalloc(fedNameLength);
	fedNameStatus = mxGetString(argv[0], fedName, fedNameLength);

	HelicsFederateInfo fi = (HelicsFederateInfo)(mxGetData(argv[1]));

	HelicsError err = helicsErrorInitialize();

	HelicsFederate result = helicsCreateCombinationFederate((char const *)fedName, fi, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_T*)mxGetData(_out)) = (uint64_T)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	mxFree(fedName);



	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsCreateCombinationFederateFromConfig(resc, resv, argc, (mxArray**)(argv)){
	char *configFile;
	size_t configFileLength;
	int configFileStatus;
	configFileLength = mxGetN(argv[0]) + 1;
	configFile = (char *)mxMalloc(configFileLength);
	configFileStatus = mxGetString(argv[0], configFile, configFileLength);

	HelicsError err = helicsErrorInitialize();

	HelicsFederate result = helicsCreateCombinationFederateFromConfig((char const *)configFile, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_T*)mxGetData(_out)) = (uint64_T)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	mxFree(configFile);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateClone(resc, resv, argc, (mxArray**)(argv)){
	HelicsFederate fed = (HelicsFederate)(mxGetData(argv[0]));

	HelicsError err = helicsErrorInitialize();

	HelicsFederate result = helicsFederateClone(fed, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_T*)mxGetData(_out)) = (uint64_T)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsCreateFederateInfo(resc, resv, argc, (mxArray**)(argv)){
	HelicsFederateInfo result = helicsCreateFederateInfo();

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_T*)mxGetData(_out)) = (uint64_T)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsFederateInfoClone(resc, resv, argc, (mxArray**)(argv)){
	HelicsFederateInfo fi = (HelicsFederateInfo)(mxGetData(argv[0]));

	HelicsError err = helicsErrorInitialize();

	HelicsFederateInfo result = helicsFederateInfoClone(fi, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_T*)mxGetData(_out)) = (uint64_T)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateInfoLoadFromArgs(resc, resv, argc, (mxArray**)(argv)){
	HelicsFederateInfo fi = (HelicsFederateInfo)(mxGetData(argv[0]));

	int arg2 = 0;
	char **arg3 = (char **)0;
	int ii;
	arg2 = static_cast<int>(mxGetNumberOfElements(argv[1]));
	arg3 = (char **)mxMalloc((arg2)*sizeof(char *));
	for (ii=0;ii<arg2;ii++){
		mxArray *cellElement=mxGetCell(argv[1], ii);
		int len = mxGetN(cellElement) + 1;
		arg3[ii] = (char *)mxMalloc(len);
		int flag = mxGetString(cellElement, arg3[ii], len);
	}

	HelicsError err = helicsErrorInitialize();

	helicsFederateInfoLoadFromArgs(fi, arg2, arg3, &err);

	mxArray *_out = (mxArray *)0;
	if(_out){
		--resc;
		*resv++ = _out;
	}

	mxFree(arg3);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateInfoLoadFromString(resc, resv, argc, (mxArray**)(argv)){
	HelicsFederateInfo fi = (HelicsFederateInfo)(mxGetData(argv[0]));

	char *args;
	size_t argsLength;
	int argsStatus;
	argsLength = mxGetN(argv[1]) + 1;
	args = (char *)mxMalloc(argsLength);
	argsStatus = mxGetString(argv[1], args, argsLength);

	HelicsError err = helicsErrorInitialize();

	helicsFederateInfoLoadFromString(fi, (char const *)args, &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	mxFree(args);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateInfoFree(resc, resv, argc, (mxArray**)(argv)){
	HelicsFederateInfo fi = (HelicsFederateInfo)(mxGetData(argv[0]));

	helicsFederateInfoFree(fi);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsFederateIsValid(resc, resv, argc, (mxArray**)(argv)){
	HelicsFederate fed = (HelicsFederate)(mxGetData(argv[0]));

	HelicsBool result = helicsFederateIsValid(fed);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
	*((int64_T*)mxGetData(out)) = (int64_T)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsFederateInfoSetCoreName(resc, resv, argc, (mxArray**)(argv)){
	HelicsFederateInfo fi = (HelicsFederateInfo)(mxGetData(argv[0]));

	char *corename;
	size_t corenameLength;
	int corenameStatus;
	corenameLength = mxGetN(argv[1]) + 1;
	corename = (char *)mxMalloc(corenameLength);
	corenameStatus = mxGetString(argv[1], corename, corenameLength);

	HelicsError err = helicsErrorInitialize();

	helicsFederateInfoSetCoreName(fi, (char const *)corename, &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	mxFree(corename);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateInfoSetCoreInitString(resc, resv, argc, (mxArray**)(argv)){
	HelicsFederateInfo fi = (HelicsFederateInfo)(mxGetData(argv[0]));

	char *coreInit;
	size_t coreInitLength;
	int coreInitStatus;
	coreInitLength = mxGetN(argv[1]) + 1;
	coreInit = (char *)mxMalloc(coreInitLength);
	coreInitStatus = mxGetString(argv[1], coreInit, coreInitLength);

	HelicsError err = helicsErrorInitialize();

	helicsFederateInfoSetCoreInitString(fi, (char const *)coreInit, &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	mxFree(coreInit);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateInfoSetBrokerInitString(resc, resv, argc, (mxArray**)(argv)){
	HelicsFederateInfo fi = (HelicsFederateInfo)(mxGetData(argv[0]));

	char *brokerInit;
	size_t brokerInitLength;
	int brokerInitStatus;
	brokerInitLength = mxGetN(argv[1]) + 1;
	brokerInit = (char *)mxMalloc(brokerInitLength);
	brokerInitStatus = mxGetString(argv[1], brokerInit, brokerInitLength);

	HelicsError err = helicsErrorInitialize();

	helicsFederateInfoSetBrokerInitString(fi, (char const *)brokerInit, &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	mxFree(brokerInit);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateInfoSetCoreType(resc, resv, argc, (mxArray**)(argv)){
	HelicsFederateInfo fi = (HelicsFederateInfo)(mxGetData(argv[0]));

	int coretype = (int)(mxGetScalar(argv[1]));

	HelicsError err = helicsErrorInitialize();

	helicsFederateInfoSetCoreType(fi, coretype, &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}





	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateInfoSetCoreTypeFromString(resc, resv, argc, (mxArray**)(argv)){
	HelicsFederateInfo fi = (HelicsFederateInfo)(mxGetData(argv[0]));

	char *coretype;
	size_t coretypeLength;
	int coretypeStatus;
	coretypeLength = mxGetN(argv[1]) + 1;
	coretype = (char *)mxMalloc(coretypeLength);
	coretypeStatus = mxGetString(argv[1], coretype, coretypeLength);

	HelicsError err = helicsErrorInitialize();

	helicsFederateInfoSetCoreTypeFromString(fi, (char const *)coretype, &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	mxFree(coretype);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateInfoSetBroker(resc, resv, argc, (mxArray**)(argv)){
	HelicsFederateInfo fi = (HelicsFederateInfo)(mxGetData(argv[0]));

	char *broker;
	size_t brokerLength;
	int brokerStatus;
	brokerLength = mxGetN(argv[1]) + 1;
	broker = (char *)mxMalloc(brokerLength);
	brokerStatus = mxGetString(argv[1], broker, brokerLength);

	HelicsError err = helicsErrorInitialize();

	helicsFederateInfoSetBroker(fi, (char const *)broker, &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	mxFree(broker);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateInfoSetBrokerKey(resc, resv, argc, (mxArray**)(argv)){
	HelicsFederateInfo fi = (HelicsFederateInfo)(mxGetData(argv[0]));

	char *brokerkey;
	size_t brokerkeyLength;
	int brokerkeyStatus;
	brokerkeyLength = mxGetN(argv[1]) + 1;
	brokerkey = (char *)mxMalloc(brokerkeyLength);
	brokerkeyStatus = mxGetString(argv[1], brokerkey, brokerkeyLength);

	HelicsError err = helicsErrorInitialize();

	helicsFederateInfoSetBrokerKey(fi, (char const *)brokerkey, &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	mxFree(brokerkey);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateInfoSetBrokerPort(resc, resv, argc, (mxArray**)(argv)){
	HelicsFederateInfo fi = (HelicsFederateInfo)(mxGetData(argv[0]));

	int brokerPort = (int)(mxGetScalar(argv[1]));

	HelicsError err = helicsErrorInitialize();

	helicsFederateInfoSetBrokerPort(fi, brokerPort, &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}





	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateInfoSetLocalPort(resc, resv, argc, (mxArray**)(argv)){
	HelicsFederateInfo fi = (HelicsFederateInfo)(mxGetData(argv[0]));

	char *localPort;
	size_t localPortLength;
	int localPortStatus;
	localPortLength = mxGetN(argv[1]) + 1;
	localPort = (char *)mxMalloc(localPortLength);
	localPortStatus = mxGetString(argv[1], localPort, localPortLength);

	HelicsError err = helicsErrorInitialize();

	helicsFederateInfoSetLocalPort(fi, (char const *)localPort, &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	mxFree(localPort);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsGetPropertyIndex(resc, resv, argc, (mxArray**)(argv)){
	char *val;
	size_t valLength;
	int valStatus;
	valLength = mxGetN(argv[0]) + 1;
	val = (char *)mxMalloc(valLength);
	valStatus = mxGetString(argv[0], val, valLength);

	int result = helicsGetPropertyIndex((char const *)val);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
	*((int64_T*)mxGetData(out)) = (int64_T)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	mxFree(val);
}


void _wrap_helicsGetFlagIndex(resc, resv, argc, (mxArray**)(argv)){
	char *val;
	size_t valLength;
	int valStatus;
	valLength = mxGetN(argv[0]) + 1;
	val = (char *)mxMalloc(valLength);
	valStatus = mxGetString(argv[0], val, valLength);

	int result = helicsGetFlagIndex((char const *)val);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
	*((int64_T*)mxGetData(out)) = (int64_T)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	mxFree(val);
}


void _wrap_helicsGetOptionIndex(resc, resv, argc, (mxArray**)(argv)){
	char *val;
	size_t valLength;
	int valStatus;
	valLength = mxGetN(argv[0]) + 1;
	val = (char *)mxMalloc(valLength);
	valStatus = mxGetString(argv[0], val, valLength);

	int result = helicsGetOptionIndex((char const *)val);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
	*((int64_T*)mxGetData(out)) = (int64_T)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	mxFree(val);
}


void _wrap_helicsGetOptionValue(resc, resv, argc, (mxArray**)(argv)){
	char *val;
	size_t valLength;
	int valStatus;
	valLength = mxGetN(argv[0]) + 1;
	val = (char *)mxMalloc(valLength);
	valStatus = mxGetString(argv[0], val, valLength);

	int result = helicsGetOptionValue((char const *)val);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
	*((int64_T*)mxGetData(out)) = (int64_T)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	mxFree(val);
}


void _wrap_helicsGetDataType(resc, resv, argc, (mxArray**)(argv)){
	char *val;
	size_t valLength;
	int valStatus;
	valLength = mxGetN(argv[0]) + 1;
	val = (char *)mxMalloc(valLength);
	valStatus = mxGetString(argv[0], val, valLength);

	int result = helicsGetDataType((char const *)val);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
	*((int64_T*)mxGetData(out)) = (int64_T)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	mxFree(val);
}


void _wrap_helicsFederateInfoSetFlagOption(resc, resv, argc, (mxArray**)(argv)){
	HelicsFederateInfo fi = (HelicsFederateInfo)(mxGetData(argv[0]));

	int flag = (int)(mxGetScalar(argv[1]));

	HelicsBool value = (HelicsBool)(mxGetScalar(argv[2]));

	HelicsError err = helicsErrorInitialize();

	helicsFederateInfoSetFlagOption(fi, flag, value, &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}







	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateInfoSetSeparator(resc, resv, argc, (mxArray**)(argv)){
	HelicsFederateInfo fi = (HelicsFederateInfo)(mxGetData(argv[0]));

	char *separator;
	size_t separatorLength;
	int separatorStatus;
	separatorLength = mxGetN(argv[1]) + 1;
	separator = (char *)mxMalloc(separatorLength);
	separatorStatus = mxGetString(argv[1], separator, separatorLength);

	HelicsError err = helicsErrorInitialize();

	helicsFederateInfoSetSeparator(fi, separator[0], &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	mxFree(separator);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateInfoSetTimeProperty(resc, resv, argc, (mxArray**)(argv)){
	HelicsFederateInfo fi = (HelicsFederateInfo)(mxGetData(argv[0]));

	int timeProperty = (int)(mxGetScalar(argv[1]));

	HelicsTime propertyValue = (HelicsTime)(mxGetScalar(argv[2]));

	HelicsError err = helicsErrorInitialize();

	helicsFederateInfoSetTimeProperty(fi, timeProperty, propertyValue, &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}







	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateInfoSetIntegerProperty(resc, resv, argc, (mxArray**)(argv)){
	HelicsFederateInfo fi = (HelicsFederateInfo)(mxGetData(argv[0]));

	int intProperty = (int)(mxGetScalar(argv[1]));

	int propertyValue = (int)(mxGetScalar(argv[2]));

	HelicsError err = helicsErrorInitialize();

	helicsFederateInfoSetIntegerProperty(fi, intProperty, propertyValue, &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}







	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateRegisterInterfaces(resc, resv, argc, (mxArray**)(argv)){
	HelicsFederate fed = (HelicsFederate)(mxGetData(argv[0]));

	char *file;
	size_t fileLength;
	int fileStatus;
	fileLength = mxGetN(argv[1]) + 1;
	file = (char *)mxMalloc(fileLength);
	fileStatus = mxGetString(argv[1], file, fileLength);

	HelicsError err = helicsErrorInitialize();

	helicsFederateRegisterInterfaces(fed, (char const *)file, &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	mxFree(file);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateGlobalError(resc, resv, argc, (mxArray**)(argv)){
	HelicsFederate fed = (HelicsFederate)(mxGetData(argv[0]));

	int errorCode = (int)(mxGetScalar(argv[1]));

	char *errorString;
	size_t errorStringLength;
	int errorStringStatus;
	errorStringLength = mxGetN(argv[2]) + 1;
	errorString = (char *)mxMalloc(errorStringLength);
	errorStringStatus = mxGetString(argv[2], errorString, errorStringLength);

	HelicsError err = helicsErrorInitialize();

	helicsFederateGlobalError(fed, errorCode, (char const *)errorString, &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}





	mxFree(errorString);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateLocalError(resc, resv, argc, (mxArray**)(argv)){
	HelicsFederate fed = (HelicsFederate)(mxGetData(argv[0]));

	int errorCode = (int)(mxGetScalar(argv[1]));

	char *errorString;
	size_t errorStringLength;
	int errorStringStatus;
	errorStringLength = mxGetN(argv[2]) + 1;
	errorString = (char *)mxMalloc(errorStringLength);
	errorStringStatus = mxGetString(argv[2], errorString, errorStringLength);

	HelicsError err = helicsErrorInitialize();

	helicsFederateLocalError(fed, errorCode, (char const *)errorString, &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}





	mxFree(errorString);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateFinalize(resc, resv, argc, (mxArray**)(argv)){
	HelicsFederate fed = (HelicsFederate)(mxGetData(argv[0]));

	HelicsError err = helicsErrorInitialize();

	helicsFederateFinalize(fed, &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateFinalizeAsync(resc, resv, argc, (mxArray**)(argv)){
	HelicsFederate fed = (HelicsFederate)(mxGetData(argv[0]));

	HelicsError err = helicsErrorInitialize();

	helicsFederateFinalizeAsync(fed, &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateFinalizeComplete(resc, resv, argc, (mxArray**)(argv)){
	HelicsFederate fed = (HelicsFederate)(mxGetData(argv[0]));

	HelicsError err = helicsErrorInitialize();

	helicsFederateFinalizeComplete(fed, &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateDisconnect(resc, resv, argc, (mxArray**)(argv)){
	HelicsFederate fed = (HelicsFederate)(mxGetData(argv[0]));

	HelicsError err = helicsErrorInitialize();

	helicsFederateDisconnect(fed, &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateDisconnectAsync(resc, resv, argc, (mxArray**)(argv)){
	HelicsFederate fed = (HelicsFederate)(mxGetData(argv[0]));

	HelicsError err = helicsErrorInitialize();

	helicsFederateDisconnectAsync(fed, &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateDisconnectComplete(resc, resv, argc, (mxArray**)(argv)){
	HelicsFederate fed = (HelicsFederate)(mxGetData(argv[0]));

	HelicsError err = helicsErrorInitialize();

	helicsFederateDisconnectComplete(fed, &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateFree(resc, resv, argc, (mxArray**)(argv)){
	HelicsFederate fed = (HelicsFederate)(mxGetData(argv[0]));

	helicsFederateFree(fed);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsCloseLibrary(resc, resv, argc, (mxArray**)(argv)){
	helicsCloseLibrary();

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsFederateEnterInitializingMode(resc, resv, argc, (mxArray**)(argv)){
	HelicsFederate fed = (HelicsFederate)(mxGetData(argv[0]));

	HelicsError err = helicsErrorInitialize();

	helicsFederateEnterInitializingMode(fed, &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateEnterInitializingModeAsync(resc, resv, argc, (mxArray**)(argv)){
	HelicsFederate fed = (HelicsFederate)(mxGetData(argv[0]));

	HelicsError err = helicsErrorInitialize();

	helicsFederateEnterInitializingModeAsync(fed, &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateIsAsyncOperationCompleted(resc, resv, argc, (mxArray**)(argv)){
	HelicsFederate fed = (HelicsFederate)(mxGetData(argv[0]));

	HelicsError err = helicsErrorInitialize();

	HelicsBool result = helicsFederateIsAsyncOperationCompleted(fed, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
	*((int64_T*)mxGetData(out)) = (int64_T)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateEnterInitializingModeComplete(resc, resv, argc, (mxArray**)(argv)){
	HelicsFederate fed = (HelicsFederate)(mxGetData(argv[0]));

	HelicsError err = helicsErrorInitialize();

	helicsFederateEnterInitializingModeComplete(fed, &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateEnterExecutingMode(resc, resv, argc, (mxArray**)(argv)){
	HelicsFederate fed = (HelicsFederate)(mxGetData(argv[0]));

	HelicsError err = helicsErrorInitialize();

	helicsFederateEnterExecutingMode(fed, &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateEnterExecutingModeAsync(resc, resv, argc, (mxArray**)(argv)){
	HelicsFederate fed = (HelicsFederate)(mxGetData(argv[0]));

	HelicsError err = helicsErrorInitialize();

	helicsFederateEnterExecutingModeAsync(fed, &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateEnterExecutingModeComplete(resc, resv, argc, (mxArray**)(argv)){
	HelicsFederate fed = (HelicsFederate)(mxGetData(argv[0]));

	HelicsError err = helicsErrorInitialize();

	helicsFederateEnterExecutingModeComplete(fed, &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateEnterExecutingModeIterative(resc, resv, argc, (mxArray**)(argv)){
	HelicsFederate fed = (HelicsFederate)(mxGetData(argv[0]));

	HelicsIterationRequest iterate = (HelicsIterationRequest)(mxGetScalar(argv[1]));

	HelicsError err = helicsErrorInitialize();

	HelicsIterationResult result = helicsFederateEnterExecutingModeIterative(fed, iterate, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
	*((int64_T*)mxGetData(out)) = (int64_T)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}





	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateEnterExecutingModeIterativeAsync(resc, resv, argc, (mxArray**)(argv)){
	HelicsFederate fed = (HelicsFederate)(mxGetData(argv[0]));

	HelicsIterationRequest iterate = (HelicsIterationRequest)(mxGetScalar(argv[1]));

	HelicsError err = helicsErrorInitialize();

	helicsFederateEnterExecutingModeIterativeAsync(fed, iterate, &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}





	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateEnterExecutingModeIterativeComplete(resc, resv, argc, (mxArray**)(argv)){
	HelicsFederate fed = (HelicsFederate)(mxGetData(argv[0]));

	HelicsError err = helicsErrorInitialize();

	HelicsIterationResult result = helicsFederateEnterExecutingModeIterativeComplete(fed, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
	*((int64_T*)mxGetData(out)) = (int64_T)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateGetState(resc, resv, argc, (mxArray**)(argv)){
	HelicsFederate fed = (HelicsFederate)(mxGetData(argv[0]));

	HelicsError err = helicsErrorInitialize();

	HelicsFederateState result = helicsFederateGetState(fed, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
	*((int64_T*)mxGetData(out)) = (int64_T)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateGetCore(resc, resv, argc, (mxArray**)(argv)){
	HelicsFederate fed = (HelicsFederate)(mxGetData(argv[0]));

	HelicsError err = helicsErrorInitialize();

	HelicsCore result = helicsFederateGetCore(fed, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_T*)mxGetData(_out)) = (uint64_T)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateRequestTime(resc, resv, argc, (mxArray**)(argv)){
	HelicsFederate fed = (HelicsFederate)(mxGetData(argv[0]));

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


void _wrap_helicsFederateRequestTimeAdvance(resc, resv, argc, (mxArray**)(argv)){
	HelicsFederate fed = (HelicsFederate)(mxGetData(argv[0]));

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


void _wrap_helicsFederateRequestNextStep(resc, resv, argc, (mxArray**)(argv)){
	HelicsFederate fed = (HelicsFederate)(mxGetData(argv[0]));

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


void _wrap_helicsFederateRequestTimeIterative(resc, resv, argc, (mxArray**)(argv)){
	HelicsFederate fed = (HelicsFederate)(mxGetData(argv[0]));

	HelicsTime requestTime = (HelicsTime)(mxGetScalar(argv[1]));

	HelicsIterationRequest iterate = (HelicsIterationRequest)(mxGetScalar(argv[2]));

	HelicsIterationResult *outIteration = (HelicsIterationResult *)0;

	HelicsError err = helicsErrorInitialize();

	HelicsTime result = helicsFederateRequestTimeIterative(fed, requestTime, iterate, outIteration, &err);

	mxArray *_out = mxCreateDoubleScalar(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(--resc>=0){
		mxArray *_out1 = mxCreateNumericMatrix(1,1,mxINT64_CLASS,mxREAL);
		*((int64_T*)mxGetData(_out1)) = (int64_T)outIteration;
		*resv++ = _out1;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateRequestTimeAsync(resc, resv, argc, (mxArray**)(argv)){
	HelicsFederate fed = (HelicsFederate)(mxGetData(argv[0]));

	HelicsTime requestTime = (HelicsTime)(mxGetScalar(argv[1]));

	HelicsError err = helicsErrorInitialize();

	helicsFederateRequestTimeAsync(fed, requestTime, &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}





	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateRequestTimeComplete(resc, resv, argc, (mxArray**)(argv)){
	HelicsFederate fed = (HelicsFederate)(mxGetData(argv[0]));

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


void _wrap_helicsFederateRequestTimeIterativeAsync(resc, resv, argc, (mxArray**)(argv)){
	HelicsFederate fed = (HelicsFederate)(mxGetData(argv[0]));

	HelicsTime requestTime = (HelicsTime)(mxGetScalar(argv[1]));

	HelicsIterationRequest iterate = (HelicsIterationRequest)(mxGetScalar(argv[2]));

	HelicsError err = helicsErrorInitialize();

	helicsFederateRequestTimeIterativeAsync(fed, requestTime, iterate, &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}







	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateRequestTimeIterativeComplete(resc, resv, argc, (mxArray**)(argv)){
	HelicsFederate fed = (HelicsFederate)(mxGetData(argv[0]));

	HelicsTime requestTime = (HelicsTime)(mxGetScalar(argv[1]));

	HelicsIterationResult *outIteration = (HelicsIterationResult *)0;

	HelicsError err = helicsErrorInitialize();

	HelicsTime result = helicsFederateRequestTimeIterativeComplete(fed, requestTime, outIteration, &err);

	mxArray *_out = mxCreateDoubleScalar(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(--resc>=0){
		mxArray *_out1 = mxCreateNumericMatrix(1,1,mxINT64_CLASS,mxREAL);
		*((int64_T*)mxGetData(_out1)) = (int64_T)outIteration;
		*resv++ = _out1;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateProcessCommunications(resc, resv, argc, (mxArray**)(argv)){
	HelicsFederate fed = (HelicsFederate)(mxGetData(argv[0]));

	HelicsTime period = (HelicsTime)(mxGetScalar(argv[1]));

	HelicsError err = helicsErrorInitialize();

	helicsFederateProcessCommunications(fed, period, &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}





	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateGetName(resc, resv, argc, (mxArray**)(argv)){
	HelicsFederate fed = (HelicsFederate)(mxGetData(argv[0]));

	char *result = helicsFederateGetName(fed);

	mxArray *_out = mxCreateString((const char *)result);

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsFederateSetTimeProperty(resc, resv, argc, (mxArray**)(argv)){
	HelicsFederate fed = (HelicsFederate)(mxGetData(argv[0]));

	int timeProperty = (int)(mxGetScalar(argv[1]));

	HelicsTime time = (HelicsTime)(mxGetScalar(argv[2]));

	HelicsError err = helicsErrorInitialize();

	helicsFederateSetTimeProperty(fed, timeProperty, time, &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}







	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateSetFlagOption(resc, resv, argc, (mxArray**)(argv)){
	HelicsFederate fed = (HelicsFederate)(mxGetData(argv[0]));

	int flag = (int)(mxGetScalar(argv[1]));

	HelicsBool flagValue = (HelicsBool)(mxGetScalar(argv[2]));

	HelicsError err = helicsErrorInitialize();

	helicsFederateSetFlagOption(fed, flag, flagValue, &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}







	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateSetSeparator(resc, resv, argc, (mxArray**)(argv)){
	HelicsFederate fed = (HelicsFederate)(mxGetData(argv[0]));

	char *separator;
	size_t separatorLength;
	int separatorStatus;
	separatorLength = mxGetN(argv[1]) + 1;
	separator = (char *)mxMalloc(separatorLength);
	separatorStatus = mxGetString(argv[1], separator, separatorLength);

	HelicsError err = helicsErrorInitialize();

	helicsFederateSetSeparator(fed, separator[0], &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	mxFree(separator);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateSetIntegerProperty(resc, resv, argc, (mxArray**)(argv)){
	HelicsFederate fed = (HelicsFederate)(mxGetData(argv[0]));

	int intProperty = (int)(mxGetScalar(argv[1]));

	int propertyVal = (int)(mxGetScalar(argv[2]));

	HelicsError err = helicsErrorInitialize();

	helicsFederateSetIntegerProperty(fed, intProperty, propertyVal, &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}







	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateGetTimeProperty(resc, resv, argc, (mxArray**)(argv)){
	HelicsFederate fed = (HelicsFederate)(mxGetData(argv[0]));

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


void _wrap_helicsFederateGetFlagOption(resc, resv, argc, (mxArray**)(argv)){
	HelicsFederate fed = (HelicsFederate)(mxGetData(argv[0]));

	int flag = (int)(mxGetScalar(argv[1]));

	HelicsError err = helicsErrorInitialize();

	HelicsBool result = helicsFederateGetFlagOption(fed, flag, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
	*((int64_T*)mxGetData(out)) = (int64_T)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}





	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateGetIntegerProperty(resc, resv, argc, (mxArray**)(argv)){
	HelicsFederate fed = (HelicsFederate)(mxGetData(argv[0]));

	int intProperty = (int)(mxGetScalar(argv[1]));

	HelicsError err = helicsErrorInitialize();

	int result = helicsFederateGetIntegerProperty(fed, intProperty, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
	*((int64_T*)mxGetData(out)) = (int64_T)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}





	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateGetCurrentTime(resc, resv, argc, (mxArray**)(argv)){
	HelicsFederate fed = (HelicsFederate)(mxGetData(argv[0]));

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


void _wrap_helicsFederateSetGlobal(resc, resv, argc, (mxArray**)(argv)){
	HelicsFederate fed = (HelicsFederate)(mxGetData(argv[0]));

	char *valueName;
	size_t valueNameLength;
	int valueNameStatus;
	valueNameLength = mxGetN(argv[1]) + 1;
	valueName = (char *)mxMalloc(valueNameLength);
	valueNameStatus = mxGetString(argv[1], valueName, valueNameLength);

	char *value;
	size_t valueLength;
	int valueStatus;
	valueLength = mxGetN(argv[2]) + 1;
	value = (char *)mxMalloc(valueLength);
	valueStatus = mxGetString(argv[2], value, valueLength);

	HelicsError err = helicsErrorInitialize();

	helicsFederateSetGlobal(fed, (char const *)valueName, (char const *)value, &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	mxFree(valueName);

	mxFree(value);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateSetTag(resc, resv, argc, (mxArray**)(argv)){
	HelicsFederate fed = (HelicsFederate)(mxGetData(argv[0]));

	char *tagName;
	size_t tagNameLength;
	int tagNameStatus;
	tagNameLength = mxGetN(argv[1]) + 1;
	tagName = (char *)mxMalloc(tagNameLength);
	tagNameStatus = mxGetString(argv[1], tagName, tagNameLength);

	char *value;
	size_t valueLength;
	int valueStatus;
	valueLength = mxGetN(argv[2]) + 1;
	value = (char *)mxMalloc(valueLength);
	valueStatus = mxGetString(argv[2], value, valueLength);

	HelicsError err = helicsErrorInitialize();

	helicsFederateSetTag(fed, (char const *)tagName, (char const *)value, &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	mxFree(tagName);

	mxFree(value);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateGetTag(resc, resv, argc, (mxArray**)(argv)){
	HelicsFederate fed = (HelicsFederate)(mxGetData(argv[0]));

	char *tagName;
	size_t tagNameLength;
	int tagNameStatus;
	tagNameLength = mxGetN(argv[1]) + 1;
	tagName = (char *)mxMalloc(tagNameLength);
	tagNameStatus = mxGetString(argv[1], tagName, tagNameLength);

	HelicsError err = helicsErrorInitialize();

	char *result = helicsFederateGetTag(fed, (char const *)tagName, &err);

	mxArray *_out = mxCreateString((const char *)result);

	if(_out){
		--resc;
		*resv++ = _out;
	}



	mxFree(tagName);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateAddDependency(resc, resv, argc, (mxArray**)(argv)){
	HelicsFederate fed = (HelicsFederate)(mxGetData(argv[0]));

	char *fedName;
	size_t fedNameLength;
	int fedNameStatus;
	fedNameLength = mxGetN(argv[1]) + 1;
	fedName = (char *)mxMalloc(fedNameLength);
	fedNameStatus = mxGetString(argv[1], fedName, fedNameLength);

	HelicsError err = helicsErrorInitialize();

	helicsFederateAddDependency(fed, (char const *)fedName, &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	mxFree(fedName);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateSetLogFile(resc, resv, argc, (mxArray**)(argv)){
	HelicsFederate fed = (HelicsFederate)(mxGetData(argv[0]));

	char *logFile;
	size_t logFileLength;
	int logFileStatus;
	logFileLength = mxGetN(argv[1]) + 1;
	logFile = (char *)mxMalloc(logFileLength);
	logFileStatus = mxGetString(argv[1], logFile, logFileLength);

	HelicsError err = helicsErrorInitialize();

	helicsFederateSetLogFile(fed, (char const *)logFile, &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	mxFree(logFile);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateLogErrorMessage(resc, resv, argc, (mxArray**)(argv)){
	HelicsFederate fed = (HelicsFederate)(mxGetData(argv[0]));

	char *logmessage;
	size_t logmessageLength;
	int logmessageStatus;
	logmessageLength = mxGetN(argv[1]) + 1;
	logmessage = (char *)mxMalloc(logmessageLength);
	logmessageStatus = mxGetString(argv[1], logmessage, logmessageLength);

	HelicsError err = helicsErrorInitialize();

	helicsFederateLogErrorMessage(fed, (char const *)logmessage, &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	mxFree(logmessage);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateLogWarningMessage(resc, resv, argc, (mxArray**)(argv)){
	HelicsFederate fed = (HelicsFederate)(mxGetData(argv[0]));

	char *logmessage;
	size_t logmessageLength;
	int logmessageStatus;
	logmessageLength = mxGetN(argv[1]) + 1;
	logmessage = (char *)mxMalloc(logmessageLength);
	logmessageStatus = mxGetString(argv[1], logmessage, logmessageLength);

	HelicsError err = helicsErrorInitialize();

	helicsFederateLogWarningMessage(fed, (char const *)logmessage, &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	mxFree(logmessage);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateLogInfoMessage(resc, resv, argc, (mxArray**)(argv)){
	HelicsFederate fed = (HelicsFederate)(mxGetData(argv[0]));

	char *logmessage;
	size_t logmessageLength;
	int logmessageStatus;
	logmessageLength = mxGetN(argv[1]) + 1;
	logmessage = (char *)mxMalloc(logmessageLength);
	logmessageStatus = mxGetString(argv[1], logmessage, logmessageLength);

	HelicsError err = helicsErrorInitialize();

	helicsFederateLogInfoMessage(fed, (char const *)logmessage, &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	mxFree(logmessage);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateLogDebugMessage(resc, resv, argc, (mxArray**)(argv)){
	HelicsFederate fed = (HelicsFederate)(mxGetData(argv[0]));

	char *logmessage;
	size_t logmessageLength;
	int logmessageStatus;
	logmessageLength = mxGetN(argv[1]) + 1;
	logmessage = (char *)mxMalloc(logmessageLength);
	logmessageStatus = mxGetString(argv[1], logmessage, logmessageLength);

	HelicsError err = helicsErrorInitialize();

	helicsFederateLogDebugMessage(fed, (char const *)logmessage, &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	mxFree(logmessage);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateLogLevelMessage(resc, resv, argc, (mxArray**)(argv)){
	HelicsFederate fed = (HelicsFederate)(mxGetData(argv[0]));

	int loglevel = (int)(mxGetScalar(argv[1]));

	char *logmessage;
	size_t logmessageLength;
	int logmessageStatus;
	logmessageLength = mxGetN(argv[2]) + 1;
	logmessage = (char *)mxMalloc(logmessageLength);
	logmessageStatus = mxGetString(argv[2], logmessage, logmessageLength);

	HelicsError err = helicsErrorInitialize();

	helicsFederateLogLevelMessage(fed, loglevel, (char const *)logmessage, &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}





	mxFree(logmessage);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateSendCommand(resc, resv, argc, (mxArray**)(argv)){
	HelicsFederate fed = (HelicsFederate)(mxGetData(argv[0]));

	char *target;
	size_t targetLength;
	int targetStatus;
	targetLength = mxGetN(argv[1]) + 1;
	target = (char *)mxMalloc(targetLength);
	targetStatus = mxGetString(argv[1], target, targetLength);

	char *command;
	size_t commandLength;
	int commandStatus;
	commandLength = mxGetN(argv[2]) + 1;
	command = (char *)mxMalloc(commandLength);
	commandStatus = mxGetString(argv[2], command, commandLength);

	HelicsError err = helicsErrorInitialize();

	helicsFederateSendCommand(fed, (char const *)target, (char const *)command, &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	mxFree(target);

	mxFree(command);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateGetCommand(resc, resv, argc, (mxArray**)(argv)){
	HelicsFederate fed = (HelicsFederate)(mxGetData(argv[0]));

	HelicsError err = helicsErrorInitialize();

	char *result = helicsFederateGetCommand(fed, &err);

	mxArray *_out = mxCreateString((const char *)result);

	if(_out){
		--resc;
		*resv++ = _out;
	}



	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateGetCommandSource(resc, resv, argc, (mxArray**)(argv)){
	HelicsFederate fed = (HelicsFederate)(mxGetData(argv[0]));

	HelicsError err = helicsErrorInitialize();

	char *result = helicsFederateGetCommandSource(fed, &err);

	mxArray *_out = mxCreateString((const char *)result);

	if(_out){
		--resc;
		*resv++ = _out;
	}



	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateWaitCommand(resc, resv, argc, (mxArray**)(argv)){
	HelicsFederate fed = (HelicsFederate)(mxGetData(argv[0]));

	HelicsError err = helicsErrorInitialize();

	char *result = helicsFederateWaitCommand(fed, &err);

	mxArray *_out = mxCreateString((const char *)result);

	if(_out){
		--resc;
		*resv++ = _out;
	}



	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsCoreSetGlobal(resc, resv, argc, (mxArray**)(argv)){
	HelicsCore core = (HelicsCore)(mxGetData(argv[0]));

	char *valueName;
	size_t valueNameLength;
	int valueNameStatus;
	valueNameLength = mxGetN(argv[1]) + 1;
	valueName = (char *)mxMalloc(valueNameLength);
	valueNameStatus = mxGetString(argv[1], valueName, valueNameLength);

	char *value;
	size_t valueLength;
	int valueStatus;
	valueLength = mxGetN(argv[2]) + 1;
	value = (char *)mxMalloc(valueLength);
	valueStatus = mxGetString(argv[2], value, valueLength);

	HelicsError err = helicsErrorInitialize();

	helicsCoreSetGlobal(core, (char const *)valueName, (char const *)value, &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	mxFree(valueName);

	mxFree(value);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsBrokerSetGlobal(resc, resv, argc, (mxArray**)(argv)){
	HelicsBroker broker = (HelicsBroker)(mxGetData(argv[0]));

	char *valueName;
	size_t valueNameLength;
	int valueNameStatus;
	valueNameLength = mxGetN(argv[1]) + 1;
	valueName = (char *)mxMalloc(valueNameLength);
	valueNameStatus = mxGetString(argv[1], valueName, valueNameLength);

	char *value;
	size_t valueLength;
	int valueStatus;
	valueLength = mxGetN(argv[2]) + 1;
	value = (char *)mxMalloc(valueLength);
	valueStatus = mxGetString(argv[2], value, valueLength);

	HelicsError err = helicsErrorInitialize();

	helicsBrokerSetGlobal(broker, (char const *)valueName, (char const *)value, &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	mxFree(valueName);

	mxFree(value);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsCoreSendCommand(resc, resv, argc, (mxArray**)(argv)){
	HelicsCore core = (HelicsCore)(mxGetData(argv[0]));

	char *target;
	size_t targetLength;
	int targetStatus;
	targetLength = mxGetN(argv[1]) + 1;
	target = (char *)mxMalloc(targetLength);
	targetStatus = mxGetString(argv[1], target, targetLength);

	char *command;
	size_t commandLength;
	int commandStatus;
	commandLength = mxGetN(argv[2]) + 1;
	command = (char *)mxMalloc(commandLength);
	commandStatus = mxGetString(argv[2], command, commandLength);

	HelicsError err = helicsErrorInitialize();

	helicsCoreSendCommand(core, (char const *)target, (char const *)command, &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	mxFree(target);

	mxFree(command);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsBrokerSendCommand(resc, resv, argc, (mxArray**)(argv)){
	HelicsBroker broker = (HelicsBroker)(mxGetData(argv[0]));

	char *target;
	size_t targetLength;
	int targetStatus;
	targetLength = mxGetN(argv[1]) + 1;
	target = (char *)mxMalloc(targetLength);
	targetStatus = mxGetString(argv[1], target, targetLength);

	char *command;
	size_t commandLength;
	int commandStatus;
	commandLength = mxGetN(argv[2]) + 1;
	command = (char *)mxMalloc(commandLength);
	commandStatus = mxGetString(argv[2], command, commandLength);

	HelicsError err = helicsErrorInitialize();

	helicsBrokerSendCommand(broker, (char const *)target, (char const *)command, &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	mxFree(target);

	mxFree(command);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsCoreSetLogFile(resc, resv, argc, (mxArray**)(argv)){
	HelicsCore core = (HelicsCore)(mxGetData(argv[0]));

	char *logFileName;
	size_t logFileNameLength;
	int logFileNameStatus;
	logFileNameLength = mxGetN(argv[1]) + 1;
	logFileName = (char *)mxMalloc(logFileNameLength);
	logFileNameStatus = mxGetString(argv[1], logFileName, logFileNameLength);

	HelicsError err = helicsErrorInitialize();

	helicsCoreSetLogFile(core, (char const *)logFileName, &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	mxFree(logFileName);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsBrokerSetLogFile(resc, resv, argc, (mxArray**)(argv)){
	HelicsBroker broker = (HelicsBroker)(mxGetData(argv[0]));

	char *logFileName;
	size_t logFileNameLength;
	int logFileNameStatus;
	logFileNameLength = mxGetN(argv[1]) + 1;
	logFileName = (char *)mxMalloc(logFileNameLength);
	logFileNameStatus = mxGetString(argv[1], logFileName, logFileNameLength);

	HelicsError err = helicsErrorInitialize();

	helicsBrokerSetLogFile(broker, (char const *)logFileName, &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	mxFree(logFileName);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsBrokerSetTimeBarrier(resc, resv, argc, (mxArray**)(argv)){
	HelicsBroker broker = (HelicsBroker)(mxGetData(argv[0]));

	HelicsTime barrierTime = (HelicsTime)(mxGetScalar(argv[1]));

	HelicsError err = helicsErrorInitialize();

	helicsBrokerSetTimeBarrier(broker, barrierTime, &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}





	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsBrokerClearTimeBarrier(resc, resv, argc, (mxArray**)(argv)){
	HelicsBroker broker = (HelicsBroker)(mxGetData(argv[0]));

	helicsBrokerClearTimeBarrier(broker);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsBrokerGlobalError(resc, resv, argc, (mxArray**)(argv)){
	HelicsBroker broker = (HelicsBroker)(mxGetData(argv[0]));

	int errorCode = (int)(mxGetScalar(argv[1]));

	char *errorString;
	size_t errorStringLength;
	int errorStringStatus;
	errorStringLength = mxGetN(argv[2]) + 1;
	errorString = (char *)mxMalloc(errorStringLength);
	errorStringStatus = mxGetString(argv[2], errorString, errorStringLength);

	HelicsError err = helicsErrorInitialize();

	helicsBrokerGlobalError(broker, errorCode, (char const *)errorString, &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}





	mxFree(errorString);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsCoreGlobalError(resc, resv, argc, (mxArray**)(argv)){
	HelicsCore core = (HelicsCore)(mxGetData(argv[0]));

	int errorCode = (int)(mxGetScalar(argv[1]));

	char *errorString;
	size_t errorStringLength;
	int errorStringStatus;
	errorStringLength = mxGetN(argv[2]) + 1;
	errorString = (char *)mxMalloc(errorStringLength);
	errorStringStatus = mxGetString(argv[2], errorString, errorStringLength);

	HelicsError err = helicsErrorInitialize();

	helicsCoreGlobalError(core, errorCode, (char const *)errorString, &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}





	mxFree(errorString);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsCreateQuery(resc, resv, argc, (mxArray**)(argv)){
	char *target;
	size_t targetLength;
	int targetStatus;
	targetLength = mxGetN(argv[0]) + 1;
	target = (char *)mxMalloc(targetLength);
	targetStatus = mxGetString(argv[0], target, targetLength);

	char *query;
	size_t queryLength;
	int queryStatus;
	queryLength = mxGetN(argv[1]) + 1;
	query = (char *)mxMalloc(queryLength);
	queryStatus = mxGetString(argv[1], query, queryLength);

	HelicsQuery result = helicsCreateQuery((char const *)target, (char const *)query);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_T*)mxGetData(_out)) = (uint64_T)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}

	mxFree(target);

	mxFree(query);
}


void _wrap_helicsQueryExecute(resc, resv, argc, (mxArray**)(argv)){
	HelicsQuery query = (HelicsQuery)(mxGetData(argv[0]));

	HelicsFederate fed = (HelicsFederate)(mxGetData(argv[1]));

	HelicsError err = helicsErrorInitialize();

	char *result = helicsQueryExecute(query, fed, &err);

	mxArray *_out = mxCreateString((const char *)result);

	if(_out){
		--resc;
		*resv++ = _out;
	}





	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsQueryCoreExecute(resc, resv, argc, (mxArray**)(argv)){
	HelicsQuery query = (HelicsQuery)(mxGetData(argv[0]));

	HelicsCore core = (HelicsCore)(mxGetData(argv[1]));

	HelicsError err = helicsErrorInitialize();

	char *result = helicsQueryCoreExecute(query, core, &err);

	mxArray *_out = mxCreateString((const char *)result);

	if(_out){
		--resc;
		*resv++ = _out;
	}





	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsQueryBrokerExecute(resc, resv, argc, (mxArray**)(argv)){
	HelicsQuery query = (HelicsQuery)(mxGetData(argv[0]));

	HelicsBroker broker = (HelicsBroker)(mxGetData(argv[1]));

	HelicsError err = helicsErrorInitialize();

	char *result = helicsQueryBrokerExecute(query, broker, &err);

	mxArray *_out = mxCreateString((const char *)result);

	if(_out){
		--resc;
		*resv++ = _out;
	}





	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsQueryExecuteAsync(resc, resv, argc, (mxArray**)(argv)){
	HelicsQuery query = (HelicsQuery)(mxGetData(argv[0]));

	HelicsFederate fed = (HelicsFederate)(mxGetData(argv[1]));

	HelicsError err = helicsErrorInitialize();

	helicsQueryExecuteAsync(query, fed, &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}





	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsQueryExecuteComplete(resc, resv, argc, (mxArray**)(argv)){
	HelicsQuery query = (HelicsQuery)(mxGetData(argv[0]));

	HelicsError err = helicsErrorInitialize();

	char *result = helicsQueryExecuteComplete(query, &err);

	mxArray *_out = mxCreateString((const char *)result);

	if(_out){
		--resc;
		*resv++ = _out;
	}



	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsQueryIsCompleted(resc, resv, argc, (mxArray**)(argv)){
	HelicsQuery query = (HelicsQuery)(mxGetData(argv[0]));

	HelicsBool result = helicsQueryIsCompleted(query);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
	*((int64_T*)mxGetData(out)) = (int64_T)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsQuerySetTarget(resc, resv, argc, (mxArray**)(argv)){
	HelicsQuery query = (HelicsQuery)(mxGetData(argv[0]));

	char *target;
	size_t targetLength;
	int targetStatus;
	targetLength = mxGetN(argv[1]) + 1;
	target = (char *)mxMalloc(targetLength);
	targetStatus = mxGetString(argv[1], target, targetLength);

	HelicsError err = helicsErrorInitialize();

	helicsQuerySetTarget(query, (char const *)target, &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	mxFree(target);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsQuerySetQueryString(resc, resv, argc, (mxArray**)(argv)){
	HelicsQuery query = (HelicsQuery)(mxGetData(argv[0]));

	char *queryString;
	size_t queryStringLength;
	int queryStringStatus;
	queryStringLength = mxGetN(argv[1]) + 1;
	queryString = (char *)mxMalloc(queryStringLength);
	queryStringStatus = mxGetString(argv[1], queryString, queryStringLength);

	HelicsError err = helicsErrorInitialize();

	helicsQuerySetQueryString(query, (char const *)queryString, &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	mxFree(queryString);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsQuerySetOrdering(resc, resv, argc, (mxArray**)(argv)){
	HelicsQuery query = (HelicsQuery)(mxGetData(argv[0]));

	int32_t mode = *((int32_t *)mxGetData(argv[1]));

	HelicsError err = helicsErrorInitialize();

	helicsQuerySetOrdering(query, mode, &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}





	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsQueryFree(resc, resv, argc, (mxArray**)(argv)){
	HelicsQuery query = (HelicsQuery)(mxGetData(argv[0]));

	helicsQueryFree(query);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsCleanupLibrary(resc, resv, argc, (mxArray**)(argv)){
	helicsCleanupLibrary();

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}
}


void _wrap_helicsFederateRegisterSubscription(resc, resv, argc, (mxArray**)(argv)){
	HelicsFederate fed = (HelicsFederate)(mxGetData(argv[0]));

	char *key;
	size_t keyLength;
	int keyStatus;
	keyLength = mxGetN(argv[1]) + 1;
	key = (char *)mxMalloc(keyLength);
	keyStatus = mxGetString(argv[1], key, keyLength);

	char *units;
	size_t unitsLength;
	int unitsStatus;
	unitsLength = mxGetN(argv[2]) + 1;
	units = (char *)mxMalloc(unitsLength);
	unitsStatus = mxGetString(argv[2], units, unitsLength);

	HelicsError err = helicsErrorInitialize();

	HelicsInput result = helicsFederateRegisterSubscription(fed, (char const *)key, (char const *)units, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_T*)mxGetData(_out)) = (uint64_T)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	mxFree(key);

	mxFree(units);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateRegisterPublication(resc, resv, argc, (mxArray**)(argv)){
	HelicsFederate fed = (HelicsFederate)(mxGetData(argv[0]));

	char *key;
	size_t keyLength;
	int keyStatus;
	keyLength = mxGetN(argv[1]) + 1;
	key = (char *)mxMalloc(keyLength);
	keyStatus = mxGetString(argv[1], key, keyLength);

	HelicsDataTypes type = (HelicsDataTypes)(mxGetScalar(argv[2]));

	char *units;
	size_t unitsLength;
	int unitsStatus;
	unitsLength = mxGetN(argv[3]) + 1;
	units = (char *)mxMalloc(unitsLength);
	unitsStatus = mxGetString(argv[3], units, unitsLength);

	HelicsError err = helicsErrorInitialize();

	HelicsPublication result = helicsFederateRegisterPublication(fed, (char const *)key, type, (char const *)units, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_T*)mxGetData(_out)) = (uint64_T)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	mxFree(key);



	mxFree(units);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateRegisterTypePublication(resc, resv, argc, (mxArray**)(argv)){
	HelicsFederate fed = (HelicsFederate)(mxGetData(argv[0]));

	char *key;
	size_t keyLength;
	int keyStatus;
	keyLength = mxGetN(argv[1]) + 1;
	key = (char *)mxMalloc(keyLength);
	keyStatus = mxGetString(argv[1], key, keyLength);

	char *type;
	size_t typeLength;
	int typeStatus;
	typeLength = mxGetN(argv[2]) + 1;
	type = (char *)mxMalloc(typeLength);
	typeStatus = mxGetString(argv[2], type, typeLength);

	char *units;
	size_t unitsLength;
	int unitsStatus;
	unitsLength = mxGetN(argv[3]) + 1;
	units = (char *)mxMalloc(unitsLength);
	unitsStatus = mxGetString(argv[3], units, unitsLength);

	HelicsError err = helicsErrorInitialize();

	HelicsPublication result = helicsFederateRegisterTypePublication(fed, (char const *)key, (char const *)type, (char const *)units, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_T*)mxGetData(_out)) = (uint64_T)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	mxFree(key);

	mxFree(type);

	mxFree(units);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateRegisterGlobalPublication(resc, resv, argc, (mxArray**)(argv)){
	HelicsFederate fed = (HelicsFederate)(mxGetData(argv[0]));

	char *key;
	size_t keyLength;
	int keyStatus;
	keyLength = mxGetN(argv[1]) + 1;
	key = (char *)mxMalloc(keyLength);
	keyStatus = mxGetString(argv[1], key, keyLength);

	HelicsDataTypes type = (HelicsDataTypes)(mxGetScalar(argv[2]));

	char *units;
	size_t unitsLength;
	int unitsStatus;
	unitsLength = mxGetN(argv[3]) + 1;
	units = (char *)mxMalloc(unitsLength);
	unitsStatus = mxGetString(argv[3], units, unitsLength);

	HelicsError err = helicsErrorInitialize();

	HelicsPublication result = helicsFederateRegisterGlobalPublication(fed, (char const *)key, type, (char const *)units, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_T*)mxGetData(_out)) = (uint64_T)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	mxFree(key);



	mxFree(units);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateRegisterGlobalTypePublication(resc, resv, argc, (mxArray**)(argv)){
	HelicsFederate fed = (HelicsFederate)(mxGetData(argv[0]));

	char *key;
	size_t keyLength;
	int keyStatus;
	keyLength = mxGetN(argv[1]) + 1;
	key = (char *)mxMalloc(keyLength);
	keyStatus = mxGetString(argv[1], key, keyLength);

	char *type;
	size_t typeLength;
	int typeStatus;
	typeLength = mxGetN(argv[2]) + 1;
	type = (char *)mxMalloc(typeLength);
	typeStatus = mxGetString(argv[2], type, typeLength);

	char *units;
	size_t unitsLength;
	int unitsStatus;
	unitsLength = mxGetN(argv[3]) + 1;
	units = (char *)mxMalloc(unitsLength);
	unitsStatus = mxGetString(argv[3], units, unitsLength);

	HelicsError err = helicsErrorInitialize();

	HelicsPublication result = helicsFederateRegisterGlobalTypePublication(fed, (char const *)key, (char const *)type, (char const *)units, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_T*)mxGetData(_out)) = (uint64_T)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	mxFree(key);

	mxFree(type);

	mxFree(units);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateRegisterInput(resc, resv, argc, (mxArray**)(argv)){
	HelicsFederate fed = (HelicsFederate)(mxGetData(argv[0]));

	char *key;
	size_t keyLength;
	int keyStatus;
	keyLength = mxGetN(argv[1]) + 1;
	key = (char *)mxMalloc(keyLength);
	keyStatus = mxGetString(argv[1], key, keyLength);

	HelicsDataTypes type = (HelicsDataTypes)(mxGetScalar(argv[2]));

	char *units;
	size_t unitsLength;
	int unitsStatus;
	unitsLength = mxGetN(argv[3]) + 1;
	units = (char *)mxMalloc(unitsLength);
	unitsStatus = mxGetString(argv[3], units, unitsLength);

	HelicsError err = helicsErrorInitialize();

	HelicsInput result = helicsFederateRegisterInput(fed, (char const *)key, type, (char const *)units, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_T*)mxGetData(_out)) = (uint64_T)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	mxFree(key);



	mxFree(units);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateRegisterTypeInput(resc, resv, argc, (mxArray**)(argv)){
	HelicsFederate fed = (HelicsFederate)(mxGetData(argv[0]));

	char *key;
	size_t keyLength;
	int keyStatus;
	keyLength = mxGetN(argv[1]) + 1;
	key = (char *)mxMalloc(keyLength);
	keyStatus = mxGetString(argv[1], key, keyLength);

	char *type;
	size_t typeLength;
	int typeStatus;
	typeLength = mxGetN(argv[2]) + 1;
	type = (char *)mxMalloc(typeLength);
	typeStatus = mxGetString(argv[2], type, typeLength);

	char *units;
	size_t unitsLength;
	int unitsStatus;
	unitsLength = mxGetN(argv[3]) + 1;
	units = (char *)mxMalloc(unitsLength);
	unitsStatus = mxGetString(argv[3], units, unitsLength);

	HelicsError err = helicsErrorInitialize();

	HelicsInput result = helicsFederateRegisterTypeInput(fed, (char const *)key, (char const *)type, (char const *)units, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_T*)mxGetData(_out)) = (uint64_T)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	mxFree(key);

	mxFree(type);

	mxFree(units);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateRegisterGlobalInput(resc, resv, argc, (mxArray**)(argv)){
	HelicsFederate fed = (HelicsFederate)(mxGetData(argv[0]));

	char *key;
	size_t keyLength;
	int keyStatus;
	keyLength = mxGetN(argv[1]) + 1;
	key = (char *)mxMalloc(keyLength);
	keyStatus = mxGetString(argv[1], key, keyLength);

	HelicsDataTypes type = (HelicsDataTypes)(mxGetScalar(argv[2]));

	char *units;
	size_t unitsLength;
	int unitsStatus;
	unitsLength = mxGetN(argv[3]) + 1;
	units = (char *)mxMalloc(unitsLength);
	unitsStatus = mxGetString(argv[3], units, unitsLength);

	HelicsError err = helicsErrorInitialize();

	HelicsPublication result = helicsFederateRegisterGlobalInput(fed, (char const *)key, type, (char const *)units, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_T*)mxGetData(_out)) = (uint64_T)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	mxFree(key);



	mxFree(units);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateRegisterGlobalTypeInput(resc, resv, argc, (mxArray**)(argv)){
	HelicsFederate fed = (HelicsFederate)(mxGetData(argv[0]));

	char *key;
	size_t keyLength;
	int keyStatus;
	keyLength = mxGetN(argv[1]) + 1;
	key = (char *)mxMalloc(keyLength);
	keyStatus = mxGetString(argv[1], key, keyLength);

	char *type;
	size_t typeLength;
	int typeStatus;
	typeLength = mxGetN(argv[2]) + 1;
	type = (char *)mxMalloc(typeLength);
	typeStatus = mxGetString(argv[2], type, typeLength);

	char *units;
	size_t unitsLength;
	int unitsStatus;
	unitsLength = mxGetN(argv[3]) + 1;
	units = (char *)mxMalloc(unitsLength);
	unitsStatus = mxGetString(argv[3], units, unitsLength);

	HelicsError err = helicsErrorInitialize();

	HelicsPublication result = helicsFederateRegisterGlobalTypeInput(fed, (char const *)key, (char const *)type, (char const *)units, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_T*)mxGetData(_out)) = (uint64_T)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	mxFree(key);

	mxFree(type);

	mxFree(units);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateGetPublication(resc, resv, argc, (mxArray**)(argv)){
	HelicsFederate fed = (HelicsFederate)(mxGetData(argv[0]));

	char *key;
	size_t keyLength;
	int keyStatus;
	keyLength = mxGetN(argv[1]) + 1;
	key = (char *)mxMalloc(keyLength);
	keyStatus = mxGetString(argv[1], key, keyLength);

	HelicsError err = helicsErrorInitialize();

	HelicsPublication result = helicsFederateGetPublication(fed, (char const *)key, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_T*)mxGetData(_out)) = (uint64_T)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	mxFree(key);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateGetPublicationByIndex(resc, resv, argc, (mxArray**)(argv)){
	HelicsFederate fed = (HelicsFederate)(mxGetData(argv[0]));

	int index = (int)(mxGetScalar(argv[1]));

	HelicsError err = helicsErrorInitialize();

	HelicsPublication result = helicsFederateGetPublicationByIndex(fed, index, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_T*)mxGetData(_out)) = (uint64_T)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}





	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateGetInput(resc, resv, argc, (mxArray**)(argv)){
	HelicsFederate fed = (HelicsFederate)(mxGetData(argv[0]));

	char *key;
	size_t keyLength;
	int keyStatus;
	keyLength = mxGetN(argv[1]) + 1;
	key = (char *)mxMalloc(keyLength);
	keyStatus = mxGetString(argv[1], key, keyLength);

	HelicsError err = helicsErrorInitialize();

	HelicsInput result = helicsFederateGetInput(fed, (char const *)key, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_T*)mxGetData(_out)) = (uint64_T)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	mxFree(key);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateGetInputByIndex(resc, resv, argc, (mxArray**)(argv)){
	HelicsFederate fed = (HelicsFederate)(mxGetData(argv[0]));

	int index = (int)(mxGetScalar(argv[1]));

	HelicsError err = helicsErrorInitialize();

	HelicsInput result = helicsFederateGetInputByIndex(fed, index, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_T*)mxGetData(_out)) = (uint64_T)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}





	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateGetSubscription(resc, resv, argc, (mxArray**)(argv)){
	HelicsFederate fed = (HelicsFederate)(mxGetData(argv[0]));

	char *key;
	size_t keyLength;
	int keyStatus;
	keyLength = mxGetN(argv[1]) + 1;
	key = (char *)mxMalloc(keyLength);
	keyStatus = mxGetString(argv[1], key, keyLength);

	HelicsError err = helicsErrorInitialize();

	HelicsInput result = helicsFederateGetSubscription(fed, (char const *)key, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_T*)mxGetData(_out)) = (uint64_T)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	mxFree(key);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateClearUpdates(resc, resv, argc, (mxArray**)(argv)){
	HelicsFederate fed = (HelicsFederate)(mxGetData(argv[0]));

	helicsFederateClearUpdates(fed);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsFederateRegisterFromPublicationJSON(resc, resv, argc, (mxArray**)(argv)){
	HelicsFederate fed = (HelicsFederate)(mxGetData(argv[0]));

	char *json;
	size_t jsonLength;
	int jsonStatus;
	jsonLength = mxGetN(argv[1]) + 1;
	json = (char *)mxMalloc(jsonLength);
	jsonStatus = mxGetString(argv[1], json, jsonLength);

	HelicsError err = helicsErrorInitialize();

	helicsFederateRegisterFromPublicationJSON(fed, (char const *)json, &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	mxFree(json);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederatePublishJSON(resc, resv, argc, (mxArray**)(argv)){
	HelicsFederate fed = (HelicsFederate)(mxGetData(argv[0]));

	char *json;
	size_t jsonLength;
	int jsonStatus;
	jsonLength = mxGetN(argv[1]) + 1;
	json = (char *)mxMalloc(jsonLength);
	jsonStatus = mxGetString(argv[1], json, jsonLength);

	HelicsError err = helicsErrorInitialize();

	helicsFederatePublishJSON(fed, (char const *)json, &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	mxFree(json);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsPublicationIsValid(resc, resv, argc, (mxArray**)(argv)){
	HelicsPublication pub = (HelicsPublication)(mxGetData(argv[0]));

	HelicsBool result = helicsPublicationIsValid(pub);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
	*((int64_T*)mxGetData(out)) = (int64_T)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsPublicationPublishBytes(resc, resv, argc, (mxArray**)(argv)){
	HelicsPublication pub = (HelicsPublication)(mxGetData(argv[0]));

	char *data;
	size_t dataLength;
	int dataStatus;
	dataLength = mxGetN(argv[1]) + 1;
	data = (char *)mxMalloc(dataLength);
	dataStatus = mxGetString(argv[1], data, dataLength);

	int inputDataLength = dataLength - 1;

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


void _wrap_helicsPublicationPublishString(resc, resv, argc, (mxArray**)(argv)){
	HelicsPublication pub = (HelicsPublication)(mxGetData(argv[0]));

	char *str;
	size_t strLength;
	int strStatus;
	strLength = mxGetN(argv[1]) + 1;
	str = (char *)mxMalloc(strLength);
	strStatus = mxGetString(argv[1], str, strLength);

	HelicsError err = helicsErrorInitialize();

	helicsPublicationPublishString(pub, (char const *)str, &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	mxFree(str);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsPublicationPublishInteger(resc, resv, argc, (mxArray**)(argv)){
	HelicsPublication pub = (HelicsPublication)(mxGetData(argv[0]));

	int64_t val = *((int64_t *)mxGetData(argv[1]));

	HelicsError err = helicsErrorInitialize();

	helicsPublicationPublishInteger(pub, val, &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}





	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsPublicationPublishBoolean(resc, resv, argc, (mxArray**)(argv)){
	HelicsPublication pub = (HelicsPublication)(mxGetData(argv[0]));

	HelicsBool val = (HelicsBool)(mxGetScalar(argv[1]));

	HelicsError err = helicsErrorInitialize();

	helicsPublicationPublishBoolean(pub, val, &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}





	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsPublicationPublishDouble(resc, resv, argc, (mxArray**)(argv)){
	HelicsPublication pub = (HelicsPublication)(mxGetData(argv[0]));

	double val = mxGetScalar(argv[1]);

	HelicsError err = helicsErrorInitialize();

	helicsPublicationPublishDouble(pub, val, &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}





	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsPublicationPublishTime(resc, resv, argc, (mxArray**)(argv)){
	HelicsPublication pub = (HelicsPublication)(mxGetData(argv[0]));

	HelicsTime val = (HelicsTime)(mxGetScalar(argv[1]));

	HelicsError err = helicsErrorInitialize();

	helicsPublicationPublishTime(pub, val, &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}





	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsPublicationPublishChar(resc, resv, argc, (mxArray**)(argv)){
	HelicsPublication pub = (HelicsPublication)(mxGetData(argv[0]));

	char *val;
	size_t valLength;
	int valStatus;
	valLength = mxGetN(argv[1]) + 1;
	val = (char *)mxMalloc(valLength);
	valStatus = mxGetString(argv[1], val, valLength);

	HelicsError err = helicsErrorInitialize();

	helicsPublicationPublishChar(pub, val[0], &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	mxFree(val);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsPublicationPublishComplex(resc, resv, argc, (mxArray**)(argv)){
	HelicsPublication pub = (HelicsPublication)(mxGetData(argv[0]));

	mxComplexDouble *complexValue = mxGetComplexDouble(argv[1]);
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


void _wrap_helicsPublicationPublishVector(resc, resv, argc, (mxArray**)(argv)){
	HelicsPublication pub = (HelicsPublication)(mxGetData(argv[0]));

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


void _wrap_helicsPublicationPublishComplexVector(resc, resv, argc, (mxArray**)(argv)){
	HelicsPublication pub = (HelicsPublication)(mxGetData(argv[0]));

	int vectorLength =  (int)mxGetN(argv[1])*2;

	double vectorInput[vectorLength];
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


void _wrap_helicsPublicationPublishNamedPoint(resc, resv, argc, (mxArray**)(argv)){
	HelicsPublication pub = (HelicsPublication)(mxGetData(argv[0]));

	char *str;
	size_t strLength;
	int strStatus;
	strLength = mxGetN(argv[1]) + 1;
	str = (char *)mxMalloc(strLength);
	strStatus = mxGetString(argv[1], str, strLength);

	double val = mxGetScalar(argv[2]);

	HelicsError err = helicsErrorInitialize();

	helicsPublicationPublishNamedPoint(pub, (char const *)str, val, &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	mxFree(str);



	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsPublicationAddTarget(resc, resv, argc, (mxArray**)(argv)){
	HelicsPublication pub = (HelicsPublication)(mxGetData(argv[0]));

	char *target;
	size_t targetLength;
	int targetStatus;
	targetLength = mxGetN(argv[1]) + 1;
	target = (char *)mxMalloc(targetLength);
	targetStatus = mxGetString(argv[1], target, targetLength);

	HelicsError err = helicsErrorInitialize();

	helicsPublicationAddTarget(pub, (char const *)target, &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	mxFree(target);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsInputIsValid(resc, resv, argc, (mxArray**)(argv)){
	HelicsInput ipt = (HelicsInput)(mxGetData(argv[0]));

	HelicsBool result = helicsInputIsValid(ipt);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
	*((int64_T*)mxGetData(out)) = (int64_T)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsInputAddTarget(resc, resv, argc, (mxArray**)(argv)){
	HelicsInput ipt = (HelicsInput)(mxGetData(argv[0]));

	char *target;
	size_t targetLength;
	int targetStatus;
	targetLength = mxGetN(argv[1]) + 1;
	target = (char *)mxMalloc(targetLength);
	targetStatus = mxGetString(argv[1], target, targetLength);

	HelicsError err = helicsErrorInitialize();

	helicsInputAddTarget(ipt, (char const *)target, &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	mxFree(target);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsInputGetByteCount(resc, resv, argc, (mxArray**)(argv)){
	HelicsInput ipt = (HelicsInput)(mxGetData(argv[0]));

	int result = helicsInputGetByteCount(ipt);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
	*((int64_T*)mxGetData(out)) = (int64_T)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsInputGetBytes(resc, resv, argc, (mxArray**)(argv)){
	HelicsInput ipt = (HelicsInput)(mxGetData(argv[0]));

	int maxDataLen = helicsInputGetByteCount(ipt) + 2;

	void *data = mxMalloc(maxDataLen);

	int *actualSize = (int *)0;

	HelicsError err = helicsErrorInitialize();

	helicsInputGetBytes(ipt, data, maxDatalen, actualSize, &err);

	mwSize dims[2] = {1, *actualSize};
	mxArray *_out = mxCreateCharArray(2, dims);
	mxChar *out_data = (mxChar *)mxGetData(_out);
	for(int i=0; i<actualSize; ++i){
		out_data[i] = data[i];
	}

	if(_out){
		--resc;
		*resv++ = _out;
	}

	mxFree(data);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsInputGetStringSize(resc, resv, argc, (mxArray**)(argv)){
	HelicsInput ipt = (HelicsInput)(mxGetData(argv[0]));

	int result = helicsInputGetStringSize(ipt);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
	*((int64_T*)mxGetData(out)) = (int64_T)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsInputGetString(resc, resv, argc, (mxArray**)(argv)){
	HelicsInput ipt = (HelicsInput)(mxGetData(argv[0]));

	int maxStringLen = helicsInputGetStringSize(ipt) + 2;

	char *outputString = (char *)mxMalloc(maxStringLen);

	int *actualLength = (int *)0;

	HelicsError err = helicsErrorInitialize();

	helicsInputGetString(ipt, outputString, maxStringLen, actualLength, val, &err);

	mwSize dims[2] = {1, *actualLength};
	mxArray *_out = mxCreateCharArray(2, dims);
	mxChar *out_data = (mxChar *)mxGetData(_out);
	for(int i=0; i<actualLength; ++i){
		out_data[i] = outputString[i];
	}

	if(_out){
		--resc;
		*resv++ = _out;
	}

	mxFree(outputString);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsInputGetInteger(resc, resv, argc, (mxArray**)(argv)){
	HelicsInput ipt = (HelicsInput)(mxGetData(argv[0]));

	HelicsError err = helicsErrorInitialize();

	int64_t result = helicsInputGetInteger(ipt, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
	*((int64_T*)mxGetData(out)) = (int64_T)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsInputGetBoolean(resc, resv, argc, (mxArray**)(argv)){
	HelicsInput ipt = (HelicsInput)(mxGetData(argv[0]));

	HelicsError err = helicsErrorInitialize();

	HelicsBool result = helicsInputGetBoolean(ipt, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
	*((int64_T*)mxGetData(out)) = (int64_T)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsInputGetDouble(resc, resv, argc, (mxArray**)(argv)){
	HelicsInput ipt = (HelicsInput)(mxGetData(argv[0]));

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


void _wrap_helicsInputGetTime(resc, resv, argc, (mxArray**)(argv)){
	HelicsInput ipt = (HelicsInput)(mxGetData(argv[0]));

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


void _wrap_helicsInputGetChar(resc, resv, argc, (mxArray**)(argv)){
	HelicsInput ipt = (HelicsInput)(mxGetData(argv[0]));

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


void _wrap_helicsInputGetComplexObject(resc, resv, argc, (mxArray**)(argv)){
	HelicsInput ipt = (HelicsInput)(mxGetData(argv[0]));

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


void _wrap_helicsInputGetComplex(resc, resv, argc, (mxArray**)(argv)){
	HelicsInput ipt = (HelicsInput)(mxGetData(argv[0]));

	double values[2];

	HelicsError err = helicsErrorInitialize();

	helicsInputGetComplex(ipt, &(vaules[0]), &(values[1]), &err);

	mxComplexDouble complex_result;
	complex_result.real = values[0];
	complex_result.imag = values[1];
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


void _wrap_helicsInputGetVectorSize(resc, resv, argc, (mxArray**)(argv)){
	HelicsInput ipt = (HelicsInput)(mxGetData(argv[0]));

	int result = helicsInputGetVectorSize(ipt);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
	*((int64_T*)mxGetData(out)) = (int64_T)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsInputGetVector(resc, resv, argc, (mxArray**)(argv)){
	HelicsInput ipt = (HelicsInput)(mxGetData(argv[0]));

	int maxLength = helicsInputGetVectorSize(ipt);

	double data[maxLength];

	int *actualSize = (int *)0;

	HelicsError err = helicsErrorInitialize();

	helicsInputGetVector(ipt, data, maxLength, actualSize, &err);

	mxDouble result_data[*actualSize];
	for(int i=0; i<(*actualSize); ++i){
		result_data[i] = (mxDouble)data[i];
	}
	mxArray *_out = mxCreateDoubleMatrix(*actualSize, 1, mxREAL);
	int status = mxSetDoubles(_out, &(resutl_data[0]));

	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsInputGetComplexVector(resc, resv, argc, (mxArray**)(argv)){
	HelicsInput ipt = (HelicsInput)(mxGetData(argv[0]));

	int maxLength = helicsInputGetVectorSize(ipt);

	double data[maxLength];

	int *actualSize = (int *)0;

	HelicsError err = helicsErrorInitialize();

	helicsInputGetComplexVector(ipt, data, maxLength, actualSize, &err);

	mxComplexDouble result_data[*actualSize/2];
	for(int i=0; i<(*actualSize/2); ++i){
		result_data[i].real = data[2*(i)];
		result_data[i].imag = data[2*(i) + 1];
	}
	mxArray *_out = mxCreateDoubleMatrix(*actualSize/2, 1, mxCOMPLEX);
	int status = mxSetComplexDoubles(_out, &(resutl_data[0]));

	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsInputGetNamedPoint(resc, resv, argc, (mxArray**)(argv)){
	HelicsInput ipt = (HelicsInput)(mxGetData(argv[0]));

	int maxStringLen = helicsInputGetStringSize(ipt) + 2;

	char *outputString = (char *)mxMalloc(maxStringLen);

	int *actualLength = (int *)0;

	double *val = (double *)0;

	HelicsError err = helicsErrorInitialize();

	helicsInputGetNamedPoint(ipt, outputString, maxStringLen, actualLength, val, &err);

	mwSize dims[2] = {1, *actualLength};
	mxArray *_out = mxCreateCharArray(2, dims);
	mxChar *out_data = (mxChar *)mxGetData(_out);
	for(int i=0; i<actualLength; ++i){
		out_data[i] = outputString[i];
	}

	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(--resc>=0){
		mxArray *_out1 = mxCreateScalar(*val);
		*resv++ = _out1;
	}

	mxFree(outputString);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsInputSetDefaultBytes(resc, resv, argc, (mxArray**)(argv)){
	HelicsInput ipt = (HelicsInput)(mxGetData(argv[0]));

	char *data;
	size_t dataLength;
	int dataStatus;
	dataLength = mxGetN(argv[1]) + 1;
	data = (char *)mxMalloc(dataLength);
	dataStatus = mxGetString(argv[1], data, dataLength);

	int inputDataLength = dataLength - 1;

	HelicsError err = helicsErrorInitialize();

	helicsInputSetDefaultBytes(ipt, (void *)data, inputDataLength, &err);

	mxArray *_out = (mxArray *)0;
	if(_out){
		--resc;
		*resv++ = _out;
	}

	mxFree(data);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsInputSetDefaultString(resc, resv, argc, (mxArray**)(argv)){
	HelicsInput ipt = (HelicsInput)(mxGetData(argv[0]));

	char *str;
	size_t strLength;
	int strStatus;
	strLength = mxGetN(argv[1]) + 1;
	str = (char *)mxMalloc(strLength);
	strStatus = mxGetString(argv[1], str, strLength);

	HelicsError err = helicsErrorInitialize();

	helicsInputSetDefaultString(ipt, (char const *)str, &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	mxFree(str);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsInputSetDefaultInteger(resc, resv, argc, (mxArray**)(argv)){
	HelicsInput ipt = (HelicsInput)(mxGetData(argv[0]));

	int64_t val = *((int64_t *)mxGetData(argv[1]));

	HelicsError err = helicsErrorInitialize();

	helicsInputSetDefaultInteger(ipt, val, &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}





	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsInputSetDefaultBoolean(resc, resv, argc, (mxArray**)(argv)){
	HelicsInput ipt = (HelicsInput)(mxGetData(argv[0]));

	HelicsBool val = (HelicsBool)(mxGetScalar(argv[1]));

	HelicsError err = helicsErrorInitialize();

	helicsInputSetDefaultBoolean(ipt, val, &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}





	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsInputSetDefaultTime(resc, resv, argc, (mxArray**)(argv)){
	HelicsInput ipt = (HelicsInput)(mxGetData(argv[0]));

	HelicsTime val = (HelicsTime)(mxGetScalar(argv[1]));

	HelicsError err = helicsErrorInitialize();

	helicsInputSetDefaultTime(ipt, val, &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}





	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsInputSetDefaultChar(resc, resv, argc, (mxArray**)(argv)){
	HelicsInput ipt = (HelicsInput)(mxGetData(argv[0]));

	char *val;
	size_t valLength;
	int valStatus;
	valLength = mxGetN(argv[1]) + 1;
	val = (char *)mxMalloc(valLength);
	valStatus = mxGetString(argv[1], val, valLength);

	HelicsError err = helicsErrorInitialize();

	helicsInputSetDefaultChar(ipt, val[0], &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	mxFree(val);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsInputSetDefaultDouble(resc, resv, argc, (mxArray**)(argv)){
	HelicsInput ipt = (HelicsInput)(mxGetData(argv[0]));

	double val = mxGetScalar(argv[1]);

	HelicsError err = helicsErrorInitialize();

	helicsInputSetDefaultDouble(ipt, val, &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}





	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsInputSetDefaultComplex(resc, resv, argc, (mxArray**)(argv)){
	HelicsInput ipt = (HelicsInput)(mxGetData(argv[0]));

	mxComplexDouble *value = xmGetComplexDoubles(argv[1]);

	HelicsError err = helicsErrorInitialize();

	helicsInputSetDefaultComplex(ipt, (double)value->real, (double)value->imag, &err);

	mxArray *_out = (mxArray *)0;
	if(_out){
		--resc;
		*resv++ = _out;
	}

	mxFree(data);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsInputSetDefaultVector(resc, resv, argc, (mxArray**)(argv)){
	HelicsInput ipt = (HelicsInput)(mxGetData(argv[0]));

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


void _wrap_helicsInputSetDefaultComplexVector(resc, resv, argc, (mxArray**)(argv)){
	HelicsInput ipt = (HelicsInput)(mxGetData(argv[0]));

	int vectorLength =  (int)mxGetN(argv[1])*2;

	double vectorInput[vectorLength];
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


void _wrap_helicsInputSetDefaultNamedPoint(resc, resv, argc, (mxArray**)(argv)){
	HelicsInput ipt = (HelicsInput)(mxGetData(argv[0]));

	char *str;
	size_t strLength;
	int strStatus;
	strLength = mxGetN(argv[1]) + 1;
	str = (char *)mxMalloc(strLength);
	strStatus = mxGetString(argv[1], str, strLength);

	double val = mxGetScalar(argv[2]);

	HelicsError err = helicsErrorInitialize();

	helicsInputSetDefaultNamedPoint(ipt, (char const *)str, val, &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	mxFree(str);



	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsInputGetType(resc, resv, argc, (mxArray**)(argv)){
	HelicsInput ipt = (HelicsInput)(mxGetData(argv[0]));

	char *result = helicsInputGetType(ipt);

	mxArray *_out = mxCreateString((const char *)result);

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsInputGetPublicationType(resc, resv, argc, (mxArray**)(argv)){
	HelicsInput ipt = (HelicsInput)(mxGetData(argv[0]));

	char *result = helicsInputGetPublicationType(ipt);

	mxArray *_out = mxCreateString((const char *)result);

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsInputGetPublicationDataType(resc, resv, argc, (mxArray**)(argv)){
	HelicsInput ipt = (HelicsInput)(mxGetData(argv[0]));

	int result = helicsInputGetPublicationDataType(ipt);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
	*((int64_T*)mxGetData(out)) = (int64_T)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsPublicationGetType(resc, resv, argc, (mxArray**)(argv)){
	HelicsPublication pub = (HelicsPublication)(mxGetData(argv[0]));

	char *result = helicsPublicationGetType(pub);

	mxArray *_out = mxCreateString((const char *)result);

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsInputGetName(resc, resv, argc, (mxArray**)(argv)){
	HelicsInput ipt = (HelicsInput)(mxGetData(argv[0]));

	char *result = helicsInputGetName(ipt);

	mxArray *_out = mxCreateString((const char *)result);

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsSubscriptionGetTarget(resc, resv, argc, (mxArray**)(argv)){
	HelicsInput ipt = (HelicsInput)(mxGetData(argv[0]));

	char *result = helicsSubscriptionGetTarget(ipt);

	mxArray *_out = mxCreateString((const char *)result);

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsPublicationGetName(resc, resv, argc, (mxArray**)(argv)){
	HelicsPublication pub = (HelicsPublication)(mxGetData(argv[0]));

	char *result = helicsPublicationGetName(pub);

	mxArray *_out = mxCreateString((const char *)result);

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsInputGetUnits(resc, resv, argc, (mxArray**)(argv)){
	HelicsInput ipt = (HelicsInput)(mxGetData(argv[0]));

	char *result = helicsInputGetUnits(ipt);

	mxArray *_out = mxCreateString((const char *)result);

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsInputGetInjectionUnits(resc, resv, argc, (mxArray**)(argv)){
	HelicsInput ipt = (HelicsInput)(mxGetData(argv[0]));

	char *result = helicsInputGetInjectionUnits(ipt);

	mxArray *_out = mxCreateString((const char *)result);

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsInputGetExtractionUnits(resc, resv, argc, (mxArray**)(argv)){
	HelicsInput ipt = (HelicsInput)(mxGetData(argv[0]));

	char *result = helicsInputGetExtractionUnits(ipt);

	mxArray *_out = mxCreateString((const char *)result);

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsPublicationGetUnits(resc, resv, argc, (mxArray**)(argv)){
	HelicsPublication pub = (HelicsPublication)(mxGetData(argv[0]));

	char *result = helicsPublicationGetUnits(pub);

	mxArray *_out = mxCreateString((const char *)result);

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsInputGetInfo(resc, resv, argc, (mxArray**)(argv)){
	HelicsInput inp = (HelicsInput)(mxGetData(argv[0]));

	char *result = helicsInputGetInfo(inp);

	mxArray *_out = mxCreateString((const char *)result);

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsInputSetInfo(resc, resv, argc, (mxArray**)(argv)){
	HelicsInput inp = (HelicsInput)(mxGetData(argv[0]));

	char *info;
	size_t infoLength;
	int infoStatus;
	infoLength = mxGetN(argv[1]) + 1;
	info = (char *)mxMalloc(infoLength);
	infoStatus = mxGetString(argv[1], info, infoLength);

	HelicsError err = helicsErrorInitialize();

	helicsInputSetInfo(inp, (char const *)info, &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	mxFree(info);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsInputGetTag(resc, resv, argc, (mxArray**)(argv)){
	HelicsInput inp = (HelicsInput)(mxGetData(argv[0]));

	char *tagname;
	size_t tagnameLength;
	int tagnameStatus;
	tagnameLength = mxGetN(argv[1]) + 1;
	tagname = (char *)mxMalloc(tagnameLength);
	tagnameStatus = mxGetString(argv[1], tagname, tagnameLength);

	char *result = helicsInputGetTag(inp, (char const *)tagname);

	mxArray *_out = mxCreateString((const char *)result);

	if(_out){
		--resc;
		*resv++ = _out;
	}



	mxFree(tagname);
}


void _wrap_helicsInputSetTag(resc, resv, argc, (mxArray**)(argv)){
	HelicsInput inp = (HelicsInput)(mxGetData(argv[0]));

	char *tagname;
	size_t tagnameLength;
	int tagnameStatus;
	tagnameLength = mxGetN(argv[1]) + 1;
	tagname = (char *)mxMalloc(tagnameLength);
	tagnameStatus = mxGetString(argv[1], tagname, tagnameLength);

	char *tagvalue;
	size_t tagvalueLength;
	int tagvalueStatus;
	tagvalueLength = mxGetN(argv[2]) + 1;
	tagvalue = (char *)mxMalloc(tagvalueLength);
	tagvalueStatus = mxGetString(argv[2], tagvalue, tagvalueLength);

	HelicsError err = helicsErrorInitialize();

	helicsInputSetTag(inp, (char const *)tagname, (char const *)tagvalue, &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	mxFree(tagname);

	mxFree(tagvalue);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsPublicationGetInfo(resc, resv, argc, (mxArray**)(argv)){
	HelicsPublication pub = (HelicsPublication)(mxGetData(argv[0]));

	char *result = helicsPublicationGetInfo(pub);

	mxArray *_out = mxCreateString((const char *)result);

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsPublicationSetInfo(resc, resv, argc, (mxArray**)(argv)){
	HelicsPublication pub = (HelicsPublication)(mxGetData(argv[0]));

	char *info;
	size_t infoLength;
	int infoStatus;
	infoLength = mxGetN(argv[1]) + 1;
	info = (char *)mxMalloc(infoLength);
	infoStatus = mxGetString(argv[1], info, infoLength);

	HelicsError err = helicsErrorInitialize();

	helicsPublicationSetInfo(pub, (char const *)info, &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	mxFree(info);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsPublicationGetTag(resc, resv, argc, (mxArray**)(argv)){
	HelicsPublication pub = (HelicsPublication)(mxGetData(argv[0]));

	char *tagname;
	size_t tagnameLength;
	int tagnameStatus;
	tagnameLength = mxGetN(argv[1]) + 1;
	tagname = (char *)mxMalloc(tagnameLength);
	tagnameStatus = mxGetString(argv[1], tagname, tagnameLength);

	char *result = helicsPublicationGetTag(pub, (char const *)tagname);

	mxArray *_out = mxCreateString((const char *)result);

	if(_out){
		--resc;
		*resv++ = _out;
	}



	mxFree(tagname);
}


void _wrap_helicsPublicationSetTag(resc, resv, argc, (mxArray**)(argv)){
	HelicsPublication pub = (HelicsPublication)(mxGetData(argv[0]));

	char *tagname;
	size_t tagnameLength;
	int tagnameStatus;
	tagnameLength = mxGetN(argv[1]) + 1;
	tagname = (char *)mxMalloc(tagnameLength);
	tagnameStatus = mxGetString(argv[1], tagname, tagnameLength);

	char *tagvalue;
	size_t tagvalueLength;
	int tagvalueStatus;
	tagvalueLength = mxGetN(argv[2]) + 1;
	tagvalue = (char *)mxMalloc(tagvalueLength);
	tagvalueStatus = mxGetString(argv[2], tagvalue, tagvalueLength);

	HelicsError err = helicsErrorInitialize();

	helicsPublicationSetTag(pub, (char const *)tagname, (char const *)tagvalue, &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	mxFree(tagname);

	mxFree(tagvalue);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsInputGetOption(resc, resv, argc, (mxArray**)(argv)){
	HelicsInput inp = (HelicsInput)(mxGetData(argv[0]));

	int option = (int)(mxGetScalar(argv[1]));

	int result = helicsInputGetOption(inp, option);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
	*((int64_T*)mxGetData(out)) = (int64_T)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}




}


void _wrap_helicsInputSetOption(resc, resv, argc, (mxArray**)(argv)){
	HelicsInput inp = (HelicsInput)(mxGetData(argv[0]));

	int option = (int)(mxGetScalar(argv[1]));

	int value = (int)(mxGetScalar(argv[2]));

	HelicsError err = helicsErrorInitialize();

	helicsInputSetOption(inp, option, value, &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}







	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsPublicationGetOption(resc, resv, argc, (mxArray**)(argv)){
	HelicsPublication pub = (HelicsPublication)(mxGetData(argv[0]));

	int option = (int)(mxGetScalar(argv[1]));

	int result = helicsPublicationGetOption(pub, option);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
	*((int64_T*)mxGetData(out)) = (int64_T)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}




}


void _wrap_helicsPublicationSetOption(resc, resv, argc, (mxArray**)(argv)){
	HelicsPublication pub = (HelicsPublication)(mxGetData(argv[0]));

	int option = (int)(mxGetScalar(argv[1]));

	int val = (int)(mxGetScalar(argv[2]));

	HelicsError err = helicsErrorInitialize();

	helicsPublicationSetOption(pub, option, val, &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}







	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsPublicationSetMinimumChange(resc, resv, argc, (mxArray**)(argv)){
	HelicsPublication pub = (HelicsPublication)(mxGetData(argv[0]));

	double tolerance = mxGetScalar(argv[1]);

	HelicsError err = helicsErrorInitialize();

	helicsPublicationSetMinimumChange(pub, tolerance, &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}





	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsInputSetMinimumChange(resc, resv, argc, (mxArray**)(argv)){
	HelicsInput inp = (HelicsInput)(mxGetData(argv[0]));

	double tolerance = mxGetScalar(argv[1]);

	HelicsError err = helicsErrorInitialize();

	helicsInputSetMinimumChange(inp, tolerance, &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}





	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsInputIsUpdated(resc, resv, argc, (mxArray**)(argv)){
	HelicsInput ipt = (HelicsInput)(mxGetData(argv[0]));

	HelicsBool result = helicsInputIsUpdated(ipt);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
	*((int64_T*)mxGetData(out)) = (int64_T)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsInputLastUpdateTime(resc, resv, argc, (mxArray**)(argv)){
	HelicsInput ipt = (HelicsInput)(mxGetData(argv[0]));

	HelicsTime result = helicsInputLastUpdateTime(ipt);

	mxArray *_out = mxCreateDoubleScalar(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsInputClearUpdate(resc, resv, argc, (mxArray**)(argv)){
	HelicsInput ipt = (HelicsInput)(mxGetData(argv[0]));

	helicsInputClearUpdate(ipt);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsFederateGetPublicationCount(resc, resv, argc, (mxArray**)(argv)){
	HelicsFederate fed = (HelicsFederate)(mxGetData(argv[0]));

	int result = helicsFederateGetPublicationCount(fed);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
	*((int64_T*)mxGetData(out)) = (int64_T)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsFederateGetInputCount(resc, resv, argc, (mxArray**)(argv)){
	HelicsFederate fed = (HelicsFederate)(mxGetData(argv[0]));

	int result = helicsFederateGetInputCount(fed);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
	*((int64_T*)mxGetData(out)) = (int64_T)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsFederateRegisterEndpoint(resc, resv, argc, (mxArray**)(argv)){
	HelicsFederate fed = (HelicsFederate)(mxGetData(argv[0]));

	char *name;
	size_t nameLength;
	int nameStatus;
	nameLength = mxGetN(argv[1]) + 1;
	name = (char *)mxMalloc(nameLength);
	nameStatus = mxGetString(argv[1], name, nameLength);

	char *type;
	size_t typeLength;
	int typeStatus;
	typeLength = mxGetN(argv[2]) + 1;
	type = (char *)mxMalloc(typeLength);
	typeStatus = mxGetString(argv[2], type, typeLength);

	HelicsError err = helicsErrorInitialize();

	HelicsEndpoint result = helicsFederateRegisterEndpoint(fed, (char const *)name, (char const *)type, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_T*)mxGetData(_out)) = (uint64_T)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	mxFree(name);

	mxFree(type);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateRegisterGlobalEndpoint(resc, resv, argc, (mxArray**)(argv)){
	HelicsFederate fed = (HelicsFederate)(mxGetData(argv[0]));

	char *name;
	size_t nameLength;
	int nameStatus;
	nameLength = mxGetN(argv[1]) + 1;
	name = (char *)mxMalloc(nameLength);
	nameStatus = mxGetString(argv[1], name, nameLength);

	char *type;
	size_t typeLength;
	int typeStatus;
	typeLength = mxGetN(argv[2]) + 1;
	type = (char *)mxMalloc(typeLength);
	typeStatus = mxGetString(argv[2], type, typeLength);

	HelicsError err = helicsErrorInitialize();

	HelicsEndpoint result = helicsFederateRegisterGlobalEndpoint(fed, (char const *)name, (char const *)type, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_T*)mxGetData(_out)) = (uint64_T)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	mxFree(name);

	mxFree(type);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateRegisterTargetedEndpoint(resc, resv, argc, (mxArray**)(argv)){
	HelicsFederate fed = (HelicsFederate)(mxGetData(argv[0]));

	char *name;
	size_t nameLength;
	int nameStatus;
	nameLength = mxGetN(argv[1]) + 1;
	name = (char *)mxMalloc(nameLength);
	nameStatus = mxGetString(argv[1], name, nameLength);

	char *type;
	size_t typeLength;
	int typeStatus;
	typeLength = mxGetN(argv[2]) + 1;
	type = (char *)mxMalloc(typeLength);
	typeStatus = mxGetString(argv[2], type, typeLength);

	HelicsError err = helicsErrorInitialize();

	HelicsEndpoint result = helicsFederateRegisterTargetedEndpoint(fed, (char const *)name, (char const *)type, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_T*)mxGetData(_out)) = (uint64_T)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	mxFree(name);

	mxFree(type);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateRegisterGlobalTargetedEndpoint(resc, resv, argc, (mxArray**)(argv)){
	HelicsFederate fed = (HelicsFederate)(mxGetData(argv[0]));

	char *name;
	size_t nameLength;
	int nameStatus;
	nameLength = mxGetN(argv[1]) + 1;
	name = (char *)mxMalloc(nameLength);
	nameStatus = mxGetString(argv[1], name, nameLength);

	char *type;
	size_t typeLength;
	int typeStatus;
	typeLength = mxGetN(argv[2]) + 1;
	type = (char *)mxMalloc(typeLength);
	typeStatus = mxGetString(argv[2], type, typeLength);

	HelicsError err = helicsErrorInitialize();

	HelicsEndpoint result = helicsFederateRegisterGlobalTargetedEndpoint(fed, (char const *)name, (char const *)type, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_T*)mxGetData(_out)) = (uint64_T)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	mxFree(name);

	mxFree(type);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateGetEndpoint(resc, resv, argc, (mxArray**)(argv)){
	HelicsFederate fed = (HelicsFederate)(mxGetData(argv[0]));

	char *name;
	size_t nameLength;
	int nameStatus;
	nameLength = mxGetN(argv[1]) + 1;
	name = (char *)mxMalloc(nameLength);
	nameStatus = mxGetString(argv[1], name, nameLength);

	HelicsError err = helicsErrorInitialize();

	HelicsEndpoint result = helicsFederateGetEndpoint(fed, (char const *)name, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_T*)mxGetData(_out)) = (uint64_T)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	mxFree(name);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateGetEndpointByIndex(resc, resv, argc, (mxArray**)(argv)){
	HelicsFederate fed = (HelicsFederate)(mxGetData(argv[0]));

	int index = (int)(mxGetScalar(argv[1]));

	HelicsError err = helicsErrorInitialize();

	HelicsEndpoint result = helicsFederateGetEndpointByIndex(fed, index, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_T*)mxGetData(_out)) = (uint64_T)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}





	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsEndpointIsValid(resc, resv, argc, (mxArray**)(argv)){
	HelicsEndpoint endpoint = (HelicsEndpoint)(mxGetData(argv[0]));

	HelicsBool result = helicsEndpointIsValid(endpoint);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
	*((int64_T*)mxGetData(out)) = (int64_T)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsEndpointSetDefaultDestination(resc, resv, argc, (mxArray**)(argv)){
	HelicsEndpoint endpoint = (HelicsEndpoint)(mxGetData(argv[0]));

	char *dst;
	size_t dstLength;
	int dstStatus;
	dstLength = mxGetN(argv[1]) + 1;
	dst = (char *)mxMalloc(dstLength);
	dstStatus = mxGetString(argv[1], dst, dstLength);

	HelicsError err = helicsErrorInitialize();

	helicsEndpointSetDefaultDestination(endpoint, (char const *)dst, &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	mxFree(dst);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsEndpointGetDefaultDestination(resc, resv, argc, (mxArray**)(argv)){
	HelicsEndpoint endpoint = (HelicsEndpoint)(mxGetData(argv[0]));

	char *result = helicsEndpointGetDefaultDestination(endpoint);

	mxArray *_out = mxCreateString((const char *)result);

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsEndpointSendBytes(resc, resv, argc, (mxArray**)(argv)){
	HelicsEndpoint endpoint = (HelicsEndpoint)(mxGetData(argv[0]));

	char *data;
	size_t dataLength;
	int dataStatus;
	dataLength = mxGetN(argv[1]) + 1;
	data = (char *)mxMalloc(dataLength);
	dataStatus = mxGetString(argv[1], data, dataLength);

	int inputDataLength = (int)(dataLength - 1);

	HelicsError err = helicsErrorInitialize();

	helicsEndpointSendBytes(endpoint, (void *)data, inputDataLength, &err);

	mxArray *_out = (mxArray *)0;
	if(_out){
		--resc;
		*resv++ = _out;
	}

	mxFree(data);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsEndpointSendBytesTo(resc, resv, argc, (mxArray**)(argv)){
	HelicsEndpoint endpoint = (HelicsEndpoint)(mxGetData(argv[0]));

	char *data;
	size_t dataLength;
	int dataStatus;
	dataLength = mxGetN(argv[1]) + 1;
	data = (char *)mxMalloc(dataLength);
	dataStatus = mxGetString(argv[1], data, dataLength);

	int inputDataLength = (int)(dataLength - 1);

	char *dst;
	size_t dstLength;
	int dstStatus;
	dstLength = mxGetN(argv[2]) + 1;
	dst = (char *)mxMalloc(dstLength);
	dstStatus = mxGetString(argv[2], dst, dstLength);

	HelicsError err = helicsErrorInitialize();

	helicsEndpointSendBytesTo(endpoint, (void *)data, inputDataLength, dst, &err);

	mxArray *_out = (mxArray *)0;
	if(_out){
		--resc;
		*resv++ = _out;
	}

	mxFree(data);

	mxFree(dst);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsEndpointSendBytesToAt(resc, resv, argc, (mxArray**)(argv)){
	HelicsEndpoint endpoint = (HelicsEndpoint)(mxGetData(argv[0]));

	char *data;
	size_t dataLength;
	int dataStatus;
	dataLength = mxGetN(argv[1]) + 1;
	data = (char *)mxMalloc(dataLength);
	dataStatus = mxGetString(argv[1], data, dataLength);

	int inputDataLength = (int)(dataLength - 1);

	char *dst;
	size_t dstLength;
	int dstStatus;
	dstLength = mxGetN(argv[2]) + 1;
	dst = (char *)mxMalloc(dstLength);
	dstStatus = mxGetString(argv[2], dst, dstLength);

	HelicsTime time = (HelicsTime)(mxGetScalar(argv[3]));

	HelicsError err = helicsErrorInitialize();

	helicsEndpointSendBytesToAt(endpoint, (void *)data, inputDataLength, dst, time, &err);

	mxArray *_out = (mxArray *)0;
	if(_out){
		--resc;
		*resv++ = _out;
	}

	mxFree(data);

	mxFree(dst);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsEndpointSendBytesAt(resc, resv, argc, (mxArray**)(argv)){
	HelicsEndpoint endpoint = (HelicsEndpoint)(mxGetData(argv[0]));

	char *data;
	size_t dataLength;
	int dataStatus;
	dataLength = mxGetN(argv[1]) + 1;
	data = (char *)mxMalloc(dataLength);
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

	mxFree(data);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsEndpointSendMessage(resc, resv, argc, (mxArray**)(argv)){
	HelicsEndpoint endpoint = (HelicsEndpoint)(mxGetData(argv[0]));

	HelicsMessage message = (HelicsMessage)(mxGetData(argv[1]));

	HelicsError err = helicsErrorInitialize();

	helicsEndpointSendMessage(endpoint, message, &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}





	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsEndpointSendMessageZeroCopy(resc, resv, argc, (mxArray**)(argv)){
	HelicsEndpoint endpoint = (HelicsEndpoint)(mxGetData(argv[0]));

	HelicsMessage message = (HelicsMessage)(mxGetData(argv[1]));

	HelicsError err = helicsErrorInitialize();

	helicsEndpointSendMessageZeroCopy(endpoint, message, &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}





	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsEndpointSubscribe(resc, resv, argc, (mxArray**)(argv)){
	HelicsEndpoint endpoint = (HelicsEndpoint)(mxGetData(argv[0]));

	char *key;
	size_t keyLength;
	int keyStatus;
	keyLength = mxGetN(argv[1]) + 1;
	key = (char *)mxMalloc(keyLength);
	keyStatus = mxGetString(argv[1], key, keyLength);

	HelicsError err = helicsErrorInitialize();

	helicsEndpointSubscribe(endpoint, (char const *)key, &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	mxFree(key);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateHasMessage(resc, resv, argc, (mxArray**)(argv)){
	HelicsFederate fed = (HelicsFederate)(mxGetData(argv[0]));

	HelicsBool result = helicsFederateHasMessage(fed);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
	*((int64_T*)mxGetData(out)) = (int64_T)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsEndpointHasMessage(resc, resv, argc, (mxArray**)(argv)){
	HelicsEndpoint endpoint = (HelicsEndpoint)(mxGetData(argv[0]));

	HelicsBool result = helicsEndpointHasMessage(endpoint);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
	*((int64_T*)mxGetData(out)) = (int64_T)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsFederatePendingMessageCount(resc, resv, argc, (mxArray**)(argv)){
	HelicsFederate fed = (HelicsFederate)(mxGetData(argv[0]));

	int result = helicsFederatePendingMessageCount(fed);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
	*((int64_T*)mxGetData(out)) = (int64_T)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsEndpointPendingMessageCount(resc, resv, argc, (mxArray**)(argv)){
	HelicsEndpoint endpoint = (HelicsEndpoint)(mxGetData(argv[0]));

	int result = helicsEndpointPendingMessageCount(endpoint);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
	*((int64_T*)mxGetData(out)) = (int64_T)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsEndpointGetMessage(resc, resv, argc, (mxArray**)(argv)){
	HelicsEndpoint endpoint = (HelicsEndpoint)(mxGetData(argv[0]));

	HelicsMessage result = helicsEndpointGetMessage(endpoint);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_T*)mxGetData(_out)) = (uint64_T)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsEndpointCreateMessage(resc, resv, argc, (mxArray**)(argv)){
	HelicsEndpoint endpoint = (HelicsEndpoint)(mxGetData(argv[0]));

	HelicsError err = helicsErrorInitialize();

	HelicsMessage result = helicsEndpointCreateMessage(endpoint, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_T*)mxGetData(_out)) = (uint64_T)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateGetMessage(resc, resv, argc, (mxArray**)(argv)){
	HelicsFederate fed = (HelicsFederate)(mxGetData(argv[0]));

	HelicsMessage result = helicsFederateGetMessage(fed);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_T*)mxGetData(_out)) = (uint64_T)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsFederateCreateMessage(resc, resv, argc, (mxArray**)(argv)){
	HelicsFederate fed = (HelicsFederate)(mxGetData(argv[0]));

	HelicsError err = helicsErrorInitialize();

	HelicsMessage result = helicsFederateCreateMessage(fed, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_T*)mxGetData(_out)) = (uint64_T)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateClearMessages(resc, resv, argc, (mxArray**)(argv)){
	HelicsFederate fed = (HelicsFederate)(mxGetData(argv[0]));

	helicsFederateClearMessages(fed);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsEndpointGetType(resc, resv, argc, (mxArray**)(argv)){
	HelicsEndpoint endpoint = (HelicsEndpoint)(mxGetData(argv[0]));

	char *result = helicsEndpointGetType(endpoint);

	mxArray *_out = mxCreateString((const char *)result);

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsEndpointGetName(resc, resv, argc, (mxArray**)(argv)){
	HelicsEndpoint endpoint = (HelicsEndpoint)(mxGetData(argv[0]));

	char *result = helicsEndpointGetName(endpoint);

	mxArray *_out = mxCreateString((const char *)result);

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsFederateGetEndpointCount(resc, resv, argc, (mxArray**)(argv)){
	HelicsFederate fed = (HelicsFederate)(mxGetData(argv[0]));

	int result = helicsFederateGetEndpointCount(fed);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
	*((int64_T*)mxGetData(out)) = (int64_T)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsEndpointGetInfo(resc, resv, argc, (mxArray**)(argv)){
	HelicsEndpoint end = (HelicsEndpoint)(mxGetData(argv[0]));

	char *result = helicsEndpointGetInfo(end);

	mxArray *_out = mxCreateString((const char *)result);

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsEndpointSetInfo(resc, resv, argc, (mxArray**)(argv)){
	HelicsEndpoint endpoint = (HelicsEndpoint)(mxGetData(argv[0]));

	char *info;
	size_t infoLength;
	int infoStatus;
	infoLength = mxGetN(argv[1]) + 1;
	info = (char *)mxMalloc(infoLength);
	infoStatus = mxGetString(argv[1], info, infoLength);

	HelicsError err = helicsErrorInitialize();

	helicsEndpointSetInfo(endpoint, (char const *)info, &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	mxFree(info);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsEndpointGetTag(resc, resv, argc, (mxArray**)(argv)){
	HelicsEndpoint endpoint = (HelicsEndpoint)(mxGetData(argv[0]));

	char *tagname;
	size_t tagnameLength;
	int tagnameStatus;
	tagnameLength = mxGetN(argv[1]) + 1;
	tagname = (char *)mxMalloc(tagnameLength);
	tagnameStatus = mxGetString(argv[1], tagname, tagnameLength);

	char *result = helicsEndpointGetTag(endpoint, (char const *)tagname);

	mxArray *_out = mxCreateString((const char *)result);

	if(_out){
		--resc;
		*resv++ = _out;
	}



	mxFree(tagname);
}


void _wrap_helicsEndpointSetTag(resc, resv, argc, (mxArray**)(argv)){
	HelicsEndpoint endpoint = (HelicsEndpoint)(mxGetData(argv[0]));

	char *tagname;
	size_t tagnameLength;
	int tagnameStatus;
	tagnameLength = mxGetN(argv[1]) + 1;
	tagname = (char *)mxMalloc(tagnameLength);
	tagnameStatus = mxGetString(argv[1], tagname, tagnameLength);

	char *tagvalue;
	size_t tagvalueLength;
	int tagvalueStatus;
	tagvalueLength = mxGetN(argv[2]) + 1;
	tagvalue = (char *)mxMalloc(tagvalueLength);
	tagvalueStatus = mxGetString(argv[2], tagvalue, tagvalueLength);

	HelicsError err = helicsErrorInitialize();

	helicsEndpointSetTag(endpoint, (char const *)tagname, (char const *)tagvalue, &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	mxFree(tagname);

	mxFree(tagvalue);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsEndpointSetOption(resc, resv, argc, (mxArray**)(argv)){
	HelicsEndpoint endpoint = (HelicsEndpoint)(mxGetData(argv[0]));

	int option = (int)(mxGetScalar(argv[1]));

	int value = (int)(mxGetScalar(argv[2]));

	HelicsError err = helicsErrorInitialize();

	helicsEndpointSetOption(endpoint, option, value, &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}







	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsEndpointGetOption(resc, resv, argc, (mxArray**)(argv)){
	HelicsEndpoint endpoint = (HelicsEndpoint)(mxGetData(argv[0]));

	int option = (int)(mxGetScalar(argv[1]));

	int result = helicsEndpointGetOption(endpoint, option);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
	*((int64_T*)mxGetData(out)) = (int64_T)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}




}


void _wrap_helicsEndpointAddSourceTarget(resc, resv, argc, (mxArray**)(argv)){
	HelicsEndpoint endpoint = (HelicsEndpoint)(mxGetData(argv[0]));

	char *targetEndpoint;
	size_t targetEndpointLength;
	int targetEndpointStatus;
	targetEndpointLength = mxGetN(argv[1]) + 1;
	targetEndpoint = (char *)mxMalloc(targetEndpointLength);
	targetEndpointStatus = mxGetString(argv[1], targetEndpoint, targetEndpointLength);

	HelicsError err = helicsErrorInitialize();

	helicsEndpointAddSourceTarget(endpoint, (char const *)targetEndpoint, &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	mxFree(targetEndpoint);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsEndpointAddDestinationTarget(resc, resv, argc, (mxArray**)(argv)){
	HelicsEndpoint endpoint = (HelicsEndpoint)(mxGetData(argv[0]));

	char *targetEndpoint;
	size_t targetEndpointLength;
	int targetEndpointStatus;
	targetEndpointLength = mxGetN(argv[1]) + 1;
	targetEndpoint = (char *)mxMalloc(targetEndpointLength);
	targetEndpointStatus = mxGetString(argv[1], targetEndpoint, targetEndpointLength);

	HelicsError err = helicsErrorInitialize();

	helicsEndpointAddDestinationTarget(endpoint, (char const *)targetEndpoint, &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	mxFree(targetEndpoint);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsEndpointRemoveTarget(resc, resv, argc, (mxArray**)(argv)){
	HelicsEndpoint endpoint = (HelicsEndpoint)(mxGetData(argv[0]));

	char *targetEndpoint;
	size_t targetEndpointLength;
	int targetEndpointStatus;
	targetEndpointLength = mxGetN(argv[1]) + 1;
	targetEndpoint = (char *)mxMalloc(targetEndpointLength);
	targetEndpointStatus = mxGetString(argv[1], targetEndpoint, targetEndpointLength);

	HelicsError err = helicsErrorInitialize();

	helicsEndpointRemoveTarget(endpoint, (char const *)targetEndpoint, &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	mxFree(targetEndpoint);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsEndpointAddSourceFilter(resc, resv, argc, (mxArray**)(argv)){
	HelicsEndpoint endpoint = (HelicsEndpoint)(mxGetData(argv[0]));

	char *filterName;
	size_t filterNameLength;
	int filterNameStatus;
	filterNameLength = mxGetN(argv[1]) + 1;
	filterName = (char *)mxMalloc(filterNameLength);
	filterNameStatus = mxGetString(argv[1], filterName, filterNameLength);

	HelicsError err = helicsErrorInitialize();

	helicsEndpointAddSourceFilter(endpoint, (char const *)filterName, &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	mxFree(filterName);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsEndpointAddDestinationFilter(resc, resv, argc, (mxArray**)(argv)){
	HelicsEndpoint endpoint = (HelicsEndpoint)(mxGetData(argv[0]));

	char *filterName;
	size_t filterNameLength;
	int filterNameStatus;
	filterNameLength = mxGetN(argv[1]) + 1;
	filterName = (char *)mxMalloc(filterNameLength);
	filterNameStatus = mxGetString(argv[1], filterName, filterNameLength);

	HelicsError err = helicsErrorInitialize();

	helicsEndpointAddDestinationFilter(endpoint, (char const *)filterName, &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	mxFree(filterName);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsMessageGetSource(resc, resv, argc, (mxArray**)(argv)){
	HelicsMessage message = (HelicsMessage)(mxGetData(argv[0]));

	char *result = helicsMessageGetSource(message);

	mxArray *_out = mxCreateString((const char *)result);

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsMessageGetDestination(resc, resv, argc, (mxArray**)(argv)){
	HelicsMessage message = (HelicsMessage)(mxGetData(argv[0]));

	char *result = helicsMessageGetDestination(message);

	mxArray *_out = mxCreateString((const char *)result);

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsMessageGetOriginalSource(resc, resv, argc, (mxArray**)(argv)){
	HelicsMessage message = (HelicsMessage)(mxGetData(argv[0]));

	char *result = helicsMessageGetOriginalSource(message);

	mxArray *_out = mxCreateString((const char *)result);

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsMessageGetOriginalDestination(resc, resv, argc, (mxArray**)(argv)){
	HelicsMessage message = (HelicsMessage)(mxGetData(argv[0]));

	char *result = helicsMessageGetOriginalDestination(message);

	mxArray *_out = mxCreateString((const char *)result);

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsMessageGetTime(resc, resv, argc, (mxArray**)(argv)){
	HelicsMessage message = (HelicsMessage)(mxGetData(argv[0]));

	HelicsTime result = helicsMessageGetTime(message);

	mxArray *_out = mxCreateDoubleScalar(result);

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsMessageGetString(resc, resv, argc, (mxArray**)(argv)){
	HelicsMessage message = (HelicsMessage)(mxGetData(argv[0]));

	char *result = helicsMessageGetString(message);

	mxArray *_out = mxCreateString((const char *)result);

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsMessageGetMessageID(resc, resv, argc, (mxArray**)(argv)){
	HelicsMessage message = (HelicsMessage)(mxGetData(argv[0]));

	int result = helicsMessageGetMessageID(message);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
	*((int64_T*)mxGetData(out)) = (int64_T)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsMessageGetFlagOption(resc, resv, argc, (mxArray**)(argv)){
	HelicsMessage message = (HelicsMessage)(mxGetData(argv[0]));

	int flag = (int)(mxGetScalar(argv[1]));

	HelicsBool result = helicsMessageGetFlagOption(message, flag);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
	*((int64_T*)mxGetData(out)) = (int64_T)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}




}


void _wrap_helicsMessageGetByteCount(resc, resv, argc, (mxArray**)(argv)){
	HelicsMessage message = (HelicsMessage)(mxGetData(argv[0]));

	int result = helicsMessageGetByteCount(message);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
	*((int64_T*)mxGetData(out)) = (int64_T)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsMessageGetBytes(resc, resv, argc, (mxArray**)(argv)){
	HelicsMessage message = (HelicsMessage)(mxGetData(argv[0]));

	int maxMessageLength = helicsMessageGetByteCount(message) + 2;

	char *data = (char *)mxMalloc(maxMessageLength);

	int *actualSize = (int *)0;

	HelicsError err = helicsErrorInitialize();

	helicsMessageGetBytes(message, (void *)data, maxMessageLength, actualSize, &err);

	mwSize dims[2] = {1,(mwSize)*actualSize};
	mxArray *_out = mxCreateCharArray(2,dims);
	mxChar *out_data = (mxChar *)mxGetData(_out);
	for(int i=0; i<*actualSize; ++i){
		out_data[i] = data[i];
	}

	if(_out){
		--resc;
		*resv++ = _out;
	}

	mxFree(data);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsMessageGetBytesPointer(resc, resv, argc, (mxArray**)(argv)){
	HelicsMessage message = (HelicsMessage)(mxGetData(argv[0]));

	void *reault = helicsMessageGetBytesPointer(message);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_T*)mxGetData(_out)) = (uint64_T)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsMessageIsValid(resc, resv, argc, (mxArray**)(argv)){
	HelicsMessage message = (HelicsMessage)(mxGetData(argv[0]));

	HelicsBool result = helicsMessageIsValid(message);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
	*((int64_T*)mxGetData(out)) = (int64_T)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsMessageSetSource(resc, resv, argc, (mxArray**)(argv)){
	HelicsMessage message = (HelicsMessage)(mxGetData(argv[0]));

	char *src;
	size_t srcLength;
	int srcStatus;
	srcLength = mxGetN(argv[1]) + 1;
	src = (char *)mxMalloc(srcLength);
	srcStatus = mxGetString(argv[1], src, srcLength);

	HelicsError err = helicsErrorInitialize();

	helicsMessageSetSource(message, (char const *)src, &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	mxFree(src);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsMessageSetDestination(resc, resv, argc, (mxArray**)(argv)){
	HelicsMessage message = (HelicsMessage)(mxGetData(argv[0]));

	char *dst;
	size_t dstLength;
	int dstStatus;
	dstLength = mxGetN(argv[1]) + 1;
	dst = (char *)mxMalloc(dstLength);
	dstStatus = mxGetString(argv[1], dst, dstLength);

	HelicsError err = helicsErrorInitialize();

	helicsMessageSetDestination(message, (char const *)dst, &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	mxFree(dst);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsMessageSetOriginalSource(resc, resv, argc, (mxArray**)(argv)){
	HelicsMessage message = (HelicsMessage)(mxGetData(argv[0]));

	char *src;
	size_t srcLength;
	int srcStatus;
	srcLength = mxGetN(argv[1]) + 1;
	src = (char *)mxMalloc(srcLength);
	srcStatus = mxGetString(argv[1], src, srcLength);

	HelicsError err = helicsErrorInitialize();

	helicsMessageSetOriginalSource(message, (char const *)src, &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	mxFree(src);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsMessageSetOriginalDestination(resc, resv, argc, (mxArray**)(argv)){
	HelicsMessage message = (HelicsMessage)(mxGetData(argv[0]));

	char *dst;
	size_t dstLength;
	int dstStatus;
	dstLength = mxGetN(argv[1]) + 1;
	dst = (char *)mxMalloc(dstLength);
	dstStatus = mxGetString(argv[1], dst, dstLength);

	HelicsError err = helicsErrorInitialize();

	helicsMessageSetOriginalDestination(message, (char const *)dst, &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	mxFree(dst);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsMessageSetTime(resc, resv, argc, (mxArray**)(argv)){
	HelicsMessage message = (HelicsMessage)(mxGetData(argv[0]));

	HelicsTime time = (HelicsTime)(mxGetScalar(argv[1]));

	HelicsError err = helicsErrorInitialize();

	helicsMessageSetTime(message, time, &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}





	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsMessageResize(resc, resv, argc, (mxArray**)(argv)){
	HelicsMessage message = (HelicsMessage)(mxGetData(argv[0]));

	int newSize = (int)(mxGetScalar(argv[1]));

	HelicsError err = helicsErrorInitialize();

	helicsMessageResize(message, newSize, &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}





	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsMessageReserve(resc, resv, argc, (mxArray**)(argv)){
	HelicsMessage message = (HelicsMessage)(mxGetData(argv[0]));

	int reserveSize = (int)(mxGetScalar(argv[1]));

	HelicsError err = helicsErrorInitialize();

	helicsMessageReserve(message, reserveSize, &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}





	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsMessageSetMessageID(resc, resv, argc, (mxArray**)(argv)){
	HelicsMessage message = (HelicsMessage)(mxGetData(argv[0]));

	int32_t messageID = *((int32_t *)mxGetData(argv[1]));

	HelicsError err = helicsErrorInitialize();

	helicsMessageSetMessageID(message, messageID, &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}





	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsMessageClearFlags(resc, resv, argc, (mxArray**)(argv)){
	HelicsMessage message = (HelicsMessage)(mxGetData(argv[0]));

	helicsMessageClearFlags(message);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsMessageSetFlagOption(resc, resv, argc, (mxArray**)(argv)){
	HelicsMessage message = (HelicsMessage)(mxGetData(argv[0]));

	int flag = (int)(mxGetScalar(argv[1]));

	HelicsBool flagValue = (HelicsBool)(mxGetScalar(argv[2]));

	HelicsError err = helicsErrorInitialize();

	helicsMessageSetFlagOption(message, flag, flagValue, &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}







	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsMessageSetString(resc, resv, argc, (mxArray**)(argv)){
	HelicsMessage message = (HelicsMessage)(mxGetData(argv[0]));

	char *str;
	size_t strLength;
	int strStatus;
	strLength = mxGetN(argv[1]) + 1;
	str = (char *)mxMalloc(strLength);
	strStatus = mxGetString(argv[1], str, strLength);

	HelicsError err = helicsErrorInitialize();

	helicsMessageSetString(message, (char const *)str, &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	mxFree(str);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsMessageSetData(resc, resv, argc, (mxArray**)(argv)){
	HelicsMessage message = (HelicsMessage)(mxGetData(argv[0]));

	char *data;
	size_t dataLength;
	int dataStatus;
	dataLength = mxGetN(argv[1]) + 1;
	data = (char *)mxMalloc(dataLength);
	dataStatus = mxGetString(argv[1], data, dataLength);

	int inputDataLength = dataLength - 1;

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


void _wrap_helicsMessageAppendData(resc, resv, argc, (mxArray**)(argv)){
	HelicsMessage message = (HelicsMessage)(mxGetData(argv[0]));

	char *data;
	size_t dataLength;
	int dataStatus;
	dataLength = mxGetN(argv[1]) + 1;
	data = (char *)mxMalloc(dataLength);
	dataStatus = mxGetString(argv[1], data, dataLength);

	int inputDataLength = dataLength - 1;

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


void _wrap_helicsMessageCopy(resc, resv, argc, (mxArray**)(argv)){
	HelicsMessage src_message = (HelicsMessage)(mxGetData(argv[0]));

	HelicsMessage dst_message = (HelicsMessage)(mxGetData(argv[1]));

	HelicsError err = helicsErrorInitialize();

	helicsMessageCopy(src_message, dst_message, &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}





	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsMessageClone(resc, resv, argc, (mxArray**)(argv)){
	HelicsMessage message = (HelicsMessage)(mxGetData(argv[0]));

	HelicsError err = helicsErrorInitialize();

	HelicsMessage result = helicsMessageClone(message, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_T*)mxGetData(_out)) = (uint64_T)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsMessageFree(resc, resv, argc, (mxArray**)(argv)){
	HelicsMessage message = (HelicsMessage)(mxGetData(argv[0]));

	helicsMessageFree(message);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsMessageClear(resc, resv, argc, (mxArray**)(argv)){
	HelicsMessage message = (HelicsMessage)(mxGetData(argv[0]));

	HelicsError err = helicsErrorInitialize();

	helicsMessageClear(message, &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateRegisterFilter(resc, resv, argc, (mxArray**)(argv)){
	HelicsFederate fed = (HelicsFederate)(mxGetData(argv[0]));

	HelicsFilterTypes type = (HelicsFilterTypes)(mxGetScalar(argv[1]));

	char *name;
	size_t nameLength;
	int nameStatus;
	nameLength = mxGetN(argv[2]) + 1;
	name = (char *)mxMalloc(nameLength);
	nameStatus = mxGetString(argv[2], name, nameLength);

	HelicsError err = helicsErrorInitialize();

	HelicsFilter result = helicsFederateRegisterFilter(fed, type, (char const *)name, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_T*)mxGetData(_out)) = (uint64_T)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}





	mxFree(name);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateRegisterGlobalFilter(resc, resv, argc, (mxArray**)(argv)){
	HelicsFederate fed = (HelicsFederate)(mxGetData(argv[0]));

	HelicsFilterTypes type = (HelicsFilterTypes)(mxGetScalar(argv[1]));

	char *name;
	size_t nameLength;
	int nameStatus;
	nameLength = mxGetN(argv[2]) + 1;
	name = (char *)mxMalloc(nameLength);
	nameStatus = mxGetString(argv[2], name, nameLength);

	HelicsError err = helicsErrorInitialize();

	HelicsFilter result = helicsFederateRegisterGlobalFilter(fed, type, (char const *)name, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_T*)mxGetData(_out)) = (uint64_T)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}





	mxFree(name);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateRegisterCloningFilter(resc, resv, argc, (mxArray**)(argv)){
	HelicsFederate fed = (HelicsFederate)(mxGetData(argv[0]));

	char *name;
	size_t nameLength;
	int nameStatus;
	nameLength = mxGetN(argv[1]) + 1;
	name = (char *)mxMalloc(nameLength);
	nameStatus = mxGetString(argv[1], name, nameLength);

	HelicsError err = helicsErrorInitialize();

	HelicsFilter result = helicsFederateRegisterCloningFilter(fed, (char const *)name, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_T*)mxGetData(_out)) = (uint64_T)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	mxFree(name);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateRegisterGlobalCloningFilter(resc, resv, argc, (mxArray**)(argv)){
	HelicsFederate fed = (HelicsFederate)(mxGetData(argv[0]));

	char *name;
	size_t nameLength;
	int nameStatus;
	nameLength = mxGetN(argv[1]) + 1;
	name = (char *)mxMalloc(nameLength);
	nameStatus = mxGetString(argv[1], name, nameLength);

	HelicsError err = helicsErrorInitialize();

	HelicsFilter result = helicsFederateRegisterGlobalCloningFilter(fed, (char const *)name, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_T*)mxGetData(_out)) = (uint64_T)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	mxFree(name);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsCoreRegisterFilter(resc, resv, argc, (mxArray**)(argv)){
	HelicsCore core = (HelicsCore)(mxGetData(argv[0]));

	HelicsFilterTypes type = (HelicsFilterTypes)(mxGetScalar(argv[1]));

	char *name;
	size_t nameLength;
	int nameStatus;
	nameLength = mxGetN(argv[2]) + 1;
	name = (char *)mxMalloc(nameLength);
	nameStatus = mxGetString(argv[2], name, nameLength);

	HelicsError err = helicsErrorInitialize();

	HelicsFilter result = helicsCoreRegisterFilter(core, type, (char const *)name, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_T*)mxGetData(_out)) = (uint64_T)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}





	mxFree(name);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsCoreRegisterCloningFilter(resc, resv, argc, (mxArray**)(argv)){
	HelicsCore core = (HelicsCore)(mxGetData(argv[0]));

	char *name;
	size_t nameLength;
	int nameStatus;
	nameLength = mxGetN(argv[1]) + 1;
	name = (char *)mxMalloc(nameLength);
	nameStatus = mxGetString(argv[1], name, nameLength);

	HelicsError err = helicsErrorInitialize();

	HelicsFilter result = helicsCoreRegisterCloningFilter(core, (char const *)name, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_T*)mxGetData(_out)) = (uint64_T)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	mxFree(name);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateGetFilterCount(resc, resv, argc, (mxArray**)(argv)){
	HelicsFederate fed = (HelicsFederate)(mxGetData(argv[0]));

	int result = helicsFederateGetFilterCount(fed);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
	*((int64_T*)mxGetData(out)) = (int64_T)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsFederateGetFilter(resc, resv, argc, (mxArray**)(argv)){
	HelicsFederate fed = (HelicsFederate)(mxGetData(argv[0]));

	char *name;
	size_t nameLength;
	int nameStatus;
	nameLength = mxGetN(argv[1]) + 1;
	name = (char *)mxMalloc(nameLength);
	nameStatus = mxGetString(argv[1], name, nameLength);

	HelicsError err = helicsErrorInitialize();

	HelicsFilter result = helicsFederateGetFilter(fed, (char const *)name, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_T*)mxGetData(_out)) = (uint64_T)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	mxFree(name);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateGetFilterByIndex(resc, resv, argc, (mxArray**)(argv)){
	HelicsFederate fed = (HelicsFederate)(mxGetData(argv[0]));

	int index = (int)(mxGetScalar(argv[1]));

	HelicsError err = helicsErrorInitialize();

	HelicsFilter result = helicsFederateGetFilterByIndex(fed, index, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_T*)mxGetData(_out)) = (uint64_T)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}





	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFilterIsValid(resc, resv, argc, (mxArray**)(argv)){
	HelicsFilter filt = (HelicsFilter)(mxGetData(argv[0]));

	HelicsBool result = helicsFilterIsValid(filt);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
	*((int64_T*)mxGetData(out)) = (int64_T)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsFilterGetName(resc, resv, argc, (mxArray**)(argv)){
	HelicsFilter filt = (HelicsFilter)(mxGetData(argv[0]));

	char *result = helicsFilterGetName(filt);

	mxArray *_out = mxCreateString((const char *)result);

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsFilterSet(resc, resv, argc, (mxArray**)(argv)){
	HelicsFilter filt = (HelicsFilter)(mxGetData(argv[0]));

	char *prop;
	size_t propLength;
	int propStatus;
	propLength = mxGetN(argv[1]) + 1;
	prop = (char *)mxMalloc(propLength);
	propStatus = mxGetString(argv[1], prop, propLength);

	double val = mxGetScalar(argv[2]);

	HelicsError err = helicsErrorInitialize();

	helicsFilterSet(filt, (char const *)prop, val, &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	mxFree(prop);



	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFilterSetString(resc, resv, argc, (mxArray**)(argv)){
	HelicsFilter filt = (HelicsFilter)(mxGetData(argv[0]));

	char *prop;
	size_t propLength;
	int propStatus;
	propLength = mxGetN(argv[1]) + 1;
	prop = (char *)mxMalloc(propLength);
	propStatus = mxGetString(argv[1], prop, propLength);

	char *val;
	size_t valLength;
	int valStatus;
	valLength = mxGetN(argv[2]) + 1;
	val = (char *)mxMalloc(valLength);
	valStatus = mxGetString(argv[2], val, valLength);

	HelicsError err = helicsErrorInitialize();

	helicsFilterSetString(filt, (char const *)prop, (char const *)val, &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	mxFree(prop);

	mxFree(val);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFilterAddDestinationTarget(resc, resv, argc, (mxArray**)(argv)){
	HelicsFilter filt = (HelicsFilter)(mxGetData(argv[0]));

	char *dst;
	size_t dstLength;
	int dstStatus;
	dstLength = mxGetN(argv[1]) + 1;
	dst = (char *)mxMalloc(dstLength);
	dstStatus = mxGetString(argv[1], dst, dstLength);

	HelicsError err = helicsErrorInitialize();

	helicsFilterAddDestinationTarget(filt, (char const *)dst, &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	mxFree(dst);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFilterAddSourceTarget(resc, resv, argc, (mxArray**)(argv)){
	HelicsFilter filt = (HelicsFilter)(mxGetData(argv[0]));

	char *source;
	size_t sourceLength;
	int sourceStatus;
	sourceLength = mxGetN(argv[1]) + 1;
	source = (char *)mxMalloc(sourceLength);
	sourceStatus = mxGetString(argv[1], source, sourceLength);

	HelicsError err = helicsErrorInitialize();

	helicsFilterAddSourceTarget(filt, (char const *)source, &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	mxFree(source);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFilterAddDeliveryEndpoint(resc, resv, argc, (mxArray**)(argv)){
	HelicsFilter filt = (HelicsFilter)(mxGetData(argv[0]));

	char *deliveryEndpoint;
	size_t deliveryEndpointLength;
	int deliveryEndpointStatus;
	deliveryEndpointLength = mxGetN(argv[1]) + 1;
	deliveryEndpoint = (char *)mxMalloc(deliveryEndpointLength);
	deliveryEndpointStatus = mxGetString(argv[1], deliveryEndpoint, deliveryEndpointLength);

	HelicsError err = helicsErrorInitialize();

	helicsFilterAddDeliveryEndpoint(filt, (char const *)deliveryEndpoint, &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	mxFree(deliveryEndpoint);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFilterRemoveTarget(resc, resv, argc, (mxArray**)(argv)){
	HelicsFilter filt = (HelicsFilter)(mxGetData(argv[0]));

	char *target;
	size_t targetLength;
	int targetStatus;
	targetLength = mxGetN(argv[1]) + 1;
	target = (char *)mxMalloc(targetLength);
	targetStatus = mxGetString(argv[1], target, targetLength);

	HelicsError err = helicsErrorInitialize();

	helicsFilterRemoveTarget(filt, (char const *)target, &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	mxFree(target);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFilterRemoveDeliveryEndpoint(resc, resv, argc, (mxArray**)(argv)){
	HelicsFilter filt = (HelicsFilter)(mxGetData(argv[0]));

	char *deliveryEndpoint;
	size_t deliveryEndpointLength;
	int deliveryEndpointStatus;
	deliveryEndpointLength = mxGetN(argv[1]) + 1;
	deliveryEndpoint = (char *)mxMalloc(deliveryEndpointLength);
	deliveryEndpointStatus = mxGetString(argv[1], deliveryEndpoint, deliveryEndpointLength);

	HelicsError err = helicsErrorInitialize();

	helicsFilterRemoveDeliveryEndpoint(filt, (char const *)deliveryEndpoint, &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	mxFree(deliveryEndpoint);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFilterGetInfo(resc, resv, argc, (mxArray**)(argv)){
	HelicsFilter filt = (HelicsFilter)(mxGetData(argv[0]));

	char *result = helicsFilterGetInfo(filt);

	mxArray *_out = mxCreateString((const char *)result);

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsFilterSetInfo(resc, resv, argc, (mxArray**)(argv)){
	HelicsFilter filt = (HelicsFilter)(mxGetData(argv[0]));

	char *info;
	size_t infoLength;
	int infoStatus;
	infoLength = mxGetN(argv[1]) + 1;
	info = (char *)mxMalloc(infoLength);
	infoStatus = mxGetString(argv[1], info, infoLength);

	HelicsError err = helicsErrorInitialize();

	helicsFilterSetInfo(filt, (char const *)info, &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	mxFree(info);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFilterGetTag(resc, resv, argc, (mxArray**)(argv)){
	HelicsFilter filt = (HelicsFilter)(mxGetData(argv[0]));

	char *tagname;
	size_t tagnameLength;
	int tagnameStatus;
	tagnameLength = mxGetN(argv[1]) + 1;
	tagname = (char *)mxMalloc(tagnameLength);
	tagnameStatus = mxGetString(argv[1], tagname, tagnameLength);

	char *result = helicsFilterGetTag(filt, (char const *)tagname);

	mxArray *_out = mxCreateString((const char *)result);

	if(_out){
		--resc;
		*resv++ = _out;
	}



	mxFree(tagname);
}


void _wrap_helicsFilterSetTag(resc, resv, argc, (mxArray**)(argv)){
	HelicsFilter filt = (HelicsFilter)(mxGetData(argv[0]));

	char *tagname;
	size_t tagnameLength;
	int tagnameStatus;
	tagnameLength = mxGetN(argv[1]) + 1;
	tagname = (char *)mxMalloc(tagnameLength);
	tagnameStatus = mxGetString(argv[1], tagname, tagnameLength);

	char *tagvalue;
	size_t tagvalueLength;
	int tagvalueStatus;
	tagvalueLength = mxGetN(argv[2]) + 1;
	tagvalue = (char *)mxMalloc(tagvalueLength);
	tagvalueStatus = mxGetString(argv[2], tagvalue, tagvalueLength);

	HelicsError err = helicsErrorInitialize();

	helicsFilterSetTag(filt, (char const *)tagname, (char const *)tagvalue, &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	mxFree(tagname);

	mxFree(tagvalue);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFilterSetOption(resc, resv, argc, (mxArray**)(argv)){
	HelicsFilter filt = (HelicsFilter)(mxGetData(argv[0]));

	int option = (int)(mxGetScalar(argv[1]));

	int value = (int)(mxGetScalar(argv[2]));

	HelicsError err = helicsErrorInitialize();

	helicsFilterSetOption(filt, option, value, &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}







	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFilterGetOption(resc, resv, argc, (mxArray**)(argv)){
	HelicsFilter filt = (HelicsFilter)(mxGetData(argv[0]));

	int option = (int)(mxGetScalar(argv[1]));

	int result = helicsFilterGetOption(filt, option);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
	*((int64_T*)mxGetData(out)) = (int64_T)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}




}


void _wrap_helicsFederateRegisterTranslator(resc, resv, argc, (mxArray**)(argv)){
	HelicsFederate fed = (HelicsFederate)(mxGetData(argv[0]));

	HelicsTranslatorTypes type = (HelicsTranslatorTypes)(mxGetScalar(argv[1]));

	char *name;
	size_t nameLength;
	int nameStatus;
	nameLength = mxGetN(argv[2]) + 1;
	name = (char *)mxMalloc(nameLength);
	nameStatus = mxGetString(argv[2], name, nameLength);

	HelicsError err = helicsErrorInitialize();

	HelicsTranslator result = helicsFederateRegisterTranslator(fed, type, (char const *)name, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_T*)mxGetData(_out)) = (uint64_T)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}





	mxFree(name);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateRegisterGlobalTranslator(resc, resv, argc, (mxArray**)(argv)){
	HelicsFederate fed = (HelicsFederate)(mxGetData(argv[0]));

	HelicsTranslatorTypes type = (HelicsTranslatorTypes)(mxGetScalar(argv[1]));

	char *name;
	size_t nameLength;
	int nameStatus;
	nameLength = mxGetN(argv[2]) + 1;
	name = (char *)mxMalloc(nameLength);
	nameStatus = mxGetString(argv[2], name, nameLength);

	HelicsError err = helicsErrorInitialize();

	HelicsTranslator result = helicsFederateRegisterGlobalTranslator(fed, type, (char const *)name, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_T*)mxGetData(_out)) = (uint64_T)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}





	mxFree(name);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsCoreRegisterTranslator(resc, resv, argc, (mxArray**)(argv)){
	HelicsCore core = (HelicsCore)(mxGetData(argv[0]));

	HelicsTranslatorTypes type = (HelicsTranslatorTypes)(mxGetScalar(argv[1]));

	char *name;
	size_t nameLength;
	int nameStatus;
	nameLength = mxGetN(argv[2]) + 1;
	name = (char *)mxMalloc(nameLength);
	nameStatus = mxGetString(argv[2], name, nameLength);

	HelicsError err = helicsErrorInitialize();

	HelicsTranslator result = helicsCoreRegisterTranslator(core, type, (char const *)name, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_T*)mxGetData(_out)) = (uint64_T)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}





	mxFree(name);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateGetTranslatorCount(resc, resv, argc, (mxArray**)(argv)){
	HelicsFederate fed = (HelicsFederate)(mxGetData(argv[0]));

	int result = helicsFederateGetTranslatorCount(fed);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
	*((int64_T*)mxGetData(out)) = (int64_T)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsFederateGetTranslator(resc, resv, argc, (mxArray**)(argv)){
	HelicsFederate fed = (HelicsFederate)(mxGetData(argv[0]));

	char *name;
	size_t nameLength;
	int nameStatus;
	nameLength = mxGetN(argv[1]) + 1;
	name = (char *)mxMalloc(nameLength);
	nameStatus = mxGetString(argv[1], name, nameLength);

	HelicsError err = helicsErrorInitialize();

	HelicsTranslator result = helicsFederateGetTranslator(fed, (char const *)name, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_T*)mxGetData(_out)) = (uint64_T)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	mxFree(name);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsFederateGetTranslatorByIndex(resc, resv, argc, (mxArray**)(argv)){
	HelicsFederate fed = (HelicsFederate)(mxGetData(argv[0]));

	int index = (int)(mxGetScalar(argv[1]));

	HelicsError err = helicsErrorInitialize();

	HelicsTranslator result = helicsFederateGetTranslatorByIndex(fed, index, &err);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uint64_T*)mxGetData(_out)) = (uint64_T)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}





	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsTranslatorIsValid(resc, resv, argc, (mxArray**)(argv)){
	HelicsTranslator trans = (HelicsTranslator)(mxGetData(argv[0]));

	HelicsBool result = helicsTranslatorIsValid(trans);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
	*((int64_T*)mxGetData(out)) = (int64_T)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsTranslatorGetName(resc, resv, argc, (mxArray**)(argv)){
	HelicsTranslator trans = (HelicsTranslator)(mxGetData(argv[0]));

	char *result = helicsTranslatorGetName(trans);

	mxArray *_out = mxCreateString((const char *)result);

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsTranslatorSet(resc, resv, argc, (mxArray**)(argv)){
	HelicsTranslator trans = (HelicsTranslator)(mxGetData(argv[0]));

	char *prop;
	size_t propLength;
	int propStatus;
	propLength = mxGetN(argv[1]) + 1;
	prop = (char *)mxMalloc(propLength);
	propStatus = mxGetString(argv[1], prop, propLength);

	double val = mxGetScalar(argv[2]);

	HelicsError err = helicsErrorInitialize();

	helicsTranslatorSet(trans, (char const *)prop, val, &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	mxFree(prop);



	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsTranslatorSetString(resc, resv, argc, (mxArray**)(argv)){
	HelicsTranslator trans = (HelicsTranslator)(mxGetData(argv[0]));

	char *prop;
	size_t propLength;
	int propStatus;
	propLength = mxGetN(argv[1]) + 1;
	prop = (char *)mxMalloc(propLength);
	propStatus = mxGetString(argv[1], prop, propLength);

	char *val;
	size_t valLength;
	int valStatus;
	valLength = mxGetN(argv[2]) + 1;
	val = (char *)mxMalloc(valLength);
	valStatus = mxGetString(argv[2], val, valLength);

	HelicsError err = helicsErrorInitialize();

	helicsTranslatorSetString(trans, (char const *)prop, (char const *)val, &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	mxFree(prop);

	mxFree(val);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsTranslatorAddInputTarget(resc, resv, argc, (mxArray**)(argv)){
	HelicsTranslator trans = (HelicsTranslator)(mxGetData(argv[0]));

	char *input;
	size_t inputLength;
	int inputStatus;
	inputLength = mxGetN(argv[1]) + 1;
	input = (char *)mxMalloc(inputLength);
	inputStatus = mxGetString(argv[1], input, inputLength);

	HelicsError err = helicsErrorInitialize();

	helicsTranslatorAddInputTarget(trans, (char const *)input, &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	mxFree(input);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsTranslatorAddPublicationTarget(resc, resv, argc, (mxArray**)(argv)){
	HelicsTranslator trans = (HelicsTranslator)(mxGetData(argv[0]));

	char *pub;
	size_t pubLength;
	int pubStatus;
	pubLength = mxGetN(argv[1]) + 1;
	pub = (char *)mxMalloc(pubLength);
	pubStatus = mxGetString(argv[1], pub, pubLength);

	HelicsError err = helicsErrorInitialize();

	helicsTranslatorAddPublicationTarget(trans, (char const *)pub, &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	mxFree(pub);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsTranslatorAddSourceEndpoint(resc, resv, argc, (mxArray**)(argv)){
	HelicsTranslator trans = (HelicsTranslator)(mxGetData(argv[0]));

	char *ept;
	size_t eptLength;
	int eptStatus;
	eptLength = mxGetN(argv[1]) + 1;
	ept = (char *)mxMalloc(eptLength);
	eptStatus = mxGetString(argv[1], ept, eptLength);

	HelicsError err = helicsErrorInitialize();

	helicsTranslatorAddSourceEndpoint(trans, (char const *)ept, &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	mxFree(ept);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsTranslatorAddDestinationEndpoint(resc, resv, argc, (mxArray**)(argv)){
	HelicsTranslator trans = (HelicsTranslator)(mxGetData(argv[0]));

	char *ept;
	size_t eptLength;
	int eptStatus;
	eptLength = mxGetN(argv[1]) + 1;
	ept = (char *)mxMalloc(eptLength);
	eptStatus = mxGetString(argv[1], ept, eptLength);

	HelicsError err = helicsErrorInitialize();

	helicsTranslatorAddDestinationEndpoint(trans, (char const *)ept, &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	mxFree(ept);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsTranslatorRemoveTarget(resc, resv, argc, (mxArray**)(argv)){
	HelicsTranslator trans = (HelicsTranslator)(mxGetData(argv[0]));

	char *target;
	size_t targetLength;
	int targetStatus;
	targetLength = mxGetN(argv[1]) + 1;
	target = (char *)mxMalloc(targetLength);
	targetStatus = mxGetString(argv[1], target, targetLength);

	HelicsError err = helicsErrorInitialize();

	helicsTranslatorRemoveTarget(trans, (char const *)target, &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	mxFree(target);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsTranslatorGetInfo(resc, resv, argc, (mxArray**)(argv)){
	HelicsTranslator trans = (HelicsTranslator)(mxGetData(argv[0]));

	char *result = helicsTranslatorGetInfo(trans);

	mxArray *_out = mxCreateString((const char *)result);

	if(_out){
		--resc;
		*resv++ = _out;
	}


}


void _wrap_helicsTranslatorSetInfo(resc, resv, argc, (mxArray**)(argv)){
	HelicsTranslator trans = (HelicsTranslator)(mxGetData(argv[0]));

	char *info;
	size_t infoLength;
	int infoStatus;
	infoLength = mxGetN(argv[1]) + 1;
	info = (char *)mxMalloc(infoLength);
	infoStatus = mxGetString(argv[1], info, infoLength);

	HelicsError err = helicsErrorInitialize();

	helicsTranslatorSetInfo(trans, (char const *)info, &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	mxFree(info);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsTranslatorGetTag(resc, resv, argc, (mxArray**)(argv)){
	HelicsTranslator trans = (HelicsTranslator)(mxGetData(argv[0]));

	char *tagname;
	size_t tagnameLength;
	int tagnameStatus;
	tagnameLength = mxGetN(argv[1]) + 1;
	tagname = (char *)mxMalloc(tagnameLength);
	tagnameStatus = mxGetString(argv[1], tagname, tagnameLength);

	char *result = helicsTranslatorGetTag(trans, (char const *)tagname);

	mxArray *_out = mxCreateString((const char *)result);

	if(_out){
		--resc;
		*resv++ = _out;
	}



	mxFree(tagname);
}


void _wrap_helicsTranslatorSetTag(resc, resv, argc, (mxArray**)(argv)){
	HelicsTranslator trans = (HelicsTranslator)(mxGetData(argv[0]));

	char *tagname;
	size_t tagnameLength;
	int tagnameStatus;
	tagnameLength = mxGetN(argv[1]) + 1;
	tagname = (char *)mxMalloc(tagnameLength);
	tagnameStatus = mxGetString(argv[1], tagname, tagnameLength);

	char *tagvalue;
	size_t tagvalueLength;
	int tagvalueStatus;
	tagvalueLength = mxGetN(argv[2]) + 1;
	tagvalue = (char *)mxMalloc(tagvalueLength);
	tagvalueStatus = mxGetString(argv[2], tagvalue, tagvalueLength);

	HelicsError err = helicsErrorInitialize();

	helicsTranslatorSetTag(trans, (char const *)tagname, (char const *)tagvalue, &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}



	mxFree(tagname);

	mxFree(tagvalue);

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsTranslatorSetOption(resc, resv, argc, (mxArray**)(argv)){
	HelicsTranslator trans = (HelicsTranslator)(mxGetData(argv[0]));

	int option = (int)(mxGetScalar(argv[1]));

	int value = (int)(mxGetScalar(argv[2]));

	HelicsError err = helicsErrorInitialize();

	helicsTranslatorSetOption(trans, option, value, &err);

	mxArray _out = (mxArray *)0;

	if(_out){
		--resc;
		*resv++ = _out;
	}







	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void _wrap_helicsTranslatorGetOption(resc, resv, argc, (mxArray**)(argv)){
	HelicsTranslator trans = (HelicsTranslator)(mxGetData(argv[0]));

	int option = (int)(mxGetScalar(argv[1]));

	int result = helicsTranslatorGetOption(trans, option);

	mxArray *_out = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
	*((int64_T*)mxGetData(out)) = (int64_T)result;

	if(_out){
		--resc;
		*resv++ = _out;
	}




}


void matlabBrokerLoggingCallback(int loglevel, const char* identifier, const char* message, void* userData){
	mxArray *lhs;
	mxArray *rhs[4];	rhs[0] = const_cast<mxArray *>(userData);
	rhs[1] = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
	*((int64_T*)mxGetData(out)) = (int64_T)loglevel;
	rhs[2] = mxCreateString(identifier);
	rhs[3] = mxCreateString(message);
	int status = mexCallMATLAB(0,&lhs,4,rhs,"feval");
	mxDestroyArray(lhs);
	mxDestroyArray(rhs[1]);
	mxDestroyArray(rhs[2]);
	mxDestroyArray(rhs[3]);
}

void _wrap_helicsBrokerSetLoggingCallback(resc, resv, argc, (mxArray**)(argv)){
	HelicsBroker broker = (HelicsBroker)(mxGetData(argv[0]));

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


void matlabCoreLoggingCallback(int loglevel, const char* identifier, const char* message, void* userData){
	mxArray *lhs;
	mxArray *rhs[4];	rhs[0] = const_cast<mxArray *>(userData);
	rhs[1] = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
	*((int64_T*)mxGetData(out)) = (int64_T)loglevel;
	rhs[2] = mxCreateString(identifier);
	rhs[3] = mxCreateString(message);
	int status = mexCallMATLAB(0,&lhs,4,rhs,"feval");
	mxDestroyArray(lhs);
	mxDestroyArray(rhs[1]);
	mxDestroyArray(rhs[2]);
	mxDestroyArray(rhs[3]);
}

void _wrap_helicsCoreSetLoggingCallback(resc, resv, argc, (mxArray**)(argv)){
	HelicsCore core = (HelicsCore)(mxGetData(argv[0]));

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


void matlabFederateLoggingCallback(int loglevel, const char* identifier, const char* message, void* userData){
	mxArray *lhs;
	mxArray *rhs[4];	rhs[0] = const_cast<mxArray *>(userData);
	rhs[1] = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
	*((int64_T*)mxGetData(out)) = (int64_T)loglevel;
	rhs[2] = mxCreateString(identifier);
	rhs[3] = mxCreateString(message);
	int status = mexCallMATLAB(0,&lhs,4,rhs,"feval");
	mxDestroyArray(lhs);
	mxDestroyArray(rhs[1]);
	mxDestroyArray(rhs[2]);
	mxDestroyArray(rhs[3]);
}

void _wrap_helicsFederateSetLoggingCallback(resc, resv, argc, (mxArray**)(argv)){
	HelicsFederate fed = (HelicsFederate)(mxGetData(argv[0]));

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


HelicsMessage matlabFilterCustomCallback(HelicsMessage message, void* userData){
	mxArray *lhs;
	mxArray *rhs[2];	rhs[0] = const_cast<mxArray *>(userData);
	rhs[1] = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((unit64_T*)mxGetData(rhs[1])) = (uint64_T)message;
	int status = mexCallMATLAB(1,&lhs,2,rhs,"feval");
	HelicsMessage rv = (HelicsMessage)(mxGetData(lhs[0]));
	mxDestroyArray(rhs[1]);
	return rv;
}

void _wrap_helicsFilterSetCustomCallback(resc, resv, argc, (mxArray**)(argv)){
	HelicsFilter filter = (HelicsFilter)(mxGetData(argv[0]));

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


HelicsMessage matlabFederateQueryCallback(const char* query, int querySize, HelicsQueryBuffer buffer, void* userData){
	mxArray *lhs;
	mxArray *rhs[2];\mxSize dims[2] = {1, querySize};
	rhs[0] = const_cast<mxArray *>(userData);
	rhs[1] = mxCreateCharArray(2, dims);
	mxChar *pQuery = (mxChar *)mxGetData(rhs[1]);
	for(int i=0; i<querySize; ++i){
		pQuery[i] = query[i];
	}
	int status = mexCallMATLAB(1,&lhs,2,rhs,"feval");
	char *rStr;]n	size_t rStrSize = mxGetN(lhs[0]) + 1;
	rStr = (char *)mxMalloc(rStrSize);
int status = mxGetString(lhs[0], (const char *)rStr, rStrSize);
	HelicsError err = helicsErrorInitialize();

helicsQueryBufferFill(buffer, (const char *)rStr, (int)rStrSize, err);
	mxDestroyArray(rhs[1]);
	mxFree(rStr);
	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}

void _wrap_helicsFederateSetQueryCallback(resc, resv, argc, (mxArray**)(argv)){
	HelicsFilter filter = (HelicsFilter)(mxGetData(argv[0]));

	void *userData = mxGetData(argv[1]);
	HelicsError err = helicsErrorInitialize();

	helicsFederateSetQueryCallback(filter, &matlabFilterCustomCallback, userData, &err);

	mxArray *_out = (mxArray *)0;
	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


void matlabFederateSetTimeRequestEntryCallback(HelicsTime currentTime, HelicsTime requestTime, HelicsBool iterating, void* userData){
	mxArray *lhs;
	mxArray *rhs[4];	rhs[0] = const_cast<mxArray *>(userData);
	rhs[1] = mxCreateDoubleScalar(currentTime);
	rhs[2] = mxCreateDoubleScalar(requestTime);
	rhs[3] = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
	*((int64_T*)mxGetData(rhs[2]) =  (int46_T)iterating;
	int status = mexCallMATLAB(0,&lhs,4,rhs,"feval");
	mxDestroyArray(lhs);
	mxDestroyArray(rhs[1]);
	mxDestroyArray(rhs[2]);
	mxDestroyArray(rhs[3]);
}

void _wrap_helicsFederateSetTimeRequestEntryCallback(resc, resv, argc, (mxArray**)(argv)){
	HelicsFederate fed = (HelicsFederate)(mxGetData(argv[0]));

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


void matlabFederateTimeUpdateCallback(HelicsTime newTime, HelicsBool iterating, void* userData){
	mxArray *lhs;
	mxArray *rhs[3];	rhs[0] = const_cast<mxArray *>(userData);
	rhs[1] = mxCreateDoubleScalar((double)newTime);
	rhs[2] = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
	*((int64_T*)mxGetData(rhs[2]) =  (int46_T)iterating;
	int status = mexCallMATLAB(0,&lhs,3,rhs,"feval");
	mxDestroyArray(lhs);
	mxDestroyArray(rhs[1]);
	mxDestroyArray(rhs[2]);
}

void _wrap_helicsFederateSetTimeUpdateCallback(resc, resv, argc, (mxArray**)(argv)){
	HelicsFederate fed = (HelicsFederate)(mxGetData(argv[0]));

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


void matlabFederateSetStateChangeCallback(HelicsFederateState newState, HelicsFederateState oldState, void* userData){
	mxArray *lhs;
	mxArray *rhs[3];	rhs[0] = const_cast<mxArray *>(userData);
	rhs[1] = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
	*((int64_T*)mxGetData(rhs[2]) =  (int46_T)newState;
	rhs[2] = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
	*((int64_T*)mxGetData(rhs[2]) =  (int46_T)oldState;
	int status = mexCallMATLAB(0,&lhs,3,rhs,"feval");
	mxDestroyArray(lhs);
	mxDestroyArray(rhs[1]);
	mxDestroyArray(rhs[2]);
}

void _wrap_helicsFederateSetStateChangeCallback(resc, resv, argc, (mxArray**)(argv)){
	HelicsFederate fed = (HelicsFederate)(mxGetData(argv[0]));

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


void matlabFederateSetTimeRequestReturnCallback(HelicsTime newTime, HelicsBool iterating, void* userData){
	mxArray *lhs;
	mxArray *rhs[3];	rhs[0] = const_cast<mxArray *>(userData);
	rhs[1] = mxCreateDoubleScalar(newTime);
	rhs[2] = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
	*((int64_T*)mxGetData(rhs[2]) =  (int46_T)iterating;
	int status = mexCallMATLAB(0,&lhs,3,rhs,"feval");
	mxDestroyArray(lhs);
	mxDestroyArray(rhs[1]);
	mxDestroyArray(rhs[2]);
}

void _wrap_helicsFederateSetTimeRequestReturnCallback(resc, resv, argc, (mxArray**)(argv)){
	HelicsFederate fed = (HelicsFederate)(mxGetData(argv[0]));

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


void _wrap_helicsQueryBufferFill(resc, resv, argc, (mxArray**)(argv)){
	HelicsQueryBuffer buffer = (HelicsQueryBuffer)(mxGetData(argv[0]));

	char *str;
	size_t strLength;
	int strStatus;
	strLength = mxGetN(argv[1]) + 1;
	str = (char *)mxMalloc(strLength);
	strStatus = mxGetString(argv[1], str, strLength);

	int strSize = strLength - 1;

	HelicsError err = helicsErrorInitialize();

	helicsQueryBufferFill(buffer, str, strSize, &err);

	mxArray *_out = (mxArray *)0;
	if(_out){
		--resc;
		*resv++ = _out;
	}

	if(err.error_code != HELICS_OK){
		throwHelicsMatlabError(&err);
	}
}


