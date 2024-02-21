'''
Copyright (c) 2017-2022,
Battelle Memorial Institute; Lawrence Livermore National Security, LLC; Alliance for Sustainable Energy, LLC.  See
the top-level NOTICE for additional details. All rights reserved.
SPDX-License-Identifier: BSD-3-Clause
'''
import json
import logging
import os
import re
import shutil
from typing import Any, Dict, List, Tuple

from . import clangParser


matlabBindingGeneratorLogger = logging.getLogger(__name__)
matlabBindingGeneratorLogger.setLevel(logging.DEBUG)
logFormatter = logging.Formatter('%(asctime)s - %(name)s - %(levelname)s - %(message)s')
logFileHandler = logging.FileHandler('matlabBindingGeneratorLog.log',mode='w',encoding='utf-8')
logStreamHandler = logging.StreamHandler()
logFileHandler.setLevel(logging.DEBUG)
logFileHandler.setFormatter(logFormatter)
logStreamHandler.setLevel(logging.INFO)
logStreamHandler.setFormatter(logFormatter)
matlabBindingGeneratorLogger.addHandler(logFileHandler)
matlabBindingGeneratorLogger.addHandler(logStreamHandler)


class MatlabBindingGenerator(object):
    '''
    This class generates all the Matlab HELICS language binding source code.
    
    @param headerFiles: List[str] - the list of the HELICS C API headers
    @method generateSource(void) - this function generates all the Matlab
        HELICS language binding source code files.
    '''


    def __init__(self, rootDir: str, headerFiles: List[str]):
        '''
        Constructor
        '''
        self.__helicsParser = clangParser.HelicsHeaderParser(headerFiles)
        self.__rootDir = os.path.abspath(rootDir)

    def getParser(self):
        return self.__helicsParser
        
    def generateSource(self):
        """
            Function that creates the HELICS MATLAB Bindings
        """
        def createBoilerPlate(headerFiles: List[str], helicsElementMapTuples) -> str:
            boilerPlateStr = "/*\n"
            boilerPlateStr += "Copyright (c) 2017-2022,\n"
            boilerPlateStr += "Battelle Memorial Institute; Lawrence Livermore National Security, LLC; Alliance for Sustainable Energy, LLC.  See\n"
            boilerPlateStr += "the top-level NOTICE for additional details. All rights reserved.\n"
            boilerPlateStr += "SPDX-License-Identifier: BSD-3-Clause\n"
            boilerPlateStr += "*/\n"
            for h in headerFiles:
                boilerPlateStr = f"#include \"helics/helics.h\"\n"
            boilerPlateStr += "#include <mex.h>\n"
            boilerPlateStr += "#include <stdexcept>\n"
            boilerPlateStr += "#include <string>\n"
            boilerPlateStr += "#include <unordered_map>\n\n"
            boilerPlateStr += "static int mexFunctionCalled = 0;\n\n"
            boilerPlateStr += "static void throwHelicsMatlabError(HelicsError *err) {\n"
            boilerPlateStr += "\tmexUnlock();\n"
            boilerPlateStr += "\tswitch (err->error_code)\n"
            boilerPlateStr += "\t{\n"
            boilerPlateStr += "\tcase HELICS_OK:\n"
            boilerPlateStr += "\t\treturn;\n"
            boilerPlateStr += "\tcase HELICS_ERROR_REGISTRATION_FAILURE:\n"
            boilerPlateStr += "\t\tmexErrMsgIdAndTxt(\"helics:registration_failure\", err->message);\n"
            boilerPlateStr += "\t\tbreak;\n"
            boilerPlateStr += "\tcase HELICS_ERROR_CONNECTION_FAILURE:\n"
            boilerPlateStr += "\t\tmexErrMsgIdAndTxt(\"helics:connection_failure\", err->message);\n"
            boilerPlateStr += "\t\tbreak;\n"
            boilerPlateStr += "\tcase HELICS_ERROR_INVALID_OBJECT:\n"
            boilerPlateStr += "\t\tmexErrMsgIdAndTxt(\"helics:invalid_object\", err->message);\n"
            boilerPlateStr += "\t\tbreak;\n"
            boilerPlateStr += "\tcase HELICS_ERROR_INVALID_ARGUMENT:\n"
            boilerPlateStr += "\t\tmexErrMsgIdAndTxt(\"helics:invalid_argument\", err->message);\n"
            boilerPlateStr += "\t\tbreak;\n"
            boilerPlateStr += "\tcase HELICS_ERROR_DISCARD:\n"
            boilerPlateStr += "\t\tmexErrMsgIdAndTxt(\"helics:discard\", err->message);\n"
            boilerPlateStr += "\t\tbreak;\n"
            boilerPlateStr += "\tcase HELICS_ERROR_SYSTEM_FAILURE:\n"
            boilerPlateStr += "\t\tmexErrMsgIdAndTxt(\"helics:system_failure\", err->message);\n"
            boilerPlateStr += "\t\tbreak;\n"
            boilerPlateStr += "\tcase HELICS_ERROR_INVALID_STATE_TRANSITION:\n"
            boilerPlateStr += "\t\tmexErrMsgIdAndTxt(\"helics:invalid_state_transition\", err->message);\n"
            boilerPlateStr += "\t\tbreak;\n"
            boilerPlateStr += "\tcase HELICS_ERROR_INVALID_FUNCTION_CALL:\n"
            boilerPlateStr += "\t\tmexErrMsgIdAndTxt(\"helics:invalid_function_call\", err->message);\n"
            boilerPlateStr += "\t\tbreak;\n"
            boilerPlateStr += "\tcase HELICS_ERROR_EXECUTION_FAILURE:\n"
            boilerPlateStr += "\t\tmexErrMsgIdAndTxt(\"helics:execution_failure\", err->message);\n"
            boilerPlateStr += "\t\tbreak;\n"
            boilerPlateStr += "\tcase HELICS_ERROR_INSUFFICIENT_SPACE:\n"
            boilerPlateStr += "\t\tmexErrMsgIdAndTxt(\"helics:insufficient_space\", err->message);\n"
            boilerPlateStr += "\t\tbreak;\n"
            boilerPlateStr += "\tcase HELICS_ERROR_OTHER:\n"
            boilerPlateStr += "\tcase HELICS_ERROR_EXTERNAL_TYPE:\n"
            boilerPlateStr += "\tdefault:\n"
            boilerPlateStr += "\t\tmexErrMsgIdAndTxt(\"helics:error\", err->message);\n"
            boilerPlateStr += "\t\tbreak;\n"
            boilerPlateStr += "\t}\n"
            boilerPlateStr += "}\n\n"
            boilerPlateStr += "static const std::unordered_map<std::string,int> wrapperFunctionMap{\n"
            for i in range(len(helicsElementMapTuples)):
                if i == 0:
                    boilerPlateStr += f"\t{{\"{helicsElementMapTuples[i][0]}\",{helicsElementMapTuples[i][1]}}}"
                else:
                    boilerPlateStr += f",\n\t{{\"{helicsElementMapTuples[i][0]}\",{helicsElementMapTuples[i][1]}}}"
            boilerPlateStr += "\n};\n\n"
            return boilerPlateStr
        
        
        def createEnum(enumDict: Dict[str,str]) -> None:
            """
                Create Matlab Binding Enumerations for each C Enumeration
            """
            matlabBindingGeneratorLogger.debug(f"creating MATLAB enum definition for:\n{json.dumps(enumDict,indent=4,sort_keys=True)}")
            enumSpelling = enumDict.get('spelling','')
            enumComment = enumDict.get('brief_comment','')
            with open(os.path.join(self.__rootDir, f"matlabBindings/+helics/{enumSpelling}.m"),"w") as enumMFile:
                enumMFile.write(f"% {enumComment}\n%\n% Attributes:")
                docStrBody = ""
                enumStrBody = ""
                for enumKey in enumDict.get('enumerations',{}).keys():
                    keywordSpelling = enumDict.get('enumerations',{}).get(enumKey,{}).get('spelling','')
                    keywordValue = enumDict.get('enumerations',{}).get(enumKey,{}).get('value')
                    keywordComment = enumDict.get('enumerations',{}).get(enumKey,{}).get('brief_comment','')
                    if keywordComment == None:
                        keywordComment = ''
                    docStrBody += f"\n%\t{keywordSpelling}: value:{keywordValue}\t{keywordComment}"
                    enumStrBody += f"\n\t\t{keywordSpelling} = int32({keywordValue});"
                enumMFile.write(docStrBody)
                enumMFile.write(f"\nclassdef {enumSpelling}\n\tproperties (Constant)")
                enumMFile.write(enumStrBody)
                enumMFile.write("\n\tend\nend")
                
        
        
        def createMacro(macroDict: Dict[str,str], cursorIdx: int):
            matlabBindingGeneratorLogger.debug(f"creating MATLAB macro definition for:\n{json.dumps(macroDict,indent=4,sort_keys=True)}")
            macroSpelling = macroDict.get("spelling","")
            macroComment = macroDict.get("brief_comment","")
            macroValue = macroDict.get("value")
            macroWrapperStr = ""
            macroMainFunctionElementStr = ""
            macroMapTuple = None
            if isinstance(macroValue, str):
                with open(os.path.join(self.__rootDir, f"matlabBindings/+helics/{macroSpelling}.m"), "w") as macroFile:
                    macroFile.write(f"function v = {macroSpelling}()\n")
                    if macroComment != None:
                        macroFile.write(f"% {macroComment}\n")
                    macroFile.write(f"\tv = helicsMex('{macroSpelling}');\n")
                    macroFile.write("end\n")
                macroWrapperStr += f"void _wrap_{macroSpelling}(int resc, mxArray *resv[], int argc, const mxArray *argv[])" + "{\n"
                macroWrapperStr += "\tif(argc != 0){\n"
                macroWrapperStr += "\t\tmexUnlock();\n"
                macroWrapperStr += f"\t\tmexErrMsgIdAndTxt(\"MATLAB:{macroSpelling}:rhs\",\"This function doesn't take arguments.\");\n"
                macroWrapperStr += "\t}\n"
                macroWrapperStr += "\tmxArray *_out = mxCreateNumericMatrix(1,1,mxINT64_CLASS,mxREAL);\n"
                macroWrapperStr += f"\t*((int64_t*)mxGetData(_out)) = (int64_t){macroSpelling};\n"
                macroWrapperStr += "\tresv[0] = _out;\n"
                macroWrapperStr += "}\n\n"
                macroMainFunctionElementStr += f"\tcase {cursorIdx}:\n"
                macroMainFunctionElementStr += f"\t\t_wrap_{macroSpelling}(resc, resv, argc, argv);\n"
                macroMainFunctionElementStr += f"\t\tbreak;\n"
                macroMapTuple = (macroSpelling, cursorIdx)
            elif isinstance(macroValue, float) or isinstance(macroValue, int):
                with open(os.path.join(self.__rootDir, f"matlabBindings/+helics/{macroSpelling}.m"), "w") as macroFile:
                    macroFile.write(f"function v = {macroSpelling}()\n")
                    if macroComment != None:
                        macroFile.write(f"% {macroComment}\n")
                    macroFile.write(f"\tv = {macroValue};\n")
                    macroFile.write("end\n")
            return macroWrapperStr, macroMainFunctionElementStr, macroMapTuple
        
        
        def createVar(varDict: Dict[str,str], cursorIdx: int):
            varsToIgnore = ["cHelicsBigNumber"]
            varSpelling = varDict.get("spelling","")
            varComment = varDict.get("brief_comment","")
            varType = varDict.get("type","")
            varValue = varDict.get("value")
            if varSpelling not in varsToIgnore:
                matlabBindingGeneratorLogger.debug(f"creating MATLAB var definition for:\n{json.dumps(varDict,indent=4,sort_keys=True)}")
                if isinstance(varValue, str):
                    with open(os.path.join(self.__rootDir, f"matlabBindings/+helics/{varSpelling}.m"), "w") as varFile:
                        varFile.write(f"function v = {varSpelling}()\n")
                        if varComment != None:
                            varFile.write(f"% {varComment}\n")
                        varFile.write(f"\tv = {varValue}();\n")
                        varFile.write("end\n")
                elif isinstance(varValue, float):
                    with open(os.path.join(self.__rootDir, f"matlabBindings/+helics/{varSpelling}.m"), "w") as varFile:
                        varFile.write(f"function v = {varSpelling}()\n")
                        if varComment != None:
                            varFile.write(f"% {varComment}\n")
                        varFile.write(f"\tv = {varValue};\n")
                        varFile.write("end\n")
                elif isinstance(varValue, int):
                    with open(os.path.join(self.__rootDir, f"matlabBindings/+helics/{varSpelling}.m"), "w") as varFile:
                        varFile.write(f"function v = {varSpelling}()\n")
                        if varComment != None:
                            varFile.write(f"% {varComment}\n")
                        if varType != "HelicsBool":
                            varFile.write(f"\tv = int32({varValue});\n")
                        else:
                            if varSpelling == "HELICS_TRUE":
                                varFile.write(f"\tv = true;\n")
                            elif varSpelling == "HELICS_FALSE":
                                varFile.write(f"\tv = false;\n")
                        varFile.write("end\n")
        
        
        def createFunction(functionDict: Dict[str,str], cursorIdx: int):
            matlabBindingGeneratorLogger.debug(f"creating MATLAB function definition for:\n{json.dumps(functionDict,indent=4,sort_keys=True)}")
            modifiedMatlabFunctionList = [
                "helicsCreateBrokerFromArgs",
                "helicsCreateCoreFromArgs",
                "helicsFederateInfoLoadFromArgs",
                "helicsEndpointSendBytes",
                "helicsEndpointSendBytesAt",
                "helicsEndpointSendBytesTo",
                "helicsEndpointSendBytesToAt",
                "helicsFederateRequestTimeIterative",
                "helicsFederateRequestTimeIterativeComplete",
                "helicsInputGetBytes",
                "helicsInputGetComplex",
                "helicsInputGetComplexObject",
                "helicsInputGetNamedPoint",
                "helicsInputGetString",
                "helicsInputGetVector",
                "helicsInputGetComplexVector",
                "helicsInputSetDefaultBytes",
                "helicsInputSetDefaultComplex",
                "helicsInputSetDefaultVector",
                "helicsInputSetDefaultComplexVector",
                "helicsMessageAppendData",
                "helicsMessageGetBytes",
                "helicsMessageSetData",
                "helicsPublicationPublishBytes",
                "helicsPublicationPublishComplex",
                "helicsPublicationPublishVector",
                "helicsPublicationPublishComplexVector",
                "helicsQueryBufferFill",
                "helicsLoadSignalHandlerCallback",
                "helicsLoadSignalHandlerCallbackNoExit",
                "helicsBrokerSetLoggingCallback",
                "helicsCoreSetLoggingCallback",
                "helicsFederateSetLoggingCallback",
                "helicsFilterSetCustomCallback",
                "helicsFederateSetQueryCallback",
                "helicsFederateSetTimeUpdateCallback",
                "helicsFederateSetStateChangeCallback",
                "helicsFederateSetTimeRequestEntryCallback",
                "helicsFederateSetTimeRequestReturnCallback",
                "helicsTranslatorSetCustomCallback",
                "helicsDataBufferFillFromComplex",
                "helicsDataBufferFillFromVector",
                "helicsDataBufferFillFromComplexVector",
                "helicsDataBufferToString",
                "helicsDataBufferToRawString",
                "helicsDataBufferToComplex",
                "helicsDataBufferToComplexObject",
                "helicsDataBufferToVector",
                "helicsDataBufferToComplexVector",
                "helicsDataBufferToNamedPoint",
                "helicsCloseLibrary",
                "helicsFederateInitializingEntryCallback",
                "helicsFederateExecutingEntryCallback",
                "helicsFederateCosimulationTerminationCallback",
                "helicsFederateErrorHandlerCallback",
                "helicsCallbackFederateNextTimeCallback",
                "helicsCallbackFederateNextTimeIterativeCallback",
                "helicsCallbackFederateInitializeCallback"
            ]
            functionsToIgnore = ["helicsErrorInitialize", "helicsErrorClear", "helicsDataBufferFillFromComplexObject"]
            functionName = functionDict.get("spelling")
            if functionName in modifiedMatlabFunctionList:
                return createModifiedMatlabFunction(functionDict, cursorIdx)
            if functionName not in functionsToIgnore:
                functionComment = functionDict.get("raw_comment")
                matlabBindingGeneratorLogger.debug(f"the raw_comment before converting to a matlab help text:\n{functionComment}")
                functionComment = re.sub("^/\*\*\n","",functionComment)
                functionComment = re.sub("^/\*\*(?=.)","%",functionComment)
                functionComment = re.sub("\n.*\*(?=[^/])","\n%",functionComment)
                functionComment = re.sub(" *\*(?=[^/])","%",functionComment)
                functionComment = re.sub("\n* *\*/","",functionComment)
                functionComment = re.sub("\n(?=[^%])","\n%",functionComment)
                functionComment = re.sub("\n.*@param\[in,out\] err.*\n","\n",functionComment)
                functionComment = re.sub("%(?=[^ ])", "% ",functionComment)
                functionComment += "\n\n"
                matlabBindingGeneratorLogger.debug(f"the raw_comment after converting to a matlab help text:\n{functionComment}")
                with open(os.path.join(self.__rootDir, f"matlabBindings/+helics/{functionName}.m"), "w") as functionMFile:
                    functionMFile.write(f"function varargout = {functionName}(varargin)\n")
                    functionMFile.write(functionComment)
                    functionMFile.write("\t[varargout{1:nargout}] = helicsMex(" + f"'{functionName}', varargin" + "{:});\n")
                    functionMFile.write("end\n")
                functionWrapperStr = ""
                functionMainElements = ""
                functionMainElements += f"\tcase {cursorIdx}:\n"
                functionMainElements += f"\t\t_wrap_{functionName}(resc, resv, argc, argv);\n"
                functionMainElements += f"\t\tbreak;\n"
                functionWrapperStr += f"void _wrap_{functionName}(int resc, mxArray *resv[], int argc, const mxArray *argv[])" + "{\n"
                lastArgPos, isLastArgOptional = isLastArgumentOptional(functionDict)
                functionWrapperStr += getNumberOfArgumentsCheckStr(functionDict,isLastArgOptional)
                for a in functionDict.get("arguments",{}).keys():
                    if a == lastArgPos:
                        functionWrapperStr += getArgInitializationStr(functionDict.get("arguments",{}).get(a,{}), int(a), functionName, isLastArgOptional)
                    else:
                        functionWrapperStr += getArgInitializationStr(functionDict.get("arguments",{}).get(a,{}), int(a), functionName)
                if functionDict.get("result_type","") != "Void":
                    functionWrapperStr += f"{getFunctionReturnInitializationStr(functionDict)} = {functionName}("
                    for a in functionDict.get("arguments",{}).keys():
                        functionWrapperStr += getArgFunctionCallStr(functionDict.get("arguments",{}).get(a,{}), int(a))
                    functionWrapperStr += ");\n\n"
                    functionWrapperStr += getFunctionReturnConversionStr(functionDict)
                    functionWrapperStr += "\n\n\tif(_out){\n"
                    functionWrapperStr += "\t\t--resc;\n"
                    functionWrapperStr += "\t\t*resv++ = _out;\n"
                    functionWrapperStr += "\t}"
                    for a in functionDict.get("arguments",{}).keys():
                        argCleanupStr = getArgFunctionCleanUpStr(functionDict.get("arguments",{}).get(a,{}))
                        if len(argCleanupStr) > 0:
                            functionWrapperStr += "\n\n"
                            functionWrapperStr += argCleanupStr
                    functionWrapperStr += "\n}\n\n\n"
                else:
                    functionWrapperStr += f"\t{functionName}("
                    for a in functionDict.get("arguments",{}).keys():
                        functionWrapperStr += getArgFunctionCallStr(functionDict.get("arguments",{}).get(a,{}), int(a))
                    functionWrapperStr += ");\n\n"
                    functionWrapperStr += "\tmxArray *_out = nullptr;\n\n"
                    functionWrapperStr += "\tif(_out){\n"
                    functionWrapperStr += "\t\t--resc;\n"
                    functionWrapperStr += "\t\t*resv++ = _out;\n"
                    functionWrapperStr += "\t}"
                    for a in functionDict.get("arguments",{}).keys():
                        argCleanupStr = getArgFunctionCleanUpStr(functionDict.get("arguments",{}).get(a,{}))
                        if len(argCleanupStr) > 0:
                            functionWrapperStr += "\n\n"
                            functionWrapperStr += argCleanupStr
                    functionWrapperStr += "\n}\n\n\n"                        
                return functionWrapperStr, functionMainElements, (functionName, cursorIdx)
            else:
                return "","", None


        def isLastArgumentOptional(functionDict: Dict[str,str]) -> Tuple[Any]:
            lastArgPos = 0
            isLastArgOptional = False
            argCount = functionDict.get("argument_count", 0)
            if argCount > 0:
                #check to see if final argument is a const char *. If so this will be treated as an optional argument.
                lastArgPos = argCount - 1
                lastArg = functionDict.get("arguments",{}).get(lastArgPos)
                lastArgType = lastArg.get("pointer_type")
                if lastArgType == "Char_S_*":
                    isLastArgOptional = True
            return lastArgPos, isLastArgOptional
        
        
        def getNumberOfArgumentsCheckStr(functionDict: Dict[str,str], lastArgOptional=False) -> str:
            argCheckStr = ""
            argCount = functionDict.get("argument_count", 0)
            for a in functionDict.get("arguments",{}).keys():
                if functionDict.get("arguments",{}).get(a,{}).get("pointer_type","") == "HelicsError_*":
                    argCount -= 1
            functionName = functionDict.get("spelling","")
            if lastArgOptional:
                argCheckStr += f"\tif(argc < {argCount - 1} || argc > {argCount}){{\n"
                argCheckStr += "\t\tmexUnlock();\n"
                argCheckStr += f"\t\tmexErrMsgIdAndTxt(\"MATLAB:{functionName}:rhs\",\"This function requires at least {argCount - 1} arguments and at most {argCount} arguments.\");\n"
                argCheckStr += "\t}\n\n"
            else:
                argCheckStr += f"\tif(argc != {argCount}){{\n"
                argCheckStr += "\t\tmexUnlock();\n"
                argCheckStr += f"\t\tmexErrMsgIdAndTxt(\"MATLAB:{functionName}:rhs\",\"This function requires {argCount} arguments.\");\n"
                argCheckStr += "\t}\n\n"
            return argCheckStr
        
        
        def getArgInitializationStr(argDict: Dict[str,str], argPos: int, functionName: str, isArgOptional: bool = False) -> str:
            argInitTextMap = {
                "Char_S": initializeArgChar(argDict.get("spelling",""), argPos, functionName),
                "Char_S_*": initializeArgChar(argDict.get("spelling",""), argPos, functionName, isArgOptional),
                "Double": initializeArgDouble(argDict.get("spelling",""), argPos, functionName),
                "Int": initializeArgInt(argDict.get("spelling",""), argPos, functionName),
                "Void_*": initializeArgVoidPtr(argDict.get("spelling",""), argPos, functionName),
                "HelicsBool": initializeArgHelicsBool(argDict.get("spelling",""), argPos, functionName),
                "HelicsBroker": initializeArgHelicsClass("HelicsBroker", argDict.get("spelling",""), argPos, functionName),
                "HelicsCore": initializeArgHelicsClass("HelicsCore", argDict.get("spelling",""), argPos, functionName),
                "HelicsDataBuffer": initializeArgHelicsClass("HelicsDataBuffer", argDict.get("spelling",""), argPos, functionName),
                "HelicsDataTypes": initializeArgHelicsEnum("HelicsDataTypes", argDict.get("spelling",""), argPos, functionName),
                "HelicsEndpoint": initializeArgHelicsClass("HelicsEndpoint", argDict.get("spelling",""), argPos, functionName),
                "HelicsError_*": initializeArgHelicsErrorPtr(argDict.get("spelling","")),
                "HelicsFederate": initializeArgHelicsClass("HelicsFederate", argDict.get("spelling",""), argPos, functionName),
                "HelicsFederateInfo": initializeArgHelicsClass("HelicsFederateInfo", argDict.get("spelling",""), argPos, functionName),
                "HelicsFilter": initializeArgHelicsClass("HelicsFilter", argDict.get("spelling",""), argPos, functionName),
                "HelicsFilterTypes": initializeArgHelicsEnum("HelicsFilterTypes", argDict.get("spelling",""), argPos, functionName),
                "HelicsInput": initializeArgHelicsClass("HelicsInput", argDict.get("spelling",""), argPos, functionName),
                "HelicsIterationRequest": initializeArgHelicsEnum("HelicsIterationRequest", argDict.get("spelling",""), argPos, functionName),
                "HelicsIterationResult_*": initializeArgHelicsIterationResultPtr(argDict.get("spelling","")), 
                "HelicsMessage": initializeArgHelicsClass("HelicsMessage", argDict.get("spelling",""), argPos, functionName),
                "HelicsPublication": initializeArgHelicsClass("HelicsPublication", argDict.get("spelling",""), argPos, functionName),
                "HelicsQuery": initializeArgHelicsClass("HelicsQuery", argDict.get("spelling",""), argPos, functionName),
                "HelicsQueryBuffer": initializeArgHelicsClass("HelicsQueryBuffer", argDict.get("spelling",""), argPos, functionName),
                "HelicsTime": initializeArgHelicsTime(argDict.get("spelling",""), argPos, functionName),
                "HelicsTranslator": initializeArgHelicsClass("HelicsTranslator", argDict.get("spelling",""), argPos, functionName),
                "HelicsTranslatorTypes": initializeArgHelicsEnum("HelicsTranslatorTypes", argDict.get("spelling",""), argPos, functionName),
                "int32_t": initializeArgInt32_t(argDict.get("spelling",""), argPos, functionName),
                "int64_t": initializeArgInt64_t(argDict.get("spelling",""), argPos, functionName)
            }
            argType = argDict.get("double_pointer_type")
            if argType == None:
                argType = argDict.get("pointer_type")
                if argType == None:
                    argType = argDict.get("type")
            argInitText = argInitTextMap.get(argType)
            if argInitText == None:
                raise RuntimeError(f"Unhandled c argument type conversion for {argType}. Please update the argInitTextMap")
            return argInitText
        
        
        def getFunctionReturnInitializationStr(functionDict: Dict[str,str]) -> str:
            returnTextMap = {
                "Char_S": "\tchar result",
                "Char_S_*": "\tconst char *result",
                "Double": "\tdouble result",
                "Int": "\tint result",
                "Void_*": "\tvoid *result",
                "HelicsBool": "\tHelicsBool result",
                "HelicsBroker": "\tHelicsBroker result",
                "HelicsCore": "\tHelicsCore result",
                "HelicsDataBuffer": "\tHelicsDataBuffer result",
                "HelicsEndpoint": "\tHelicsEndpoint result",
                "HelicsFederate": "\tHelicsFederate result",
                "HelicsFederateInfo": "\tHelicsFederateInfo result",
                "HelicsFederateState": "\tHelicsFederateState result",
                "HelicsFilter": "\tHelicsFilter result",
                "HelicsInput": "\tHelicsInput result",
                "HelicsIterationResult": "\tHelicsIterationResult result",
                "HelicsMessage": "\tHelicsMessage result",
                "HelicsPublication": "\tHelicsPublication result",
                "HelicsQuery": "\tHelicsQuery result",
                "HelicsTime": "\tHelicsTime result",
                "HelicsTranslator": "\tHelicsTranslator result",
                "int32_t": "\tint32_t result",
                "int64_t": "\tint64_t result",
            }
            retStr = ""           
            returnType = functionDict.get('double_pointer_type')
            if returnType == None:
                returnType = functionDict.get('pointer_type')
                if returnType== None:
                    returnType = functionDict.get('result_type')
            retStr = returnTextMap.get(returnType)
            if retStr == None:
                raise RuntimeError(f"Unhandled c return type conversion for {returnType}. Please update the returnTextMap. functionDict: {json.dumps(functionDict, indent=4, sort_keys=True)}")
            return retStr
        
        
        def getArgFunctionCallStr(argDict: Dict[str,str], position: int) -> str:
            argFunctionCallTextMap = {
                "Char_S": argCharFunctionCall(argDict.get("spelling",""), position),
                "Char_S_*": argCharPtrFunctionCall(argDict.get("spelling",""), position),
                "Double": argDefaultFunctionCall(argDict.get("spelling",""), position),
                "Int": argDefaultFunctionCall(argDict.get("spelling",""), position),
                "Void_*": argDefaultFunctionCall(argDict.get("spelling",""), position),
                "HelicsBool": argDefaultFunctionCall(argDict.get("spelling",""), position),
                "HelicsBroker": argDefaultFunctionCall(argDict.get("spelling",""), position),
                "HelicsCore": argDefaultFunctionCall(argDict.get("spelling",""), position),
                "HelicsDataBuffer": argDefaultFunctionCall(argDict.get("spelling",""), position),
                "HelicsDataTypes": argDefaultFunctionCall(argDict.get("spelling",""), position),
                "HelicsEndpoint": argDefaultFunctionCall(argDict.get("spelling",""), position),
                "HelicsError_*": argHelicsErrorPtrFunctionCall(argDict.get("spelling",""), position),
                "HelicsFederate": argDefaultFunctionCall(argDict.get("spelling",""), position),
                "HelicsFederateInfo": argDefaultFunctionCall(argDict.get("spelling",""), position),
                "HelicsFilter": argDefaultFunctionCall(argDict.get("spelling",""), position),
                "HelicsFilterTypes": argDefaultFunctionCall(argDict.get("spelling",""), position),
                "HelicsInput": argDefaultFunctionCall(argDict.get("spelling",""), position),
                "HelicsIterationRequest": argDefaultFunctionCall(argDict.get("spelling",""), position),
                "HelicsIterationResult_*": argHelicsErrorPtrFunctionCall(argDict.get("spelling",""), position), 
                "HelicsMessage": argDefaultFunctionCall(argDict.get("spelling",""), position),
                "HelicsPublication": argDefaultFunctionCall(argDict.get("spelling",""), position),
                "HelicsQuery": argDefaultFunctionCall(argDict.get("spelling",""), position),
                "HelicsQueryBuffer": argDefaultFunctionCall(argDict.get("spelling",""), position),
                "HelicsTime": argDefaultFunctionCall(argDict.get("spelling",""), position),
                "HelicsTranslator": argDefaultFunctionCall(argDict.get("spelling",""), position),
                "HelicsTranslatorTypes": argDefaultFunctionCall(argDict.get("spelling",""), position),
                "int32_t": argDefaultFunctionCall(argDict.get("spelling",""), position),
                "int64_t": argDefaultFunctionCall(argDict.get("spelling",""), position)
            }
            argType = argDict.get("double_pointer_type")
            if argType == None:
                argType = argDict.get("pointer_type")
                if argType == None:
                    argType = argDict.get("type")
            argFunctionCallText = argFunctionCallTextMap.get(argType)
            if argFunctionCallText == None:
                raise RuntimeError(f"Unhandled c argument type conversion for {argType}. Please update the argFunctionCallTextMap")
            return argFunctionCallText
        
        
        def getFunctionReturnConversionStr(functionDict: Dict[str,str]) -> str:
            returnTextMap = {
                "Char_S": returnCharTomxArray(),
                "Char_S_*": returnCharPtrTomxArray(),
                "Double": returnDoubleTomxArray(),
                "Int": returnIntTomxArray(),
                "Void_*": returnVoidPtrTomxArray(),
                "HelicsBool": returnHelicsBoolTomxArray(),
                "HelicsBroker": returnVoidPtrTomxArray(),
                "HelicsCore": returnVoidPtrTomxArray(),
                "HelicsDataBuffer": returnVoidPtrTomxArray(),
                "HelicsEndpoint": returnVoidPtrTomxArray(),
                "HelicsFederate": returnVoidPtrTomxArray(),
                "HelicsFederateInfo": returnVoidPtrTomxArray(),
                "HelicsFederateState": returnEnumTomxArray(),
                "HelicsFilter": returnVoidPtrTomxArray(),
                "HelicsInput": returnVoidPtrTomxArray(),
                "HelicsIterationResult": returnEnumTomxArray(),
                "HelicsMessage": returnVoidPtrTomxArray(),
                "HelicsPublication": returnVoidPtrTomxArray(),
                "HelicsQuery": returnVoidPtrTomxArray(),
                "HelicsTime": returnDoubleTomxArray(),
                "HelicsTranslator": returnVoidPtrTomxArray(),
                "int32_t": returnEnumTomxArray(),
                "int64_t": returnIntTomxArray(),
            }
            retStr = ""           
            returnType = functionDict.get('double_pointer_type')
            if returnType == None:
                returnType = functionDict.get('pointer_type')
                if returnType== None:
                    returnType = functionDict.get('result_type')
            retStr = returnTextMap.get(returnType)
            if retStr == None:
                raise RuntimeError(f"Unhandled c return type conversion for {returnType}. Please update the returnTextMap. functionDict: {json.dumps(functionDict, indent=4, sort_keys=True)}")
            return retStr
        
        
        def getArgFunctionCleanUpStr(argDict: Dict[str,str]) -> str:
            argCleanUpMap = {
                "Char_S": argCharPostFunctionCall(argDict.get("spelling","")),
                "Char_S_*": argCharPostFunctionCall(argDict.get("spelling","")),
                "Double": "",
                "Int": "",
                "Void_*": "",
                "HelicsBool": "",
                "HelicsBroker": "",
                "HelicsCore": "",
                "HelicsDataBuffer": "",
                "HelicsDataTypes": "",
                "HelicsEndpoint": "",
                "HelicsError_*": argHelicsErrorPtrPostFunctionCall(argDict.get("spelling","")),
                "HelicsFederate": "",
                "HelicsFederateInfo": "",
                "HelicsFilter": "",
                "HelicsFilterTypes": "",
                "HelicsInput": "",
                "HelicsIterationRequest": "",
                "HelicsIterationResult_*": HelicsIterationResultPtrPostFunctionCall(argDict.get("spelling","")), 
                "HelicsMessage": "",
                "HelicsPublication": "",
                "HelicsQuery": "",
                "HelicsQueryBuffer": "",
                "HelicsTime": "",
                "HelicsTranslator": "",
                "HelicsTranslatorTypes": "",
                "int32_t": "",
                "int64_t": ""
            }
            argType = argDict.get("double_pointer_type")
            if argType == None:
                argType = argDict.get("pointer_type")
                if argType == None:
                    argType = argDict.get("type")
            argCleanUpText = argCleanUpMap.get(argType)
            if argCleanUpText == None:
                raise RuntimeError(f"Unhandled c argument type conversion for {argType}. Please update the argCleanupMap")
            return argCleanUpText 
        
        
        def initializeArgChar(argName: str, position: int, functionName: str, argIsOptional: bool = False) -> str:
            retStr = ""
            if not argIsOptional:
                retStr += f"\tif(!mxIsChar(argv[{position}])){{\n"
                retStr += "\t\tmexUnlock();\n"
                retStr += f"\t\tmexErrMsgIdAndTxt(\"MATLAB:{functionName}:TypeError\",\"Argument {position+1} must be a string.\");\n"
                retStr += "\t}\n"
            else:
                retStr += f"\tif(argc > {position}){{\n"
                retStr += f"\t\tif(!mxIsChar(argv[{position}])){{\n"
                retStr += "\t\t\tmexUnlock();\n"
                retStr += f"\t\t\tmexErrMsgIdAndTxt(\"MATLAB:{functionName}:TypeError\",\"Argument {position+1} must be a string.\");\n"
                retStr += "\t\t}\n"
                retStr += "\t}\n"
            retStr += f"\tchar *{argName} = nullptr;\n"
            retStr += f"\tsize_t {argName}Length = 0;\n"
            retStr += f"\tint {argName}Status = 0;\n"
            retStr += f"\tif(argc > {position}){{\n"
            retStr += f"\t\t{argName}Length = mxGetN(argv[{position}]) + 1;\n"
            retStr += f"\t\t{argName} = static_cast<char *>(malloc({argName}Length));\n"
            retStr += f"\t\t{argName}Status = mxGetString(argv[{position}], {argName}, {argName}Length);\n\n"
            retStr += "\t}\n"
            return retStr
        
        
        def initializeArgDouble(argName: str, position: int, functionName: str) -> str:
            retStr = f"\tif(!mxIsNumeric(argv[{position}])){{\n"
            retStr += "\t\tmexUnlock();\n"
            retStr += f"\t\tmexErrMsgIdAndTxt(\"MATLAB:{functionName}:TypeError\",\"Argument {position+1} must be of type double.\");\n"
            retStr += "\t}\n"
            retStr += f"\tdouble {argName} = mxGetScalar(argv[{position}]);\n\n"
            return retStr
        
        
        def initializeArgInt(argName: str, position: int, functionName: str) -> str:
            retStr = f"\tif(!mxIsNumeric(argv[{position}])){{\n"
            retStr += "\t\tmexUnlock();\n"
            retStr += f"\t\tmexErrMsgIdAndTxt(\"MATLAB:{functionName}:TypeError\",\"Argument {position+1} must be of type integer.\");\n"
            retStr += "\t}\n"
            retStr += f"\t\tint {argName} = static_cast<int>(mxGetScalar(argv[{position}]));\n\n"
            
            return retStr
        
        
        def initializeArgVoidPtr(argName: str, position: int, functionName: str) -> str:
            retStr = f"\tif(mxGetClassID(argv[{position}]) != mxUINT64_CLASS){{\n"
            retStr += "\t\tmexUnlock();\n"
            retStr += f"\t\tmexErrMsgIdAndTxt(\"MATLAB:{functionName}:TypeError\",\"Argument {position+1} must be of type uint64.\");\n"
            retStr += "\t}\n"
            retStr += f"\t\tvoid *{argName} = mxGetData(argv[{position}]);\n\n"
            return retStr
        
        
        def initializeArgHelicsBool(argName: str, position: int, functionName: str) -> str:
            retStr = f"\tmxLogical *p{argName} = nullptr;\n"
            retStr += f"\tif(mxIsLogical(argv[{position}])){{\n"
            retStr += f"\t\tp{argName} = mxGetLogicals(argv[{position}]);\n"
            retStr += f"\t}} else if(mxIsNumeric(argv[{position}])){{\n"
            retStr += f"\t\tif(mxGetScalar(argv[{position}]) == 0.0){{\n"
            retStr += f"\t\t\tmxArray *logical{argName} = mxCreateLogicalScalar(false);\n"
            retStr += f"\t\t\tp{argName} = mxGetLogicals(logical{argName});\n"
            retStr += f"\t\t}} else if(mxGetScalar(argv[{position}]) == 1.0){{\n"
            retStr += f"\t\t\tmxArray *logical{argName} = mxCreateLogicalScalar(true);\n"
            retStr += f"\t\t\tp{argName} = mxGetLogicals(logical{argName});\n"
            retStr += "\t\t} else {\n"
            retStr += "\t\t\tmexUnlock();\n"
            retStr += f"\t\t\tmexErrMsgIdAndTxt(\"MATLAB:{functionName}:TypeError\",\"Argument {position+1} must be a logical type or a 0 or 1.\");\n"
            retStr += "\t\t}\n"
            retStr += "\t} else {\n"
            retStr += "\t\tmexUnlock();\n"
            retStr += f"\t\tmexErrMsgIdAndTxt(\"MATLAB:{functionName}:TypeError\",\"Argument {position+1} must be a logical type or a 0 or 1.\");\n"
            retStr += "\t}\n"
            retStr += f"\tHelicsBool {argName} = HELICS_FALSE;\n"
            retStr += f"\tif(p{argName}[0]){{\n"
            retStr += f"\t\t{argName} = HELICS_TRUE;\n"
            retStr += "\t}\n\n"
            return retStr
        
        
        def initializeArgHelicsClass(helicsClass: str, argName: str, position: int, functionName: str) -> str:
            retStr = f"\tif(mxGetClassID(argv[{position}]) != mxUINT64_CLASS){{\n"
            retStr += "\t\tmexUnlock();\n"
            retStr += f"\t\tmexErrMsgIdAndTxt(\"MATLAB:{functionName}:TypeError\",\"Argument {position+1} must be of type uint64.\");\n"
            retStr += "\t}\n"
            retStr += f"\t{helicsClass} {argName} = *(static_cast<{helicsClass}*>(mxGetData(argv[{position}])));\n\n"
            return retStr
        
        
        def initializeArgHelicsEnum(helicsEnum: str, argName: str, position: int, functionName: str) -> str:
            retStr = f"\tif(!mxIsNumeric(argv[{position}])){{\n"
            retStr += "\t\tmexUnlock();\n"
            retStr += f"\t\tmexErrMsgIdAndTxt(\"MATLAB:{functionName}:TypeError\",\"Argument {position+1} must be of type int32.\");\n"
            retStr += "\t}\n"
            retStr += f"\tint {argName}Int = static_cast<int>(mxGetScalar(argv[{position}]));\n"
            retStr += f"\t{helicsEnum} {argName} = static_cast<{helicsEnum}>({argName}Int);\n\n"
            return retStr
        
        
        def initializeArgHelicsIterationResultPtr(argName: str) -> str:
            retStr = f"\tHelicsIterationResult {argName} = HELICS_ITERATION_RESULT_ERROR;\n\n"
            return retStr
        
        
        def initializeArgHelicsTime(argName: str, position: int, functionName: str) -> str:
            retStr = f"\tif(!mxIsNumeric(argv[{position}])){{\n"
            retStr += "\t\tmexUnlock();\n"
            retStr += f"\t\tmexErrMsgIdAndTxt(\"MATLAB:{functionName}:TypeError\",\"Argument {position+1} must be of type double.\");\n"
            retStr += "\t}\n"
            retStr += f"\tHelicsTime {argName} = (HelicsTime)(mxGetScalar(argv[{position}]));\n\n"
            return retStr
        
        
        def initializeArgInt32_t(argName: str, position: int, functionName: str) -> str:
            retStr = f"\tif(mxGetClassID(argv[{position}]) != mxINT32_CLASS){{\n"
            retStr += "\t\tmexUnlock();\n"
            retStr += f"\t\tmexErrMsgIdAndTxt(\"MATLAB:{functionName}:TypeError\",\"Argument {position+1} must be of type int32.\");\n"
            retStr += "\t}\n"
            retStr += f"\tmxInt32 *p{argName} = mxGetInt32s(argv[{position}]);\n"
            retStr += f"\tint32_t {argName} = static_cast<int32_t>(p{argName}[0]);\n\n"
            return retStr
        
        
        def initializeArgInt64_t(argName: str, position: int, functionName: str) -> str:
            retStr = f"\tif(mxGetClassID(argv[{position}]) != mxINT64_CLASS){{\n"
            retStr += "\t\tmexUnlock();\n"
            retStr += f"\t\tmexErrMsgIdAndTxt(\"MATLAB:{functionName}:TypeError\",\"Argument {position+1} must be of type int64.\");\n"
            retStr += "\t}\n"
            retStr += f"\tmxInt64 *p{argName} = mxGetInt64s(argv[{position}]);\n"
            retStr += f"\tint64_t {argName} = static_cast<int64_t>(p{argName}[0]);\n\n"
            
            return retStr
        
        
        def initializeArgHelicsErrorPtr(argName: str) -> str:
            retStr = f"\tHelicsError {argName} = helicsErrorInitialize();\n\n"
            return retStr
        
        
        def argCharFunctionCall(argName: str, position: int) -> str:
            if position == 0:
                retStr = f"{argName}[0]"
            else:
                retStr = f", {argName}[0]"
            return retStr
        
        
        def argCharPtrFunctionCall(argName: str, position: int) -> str:
            if position == 0:
                retStr = f"static_cast<const char *>({argName})"
            else:
                retStr = f", static_cast<const char *>({argName})"
            return retStr
        
        
        def argDefaultFunctionCall(argName: str, position: int) -> str:
            if position == 0:
                retStr = f"{argName}"
            else:
                retStr = f", {argName}"
            return retStr
        
        
        def argHelicsErrorPtrFunctionCall(argName: str, position: int) -> str:
            if position == 0:
                retStr = f"&{argName}"
            else:
                retStr = f", &{argName}"
            return retStr
        
        
        def returnCharTomxArray() -> str:
            retStr = "\tmxArray *_out = mxCreateString((const char *)(&result));"
            return retStr
        
        
        def returnCharPtrTomxArray() -> str:
            retStr = "\tmxArray *_out = mxCreateString(result);"
            return retStr
        
        
        def returnDoubleTomxArray() -> str:
            retStr = "\tmxArray *_out = mxCreateDoubleScalar(result);"
            return retStr
        
        
        def returnIntTomxArray() -> str:
            retStr = "\tmxArray *_out = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);\n"
            retStr += "\tmxInt64 *rv = mxGetInt64s(_out);\n"
            retStr += "\trv[0] = static_cast<mxInt64>(result);"
            return retStr
        
        
        def returnEnumTomxArray() -> str:
            retStr = "\tmxArray *_out = mxCreateNumericMatrix(1, 1, mxINT32_CLASS, mxREAL);\n"
            retStr += "\tmxInt32 *rv = mxGetInt32s(_out);\n"
            retStr += "\trv[0] = static_cast<mxInt32>(result);"
            return retStr
        
        
        def returnHelicsBoolTomxArray() -> str:
            retStr = "\tmxArray *_out = mxCreateLogicalMatrix(1, 1);\n"
            retStr += "\tmxLogical *rv = mxGetLogicals(_out);\n"
            retStr += "\tif(result == HELICS_TRUE){\n"
            retStr += "\t\trv[0] = true;\n"
            retStr += "\t}else{\n"
            retStr += "\t\trv[0] = false;\n"
            retStr += "\t}"
            return retStr
        
        
        def returnVoidPtrTomxArray() -> str:
            retStr = "\tmxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);\n"
            retStr += "\tmxUint64 *rv = mxGetUint64s(_out);\n"
            retStr += "\trv[0] = reinterpret_cast<mxUint64>(result);"
            return retStr
        
        
        def argCharPostFunctionCall(argName: str) -> str:
            return f"\tfree({argName});"
        
        
        def argHelicsErrorPtrPostFunctionCall(argName: str) -> str:
            retStr = f"\tif({argName}.error_code != HELICS_OK)" + "{\n"
            retStr += f"\t\tthrowHelicsMatlabError(&{argName});\n"
            retStr += "\t}"
            return retStr
        
        
        def HelicsIterationResultPtrPostFunctionCall(argName: str) -> str:
            retStr = f"\tmxArray *{argName}Mx = mxCreateNumericMatrix(1, 1, mxINT32_CLASS, mxREAL);\n"
            retStr += f"\tmxInt32 *{argName}Ptr = mxGetInt32s({argName}Mx);\n"
            retStr += f"\t{argName}Ptr[0] = static_cast<mxInt32>({argName});\n"
            retStr += "\tif(--resc >= 0) {\n"
            retStr += f"\t\t*resv++ = *{argName}Mx;\n"
            retStr += "\t}"
            return retStr
        
        
        def createModifiedMatlabFunction(functionDict: Dict[str,str], cursorIdx: int):
            modifiedPythonFunctionList = {
                "helicsCreateBrokerFromArgs": helicsCreateBrokerFromArgsMatlabWrapper,
                "helicsCreateCoreFromArgs": helicsCreateCoreFromArgsMatlabWrapper,
                "helicsFederateInfoLoadFromArgs": helicsFederateInfoLoadFromArgsMatlabWrapper,
                "helicsEndpointSendBytes": helicsEndpointSendBytesMatlabWrapper,
                "helicsEndpointSendBytesAt": helicsEndpointSendBytesAtMatlabWrapper,
                "helicsEndpointSendBytesTo": helicsEndpointSendBytesToMatlabWrapper,
                "helicsEndpointSendBytesToAt": helicsEndpointSendBytesToAtMatlabWrapper,
                "helicsFederateRequestTimeIterative": helicsFederateRequestTimeIterativeMatlabWrapper,
                "helicsFederateRequestTimeIterativeComplete": helicsFederateRequestTimeIterativeCompleteMatlabWrapper,
                "helicsInputGetBytes": helicsInputGetBytesMatlabWrapper,
                "helicsInputGetComplex": helicsInputGetComplexMatlabWrapper,
                "helicsInputGetComplexObject": helicsInputGetComplexObjectMatlabWrapper,
                "helicsInputGetNamedPoint": helicsInputGetNamedPointMatlabWrapper,
                "helicsInputGetString": helicsInputGetStringMatlabWrapper,
                "helicsInputGetVector": helicsInputGetVectorMatlabWrapper,
                "helicsInputGetComplexVector": helicsInputGetComplexVectorMatlabWrapper,
                "helicsInputSetDefaultBytes": helicsInputSetDefaultBytesMatlabWrapper,
                "helicsInputSetDefaultComplex": helicsInputSetDefaultComplexMatlabWrapper,
                "helicsInputSetDefaultVector": helicsInputSetDefaultVectorMatlabWrapper,
                "helicsInputSetDefaultComplexVector": helicsInputSetDefaultComplexVectorMatlabWrapper,
                "helicsMessageAppendData": helicsMessageAppendDataMatlabWrapper,
                "helicsMessageGetBytes": helicsMessageGetBytesMatlabWrapper,
                "helicsMessageSetData": helicsMessageSetDataMatlabWrapper,
                "helicsPublicationPublishBytes": helicsPublicationPublishBytesMatlabWrapper,
                "helicsPublicationPublishComplex": helicsPublicationPublishComplexMatlabWrapper,
                "helicsPublicationPublishVector": helicsPublicationPublishVectorMatlabWrapper,
                "helicsPublicationPublishComplexVector": helicsPublicationPublishComplexVectorMatlabWrapper,
                "helicsQueryBufferFill": helicsQueryBufferFillMatlabWrapper,
                "helicsLoadSignalHandlerCallback": helicsLoadSignalHandlerCallbackMatlabWrapper,
                "helicsLoadSignalHandlerCallbackNoExit": helicsLoadSignalHandlerCallbackNoExitMatlabWrapper,
                "helicsBrokerSetLoggingCallback": helicsBrokerSetLoggingCallbackMatlabWrapper,
                "helicsCoreSetLoggingCallback": helicsCoreSetLoggingCallbackMatlabWrapper,
                "helicsFederateSetLoggingCallback": helicsFederateSetLoggingCallbackMatlabWrapper,
                "helicsFilterSetCustomCallback": helicsFilterSetCustomCallbackMatlabWrapper,
                "helicsFederateSetQueryCallback": helicsFederateSetQueryCallbackMatlabWrapper,
                "helicsFederateSetTimeUpdateCallback": helicsFederateSetTimeUpdateCallbackMatlabWrapper,
                "helicsFederateSetStateChangeCallback": helicsFederateSetStateChangeCallbackMatlabWrapper,
                "helicsFederateSetTimeRequestEntryCallback": helicsFederateSetTimeRequestEntryCallbackMatlabWrapper,
                "helicsFederateSetTimeRequestReturnCallback": helicsFederateSetTimeRequestReturnCallbackMatlabWrapper,
                "helicsDataBufferFillFromComplex": helicsDataBufferFillFromComplexMatlabWrapper,
                "helicsDataBufferFillFromVector": helicsDataBufferFillFromVectorMatlabWrapper,
                "helicsDataBufferFillFromComplexVector": helicsDataBufferFillFromComplexVectorMatlabWrapper,
                "helicsDataBufferToString": helicsDataBufferToStringMatlabWrapper,
                "helicsDataBufferToRawString": helicsDataBufferToRawStringMatlabWrapper,
                "helicsDataBufferToComplex": helicsDataBufferToComplexMatlabWrapper,
                "helicsDataBufferToComplexObject": helicsDataBufferToComplexObjectMatlabWrapper,
                "helicsDataBufferToVector": helicsDataBufferToVectorMatlabWrapper,
                "helicsDataBufferToComplexVector": helicsDataBufferToComplexVectorMatlabWrapper,
                "helicsDataBufferToNamedPoint": helicsDataBufferToNamedPointMatlabWrapper,
                "helicsTranslatorSetCustomCallback": helicsTranslatorSetCustomCallbackMatlabWrapper,
                "helicsCloseLibrary": helicsCloseLibraryMatlabWrapper,
                "helicsFederateInitializingEntryCallback": helicsFederateInitializingEntryCallbackMatlabWrapper,
                "helicsFederateExecutingEntryCallback": helicsFederateExecutingEntryCallbackMatlabWrapper,
                "helicsFederateCosimulationTerminationCallback": helicsFederateCosimulationTerminationCallbackMatlabWrapper,
                "helicsFederateErrorHandlerCallback": helicsFederateErrorHandlerCallbackMatlabWrapper,
                "helicsCallbackFederateNextTimeCallback": helicsCallbackFederateNextTimeCallbackMatlabWrapper,
                "helicsCallbackFederateNextTimeIterativeCallback": helicsCallbackFederateNextTimeIterativeCallbackMatlabWrapper,
                "helicsCallbackFederateInitializeCallback": helicsCallbackFederateInitializeCallbackMatlabWrapper
            }
            functionComment, functionWrapper, functionMainElement = modifiedPythonFunctionList[functionDict.get("spelling","")](functionDict, cursorIdx)
            
            with open(os.path.join(self.__rootDir, f'matlabBindings/+helics/{functionDict.get("spelling","")}.m'), "w") as functionMFile:
                functionMFile.write(f'function varargout = {functionDict.get("spelling","")}(varargin)\n')
                functionMFile.write(functionComment)
                functionMFile.write("\t[varargout{1:nargout}] = helicsMex(" + f'\'{functionDict.get("spelling","")}\', varargin' + "{:});\n")
                functionMFile.write("end\n")
            return functionWrapper, functionMainElement, (functionDict.get("spelling",""), cursorIdx)
        
        
        def helicsCreateCoreFromArgsMatlabWrapper(functionDict: Dict[str,str], cursorIdx: int):
            #check to see if function signiture changed
            argNum = len(functionDict.get("arguments", {}).keys())
            if argNum != 5:
                raise RuntimeError("the function signature for helicsCreateCoreFromArgs has changed!")
            arg0 = functionDict.get("arguments", {}).get(0, {})
            if arg0.get("spelling","") != "type" or arg0.get("pointer_type", "") != "Char_S_*":
                raise RuntimeError(f"the function signature for helicsCreateCoreFromArgs has changed! arg: {arg0.get('spelling','')} type: {arg0.get('pointer_type','')}")
            arg1 = functionDict.get("arguments", {}).get(1, {})
            if arg1.get("spelling","") != "name" or arg1.get("pointer_type", "") != "Char_S_*":
                raise RuntimeError("the function signature for helicsCreateCoreFromArgs has changed!")
            arg2 = functionDict.get("arguments", {}).get(2, {})
            if arg2.get("spelling","") != "argc" or arg2.get("type", "") != "Int":
                raise RuntimeError("the function signature for helicsCreateCoreFromArgs has changed!")
            arg3 = functionDict.get("arguments", {}).get(3, {})
            if arg3.get("spelling","") != "argv" or arg3.get("double_pointer_type", "") != "Char_S_**":
                raise RuntimeError("the function signature for helicsCreateCoreFromArgs has changed!")
            arg4 = functionDict.get("arguments", {}).get(4, {})
            if arg4.get("spelling","") != "err" or arg4.get("pointer_type", "") != "HelicsError_*":
                raise RuntimeError("the function signature for helicsCreateCoreFromArgs has changed!")
            functionName = functionDict.get("spelling","")
            functionComment = "%\tCreate a core object by passing command line arguments.\n\n"
            functionComment += "%\t@param type The type of the core to create.\n"
            functionComment += "%\t@param name The name of the core.\n"
            functionComment += "%\t@param arguments The list of string values from a command line.\n\n"
            functionComment += "%\t@return A HelicsCore object.\n"
            functionWrapper = f"void _wrap_{functionName}(int resc, mxArray *resv[], int argc, const mxArray *argv[])" + "{\n"
            functionWrapper += f"\tif(argc != 3){{\n"
            functionWrapper += "\t\tmexUnlock();\n"
            functionWrapper += f"\t\tmexErrMsgIdAndTxt(\"MATLAB:{functionName}:rhs\",\"This function requires 3 arguments.\");\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += initializeArgChar("type", 0, functionName)
            functionWrapper += initializeArgChar("name", 1, functionName)
            functionWrapper += "\tint arg2 = 0;\n"
            functionWrapper += "\tchar **arg3;\n"
            functionWrapper += "\tint ii;\n"
            functionWrapper += f"\tif(!mxIsCell(argv[2])){{\n"
            functionWrapper += "\t\tmexUnlock();\n"
            functionWrapper += f"\t\tmexErrMsgIdAndTxt(\"MATLAB:{functionName}:TypeError\",\"Argument 3 must be a cell array of strings.\");\n"
            functionWrapper += "\t}\n"
            functionWrapper += "\targ2 = static_cast<int>(mxGetNumberOfElements(argv[2]));\n"
            functionWrapper += "\targ3 = static_cast<char **>(malloc((arg2)*sizeof(char *)));\n"
            functionWrapper += "\tfor (ii=0;ii<arg2;ii++){\n"
            functionWrapper += "\t\tmxArray *cellElement=mxGetCell(argv[2], ii);\n"
            functionWrapper += "\t\tsize_t len = mxGetN(cellElement) + 1;\n"
            functionWrapper += "\t\targ3[ii] = static_cast<char *>(malloc(static_cast<int>(len)));\n"
            functionWrapper += "\t\tint flag = mxGetString(cellElement, arg3[ii], static_cast<int>(len));\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += initializeArgHelicsErrorPtr("err")
            functionWrapper += f"\tHelicsCore result = {functionName}(type, name, arg2, arg3, &err);\n\n"
            functionWrapper += returnVoidPtrTomxArray()
            functionWrapper += "\tif(_out){\n"
            functionWrapper += "\t\t--resc;\n"
            functionWrapper += "\t\t*resv++ = _out;\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += f'{argCharPostFunctionCall("type")}\n\n'
            functionWrapper += f'{argCharPostFunctionCall("name")}\n\n'
            functionWrapper += f'{argCharPostFunctionCall("arg3")}\n\n'
            functionWrapper += f'{argHelicsErrorPtrPostFunctionCall("err")}\n'
            functionWrapper += "}\n\n\n"
            functionMainElements = f"\tcase {cursorIdx}:\n"
            functionMainElements += f"\t\t_wrap_{functionName}(resc, resv, argc, argv);\n"
            functionMainElements += f"\t\tbreak;\n"
            return functionComment, functionWrapper, functionMainElements
            
            
        def helicsCreateBrokerFromArgsMatlabWrapper(functionDict: Dict[str,str], cursorIdx: int):
            #check to see if function signiture changed
            argNum = len(functionDict.get("arguments", {}).keys())
            if argNum != 5:
                raise RuntimeError("the function signature for helicsCreateBrokerFromArgs has changed!")
            arg0 = functionDict.get("arguments", {}).get(0, {})
            if arg0.get("spelling","") != "type" or arg0.get("pointer_type", "") != "Char_S_*":
                raise RuntimeError("the function signature for helicsCreateBrokerFromArgs has changed!")
            arg1 = functionDict.get("arguments", {}).get(1, {})
            if arg1.get("spelling","") != "name" or arg1.get("pointer_type", "") != "Char_S_*":
                raise RuntimeError("the function signature for helicsCreateBrokerFromArgs has changed!")
            arg2 = functionDict.get("arguments", {}).get(2, {})
            if arg2.get("spelling","") != "argc" or arg2.get("type", "") != "Int":
                raise RuntimeError("the function signature for helicsCreateBrokerFromArgs has changed!")
            arg3 = functionDict.get("arguments", {}).get(3, {})
            if arg3.get("spelling","") != "argv" or arg3.get("double_pointer_type", "") != "Char_S_**":
                raise RuntimeError("the function signature for helicsCreateBrokerFromArgs has changed!")
            arg4 = functionDict.get("arguments", {}).get(4, {})
            if arg4.get("spelling","") != "err" or arg4.get("pointer_type", "") != "HelicsError_*":
                raise RuntimeError("the function signature for helicsCreateBrokerFromArgs has changed!")
            functionName = functionDict.get("spelling","")
            functionComment = "%\tCreate a broker object by passing command line arguments.\n\n"
            functionComment += "%\t@param type The type of the core to create.\n"
            functionComment += "%\t@param name The name of the core.\n"
            functionComment += "%\t@param arguments The list of string values from a command line.\n\n"
            functionComment += "%\t@return A HelicsBroker object.\n"
            functionWrapper = f"void _wrap_{functionName}(int resc, mxArray *resv[], int argc, const mxArray *argv[])" + "{\n"
            functionWrapper += f"\tif(argc != 3){{\n"
            functionWrapper += "\t\tmexUnlock();\n"
            functionWrapper += f"\t\tmexErrMsgIdAndTxt(\"MATLAB:{functionName}:rhs\",\"This function requires 3 arguments.\");\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += initializeArgChar("type", 0, functionName)
            functionWrapper += initializeArgChar("name", 1, functionName)
            functionWrapper += f"\tif(!mxIsCell(argv[2])){{\n"
            functionWrapper += "\t\tmexUnlock();\n"
            functionWrapper += f"\t\tmexErrMsgIdAndTxt(\"MATLAB:{functionName}:TypeError\",\"Argument 3 must be a cell array of strings.\");\n"
            functionWrapper += "\t}\n"
            functionWrapper += "\tint arg2 = 0;\n"
            functionWrapper += "\tchar **arg3;\n"
            functionWrapper += "\tint ii;\n"
            functionWrapper += "\targ2 = static_cast<int>(mxGetNumberOfElements(argv[2]));\n"
            functionWrapper += "\targ3 = static_cast<char **>(malloc((arg2)*sizeof(char *)));\n"
            functionWrapper += "\tfor (ii=0;ii<arg2;ii++){\n"
            functionWrapper += "\t\tmxArray *cellElement=mxGetCell(argv[2], ii);\n"
            functionWrapper += "\t\tsize_t len = mxGetN(cellElement) + 1;\n"
            functionWrapper += "\t\targ3[ii] = static_cast<char *>(malloc(static_cast<int>(len)));\n"
            functionWrapper += "\t\tint flag = mxGetString(cellElement, arg3[ii], static_cast<int>(len));\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += initializeArgHelicsErrorPtr("err")
            functionWrapper += f"\tHelicsBroker result = {functionName}(type, name, arg2, arg3, &err);\n\n"
            functionWrapper += returnVoidPtrTomxArray()
            functionWrapper += "\tif(_out){\n"
            functionWrapper += "\t\t--resc;\n"
            functionWrapper += "\t\t*resv++ = _out;\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += f'{argCharPostFunctionCall("type")}\n\n'
            functionWrapper += f'{argCharPostFunctionCall("name")}\n\n'
            functionWrapper += f'{argCharPostFunctionCall("arg3")}\n\n'
            functionWrapper += f'{argHelicsErrorPtrPostFunctionCall("err")}\n'
            functionWrapper += "}\n\n\n"
            functionMainElements = f"\tcase {cursorIdx}:\n"
            functionMainElements += f"\t\t_wrap_{functionName}(resc, resv, argc, argv);\n"
            functionMainElements += f"\t\tbreak;\n"
            return functionComment, functionWrapper, functionMainElements
        
        
        def helicsFederateInfoLoadFromArgsMatlabWrapper(functionDict: Dict[str,str], cursorIdx: int):
            #check to see if function signiture changed
            argNum = len(functionDict.get("arguments", {}).keys())
            if argNum != 4:
                raise RuntimeError("the function signature for helicsFederateInfoLoadFromArgs has changed!")
            arg0 = functionDict.get("arguments", {}).get(0, {})
            if arg0.get("spelling","") != "fedInfo" or arg0.get("type", "") != "HelicsFederateInfo":
                raise RuntimeError("the function signature for helicsFederateInfoLoadFromArgs has changed!")
            arg1 = functionDict.get("arguments", {}).get(1, {})
            if arg1.get("spelling","") != "argc" or arg1.get("type", "") != "Int":
                raise RuntimeError("the function signature for helicsFederateInfoLoadFromArgs has changed!")
            arg2 = functionDict.get("arguments", {}).get(2, {})
            if arg2.get("spelling","") != "argv" or arg2.get("double_pointer_type", "") != "Char_S_**":
                raise RuntimeError("the function signature for helicsFederateInfoLoadFromArgs has changed!")
            arg3 = functionDict.get("arguments", {}).get(3, {})
            if arg3.get("spelling","") != "err" or arg3.get("pointer_type", "") != "HelicsError_*":
                raise RuntimeError("the function signature for helicsFederateInfoLoadFromArgs has changed!")
            functionName = functionDict.get("spelling","")
            functionComment = "%\tLoad federate info from command line arguments.\n\n"
            functionComment += "%\t@param fi A federateInfo object.\n"
            functionComment += "%\t@param arguments A list of strings from the command line.\n"
            functionWrapper = f"void _wrap_{functionName}(int resc, mxArray *resv[], int argc, const mxArray *argv[])" + "{\n"
            functionWrapper += f"\tif(argc != 2){{\n"
            functionWrapper += "\t\tmexUnlock();\n"
            functionWrapper += f"\t\tmexErrMsgIdAndTxt(\"MATLAB:{functionName}:rhs\",\"This function requires 2 arguments.\");\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += initializeArgHelicsClass("HelicsFederateInfo", "fedInfo", 0, functionName)
            functionWrapper += f"\tif(!mxIsCell(argv[1])){{\n"
            functionWrapper += "\t\tmexUnlock();\n"
            functionWrapper += f"\t\tmexErrMsgIdAndTxt(\"MATLAB:{functionName}:TypeError\",\"Argument 2 must be of cell array of strings.\");\n"
            functionWrapper += "\t}\n"
            functionWrapper += "\tint arg1 = 0;\n"
            functionWrapper += "\tchar **arg2;\n"
            functionWrapper += "\tint ii;\n"
            functionWrapper += "\targ1 = static_cast<int>(mxGetNumberOfElements(argv[1]));\n"
            functionWrapper += "\targ2 = static_cast<char **>(malloc((arg1)*sizeof(char *)));\n"
            functionWrapper += "\tfor (ii=0;ii<arg1;ii++){\n"
            functionWrapper += "\t\tmxArray *cellElement=mxGetCell(argv[1], ii);\n"
            functionWrapper += "\t\tsize_t len = mxGetN(cellElement) + 1;\n"
            functionWrapper += "\t\targ2[ii] = static_cast<char *>(malloc(static_cast<int>(len)));\n"
            functionWrapper += "\t\tint flag = mxGetString(cellElement, arg2[ii], static_cast<int>(len));\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += initializeArgHelicsErrorPtr("err")
            functionWrapper += f"\t{functionName}(fedInfo, arg1, arg2, &err);\n\n"
            functionWrapper += "\tmxArray *_out = nullptr;\n"
            functionWrapper += "\tif(_out){\n"
            functionWrapper += "\t\t--resc;\n"
            functionWrapper += "\t\t*resv++ = _out;\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += f'{argCharPostFunctionCall("arg2")}\n\n'
            functionWrapper += f'{argHelicsErrorPtrPostFunctionCall("err")}\n'
            functionWrapper += "}\n\n\n"
            functionMainElements = f"\tcase {cursorIdx}:\n"
            functionMainElements += f"\t\t_wrap_{functionName}(resc, resv, argc, argv);\n"
            functionMainElements += f"\t\tbreak;\n"
            return functionComment, functionWrapper, functionMainElements
        
        
        def helicsEndpointSendBytesMatlabWrapper(functionDict: Dict[str,str], cursorIdx: int):
            #check to see if function signiture changed
            argNum = len(functionDict.get("arguments", {}).keys())
            if argNum != 4:
                raise RuntimeError("the function signature for helicsEndpointSendBytes has changed!")
            arg0 = functionDict.get("arguments", {}).get(0, {})
            if arg0.get("spelling","") != "endpoint" or arg0.get("type", "") != "HelicsEndpoint":
                raise RuntimeError("the function signature for helicsEndpointSendBytes has changed!")
            arg1 = functionDict.get("arguments", {}).get(1, {})
            if arg1.get("spelling","") != "data" or arg1.get("pointer_type", "") != "Void_*":
                raise RuntimeError("the function signature for helicsEndpointSendBytes has changed!")
            arg2 = functionDict.get("arguments", {}).get(2, {})
            if arg2.get("spelling","") != "inputDataLength" or arg2.get("type", "") != "Int":
                raise RuntimeError("the function signature for helicsEndpointSendBytes has changed!")
            arg3 = functionDict.get("arguments", {}).get(3, {})
            if arg3.get("spelling","") != "err" or arg3.get("pointer_type", "") != "HelicsError_*":
                raise RuntimeError("the function signature for helicsEndpointSendBytes has changed!")
            functionName = functionDict.get("spelling","")
            functionComment = "%\tSend a message to the targeted destinations.\n\n"
            functionComment += "%\t@param endpoint The endpoint to send the data from.\n"
            functionComment += "%\t@param data The data to send.\n"
            functionWrapper = f"void _wrap_{functionName}(int resc, mxArray *resv[], int argc, const mxArray *argv[])" + "{\n"
            functionWrapper += f"\tif(argc != 2){{\n"
            functionWrapper += "\t\tmexUnlock();\n"
            functionWrapper += f"\t\tmexErrMsgIdAndTxt(\"MATLAB:{functionName}:rhs\",\"This function requires 2 arguments.\");\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += initializeArgHelicsClass("HelicsEndpoint", "endpoint", 0, functionName)
            functionWrapper += initializeArgChar("data", 1, functionName)
            functionWrapper += "\tint inputDataLength = (int)(dataLength - 1);\n\n"
            functionWrapper += initializeArgHelicsErrorPtr("err")
            functionWrapper += f"\t{functionName}(endpoint, (void *)data, inputDataLength, &err);\n\n"
            functionWrapper += "\tmxArray *_out = nullptr;\n"
            functionWrapper += "\tif(_out){\n"
            functionWrapper += "\t\t--resc;\n"
            functionWrapper += "\t\t*resv++ = _out;\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += f'{argCharPostFunctionCall("data")}\n\n'
            functionWrapper += f'{argHelicsErrorPtrPostFunctionCall("err")}\n'
            functionWrapper += "}\n\n\n"
            functionMainElements = f"\tcase {cursorIdx}:\n"
            functionMainElements += f"\t\t_wrap_{functionName}(resc, resv, argc, argv);\n"
            functionMainElements += f"\t\tbreak;\n"
            return functionComment, functionWrapper, functionMainElements
        
        
        def helicsEndpointSendBytesAtMatlabWrapper(functionDict: Dict[str,str], cursorIdx: int):
            #check to see if function signiture changed
            argNum = len(functionDict.get("arguments", {}).keys())
            if argNum != 5:
                raise RuntimeError("the function signature for helicsEndpointSendBytesAt has changed!")
            arg0 = functionDict.get("arguments", {}).get(0, {})
            if arg0.get("spelling","") != "endpoint" or arg0.get("type", "") != "HelicsEndpoint":
                raise RuntimeError("the function signature for helicsEndpointSendBytesAt has changed!")
            arg1 = functionDict.get("arguments", {}).get(1, {})
            if arg1.get("spelling","") != "data" or arg1.get("pointer_type", "") != "Void_*":
                raise RuntimeError("the function signature for helicsEndpointSendBytesAt has changed!")
            arg2 = functionDict.get("arguments", {}).get(2, {})
            if arg2.get("spelling","") != "inputDataLength" or arg2.get("type", "") != "Int":
                raise RuntimeError("the function signature for helicsEndpointSendBytesAt has changed!")
            arg3 = functionDict.get("arguments", {}).get(3, {})
            if arg3.get("spelling","") != "time" or arg3.get("type", "") != "HelicsTime":
                raise RuntimeError("the function signature for helicsEndpointSendBytesAt has changed!")
            arg4 = functionDict.get("arguments", {}).get(4, {})
            if arg4.get("spelling","") != "err" or arg4.get("pointer_type", "") != "HelicsError_*":
                raise RuntimeError("the function signature for helicsEndpointSendBytesAt has changed!")
            functionName = functionDict.get("spelling","")
            functionComment = "%\tSend a message to the targeted destinations at a specified time.\n\n"
            functionComment += "%\t@param endpoint The endpoint to send the data from.\n"
            functionComment += "%\t@param data The data to send.\n"
            functionComment += "%\t@param time The time to send the message at.\n"
            functionWrapper = f"void _wrap_{functionName}(int resc, mxArray *resv[], int argc, const mxArray *argv[])" + "{\n"
            functionWrapper += f"\tif(argc != 3){{\n"
            functionWrapper += "\t\tmexUnlock();\n"
            functionWrapper += f"\t\tmexErrMsgIdAndTxt(\"MATLAB:{functionName}:rhs\",\"This function requires 3 arguments.\");\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += initializeArgHelicsClass("HelicsEndpoint", "endpoint", 0, functionName)
            functionWrapper += initializeArgChar("data", 1, functionName)
            functionWrapper += "\tint inputDataLength = (int)(dataLength - 1);\n\n"
            functionWrapper += initializeArgHelicsTime("time", 2, functionName)
            functionWrapper += initializeArgHelicsErrorPtr("err")
            functionWrapper += f"\t{functionName}(endpoint, (void *)data, inputDataLength, time, &err);\n\n"
            functionWrapper += "\tmxArray *_out = nullptr;\n"
            functionWrapper += "\tif(_out){\n"
            functionWrapper += "\t\t--resc;\n"
            functionWrapper += "\t\t*resv++ = _out;\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += f'{argCharPostFunctionCall("data")}\n\n'
            functionWrapper += f'{argHelicsErrorPtrPostFunctionCall("err")}\n'
            functionWrapper += "}\n\n\n"
            functionMainElements = f"\tcase {cursorIdx}:\n"
            functionMainElements += f"\t\t_wrap_{functionName}(resc, resv, argc, argv);\n"
            functionMainElements += f"\t\tbreak;\n"
            return functionComment, functionWrapper, functionMainElements
        
        
        def helicsEndpointSendBytesToMatlabWrapper(functionDict: Dict[str,str], cursorIdx: int):
            #check to see if function signiture changed
            argNum = len(functionDict.get("arguments", {}).keys())
            if argNum != 5:
                raise RuntimeError("the function signature for helicsEndpointSendBytesTo has changed!")
            arg0 = functionDict.get("arguments", {}).get(0, {})
            if arg0.get("spelling","") != "endpoint" or arg0.get("type", "") != "HelicsEndpoint":
                raise RuntimeError("the function signature for helicsEndpointSendBytesTo has changed!")
            arg1 = functionDict.get("arguments", {}).get(1, {})
            if arg1.get("spelling","") != "data" or arg1.get("pointer_type", "") != "Void_*":
                raise RuntimeError("the function signature for helicsEndpointSendBytesTo has changed!")
            arg2 = functionDict.get("arguments", {}).get(2, {})
            if arg2.get("spelling","") != "inputDataLength" or arg2.get("type", "") != "Int":
                raise RuntimeError("the function signature for helicsEndpointSendBytesTo has changed!")
            arg3 = functionDict.get("arguments", {}).get(3, {})
            if arg3.get("spelling","") != "dst" or arg3.get("pointer_type", "") != "Char_S_*":
                raise RuntimeError("the function signature for helicsEndpointSendBytesTo has changed!")
            arg4 = functionDict.get("arguments", {}).get(4, {})
            if arg4.get("spelling","") != "err" or arg4.get("pointer_type", "") != "HelicsError_*":
                raise RuntimeError("the function signature for helicsEndpointSendBytesTo has changed!")
            functionName = functionDict.get("spelling","")
            functionComment = "%\tSend a message to the specified destination.\n\n"
            functionComment += "%\t@param endpoint The endpoint to send the data from.\n"
            functionComment += "%\t@param data The data to send.\n"
            functionComment += "%\t@param dst The destination to send the message to.\n"
            functionWrapper = f"void _wrap_{functionName}(int resc, mxArray *resv[], int argc, const mxArray *argv[])" + "{\n"
            functionWrapper += f"\tif(argc != 3){{\n"
            functionWrapper += "\t\tmexUnlock();\n"
            functionWrapper += f"\t\tmexErrMsgIdAndTxt(\"MATLAB:{functionName}:rhs\",\"This function requires 3 arguments.\");\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += initializeArgHelicsClass("HelicsEndpoint", "endpoint", 0, functionName)
            functionWrapper += initializeArgChar("data", 1, functionName)
            functionWrapper += "\tint inputDataLength = (int)(dataLength - 1);\n\n"
            functionWrapper += initializeArgChar("dst", 2, functionName)
            functionWrapper += initializeArgHelicsErrorPtr("err")
            functionWrapper += f"\t{functionName}(endpoint, (void *)data, inputDataLength, dst, &err);\n\n"
            functionWrapper += "\tmxArray *_out = nullptr;\n"
            functionWrapper += "\tif(_out){\n"
            functionWrapper += "\t\t--resc;\n"
            functionWrapper += "\t\t*resv++ = _out;\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += f'{argCharPostFunctionCall("data")}\n\n'
            functionWrapper += f'{argCharPostFunctionCall("dst")}\n\n'
            functionWrapper += f'{argHelicsErrorPtrPostFunctionCall("err")}\n'
            functionWrapper += "}\n\n\n"
            functionMainElements = f"\tcase {cursorIdx}:\n"
            functionMainElements += f"\t\t_wrap_{functionName}(resc, resv, argc, argv);\n"
            functionMainElements += f"\t\tbreak;\n"
            return functionComment, functionWrapper, functionMainElements
        
        
        def helicsEndpointSendBytesToAtMatlabWrapper(functionDict: Dict[str,str], cursorIdx: int):
            #check to see if function signiture changed
            argNum = len(functionDict.get("arguments", {}).keys())
            if argNum != 6:
                raise RuntimeError("the function signature for helicsEndpointSendBytesToAt has changed!")
            arg0 = functionDict.get("arguments", {}).get(0, {})
            if arg0.get("spelling","") != "endpoint" or arg0.get("type", "") != "HelicsEndpoint":
                raise RuntimeError("the function signature for helicsEndpointSendBytesToAt has changed!")
            arg1 = functionDict.get("arguments", {}).get(1, {})
            if arg1.get("spelling","") != "data" or arg1.get("pointer_type", "") != "Void_*":
                raise RuntimeError("the function signature for helicsEndpointSendBytesToAt has changed!")
            arg2 = functionDict.get("arguments", {}).get(2, {})
            if arg2.get("spelling","") != "inputDataLength" or arg2.get("type", "") != "Int":
                raise RuntimeError("the function signature for helicsEndpointSendBytesToAt has changed!")
            arg3 = functionDict.get("arguments", {}).get(3, {})
            if arg3.get("spelling","") != "dst" or arg3.get("pointer_type", "") != "Char_S_*":
                raise RuntimeError("the function signature for helicsEndpointSendBytesToAt has changed!")
            arg4 = functionDict.get("arguments", {}).get(4, {})
            if arg4.get("spelling","") != "time" or arg4.get("type", "") != "HelicsTime":
                raise RuntimeError("the function signature for helicsEndpointSendBytesToAt has changed!")
            arg5 = functionDict.get("arguments", {}).get(5, {})
            if arg5.get("spelling","") != "err" or arg5.get("pointer_type", "") != "HelicsError_*":
                raise RuntimeError("the function signature for helicsEndpointSendBytesToAt has changed!")
            functionName = functionDict.get("spelling","")
            functionComment = "%\tSend a message to the specified destination at a specified time.\n\n"
            functionComment += "%\t@param endpoint The endpoint to send the data from.\n"
            functionComment += "%\t@param data The data to send.\n"
            functionComment += "%\t@param dst The destination to send the message to.\n"
            functionComment += "%\t@param time The time to send the message at.\n"
            functionWrapper = f"void _wrap_{functionName}(int resc, mxArray *resv[], int argc, const mxArray *argv[])" + "{\n"
            functionWrapper += f"\tif(argc != 4){{\n"
            functionWrapper += "\t\tmexUnlock();\n"
            functionWrapper += f"\t\tmexErrMsgIdAndTxt(\"MATLAB:{functionName}:rhs\",\"This function requires 4 arguments.\");\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += initializeArgHelicsClass("HelicsEndpoint", "endpoint", 0, functionName)
            functionWrapper += initializeArgChar("data", 1, functionName)
            functionWrapper += "\tint inputDataLength = (int)(dataLength - 1);\n\n"
            functionWrapper += initializeArgChar("dst", 2, functionName)
            functionWrapper += initializeArgHelicsTime("time", 3, functionName)
            functionWrapper += initializeArgHelicsErrorPtr("err")
            functionWrapper += f"\t{functionName}(endpoint, (void *)data, inputDataLength, dst, time, &err);\n\n"
            functionWrapper += "\tmxArray *_out = nullptr;\n"
            functionWrapper += "\tif(_out){\n"
            functionWrapper += "\t\t--resc;\n"
            functionWrapper += "\t\t*resv++ = _out;\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += f'{argCharPostFunctionCall("data")}\n\n'
            functionWrapper += f'{argCharPostFunctionCall("dst")}\n\n'
            functionWrapper += f'{argHelicsErrorPtrPostFunctionCall("err")}\n'
            functionWrapper += "}\n\n\n"
            functionMainElements = f"\tcase {cursorIdx}:\n"
            functionMainElements += f"\t\t_wrap_{functionName}(resc, resv, argc, argv);\n"
            functionMainElements += f"\t\tbreak;\n"
            return functionComment, functionWrapper, functionMainElements
        
        
        def helicsFederateRequestTimeIterativeMatlabWrapper(functionDict: Dict[str,str], cursorIdx: int):
            #check to see if function signiture changed
            argNum = len(functionDict.get("arguments", {}).keys())
            if argNum != 5:
                raise RuntimeError("the function signature for helicsFederateRequestTimeIterative has changed!")
            arg0 = functionDict.get("arguments", {}).get(0, {})
            if arg0.get("spelling","") != "fed" or arg0.get("type", "") != "HelicsFederate":
                raise RuntimeError("the function signature for helicsFederateRequestTimeIterative has changed!")
            arg1 = functionDict.get("arguments", {}).get(1, {})
            if arg1.get("spelling","") != "requestTime" or arg1.get("type", "") != "HelicsTime":
                raise RuntimeError("the function signature for helicsFederateRequestTimeIterative has changed!")
            arg2 = functionDict.get("arguments", {}).get(2, {})
            if arg2.get("spelling","") != "iterate" or arg2.get("type", "") != "HelicsIterationRequest":
                raise RuntimeError("the function signature for helicsFederateRequestTimeIterative has changed!")
            arg3 = functionDict.get("arguments", {}).get(3, {})
            if arg3.get("spelling","") != "outIteration" or arg3.get("pointer_type", "") != "HelicsIterationResult_*":
                raise RuntimeError("the function signature for helicsFederateRequestTimeIterative has changed!")
            arg4 = functionDict.get("arguments", {}).get(4, {})
            if arg4.get("spelling","") != "err" or arg4.get("pointer_type", "") != "HelicsError_*":
                raise RuntimeError("the function signature for helicsFederateRequestTimeIterative has changed!")
            functionName = functionDict.get("spelling","")
            functionComment = "%\tRequest an iterative time.\n\n"
            functionComment += "%\t@details This call allows for finer grain control of the iterative process than /ref helicsFederateRequestTime. It takes a time and and\n"
            functionComment += "%\titeration request, and returns a time and iteration status.\n\n"
            functionComment += "%\t@param fed The federate to make the request of.\n"
            functionComment += "%\t@param requestTime The next desired time.\n"
            functionComment += "%\t@param iterate The requested iteration mode.\n\n"
            functionComment += "%\t@return granted time and HelicsIterationResult.\n"
            functionWrapper = f"void _wrap_{functionName}(int resc, mxArray *resv[], int argc, const mxArray *argv[])" + "{\n"
            functionWrapper += f"\tif(argc != 3){{\n"
            functionWrapper += "\t\tmexUnlock();\n"
            functionWrapper += f"\t\tmexErrMsgIdAndTxt(\"MATLAB:{functionName}:rhs\",\"This function requires 3 arguments.\");\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += initializeArgHelicsClass("HelicsFederate", "fed", 0, functionName)
            functionWrapper += initializeArgHelicsTime("requestTime", 1, functionName)
            functionWrapper += initializeArgHelicsEnum("HelicsIterationRequest", "iterate", 2, functionName)
            functionWrapper += initializeArgHelicsIterationResultPtr("outIteration")
            functionWrapper += initializeArgHelicsErrorPtr("err")
            functionWrapper += f"\tHelicsTime result = {functionName}(fed, requestTime, iterate, &outIteration, &err);\n\n"
            functionWrapper += returnDoubleTomxArray()
            functionWrapper += "\tif(_out){\n"
            functionWrapper += "\t\t--resc;\n"
            functionWrapper += "\t\t*resv++ = _out;\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += "\tif(--resc>=0){\n"
            functionWrapper += "\t\tmxArray *_out1 = mxCreateNumericMatrix(1,1,mxINT32_CLASS,mxREAL);\n"
            functionWrapper += "\t\tmxInt32 *rv = mxGetInt32s(_out1);\n"
            functionWrapper += "\t\trv[0] = static_cast<mxInt32>(outIteration);\n"
            functionWrapper += "\t\t*resv++ = _out1;\n"
            functionWrapper += "\t}\n\n"            
            functionWrapper += f'{argHelicsErrorPtrPostFunctionCall("err")}\n'
            functionWrapper += "}\n\n\n"
            functionMainElements = f"\tcase {cursorIdx}:\n"
            functionMainElements += f"\t\t_wrap_{functionName}(resc, resv, argc, argv);\n"
            functionMainElements += f"\t\tbreak;\n"
            return functionComment, functionWrapper, functionMainElements
        
        
        def helicsFederateRequestTimeIterativeCompleteMatlabWrapper(functionDict: Dict[str,str], cursorIdx: int):
            #check to see if function signiture changed
            argNum = len(functionDict.get("arguments", {}).keys())
            if argNum != 3:
                raise RuntimeError("the function signature for helicsFederateRequestTimeIterativeComplete has changed!")
            arg0 = functionDict.get("arguments", {}).get(0, {})
            if arg0.get("spelling","") != "fed" or arg0.get("type", "") != "HelicsFederate":
                raise RuntimeError("the function signature for helicsFederateRequestTimeIterativeComplete has changed!")
            arg1 = functionDict.get("arguments", {}).get(1, {})
            if arg1.get("spelling","") != "outIterate" or arg1.get("pointer_type", "") != "HelicsIterationResult_*":
                raise RuntimeError("the function signature for helicsFederateRequestTimeIterativeComplete has changed!")
            arg2 = functionDict.get("arguments", {}).get(2, {})
            if arg2.get("spelling","") != "err" or arg2.get("pointer_type", "") != "HelicsError_*":
                raise RuntimeError("the function signature for helicsFederateRequestTimeIterativeComplete has changed!")
            functionName = functionDict.get("spelling","")
            functionComment = "%\tComplete an iterative time request asynchronous call.\n\n"
            functionComment += "%\t@param fed The federate to make the request of.\n\n"
            functionComment += "%\t@return tuple of HelicsTime and HelicsIterationResult.\n"
            functionWrapper = f"void _wrap_{functionName}(int resc, mxArray *resv[], int argc, const mxArray *argv[])" + "{\n"
            functionWrapper += f"\tif(argc != 1){{\n"
            functionWrapper += "\t\tmexUnlock();\n"
            functionWrapper += f"\t\tmexErrMsgIdAndTxt(\"MATLAB:{functionName}:rhs\",\"This function requires 1 arguments.\");\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += initializeArgHelicsClass("HelicsFederate", "fed", 0, functionName)
            functionWrapper += initializeArgHelicsIterationResultPtr("outIteration")
            functionWrapper += initializeArgHelicsErrorPtr("err")
            functionWrapper += f"\tHelicsTime result = {functionName}(fed, &outIteration, &err);\n\n"
            functionWrapper += returnDoubleTomxArray()
            functionWrapper += "\tif(_out){\n"
            functionWrapper += "\t\t--resc;\n"
            functionWrapper += "\t\t*resv++ = _out;\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += "\tif(--resc>=0){\n"
            functionWrapper += "\t\tmxArray *_out1 = mxCreateNumericMatrix(1,1,mxINT32_CLASS,mxREAL);\n"
            functionWrapper += "\t\tmxInt32 *rv = mxGetInt32s(_out1);\n"
            functionWrapper += "\t\trv[0] = static_cast<mxInt32>(outIteration);\n"
            functionWrapper += "\t\t*resv++ = _out1;\n"
            functionWrapper += "\t}\n\n"            
            functionWrapper += f'{argHelicsErrorPtrPostFunctionCall("err")}\n'
            functionWrapper += "}\n\n\n"
            functionMainElements = f"\tcase {cursorIdx}:\n"
            functionMainElements += f"\t\t_wrap_{functionName}(resc, resv, argc, argv);\n"
            functionMainElements += f"\t\tbreak;\n"
            return functionComment, functionWrapper, functionMainElements
        
        
        def helicsInputGetBytesMatlabWrapper(functionDict: Dict[str,str], cursorIdx: int):
            #check to see if function signiture changed
            argNum = len(functionDict.get("arguments", {}).keys())
            if argNum != 5:
                raise RuntimeError("the function signature for helicsInputGetBytes has changed!")
            arg0 = functionDict.get("arguments", {}).get(0, {})
            if arg0.get("spelling","") != "ipt" or arg0.get("type", "") != "HelicsInput":
                raise RuntimeError("the function signature for helicsInputGetBytes has changed!")
            arg1 = functionDict.get("arguments", {}).get(1, {})
            if arg1.get("spelling","") != "data" or arg1.get("pointer_type", "") != "Void_*":
                raise RuntimeError("the function signature for helicsInputGetBytes has changed!")
            arg2 = functionDict.get("arguments", {}).get(2, {})
            if arg2.get("spelling","") != "maxDataLength" or arg2.get("type", "") != "Int":
                raise RuntimeError("the function signature for helicsInputGetBytes has changed!")
            arg3 = functionDict.get("arguments", {}).get(3, {})
            if arg3.get("spelling","") != "actualSize" or arg3.get("pointer_type", "") != "Int_*":
                raise RuntimeError("the function signature for helicsInputGetBytes has changed!")
            arg4 = functionDict.get("arguments", {}).get(4, {})
            if arg4.get("spelling","") != "err" or arg4.get("pointer_type", "") != "HelicsError_*":
                raise RuntimeError("the function signature for helicsInputGetBytes has changed!")
            functionName = functionDict.get("spelling","")
            functionComment = "%\tGet the raw data for the latest value of a subscription.\n\n"
            functionComment += "%\t@param ipt The input to get the data for.\n\n"
            functionComment += "%\t@return  raw Bytes of the value, the value is uninterpreted raw bytes.\n"
            functionWrapper = f"void _wrap_{functionName}(int resc, mxArray *resv[], int argc, const mxArray *argv[])" + "{\n"
            functionWrapper += f"\tif(argc != 1){{\n"
            functionWrapper += "\t\tmexUnlock();\n"
            functionWrapper += f"\t\tmexErrMsgIdAndTxt(\"MATLAB:{functionName}:rhs\",\"This function requires 1 arguments.\");\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += initializeArgHelicsClass("HelicsInput", "ipt", 0, functionName)
            functionWrapper += "\tint maxDataLen = helicsInputGetByteCount(ipt) + 2;\n\n"
            functionWrapper += "\tvoid *data = malloc(maxDataLen);\n\n"
            functionWrapper += "\tint actualSize = 0;\n\n"
            functionWrapper += initializeArgHelicsErrorPtr("err")
            functionWrapper += f"\t{functionName}(ipt, data, maxDataLen, &actualSize, &err);\n\n"
            functionWrapper += "\tmxChar *dataChar = static_cast<mxChar *>(data);\n"
            functionWrapper += "\tmwSize dims[2] = {1, static_cast<mwSize>(actualSize)};\n"
            functionWrapper += "\tmxArray *_out = mxCreateCharArray(2, dims);\n"
            functionWrapper += "\tmxChar *out_data = static_cast<mxChar *>(mxGetData(_out));\n"
            functionWrapper += "\tfor(int i=0; i<actualSize; ++i){\n"
            functionWrapper += "\t\tout_data[i] = dataChar[i];\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += "\tif(_out){\n"
            functionWrapper += "\t\t--resc;\n"
            functionWrapper += "\t\t*resv++ = _out;\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += f'{argCharPostFunctionCall("data")}\n\n'
            functionWrapper += f'{argHelicsErrorPtrPostFunctionCall("err")}\n'
            functionWrapper += "}\n\n\n"
            functionMainElements = f"\tcase {cursorIdx}:\n"
            functionMainElements += f"\t\t_wrap_{functionName}(resc, resv, argc, argv);\n"
            functionMainElements += f"\t\tbreak;\n"
            return functionComment, functionWrapper, functionMainElements
        
        
        def helicsInputGetComplexObjectMatlabWrapper(functionDict: Dict[str,str], cursorIdx: int):
            #check to see if function signiture changed
            argNum = len(functionDict.get("arguments", {}).keys())
            if argNum != 2:
                raise RuntimeError("the function signature for helicsInputGetComplexObject has changed!")
            arg0 = functionDict.get("arguments", {}).get(0, {})
            if arg0.get("spelling","") != "ipt" or arg0.get("type", "") != "HelicsInput":
                raise RuntimeError("the function signature for helicsInputGetComplexObject has changed!")
            arg1 = functionDict.get("arguments", {}).get(1, {})
            if arg1.get("spelling","") != "err" or arg1.get("pointer_type", "") != "HelicsError_*":
                raise RuntimeError("the function signature for helicsInputGetComplexObject has changed!")
            functionName = functionDict.get("spelling","")
            functionComment = "%\tGet a complex value from an input object.\n\n"
            functionComment += "%\t@param ipt The input to get the data for.\n\n"
            functionComment += "%\t@return  A complex number.\n"
            functionWrapper = f"void _wrap_{functionName}(int resc, mxArray *resv[], int argc, const mxArray *argv[])" + "{\n"
            functionWrapper += f"\tif(argc != 1){{\n"
            functionWrapper += "\t\tmexUnlock();\n"
            functionWrapper += f"\t\tmexErrMsgIdAndTxt(\"MATLAB:{functionName}:rhs\",\"This function requires 1 arguments.\");\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += initializeArgHelicsClass("HelicsInput", "ipt", 0, functionName)
            functionWrapper += initializeArgHelicsErrorPtr("err")
            functionWrapper += f"\tHelicsComplex result = {functionName}(ipt, &err);\n\n"
            functionWrapper += "\tmxComplexDouble complex_result;\n"
            functionWrapper += "\tcomplex_result.real = result.real;\n"
            functionWrapper += "\tcomplex_result.imag = result.imag;\n"
            functionWrapper += "\tmxArray *_out = mxCreateDoubleMatrix(1,1,mxCOMPLEX);\n"
            functionWrapper += "\tint status = mxSetComplexDoubles(_out, &complex_result);\n\n"
            functionWrapper += "\tif(_out){\n"
            functionWrapper += "\t\t--resc;\n"
            functionWrapper += "\t\t*resv++ = _out;\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += f'{argHelicsErrorPtrPostFunctionCall("err")}\n'
            functionWrapper += "}\n\n\n"
            functionMainElements = f"\tcase {cursorIdx}:\n"
            functionMainElements += f"\t\t_wrap_{functionName}(resc, resv, argc, argv);\n"
            functionMainElements += f"\t\tbreak;\n"
            return functionComment, functionWrapper, functionMainElements
        
        
        def helicsInputGetComplexMatlabWrapper(functionDict: Dict[str,str], cursorIdx: int):
            #check to see if function signiture changed
            argNum = len(functionDict.get("arguments", {}).keys())
            if argNum != 4:
                raise RuntimeError("the function signature for helicsInputGetComplex has changed!")
            arg0 = functionDict.get("arguments", {}).get(0, {})
            if arg0.get("spelling","") != "ipt" or arg0.get("type", "") != "HelicsInput":
                raise RuntimeError("the function signature for helicsInputGetComplex has changed!")
            arg1 = functionDict.get("arguments", {}).get(1, {})
            if arg1.get("spelling","") != "real" or arg1.get("pointer_type", "") != "Double_*":
                raise RuntimeError("the function signature for helicsInputGetComplex has changed!")
            arg2 = functionDict.get("arguments", {}).get(2, {})
            if arg2.get("spelling","") != "imag" or arg2.get("pointer_type", "") != "Double_*":
                raise RuntimeError("the function signature for helicsInputGetComplex has changed!")
            arg3 = functionDict.get("arguments", {}).get(3, {})
            if arg3.get("spelling","") != "err" or arg3.get("pointer_type", "") != "HelicsError_*":
                raise RuntimeError("the function signature for helicsInputGetComplex has changed!")
            functionName = functionDict.get("spelling","")
            functionComment = "%\tGet a complex value from an input object.\n\n"
            functionComment += "%\t@param ipt The input to get the data for.\n\n"
            functionComment += "%\t@return  A complex number.\n"
            functionWrapper = f"void _wrap_{functionName}(int resc, mxArray *resv[], int argc, const mxArray *argv[])" + "{\n"
            functionWrapper += f"\tif(argc != 1){{\n"
            functionWrapper += "\t\tmexUnlock();\n"
            functionWrapper += f"\t\tmexErrMsgIdAndTxt(\"MATLAB:{functionName}:rhs\",\"This function requires 1 arguments.\");\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += initializeArgHelicsClass("HelicsInput", "ipt", 0, functionName)
            functionWrapper += "\tdouble values[2];\n\n"
            functionWrapper += initializeArgHelicsErrorPtr("err")
            functionWrapper += f"\t{functionName}(ipt, &(values[0]), &(values[1]), &err);\n\n"
            functionWrapper += "\tmxArray *_out = mxCreateDoubleMatrix(1,1,mxCOMPLEX);\n"
            functionWrapper += "\tmxComplexDouble *complex_result = mxGetComplexDoubles(_out);\n"
            functionWrapper += "\tcomplex_result->real = values[0];\n"
            functionWrapper += "\tcomplex_result->imag = values[1];\n"
            functionWrapper += "\tif(_out){\n"
            functionWrapper += "\t\t--resc;\n"
            functionWrapper += "\t\t*resv++ = _out;\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += f'{argHelicsErrorPtrPostFunctionCall("err")}\n'
            functionWrapper += "}\n\n\n"
            functionMainElements = f"\tcase {cursorIdx}:\n"
            functionMainElements += f"\t\t_wrap_{functionName}(resc, resv, argc, argv);\n"
            functionMainElements += f"\t\tbreak;\n"
            return functionComment, functionWrapper, functionMainElements
        
        
        def helicsInputGetNamedPointMatlabWrapper(functionDict: Dict[str,str], cursorIdx: int):
            #check to see if function signiture changed
            argNum = len(functionDict.get("arguments", {}).keys())
            if argNum != 6:
                raise RuntimeError("the function signature for helicsInputGetNamedPoint has changed!")
            arg0 = functionDict.get("arguments", {}).get(0, {})
            if arg0.get("spelling","") != "ipt" or arg0.get("type", "") != "HelicsInput":
                raise RuntimeError("the function signature for helicsInputGetNamedPoint has changed!")
            arg1 = functionDict.get("arguments", {}).get(1, {})
            if arg1.get("spelling","") != "outputString" or arg1.get("pointer_type", "") != "Char_S_*":
                raise RuntimeError("the function signature for helicsInputGetNamedPoint has changed!")
            arg2 = functionDict.get("arguments", {}).get(2, {})
            if arg2.get("spelling","") != "maxStringLength" or arg2.get("type", "") != "Int":
                raise RuntimeError("the function signature for helicsInputGetNamedPoint has changed!")
            arg3 = functionDict.get("arguments", {}).get(3, {})
            if arg3.get("spelling","") != "actualLength" or arg3.get("pointer_type", "") != "Int_*":
                raise RuntimeError("the function signature for helicsInputGetNamedPoint has changed!")
            arg4 = functionDict.get("arguments", {}).get(4, {})
            if arg4.get("spelling","") != "val" or arg4.get("pointer_type", "") != "Double_*":
                raise RuntimeError("the function signature for helicsInputGetNamedPoint has changed!")
            arg5 = functionDict.get("arguments", {}).get(5, {})
            if arg5.get("spelling","") != "err" or arg5.get("pointer_type", "") != "HelicsError_*":
                raise RuntimeError("the function signature for helicsInputGetNamedPoint has changed!")
            functionName = functionDict.get("spelling","")
            functionComment = "%\tGet a named point from a subscription.\n\n"
            functionComment += "%\t@param ipt The input to get the result for.\n\n"
            functionComment += "%\t@return a string and a double value for the named point\n"
            functionWrapper = f"void _wrap_{functionName}(int resc, mxArray *resv[], int argc, const mxArray *argv[])" + "{\n"
            functionWrapper += f"\tif(argc != 1){{\n"
            functionWrapper += "\t\tmexUnlock();\n"
            functionWrapper += f"\t\tmexErrMsgIdAndTxt(\"MATLAB:{functionName}:rhs\",\"This function requires 1 arguments.\");\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += initializeArgHelicsClass("HelicsInput", "ipt", 0, functionName)
            functionWrapper += "\tint maxStringLen = helicsInputGetStringSize(ipt) + 2;\n\n"
            functionWrapper += "\tchar *outputString = static_cast<char *>(malloc(maxStringLen));\n\n"
            functionWrapper += "\tint actualLength = 0;\n\n"
            functionWrapper += "\tdouble val = 0;\n\n"
            functionWrapper += initializeArgHelicsErrorPtr("err")
            functionWrapper += f"\t{functionName}(ipt, outputString, maxStringLen, &actualLength, &val, &err);\n\n"
            functionWrapper += "\tmwSize dims[2] = {1, static_cast<mwSize>(actualLength)-1};\n"
            functionWrapper += "\tmxArray *_out = mxCreateCharArray(2, dims);\n"
            functionWrapper += "\tmxChar *out_data = static_cast<mxChar *>(mxGetData(_out));\n"
            functionWrapper += "\tfor(int i=0; i<(actualLength-1); ++i){\n"
            functionWrapper += "\t\tout_data[i] = outputString[i];\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += "\tif(_out){\n"
            functionWrapper += "\t\t--resc;\n"
            functionWrapper += "\t\t*resv++ = _out;\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += "\tif(--resc>=0){\n"
            functionWrapper += "\t\tmxArray *_out1 = mxCreateDoubleScalar(val);\n"
            functionWrapper += "\t\t*resv++ = _out1;\n"
            functionWrapper += "\t}\n\n" 
            functionWrapper += f'{argCharPostFunctionCall("outputString")}\n\n'
            functionWrapper += f'{argHelicsErrorPtrPostFunctionCall("err")}\n'
            functionWrapper += "}\n\n\n"
            functionMainElements = f"\tcase {cursorIdx}:\n"
            functionMainElements += f"\t\t_wrap_{functionName}(resc, resv, argc, argv);\n"
            functionMainElements += f"\t\tbreak;\n"
            return functionComment, functionWrapper, functionMainElements
        
        
        def helicsInputGetStringMatlabWrapper(functionDict: Dict[str,str], cursorIdx: int):
            #check to see if function signiture changed
            argNum = len(functionDict.get("arguments", {}).keys())
            if argNum != 5:
                raise RuntimeError("the function signature for helicsInputGetString has changed!")
            arg0 = functionDict.get("arguments", {}).get(0, {})
            if arg0.get("spelling","") != "ipt" or arg0.get("type", "") != "HelicsInput":
                raise RuntimeError("the function signature for helicsInputGetString has changed!")
            arg1 = functionDict.get("arguments", {}).get(1, {})
            if arg1.get("spelling","") != "outputString" or arg1.get("pointer_type", "") != "Char_S_*":
                raise RuntimeError("the function signature for helicsInputGetString has changed!")
            arg2 = functionDict.get("arguments", {}).get(2, {})
            if arg2.get("spelling","") != "maxStringLength" or arg2.get("type", "") != "Int":
                raise RuntimeError("the function signature for helicsInputGetString has changed!")
            arg3 = functionDict.get("arguments", {}).get(3, {})
            if arg3.get("spelling","") != "actualLength" or arg3.get("pointer_type", "") != "Int_*":
                raise RuntimeError("the function signature for helicsInputGetString has changed!")
            arg4 = functionDict.get("arguments", {}).get(4, {})
            if arg4.get("spelling","") != "err" or arg4.get("pointer_type", "") != "HelicsError_*":
                raise RuntimeError("the function signature for helicsInputGetString has changed!")
            functionName = functionDict.get("spelling","")
            functionComment = "%\tGet a string value from a subscription.\n\n"
            functionComment += "%\t@param ipt The input to get the string for.\n\n"
            functionComment += "%\t@return the string value.\n"
            functionWrapper = f"void _wrap_{functionName}(int resc, mxArray *resv[], int argc, const mxArray *argv[])" + "{\n"
            functionWrapper += f"\tif(argc != 1){{\n"
            functionWrapper += "\t\tmexUnlock();\n"
            functionWrapper += f"\t\tmexErrMsgIdAndTxt(\"MATLAB:{functionName}:rhs\",\"This function requires 1 arguments.\");\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += initializeArgHelicsClass("HelicsInput", "ipt", 0, functionName)
            functionWrapper += "\tint maxStringLen = helicsInputGetStringSize(ipt) + 2;\n\n"
            functionWrapper += "\tchar *outputString = static_cast<char *>(malloc(maxStringLen));\n\n"
            functionWrapper += "\tint actualLength = 0;\n\n"
            functionWrapper += initializeArgHelicsErrorPtr("err")
            functionWrapper += f"\t{functionName}(ipt, outputString, maxStringLen, &actualLength, &err);\n\n"
            functionWrapper += "\tmwSize dims[2] = {1, static_cast<mwSize>(actualLength) - 1};\n"
            functionWrapper += "\tmxArray *_out = mxCreateCharArray(2, dims);\n"
            functionWrapper += "\tmxChar *out_data = static_cast<mxChar *>(mxGetData(_out));\n"
            functionWrapper += "\tfor(int i=0; i<(actualLength-1); ++i){\n"
            functionWrapper += "\t\tout_data[i] = outputString[i];\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += "\tif(_out){\n"
            functionWrapper += "\t\t--resc;\n"
            functionWrapper += "\t\t*resv++ = _out;\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += f'{argCharPostFunctionCall("outputString")}\n\n'
            functionWrapper += f'{argHelicsErrorPtrPostFunctionCall("err")}\n'
            functionWrapper += "}\n\n\n"
            functionMainElements = f"\tcase {cursorIdx}:\n"
            functionMainElements += f"\t\t_wrap_{functionName}(resc, resv, argc, argv);\n"
            functionMainElements += f"\t\tbreak;\n"
            return functionComment, functionWrapper, functionMainElements
        
        
        def helicsInputGetVectorMatlabWrapper(functionDict: Dict[str,str], cursorIdx: int):
            #check to see if function signiture changed
            argNum = len(functionDict.get("arguments", {}).keys())
            if argNum != 5:
                raise RuntimeError("the function signature for helicsInputGetVector has changed!")
            arg0 = functionDict.get("arguments", {}).get(0, {})
            if arg0.get("spelling","") != "ipt" or arg0.get("type", "") != "HelicsInput":
                raise RuntimeError("the function signature for helicsInputGetVector has changed!")
            arg1 = functionDict.get("arguments", {}).get(1, {})
            if arg1.get("spelling","") != "data" or arg1.get("type", "") != "IncompleteArray":
                raise RuntimeError("the function signature for helicsInputGetVector has changed!")
            arg2 = functionDict.get("arguments", {}).get(2, {})
            if arg2.get("spelling","") != "maxLength" or arg2.get("type", "") != "Int":
                raise RuntimeError("the function signature for helicsInputGetVector has changed!")
            arg3 = functionDict.get("arguments", {}).get(3, {})
            if arg3.get("spelling","") != "actualSize" or arg3.get("pointer_type", "") != "Int_*":
                raise RuntimeError("the function signature for helicsInputGetVector has changed!")
            arg4 = functionDict.get("arguments", {}).get(4, {})
            if arg4.get("spelling","") != "err" or arg4.get("pointer_type", "") != "HelicsError_*":
                raise RuntimeError("the function signature for helicsInputGetVector has changed!")
            functionName = functionDict.get("spelling","")
            functionComment = "%\tGet a vector from a subscription.\n\n"
            functionComment += "%\t@param ipt The input to get the vector for.\n\n"
            functionComment += "%\t@return  a list of floating point values.\n"
            functionWrapper = f"void _wrap_{functionName}(int resc, mxArray *resv[], int argc, const mxArray *argv[])" + "{\n"
            functionWrapper += f"\tif(argc != 1){{\n"
            functionWrapper += "\t\tmexUnlock();\n"
            functionWrapper += f"\t\tmexErrMsgIdAndTxt(\"MATLAB:{functionName}:rhs\",\"This function requires 1 arguments.\");\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += initializeArgHelicsClass("HelicsInput", "ipt", 0, functionName)
            functionWrapper += "\tint maxLength = helicsInputGetVectorSize(ipt);\n\n"
            functionWrapper += "\tdouble *data = static_cast<double *>(malloc(maxLength * sizeof(double)));\n\n"
            functionWrapper += "\tint actualSize = 0;\n\n"
            functionWrapper += initializeArgHelicsErrorPtr("err")
            functionWrapper += f"\t{functionName}(ipt, data, maxLength, &actualSize, &err);\n\n"
            functionWrapper += "\tmxDouble *result_data = static_cast<mxDouble *>(mxMalloc(actualSize * sizeof(mxDouble)));\n"
            functionWrapper += "\tfor(int i=0; i<actualSize; ++i){\n"
            functionWrapper += "\t\tresult_data[i] = static_cast<mxDouble>(data[i]);\n"
            functionWrapper += "\t}\n"
            functionWrapper += "\tmxArray *_out = mxCreateDoubleMatrix(actualSize, 1, mxREAL);\n"
            functionWrapper += "\tint status = mxSetDoubles(_out, &(result_data[0]));\n\n"
            functionWrapper += "\tif(_out){\n"
            functionWrapper += "\t\t--resc;\n"
            functionWrapper += "\t\t*resv++ = _out;\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += f'{argHelicsErrorPtrPostFunctionCall("err")}\n'
            functionWrapper += "}\n\n\n"
            functionMainElements = f"\tcase {cursorIdx}:\n"
            functionMainElements += f"\t\t_wrap_{functionName}(resc, resv, argc, argv);\n"
            functionMainElements += f"\t\tbreak;\n"
            return functionComment, functionWrapper, functionMainElements
        
        
        def helicsInputGetComplexVectorMatlabWrapper(functionDict: Dict[str,str], cursorIdx: int):
            #check to see if function signiture changed
            argNum = len(functionDict.get("arguments", {}).keys())
            if argNum != 5:
                raise RuntimeError("the function signature for helicsInputGetComplexVector has changed!")
            arg0 = functionDict.get("arguments", {}).get(0, {})
            if arg0.get("spelling","") != "ipt" or arg0.get("type", "") != "HelicsInput":
                raise RuntimeError("the function signature for helicsInputGetComplexVector has changed!")
            arg1 = functionDict.get("arguments", {}).get(1, {})
            if arg1.get("spelling","") != "data" or arg1.get("type", "") != "IncompleteArray":
                raise RuntimeError("the function signature for helicsInputGetComplexVector has changed!")
            arg2 = functionDict.get("arguments", {}).get(2, {})
            if arg2.get("spelling","") != "maxLength" or arg2.get("type", "") != "Int":
                raise RuntimeError("the function signature for helicsInputGetComplexVector has changed!")
            arg3 = functionDict.get("arguments", {}).get(3, {})
            if arg3.get("spelling","") != "actualSize" or arg3.get("pointer_type", "") != "Int_*":
                raise RuntimeError("the function signature for helicsInputGetComplexVector has changed!")
            arg4 = functionDict.get("arguments", {}).get(4, {})
            if arg4.get("spelling","") != "err" or arg4.get("pointer_type", "") != "HelicsError_*":
                raise RuntimeError("the function signature for helicsInputGetComplexVector has changed!")
            functionName = functionDict.get("spelling","")
            functionComment = "%\tGet a compelx vector from a subscription.\n\n"
            functionComment += "%\t@param ipt The input to get the vector for.\n\n"
            functionComment += "%\t@return a list of complex values.\n"
            functionWrapper = f"void _wrap_{functionName}(int resc, mxArray *resv[], int argc, const mxArray *argv[])" + "{\n"
            functionWrapper += f"\tif(argc != 1){{\n"
            functionWrapper += "\t\tmexUnlock();\n"
            functionWrapper += f"\t\tmexErrMsgIdAndTxt(\"MATLAB:{functionName}:rhs\",\"This function requires 1 arguments.\");\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += initializeArgHelicsClass("HelicsInput", "ipt", 0, functionName)
            functionWrapper += "\tint maxLength = helicsInputGetVectorSize(ipt);\n\n"
            functionWrapper += "\tdouble *data = static_cast<double *>(malloc(maxLength * sizeof(double)));\n\n"
            functionWrapper += "\tint actualSize = 0;\n\n"
            functionWrapper += initializeArgHelicsErrorPtr("err")
            functionWrapper += f"\t{functionName}(ipt, data, maxLength, &actualSize, &err);\n\n"
            functionWrapper += "\tmxComplexDouble *result_data = static_cast<mxComplexDouble *>(mxMalloc((actualSize/2)*sizeof(mxComplexDouble)));\n"
            functionWrapper += "\tfor(int i=0; i<(actualSize/2); ++i){\n"
            functionWrapper += "\t\tresult_data[i].real = data[2*(i)];\n"
            functionWrapper += "\t\tresult_data[i].imag = data[2*(i) + 1];\n"
            functionWrapper += "\t}\n"
            functionWrapper += "\tmxArray *_out = mxCreateDoubleMatrix(actualSize/2, 1, mxCOMPLEX);\n"
            functionWrapper += "\tint status = mxSetComplexDoubles(_out, &(result_data[0]));\n\n"
            functionWrapper += "\tif(_out){\n"
            functionWrapper += "\t\t--resc;\n"
            functionWrapper += "\t\t*resv++ = _out;\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += f'{argHelicsErrorPtrPostFunctionCall("err")}\n'
            functionWrapper += "}\n\n\n"
            functionMainElements = f"\tcase {cursorIdx}:\n"
            functionMainElements += f"\t\t_wrap_{functionName}(resc, resv, argc, argv);\n"
            functionMainElements += f"\t\tbreak;\n"
            return functionComment, functionWrapper, functionMainElements
        
        
        def helicsInputSetDefaultBytesMatlabWrapper(functionDict: Dict[str,str], cursorIdx: int):
            #check to see if function signiture changed
            argNum = len(functionDict.get("arguments", {}).keys())
            if argNum != 4:
                raise RuntimeError("the function signature for helicsInputSetDefaultBytes has changed!")
            arg0 = functionDict.get("arguments", {}).get(0, {})
            if arg0.get("spelling","") != "ipt" or arg0.get("type", "") != "HelicsInput":
                raise RuntimeError("the function signature for helicsInputSetDefaultBytes has changed!")
            arg1 = functionDict.get("arguments", {}).get(1, {})
            if arg1.get("spelling","") != "data" or arg1.get("pointer_type", "") != "Void_*":
                raise RuntimeError("the function signature for helicsInputSetDefaultBytes has changed!")
            arg2 = functionDict.get("arguments", {}).get(2, {})
            if arg2.get("spelling","") != "inputDataLength" or arg2.get("type", "") != "Int":
                raise RuntimeError("the function signature for helicsInputSetDefaultBytes has changed!")
            arg3 = functionDict.get("arguments", {}).get(3, {})
            if arg3.get("spelling","") != "err" or arg3.get("pointer_type", "") != "HelicsError_*":
                raise RuntimeError("the function signature for helicsInputSetDefaultBytes has changed!")
            functionName = functionDict.get("spelling","")
            functionComment = "%\tSet the default as a raw data array.\n\n"
            functionComment += "%\t@param ipt The input to set the default for.\n"
            functionComment += "%\t@param raw data to use for the default.\n"
            functionWrapper = f"void _wrap_{functionName}(int resc, mxArray *resv[], int argc, const mxArray *argv[])" + "{\n"
            functionWrapper += f"\tif(argc != 2){{\n"
            functionWrapper += "\t\tmexUnlock();\n"
            functionWrapper += f"\t\tmexErrMsgIdAndTxt(\"MATLAB:{functionName}:rhs\",\"This function requires 2 arguments.\");\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += initializeArgHelicsClass("HelicsInput", "ipt", 0, functionName)
            functionWrapper += initializeArgChar("data", 1, functionName)
            functionWrapper += "\tint inputDataLength = static_cast<int>(dataLength) - 1;\n\n"
            functionWrapper += initializeArgHelicsErrorPtr("err")
            functionWrapper += f"\t{functionName}(ipt, static_cast<void *>(data), inputDataLength, &err);\n\n"
            functionWrapper += "\tmxArray *_out = nullptr;\n"
            functionWrapper += "\tif(_out){\n"
            functionWrapper += "\t\t--resc;\n"
            functionWrapper += "\t\t*resv++ = _out;\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += f'{argCharPostFunctionCall("data")}\n\n'
            functionWrapper += f'{argHelicsErrorPtrPostFunctionCall("err")}\n'
            functionWrapper += "}\n\n\n"
            functionMainElements = f"\tcase {cursorIdx}:\n"
            functionMainElements += f"\t\t_wrap_{functionName}(resc, resv, argc, argv);\n"
            functionMainElements += f"\t\tbreak;\n"
            return functionComment, functionWrapper, functionMainElements
        
        
        def helicsInputSetDefaultComplexMatlabWrapper(functionDict: Dict[str,str], cursorIdx: int):
            #check to see if function signiture changed
            argNum = len(functionDict.get("arguments", {}).keys())
            if argNum != 4:
                raise RuntimeError("the function signature for helicsInputSetDefaultComplex has changed!")
            arg0 = functionDict.get("arguments", {}).get(0, {})
            if arg0.get("spelling","") != "ipt" or arg0.get("type", "") != "HelicsInput":
                raise RuntimeError("the function signature for helicsInputSetDefaultComplex has changed!")
            arg1 = functionDict.get("arguments", {}).get(1, {})
            if arg1.get("spelling","") != "real" or arg1.get("type", "") != "Double":
                raise RuntimeError("the function signature for helicsInputSetDefaultComplex has changed!")
            arg2 = functionDict.get("arguments", {}).get(2, {})
            if arg2.get("spelling","") != "imag" or arg2.get("type", "") != "Double":
                raise RuntimeError("the function signature for helicsInputSetDefaultComplex has changed!")
            arg3 = functionDict.get("arguments", {}).get(3, {})
            if arg3.get("spelling","") != "err" or arg3.get("pointer_type", "") != "HelicsError_*":
                raise RuntimeError("the function signature for helicsInputSetDefaultComplex has changed!")
            functionName = functionDict.get("spelling","")
            functionComment = "%\tSet the default as a complex number.\n\n"
            functionComment += "%\t@param ipt The input to get the data for.\n"
            functionComment += "%\t@param value The default complex value.\n"
            functionWrapper = f"void _wrap_{functionName}(int resc, mxArray *resv[], int argc, const mxArray *argv[])" + "{\n"
            functionWrapper += f"\tif(argc != 2){{\n"
            functionWrapper += "\t\tmexUnlock();\n"
            functionWrapper += f"\t\tmexErrMsgIdAndTxt(\"MATLAB:{functionName}:rhs\",\"This function requires 2 arguments.\");\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += initializeArgHelicsClass("HelicsInput", "ipt", 0, functionName)
            functionWrapper += f"\tif(!mxIsComplex(argv[1])){{\n"
            functionWrapper += "\t\tmexUnlock();\n"
            functionWrapper += f"\t\tmexErrMsgIdAndTxt(\"MATLAB:{functionName}:TypeError\",\"Argument 2 must be of type complex.\");\n"
            functionWrapper += "\t}\n"
            functionWrapper += "\tmxComplexDouble *value = mxGetComplexDoubles(argv[1]);\n\n"
            functionWrapper += initializeArgHelicsErrorPtr("err")
            functionWrapper += f"\t{functionName}(ipt, static_cast<double>(value->real), static_cast<double>(value->imag), &err);\n\n"
            functionWrapper += "\tmxArray *_out = nullptr;\n"
            functionWrapper += "\tif(_out){\n"
            functionWrapper += "\t\t--resc;\n"
            functionWrapper += "\t\t*resv++ = _out;\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += f'{argHelicsErrorPtrPostFunctionCall("err")}\n'
            functionWrapper += "}\n\n\n"
            functionMainElements = f"\tcase {cursorIdx}:\n"
            functionMainElements += f"\t\t_wrap_{functionName}(resc, resv, argc, argv);\n"
            functionMainElements += f"\t\tbreak;\n"
            return functionComment, functionWrapper, functionMainElements
        
        
        def helicsInputSetDefaultVectorMatlabWrapper(functionDict: Dict[str,str], cursorIdx: int):
            #check to see if function signiture changed
            argNum = len(functionDict.get("arguments", {}).keys())
            if argNum != 4:
                raise RuntimeError("the function signature for helicsInputSetDefaultVector has changed!")
            arg0 = functionDict.get("arguments", {}).get(0, {})
            if arg0.get("spelling","") != "ipt" or arg0.get("type", "") != "HelicsInput":
                raise RuntimeError("the function signature for helicsInputSetDefaultVector has changed!")
            arg1 = functionDict.get("arguments", {}).get(1, {})
            if arg1.get("spelling","") != "vectorInput" or arg1.get("pointer_type", "") != "Double_*":
                raise RuntimeError("the function signature for helicsInputSetDefaultVector has changed!")
            arg2 = functionDict.get("arguments", {}).get(2, {})
            if arg2.get("spelling","") != "vectorLength" or arg2.get("type", "") != "Int":
                raise RuntimeError("the function signature for helicsInputSetDefaultVector has changed!")
            arg3 = functionDict.get("arguments", {}).get(3, {})
            if arg3.get("spelling","") != "err" or arg3.get("pointer_type", "") != "HelicsError_*":
                raise RuntimeError("the function signature for helicsInputSetDefaultVector has changed!")
            functionName = functionDict.get("spelling","")
            functionComment = "%\tSet the default as a list of floats.\n\n"
            functionComment += "%\t@param ipt The input to get the data for.\n"
            functionComment += "%\t@param vectorInput The default list of floating point values.\n"
            functionWrapper = f"void _wrap_{functionName}(int resc, mxArray *resv[], int argc, const mxArray *argv[])" + "{\n"
            functionWrapper += f"\tif(argc != 2){{\n"
            functionWrapper += "\t\tmexUnlock();\n"
            functionWrapper += f"\t\tmexErrMsgIdAndTxt(\"MATLAB:{functionName}:rhs\",\"This function requires 2 arguments.\");\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += initializeArgHelicsClass("HelicsInput", "ipt", 0, functionName)
            functionWrapper += f"\tif(!mxIsNumeric(argv[1])){{\n"
            functionWrapper += "\t\tmexUnlock();\n"
            functionWrapper += f"\t\tmexErrMsgIdAndTxt(\"MATLAB:{functionName}:TypeError\",\"Argument 2 must be an array of doubles.\");\n"
            functionWrapper += "\t}\n"
            functionWrapper += "\tint vectorLength =  static_cast<int>(mxGetNumberOfElements(argv[1]));\n\n"
            functionWrapper += "\tdouble *vectorInput =  static_cast<double *>(mxGetDoubles(argv[1]));\n\n"
            functionWrapper += initializeArgHelicsErrorPtr("err")
            functionWrapper += f"\t{functionName}(ipt, static_cast<const double *>(vectorInput), vectorLength, &err);\n\n"
            functionWrapper += "\tmxArray *_out = nullptr;\n"
            functionWrapper += "\tif(_out){\n"
            functionWrapper += "\t\t--resc;\n"
            functionWrapper += "\t\t*resv++ = _out;\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += f'{argHelicsErrorPtrPostFunctionCall("err")}\n'
            functionWrapper += "}\n\n\n"
            functionMainElements = f"\tcase {cursorIdx}:\n"
            functionMainElements += f"\t\t_wrap_{functionName}(resc, resv, argc, argv);\n"
            functionMainElements += f"\t\tbreak;\n"
            return functionComment, functionWrapper, functionMainElements
        
        
        def helicsInputSetDefaultComplexVectorMatlabWrapper(functionDict: Dict[str,str], cursorIdx: int):
            #check to see if function signiture changed
            argNum = len(functionDict.get("arguments", {}).keys())
            if argNum != 4:
                raise RuntimeError("the function signature for helicsInputSetDefaultComplexVector has changed!")
            arg0 = functionDict.get("arguments", {}).get(0, {})
            if arg0.get("spelling","") != "ipt" or arg0.get("type", "") != "HelicsInput":
                raise RuntimeError("the function signature for helicsInputSetDefaultComplexVector has changed!")
            arg1 = functionDict.get("arguments", {}).get(1, {})
            if arg1.get("spelling","") != "vectorInput" or arg1.get("pointer_type", "") != "Double_*":
                raise RuntimeError("the function signature for helicsInputSetDefaultComplexVector has changed!")
            arg2 = functionDict.get("arguments", {}).get(2, {})
            if arg2.get("spelling","") != "vectorLength" or arg2.get("type", "") != "Int":
                raise RuntimeError("the function signature for helicsInputSetDefaultComplexVector has changed!")
            arg3 = functionDict.get("arguments", {}).get(3, {})
            if arg3.get("spelling","") != "err" or arg3.get("pointer_type", "") != "HelicsError_*":
                raise RuntimeError("the function signature for helicsInputSetDefaultComplexVector has changed!")
            functionName = functionDict.get("spelling","")
            functionComment = "%\tSet the default as a list of floats.\n\n"
            functionComment += "%\t@param ipt The input to get the data for.\n"
            functionComment += "%\t@param vectorInput The default list of complex values.\n"
            functionWrapper = f"void _wrap_{functionName}(int resc, mxArray *resv[], int argc, const mxArray *argv[])" + "{\n"
            functionWrapper += f"\tif(argc != 2){{\n"
            functionWrapper += "\t\tmexUnlock();\n"
            functionWrapper += f"\t\tmexErrMsgIdAndTxt(\"MATLAB:{functionName}:rhs\",\"This function requires 2 arguments.\");\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += initializeArgHelicsClass("HelicsInput", "ipt", 0, functionName)
            functionWrapper += f"\tif(!mxIsComplex(argv[1])){{\n"
            functionWrapper += "\t\tmexUnlock();\n"
            functionWrapper += f"\t\tmexErrMsgIdAndTxt(\"MATLAB:{functionName}:TypeError\",\"Argument 2 must be of an array of type complex.\");\n"
            functionWrapper += "\t}\n"
            functionWrapper += "\tint vectorLength =  static_cast<int>(mxGetN(argv[1])*2);\n\n"
            functionWrapper += "\tdouble *vectorInput = static_cast<double *>(malloc(vectorLength * sizeof(double)));\n"
            functionWrapper += "\tmxComplexDouble *vals = mxGetComplexDoubles(argv[1]);\n"
            functionWrapper += "\tfor(int i=0; i<vectorLength/2; ++i){\n"
            functionWrapper += "\t\tvectorInput[2*i] = vals[i].real;\n"
            functionWrapper += "\t\tvectorInput[2*i + 1] = vals[i].imag;\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += initializeArgHelicsErrorPtr("err")
            functionWrapper += f"\t{functionName}(ipt, static_cast<const double *>(vectorInput), vectorLength, &err);\n\n"
            functionWrapper += "\tmxArray *_out = nullptr;\n"
            functionWrapper += "\tif(_out){\n"
            functionWrapper += "\t\t--resc;\n"
            functionWrapper += "\t\t*resv++ = _out;\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += f'{argHelicsErrorPtrPostFunctionCall("err")}\n'
            functionWrapper += "}\n\n\n"
            functionMainElements = f"\tcase {cursorIdx}:\n"
            functionMainElements += f"\t\t_wrap_{functionName}(resc, resv, argc, argv);\n"
            functionMainElements += f"\t\tbreak;\n"
            return functionComment, functionWrapper, functionMainElements
        
        
        def helicsMessageAppendDataMatlabWrapper(functionDict: Dict[str,str], cursorIdx: int):
            #check to see if function signiture changed
            argNum = len(functionDict.get("arguments", {}).keys())
            if argNum != 4:
                raise RuntimeError("the function signature for helicsMessageAppendData has changed!")
            arg0 = functionDict.get("arguments", {}).get(0, {})
            if arg0.get("spelling","") != "message" or arg0.get("type", "") != "HelicsMessage":
                raise RuntimeError("the function signature for helicsMessageAppendData has changed!")
            arg1 = functionDict.get("arguments", {}).get(1, {})
            if arg1.get("spelling","") != "data" or arg1.get("pointer_type", "") != "Void_*":
                raise RuntimeError("the function signature for helicsMessageAppendData has changed!")
            arg2 = functionDict.get("arguments", {}).get(2, {})
            if arg2.get("spelling","") != "inputDataLength" or arg2.get("type", "") != "Int":
                raise RuntimeError("the function signature for helicsMessageAppendData has changed!")
            arg3 = functionDict.get("arguments", {}).get(3, {})
            if arg3.get("spelling","") != "err" or arg3.get("pointer_type", "") != "HelicsError_*":
                raise RuntimeError("the function signature for helicsMessageAppendData has changed!")
            functionName = functionDict.get("spelling","")
            functionComment = "%\tAppend data to the payload.\n\n"
            functionComment += "%\t@param message The message object in question.\n"
            functionComment += "%\t@param data A string containing the message data to append.\n"
            functionWrapper = f"void _wrap_{functionName}(int resc, mxArray *resv[], int argc, const mxArray *argv[])" + "{\n"
            functionWrapper += f"\tif(argc != 2){{\n"
            functionWrapper += "\t\tmexUnlock();\n"
            functionWrapper += f"\t\tmexErrMsgIdAndTxt(\"MATLAB:{functionName}:rhs\",\"This function requires 2 arguments.\");\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += initializeArgHelicsClass("HelicsMessage", "message", 0, functionName)
            functionWrapper += initializeArgChar("data", 1, functionName)
            functionWrapper += "\tint inputDataLength = static_cast<int>(dataLength) - 1;\n\n"
            functionWrapper += initializeArgHelicsErrorPtr("err")
            functionWrapper += f"\t{functionName}(message, static_cast<void *>(data), inputDataLength, &err);\n\n"
            functionWrapper += "\tmxArray *_out = nullptr;\n"
            functionWrapper += "\tif(_out){\n"
            functionWrapper += "\t\t--resc;\n"
            functionWrapper += "\t\t*resv++ = _out;\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += f'{argHelicsErrorPtrPostFunctionCall("err")}\n'
            functionWrapper += "}\n\n\n"
            functionMainElements = f"\tcase {cursorIdx}:\n"
            functionMainElements += f"\t\t_wrap_{functionName}(resc, resv, argc, argv);\n"
            functionMainElements += f"\t\tbreak;\n"
            return functionComment, functionWrapper, functionMainElements
        
        
        def helicsMessageGetBytesMatlabWrapper(functionDict: Dict[str,str], cursorIdx: int):
            #check to see if function signiture changed
            argNum = len(functionDict.get("arguments", {}).keys())
            if argNum != 5:
                raise RuntimeError("the function signature for helicsMessageGetBytes has changed!")
            arg0 = functionDict.get("arguments", {}).get(0, {})
            if arg0.get("spelling","") != "message" or arg0.get("type", "") != "HelicsMessage":
                raise RuntimeError("the function signature for helicsMessageGetBytes has changed!")
            arg1 = functionDict.get("arguments", {}).get(1, {})
            if arg1.get("spelling","") != "data" or arg1.get("pointer_type", "") != "Void_*":
                raise RuntimeError("the function signature for helicsMessageGetBytes has changed!")
            arg2 = functionDict.get("arguments", {}).get(2, {})
            if arg2.get("spelling","") != "maxMessageLength" or arg2.get("type", "") != "Int":
                raise RuntimeError("the function signature for helicsMessageGetBytes has changed!")
            arg3 = functionDict.get("arguments", {}).get(3, {})
            if arg3.get("spelling","") != "actualSize" or arg3.get("pointer_type", "") != "Int_*":
                raise RuntimeError("the function signature for helicsMessageGetBytes has changed!")
            arg4 = functionDict.get("arguments", {}).get(4, {})
            if arg4.get("spelling","") != "err" or arg4.get("pointer_type", "") != "HelicsError_*":
                raise RuntimeError("the function signature for helicsMessageGetBytes has changed!")
            functionName = functionDict.get("spelling","")
            functionComment = "%\tGet the raw data for a message object.\n\n"
            functionComment += "%\t@param message A message object to get the data for.\n\n"
            functionComment += "%\t@return Raw string data.\n"
            functionWrapper = f"void _wrap_{functionName}(int resc, mxArray *resv[], int argc, const mxArray *argv[])" + "{\n"
            functionWrapper += f"\tif(argc != 1){{\n"
            functionWrapper += "\t\tmexUnlock();\n"
            functionWrapper += f"\t\tmexErrMsgIdAndTxt(\"MATLAB:{functionName}:rhs\",\"This function requires 1 arguments.\");\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += initializeArgHelicsClass("HelicsMessage", "message", 0, functionName)
            functionWrapper += "\tint maxMessageLength = helicsMessageGetByteCount(message) + 2;\n\n"
            functionWrapper += "\tchar *data = static_cast<char *>(malloc(maxMessageLength));\n\n"
            functionWrapper += "\tint actualSize = 0;\n\n"
            functionWrapper += initializeArgHelicsErrorPtr("err")
            functionWrapper += f"\t{functionName}(message, static_cast<void *>(data), maxMessageLength, &actualSize, &err);\n\n"
            functionWrapper += "\tmwSize dims[2] = {1,static_cast<mwSize>(actualSize)};\n"
            functionWrapper += "\tmxArray *_out = mxCreateCharArray(2,dims);\n"
            functionWrapper += "\tmxChar *out_data = static_cast<mxChar*>(mxGetData(_out));\n"
            functionWrapper += "\tfor(int i=0; i<actualSize; ++i){\n"
            functionWrapper += "\t\tout_data[i] = data[i];\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += "\tif(_out){\n"
            functionWrapper += "\t\t--resc;\n"
            functionWrapper += "\t\t*resv++ = _out;\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += f'{argCharPostFunctionCall("data")}\n\n'
            functionWrapper += f'{argHelicsErrorPtrPostFunctionCall("err")}\n'
            functionWrapper += "}\n\n\n"
            functionMainElements = f"\tcase {cursorIdx}:\n"
            functionMainElements += f"\t\t_wrap_{functionName}(resc, resv, argc, argv);\n"
            functionMainElements += f"\t\tbreak;\n"
            return functionComment, functionWrapper, functionMainElements
        
        
        def helicsMessageSetDataMatlabWrapper(functionDict: Dict[str,str], cursorIdx: int):
            #check to see if function signiture changed
            argNum = len(functionDict.get("arguments", {}).keys())
            if argNum != 4:
                raise RuntimeError("the function signature for helicsMessageSetData has changed!")
            arg0 = functionDict.get("arguments", {}).get(0, {})
            if arg0.get("spelling","") != "message" or arg0.get("type", "") != "HelicsMessage":
                raise RuntimeError("the function signature for helicsMessageSetData has changed!")
            arg1 = functionDict.get("arguments", {}).get(1, {})
            if arg1.get("spelling","") != "data" or arg1.get("pointer_type", "") != "Void_*":
                raise RuntimeError("the function signature for helicsMessageSetData has changed!")
            arg2 = functionDict.get("arguments", {}).get(2, {})
            if arg2.get("spelling","") != "inputDataLength" or arg2.get("type", "") != "Int":
                raise RuntimeError("the function signature for helicsMessageSetData has changed!")
            arg3 = functionDict.get("arguments", {}).get(3, {})
            if arg3.get("spelling","") != "err" or arg3.get("pointer_type", "") != "HelicsError_*":
                raise RuntimeError("the function signature for helicsMessageSetData has changed!")
            functionName = functionDict.get("spelling","")
            functionComment = "%\tSet the data payload of a message as raw data.\n\n"
            functionComment += "%\t@param message The message object in question.\n"
            functionComment += "%\t@param data A string containing the message data.\n"
            functionWrapper = f"void _wrap_{functionName}(int resc, mxArray *resv[], int argc, const mxArray *argv[])" + "{\n"
            functionWrapper += f"\tif(argc != 2){{\n"
            functionWrapper += "\t\tmexUnlock();\n"
            functionWrapper += f"\t\tmexErrMsgIdAndTxt(\"MATLAB:{functionName}:rhs\",\"This function requires 2 arguments.\");\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += initializeArgHelicsClass("HelicsMessage", "message", 0, functionName)
            functionWrapper += initializeArgChar("data", 1, functionName)
            functionWrapper += "\tint inputDataLength = static_cast<int>(dataLength) - 1;\n\n"
            functionWrapper += initializeArgHelicsErrorPtr("err")
            functionWrapper += f"\t{functionName}(message, static_cast<void *>(data), inputDataLength, &err);\n\n"
            functionWrapper += "\tmxArray *_out = nullptr;\n\n"
            functionWrapper += "\tif(_out){\n"
            functionWrapper += "\t\t--resc;\n"
            functionWrapper += "\t\t*resv++ = _out;\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += f'{argHelicsErrorPtrPostFunctionCall("err")}\n'
            functionWrapper += "}\n\n\n"
            functionMainElements = f"\tcase {cursorIdx}:\n"
            functionMainElements += f"\t\t_wrap_{functionName}(resc, resv, argc, argv);\n"
            functionMainElements += f"\t\tbreak;\n"
            return functionComment, functionWrapper, functionMainElements
        
        
        def helicsPublicationPublishBytesMatlabWrapper(functionDict: Dict[str,str], cursorIdx: int):
            #check to see if function signiture changed
            argNum = len(functionDict.get("arguments", {}).keys())
            if argNum != 4:
                raise RuntimeError("the function signature for helicsPublicationPublishBytes has changed!")
            arg0 = functionDict.get("arguments", {}).get(0, {})
            if arg0.get("spelling","") != "pub" or arg0.get("type", "") != "HelicsPublication":
                raise RuntimeError("the function signature for helicsPublicationPublishBytes has changed!")
            arg1 = functionDict.get("arguments", {}).get(1, {})
            if arg1.get("spelling","") != "data" or arg1.get("pointer_type", "") != "Void_*":
                raise RuntimeError("the function signature for helicsPublicationPublishBytes has changed!")
            arg2 = functionDict.get("arguments", {}).get(2, {})
            if arg2.get("spelling","") != "inputDataLength" or arg2.get("type", "") != "Int":
                raise RuntimeError("the function signature for helicsPublicationPublishBytes has changed!")
            arg3 = functionDict.get("arguments", {}).get(3, {})
            if arg3.get("spelling","") != "err" or arg3.get("pointer_type", "") != "HelicsError_*":
                raise RuntimeError("the function signature for helicsPublicationPublishBytes has changed!")
            functionName = functionDict.get("spelling","")
            functionComment = "%\tPublish raw data.\n\n"
            functionComment += "%\t@param pub The publication to publish for.\n"
            functionComment += "%\t@param data the raw byte data to publish.\n"
            functionWrapper = f"void _wrap_{functionName}(int resc, mxArray *resv[], int argc, const mxArray *argv[])" + "{\n"
            functionWrapper += f"\tif(argc != 2){{\n"
            functionWrapper += "\t\tmexUnlock();\n"
            functionWrapper += f"\t\tmexErrMsgIdAndTxt(\"MATLAB:{functionName}:rhs\",\"This function requires 2 arguments.\");\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += initializeArgHelicsClass("HelicsPublication", "pub", 0, functionName)
            functionWrapper += initializeArgChar("data", 1, functionName)
            functionWrapper += "\tint inputDataLength = static_cast<int>(dataLength) - 1;\n\n"
            functionWrapper += initializeArgHelicsErrorPtr("err")
            functionWrapper += f"\t{functionName}(pub, static_cast<void *>(data), inputDataLength, &err);\n\n"
            functionWrapper += "\tmxArray *_out = nullptr;\n\n"
            functionWrapper += "\tif(_out){\n"
            functionWrapper += "\t\t--resc;\n"
            functionWrapper += "\t\t*resv++ = _out;\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += f'{argHelicsErrorPtrPostFunctionCall("err")}\n'
            functionWrapper += "}\n\n\n"
            functionMainElements = f"\tcase {cursorIdx}:\n"
            functionMainElements += f"\t\t_wrap_{functionName}(resc, resv, argc, argv);\n"
            functionMainElements += f"\t\tbreak;\n"
            return functionComment, functionWrapper, functionMainElements
        
        
        def helicsPublicationPublishComplexMatlabWrapper(functionDict: Dict[str,str], cursorIdx: int):
            #check to see if function signiture changed
            argNum = len(functionDict.get("arguments", {}).keys())
            if argNum != 4:
                raise RuntimeError("the function signature for helicsPublicationPublishComplex has changed!")
            arg0 = functionDict.get("arguments", {}).get(0, {})
            if arg0.get("spelling","") != "pub" or arg0.get("type", "") != "HelicsPublication":
                raise RuntimeError("the function signature for helicsPublicationPublishComplex has changed!")
            arg1 = functionDict.get("arguments", {}).get(1, {})
            if arg1.get("spelling","") != "real" or arg1.get("type", "") != "Double":
                raise RuntimeError("the function signature for helicsPublicationPublishComplex has changed!")
            arg2 = functionDict.get("arguments", {}).get(2, {})
            if arg2.get("spelling","") != "imag" or arg2.get("type", "") != "Double":
                raise RuntimeError("the function signature for helicsPublicationPublishComplex has changed!")
            arg3 = functionDict.get("arguments", {}).get(3, {})
            if arg3.get("spelling","") != "err" or arg3.get("pointer_type", "") != "HelicsError_*":
                raise RuntimeError("the function signature for helicsPublicationPublishComplex has changed!")
            functionName = functionDict.get("spelling","")
            functionComment = "%\tPublish a complex number.\n\n"
            functionComment += "%\t@param pub The publication to publish for.\n"
            functionComment += "%\t@param value The complex number.\n"
            functionWrapper = f"void _wrap_{functionName}(int resc, mxArray *resv[], int argc, const mxArray *argv[])" + "{\n"
            functionWrapper += f"\tif(argc != 2){{\n"
            functionWrapper += "\t\tmexUnlock();\n"
            functionWrapper += f"\t\tmexErrMsgIdAndTxt(\"MATLAB:{functionName}:rhs\",\"This function requires 2 arguments.\");\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += initializeArgHelicsClass("HelicsPublication", "pub", 0, functionName)
            functionWrapper += f"\tif(!mxIsComplex(argv[1])){{\n"
            functionWrapper += "\t\tmexUnlock();\n"
            functionWrapper += f"\t\tmexErrMsgIdAndTxt(\"MATLAB:{functionName}:TypeError\",\"Argument 2 must be of type complex.\");\n"
            functionWrapper += "\t}\n"
            functionWrapper += "\tmxComplexDouble *complexValue = mxGetComplexDoubles(argv[1]);\n"
            functionWrapper += "\tdouble value[2] = {complexValue[0].real, complexValue[0].imag};\n\n"
            functionWrapper += initializeArgHelicsErrorPtr("err")
            functionWrapper += f"\t{functionName}(pub, value[0], value[1], &err);\n\n"
            functionWrapper += "\tmxArray *_out = nullptr;\n\n"
            functionWrapper += "\tif(_out){\n"
            functionWrapper += "\t\t--resc;\n"
            functionWrapper += "\t\t*resv++ = _out;\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += f'{argHelicsErrorPtrPostFunctionCall("err")}\n'
            functionWrapper += "}\n\n\n"
            functionMainElements = f"\tcase {cursorIdx}:\n"
            functionMainElements += f"\t\t_wrap_{functionName}(resc, resv, argc, argv);\n"
            functionMainElements += f"\t\tbreak;\n"
            return functionComment, functionWrapper, functionMainElements
        
        
        def helicsPublicationPublishVectorMatlabWrapper(functionDict: Dict[str,str], cursorIdx: int):
            #check to see if function signiture changed
            argNum = len(functionDict.get("arguments", {}).keys())
            if argNum != 4:
                raise RuntimeError("the function signature for helicsPublicationPublishVector has changed!")
            arg0 = functionDict.get("arguments", {}).get(0, {})
            if arg0.get("spelling","") != "pub" or arg0.get("type", "") != "HelicsPublication":
                raise RuntimeError("the function signature for helicsPublicationPublishVector has changed!")
            arg1 = functionDict.get("arguments", {}).get(1, {})
            if arg1.get("spelling","") != "vectorInput" or arg1.get("pointer_type", "") != "Double_*":
                raise RuntimeError("the function signature for helicsPublicationPublishVector has changed!")
            arg2 = functionDict.get("arguments", {}).get(2, {})
            if arg2.get("spelling","") != "vectorLength" or arg2.get("type", "") != "Int":
                raise RuntimeError("the function signature for helicsPublicationPublishVector has changed!")
            arg3 = functionDict.get("arguments", {}).get(3, {})
            if arg3.get("spelling","") != "err" or arg3.get("pointer_type", "") != "HelicsError_*":
                raise RuntimeError("the function signature for helicsPublicationPublishVector has changed!")
            functionName = functionDict.get("spelling","")
            functionComment = "%\tPublish a vector of doubles.\n\n"
            functionComment += "%\t@param pub The publication to publish for.\n"
            functionComment += "%\t@param vectorInput The list of floating point values.\n"
            functionWrapper = f"void _wrap_{functionName}(int resc, mxArray *resv[], int argc, const mxArray *argv[])" + "{\n"
            functionWrapper += f"\tif(argc != 2){{\n"
            functionWrapper += "\t\tmexUnlock();\n"
            functionWrapper += f"\t\tmexErrMsgIdAndTxt(\"MATLAB:{functionName}:rhs\",\"This function requires 2 arguments.\");\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += initializeArgHelicsClass("HelicsPublication", "pub", 0, functionName)
            functionWrapper += f"\tif(!mxIsNumeric(argv[1])){{\n"
            functionWrapper += "\t\tmexUnlock();\n"
            functionWrapper += f"\t\tmexErrMsgIdAndTxt(\"MATLAB:{functionName}:TypeError\",\"Argument 2 must be an array of type double.\");\n"
            functionWrapper += "\t}\n"
            functionWrapper += "\tint vectorLength =  static_cast<int>(mxGetNumberOfElements(argv[1]));\n\n"
            functionWrapper += "\tdouble *vectorInput =  static_cast<double *>(mxGetDoubles(argv[1]));\n\n"
            functionWrapper += initializeArgHelicsErrorPtr("err")
            functionWrapper += f"\t{functionName}(pub, static_cast<const double *>(vectorInput), vectorLength, &err);\n\n"
            functionWrapper += "\tmxArray *_out = nullptr;\n"
            functionWrapper += "\tif(_out){\n"
            functionWrapper += "\t\t--resc;\n"
            functionWrapper += "\t\t*resv++ = _out;\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += f'{argHelicsErrorPtrPostFunctionCall("err")}\n'
            functionWrapper += "}\n\n\n"
            functionMainElements = f"\tcase {cursorIdx}:\n"
            functionMainElements += f"\t\t_wrap_{functionName}(resc, resv, argc, argv);\n"
            functionMainElements += f"\t\tbreak;\n"
            return functionComment, functionWrapper, functionMainElements
        
        
        def helicsPublicationPublishComplexVectorMatlabWrapper(functionDict: Dict[str,str], cursorIdx: int):
            #check to see if function signiture changed
            argNum = len(functionDict.get("arguments", {}).keys())
            if argNum != 4:
                raise RuntimeError("the function signature for helicsPublicationPublishVector has changed!")
            arg0 = functionDict.get("arguments", {}).get(0, {})
            if arg0.get("spelling","") != "pub" or arg0.get("type", "") != "HelicsPublication":
                raise RuntimeError("the function signature for helicsPublicationPublishVector has changed!")
            arg1 = functionDict.get("arguments", {}).get(1, {})
            if arg1.get("spelling","") != "vectorInput" or arg1.get("pointer_type", "") != "Double_*":
                raise RuntimeError("the function signature for helicsPublicationPublishVector has changed!")
            arg2 = functionDict.get("arguments", {}).get(2, {})
            if arg2.get("spelling","") != "vectorLength" or arg2.get("type", "") != "Int":
                raise RuntimeError("the function signature for helicsPublicationPublishVector has changed!")
            arg3 = functionDict.get("arguments", {}).get(3, {})
            if arg3.get("spelling","") != "err" or arg3.get("pointer_type", "") != "HelicsError_*":
                raise RuntimeError("the function signature for helicsPublicationPublishVector has changed!")
            functionName = functionDict.get("spelling","")
            functionComment = "%\tPublish a vector of doubles.\n\n"
            functionComment += "%\t@param pub The publication to publish for.\n"
            functionComment += "%\t@param vectorInput The list of complex values.\n"
            functionWrapper = f"void _wrap_{functionName}(int resc, mxArray *resv[], int argc, const mxArray *argv[])" + "{\n"
            functionWrapper += f"\tif(argc != 2){{\n"
            functionWrapper += "\t\tmexUnlock();\n"
            functionWrapper += f"\t\tmexErrMsgIdAndTxt(\"MATLAB:{functionName}:rhs\",\"This function requires 2 arguments.\");\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += initializeArgHelicsClass("HelicsPublication", "pub", 0, functionName)
            functionWrapper += f"\tif(!mxIsComplex(argv[1])){{\n"
            functionWrapper += "\t\tmexUnlock();\n"
            functionWrapper += f"\t\tmexErrMsgIdAndTxt(\"MATLAB:{functionName}:TypeError\",\"Argument 2 must be an array of type complex.\");\n"
            functionWrapper += "\t}\n"
            functionWrapper += "\tint vectorLength =  static_cast<int>(mxGetN(argv[1])*2);\n\n"
            functionWrapper += "\tdouble *vectorInput = static_cast<double *>(malloc(vectorLength * sizeof(double)));\n"
            functionWrapper += "\tmxComplexDouble *vals = mxGetComplexDoubles(argv[1]);\n"
            functionWrapper += "\tfor(int i=0; i<vectorLength/2; ++i){\n"
            functionWrapper += "\t\tvectorInput[2*i] = vals[i].real;\n"
            functionWrapper += "\t\tvectorInput[2*i + 1] = vals[i].imag;\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += initializeArgHelicsErrorPtr("err")
            functionWrapper += f"\t{functionName}(pub, static_cast<const double *>(vectorInput), vectorLength, &err);\n\n"
            functionWrapper += "\tmxArray *_out = nullptr;\n"
            functionWrapper += "\tif(_out){\n"
            functionWrapper += "\t\t--resc;\n"
            functionWrapper += "\t\t*resv++ = _out;\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += f'{argHelicsErrorPtrPostFunctionCall("err")}\n'
            functionWrapper += "}\n\n\n"
            functionMainElements = f"\tcase {cursorIdx}:\n"
            functionMainElements += f"\t\t_wrap_{functionName}(resc, resv, argc, argv);\n"
            functionMainElements += f"\t\tbreak;\n"
            return functionComment, functionWrapper, functionMainElements
        
        
        def helicsQueryBufferFillMatlabWrapper(functionDict: Dict[str,str], cursorIdx: int):
            #check to see if function signiture changed
            argNum = len(functionDict.get("arguments", {}).keys())
            if argNum != 4:
                raise RuntimeError("the function signature for helicsQueryBufferFill has changed!")
            arg0 = functionDict.get("arguments", {}).get(0, {})
            if arg0.get("spelling","") != "buffer" or arg0.get("type", "") != "HelicsQueryBuffer":
                raise RuntimeError("the function signature for helicsQueryBufferFill has changed!")
            arg1 = functionDict.get("arguments", {}).get(1, {})
            if arg1.get("spelling","") != "queryResult" or arg1.get("pointer_type", "") != "Char_S_*":
                raise RuntimeError("the function signature for helicsQueryBufferFill has changed!")
            arg2 = functionDict.get("arguments", {}).get(2, {})
            if arg2.get("spelling","") != "strSize" or arg2.get("type", "") != "Int":
                raise RuntimeError("the function signature for helicsQueryBufferFill has changed!")
            arg3 = functionDict.get("arguments", {}).get(3, {})
            if arg3.get("spelling","") != "err" or arg3.get("pointer_type", "") != "HelicsError_*":
                raise RuntimeError("the function signature for helicsQueryBufferFill has changed!")
            functionName = functionDict.get("spelling","")
            functionComment = "%\tSet the data for a query callback.\n\n"
            functionComment += "%\t@details There are many queries that HELICS understands directly, but it is occasionally useful to have a federate be able to respond to specific queries with answers specific to a federate.\n\n"
            functionComment += "%\t@param buffer The buffer received in a helicsQueryCallback.\n"
            functionComment += "%\t@param queryResult The string with the data to fill the buffer with.\n"
            functionWrapper = f"void _wrap_{functionName}(int resc, mxArray *resv[], int argc, const mxArray *argv[])" + "{\n"
            functionWrapper += f"\tif(argc != 2){{\n"
            functionWrapper += "\t\tmexUnlock();\n"
            functionWrapper += f"\t\tmexErrMsgIdAndTxt(\"MATLAB:{functionName}:rhs\",\"This function requires 2 arguments.\");\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += initializeArgHelicsClass("HelicsQueryBuffer", "buffer", 0, functionName)
            functionWrapper += initializeArgChar("queryResult", 1, functionName)
            functionWrapper += "\tint strSize = static_cast<int>(queryResultLength) - 1;\n\n"
            functionWrapper += initializeArgHelicsErrorPtr("err")
            functionWrapper += f"\t{functionName}(buffer, queryResult, strSize, &err);\n\n"
            functionWrapper += "\tmxArray *_out = nullptr;\n"
            functionWrapper += "\tif(_out){\n"
            functionWrapper += "\t\t--resc;\n"
            functionWrapper += "\t\t*resv++ = _out;\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += f'{argHelicsErrorPtrPostFunctionCall("err")}\n'
            functionWrapper += "}\n\n\n"
            functionMainElements = f"\tcase {cursorIdx}:\n"
            functionMainElements += f"\t\t_wrap_{functionName}(resc, resv, argc, argv);\n"
            functionMainElements += f"\t\tbreak;\n"
            return functionComment, functionWrapper, functionMainElements
        
        
        def helicsLoadSignalHandlerCallbackMatlabWrapper(functionDict: Dict[str,str], cursorIdx: int):
            return "","",""
        
        
        def helicsLoadSignalHandlerCallbackNoExitMatlabWrapper(functionDict: Dict[str,str], cursorIdx: int):
            return "","",""
        
        
        def helicsBrokerSetLoggingCallbackMatlabWrapper(functionDict: Dict[str,str], cursorIdx: int):
            #check to see if function signiture changed
            argNum = len(functionDict.get("arguments", {}).keys())
            if argNum != 4:
                raise RuntimeError("the function signature for helicsBrokerSetLoggingCallback has changed!")
            arg0 = functionDict.get("arguments", {}).get(0, {})
            if arg0.get("spelling","") != "broker" or arg0.get("type", "") != "HelicsBroker":
                raise RuntimeError("the function signature for helicsBrokerSetLoggingCallback has changed!")
            arg1 = functionDict.get("arguments", {}).get(1, {})
            if arg1.get("spelling","") != "logger" or arg1.get("pointer_type", "") != "FunctionProto_*":
                raise RuntimeError("the function signature for helicsBrokerSetLoggingCallback has changed!")
            arg2 = functionDict.get("arguments", {}).get(2, {})
            if arg2.get("spelling","") != "userdata" or arg2.get("pointer_type", "") != "Void_*":
                raise RuntimeError("the function signature for helicsBrokerSetLoggingCallback has changed!")
            arg3 = functionDict.get("arguments", {}).get(3, {})
            if arg3.get("spelling","") != "err" or arg3.get("pointer_type", "") != "HelicsError_*":
                raise RuntimeError("the function signature for helicsBrokerSetLoggingCallback has changed!")
            functionName = functionDict.get("spelling","")
            functionComment = "%\tSet the logging callback to a broker\n\n"
            functionComment += "%\t@details Add a logging callback function to a broker.\n%\t\tThe logging callback will be called when\n%\t\ta message flows into a broker from the core or from a broker.\n\n"
            functionComment += "%\t@param broker The broker object in which to set the callback.\n"
            functionComment += "%\t@param logger A function handle with the signature void(int loglevel, string identifier, string message).\n"
            functionWrapper = "void matlabBrokerLoggingCallback(int loglevel, const char* identifier, const char* message, void *userData){\n"
            functionWrapper += "\tmxArray *lhs;\n"
            functionWrapper += "\tmxArray *rhs[4];\n"
            functionWrapper += "\trhs[0] = reinterpret_cast<mxArray *>(userData);\n"
            functionWrapper += "\trhs[1] = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);\n"
            functionWrapper += "\tmxInt64 *rhs1Ptr = mxGetInt64s(rhs[1]);\n"
            functionWrapper += "\trhs1Ptr[0] = static_cast<mxInt64>(loglevel);\n"
            functionWrapper += "\trhs[2] = mxCreateString(identifier);\n"
            functionWrapper += "\trhs[3] = mxCreateString(message);\n"
            functionWrapper += '\tint status = mexCallMATLAB(0,&lhs,4,rhs,"feval");\n'
            functionWrapper += "\tmxDestroyArray(lhs);\n"
            functionWrapper += "\tmxDestroyArray(rhs[1]);\n"
            functionWrapper += "\tmxDestroyArray(rhs[2]);\n"
            functionWrapper += "\tmxDestroyArray(rhs[3]);\n"
            functionWrapper += "}\n\n"
            functionWrapper += f"void _wrap_{functionName}(int resc, mxArray *resv[], int argc, const mxArray *argv[])" + "{\n"
            functionWrapper += f"\tif(argc != 2){{\n"
            functionWrapper += "\t\tmexUnlock();\n"
            functionWrapper += f"\t\tmexErrMsgIdAndTxt(\"MATLAB:{functionName}:rhs\",\"This function requires 2 arguments.\");\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += initializeArgHelicsClass("HelicsBroker", "broker", 0, functionName)
            functionWrapper += "\tvoid *userData = mxGetData(argv[1]);\n"
            functionWrapper += initializeArgHelicsErrorPtr("err")
            functionWrapper += f"\t{functionName}(broker, &matlabBrokerLoggingCallback, userData, &err);\n\n"
            functionWrapper += "\tmxArray *_out = nullptr;\n"
            functionWrapper += "\tif(_out){\n"
            functionWrapper += "\t\t--resc;\n"
            functionWrapper += "\t\t*resv++ = _out;\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += f'{argHelicsErrorPtrPostFunctionCall("err")}\n'
            functionWrapper += "}\n\n\n"
            functionMainElements = f"\tcase {cursorIdx}:\n"
            functionMainElements += f"\t\t_wrap_{functionName}(resc, resv, argc, argv);\n"
            functionMainElements += f"\t\tbreak;\n"
            return functionComment, functionWrapper, functionMainElements
        
        
        def helicsCoreSetLoggingCallbackMatlabWrapper(functionDict: Dict[str,str], cursorIdx: int):
            #check to see if function signiture changed
            argNum = len(functionDict.get("arguments", {}).keys())
            if argNum != 4:
                raise RuntimeError("the function signature for helicsCoreSetLoggingCallback has changed!")
            arg0 = functionDict.get("arguments", {}).get(0, {})
            if arg0.get("spelling","") != "core" or arg0.get("type", "") != "HelicsCore":
                raise RuntimeError("the function signature for helicsCoreSetLoggingCallback has changed!")
            arg1 = functionDict.get("arguments", {}).get(1, {})
            if arg1.get("spelling","") != "logger" or arg1.get("pointer_type", "") != "FunctionProto_*":
                raise RuntimeError("the function signature for helicsCoreSetLoggingCallback has changed!")
            arg2 = functionDict.get("arguments", {}).get(2, {})
            if arg2.get("spelling","") != "userdata" or arg2.get("pointer_type", "") != "Void_*":
                raise RuntimeError("the function signature for helicsCoreSetLoggingCallback has changed!")
            arg3 = functionDict.get("arguments", {}).get(3, {})
            if arg3.get("spelling","") != "err" or arg3.get("pointer_type", "") != "HelicsError_*":
                raise RuntimeError("the function signature for helicsCoreSetLoggingCallback has changed!")
            functionName = functionDict.get("spelling","")
            functionComment = "%\tSet the logging callback to a core\n\n"
            functionComment += "%\t@details Add a logging callback function to a core.\n%\t\tThe logging callback will be called when\n%\t\ta message flows into a core from the core or from a broker.\n\n"
            functionComment += "%\t@param core The core object in which to set the callback.\n"
            functionComment += "%\t@param logger A function handle with the signature void(int loglevel, string identifier, string message).\n"
            functionWrapper = "void matlabCoreLoggingCallback(int loglevel, const char* identifier, const char* message, void *userData){\n"
            functionWrapper += "\tmxArray *lhs;\n"
            functionWrapper += "\tmxArray *rhs[4];\n"
            functionWrapper += "\trhs[0] = reinterpret_cast<mxArray *>(userData);\n"
            functionWrapper += "\trhs[1] = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);\n"
            functionWrapper += "\tmxInt64 *rhs1Ptr = mxGetInt64s(rhs[1]);\n"
            functionWrapper += "\trhs1Ptr[0] = static_cast<mxInt64>(loglevel);\n"
            functionWrapper += "\trhs[2] = mxCreateString(identifier);\n"
            functionWrapper += "\trhs[3] = mxCreateString(message);\n"
            functionWrapper += '\tint status = mexCallMATLAB(0,&lhs,4,rhs,"feval");\n'
            functionWrapper += "\tmxDestroyArray(lhs);\n"
            functionWrapper += "\tmxDestroyArray(rhs[1]);\n"
            functionWrapper += "\tmxDestroyArray(rhs[2]);\n"
            functionWrapper += "\tmxDestroyArray(rhs[3]);\n"
            functionWrapper += "}\n\n"
            functionWrapper += f"void _wrap_{functionName}(int resc, mxArray *resv[], int argc, const mxArray *argv[])" + "{\n"
            functionWrapper += f"\tif(argc != 2){{\n"
            functionWrapper += "\t\tmexUnlock();\n"
            functionWrapper += f"\t\tmexErrMsgIdAndTxt(\"MATLAB:{functionName}:rhs\",\"This function requires 2 arguments.\");\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += initializeArgHelicsClass("HelicsCore", "core", 0, functionName)
            functionWrapper += "\tvoid *userData = mxGetData(argv[1]);\n"
            functionWrapper += initializeArgHelicsErrorPtr("err")
            functionWrapper += f"\t{functionName}(core, &matlabCoreLoggingCallback, userData, &err);\n\n"
            functionWrapper += "\tmxArray *_out = nullptr;\n"
            functionWrapper += "\tif(_out){\n"
            functionWrapper += "\t\t--resc;\n"
            functionWrapper += "\t\t*resv++ = _out;\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += f'{argHelicsErrorPtrPostFunctionCall("err")}\n'
            functionWrapper += "}\n\n\n"
            functionMainElements = f"\tcase {cursorIdx}:\n"
            functionMainElements += f"\t\t_wrap_{functionName}(resc, resv, argc, argv);\n"
            functionMainElements += f"\t\tbreak;\n"
            return functionComment, functionWrapper, functionMainElements
        
        
        def helicsFederateSetLoggingCallbackMatlabWrapper(functionDict: Dict[str,str], cursorIdx: int):
            #check to see if function signiture changed
            argNum = len(functionDict.get("arguments", {}).keys())
            if argNum != 4:
                raise RuntimeError("the function signature for helicsFederateSetLoggingCallback has changed!")
            arg0 = functionDict.get("arguments", {}).get(0, {})
            if arg0.get("spelling","") != "fed" or arg0.get("type", "") != "HelicsFederate":
                raise RuntimeError("the function signature for helicsFederateSetLoggingCallback has changed!")
            arg1 = functionDict.get("arguments", {}).get(1, {})
            if arg1.get("spelling","") != "logger" or arg1.get("pointer_type", "") != "FunctionProto_*":
                raise RuntimeError("the function signature for helicsFederateSetLoggingCallback has changed!")
            arg2 = functionDict.get("arguments", {}).get(2, {})
            if arg2.get("spelling","") != "userdata" or arg2.get("pointer_type", "") != "Void_*":
                raise RuntimeError("the function signature for helicsFederateSetLoggingCallback has changed!")
            arg3 = functionDict.get("arguments", {}).get(3, {})
            if arg3.get("spelling","") != "err" or arg3.get("pointer_type", "") != "HelicsError_*":
                raise RuntimeError("the function signature for helicsFederateSetLoggingCallback has changed!")
            functionName = functionDict.get("spelling","")
            functionComment = "%\tSet the logging callback for a federate\n\n"
            functionComment += "%\t@details Add a logging callback function for a federate.\n%\t\tThe logging callback will be called when\n%\t\ta message flows into a federate from the core or from a federate.\n\n"
            functionComment += "%\t@param fed The federate object in which to set the callback.\n"
            functionComment += "%\t@param logger A function handle with the signature void(int loglevel, string identifier, string message).\n"
            functionWrapper = "void matlabFederateLoggingCallback(int loglevel, const char* identifier, const char* message, void *userData){\n"
            functionWrapper += "\tmxArray *lhs;\n"
            functionWrapper += "\tmxArray *rhs[4];\n"
            functionWrapper += "\trhs[0] = reinterpret_cast<mxArray *>(userData);\n"
            functionWrapper += "\trhs[1] = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);\n"
            functionWrapper += "\tmxInt64 *rhs1Ptr = mxGetInt64s(rhs[1]);\n"
            functionWrapper += "\trhs1Ptr[0] = static_cast<mxInt64>(loglevel);\n"
            functionWrapper += "\trhs[2] = mxCreateString(identifier);\n"
            functionWrapper += "\trhs[3] = mxCreateString(message);\n"
            functionWrapper += '\tint status = mexCallMATLAB(0,&lhs,4,rhs,"feval");\n'
            functionWrapper += "\tmxDestroyArray(lhs);\n"
            functionWrapper += "\tmxDestroyArray(rhs[1]);\n"
            functionWrapper += "\tmxDestroyArray(rhs[2]);\n"
            functionWrapper += "\tmxDestroyArray(rhs[3]);\n"
            functionWrapper += "}\n\n"
            functionWrapper += f"void _wrap_{functionName}(int resc, mxArray *resv[], int argc, const mxArray *argv[])" + "{\n"
            functionWrapper += f"\tif(argc != 2){{\n"
            functionWrapper += "\t\tmexUnlock();\n"
            functionWrapper += f"\t\tmexErrMsgIdAndTxt(\"MATLAB:{functionName}:rhs\",\"This function requires 2 arguments.\");\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += initializeArgHelicsClass("HelicsFederate", "fed", 0, functionName)
            functionWrapper += "\tvoid *userData = mxGetData(argv[1]);\n"
            functionWrapper += initializeArgHelicsErrorPtr("err")
            functionWrapper += f"\t{functionName}(fed, &matlabFederateLoggingCallback, userData, &err);\n\n"
            functionWrapper += "\tmxArray *_out = nullptr;\n"
            functionWrapper += "\tif(_out){\n"
            functionWrapper += "\t\t--resc;\n"
            functionWrapper += "\t\t*resv++ = _out;\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += f'{argHelicsErrorPtrPostFunctionCall("err")}\n'
            functionWrapper += "}\n\n\n"
            functionMainElements = f"\tcase {cursorIdx}:\n"
            functionMainElements += f"\t\t_wrap_{functionName}(resc, resv, argc, argv);\n"
            functionMainElements += f"\t\tbreak;\n"
            return functionComment, functionWrapper, functionMainElements
        
        
        def helicsFilterSetCustomCallbackMatlabWrapper(functionDict: Dict[str,str], cursorIdx: int):
            #check to see if function signiture changed
            argNum = len(functionDict.get("arguments", {}).keys())
            if argNum != 4:
                raise RuntimeError("the function signature for helicsFilterSetCustomCallback has changed!")
            arg0 = functionDict.get("arguments", {}).get(0, {})
            if arg0.get("spelling","") != "filter" or arg0.get("type", "") != "HelicsFilter":
                raise RuntimeError("the function signature for helicsFilterSetCustomCallback has changed!")
            arg1 = functionDict.get("arguments", {}).get(1, {})
            if arg1.get("spelling","") != "filtCall" or arg1.get("pointer_type", "") != "FunctionProto_*":
                raise RuntimeError("the function signature for helicsFilterSetCustomCallback has changed!")
            arg2 = functionDict.get("arguments", {}).get(2, {})
            if arg2.get("spelling","") != "userdata" or arg2.get("pointer_type", "") != "Void_*":
                raise RuntimeError("the function signature for helicsFilterSetCustomCallback has changed!")
            arg3 = functionDict.get("arguments", {}).get(3, {})
            if arg3.get("spelling","") != "err" or arg3.get("pointer_type", "") != "HelicsError_*":
                raise RuntimeError("the function signature for helicsFilterSetCustomCallback has changed!")
            functionName = functionDict.get("spelling","")
            functionComment = "%\tSet a general callback for a custom filter.\n\n"
            functionComment += "%\t@details Add a custom filter callback function for creating a custom filter operation in the c shared library.\n\n"
            functionComment += "%\t@param filter The filter object in which to set the callback.\n"
            functionComment += "%\t@param filtCall A function handle with the signature HelicsMessage(HelicsMessage message).\n"
            functionWrapper = "HelicsMessage matlabFilterCustomCallback(HelicsMessage message, void *userData){\n"
            functionWrapper += "\tmxArray *lhs[1];\n"
            functionWrapper += "\tmxArray *rhs[2];\n"
            functionWrapper += "\trhs[0] = reinterpret_cast<mxArray *>(userData);\n"
            functionWrapper += "\trhs[1] = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);\n"
            functionWrapper += "\tmxUint64 *rhs1Ptr = mxGetUint64s(rhs[1]);\n"
            functionWrapper += "\trhs1Ptr[0] = reinterpret_cast<mxUint64>(message);\n"
            functionWrapper += '\tint status = mexCallMATLAB(1,lhs,2,rhs,"feval");\n'
            functionWrapper += "\tHelicsMessage rv = static_cast<HelicsMessage>(mxGetData(lhs[0]));\n"
            functionWrapper += "\tmxDestroyArray(rhs[1]);\n"
            functionWrapper += "\treturn rv;\n"
            functionWrapper += "}\n\n"
            functionWrapper += f"void _wrap_{functionName}(int resc, mxArray *resv[], int argc, const mxArray *argv[])" + "{\n"
            functionWrapper += f"\tif(argc != 2){{\n"
            functionWrapper += "\t\tmexUnlock();\n"
            functionWrapper += f"\t\tmexErrMsgIdAndTxt(\"MATLAB:{functionName}:rhs\",\"This function requires 2 arguments.\");\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += initializeArgHelicsClass("HelicsFilter", "filter", 0, functionName)
            functionWrapper += "\tvoid *userData = mxGetData(argv[1]);\n"
            functionWrapper += initializeArgHelicsErrorPtr("err")
            functionWrapper += f"\t{functionName}(filter, &matlabFilterCustomCallback, userData, &err);\n\n"
            functionWrapper += "\tmxArray *_out = nullptr;\n"
            functionWrapper += "\tif(_out){\n"
            functionWrapper += "\t\t--resc;\n"
            functionWrapper += "\t\t*resv++ = _out;\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += f'{argHelicsErrorPtrPostFunctionCall("err")}\n'
            functionWrapper += "}\n\n\n"
            functionMainElements = f"\tcase {cursorIdx}:\n"
            functionMainElements += f"\t\t_wrap_{functionName}(resc, resv, argc, argv);\n"
            functionMainElements += f"\t\tbreak;\n"
            return functionComment, functionWrapper, functionMainElements
        
        
        def helicsFederateSetQueryCallbackMatlabWrapper(functionDict: Dict[str,str], cursorIdx: int):
            #check to see if function signiture changed
            argNum = len(functionDict.get("arguments", {}).keys())
            if argNum != 4:
                raise RuntimeError("the function signature for helicsFederateSetQueryCallback has changed!")
            arg0 = functionDict.get("arguments", {}).get(0, {})
            if arg0.get("spelling","") != "fed" or arg0.get("type", "") != "HelicsFederate":
                raise RuntimeError("the function signature for helicsFederateSetQueryCallback has changed!")
            arg1 = functionDict.get("arguments", {}).get(1, {})
            if arg1.get("spelling","") != "queryAnswer" or arg1.get("pointer_type", "") != "FunctionProto_*":
                raise RuntimeError("the function signature for helicsFederateSetQueryCallback has changed!")
            arg2 = functionDict.get("arguments", {}).get(2, {})
            if arg2.get("spelling","") != "userdata" or arg2.get("pointer_type", "") != "Void_*":
                raise RuntimeError("the function signature for helicsFederateSetQueryCallback has changed!")
            arg3 = functionDict.get("arguments", {}).get(3, {})
            if arg3.get("spelling","") != "err" or arg3.get("pointer_type", "") != "HelicsError_*":
                raise RuntimeError("the function signature for helicsFederateSetQueryCallback has changed!")
            functionName = functionDict.get("spelling","")
            functionComment = "%\tSet callback for queries executed against a federate.\n\n"
            functionComment += "%\t@details There are many queries that HELICS understands directly, but it is occasionally useful to a have a federate be able to respond\n%\ttospecific queries with answers specific to a federate.\n\n"
            functionComment += "%\t@param fed The federate object in which to set the callback.\n"
            functionComment += "%\t@param queryAnswer A function handle with the signature const void(const char *query, int querySize, HelicsQueryBuffer buffer).\n"
            functionWrapper = "void matlabFederateQueryCallback(const char* query, int querySize, HelicsQueryBuffer buffer, void *userData){\n"
            functionWrapper += "\tmxArray *lhs;\n"
            functionWrapper += "\tmxArray *rhs[4];\n"
            functionWrapper += "\tmwSize dims[2] = {1, static_cast<mwSize>(querySize)};\n"
            functionWrapper += "\trhs[0] = reinterpret_cast<mxArray *>(userData);\n"
            functionWrapper += "\trhs[1] = mxCreateCharArray(2, dims);\n"
            functionWrapper += "\tmxChar *pQuery = static_cast<mxChar *>(mxGetData(rhs[1]));\n"
            functionWrapper += "\tfor(int i=0; i<querySize; ++i){\n"
            functionWrapper += "\t\tpQuery[i] = query[i];\n"
            functionWrapper += "\t}\n"
            functionWrapper += "\trhs[2] = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);\n"
            functionWrapper += "\tmxInt64 *rhs2Ptr = mxGetInt64s(rhs[2]);\n"
            functionWrapper += "\trhs2Ptr[0] = static_cast<mxInt64>(querySize);\n"
            functionWrapper += "\trhs[3] = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);\n"
            functionWrapper += "\tmxUint64 *rhs3Ptr = mxGetUint64s(rhs[3]);\n"
            functionWrapper += "\trhs3Ptr[0] = reinterpret_cast<mxUint64>(buffer);\n"
            functionWrapper += '\tint status = mexCallMATLAB(0,&lhs,4,rhs,"feval");\n'
            functionWrapper += "\tmxDestroyArray(rhs[1]);\n"
            functionWrapper += "\tmxDestroyArray(rhs[2]);\n"
            functionWrapper += "\tmxDestroyArray(rhs[3]);\n"
            functionWrapper += "}\n\n"
            functionWrapper += f"void _wrap_{functionName}(int resc, mxArray *resv[], int argc, const mxArray *argv[])" + "{\n"
            functionWrapper += f"\tif(argc != 2){{\n"
            functionWrapper += "\t\tmexUnlock();\n"
            functionWrapper += f"\t\tmexErrMsgIdAndTxt(\"MATLAB:{functionName}:rhs\",\"This function requires 2 arguments.\");\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += initializeArgHelicsClass("HelicsFilter", "filter", 0, functionName)
            functionWrapper += "\tvoid *userData = mxGetData(argv[1]);\n"
            functionWrapper += initializeArgHelicsErrorPtr("err")
            functionWrapper += f"\t{functionName}(filter, &matlabFederateQueryCallback, userData, &err);\n\n"
            functionWrapper += "\tmxArray *_out = nullptr;\n"
            functionWrapper += "\tif(_out){\n"
            functionWrapper += "\t\t--resc;\n"
            functionWrapper += "\t\t*resv++ = _out;\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += f'{argHelicsErrorPtrPostFunctionCall("err")}\n'
            functionWrapper += "}\n\n\n"
            functionMainElements = f"\tcase {cursorIdx}:\n"
            functionMainElements += f"\t\t_wrap_{functionName}(resc, resv, argc, argv);\n"
            functionMainElements += f"\t\tbreak;\n"
            return functionComment, functionWrapper, functionMainElements
        
        
        def helicsFederateSetTimeUpdateCallbackMatlabWrapper(functionDict: Dict[str,str], cursorIdx: int):
            #check to see if function signiture changed
            argNum = len(functionDict.get("arguments", {}).keys())
            if argNum != 4:
                raise RuntimeError("the function signature for helicsFederateSetTimeUpdateCallback has changed!")
            arg0 = functionDict.get("arguments", {}).get(0, {})
            if arg0.get("spelling","") != "fed" or arg0.get("type", "") != "HelicsFederate":
                raise RuntimeError("the function signature for helicsFederateSetTimeUpdateCallback has changed!")
            arg1 = functionDict.get("arguments", {}).get(1, {})
            if arg1.get("spelling","") != "timeUpdate" or arg1.get("pointer_type", "") != "FunctionProto_*":
                raise RuntimeError("the function signature for helicsFederateSetTimeUpdateCallback has changed!")
            arg2 = functionDict.get("arguments", {}).get(2, {})
            if arg2.get("spelling","") != "userdata" or arg2.get("pointer_type", "") != "Void_*":
                raise RuntimeError("the function signature for helicsFederateSetTimeUpdateCallback has changed!")
            arg3 = functionDict.get("arguments", {}).get(3, {})
            if arg3.get("spelling","") != "err" or arg3.get("pointer_type", "") != "HelicsError_*":
                raise RuntimeError("the function signature for helicsFederateSetTimeUpdateCallback has changed!")
            functionName = functionDict.get("spelling","")
            functionComment = "%\tSet callback for the federate time update.n\n"
            functionComment += "%\t@details This callback will be executed every time the simulation time is updated starting on entry to executing mode.\n\n"
            functionComment += "%\t@param fed The federate object in which to set the callback.\n"
            functionComment += "%\t@param timeUpdate A function handle with the signature void(double newTime, int iterating).\n"
            functionWrapper = "void matlabFederateTimeUpdateCallback(HelicsTime newTime, HelicsBool iterating, void *userData){\n"
            functionWrapper += "\tmxArray *lhs;\n"
            functionWrapper += "\tmxArray *rhs[3];\n"
            functionWrapper += "\trhs[0] = reinterpret_cast<mxArray *>(userData);\n"
            functionWrapper += "\trhs[1] = mxCreateDoubleScalar(static_cast<double>(newTime));\n"
            functionWrapper += "\trhs[2] = mxCreateLogicalMatrix(1, 1);\n"
            functionWrapper += "\tmxLogical *rhs2Ptr = mxGetLogicals(rhs[2]);\n"
            functionWrapper += "\trhs2Ptr[0] = false;\n"
            functionWrapper += "\tif(iterating == HELICS_TRUE){\n"
            functionWrapper += "\t\trhs2Ptr[0] = true;\n"
            functionWrapper += "\t}\n"
            functionWrapper += '\tint status = mexCallMATLAB(0,&lhs,3,rhs,"feval");\n'
            functionWrapper += "\tmxDestroyArray(lhs);\n"
            functionWrapper += "\tmxDestroyArray(rhs[1]);\n"
            functionWrapper += "\tmxDestroyArray(rhs[2]);\n"
            functionWrapper += "}\n\n"
            functionWrapper += f"void _wrap_{functionName}(int resc, mxArray *resv[], int argc, const mxArray *argv[])" + "{\n"
            functionWrapper += f"\tif(argc != 2){{\n"
            functionWrapper += "\t\tmexUnlock();\n"
            functionWrapper += f"\t\tmexErrMsgIdAndTxt(\"MATLAB:{functionName}:rhs\",\"This function requires 2 arguments.\");\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += initializeArgHelicsClass("HelicsFederate", "fed", 0, functionName)
            functionWrapper += "\tvoid *userData = mxGetData(argv[1]);\n"
            functionWrapper += initializeArgHelicsErrorPtr("err")
            functionWrapper += f"\t{functionName}(fed, &matlabFederateTimeUpdateCallback, userData, &err);\n\n"
            functionWrapper += "\tmxArray *_out = nullptr;\n"
            functionWrapper += "\tif(_out){\n"
            functionWrapper += "\t\t--resc;\n"
            functionWrapper += "\t\t*resv++ = _out;\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += f'{argHelicsErrorPtrPostFunctionCall("err")}\n'
            functionWrapper += "}\n\n\n"
            functionMainElements = f"\tcase {cursorIdx}:\n"
            functionMainElements += f"\t\t_wrap_{functionName}(resc, resv, argc, argv);\n"
            functionMainElements += f"\t\tbreak;\n"
            return functionComment, functionWrapper, functionMainElements
        
        
        def helicsFederateSetStateChangeCallbackMatlabWrapper(functionDict: Dict[str,str], cursorIdx: int):
            #check to see if function signiture changed
            argNum = len(functionDict.get("arguments", {}).keys())
            if argNum != 4:
                raise RuntimeError("the function signature for helicsFederateSetStateChangeCallback has changed!")
            arg0 = functionDict.get("arguments", {}).get(0, {})
            if arg0.get("spelling","") != "fed" or arg0.get("type", "") != "HelicsFederate":
                raise RuntimeError("the function signature for helicsFederateSetStateChangeCallback has changed!")
            arg1 = functionDict.get("arguments", {}).get(1, {})
            if arg1.get("spelling","") != "stateChange" or arg1.get("pointer_type", "") != "FunctionProto_*":
                raise RuntimeError("the function signature for helicsFederateSetStateChangeCallback has changed!")
            arg2 = functionDict.get("arguments", {}).get(2, {})
            if arg2.get("spelling","") != "userdata" or arg2.get("pointer_type", "") != "Void_*":
                raise RuntimeError("the function signature for helicsFederateSetStateChangeCallback has changed!")
            arg3 = functionDict.get("arguments", {}).get(3, {})
            if arg3.get("spelling","") != "err" or arg3.get("pointer_type", "") != "HelicsError_*":
                raise RuntimeError("the function signature for helicsFederateSetStateChangeCallback has changed!")
            functionName = functionDict.get("spelling","")
            functionComment = "%\tSet callback for the federate mode change.n\n"
            functionComment += "%\t@details This callback will be executed every time the operating mode of the federate changes.\n\n"
            functionComment += "%\t@param fed The federate object in which to set the callback.\n"
            functionComment += "%\t@param stateChange A function handle with the signature void(int newState, int oldState).\n"
            functionWrapper = "void matlabFederateSetStateChangeCallback(HelicsFederateState newState, HelicsFederateState oldState, void *userData){\n"
            functionWrapper += "\tmxArray *lhs;\n"
            functionWrapper += "\tmxArray *rhs[3];\n"
            functionWrapper += "\trhs[0] = reinterpret_cast<mxArray *>(userData);\n"
            functionWrapper += "\trhs[1] = mxCreateNumericMatrix(1, 1, mxINT32_CLASS, mxREAL);\n"
            functionWrapper += "\tmxInt32 *rhs1Ptr = mxGetInt32s(rhs[1]);\n"
            functionWrapper += "\trhs1Ptr[0] = static_cast<mxInt32>(newState);\n"
            functionWrapper += "\trhs[2] = mxCreateNumericMatrix(1, 1, mxINT32_CLASS, mxREAL);\n"
            functionWrapper += "\tmxInt32 *rhs2Ptr = mxGetInt32s(rhs[2]);\n"
            functionWrapper += "\trhs2Ptr[0] = static_cast<mxInt32>(oldState);\n"
            functionWrapper += '\tint status = mexCallMATLAB(0,&lhs,3,rhs,"feval");\n'
            functionWrapper += "\tmxDestroyArray(lhs);\n"
            functionWrapper += "\tmxDestroyArray(rhs[1]);\n"
            functionWrapper += "\tmxDestroyArray(rhs[2]);\n"
            functionWrapper += "}\n\n"
            functionWrapper += f"void _wrap_{functionName}(int resc, mxArray *resv[], int argc, const mxArray *argv[])" + "{\n"
            functionWrapper += f"\tif(argc != 2){{\n"
            functionWrapper += "\t\tmexUnlock();\n"
            functionWrapper += f"\t\tmexErrMsgIdAndTxt(\"MATLAB:{functionName}:rhs\",\"This function requires 2 arguments.\");\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += initializeArgHelicsClass("HelicsFederate", "fed", 0, functionName)
            functionWrapper += "\tvoid *userData = mxGetData(argv[1]);\n"
            functionWrapper += initializeArgHelicsErrorPtr("err")
            functionWrapper += f"\t{functionName}(fed, &matlabFederateSetStateChangeCallback, userData, &err);\n\n"
            functionWrapper += "\tmxArray *_out = nullptr;\n"
            functionWrapper += "\tif(_out){\n"
            functionWrapper += "\t\t--resc;\n"
            functionWrapper += "\t\t*resv++ = _out;\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += f'{argHelicsErrorPtrPostFunctionCall("err")}\n'
            functionWrapper += "}\n\n\n"
            functionMainElements = f"\tcase {cursorIdx}:\n"
            functionMainElements += f"\t\t_wrap_{functionName}(resc, resv, argc, argv);\n"
            functionMainElements += f"\t\tbreak;\n"
            return functionComment, functionWrapper, functionMainElements
        
        
        def helicsFederateSetTimeRequestEntryCallbackMatlabWrapper(functionDict: Dict[str,str], cursorIdx: int):
            #check to see if function signiture changed
            argNum = len(functionDict.get("arguments", {}).keys())
            if argNum != 4:
                raise RuntimeError("the function signature for helicsFederateSetTimeRequestEntryCallback has changed!")
            arg0 = functionDict.get("arguments", {}).get(0, {})
            if arg0.get("spelling","") != "fed" or arg0.get("type", "") != "HelicsFederate":
                raise RuntimeError("the function signature for helicsFederateSetTimeRequestEntryCallback has changed!")
            arg1 = functionDict.get("arguments", {}).get(1, {})
            if arg1.get("spelling","") != "requestTime" or arg1.get("pointer_type", "") != "FunctionProto_*":
                raise RuntimeError("the function signature for helicsFederateSetTimeRequestEntryCallback has changed!")
            arg2 = functionDict.get("arguments", {}).get(2, {})
            if arg2.get("spelling","") != "userdata" or arg2.get("pointer_type", "") != "Void_*":
                raise RuntimeError("the function signature for helicsFederateSetTimeRequestEntryCallback has changed!")
            arg3 = functionDict.get("arguments", {}).get(3, {})
            if arg3.get("spelling","") != "err" or arg3.get("pointer_type", "") != "HelicsError_*":
                raise RuntimeError("the function signature for helicsFederateSetTimeRequestEntryCallback has changed!")
            functionName = functionDict.get("spelling","")
            functionComment = "%\tSet callback for the federate time request.n\n"
            functionComment += "%\t@details This callback will be executed when a valid time request is made.\n"
            functionComment += "%\tIt is intended for the possibility of embedded data grabbers in a callback to simplify user code.\n\n"
            functionComment += "%\t@param fed The federate object in which to set the callback.\n"
            functionComment += "%\t@param requestTime A callback with the signature void(double currentTime, double requestTime, bool iterating).\n"
            functionWrapper = "void matlabFederateSetTimeRequestEntryCallback(HelicsTime currentTime, HelicsTime requestTime, HelicsBool iterating, void *userData){\n"
            functionWrapper += "\tmxArray *lhs;\n"
            functionWrapper += "\tmxArray *rhs[4];\n"
            functionWrapper += "\trhs[0] = reinterpret_cast<mxArray *>(userData);\n"
            functionWrapper += "\trhs[1] = mxCreateDoubleScalar(currentTime);\n"
            functionWrapper += "\trhs[2] = mxCreateDoubleScalar(requestTime);\n"
            functionWrapper += "\trhs[3] = mxCreateLogicalMatrix(1, 1);\n"
            functionWrapper += "\tmxLogical *rhs3Ptr = mxGetLogicals(rhs[3]);\n"
            functionWrapper += "\trhs3Ptr[0] = false;\n"
            functionWrapper += "\tif(iterating == HELICS_TRUE){\n"
            functionWrapper += "\t\trhs3Ptr[0] = true;\n"
            functionWrapper += "\t}\n"
            functionWrapper += '\tint status = mexCallMATLAB(0,&lhs,4,rhs,"feval");\n'
            functionWrapper += "\tmxDestroyArray(lhs);\n"
            functionWrapper += "\tmxDestroyArray(rhs[1]);\n"
            functionWrapper += "\tmxDestroyArray(rhs[2]);\n"
            functionWrapper += "\tmxDestroyArray(rhs[3]);\n"
            functionWrapper += "}\n\n"
            functionWrapper += f"void _wrap_{functionName}(int resc, mxArray *resv[], int argc, const mxArray *argv[])" + "{\n"
            functionWrapper += f"\tif(argc != 2){{\n"
            functionWrapper += "\t\tmexUnlock();\n"
            functionWrapper += f"\t\tmexErrMsgIdAndTxt(\"MATLAB:{functionName}:rhs\",\"This function requires 2 arguments.\");\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += initializeArgHelicsClass("HelicsFederate", "fed", 0, functionName)
            functionWrapper += "\tvoid *userData = mxGetData(argv[1]);\n"
            functionWrapper += initializeArgHelicsErrorPtr("err")
            functionWrapper += f"\t{functionName}(fed, &matlabFederateSetTimeRequestEntryCallback, userData, &err);\n\n"
            functionWrapper += "\tmxArray *_out = nullptr;\n"
            functionWrapper += "\tif(_out){\n"
            functionWrapper += "\t\t--resc;\n"
            functionWrapper += "\t\t*resv++ = _out;\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += f'{argHelicsErrorPtrPostFunctionCall("err")}\n'
            functionWrapper += "}\n\n\n"
            functionMainElements = f"\tcase {cursorIdx}:\n"
            functionMainElements += f"\t\t_wrap_{functionName}(resc, resv, argc, argv);\n"
            functionMainElements += f"\t\tbreak;\n"
            return functionComment, functionWrapper, functionMainElements
        
        
        def helicsFederateSetTimeRequestReturnCallbackMatlabWrapper(functionDict: Dict[str,str], cursorIdx: int):
            #check to see if function signiture changed
            argNum = len(functionDict.get("arguments", {}).keys())
            if argNum != 4:
                raise RuntimeError("the function signature for helicsFederateSetTimeRequestReturnCallback has changed!")
            arg0 = functionDict.get("arguments", {}).get(0, {})
            if arg0.get("spelling","") != "fed" or arg0.get("type", "") != "HelicsFederate":
                raise RuntimeError("the function signature for helicsFederateSetTimeRequestReturnCallback has changed!")
            arg1 = functionDict.get("arguments", {}).get(1, {})
            if arg1.get("spelling","") != "requestTimeReturn" or arg1.get("pointer_type", "") != "FunctionProto_*":
                raise RuntimeError("the function signature for helicsFederateSetTimeRequestReturnCallback has changed!")
            arg2 = functionDict.get("arguments", {}).get(2, {})
            if arg2.get("spelling","") != "userdata" or arg2.get("pointer_type", "") != "Void_*":
                raise RuntimeError("the function signature for helicsFederateSetTimeRequestReturnCallback has changed!")
            arg3 = functionDict.get("arguments", {}).get(3, {})
            if arg3.get("spelling","") != "err" or arg3.get("pointer_type", "") != "HelicsError_*":
                raise RuntimeError("the function signature for helicsFederateSetTimeRequestReturnCallback has changed!")
            functionName = functionDict.get("spelling","")
            functionComment = "%\tSet callback for the federate time request return.n\n"
            functionComment += "%\t@details This callback will be executed when after all other callbacks for a time request return.\n"
            functionComment += "%\tThis callback will be the last thing executed before returning control the user program.\n\n"
            functionComment += "%\t@param fed The federate object in which to set the callback.\n"
            functionComment += "%\t@param requestTimeReturn A callback with the signature void(double newTime, bool iterating).\n"
            functionWrapper = "void matlabFederateSetTimeRequestReturnCallback(HelicsTime newTime, HelicsBool iterating, void *userData){\n"
            functionWrapper += "\tmxArray *lhs;\n"
            functionWrapper += "\tmxArray *rhs[3];\n"
            functionWrapper += "\trhs[0] = reinterpret_cast<mxArray *>(userData);\n"
            functionWrapper += "\trhs[1] = mxCreateDoubleScalar(newTime);\n"
            functionWrapper += "\trhs[2] = mxCreateLogicalMatrix(1, 1);\n"
            functionWrapper += "\tmxLogical *rhs2Ptr = mxGetLogicals(rhs[2]);\n"
            functionWrapper += "\trhs2Ptr[0] = false;\n"
            functionWrapper += "\tif(iterating == HELICS_TRUE){\n"
            functionWrapper += "\t\trhs2Ptr[0] = true;\n"
            functionWrapper += "\t}\n"
            functionWrapper += '\tint status = mexCallMATLAB(0,&lhs,3,rhs,"feval");\n'
            functionWrapper += "\tmxDestroyArray(lhs);\n"
            functionWrapper += "\tmxDestroyArray(rhs[1]);\n"
            functionWrapper += "\tmxDestroyArray(rhs[2]);\n"
            functionWrapper += "}\n\n"
            functionWrapper += f"void _wrap_{functionName}(int resc, mxArray *resv[], int argc, const mxArray *argv[])" + "{\n"
            functionWrapper += f"\tif(argc != 2){{\n"
            functionWrapper += "\t\tmexUnlock();\n"
            functionWrapper += f"\t\tmexErrMsgIdAndTxt(\"MATLAB:{functionName}:rhs\",\"This function requires 2 arguments.\");\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += initializeArgHelicsClass("HelicsFederate", "fed", 0, functionName)
            functionWrapper += "\tvoid *userData = mxGetData(argv[1]);\n"
            functionWrapper += initializeArgHelicsErrorPtr("err")
            functionWrapper += f"\t{functionName}(fed, &matlabFederateSetTimeRequestReturnCallback, userData, &err);\n\n"
            functionWrapper += "\tmxArray *_out = nullptr;\n"
            functionWrapper += "\tif(_out){\n"
            functionWrapper += "\t\t--resc;\n"
            functionWrapper += "\t\t*resv++ = _out;\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += f'{argHelicsErrorPtrPostFunctionCall("err")}\n'
            functionWrapper += "}\n\n\n"
            functionMainElements = f"\tcase {cursorIdx}:\n"
            functionMainElements += f"\t\t_wrap_{functionName}(resc, resv, argc, argv);\n"
            functionMainElements += f"\t\tbreak;\n"
            return functionComment, functionWrapper, functionMainElements
        
        
        def helicsTranslatorSetCustomCallbackMatlabWrapper(functionDict: Dict[str,str], cursorIdx: int):
            #check to see if function signiture changed
            argNum = len(functionDict.get("arguments", {}).keys())
            if argNum != 5:
                raise RuntimeError("the function signature for helicsTranslatorSetCustomCallback has changed!")
            arg0 = functionDict.get("arguments", {}).get(0, {})
            if arg0.get("spelling","") != "translator" or arg0.get("type", "") != "HelicsTranslator":
                raise RuntimeError("the function signature for helicsFederateSetTimeRequestReturnCallback has changed!")
            arg1 = functionDict.get("arguments", {}).get(1, {})
            if arg1.get("spelling","") != "toMessageCall" or arg1.get("pointer_type", "") != "FunctionProto_*":
                raise RuntimeError("the function signature for helicsTranslatorSetCustomCallback has changed!")
            arg2 = functionDict.get("arguments", {}).get(2, {})
            if arg2.get("spelling","") != "toValueCall" or arg2.get("pointer_type", "") != "FunctionProto_*":
                raise RuntimeError("the function signature for helicsTranslatorSetCustomCallback has changed!")
            arg3 = functionDict.get("arguments", {}).get(3, {})
            if arg3.get("spelling","") != "userdata" or arg3.get("pointer_type", "") != "Void_*":
                raise RuntimeError("the function signature for helicsTranslatorSetCustomCallback has changed!")
            arg4 = functionDict.get("arguments", {}).get(4, {})
            if arg4.get("spelling","") != "err" or arg4.get("pointer_type", "") != "HelicsError_*":
                raise RuntimeError("the function signature for helicsTranslatorSetCustomCallback has changed!")
            functionName = functionDict.get("spelling","")
            functionComment = "%\tSet a general callback for a custom Translator.n\n"
            functionComment += "%\t@details Add a pair of custom callbacks for running a translator operation in the C shared library.\n"
            functionComment += "%\t@param translator The translator object to set the callbacks for.\n"
            functionComment += "%\t@param toMessageCall A callback with the signature void(HelicsDataBuffer, HelicsMessage).\n"
            functionComment += "%\t@param toValueCall A callback with the signature void(HelicsMessage, HelicsDataBuffer).\n"
            functionWrapper = "void matlabToMessageCallCallback(HelicsDataBuffer value, HelicsMessage message, void *userData){\n"
            functionWrapper += "\tmxArray *lhs;\n"
            functionWrapper += "\tmxArray *rhs[3];\n"
            functionWrapper += "\trhs[0] = static_cast<mxArray **>(userData)[0];\n"
            functionWrapper += "\trhs[1] = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);\n"
            functionWrapper += "\tmxUint64 *rhs1Ptr = mxGetUint64s(rhs[1]);\n"
            functionWrapper += "\trhs1Ptr[0] = reinterpret_cast<mxUint64>(value);\n"
            functionWrapper += "\trhs[2] = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);\n"
            functionWrapper += "\tmxUint64 *rhs2Ptr = mxGetUint64s(rhs[1]);\n"
            functionWrapper += "\trhs2Ptr[0] = reinterpret_cast<mxUint64>(message);\n"
            functionWrapper += '\tint status = mexCallMATLAB(0,&lhs,3,rhs,"feval");\n'
            functionWrapper += "\tmxDestroyArray(lhs);\n"
            functionWrapper += "\tmxDestroyArray(rhs[1]);\n"
            functionWrapper += "\tmxDestroyArray(rhs[2]);\n"
            functionWrapper += "}\n\n"
            functionWrapper += "void matlabToValueCallCallback(HelicsMessage message, HelicsDataBuffer value, void *userData){\n"
            functionWrapper += "\tmxArray *lhs;\n"
            functionWrapper += "\tmxArray *rhs[3];\n"
            functionWrapper += "\trhs[0] = static_cast<mxArray **>(userData)[1];\n"
            functionWrapper += "\trhs[1] = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);\n"
            functionWrapper += "\tmxUint64 *rhs1Ptr = mxGetUint64s(rhs[1]);\n"
            functionWrapper += "\trhs1Ptr[0] = reinterpret_cast<mxUint64>(message);\n"
            functionWrapper += "\trhs[2] = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);\n"
            functionWrapper += "\tmxUint64 *rhs2Ptr = mxGetUint64s(rhs[1]);\n"
            functionWrapper += "\trhs2Ptr[0] = reinterpret_cast<mxUint64>(value);\n"
            functionWrapper += '\tint status = mexCallMATLAB(0,&lhs,3,rhs,"feval");\n'
            functionWrapper += "\tmxDestroyArray(lhs);\n"
            functionWrapper += "\tmxDestroyArray(rhs[1]);\n"
            functionWrapper += "\tmxDestroyArray(rhs[2]);\n"
            functionWrapper += "}\n\n"
            functionWrapper += f"void _wrap_{functionName}(int resc, mxArray *resv[], int argc, const mxArray *argv[])" + "{\n"
            functionWrapper += f"\tif(argc != 3){{\n"
            functionWrapper += "\t\tmexUnlock();\n"
            functionWrapper += f"\t\tmexErrMsgIdAndTxt(\"MATLAB:{functionName}:rhs\",\"This function requires 3 arguments.\");\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += initializeArgHelicsClass("HelicsTranslator", "translator", 0, functionName)
            functionWrapper += "\tmxArray *callbacks[2];\n"
            functionWrapper += "\tcallbacks[0] = const_cast<mxArray *>(argv[1]);\n"
            functionWrapper += "\tcallbacks[1] = const_cast<mxArray *>(argv[2]);\n"
            functionWrapper += "\tvoid *userData = reinterpret_cast<void *>(callbacks);\n"
            functionWrapper += initializeArgHelicsErrorPtr("err")
            functionWrapper += f"\t{functionName}(translator, &matlabToMessageCallCallback, &matlabToValueCallCallback, userData, &err);\n\n"
            functionWrapper += "\tmxArray *_out = nullptr;\n"
            functionWrapper += "\tif(_out){\n"
            functionWrapper += "\t\t--resc;\n"
            functionWrapper += "\t\t*resv++ = _out;\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += f'{argHelicsErrorPtrPostFunctionCall("err")}\n'
            functionWrapper += "}\n\n\n"
            functionMainElements = f"\tcase {cursorIdx}:\n"
            functionMainElements += f"\t\t_wrap_{functionName}(resc, resv, argc, argv);\n"
            functionMainElements += f"\t\tbreak;\n"
            return functionComment, functionWrapper, functionMainElements
        
        
        def helicsDataBufferFillFromComplexMatlabWrapper(functionDict: Dict[str,str], cursorIdx: int):
            #check to see if function signiture changed
            argNum = len(functionDict.get("arguments", {}).keys())
            if argNum != 3:
                raise RuntimeError("the function signature for helicsDataBufferFillFromComplex has changed!")
            arg0 = functionDict.get("arguments", {}).get(0, {})
            if arg0.get("spelling","") != "data" or arg0.get("type", "") != "HelicsDataBuffer":
                raise RuntimeError("the function signature for helicsDataBufferFillFromComplex has changed!")
            arg1 = functionDict.get("arguments", {}).get(1, {})
            if arg1.get("spelling","") != "real" or arg1.get("type", "") != "Double":
                raise RuntimeError("the function signature for helicsDataBufferFillFromComplex has changed!")
            arg2 = functionDict.get("arguments", {}).get(2, {})
            if arg2.get("spelling","") != "imag" or arg2.get("type", "") != "Double":
                raise RuntimeError("the function signature for helicsDataBufferFillFromComplex has changed!")
            functionName = functionDict.get("spelling","")
            functionComment = "%\tconvert a complex to serialized bytes.\n\n"
            functionComment += "%\t@param data The helicsDataBuffer to fill.\n"
            functionComment += "%\t@param value The complex value.\n"
            functionComment += "%\t@return int The buffer size.\n"
            functionWrapper = f"void _wrap_{functionName}(int resc, mxArray *resv[], int argc, const mxArray *argv[])" + "{\n"
            functionWrapper += f"\tif(argc != 2){{\n"
            functionWrapper += "\t\tmexUnlock();\n"
            functionWrapper += f"\t\tmexErrMsgIdAndTxt(\"MATLAB:{functionName}:rhs\",\"This function requires 2 arguments.\");\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += initializeArgHelicsClass("HelicsDataBuffer", "data", 0, functionName)
            functionWrapper += f"\tif(!mxIsComplex(argv[1])){{\n"
            functionWrapper += "\t\tmexUnlock();\n"
            functionWrapper += f"\t\tmexErrMsgIdAndTxt(\"MATLAB:{functionName}:TypeError\",\"Argument 1 must be of type complex.\");\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += "\tmxComplexDouble *value = mxGetComplexDoubles(argv[1]);\n\n"
            functionWrapper += f"\tint32_t result = {functionName}(data, value->real, value->imag);\n\n"
            functionWrapper += "\tmxArray *_out = mxCreateNumericMatrix(1, 1, mxINT32_CLASS, mxREAL);\n"
            functionWrapper += "\tmxInt32 *rv = mxGetInt32s(_out);\n"
            functionWrapper += "\trv[0] = static_cast<mxInt32>(result);\n"
            functionWrapper += "\tif(_out){\n"
            functionWrapper += "\t\t--resc;\n"
            functionWrapper += "\t\t*resv++ = _out;\n"
            functionWrapper += "\t}\n"
            functionWrapper += "}\n\n\n"
            functionMainElements = f"\tcase {cursorIdx}:\n"
            functionMainElements += f"\t\t_wrap_{functionName}(resc, resv, argc, argv);\n"
            functionMainElements += f"\t\tbreak;\n"
            return functionComment, functionWrapper, functionMainElements
        
        
        def helicsDataBufferFillFromVectorMatlabWrapper(functionDict: Dict[str,str], cursorIdx: int):
            #check to see if function signiture changed
            argNum = len(functionDict.get("arguments", {}).keys())
            if argNum != 3:
                raise RuntimeError("the function signature for helicsDataBufferFillFromVector has changed!")
            arg0 = functionDict.get("arguments", {}).get(0, {})
            if arg0.get("spelling","") != "data" or arg0.get("type", "") != "HelicsDataBuffer":
                raise RuntimeError("the function signature for helicsDataBufferFillFromVector has changed!")
            arg1 = functionDict.get("arguments", {}).get(1, {})
            if arg1.get("spelling","") != "value" or arg1.get("pointer_type", "") != "Double_*":
                raise RuntimeError("the function signature for helicsDataBufferFillFromVector has changed!")
            arg2 = functionDict.get("arguments", {}).get(2, {})
            if arg2.get("spelling","") != "dataSize" or arg2.get("type", "") != "Int":
                raise RuntimeError("the function signature for helicsDataBufferFillFromVector has changed!")
            functionName = functionDict.get("spelling","")
            functionComment = "%\tconvert a real vector to serialized bytes.\n\n"
            functionComment += "%\t@param data The helicsDataBuffer to fill.\n"
            functionComment += "%\t@param value The vector of doubles.\n"
            functionComment += "%\t@return int The buffer size.\n"
            functionWrapper = f"void _wrap_{functionName}(int resc, mxArray *resv[], int argc, const mxArray *argv[])" + "{\n"
            functionWrapper += f"\tif(argc != 2){{\n"
            functionWrapper += "\t\tmexUnlock();\n"
            functionWrapper += f"\t\tmexErrMsgIdAndTxt(\"MATLAB:{functionName}:rhs\",\"This function requires 2 arguments.\");\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += initializeArgHelicsClass("HelicsDataBuffer", "data", 0, functionName)
            functionWrapper += f"\tif(!mxIsNumeric(argv[1])){{\n"
            functionWrapper += "\t\tmexUnlock();\n"
            functionWrapper += f"\t\tmexErrMsgIdAndTxt(\"MATLAB:{functionName}:TypeError\",\"Argument 1 must be an array of doubles.\");\n"
            functionWrapper += "\t}\n"
            functionWrapper += "\tint dataSize =  static_cast<int>(mxGetNumberOfElements(argv[1]));\n\n"
            functionWrapper += "\tdouble *value =  static_cast<double *>(mxGetDoubles(argv[1]));\n\n"
            functionWrapper += f"\tint32_t result = {functionName}(data, static_cast<const double *>(value), dataSize);\n\n"
            functionWrapper += "\tmxArray *_out = mxCreateNumericMatrix(1, 1, mxINT32_CLASS, mxREAL);\n"
            functionWrapper += "\tmxInt32 *rv = mxGetInt32s(_out);\n"
            functionWrapper += "\trv[0] = static_cast<mxInt32>(result);\n"
            functionWrapper += "\tif(_out){\n"
            functionWrapper += "\t\t--resc;\n"
            functionWrapper += "\t\t*resv++ = _out;\n"
            functionWrapper += "\t}\n"
            functionWrapper += "}\n\n\n"
            functionMainElements = f"\tcase {cursorIdx}:\n"
            functionMainElements += f"\t\t_wrap_{functionName}(resc, resv, argc, argv);\n"
            functionMainElements += f"\t\tbreak;\n"
            return functionComment, functionWrapper, functionMainElements
                
        
        def helicsDataBufferFillFromComplexVectorMatlabWrapper(functionDict: Dict[str,str], cursorIdx: int):
            #check to see if function signiture changed
            argNum = len(functionDict.get("arguments", {}).keys())
            if argNum != 3:
                raise RuntimeError("the function signature for helicsDataBufferFillFromComplexVectorMatlabWrapper has changed!")
            arg0 = functionDict.get("arguments", {}).get(0, {})
            if arg0.get("spelling","") != "data" or arg0.get("type", "") != "HelicsDataBuffer":
                raise RuntimeError("the function signature for helicsDataBufferFillFromComplexVectorMatlabWrapper has changed!")
            arg1 = functionDict.get("arguments", {}).get(1, {})
            if arg1.get("spelling","") != "value" or arg1.get("pointer_type", "") != "Double_*":
                raise RuntimeError("the function signature for helicsDataBufferFillFromComplexVectorMatlabWrapper has changed!")
            arg2 = functionDict.get("arguments", {}).get(2, {})
            if arg2.get("spelling","") != "dataSize" or arg2.get("type", "") != "Int":
                raise RuntimeError("the function signature for helicsDataBufferFillFromComplexVectorMatlabWrapper has changed!")
            functionName = functionDict.get("spelling","")
            functionComment = "%\tconvert a complex vector to serialized bytes.\n\n"
            functionComment += "%\t@param data The helicsDataBuffer to fill.\n"
            functionComment += "%\t@param value The vector of complex values.\n"
            functionComment += "%\t@return int The buffer size.\n"
            functionWrapper = f"void _wrap_{functionName}(int resc, mxArray *resv[], int argc, const mxArray *argv[])" + "{\n"
            functionWrapper += f"\tif(argc != 2){{\n"
            functionWrapper += "\t\tmexUnlock();\n"
            functionWrapper += f"\t\tmexErrMsgIdAndTxt(\"MATLAB:{functionName}:rhs\",\"This function requires 2 arguments.\");\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += initializeArgHelicsClass("HelicsDataBuffer", "data", 0, functionName)
            functionWrapper += f"\tif(!mxIsComplex(argv[1])){{\n"
            functionWrapper += "\t\tmexUnlock();\n"
            functionWrapper += f"\t\tmexErrMsgIdAndTxt(\"MATLAB:{functionName}:TypeError\",\"Argument 2 must be of an array of type complex.\");\n"
            functionWrapper += "\t}\n"
            functionWrapper += "\tint dataSize =  static_cast<int>(mxGetN(argv[1])*2);\n\n"
            functionWrapper += "\tdouble *value = static_cast<double *>(malloc(dataSize * sizeof(double)));\n"
            functionWrapper += "\tmxComplexDouble *vals = mxGetComplexDoubles(argv[1]);\n"
            functionWrapper += "\tfor(int i=0; i<dataSize/2; ++i){\n"
            functionWrapper += "\t\tvalue[2*i] = vals[i].real;\n"
            functionWrapper += "\t\tvalue[2*i + 1] = vals[i].imag;\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += f"\tint32_t result = {functionName}(data, static_cast<const double *>(value), dataSize);\n\n"
            functionWrapper += "\tmxArray *_out = mxCreateNumericMatrix(1, 1, mxINT32_CLASS, mxREAL);\n"
            functionWrapper += "\tmxInt32 *rv = mxGetInt32s(_out);\n"
            functionWrapper += "\trv[0] = static_cast<mxInt32>(result);\n"
            functionWrapper += "\tif(_out){\n"
            functionWrapper += "\t\t--resc;\n"
            functionWrapper += "\t\t*resv++ = _out;\n"
            functionWrapper += "\t}\n"
            functionWrapper += "}\n\n\n"
            functionMainElements = f"\tcase {cursorIdx}:\n"
            functionMainElements += f"\t\t_wrap_{functionName}(resc, resv, argc, argv);\n"
            functionMainElements += f"\t\tbreak;\n"
            return functionComment, functionWrapper, functionMainElements
        
        
        def helicsDataBufferToStringMatlabWrapper(functionDict: Dict[str,str], cursorIdx: int):
            #check to see if function signiture changed
            argNum = len(functionDict.get("arguments", {}).keys())
            if argNum != 4:
                raise RuntimeError("the function signature for helicsDataBufferToString has changed!")
            arg0 = functionDict.get("arguments", {}).get(0, {})
            if arg0.get("spelling","") != "data" or arg0.get("type", "") != "HelicsDataBuffer":
                raise RuntimeError("the function signature for helicsDataBufferToString has changed!")
            arg1 = functionDict.get("arguments", {}).get(1, {})
            if arg1.get("spelling","") != "outputString" or arg1.get("pointer_type", "") != "Char_S_*":
                raise RuntimeError("the function signature for helicsDataBufferToString has changed!")
            arg2 = functionDict.get("arguments", {}).get(2, {})
            if arg2.get("spelling","") != "maxStringLen" or arg2.get("type", "") != "Int":
                raise RuntimeError("the function signature for helicsDataBufferToString has changed!")
            arg3 = functionDict.get("arguments", {}).get(3, {})
            if arg3.get("spelling","") != "actualLength" or arg3.get("pointer_type", "") != "Int_*":
                raise RuntimeError("the function signature for helicsDataBufferToString has changed!")
            functionName = functionDict.get("spelling","")
            functionComment = "%\tGet a string value from a HelicsDataBuffer.\n\n"
            functionComment += "%\t@param data The HelicsDataBuffer to get the string from.\n\n"
            functionComment += "%\t@return the string value.\n"
            functionWrapper = f"void _wrap_{functionName}(int resc, mxArray *resv[], int argc, const mxArray *argv[])" + "{\n"
            functionWrapper += f"\tif(argc != 1){{\n"
            functionWrapper += "\t\tmexUnlock();\n"
            functionWrapper += f"\t\tmexErrMsgIdAndTxt(\"MATLAB:{functionName}:rhs\",\"This function requires 1 arguments.\");\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += initializeArgHelicsClass("HelicsDataBuffer", "data", 0, functionName)
            functionWrapper += "\tint maxStringLen = helicsDataBufferStringSize(data) + 2;\n\n"
            functionWrapper += "\tchar *outputString = static_cast<char *>(malloc(maxStringLen));\n\n"
            functionWrapper += "\tint actualLength = 0;\n\n"
            functionWrapper += f"\t{functionName}(data, outputString, maxStringLen, &actualLength);\n\n"
            functionWrapper += "\tmwSize dims[2] = {1, static_cast<mwSize>(actualLength)};\n"
            functionWrapper += "\tmxArray *_out = mxCreateCharArray(2, dims);\n"
            functionWrapper += "\tmxChar *out_data = static_cast<mxChar *>(mxGetData(_out));\n"
            functionWrapper += "\tfor(int i=0; i<(actualLength); ++i){\n"
            functionWrapper += "\t\tout_data[i] = outputString[i];\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += "\tif(_out){\n"
            functionWrapper += "\t\t--resc;\n"
            functionWrapper += "\t\t*resv++ = _out;\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += f'{argCharPostFunctionCall("outputString")}\n\n'
            functionWrapper += "}\n\n\n"
            functionMainElements = f"\tcase {cursorIdx}:\n"
            functionMainElements += f"\t\t_wrap_{functionName}(resc, resv, argc, argv);\n"
            functionMainElements += f"\t\tbreak;\n"
            return functionComment, functionWrapper, functionMainElements
        
        
        def helicsDataBufferToRawStringMatlabWrapper(functionDict: Dict[str,str], cursorIdx: int):
            #check to see if function signiture changed
            argNum = len(functionDict.get("arguments", {}).keys())
            if argNum != 4:
                raise RuntimeError("the function signature for helicsDataBufferToRawString has changed!")
            arg0 = functionDict.get("arguments", {}).get(0, {})
            if arg0.get("spelling","") != "data" or arg0.get("type", "") != "HelicsDataBuffer":
                raise RuntimeError("the function signature for helicsDataBufferToRawString has changed!")
            arg1 = functionDict.get("arguments", {}).get(1, {})
            if arg1.get("spelling","") != "outputString" or arg1.get("pointer_type", "") != "Char_S_*":
                raise RuntimeError("the function signature for helicsDataBufferToRawString has changed!")
            arg2 = functionDict.get("arguments", {}).get(2, {})
            if arg2.get("spelling","") != "maxStringLen" or arg2.get("type", "") != "Int":
                raise RuntimeError("the function signature for helicsDataBufferToRawString has changed!")
            arg3 = functionDict.get("arguments", {}).get(3, {})
            if arg3.get("spelling","") != "actualLength" or arg3.get("pointer_type", "") != "Int_*":
                raise RuntimeError("the function signature for helicsDataBufferToRawString has changed!")
            functionName = functionDict.get("spelling","")
            functionComment = "%\tGet a raw string value from a HelicsDataBuffer.\n\n"
            functionComment += "%\t@param data The HelicsDataBuffer to get the raw string from.\n\n"
            functionComment += "%\t@return the raw string value.\n"
            functionWrapper = f"void _wrap_{functionName}(int resc, mxArray *resv[], int argc, const mxArray *argv[])" + "{\n"
            functionWrapper += f"\tif(argc != 1){{\n"
            functionWrapper += "\t\tmexUnlock();\n"
            functionWrapper += f"\t\tmexErrMsgIdAndTxt(\"MATLAB:{functionName}:rhs\",\"This function requires 1 arguments.\");\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += initializeArgHelicsClass("HelicsDataBuffer", "data", 0, functionName)
            functionWrapper += "\tint maxStringLen = helicsDataBufferStringSize(data) + 2;\n\n"
            functionWrapper += "\tchar *outputString = static_cast<char *>(malloc(maxStringLen));\n\n"
            functionWrapper += "\tint actualLength = 0;\n\n"
            functionWrapper += f"\t{functionName}(data, outputString, maxStringLen, &actualLength);\n\n"
            functionWrapper += "\tmwSize dims[2] = {1, static_cast<mwSize>(actualLength) - 1};\n"
            functionWrapper += "\tmxArray *_out = mxCreateCharArray(2, dims);\n"
            functionWrapper += "\tmxChar *out_data = static_cast<mxChar *>(mxGetData(_out));\n"
            functionWrapper += "\tfor(int i=0; i<(actualLength - 1); ++i){\n"
            functionWrapper += "\t\tout_data[i] = outputString[i];\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += "\tif(_out){\n"
            functionWrapper += "\t\t--resc;\n"
            functionWrapper += "\t\t*resv++ = _out;\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += f'{argCharPostFunctionCall("outputString")}\n\n'
            functionWrapper += "}\n\n\n"
            functionMainElements = f"\tcase {cursorIdx}:\n"
            functionMainElements += f"\t\t_wrap_{functionName}(resc, resv, argc, argv);\n"
            functionMainElements += f"\t\tbreak;\n"
            return functionComment, functionWrapper, functionMainElements
        
        
        def helicsDataBufferToComplexMatlabWrapper(functionDict: Dict[str,str], cursorIdx: int):
             #check to see if function signiture changed
            argNum = len(functionDict.get("arguments", {}).keys())
            if argNum != 3:
                raise RuntimeError("the function signature for helicsDataBufferToComplex has changed!")
            arg0 = functionDict.get("arguments", {}).get(0, {})
            if arg0.get("spelling","") != "data" or arg0.get("type", "") != "HelicsDataBuffer":
                raise RuntimeError("the function signature for helicsDataBufferToComplex has changed!")
            arg1 = functionDict.get("arguments", {}).get(1, {})
            if arg1.get("spelling","") != "real" or arg1.get("pointer_type", "") != "Double_*":
                raise RuntimeError("the function signature for helicsDataBufferToComplex has changed!")
            arg2 = functionDict.get("arguments", {}).get(2, {})
            if arg2.get("spelling","") != "imag" or arg2.get("pointer_type", "") != "Double_*":
                raise RuntimeError("the function signature for helicsDataBufferToComplex has changed!")
            functionName = functionDict.get("spelling","")
            functionComment = "%\tGet a complex value from an input object.\n\n"
            functionComment += "%\t@param ipt The input to get the data for.\n\n"
            functionComment += "%\t@return  A complex number.\n"
            functionWrapper = f"void _wrap_{functionName}(int resc, mxArray *resv[], int argc, const mxArray *argv[])" + "{\n"
            functionWrapper += f"\tif(argc != 1){{\n"
            functionWrapper += "\t\tmexUnlock();\n"
            functionWrapper += f"\t\tmexErrMsgIdAndTxt(\"MATLAB:{functionName}:rhs\",\"This function requires 1 arguments.\");\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += initializeArgHelicsClass("HelicsDataBuffer", "data", 0, functionName)
            functionWrapper += "\tdouble values[2];\n\n"
            functionWrapper += f"\t{functionName}(data, &(values[0]), &(values[1]));\n\n"
            functionWrapper += "\tmxArray *_out = mxCreateDoubleMatrix(1,1,mxCOMPLEX);\n"
            functionWrapper += "\tmxComplexDouble *complex_result = mxGetComplexDoubles(_out);\n"
            functionWrapper += "\tcomplex_result->real = values[0];\n"
            functionWrapper += "\tcomplex_result->imag = values[1];\n"
            functionWrapper += "\tif(_out){\n"
            functionWrapper += "\t\t--resc;\n"
            functionWrapper += "\t\t*resv++ = _out;\n"
            functionWrapper += "\t}\n"
            functionWrapper += "}\n\n\n"
            functionMainElements = f"\tcase {cursorIdx}:\n"
            functionMainElements += f"\t\t_wrap_{functionName}(resc, resv, argc, argv);\n"
            functionMainElements += f"\t\tbreak;\n"
            return functionComment, functionWrapper, functionMainElements
        
        
        def helicsDataBufferToComplexObjectMatlabWrapper(functionDict: Dict[str,str], cursorIdx: int):
            #check to see if function signiture changed
            argNum = len(functionDict.get("arguments", {}).keys())
            if argNum != 1:
                raise RuntimeError("the function signature for helicsDataBufferToComplexObject has changed!")
            arg0 = functionDict.get("arguments", {}).get(0, {})
            if arg0.get("spelling","") != "data" or arg0.get("type", "") != "HelicsDataBuffer":
                raise RuntimeError("the function signature for helicsDataBufferToComplexObject has changed!")
            functionName = functionDict.get("spelling","")
            functionComment = "%\tGet a complex value from an HelicsDataBuffer.\n\n"
            functionComment += "%\t@param data The HelicsDataBuffer to get the data for.\n\n"
            functionComment += "%\t@return  A complex number.\n"
            functionWrapper = f"void _wrap_{functionName}(int resc, mxArray *resv[], int argc, const mxArray *argv[])" + "{\n"
            functionWrapper += f"\tif(argc != 1){{\n"
            functionWrapper += "\t\tmexUnlock();\n"
            functionWrapper += f"\t\tmexErrMsgIdAndTxt(\"MATLAB:{functionName}:rhs\",\"This function requires 1 arguments.\");\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += initializeArgHelicsClass("HelicsDataBuffer", "data", 0, functionName)
            functionWrapper += f"\tHelicsComplex result = {functionName}(data);\n\n"
            functionWrapper += "\tmxComplexDouble complex_result;\n"
            functionWrapper += "\tcomplex_result.real = result.real;\n"
            functionWrapper += "\tcomplex_result.imag = result.imag;\n"
            functionWrapper += "\tmxArray *_out = mxCreateDoubleMatrix(1,1,mxCOMPLEX);\n"
            functionWrapper += "\tint status = mxSetComplexDoubles(_out, &complex_result);\n\n"
            functionWrapper += "\tif(_out){\n"
            functionWrapper += "\t\t--resc;\n"
            functionWrapper += "\t\t*resv++ = _out;\n"
            functionWrapper += "\t}\n"
            functionWrapper += "}\n\n\n"
            functionMainElements = f"\tcase {cursorIdx}:\n"
            functionMainElements += f"\t\t_wrap_{functionName}(resc, resv, argc, argv);\n"
            functionMainElements += f"\t\tbreak;\n"
            return functionComment, functionWrapper, functionMainElements
        
        
        def helicsDataBufferToVectorMatlabWrapper(functionDict: Dict[str,str], cursorIdx: int):
            #check to see if function signiture changed
            argNum = len(functionDict.get("arguments", {}).keys())
            if argNum != 4:
                raise RuntimeError("the function signature for helicsDataBufferToVector has changed!")
            arg0 = functionDict.get("arguments", {}).get(0, {})
            if arg0.get("spelling","") != "data" or arg0.get("type", "") != "HelicsDataBuffer":
                raise RuntimeError("the function signature for helicsDataBufferToVector has changed!")
            arg1 = functionDict.get("arguments", {}).get(1, {})
            if arg1.get("spelling","") != "values" or arg1.get("type", "") != "IncompleteArray":
                raise RuntimeError("the function signature for helicsDataBufferToVector has changed!")
            arg2 = functionDict.get("arguments", {}).get(2, {})
            if arg2.get("spelling","") != "maxlen" or arg2.get("type", "") != "Int":
                raise RuntimeError("the function signature for helicsDataBufferToVector has changed!")
            arg3 = functionDict.get("arguments", {}).get(3, {})
            if arg3.get("spelling","") != "actualSize" or arg3.get("pointer_type", "") != "Int_*":
                raise RuntimeError("the function signature for helicsDataBufferToVector has changed!")
            functionName = functionDict.get("spelling","")
            functionComment = "%\tGet a vector from a HelicsDataBuffer.\n\n"
            functionComment += "%\t@param data The HelicsDataBuffer to get the vector for.\n\n"
            functionComment += "%\t@return  a list of floating point values.\n"
            functionWrapper = f"void _wrap_{functionName}(int resc, mxArray *resv[], int argc, const mxArray *argv[])" + "{\n"
            functionWrapper += f"\tif(argc != 1){{\n"
            functionWrapper += "\t\tmexUnlock();\n"
            functionWrapper += f"\t\tmexErrMsgIdAndTxt(\"MATLAB:{functionName}:rhs\",\"This function requires 1 arguments.\");\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += initializeArgHelicsClass("HelicsDataBuffer", "data", 0, functionName)
            functionWrapper += "\tint maxLen = helicsDataBufferVectorSize(data);\n\n"
            functionWrapper += "\tdouble *values = static_cast<double *>(malloc(maxLen * sizeof(double)));\n\n"
            functionWrapper += "\tint actualSize = 0;\n\n"
            functionWrapper += f"\t{functionName}(data, values, maxLen, &actualSize);\n\n"
            functionWrapper += "\tmxDouble *result_data = static_cast<mxDouble *>(mxMalloc(actualSize * sizeof(mxDouble)));\n"
            functionWrapper += "\tfor(int i=0; i<actualSize; ++i){\n"
            functionWrapper += "\t\tresult_data[i] = static_cast<mxDouble>(values[i]);\n"
            functionWrapper += "\t}\n"
            functionWrapper += "\tmxArray *_out = mxCreateDoubleMatrix(actualSize, 1, mxREAL);\n"
            functionWrapper += "\tint status = mxSetDoubles(_out, &(result_data[0]));\n\n"
            functionWrapper += "\tif(_out){\n"
            functionWrapper += "\t\t--resc;\n"
            functionWrapper += "\t\t*resv++ = _out;\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += "}\n\n\n"
            functionMainElements = f"\tcase {cursorIdx}:\n"
            functionMainElements += f"\t\t_wrap_{functionName}(resc, resv, argc, argv);\n"
            functionMainElements += f"\t\tbreak;\n"
            return functionComment, functionWrapper, functionMainElements
        
        
        def helicsDataBufferToComplexVectorMatlabWrapper(functionDict: Dict[str,str], cursorIdx: int):
            #check to see if function signiture changed
            argNum = len(functionDict.get("arguments", {}).keys())
            if argNum != 4:
                raise RuntimeError("the function signature for helicsDataBufferToComplexVector has changed!")
            arg0 = functionDict.get("arguments", {}).get(0, {})
            if arg0.get("spelling","") != "data" or arg0.get("type", "") != "HelicsDataBuffer":
                raise RuntimeError("the function signature for helicsDataBufferToComplexVector has changed!")
            arg1 = functionDict.get("arguments", {}).get(1, {})
            if arg1.get("spelling","") != "values" or arg1.get("type", "") != "IncompleteArray":
                raise RuntimeError("the function signature for helicsDataBufferToComplexVector has changed!")
            arg2 = functionDict.get("arguments", {}).get(2, {})
            if arg2.get("spelling","") != "maxlen" or arg2.get("type", "") != "Int":
                raise RuntimeError("the function signature for helicsDataBufferToComplexVector has changed!")
            arg3 = functionDict.get("arguments", {}).get(3, {})
            if arg3.get("spelling","") != "actualSize" or arg3.get("pointer_type", "") != "Int_*":
                raise RuntimeError("the function signature for helicsDataBufferToComplexVector has changed!")
            functionName = functionDict.get("spelling","")
            functionComment = "%\tGet a compelx vector from a HelicsDataBuffer.\n\n"
            functionComment += "%\t@param data The HelicsDataBuffer to get the vector for.\n\n"
            functionComment += "%\t@return a list of complex values.\n"
            functionWrapper = f"void _wrap_{functionName}(int resc, mxArray *resv[], int argc, const mxArray *argv[])" + "{\n"
            functionWrapper += f"\tif(argc != 1){{\n"
            functionWrapper += "\t\tmexUnlock();\n"
            functionWrapper += f"\t\tmexErrMsgIdAndTxt(\"MATLAB:{functionName}:rhs\",\"This function requires 1 arguments.\");\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += initializeArgHelicsClass("HelicsDataBuffer", "data", 0, functionName)
            functionWrapper += "\tint maxLen = helicsDataBufferVectorSize(data);\n\n"
            functionWrapper += "\tdouble *values = static_cast<double *>(malloc(maxLen * sizeof(double)));\n\n"
            functionWrapper += "\tint actualSize = 0;\n\n"
            functionWrapper += f"\t{functionName}(data, values, maxLen, &actualSize);\n\n"
            functionWrapper += "\tmxComplexDouble *result_data = static_cast<mxComplexDouble *>(mxMalloc((actualSize/2)*sizeof(mxComplexDouble)));\n"
            functionWrapper += "\tfor(int i=0; i<(actualSize/2); ++i){\n"
            functionWrapper += "\t\tresult_data[i].real = values[2*(i)];\n"
            functionWrapper += "\t\tresult_data[i].imag = values[2*(i) + 1];\n"
            functionWrapper += "\t}\n"
            functionWrapper += "\tmxArray *_out = mxCreateDoubleMatrix(actualSize/2, 1, mxCOMPLEX);\n"
            functionWrapper += "\tint status = mxSetComplexDoubles(_out, &(result_data[0]));\n\n"
            functionWrapper += "\tif(_out){\n"
            functionWrapper += "\t\t--resc;\n"
            functionWrapper += "\t\t*resv++ = _out;\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += "}\n\n\n"
            functionMainElements = f"\tcase {cursorIdx}:\n"
            functionMainElements += f"\t\t_wrap_{functionName}(resc, resv, argc, argv);\n"
            functionMainElements += f"\t\tbreak;\n"
            return functionComment, functionWrapper, functionMainElements
        
        
        def helicsDataBufferToNamedPointMatlabWrapper(functionDict: Dict[str,str], cursorIdx: int):
            #check to see if function signiture changed
            argNum = len(functionDict.get("arguments", {}).keys())
            if argNum != 5:
                raise RuntimeError("the function signature for helicsDataBufferToNamedPoint has changed!")
            arg0 = functionDict.get("arguments", {}).get(0, {})
            if arg0.get("spelling","") != "data" or arg0.get("type", "") != "HelicsDataBuffer":
                raise RuntimeError("the function signature for helicsDataBufferToNamedPoint has changed!")
            arg1 = functionDict.get("arguments", {}).get(1, {})
            if arg1.get("spelling","") != "outputString" or arg1.get("pointer_type", "") != "Char_S_*":
                raise RuntimeError("the function signature for helicsDataBufferToNamedPoint has changed!")
            arg2 = functionDict.get("arguments", {}).get(2, {})
            if arg2.get("spelling","") != "maxStringLength" or arg2.get("type", "") != "Int":
                raise RuntimeError("the function signature for helicsDataBufferToNamedPoint has changed!")
            arg3 = functionDict.get("arguments", {}).get(3, {})
            if arg3.get("spelling","") != "actualLength" or arg3.get("pointer_type", "") != "Int_*":
                raise RuntimeError("the function signature for helicsDataBufferToNamedPoint has changed!")
            arg4 = functionDict.get("arguments", {}).get(4, {})
            if arg4.get("spelling","") != "val" or arg4.get("pointer_type", "") != "Double_*":
                raise RuntimeError("the function signature for helicsDataBufferToNamedPoint has changed!")
            functionName = functionDict.get("spelling","")
            functionComment = "%\tGet a named point from a subscription.\n\n"
            functionComment += "%\t@param ipt The input to get the result for.\n\n"
            functionComment += "%\t@return a string and a double value for the named point\n"
            functionWrapper = f"void _wrap_{functionName}(int resc, mxArray *resv[], int argc, const mxArray *argv[])" + "{\n"
            functionWrapper += f"\tif(argc != 1){{\n"
            functionWrapper += "\t\tmexUnlock();\n"
            functionWrapper += f"\t\tmexErrMsgIdAndTxt(\"MATLAB:{functionName}:rhs\",\"This function requires 1 arguments.\");\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += initializeArgHelicsClass("HelicsDataBuffer", "data", 0, functionName)
            functionWrapper += "\tint maxStringLen = helicsInputGetStringSize(data) + 2;\n\n"
            functionWrapper += "\tchar *outputString = static_cast<char *>(malloc(maxStringLen));\n\n"
            functionWrapper += "\tint actualLength = 0;\n\n"
            functionWrapper += "\tdouble val = 0;\n\n"
            functionWrapper += f"\t{functionName}(data, outputString, maxStringLen, &actualLength, &val);\n\n"
            functionWrapper += "\tmwSize dims[2] = {1, static_cast<mwSize>(actualLength)-1};\n"
            functionWrapper += "\tmxArray *_out = mxCreateCharArray(2, dims);\n"
            functionWrapper += "\tmxChar *out_data = static_cast<mxChar *>(mxGetData(_out));\n"
            functionWrapper += "\tfor(int i=0; i<(actualLength-1); ++i){\n"
            functionWrapper += "\t\tout_data[i] = outputString[i];\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += "\tif(_out){\n"
            functionWrapper += "\t\t--resc;\n"
            functionWrapper += "\t\t*resv++ = _out;\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += "\tif(--resc>=0){\n"
            functionWrapper += "\t\tmxArray *_out1 = mxCreateDoubleScalar(val);\n"
            functionWrapper += "\t\t*resv++ = _out1;\n"
            functionWrapper += "\t}\n\n" 
            functionWrapper += f'{argCharPostFunctionCall("outputString")}\n'
            functionWrapper += "}\n\n\n"
            functionMainElements = f"\tcase {cursorIdx}:\n"
            functionMainElements += f"\t\t_wrap_{functionName}(resc, resv, argc, argv);\n"
            functionMainElements += f"\t\tbreak;\n"
            return functionComment, functionWrapper, functionMainElements
        
        
        def helicsCloseLibraryMatlabWrapper(functionDict: Dict[str,str], cursorIdx: int):
            #check to see if function signiture changed
            argNum = len(functionDict.get("arguments", {}).keys())
            if argNum != 0:
                raise RuntimeError("the function signature for helicsCloseLibrary has changed!")
            functionName = functionDict.get("spelling","")
            functionComment = "%\tCall when done using the helics library.\n"
            functionComment += "%\tThis function will ensure the threads are closed properly.\n"
            functionComment += "%\tIf possible this should be the last call before exiting.\n"
            functionWrapper = f"void _wrap_{functionName}(int resc, mxArray *resv[], int argc, const mxArray *argv[])" + "{\n"
            functionWrapper += f"\tif(argc != 0){{\n"
            functionWrapper += "\t\tmexUnlock();\n"
            functionWrapper += f"\t\tmexErrMsgIdAndTxt(\"MATLAB:{functionName}:rhs\",\"This function requires 0 arguments.\");\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += f"\t{functionName}();\n\n"
            functionWrapper += "\tmxArray *_out = nullptr;\n"
            functionWrapper += "\tif(_out){\n"
            functionWrapper += "\t\t--resc;\n"
            functionWrapper += "\t\t*resv++ = _out;\n"
            functionWrapper += "\t}\n"
            functionWrapper += "\tmexUnlock();\n"
            functionWrapper += "}\n\n\n"
            functionMainElements = f"\tcase {cursorIdx}:\n"
            functionMainElements += f"\t\t_wrap_{functionName}(resc, resv, argc, argv);\n"
            functionMainElements += f"\t\tbreak;\n"
            return functionComment, functionWrapper, functionMainElements
        
        
        def helicsFederateInitializingEntryCallbackMatlabWrapper(functionDict: Dict[str,str], cursorIdx: int):
            #check to see if function signiture changed
            argNum = len(functionDict.get("arguments", {}).keys())
            if argNum != 4:
                raise RuntimeError("the function signature for helicsFederateInitializingEntryCallback has changed!")
            arg0 = functionDict.get("arguments", {}).get(0, {})
            if arg0.get("spelling","") != "fed" or arg0.get("type", "") != "HelicsFederate":
                raise RuntimeError("the function signature for helicsFederateInitializingEntryCallback has changed!")
            arg1 = functionDict.get("arguments", {}).get(1, {})
            if arg1.get("spelling","") != "initializingEntry" or arg1.get("pointer_type", "") != "FunctionProto_*":
                raise RuntimeError("the function signature for helicsFederateInitializingEntryCallback has changed!")
            arg2 = functionDict.get("arguments", {}).get(2, {})
            if arg2.get("spelling","") != "userdata" or arg2.get("pointer_type", "") != "Void_*":
                raise RuntimeError("the function signature for helicsFederateInitializingEntryCallback has changed!")
            arg3 = functionDict.get("arguments", {}).get(3, {})
            if arg3.get("spelling","") != "err" or arg3.get("pointer_type", "") != "HelicsError_*":
                raise RuntimeError("the function signature for helicsFederateInitializingEntryCallback has changed!")
            functionName = functionDict.get("spelling","")
            functionComment = "\tSet callback for the entry to initializingMode.n\n"
            functionComment += "\t@details This callback will be executed when the initializingMode is entered.\n\n"
            functionComment += "\t@param fed The federate object in which to set the callback.\n"
            functionComment += "\t@param initializingEntry A function handle with the signature void(HelicsBool iterating).\n"
            functionWrapper = "void matlabFederateInitializingEntryCallback(HelicsBool iterating, void *userData){\n"
            functionWrapper += "\tmxArray *lhs;\n"
            functionWrapper += "\tmxArray *rhs[2];\n"
            functionWrapper += "\trhs[0] = reinterpret_cast<mxArray *>(userData);\n"
            functionWrapper += "\trhs[1] = mxCreateLogicalMatrix(1, 1);\n"
            functionWrapper += "\tmxLogical *rhs1Ptr = mxGetLogicals(rhs[1]);\n"
            functionWrapper += "\trhs1Ptr[0] = false;\n"
            functionWrapper += "\tif(iterating == HELICS_TRUE){\n"
            functionWrapper += "\t\trhs1Ptr[0] = true;\n"
            functionWrapper += "\t}\n"
            functionWrapper += '\tint status = mexCallMATLAB(0,&lhs,2,rhs,"feval");\n'
            functionWrapper += "\tmxDestroyArray(lhs);\n"
            functionWrapper += "\tmxDestroyArray(rhs[1]);\n"
            functionWrapper += "}\n\n"
            functionWrapper += f"void _wrap_{functionName}(int resc, mxArray *resv[], int argc, const mxArray *argv[])" + "{\n"
            functionWrapper += f"\tif(argc != 2){{\n"
            functionWrapper += "\t\tmexUnlock();\n"
            functionWrapper += f"\t\tmexErrMsgIdAndTxt(\"MATLAB:{functionName}:rhs\",\"This function requires 2 arguments.\");\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += initializeArgHelicsClass("HelicsFederate", "fed", 0, functionName)
            functionWrapper += "\tvoid *userData = mxGetData(argv[1]);\n"
            functionWrapper += initializeArgHelicsErrorPtr("err")
            functionWrapper += f"\t{functionName}(fed, &matlabFederateInitializingEntryCallback, userData, &err);\n\n"
            functionWrapper += "\tmxArray *_out = nullptr;\n"
            functionWrapper += "\tif(_out){\n"
            functionWrapper += "\t\t--resc;\n"
            functionWrapper += "\t\t*resv++ = _out;\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += f'{argHelicsErrorPtrPostFunctionCall("err")}\n'
            functionWrapper += "}\n\n\n"
            functionMainElements = f"\tcase {cursorIdx}:\n"
            functionMainElements += f"\t\t_wrap_{functionName}(resc, resv, argc, argv);\n"
            functionMainElements += f"\t\tbreak;\n"
            return functionComment, functionWrapper, functionMainElements
        
        
        def helicsFederateExecutingEntryCallbackMatlabWrapper(functionDict: Dict[str,str], cursorIdx: int):
            #check to see if function signiture changed
            argNum = len(functionDict.get("arguments", {}).keys())
            if argNum != 4:
                raise RuntimeError("the function signature for helicsFederateExecutingEntryCallback has changed!")
            arg0 = functionDict.get("arguments", {}).get(0, {})
            if arg0.get("spelling","") != "fed" or arg0.get("type", "") != "HelicsFederate":
                raise RuntimeError("the function signature for helicsFederateExecutingEntryCallback has changed!")
            arg1 = functionDict.get("arguments", {}).get(1, {})
            if arg1.get("spelling","") != "executingEntry" or arg1.get("pointer_type", "") != "FunctionProto_*":
                raise RuntimeError("the function signature for helicsFederateExecutingEntryCallback has changed!")
            arg2 = functionDict.get("arguments", {}).get(2, {})
            if arg2.get("spelling","") != "userdata" or arg2.get("pointer_type", "") != "Void_*":
                raise RuntimeError("the function signature for helicsFederateExecutingEntryCallback has changed!")
            arg3 = functionDict.get("arguments", {}).get(3, {})
            if arg3.get("spelling","") != "err" or arg3.get("pointer_type", "") != "HelicsError_*":
                raise RuntimeError("the function signature for helicsFederateExecutingEntryCallback has changed!")
            functionName = functionDict.get("spelling","")
            functionComment = "\tSet callback for the entry to ExecutingMode.n\n"
            functionComment += "\t@details This callback will be executed once on first entry to executing mode.\n\n"
            functionComment += "\t@param fed The federate object in which to set the callback.\n"
            functionComment += "\t@param executingEntry A function handle with the signature void(void).\n"
            functionWrapper = "void matlabFederateExecutingEntryCallback(void *userData){\n"
            functionWrapper += "\tmxArray *lhs;\n"
            functionWrapper += "\tmxArray *rhs[1];\n"
            functionWrapper += "\trhs[0] = reinterpret_cast<mxArray *>(userData);\n"
            functionWrapper += '\tint status = mexCallMATLAB(0,&lhs,1,rhs,"feval");\n'
            functionWrapper += "\tmxDestroyArray(lhs);\n"
            functionWrapper += "}\n\n"
            functionWrapper += f"void _wrap_{functionName}(int resc, mxArray *resv[], int argc, const mxArray *argv[])" + "{\n"
            functionWrapper += f"\tif(argc != 2){{\n"
            functionWrapper += "\t\tmexUnlock();\n"
            functionWrapper += f"\t\tmexErrMsgIdAndTxt(\"MATLAB:{functionName}:rhs\",\"This function requires 2 arguments.\");\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += initializeArgHelicsClass("HelicsFederate", "fed", 0, functionName)
            functionWrapper += "\tvoid *userData = mxGetData(argv[1]);\n"
            functionWrapper += initializeArgHelicsErrorPtr("err")
            functionWrapper += f"\t{functionName}(fed, &matlabFederateExecutingEntryCallback, userData, &err);\n\n"
            functionWrapper += "\tmxArray *_out = nullptr;\n"
            functionWrapper += "\tif(_out){\n"
            functionWrapper += "\t\t--resc;\n"
            functionWrapper += "\t\t*resv++ = _out;\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += f'{argHelicsErrorPtrPostFunctionCall("err")}\n'
            functionWrapper += "}\n\n\n"
            functionMainElements = f"\tcase {cursorIdx}:\n"
            functionMainElements += f"\t\t_wrap_{functionName}(resc, resv, argc, argv);\n"
            functionMainElements += f"\t\tbreak;\n"
            return functionComment, functionWrapper, functionMainElements
        
        
        def helicsFederateCosimulationTerminationCallbackMatlabWrapper(functionDict: Dict[str,str], cursorIdx: int):
            #check to see if function signiture changed
            argNum = len(functionDict.get("arguments", {}).keys())
            if argNum != 4:
                raise RuntimeError("the function signature for helicsFederateCosimulationTerminationCallback has changed!")
            arg0 = functionDict.get("arguments", {}).get(0, {})
            if arg0.get("spelling","") != "fed" or arg0.get("type", "") != "HelicsFederate":
                raise RuntimeError("the function signature for helicsFederateCosimulationTerminationCallback has changed!")
            arg1 = functionDict.get("arguments", {}).get(1, {})
            if arg1.get("spelling","") != "cosimTermination" or arg1.get("pointer_type", "") != "FunctionProto_*":
                raise RuntimeError("the function signature for helicsFederateCosimulationTerminationCallback has changed!")
            arg2 = functionDict.get("arguments", {}).get(2, {})
            if arg2.get("spelling","") != "userdata" or arg2.get("pointer_type", "") != "Void_*":
                raise RuntimeError("the function signature for helicsFederateCosimulationTerminationCallback has changed!")
            arg3 = functionDict.get("arguments", {}).get(3, {})
            if arg3.get("spelling","") != "err" or arg3.get("pointer_type", "") != "HelicsError_*":
                raise RuntimeError("the function signature for helicsFederateCosimulationTerminationCallback has changed!")
            functionName = functionDict.get("spelling","")
            functionComment = "\tSet callback for cosimulation termination.n\n"
            functionComment += "\t@details This callback will be executed once when the time advance of the federate/co-simulation has terminated\n\tthis may be called as part of the finalize operation, or when a maxTime signal is returned from the requestTime or when and error is encountered.\n\n"
            functionComment += "\t@param fed The federate object in which to set the callback.\n"
            functionComment += "\t@param cosimTermination A function handle with the signature void(void).\n"
            functionWrapper = "void matlabFederateCosimulationTerminationCallback(void *userData){\n"
            functionWrapper += "\tmxArray *lhs;\n"
            functionWrapper += "\tmxArray *rhs[1];\n"
            functionWrapper += "\trhs[0] = reinterpret_cast<mxArray *>(userData);\n"
            functionWrapper += '\tint status = mexCallMATLAB(0,&lhs,1,rhs,"feval");\n'
            functionWrapper += "\tmxDestroyArray(lhs);\n"
            functionWrapper += "}\n\n"
            functionWrapper += f"void _wrap_{functionName}(int resc, mxArray *resv[], int argc, const mxArray *argv[])" + "{\n"
            functionWrapper += f"\tif(argc != 2){{\n"
            functionWrapper += "\t\tmexUnlock();\n"
            functionWrapper += f"\t\tmexErrMsgIdAndTxt(\"MATLAB:{functionName}:rhs\",\"This function requires 2 arguments.\");\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += initializeArgHelicsClass("HelicsFederate", "fed", 0, functionName)
            functionWrapper += "\tvoid *userData = mxGetData(argv[1]);\n"
            functionWrapper += initializeArgHelicsErrorPtr("err")
            functionWrapper += f"\t{functionName}(fed, &matlabFederateCosimulationTerminationCallback, userData, &err);\n\n"
            functionWrapper += "\tmxArray *_out = nullptr;\n"
            functionWrapper += "\tif(_out){\n"
            functionWrapper += "\t\t--resc;\n"
            functionWrapper += "\t\t*resv++ = _out;\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += f'{argHelicsErrorPtrPostFunctionCall("err")}\n'
            functionWrapper += "}\n\n\n"
            functionMainElements = f"\tcase {cursorIdx}:\n"
            functionMainElements += f"\t\t_wrap_{functionName}(resc, resv, argc, argv);\n"
            functionMainElements += f"\t\tbreak;\n"
            return functionComment, functionWrapper, functionMainElements
        
        
        def helicsFederateErrorHandlerCallbackMatlabWrapper(functionDict: Dict[str,str], cursorIdx: int):
            #check to see if function signiture changed
            argNum = len(functionDict.get("arguments", {}).keys())
            if argNum != 4:
                raise RuntimeError("the function signature for helicsFederateErrorHandlerCallback has changed!")
            arg0 = functionDict.get("arguments", {}).get(0, {})
            if arg0.get("spelling","") != "fed" or arg0.get("type", "") != "HelicsFederate":
                raise RuntimeError("the function signature for helicsFederateErrorHandlerCallback has changed!")
            arg1 = functionDict.get("arguments", {}).get(1, {})
            if arg1.get("spelling","") != "errorHandler" or arg1.get("pointer_type", "") != "FunctionProto_*":
                raise RuntimeError("the function signature for helicsFederateErrorHandlerCallback has changed!")
            arg2 = functionDict.get("arguments", {}).get(2, {})
            if arg2.get("spelling","") != "userdata" or arg2.get("pointer_type", "") != "Void_*":
                raise RuntimeError("the function signature for helicsFederateErrorHandlerCallback has changed!")
            arg3 = functionDict.get("arguments", {}).get(3, {})
            if arg3.get("spelling","") != "err" or arg3.get("pointer_type", "") != "HelicsError_*":
                raise RuntimeError("the function signature for helicsFederateErrorHandlerCallback has changed!")
            functionName = functionDict.get("spelling","")
            functionComment = "\tSet callback for error handling.n\n"
            functionComment += "\t@details This callback will be executed when a federate error is encountered\n\n"
            functionComment += "\t@param fed The federate object in which to set the callback.\n"
            functionComment += "\t@param errorHandler A function handle with the signature void(int errorCode, const char* errorString).\n"
            functionWrapper = "void matlabFederateErrorHandlerCallback(int errorCode, const char* errorString, void *userData){\n"
            functionWrapper += "\tmxArray *lhs;\n"
            functionWrapper += "\tmxArray *rhs[3];\n"
            functionWrapper += "\trhs[0] = reinterpret_cast<mxArray *>(userData);\n"
            functionWrapper += "\trhs[1] = mxCreateNumericMatrix(1, 1, mxINT32_CLASS, mxREAL);\n"
            functionWrapper += "\tmxInt32 *rhs1Ptr = mxGetInt32s(rhs[1]);\n"
            functionWrapper += "\trhs1Ptr[0] = static_cast<mxInt32>(errorCode);\n"
            functionWrapper += "\trhs[2] = mxCreateString(errorString);\n"
            functionWrapper += '\tint status = mexCallMATLAB(0,&lhs,3,rhs,"feval");\n'
            functionWrapper += "\tmxDestroyArray(lhs);\n"
            functionWrapper += "\tmxDestroyArray(rhs[1]);\n"
            functionWrapper += "\tmxDestroyArray(rhs[2]);\n"
            functionWrapper += "}\n\n"
            functionWrapper += f"void _wrap_{functionName}(int resc, mxArray *resv[], int argc, const mxArray *argv[])" + "{\n"
            functionWrapper += f"\tif(argc != 2){{\n"
            functionWrapper += "\t\tmexUnlock();\n"
            functionWrapper += f"\t\tmexErrMsgIdAndTxt(\"MATLAB:{functionName}:rhs\",\"This function requires 2 arguments.\");\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += initializeArgHelicsClass("HelicsFederate", "fed", 0, functionName)
            functionWrapper += "\tvoid *userData = mxGetData(argv[1]);\n"
            functionWrapper += initializeArgHelicsErrorPtr("err")
            functionWrapper += f"\t{functionName}(fed, &matlabFederateErrorHandlerCallback, userData, &err);\n\n"
            functionWrapper += "\tmxArray *_out = nullptr;\n"
            functionWrapper += "\tif(_out){\n"
            functionWrapper += "\t\t--resc;\n"
            functionWrapper += "\t\t*resv++ = _out;\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += f'{argHelicsErrorPtrPostFunctionCall("err")}\n'
            functionWrapper += "}\n\n\n"
            functionMainElements = f"\tcase {cursorIdx}:\n"
            functionMainElements += f"\t\t_wrap_{functionName}(resc, resv, argc, argv);\n"
            functionMainElements += f"\t\tbreak;\n"
            return functionComment, functionWrapper, functionMainElements
        
        
        def helicsCallbackFederateNextTimeCallbackMatlabWrapper(functionDict: Dict[str,str], cursorIdx: int):
            #check to see if function signiture changed
            argNum = len(functionDict.get("arguments", {}).keys())
            if argNum != 4:
                raise RuntimeError("the function signature for helicsCallbackFederateNextTimeCallback has changed!")
            arg0 = functionDict.get("arguments", {}).get(0, {})
            if arg0.get("spelling","") != "fed" or arg0.get("type", "") != "HelicsFederate":
                raise RuntimeError("the function signature for helicsCallbackFederateNextTimeCallback has changed!")
            arg1 = functionDict.get("arguments", {}).get(1, {})
            if arg1.get("spelling","") != "timeUpdate" or arg1.get("pointer_type", "") != "FunctionProto_*":
                raise RuntimeError("the function signature for helicsCallbackFederateNextTimeCallback has changed!")
            arg2 = functionDict.get("arguments", {}).get(2, {})
            if arg2.get("spelling","") != "userdata" or arg2.get("pointer_type", "") != "Void_*":
                raise RuntimeError("the function signature for helicsCallbackFederateNextTimeCallback has changed!")
            arg3 = functionDict.get("arguments", {}).get(3, {})
            if arg3.get("spelling","") != "err" or arg3.get("pointer_type", "") != "HelicsError_*":
                raise RuntimeError("the function signature for helicsCallbackFederateNextTimeCallback has changed!")
            functionName = functionDict.get("spelling","")
            functionComment = "\tSet callback for the next time update=.\n\n"
            functionComment += "\t@details This callback will be executed to compute the next time update for a callback federate.\n\n"
            functionComment += "\t@param fed The federate object in which to set the callback.\n"
            functionComment += "\t@param timeUpdate A function handle with the signature HelicsTime (HelicsTime time).\n"
            functionWrapper = "HelicsTime matlabCallbackFederateNextTimeCallback(HelicsTime time, void *userData){\n"
            functionWrapper += "\tmxArray *lhs[1];\n"
            functionWrapper += "\tmxArray *rhs[2];\n"
            functionWrapper += "\trhs[0] = reinterpret_cast<mxArray *>(userData);\n"
            functionWrapper += "\trhs[1] = mxCreateDoubleScalar(time);\n"
            functionWrapper += '\tint status = mexCallMATLAB(1,lhs,2,rhs,"feval");\n'
            functionWrapper += f"\tif(!mxIsNumeric(lhs[0])){{\n"
            functionWrapper += "\t\tmexUnlock();\n"
            functionWrapper += f"\t\tmexErrMsgIdAndTxt(\"MATLAB:{functionName}:TypeError\",\"return type must be of type double.\");\n"
            functionWrapper += "\t}\n"
            functionWrapper += f"\tHelicsTime rv = (HelicsTime)(mxGetScalar(lhs[0]));\n\n"
            functionWrapper += "\tmxDestroyArray(lhs[0]);\n"
            functionWrapper += "\tmxDestroyArray(rhs[1]);\n"
            functionWrapper += "\treturn rv;\n"
            functionWrapper += "}\n\n"
            functionWrapper += f"void _wrap_{functionName}(int resc, mxArray *resv[], int argc, const mxArray *argv[])" + "{\n"
            functionWrapper += f"\tif(argc != 2){{\n"
            functionWrapper += "\t\tmexUnlock();\n"
            functionWrapper += f"\t\tmexErrMsgIdAndTxt(\"MATLAB:matlabCallbackFederateNextTimeCallback:rhs\",\"This function requires 2 arguments.\");\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += initializeArgHelicsClass("HelicsFederate", "fed", 0, functionName)
            functionWrapper += "\tvoid *userData = mxGetData(argv[1]);\n"
            functionWrapper += initializeArgHelicsErrorPtr("err")
            functionWrapper += f"\t{functionName}(fed, &matlabCallbackFederateNextTimeCallback, userData, &err);\n\n"
            functionWrapper += "\tmxArray *_out = nullptr;\n"
            functionWrapper += "\tif(_out){\n"
            functionWrapper += "\t\t--resc;\n"
            functionWrapper += "\t\t*resv++ = _out;\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += f'{argHelicsErrorPtrPostFunctionCall("err")}\n'
            functionWrapper += "}\n\n\n"
            functionMainElements = f"\tcase {cursorIdx}:\n"
            functionMainElements += f"\t\t_wrap_{functionName}(resc, resv, argc, argv);\n"
            functionMainElements += f"\t\tbreak;\n"
            return functionComment, functionWrapper, functionMainElements
        
        
        def helicsCallbackFederateNextTimeIterativeCallbackMatlabWrapper(functionDict: Dict[str,str], cursorIdx: int):
            #check to see if function signiture changed
            argNum = len(functionDict.get("arguments", {}).keys())
            if argNum != 4:
                raise RuntimeError("the function signature for helicsCallbackFederateNextTimeIterativeCallback has changed!")
            arg0 = functionDict.get("arguments", {}).get(0, {})
            if arg0.get("spelling","") != "fed" or arg0.get("type", "") != "HelicsFederate":
                raise RuntimeError("the function signature for helicsCallbackFederateNextTimeIterativeCallback has changed!")
            arg1 = functionDict.get("arguments", {}).get(1, {})
            if arg1.get("spelling","") != "timeUpdate" or arg1.get("pointer_type", "") != "FunctionProto_*":
                raise RuntimeError("the function signature for helicsCallbackFederateNextTimeIterativeCallback has changed!")
            arg2 = functionDict.get("arguments", {}).get(2, {})
            if arg2.get("spelling","") != "userdata" or arg2.get("pointer_type", "") != "Void_*":
                raise RuntimeError("the function signature for helicsCallbackFederateNextTimeIterativeCallback has changed!")
            arg3 = functionDict.get("arguments", {}).get(3, {})
            if arg3.get("spelling","") != "err" or arg3.get("pointer_type", "") != "HelicsError_*":
                raise RuntimeError("the function signature for helicsCallbackFederateNextTimeIterativeCallback has changed!")
            functionName = functionDict.get("spelling","")
            functionComment = "\tSet callback for the next time update with iteration capability.\n\n"
            functionComment += "\t@details This callback will be executed to compute the next time update for a callback federate.\n\n"
            functionComment += "\t@param fed The federate object in which to set the callback.\n"
            functionComment += "\t@param timeUpdate A function handle with the signature void(HelicsTime time, HelicsIterationResult iterationResult, HelicsIterationRequest* iteration).\n"
            functionWrapper = "HelicsTime matlabCallbackFederateNextTimeIterativeCallback(HelicsTime time, HelicsIterationResult iterationResult, HelicsIterationRequest *iteration, void *userData){\n"
            functionWrapper += "\tmxArray *lhs[2];\n"
            functionWrapper += "\tmxArray *rhs[4];\n"
            functionWrapper += "\trhs[0] = reinterpret_cast<mxArray *>(userData);\n"
            functionWrapper += "\trhs[1] = mxCreateDoubleScalar(time);\n"
            functionWrapper += "\trhs[2] = mxCreateNumericMatrix(1, 1, mxINT32_CLASS, mxREAL);\n"
            functionWrapper += "\tmxInt32 *pRhs2 = mxGetInt32s(rhs[2]);\n"
            functionWrapper += "\tpRhs2[0] = static_cast<mxInt32>(iterationResult);\n"
            functionWrapper += "\trhs[3] = mxCreateNumericMatrix(1, 1, mxINT32_CLASS, mxREAL);\n"
            functionWrapper += "\tmxInt32 *pRhs3 = mxGetInt32s(rhs[3]);\n"
            functionWrapper += "\tpRhs3[0] = static_cast<mxInt32>(*iteration);\n"
            functionWrapper += '\tint status = mexCallMATLAB(2,lhs,4,rhs,"feval");\n'
            functionWrapper += f"\tif(!mxIsNumeric(lhs[0])){{\n"
            functionWrapper += "\t\tmexUnlock();\n"
            functionWrapper += f"\t\tmexErrMsgIdAndTxt(\"MATLAB:matlabCallbackFederateNextTimeIterativeCallback:TypeError\",\"first type returned must be of type double.\");\n"
            functionWrapper += "\t}\n"
            functionWrapper += f"\tHelicsTime rv = (HelicsTime)(mxGetScalar(lhs[0]));\n\n"
            functionWrapper += f"\tif(mxGetClassID(lhs[1]) != mxINT32_CLASS){{\n"
            functionWrapper += "\t\tmexUnlock();\n"
            functionWrapper += f"\t\tmexErrMsgIdAndTxt(\"MATLAB:matlabCallbackFederateNextTimeIterativeCallback:TypeError\",\"second type returned must be of type int32.\");\n"
            functionWrapper += "\t}\n"
            functionWrapper += f"\tmxInt32 *pIterationRequest = mxGetInt32s(lhs[1]);\n"
            functionWrapper += f"\t*iteration = static_cast<HelicsIterationRequest>(pIterationRequest[0]);\n\n"
            functionWrapper += "\tmxDestroyArray(lhs[0]);\n"
            functionWrapper += "\tmxDestroyArray(lhs[1]);\n"
            functionWrapper += "\tmxDestroyArray(rhs[1]);\n"
            functionWrapper += "\tmxDestroyArray(rhs[2]);\n"
            functionWrapper += "\tmxDestroyArray(rhs[3]);\n"
            functionWrapper += "\treturn rv;\n"
            functionWrapper += "}\n\n"
            functionWrapper += f"void _wrap_{functionName}(int resc, mxArray *resv[], int argc, const mxArray *argv[])" + "{\n"
            functionWrapper += f"\tif(argc != 2){{\n"
            functionWrapper += "\t\tmexUnlock();\n"
            functionWrapper += f"\t\tmexErrMsgIdAndTxt(\"MATLAB:{functionName}:rhs\",\"This function requires 2 arguments.\");\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += initializeArgHelicsClass("HelicsFederate", "fed", 0, functionName)
            functionWrapper += "\tvoid *userData = mxGetData(argv[1]);\n"
            functionWrapper += initializeArgHelicsErrorPtr("err")
            functionWrapper += f"\t{functionName}(fed, &matlabCallbackFederateNextTimeIterativeCallback, userData, &err);\n\n"
            functionWrapper += "\tmxArray *_out = nullptr;\n"
            functionWrapper += "\tif(_out){\n"
            functionWrapper += "\t\t--resc;\n"
            functionWrapper += "\t\t*resv++ = _out;\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += f'{argHelicsErrorPtrPostFunctionCall("err")}\n'
            functionWrapper += "}\n\n\n"
            functionMainElements = f"\tcase {cursorIdx}:\n"
            functionMainElements += f"\t\t_wrap_{functionName}(resc, resv, argc, argv);\n"
            functionMainElements += f"\t\tbreak;\n"
            return functionComment, functionWrapper, functionMainElements
        
        
        def helicsCallbackFederateInitializeCallbackMatlabWrapper(functionDict: Dict[str,str], cursorIdx: int):
            #check to see if function signiture changed
            argNum = len(functionDict.get("arguments", {}).keys())
            if argNum != 4:
                raise RuntimeError("the function signature for helicsCallbackFederateInitializeCallback has changed!")
            arg0 = functionDict.get("arguments", {}).get(0, {})
            if arg0.get("spelling","") != "fed" or arg0.get("type", "") != "HelicsFederate":
                raise RuntimeError("the function signature for helicsCallbackFederateInitializeCallback has changed!")
            arg1 = functionDict.get("arguments", {}).get(1, {})
            if arg1.get("spelling","") != "initialize" or arg1.get("pointer_type", "") != "FunctionProto_*":
                raise RuntimeError("the function signature for helicsCallbackFederateInitializeCallback has changed!")
            arg2 = functionDict.get("arguments", {}).get(2, {})
            if arg2.get("spelling","") != "userdata" or arg2.get("pointer_type", "") != "Void_*":
                raise RuntimeError("the function signature for helicsCallbackFederateInitializeCallback has changed!")
            arg3 = functionDict.get("arguments", {}).get(3, {})
            if arg3.get("spelling","") != "err" or arg3.get("pointer_type", "") != "HelicsError_*":
                raise RuntimeError("the function signature for helicsCallbackFederateInitializeCallback has changed!")
            functionName = functionDict.get("spelling","")
            functionComment = "\tSet callback for initialization.\n\n"
            functionComment += "\t@details This callback will be executed when computing whether to iterate in initialization mode.\n\n"
            functionComment += "\t@param fed The federate object in which to set the callback.\n"
            functionComment += "\t@param initialize A function handle with the signature HelicsIterationRequest(void).\n"
            functionWrapper = "HelicsIterationRequest matlabCallbackFederateInitializeCallback(void *userData){\n"
            functionWrapper += "\tmxArray *lhs[1];\n"
            functionWrapper += "\tmxArray *rhs[1];\n"
            functionWrapper += "\trhs[0] = reinterpret_cast<mxArray *>(userData);\n"
            functionWrapper += '\tint status = mexCallMATLAB(1,lhs,1,rhs,"feval");\n'
            functionWrapper += f"\tif(mxGetClassID(lhs[0]) != mxINT32_CLASS){{\n"
            functionWrapper += "\t\tmexUnlock();\n"
            functionWrapper += f"\t\tmexErrMsgIdAndTxt(\"MATLAB:matlabCallbackFederateInitializeCallback:TypeError\",\"return type must be of type int32.\");\n"
            functionWrapper += "\t}\n"
            functionWrapper += f"\tmxInt32 *pIterationRequest = mxGetInt32s(lhs[0]);\n"
            functionWrapper += f"\tHelicsIterationRequest rv = static_cast<HelicsIterationRequest>(pIterationRequest[0]);\n\n"
            functionWrapper += "\tmxDestroyArray(lhs[0]);\n"
            functionWrapper += "\treturn rv;\n"
            functionWrapper += "}\n\n"
            functionWrapper += f"void _wrap_{functionName}(int resc, mxArray *resv[], int argc, const mxArray *argv[])" + "{\n"
            functionWrapper += f"\tif(argc != 2){{\n"
            functionWrapper += "\t\tmexUnlock();\n"
            functionWrapper += f"\t\tmexErrMsgIdAndTxt(\"MATLAB:{functionName}:rhs\",\"This function requires 2 arguments.\");\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += initializeArgHelicsClass("HelicsFederate", "fed", 0, functionName)
            functionWrapper += "\tvoid *userData = mxGetData(argv[1]);\n"
            functionWrapper += initializeArgHelicsErrorPtr("err")
            functionWrapper += f"\t{functionName}(fed, &matlabCallbackFederateInitializeCallback, userData, &err);\n\n"
            functionWrapper += "\tmxArray *_out = nullptr;\n"
            functionWrapper += "\tif(_out){\n"
            functionWrapper += "\t\t--resc;\n"
            functionWrapper += "\t\t*resv++ = _out;\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += f'{argHelicsErrorPtrPostFunctionCall("err")}\n'
            functionWrapper += "}\n\n\n"
            functionMainElements = f"\tcase {cursorIdx}:\n"
            functionMainElements += f"\t\t_wrap_{functionName}(resc, resv, argc, argv);\n"
            functionMainElements += f"\t\tbreak;\n"
            return functionComment, functionWrapper, functionMainElements
        
        
        def createMexMain() -> str:
            mexMainStr = "void mexFunction(int resc, mxArray *resv[], int argc, const mxArray *argv[]) {\n"
            mexMainStr += "\tif(--argc < 0 || !mxIsChar(*argv)){\n"
            mexMainStr += "\t\tmexErrMsgTxt(\"This mex file should only be called from inside the .m files. First input should be the function ID.\");\n"
            mexMainStr += "\t}\n"
            mexMainStr += "\tint functionId;\n"
            mexMainStr += "\ttry {\n"
            mexMainStr += "\t\tfunctionId = wrapperFunctionMap.at(std::string(mxArrayToString(*argv++)));\n"
            mexMainStr += "\t} catch (const std::out_of_range& e) {\n"
            mexMainStr += "\t\tmexErrMsgTxt(\"unrecognized function id.\");\n"
            mexMainStr += "\t}\n"
            mexMainStr += "\tint flag=0;\n"
            mexMainStr += "\t/* Prevent unloading this file until MATLAB exits */\n"
            mexMainStr += "\tif(!mexFunctionCalled) {\n"
            mexMainStr += "\t\tmexFunctionCalled = 1;\n"
            mexMainStr += "\t\tmexLock();\n"
            mexMainStr += "\t}\n"
            mexMainStr += "\tswitch (functionId) {\n"
            return mexMainStr
        
        
        def closeBoilerPlate() -> str:
            boilerPlateStr = "\tdefault:\n"
            boilerPlateStr += "\t\tmexErrMsgIdAndTxt(\"helics:mexFunction\",\"An unknown function id was encountered. Call the mex function with a valid function id.\");\n"
            boilerPlateStr += "\t}\n}\n\n"
            return boilerPlateStr 
        filePath = os.path.dirname(__file__)   
        if not os.path.exists(os.path.join(self.__rootDir, "matlabBindings/+helics")):
            os.makedirs(os.path.join(self.__rootDir, "matlabBindings/+helics"))
            try:
                shutil.copy2(os.path.join(filePath, "extra_m_codes/helicsInputSetDefault.m"), os.path.join(self.__rootDir, "matlabBindings/+helics"))
                shutil.copy2(os.path.join(filePath, "extra_m_codes/helicsPublicationPublish.m"), os.path.join(self.__rootDir, "matlabBindings/+helics"))
            except:
                matlabBindingGeneratorLogger.warning("couldn't copy extra_m_codes.")
        else:
            try:
                shutil.rmtree(os.path.join(self.__rootDir, 'matlabBindings'))
                os.makedirs(os.path.join(self.__rootDir, "matlabBindings/+helics"))
            except:
                matlabBindingGeneratorLogger.warning("couldn't delete the old bindings files.")
            try:
                shutil.copy2(os.path.join(filePath, "extra_m_codes/helicsInputSetDefault.m"), os.path.join(self.__rootDir, "matlabBindings/+helics"))
                shutil.copy2(os.path.join(filePath, "extra_m_codes/helicsPublicationPublish.m"), os.path.join(self.__rootDir, "matlabBindings/+helics"))
            except:
                matlabBindingGeneratorLogger.warning("couldn't copy extra_m_codes.")
        helicsMexStr = ""
        helicsMexWrapperFunctions = []
        helicsMexMainFunctionElements = []
        helicsMapTuples = []
        for cu in self.__helicsParser.parsedInfo.keys():
            if self.__helicsParser.parsedInfo[cu]["kind"] == "ENUM_DECL":
                createEnum(self.__helicsParser.parsedInfo[cu])
            if self.__helicsParser.parsedInfo[cu]["kind"] == "MACRO_DEFINITION":
                macroMexWrapperFunctionStr, macroMexMainFunctionElementStr, macroMapTuple= createMacro(self.__helicsParser.parsedInfo[cu],int(cu))
                helicsMexWrapperFunctions.append(macroMexWrapperFunctionStr)
                helicsMexMainFunctionElements.append(macroMexMainFunctionElementStr)
                if macroMapTuple != None:
                    helicsMapTuples.append(macroMapTuple)
            if self.__helicsParser.parsedInfo[cu]["kind"] == "VAR_DECL":
                createVar(self.__helicsParser.parsedInfo[cu],int(cu))
            if self.__helicsParser.parsedInfo[cu]["kind"] == "FUNCTION_DECL":
                functionMexWrapperFunctionStr, functionMexMainFunctionElementStr, functionMapTuple= createFunction(self.__helicsParser.parsedInfo[cu],int(cu))
                helicsMexWrapperFunctions.append(functionMexWrapperFunctionStr)
                helicsMexMainFunctionElements.append(functionMexMainFunctionElementStr)
                if functionMapTuple != None:
                    helicsMapTuples.append(functionMapTuple)
        helicsMexStr += createBoilerPlate(self.__helicsParser.headerFiles, helicsMapTuples)
        for wrapperStr in helicsMexWrapperFunctions:
            helicsMexStr += wrapperStr
        helicsMexStr += createMexMain()
        for element in helicsMexMainFunctionElements:
            helicsMexStr += element
        helicsMexStr += closeBoilerPlate()
        with open(os.path.join(self.__rootDir, "helicsMex.cpp"), "w") as helicsMexFile:
            helicsMexFile.write(helicsMexStr)
        matlabBindingGeneratorLogger.info("MATLAB HELICS API successfully created!")
