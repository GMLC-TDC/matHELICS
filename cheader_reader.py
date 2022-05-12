from argparse import ArgumentParser
import json
import logging
import os
from typing import ClassVar, List, Dict, Any

import clang.cindex as cidx

clangLogger = logging.getLogger('HelicsHeaderParser')
clangLogger.setLevel(logging.DEBUG)
logFormatter = logging.Formatter('%(asctime)s - %(name)s - %(levelname)s - %(message)s')
clangLogFileHandler = logging.FileHandler('HELICSHeaderClangParseLog.log',mode='w',encoding='utf-8')
clangLogStreamHandler = logging.StreamHandler()
clangLogFileHandler.setLevel(logging.DEBUG)
clangLogFileHandler.setFormatter(logFormatter)
clangLogStreamHandler.setLevel(logging.INFO)
clangLogStreamHandler.setFormatter(logFormatter)
clangLogger.addHandler(clangLogFileHandler)
clangLogger.addHandler(clangLogStreamHandler)


class HelicsHeaderParser (object):
    """
        Class that will parse the HELICS C API headers and create other language bindings
        
        @ivar parsedInfo: a dictionary containing all the parsed cursors found in the HELICS C API headers
        
    """
    _types = {}
    def __init__(self, helicsHeaders: List[str]):
        """
            Constructor
        """
        HelicsHeaderParser._types["functions"] = {} 
        self.parsedInfo = {}
        self.headerFiles = helicsHeaders
        self.parseHelicsHeaderFiles(helicsHeaders)
    
    
    def _cursorInfo(self, node: cidx.Cursor) -> dict():
        """
            Helper function for parseHelicsHeaderFiles()
        """
        cursorInfoDict = {
             "kind" : node.kind.name,
             "spelling" : node.spelling,
             "location" : node.location.file.name,
             "type" : node.type.kind.spelling,
             "result_type" : node.result_type.kind.spelling,
             "brief_comment" : node.brief_comment,
        }
        cursor_range = node.extent
        cursorInfoDict["start_line"] = cursor_range.start.line
        cursorInfoDict["end_line"] = cursor_range.end.line
        if node.kind == cidx.CursorKind.FUNCTION_DECL:
            cursorInfoDict["raw_comment"] = node.raw_comment
            if node.result_type.kind == cidx.TypeKind.POINTER:
                pointerType = node.result_type.get_pointee()
                if pointerType.kind == cidx.TypeKind.POINTER:
                    cursorInfoDict["result_type"] = "Double Pointer"
                    cursorInfoDict["double_pointer_type"] = pointerType.get_pointee().kind.spelling + "_**"
                else:
                    cursorInfoDict["pointer_type"] = pointerType.kind.spelling + "_*"
            if node.result_type.kind == cidx.TypeKind.TYPEDEF:
                cursorInfoDict["result_type"] = node.result_type.get_typedef_name()
            if cursorInfoDict.get("result_type","") != "":
                if cursorInfoDict.get("result_type","") not in HelicsHeaderParser._types.keys():
                    HelicsHeaderParser._types[cursorInfoDict.get("result_type","")] = [cursorInfoDict.get("spelling","")]
                else:
                    HelicsHeaderParser._types.get(cursorInfoDict.get("result_type",""),[]).append(cursorInfoDict.get("spelling",""))
            if cursorInfoDict.get("pointer_type","") != "":
                if cursorInfoDict.get("pointer_type","") not in HelicsHeaderParser._types.keys():
                    HelicsHeaderParser._types[cursorInfoDict.get("pointer_type","")] = [cursorInfoDict.get("spelling","")]
                else:
                    HelicsHeaderParser._types.get(cursorInfoDict.get("pointer_type",""),[]).append(cursorInfoDict.get("spelling",""))
            if cursorInfoDict.get("double_pointer_type","") != "":
                if cursorInfoDict.get("double_pointer_type","") not in HelicsHeaderParser._types.keys():
                    HelicsHeaderParser._types[cursorInfoDict.get("double_pointer_type","")] = [cursorInfoDict.get("spelling","")]
                else:
                    HelicsHeaderParser._types.get(cursorInfoDict.get("double_pointer_type",""),[]).append(cursorInfoDict.get("spelling",""))
            cursorInfoDict["arguments"] = {}
            argNum = 0
            for arg in node.get_arguments():
                cursorInfoDict["arguments"][argNum] = self._cursorInfo(arg)
                argNum += 1
            HelicsHeaderParser._types["functions"][cursorInfoDict.get("spelling","")] = cursorInfoDict["arguments"]
        if node.kind == cidx.CursorKind.PARM_DECL:
            if node.type.kind == cidx.TypeKind.TYPEDEF:
                cursorInfoDict["type"] = node.type.get_typedef_name()
            if node.type.kind == cidx.TypeKind.POINTER:
                typePointee = node.type.get_pointee()
                if typePointee.kind == cidx.TypeKind.TYPEDEF:
                    cursorInfoDict["pointer_type"] = typePointee.get_typedef_name() + "_*"
                elif typePointee.kind == cidx.TypeKind.POINTER:
                    cursorInfoDict["type"] = "Double Pointer"
                    cursorInfoDict["double_pointer_type"] = typePointee.get_pointee().kind.spelling + "_**"
                else:
                    cursorInfoDict["pointer_type"] = typePointee.kind.spelling + "_*"
            if cursorInfoDict.get("type","") != "":
                if cursorInfoDict.get("type","") not in HelicsHeaderParser._types.keys():
                    HelicsHeaderParser._types[cursorInfoDict.get("type","")] = [cursorInfoDict.get("spelling","")]
                else:
                    HelicsHeaderParser._types.get(cursorInfoDict.get("type",""),[]).append(cursorInfoDict.get("spelling",""))
            if cursorInfoDict.get("pointer_type","") != "":
                if cursorInfoDict.get("pointer_type","") not in HelicsHeaderParser._types.keys():
                    HelicsHeaderParser._types[cursorInfoDict.get("pointer_type","")] = [cursorInfoDict.get("spelling","")]
                else:
                    HelicsHeaderParser._types.get(cursorInfoDict.get("pointer_type",""),[]).append(cursorInfoDict.get("spelling",""))
            if cursorInfoDict.get("double_pointer_type","") != "":
                if cursorInfoDict.get("double_pointer_type","") not in HelicsHeaderParser._types.keys():
                    HelicsHeaderParser._types[cursorInfoDict.get("double_pointer_type","")] = [cursorInfoDict.get("spelling","")]
                else:
                    HelicsHeaderParser._types.get(cursorInfoDict.get("double_pointer_type",""),[]).append(cursorInfoDict.get("spelling",""))                  
        if node.kind == cidx.CursorKind.TYPEDEF_DECL or node.type.kind == cidx.TypeKind.TYPEDEF:
            cursorInfoDict["type"] = node.underlying_typedef_type.spelling
            if cursorInfoDict["type"] == "":
                cursorInfoDict["type"] = node.type.get_typedef_name()
        if node.kind == cidx.CursorKind.ENUM_DECL:
            cursorInfoDict["enumerations"] = {}
            enumNum = 0
            for i in node.get_children():
                cursorInfoDict["enumerations"][enumNum] = self._cursorInfo(i)
                enumNum += 1
        if node.kind == cidx.CursorKind.ENUM_CONSTANT_DECL:
            cursorInfoDict["value"] = node.enum_value
        if node.kind == cidx.CursorKind.VAR_DECL:
            tokens = []
            for t in node.get_tokens():
                tokens.append(t.spelling)
            if tokens[len(tokens)-2] == "-":
                value = tokens[len(tokens)-2]+tokens[len(tokens)-1]
            else:
                value = tokens[len(tokens)-1]
            try:
                cursorInfoDict["value"] = json.loads(value)
            except:
                cursorInfoDict["value"] = value
        if node.kind == cidx.CursorKind.STRUCT_DECL:
            cursorInfoDict["members"] = {}
            memberNum = 0
            for i in node.get_children():
                cursorInfoDict["members"][memberNum] = self._cursorInfo(i)
                memberNum += 1
        if node.kind == cidx.CursorKind.MACRO_DEFINITION:
            value = ''
            for t in node.get_tokens():
                value = t.spelling
            try:
                cursorInfoDict["value"] = json.loads(value)
            except:
                cursorInfoDict["value"] = value
        return cursorInfoDict
        
        
    def parseHelicsHeaderFiles(self, helicsHeaders: List[str]) -> None:
        """
            Function that parses the HELICS C header files
            @param helicsHeaders: A list of the HELICS C header files to parse
        """
        ignoredMacros = ["HELICS_C_API_H_", "HELICS_EXPORT", "HELICS__DEPRECATED"]
        idx = cidx.Index.create()
        cursorNum = 0
        for headerFile in helicsHeaders:
            tu = idx.parse(headerFile, options=cidx.TranslationUnit.PARSE_DETAILED_PROCESSING_RECORD)
            for c in tu.cursor.get_children():
                location_file = c.location.file
                if c.location.file != None:
                    if c.location.file.name == headerFile and c.displayname not in ignoredMacros:
                        self.parsedInfo[cursorNum] = self._cursorInfo(c)
                        cursorNum += 1
            deletekeys = []
            for key in self.parsedInfo.keys():
                
                if self.parsedInfo[key]["spelling"] == "":
                    for i in self.parsedInfo.keys():
                        if i != key:
                            if self.parsedInfo[key]["start_line"] == self.parsedInfo[i]["start_line"] and self.parsedInfo[key]["end_line"] == self.parsedInfo[i]["end_line"]:
                                self.parsedInfo[key]["spelling"] = self.parsedInfo[i]["spelling"]
                                deletekeys.append(i)
            for key in deletekeys:
                del self.parsedInfo[key]
        clangLogger.info("Clang successfully parsed the HELICS header files!")
        clangLogger.debug(f"The clang parser result:\n{json.dumps(self.parsedInfo, indent=4, sort_keys=True)}\n{json.dumps(HelicsHeaderParser._types, indent=4, sort_keys=True)}")

    
    
    def createPythonBindings(self) -> str:
        """
            Function that creates the HELICS Python Bindings
        """
        clangLogger.debug("Creating the python HELICS library API.")
        def createBoilerPlate() -> str:
            """
                Create Python Binding boiler plate string 
            """
            clangLogger.debug("Entering createBoilerPlate.")
            boilerPlateStr = "import enum\n"
            boilerPlateStr += "import signal\n"
            boilerPlateStr += "import sys\n"
            boilerPlateStr += "try:\n\tfrom typing import List, Tuple, Union\nexcept ImportError:\n\tpass\n\n"
            boilerPlateStr += "from . import _build\n\n\n"
            boilerPlateStr += "lib = _build.lib\n"
            boilerPlateStr += "ffi = _build.ffi\n\n\n" 
            boilerPlateStr += "def signal_handler(sig, frame):\n"
            boilerPlateStr += "\thelicsCloseLibrary()\n"
            boilerPlateStr += "\tprint(\"User pressed 'CTRL-C'. Exiting...\")\n"
            boilerPlateStr += "\tsys.exit(0)\n\n\n"
            boilerPlateStr += "signal.signal(signal.SIGINT, signal_handler)\n\n\n"
            boilerPlateStr += "class _HelicsCHandle:\n"
            boilerPlateStr += "\tdef __init__(self, handle):\n"
            boilerPlateStr += "\t\tself.handle = handle\n\n\n"
            boilerPlateStr += "class HelicsException(Exception):\n"
            boilerPlateStr += "\tpass\n\n\n"
            clangLogger.debug("Exiting createBoilerPlate.")
            return boilerPlateStr
        
        def createMacro(macroDict: dict()) -> str:
            """
                Create Python Binding for each C Macro definition
            """
            clangLogger.debug(f"creating Python macro definition for:\n{json.dumps(macroDict,indent=4,sort_keys=True)}")
            macroSpelling = macroDict.get("spelling","")
            macroComment = macroDict.get("brief_comment","")
            macroValue = macroDict.get("value","")
            if macroComment != None:
                return f"{macroSpelling} = {macroValue}\t#{macroComment}\n\n\n"
            else:
                return f"{macroSpelling} = {macroValue}\n\n\n"
        
        
        def createEnum(enumDict: dict()) -> str:
            """
                Create Python Binding Enumerations for each C Enumeration
            """
            clangLogger.debug(f"creating Python enum definition for:\n{json.dumps(enumDict,indent=4,sort_keys=True)}")
            enumSpelling = enumDict.get('spelling','')
            enumComment = enumDict.get('brief_comment','')
            enumStr = f"class {enumSpelling}(enum.IntEnum):\n"
            enumStr += f"\t\"\"\" {enumComment}\n\n\tAttributes:"
            docStrBody = ""
            enumStrBody = ""
            enumKeywordGlobalVarBody = ""
            enumKeywordGlobalVarLowerCaseBody = ""
            for enumKey in enumDict.get('enumerations',{}).keys():
                keywordSpelling = enumDict.get('enumerations',{}).get(enumKey,{}).get('spelling','')
                keywordValue = enumDict.get('enumerations',{}).get(enumKey,{}).get('value')
                keywordComment = enumDict.get('enumerations',{}).get(enumKey,{}).get('brief_comment','')
                if keywordComment == None:
                    keywordComment = ''
                docStrBody += f"\n\t\t{keywordSpelling}: value:{keywordValue}\t{keywordComment}"
                enumStrBody += f"\n\t{keywordSpelling} = {keywordValue}"
                enumKeywordGlobalVarBody += f"\n{keywordSpelling} = {enumSpelling}.{keywordSpelling}"
                enumKeywordGlobalVarLowerCaseBody += f"\n{keywordSpelling.lower()} = {enumSpelling}.{keywordSpelling}"
            enumStr += docStrBody
            enumStr += "\n\t\"\"\"\n"
            enumStr += enumStrBody
            enumStr += "\n\n"
            enumStr += enumKeywordGlobalVarBody
            enumStr += "\n"
            enumStr += enumKeywordGlobalVarLowerCaseBody
            enumStr += "\n\n\n"
            return enumStr
        
        
        def createTypeDef(typedefDict: dict()) -> str:
            """
                Create Python Classes for each HELICS C TypeDef Void *
            """
            clangLogger.debug(f"creating Python typedef definition for:\n{json.dumps(typedefDict,indent=4,sort_keys=True)}")
            typedefSpelling = typedefDict.get('spelling','')
            typedefComment = typedefDict.get('brief_comment','')
            if typedefSpelling != "HelicsComplex" and typedefSpelling != "HelicsError":
                if typedefDict.get('type', '') == "void *":
                    typedefStr = f"class {typedefSpelling}(_HelicsCHandle):\n"
                    typedefStr += f"\t\"\"\"\n\t{typedefComment}\n\t\"\"\"\n"
                    typedefStr += "\tpass\n\n\n"
                    return  typedefStr
                elif typedefDict.get('type', '') == "double":
                    return f"{typedefSpelling} = float\n\n\n"
                elif typedefDict.get('type', '') == "int":
                    return f"{typedefSpelling} = int\n\n\n"
                else:
                    raise RuntimeError(f"unhandled typedef type for {typedefSpelling}. type is {typedefDict.get('type','unknown')}")
            return ""
            
            
        def createStruct(structDict: dict()) -> str:
            """
            """
            clangLogger.debug(f"creating Python struct definition for:\n{json.dumps(structDict,indent=4,sort_keys=True)}")
            structSpelling = structDict.get('spelling','')
            structComment = structDict.get('brief_comment','')
            structStr = f"\"\"\"\n\t{structComment}\n\n\tmembers:\n"
            for mem in structDict.get('members',{}).values():
                structStr += f"\t{mem.get('spelling','')}\t{mem.get('brief_comment','')}\n"
            structStr += f"\"\"\"\n{structDict.get('spelling','')} = ffi.new(\"{structDict.get('spelling','')} *\")\n\n\n"
            return structStr
            
                
        def createVarDeclaration(varDict: dict()) -> str:
            """
            """
            clangLogger.debug(f"creating Python var definition for:\n{json.dumps(varDict,indent=4,sort_keys=True)}")
            varSpelling = varDict.get('spelling','')
            varValue = varDict.get('value')
            varComment = varDict.get('brief_comment','')
            if varComment != None:
                return f"{varSpelling} = {varValue}\t#{varComment}\n\n\n"
            else:
                return f"{varSpelling} = {varValue}\n\n\n"
            
                
        def createFunctionDeclaration(functionDict: dict()) -> str:
            clangLogger.debug(f"creating Python function definition for:\n{json.dumps(functionDict,indent=4,sort_keys=True)}")
            modifiedPythonFunctionList = [
                "helicsCreateBrokerFromArgs",
                "helicsCreateCoreFromArgs",
                "helicsFederateInfoLoadFromArgs",
                "helicsEndpointSendBytes",
                "helicsEndpointSendBytesAt",
                "helicsEndpointSendBytesTo",
                "helicsEndpointSendBytesToAt",
                "helicsErrorClear",
                "helicsFederateRequestTimeIterative",
                "helicsFederateRequestTimeIterativeComplete",
                "helicsInputGetBytes",
                "helicsInputGetComplex",
                "helicsInputGetComplexObject",
                "helicsInputGetNamedPoint",
                "helicsInputGetString",
                "helicsInputGetVector",
                "helicsInputSetDefaultBytes",
                "helicsInputSetDefaultComplex",
                "helicsInputSetDefaultVector",
                "helicsMessageAppendData",
                "helicsMessageGetBytes",
                "helicsMessageSetData",
                "helicsPublicationPublishBytes",
                "helicsPublicationPublishComplex",
                "helicsPublicationPublishVector",
                "helicsQueryBufferFill",
                "helicsGetPropertyIndex",
                "helicsGetFlagIndex",
                "helicsGetOptionIndex",
                "helicsGetOptionValue"
            ]
            if functionDict.get("spelling") in modifiedPythonFunctionList:
                return createModifiedPythonFunction(functionDict)
            functionReturnInfo = getReturnInfo(functionDict)
            argumentInfo = {}
            for a in functionDict.get('arguments',{}).keys():
                argumentInfo[a] = {}
                argumentInfo[a]["name"] = functionDict.get('arguments',{}).get(a,{}).get('spelling','')
                if argumentInfo[a]["name"] == 'str':
                    argumentInfo[a]["name"] = 'string'
                    functionDict['arguments'][a]['spelling'] = 'string'
                argumentInfo[a]["type"] = getPythonType(functionDict.get('arguments',{}).get(a,{}))
                argumentInfo[a]["arg_initialization"] = getArgInitializationText(functionDict.get('arguments',{}).get(a,{}))
                argumentInfo[a]["arg_function_call"] = getArgFunctionCallText(functionDict.get('arguments',{}).get(a,{}))
                argumentInfo[a]["arg_post_function_call"] = getArgPostFunctionCallText(functionDict.get('arguments',{}).get(a,{}))
            functionStr = f'def {functionDict.get("spelling","")}('
            for a in argumentInfo.keys():
                if a == 0:
                        functionStr += f'{argumentInfo[a].get("name","")}{argumentInfo[a].get("type","")}'
                else:
                    if argumentInfo[a].get("type","") != ": HelicsError":
                        functionStr += f', {argumentInfo[a].get("name","")}{argumentInfo[a].get("type","")}'
            functionStr += f'){functionReturnInfo.get("typing_hint","")}:\n'
            functionComment = functionDict.get("raw_comment")
            functionComment = functionComment.replace("/**", '\t"""')
            functionComment = functionComment.replace("\n */", "\n")
            functionComment = functionComment.replace("\n * ", "\n\t\t")
            functionComment = functionComment.replace(" *", "")
            if "@forcpponly" in functionComment:
                funComPart = functionComment.partition("\t\t@forcpponly")
                funComPart1 = funComPart[2].partition("@endforcpponly\n")
                if "@forcpponly in funComPart1[2]":
                    funComPart2 = funComPart1[2].partition("\t\t@forcpponly")
                    functionStr += funComPart[0] + funComPart2[0] + '\t"""\n'
                else:
                    functionStr += funComPart[0] + funComPart1[2] + '\n\t"""\n'
            else:
                functionStr += functionComment + '\t"""\n'
            functionStr += f'\tfn = getattr(lib, "{functionDict.get("spelling","")}")\n'
            for a in argumentInfo.keys():
                functionStr += argumentInfo[a]["arg_initialization"]
            if functionDict.get("result_type",'') == "Void":
                functionStr += "\tfn("
            else:
                functionStr += "\tresult = fn("
            for a in argumentInfo.keys():
                if a == 0:
                    functionStr += f'{argumentInfo[a].get("arg_function_call","")}'
                else:
                    functionStr += f', {argumentInfo[a].get("arg_function_call","")}'
            functionStr += ")\n"
            for a in argumentInfo.keys():
                functionStr += argumentInfo[a]["arg_post_function_call"]
            functionStr += functionReturnInfo.get("return_line")
            return functionStr
        
        
        def getReturnInfo(functionDict: dict()) -> dict():
            returnTextMap = {
                "Char_S": "\treturn result.decode()\n\n\n",
                "Char_S_*": "\treturn ffi.string(result).decode()\n\n\n",
                "Double": "\treturn result\n\n\n",
                "Int": "\treturn result\n\n\n",
                "Void": "\n\n",
                "Void_*": "\treturn result\n\n\n",
                "HelicsBool": "\treturn result==1\n\n\n",
                "HelicsBroker": "\treturn HelicsBroker(result)\n\n\n",
                "HelicsComplex": "\treturn complex(result.real,result.imag)\n\n\n",
                "HelicsCore": "\treturn HelicsCore(result)\n\n\n",
                "HelicsDataBuffer": "\treturn HelicsDataBuffer(result)\n\n\n",
                "HelicsEndpoint": "\treturn HelicsEndpoint(result)\n\n\n",
                "HelicsError": '\treturn ffi.new("HelicsError *",result)\n\n\n',
                "HelicsFederate": "\treturn HelicsFederate(result)\n\n\n",
                "HelicsFederateInfo": "\treturn HelicsFederateInfo(result)\n\n\n",
                "HelicsFederateState": "\treturn HelicsFederateState(result)\n\n\n",
                "HelicsFilter": "\treturn HelicsFilter(result)\n\n\n",
                "HelicsInput": "\treturn HelicsInput(result)\n\n\n",
                "HelicsIterationResult": "\treturn HelicsIterationResult(result)\n\n\n",
                "HelicsMessage": "\treturn HelicsMessage(result)\n\n\n",
                "HelicsPublication": "\treturn HelicsPublication(result)\n\n\n",
                "HelicsQuery": "\treturn HelicsQuery(result)\n\n\n",
                "HelicsTime": "\treturn result\n\n\n",
                "HelicsTranslator": "\treturn HelicsTranslator(result)\n\n\n",
                "int32_t": "\return result\n\n\n",
                "int64_t": "\treturn result\n\n\n"
            }
            functionReturnInfo = {}
            functionReturnInfo["typing_hint"] = getPythonType(functionDict)            
            returnType = functionDict.get('double_pointer_type')
            if returnType == None:
                returnType = functionDict.get('pointer_type')
                if returnType== None:
                    returnType = functionDict.get('result_type')
            functionReturnInfo["return_line"] = returnTextMap.get(returnType)
            if functionReturnInfo["return_line"] == None:
                raise RuntimeError(f"Unhandled c return type conversion for {returnType}. Please update the returnTextMap. functionDict: {json.dumps(functionDict, indent=4, sort_keys=True)}")
            return functionReturnInfo
        
        
        def getPythonType(argDict: dict()) -> str:
            pythonTypeMap = {
                "Char_S": "str",
                "Char_S_*": "str",
                "Char_S_**": "List[str]",
                "Double": "float",
                "Double_*": "float",
                "FunctionProto_*": None,
                "IncompleteArray": "List[float]",
                "Int": "int",
                "Int_*": "int",
                "Void": None,
                "Void_*": None,
                "HelicsBool": "bool",
                "HelicsBroker": "HelicsBroker",
                "HelicsComplex": "complex",
                "HelicsCore": "HelicsCore",
                "HelicsDataTypes": "HelicsDataTypes",
                "HelicsDataBuffer": "HelicsDataBuffer",
                "HelicsEndpoint": "HelicsEndpoint",
                "HelicsError": "HelicsError",
                "HelicsError_*": "HelicsError",
                "HelicsFederate": "HelicsFederate",
                "HelicsFederateInfo": "HelicsFederateInfo",
                "HelicsFederateState": "HelicsFederateState",
                "HelicsFilter": "HelicsFilter",
                "HelicsFilterTypes": "HelicsFilterTypes",
                "HelicsInput": "HelicsInput",
                "HelicsIterationRequest": "HelicsIterationRequest",
                "HelicsIterationResult": "HelicsIterationResult",
                "HelicsIterationResult_*": "HelicsIterationResult",
                "HelicsMessage": "HelicsMessage",
                "HelicsPublication": "HelicsPublication",
                "HelicsQuery": "HelicsQuery",
                "HelicsQueryBuffer": "HelicsQueryBuffer",
                "HelicsTime": "HelicsTime",
                "HelicsTranslator": "HelicsTranslator",
                "HelicsTranslatorTypes": "HelicsTranslatorTypes",
                "int32_t": "int",
                "int64_t": "int"
            }
            if argDict.get("result_type",'Invalid') == "Invalid":
                typeHintPrependStr = ": "
                nativeType = argDict.get('type','')
            else:
                typeHintPrependStr = " -> "
                nativeType = argDict.get("result_type","")
            if nativeType == "Pointer":
                nativeType = argDict.get("pointer_type","")
            elif nativeType == "Double Pointer":
                nativeType = argDict.get("double_pointer_type","")
            pythonType = pythonTypeMap.get(nativeType,"Invalid")
            if pythonType == "Invalid":
                raise RuntimeError(f"Unhandled c type conversion for {nativeType}. Please update the pythonTypeMap")
            if pythonType == None:
                return ""
            else:
                return f"{typeHintPrependStr}{pythonType}"
        
        
        def getArgInitializationText(argDict: dict()) -> str:
            argInitTextMap = {
                "Char_S": "",
                "Char_S_*": "",
                "Char_S_**":'\targc = len(arguments)\n\targv = ffi.new(f"char*[{argc}]")\n\tfor i, s in enumerate(arguments):\n\t\targv[i] = ffi.new("char[]",s.encode())\n',
                "Double": "",
                "Double_*": "",
                "FunctionProto_*": "",
                "IncompleteArray": "",
                "Int": "",
                "Int_*": f'\t{argDict.get("spelling","")} = ffi.new("int[1]")\n',
                "Void_*": "",
                "HelicsBool": "",
                "HelicsBroker": "",
                "HelicsCore": "",
                "HelicsDataBuffer": "",
                "HelicsDataTypes": "",
                "HelicsEndpoint": "",
                "HelicsError_*": f'\t{argDict.get("spelling","")} = helicsErrorInitialize()\n',
                "HelicsFederate": "",
                "HelicsFederateInfo": "",
                "HelicsFilter": "",
                "HelicsFilterTypes": "",
                "HelicsInput": "",
                "HelicsIterationRequest": "",
                "HelicsIterationResult_*": f'\t{argDict.get("spelling","")} = ffi.new("HelicsIterationResult *")\n', 
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
            argInitText = argInitTextMap.get(argType)
            if argInitText == None:
                raise RuntimeError(f"Unhandled c argument type conversion for {argType}. Please update the argInitTextMap")
            return argInitText    
        
        
        def getArgFunctionCallText(argDict: dict()) -> str:
            argFunctionCallTextMap = {
                "Char_S": f'{argDict.get("spelling","")}.encode()',
                "Char_S_*": f'ffi.new("char[]",{argDict.get("spelling","")}.encode())',
                "Char_S_**":f'{argDict.get("spelling","")}',
                "Double": f'{argDict.get("spelling","")}',
                "Double_*": f'{argDict.get("spelling","")}',
                "FunctionProto_*": f'{argDict.get("spelling","")}',
                "IncompleteArray": f'{argDict.get("spelling","")}',
                "Int": f'{argDict.get("spelling","")}',
                "Int_*": f'{argDict.get("spelling","")}',
                "Void_*": f'{argDict.get("spelling","")}',
                "HelicsBool": f'{argDict.get("spelling","")}',
                "HelicsBroker": f'{argDict.get("spelling","")}.handle',
                "HelicsCore": f'{argDict.get("spelling","")}.handle',
                "HelicsDataBuffer": f'{argDict.get("spelling","")}.handle',
                "HelicsDataTypes": f'HelicsDataTypes({argDict.get("spelling","")})',
                "HelicsEndpoint": f'{argDict.get("spelling","")}.handle',
                "HelicsError_*": f'{argDict.get("spelling","")}',
                "HelicsFederate": f'{argDict.get("spelling","")}.handle',
                "HelicsFederateInfo": f'{argDict.get("spelling","")}.handle',
                "HelicsFilter": f'{argDict.get("spelling","")}.handle',
                "HelicsFilterTypes": f'HelicsFilterTypes({argDict.get("spelling","")})',
                "HelicsInput": f'{argDict.get("spelling","")}.handle',
                "HelicsIterationRequest": f'HelicsIterationRequest({argDict.get("spelling","")})',
                "HelicsIterationResult_*": f'{argDict.get("spelling","")}', 
                "HelicsMessage": f'{argDict.get("spelling","")}.handle',
                "HelicsPublication": f'{argDict.get("spelling","")}.handle',
                "HelicsQuery": f'{argDict.get("spelling","")}.handle',
                "HelicsQueryBuffer": f'{argDict.get("spelling","")}.handle',
                "HelicsTime": f'{argDict.get("spelling","")}',
                "HelicsTranslator": f'{argDict.get("spelling")}.handle',
                "HelicsTranslatorTypes": f'HelicsTranslatorTypes({argDict.get("spelling","")})',
                "int32_t": f'{argDict.get("spelling","")}',
                "int64_t": f'{argDict.get("spelling","")}'
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
        
        
        def getArgPostFunctionCallText(argDict: dict()) -> str:
            argPostFunctionCallTextMap = {
                "Char_S": "",
                "Char_S_*": "",
                "Char_S_**": "",
                "Double": "",
                "Double_*": "",
                "FunctionProto_*": "",
                "IncompleteArray": "",
                "Int": "",
                "Int_*": "",
                "Void_*": "",
                "HelicsBool": "",
                "HelicsBroker": "",
                "HelicsCore": "",
                "HelicsDataBuffer": "",
                "HelicsDataTypes": "",
                "HelicsEndpoint": "",
                "HelicsError_*": f'\tif {argDict.get("spelling","")}.error_code != 0:\n\t\traise HelicsException("[" + str({argDict.get("spelling","")}.error_code) + "] " + ffi.string({argDict.get("spelling","")}.message).decode())\n',
                "HelicsFederate": "",
                "HelicsFederateInfo": "",
                "HelicsFilter": "",
                "HelicsFilterTypes": "",
                "HelicsInput": "",
                "HelicsIterationRequest": "",
                "HelicsIterationResult_*": "", 
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
            argPostFunctionCallText = argPostFunctionCallTextMap.get(argType)
            if argPostFunctionCallText == None:
                raise RuntimeError(f"Unhandled c argument type conversion for {argType}. Please update the argPostFunctionCallTextMap")
            return argPostFunctionCallText
        
        
        def createModifiedPythonFunction(functionDict: dict()) -> str:
            if functionDict.get("spelling", "") == "helicsCreateCoreFromArgs":
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
                functionStr = "def helicsCreateCoreFromArgs(type: str, name: str, arguments: List[str]) -> HelicsCore:\n"
                functionStr += "\t\"\"\"\n"
                functionStr += "\t\tCreate a core object by passing command line arguments.\n\n"
                functionStr += "\t\t@param type The type of the core to create.\n"
                functionStr += "\t\t@param name The name of the core. It can be an empty string to have a name automatically assigned..\n"
                functionStr += "\t\t@param arguments The list of string values from a command line.\n\n"
                functionStr += "\t\t@return A HelicsCore object.\n\t\"\"\"\n"
                functionStr += "\tfn = getattr(lib, \"helicsCreateCoreFromArgs\")\n"
                functionStr += "\terr = helicsErrorInitialize()\n"
                functionStr += "\targc = len(arguments)\n"
                functionStr += "\targv = ffi.new(\"char*[{argc}]\".format(argc=argc))\n"
                functionStr += "\tfor i, s in enumerate(arguments):\n"
                functionStr += "\t\targv[i] = ffi.new(\"char[]\",s.encode())\n"
                functionStr += "\tresult = fn(ffi.new(\"char[]\",type.encode()), ffi.new(\"char[]\",name.encode()), argc, argv, err)\n"
                functionStr += "\tif err.error_code != 0:\n"
                functionStr += "\t\traise HelicsException(\"[\" + str(err.error_code) + \"] \" + ffi.string(err.message).decode())\n"
                functionStr += "\treturn HelicsCore(result)\n\n\n"
            elif functionDict.get("spelling", "") == "helicsCreateBrokerFromArgs":
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
                functionStr = "def helicsCreateBrokerFromArgs(type: str, name: str, arguments: List[str]) -> HelicsBroker:\n"
                functionStr += "\t\"\"\"\n"
                functionStr += "\t\tCreate a core object by passing command line arguments.\n\n"
                functionStr += "\t\t@param type The type of the core to create.\n"
                functionStr += "\t\t@param name The name of the core. It can be an empty string to have a name automatically assigned.\n"
                functionStr += "\t\t@param arguments The list of string values from a command line.\n\n"
                functionStr += "\t\t@return a HelicsBroker object.\n"
                functionStr += "\t\"\"\"\n"
                functionStr += "\tfn = getattr(lib, \"helicsCreateBrokerFromArgs\")\n"
                functionStr += "\terr = helicsErrorInitialize()\n"
                functionStr += "\targc = len(arguments)\n"
                functionStr += "\targv = ffi.new(\"char*[{argc}]\".format(argc=argc))\n"
                functionStr += "\tfor i, s in enumerate(arguments):\n"
                functionStr += "\t\targv[i] = ffi.new(\"char[]\",s.encode())\n"
                functionStr += "\tresult = fn(ffi.new(\"char[]\",type.encode()), ffi.new(\"char[]\",name.encode()), argc, argv, err)\n"
                functionStr += "\tif err.error_code != 0:\n"
                functionStr += "\t\traise HelicsException(\"[\" + str(err.error_code) + \"] \" + ffi.string(err.message).decode())\n"
                functionStr += "\treturn HelicsBroker(result)\n\n\n"
            elif functionDict.get("spelling", "") == "helicsFederateInfoLoadFromArgs":
                #check to see if function signiture changed
                argNum = len(functionDict.get("arguments", {}).keys())
                if argNum != 4:
                    raise RuntimeError("the function signature for helicsFederateInfoLoadFromArgs has changed!")
                arg0 = functionDict.get("arguments", {}).get(0, {})
                if arg0.get("spelling","") != "fi" or arg0.get("type", "") != "HelicsFederateInfo":
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
                functionStr = "def helicsFederateInfoLoadFromArgs(fi: HelicsFederateInfo, arguments: List[str]):\n"
                functionStr += "\t\"\"\"\n"
                functionStr += "\t\tLoad federate info from command line arguments.\n\n"
                functionStr += "\t\t@param fi A federateInfo object.\n"
                functionStr += "\t\t@param arguments A list of strings from the command line.\n"
                functionStr += "\t\"\"\"\n"
                functionStr += "\tfn = getattr(lib, \"helicsFederateInfoLoadFromArgs\")\n"
                functionStr += "\terr = helicsErrorInitialize()\n"
                functionStr += "\targc = len(arguments)\n"
                functionStr += "\targv = ffi.new(\"char*[{argc}]\".format(argc=argc))\n"
                functionStr += "\tfor i, s in enumerate(arguments):\n"
                functionStr += "\t\targv[i] = ffi.new(\"char[]\",s.encode())\n"
                functionStr += "\tfn(fi.handle, argc, argv, err)\n"
                functionStr += "\tif err.error_code != 0:\n"
                functionStr += "\t\traise HelicsException(\"[\" + str(err.error_code) + \"] \" + ffi.string(err.message).decode())\n\n\n"
            elif functionDict.get("spelling", "") == "helicsEndpointSendBytes":
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
                functionStr = "def helicsEndpointSendBytes(endpoint: HelicsEndpoint, data: bytes):\n"
                functionStr += "\t\"\"\"\n"
                functionStr += "\t\tSend a message to the targeted destinations.\n\n"
                functionStr += "\t\t@param endpoint The endpoint to send the data from.\n"
                functionStr += "\t\t@param data The data to send.\n"
                functionStr += "\t\"\"\"\n"
                functionStr += "\tfn = getattr(lib, \"helicsEndpointSendBytes\")\n"
                functionStr += "\tif isinstance(data, str):\n"
                functionStr += "\t\tdata = data.encode()\n"
                functionStr += "\tif not isinstance(data, bytes):\n"
                functionStr += "\t\traise Exception(\"\"\"data must be of type `bytes`. Got {t} instead. Try converting it to bytes (e.g. `\"hello world\".encode()`\"\"\".format(t=type(data)))\n"
                functionStr += "\tinputDataLength = len(data)\n"
                functionStr += "\terr = helicsErrorInitialize()\n"
                functionStr += "\tfn(endpoint.handle, data, inputDataLength, err)\n"
                functionStr += "\tif err.error_code != 0:\n"
                functionStr += "\t\traise HelicsException(\"[\" + str(err.error_code) + \"] \" + ffi.string(err.message).decode())\n\n\n"
            elif functionDict.get("spelling", "") == "helicsEndpointSendBytesAt":
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
                functionStr = "def helicsEndpointSendBytesAt(endpoint: HelicsEndpoint, data: bytes, time: HelicsTime):\n"
                functionStr += "\t\"\"\"\n"
                functionStr += "\t\tSend a message to the targeted destinations at a specified time.\n\n"
                functionStr += "\t\t@param endpoint The endpoint to send the data from.\n"
                functionStr += "\t\t@param data The data to send.\n"
                functionStr += "\t\t@param time The time to send the message at.\n"
                functionStr += "\t\"\"\"\n"
                functionStr += "\tfn = getattr(lib, \"helicsEndpointSendBytesAt\")\n"
                functionStr += "\tif isinstance(data, str):\n"
                functionStr += "\t\tdata = data.encode()\n"
                functionStr += "\tif not isinstance(data, bytes):\n"
                functionStr += "\t\traise Exception(\"\"\"data must be of type `bytes`. Got {t} instead. Try converting it to bytes (e.g. `\"hello world\".encode()`\"\"\".format(t=type(data)))\n"
                functionStr += "\tinputDataLength = len(data)\n"
                functionStr += "\terr = helicsErrorInitialize()\n"
                functionStr += "\tfn(endpoint.handle, data, inputDataLength, time, err)\n"
                functionStr += "\tif err.error_code != 0:\n"
                functionStr += "\t\traise HelicsException(\"[\" + str(err.error_code) + \"] \" + ffi.string(err.message).decode())\n\n\n"
            elif functionDict.get("spelling", "") == "helicsEndpointSendBytesTo":
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
                functionStr = "def helicsEndpointSendBytesTo(endpoint: HelicsEndpoint, data: bytes, dst: str):\n"
                functionStr += "\t\"\"\"\n"
                functionStr += "\t\tSend a message to the specified destination.\n\n"
                functionStr += "\t\t@param endpoint The endpoint to send the data from.\n"
                functionStr += "\t\t@param data The data to send.\n"
                functionStr += "\t\t@param dst The destination to send the message to.\n"
                functionStr += "\t\"\"\"\n"
                functionStr += "\tfn = getattr(lib, \"helicsEndpointSendBytesTo\")\n"
                functionStr += "\tif isinstance(data, str):\n"
                functionStr += "\t\tdata = data.encode()\n"
                functionStr += "\tif not isinstance(data, bytes):\n"
                functionStr += "\t\traise Exception(\"\"\"data must be of type `bytes`. Got {t} instead. Try converting it to bytes (e.g. `\"hello world\".encode()`\"\"\".format(t=type(data)))\n"
                functionStr += "\tinputDataLength = len(data)\n"
                functionStr += "\terr = helicsErrorInitialize()\n"
                functionStr += "\tfn(endpoint.handle, data, inputDataLength, ffi.new(\"char[]\",dst.encode()), err)\n"
                functionStr += "\tif err.error_code != 0:\n"
                functionStr += "\t\traise HelicsException(\"[\" + str(err.error_code) + \"] \" + ffi.string(err.message).decode())\n\n\n"
            elif functionDict.get("spelling", "") == "helicsEndpointSendBytesToAt":
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
                functionStr = "def helicsEndpointSendBytesToAt(endpoint: HelicsEndpoint, data: bytes, dst: str, time: HelicsTime):\n"
                functionStr += "\t\"\"\"\n"
                functionStr += "\t\tSend a message to the specified destination at a specified time.\n\n"
                functionStr += "\t\t@param endpoint The endpoint to send the data from.\n"
                functionStr += "\t\t@param data The data to send.\n"
                functionStr += "\t\t@param dst The destination to send the message to.\n"
                functionStr += "\t\t@param time The time to send the message at.\n"
                functionStr += "\t\"\"\"\n"
                functionStr += "\tfn = getattr(lib, \"helicsEndpointSendBytesToAt\")\n"
                functionStr += "\tif isinstance(data, str):\n"
                functionStr += "\t\tdata = data.encode()\n"
                functionStr += "\tif not isinstance(data, bytes):\n"
                functionStr += "\t\traise Exception(\"\"\"data must be of type `bytes`. Got {t} instead. Try converting it to bytes (e.g. `\"hello world\".encode()`\"\"\".format(t=type(data)))\n"
                functionStr += "\tinputDataLength = len(data)\n"
                functionStr += "\terr = helicsErrorInitialize()\n"
                functionStr += "\tfn(endpoint.handle, data, inputDataLength, ffi.new(\"char[]\",dst.encode()), time, err)\n"
                functionStr += "\tif err.error_code != 0:\n"
                functionStr += "\t\traise HelicsException(\"[\" + str(err.error_code) + \"] \" + ffi.string(err.message).decode())\n\n\n"
            elif functionDict.get("spelling", "") == "helicsErrorClear":
                #check to see if function signiture changed
                argNum = len(functionDict.get("arguments", {}).keys())
                if argNum != 1:
                    raise RuntimeError("the function signature for helicsErrorClear has changed!")
                arg0 = functionDict.get("arguments", {}).get(0, {})
                if arg0.get("spelling","") != "err" or arg0.get("pointer_type", "") != "HelicsError_*":
                    raise RuntimeError("the function signature for helicsErrorClear has changed!")
                functionStr = "def helicsErrorClear(err):\n"
                functionStr += "\t\"\"\"\n"
                functionStr += "\t\tClear an error object.\n"
                functionStr += "\t\"\"\"\n"
                functionStr += "\tf = getattr(lib, \"helicsErrorClear\")\n"
                functionStr += "\tf(err)\n\n\n"
            elif functionDict.get("spelling", "") == "helicsFederateRequestTimeIterative":
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
                functionStr = "def helicsFederateRequestTimeIterative(fed: HelicsFederate, requestTime: HelicsTime, iterate: HelicsIterationRequest) -> Tuple[HelicsTime, HelicsIterationResult]:\n"
                functionStr += "\t\"\"\"\n"
                functionStr += "\t\tRequest an iterative time.\n\n"
                functionStr += "\t\t@details This call allows for finer grain control of the iterative process than /ref helicsFederateRequestTime. It takes a time and and\n"
                functionStr += "\t\titeration request, and returns a time and iteration status.\n\n"
                functionStr += "\t\t@param fed The federate to make the request of.\n"
                functionStr += "\t\t@param requestTime The next desired time.\n"
                functionStr += "\t\t@param iterate The requested iteration mode.\n\n"
                functionStr += "\t\t@return tuple of HelicsTime and HelicsIterationResult.\n"
                functionStr += "\t\"\"\"\n"
                functionStr += "\tfn = getattr(lib, \"helicsFederateRequestTimeIterative\")\n"
                functionStr += "\toutIteration = ffi.new(\"HelicsIterationResult *\")\n"
                functionStr += "\terr = helicsErrorInitialize()\n"
                functionStr += "\tresult = fn(fed.handle, requestTime, HelicsIterationRequest(iterate), outIteration, err)\n"
                functionStr += "\tif err.error_code != 0:\n"
                functionStr += "\t\traise HelicsException(\"[\" + str(err.error_code) + \"] \" + ffi.string(err.message).decode())\n"
                functionStr += "\treturn (result, HelicsIterationResult(outIteration[0]))\n\n\n"
            elif functionDict.get("spelling", "") == "helicsFederateRequestTimeIterativeComplete":
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
                functionStr = "def helicsFederateRequestTimeIterativeComplete(fed: HelicsFederate) -> Tuple[HelicsTime, HelicsIterationResult]:\n"
                functionStr += "\t\"\"\"\n"
                functionStr += "\t\tComplete an iterative time request asynchronous call.\n\n"
                functionStr += "\t\t@param fed The federate to make the request of.\n\n"
                functionStr += "\t\t@return tuple of HelicsTime and HelicsIterationResult.\n"
                functionStr += "\t\"\"\"\n"
                functionStr += "\tfn = getattr(lib, \"helicsFederateRequestTimeIterativeComplete\")\n"
                functionStr += "\toutIterate = ffi.new(\"HelicsIterationResult *\")\n"
                functionStr += "\terr = helicsErrorInitialize()\n"
                functionStr += "\tresult = fn(fed.handle, outIterate, err)\n"
                functionStr += "\tif err.error_code != 0:\n"
                functionStr += "\t\traise HelicsException(\"[\" + str(err.error_code) + \"] \" + ffi.string(err.message).decode())\n"
                functionStr += "\treturn (result, HelicsIterationResult(outIterate[0]))\n\n\n"
            elif functionDict.get("spelling", "") == "helicsInputGetBytes":
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
                functionStr = "def helicsInputGetBytes(ipt: HelicsInput) -> bytes:\n"
                functionStr += "\t\"\"\"\n"
                functionStr += "\t\tGet the raw data for the latest value of a subscription.\n\n"
                functionStr += "\t\t@param ipt The input to get the data for.\n\n"
                functionStr += "\t\t@return  raw Bytes of the value, the value is uninterpreted raw bytes.\n"
                functionStr += "\t\"\"\"\n"
                functionStr += "\tfn = getattr(lib, \"helicsInputGetBytes\")\n"
                functionStr += "\tmaxDataLen = helicsInputGetByteCount(ipt) + 1024\n"
                functionStr += "\tdata = ffi.new(f\"char[{maxDataLen}]\")\n"
                functionStr += "\tactualSize = ffi.new(\"int[1]\")\n"
                functionStr += "\terr = helicsErrorInitialize()\n"
                functionStr += "\tfn(ipt.handle, data, maxDataLen, actualSize, err)\n"
                functionStr += "\tif err.error_code != 0:\n"
                functionStr += "\t\traise HelicsException(\"[\" + str(err.error_code) + \"] \" + ffi.string(err.message).decode())\n"
                functionStr += "\treturn ffi.unpack(data, length=actualSize[0])\n\n\n"
            elif functionDict.get("spelling", "") == "helicsInputGetComplex":
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
                functionStr = "def helicsInputGetComplex(ipt: HelicsInput) -> complex:\n"
                functionStr += "\t\"\"\"\n"
                functionStr += "\t\tGet a complex value from an input object.\n\n"
                functionStr += "\t\t@param ipt The input to get the data for.\n\n"
                functionStr += "\t\t@return  A complex number.\n"
                functionStr += "\t\"\"\"\n"
                functionStr += "\tfn = getattr(lib, \"helicsInputGetComplex\")\n"
                functionStr += "\treal = ffi.new(\"double *\")\n"
                functionStr += "\timag = ffi.new(\"double *\")\n"
                functionStr += "\terr = helicsErrorInitialize()\n"
                functionStr += "\tfn(ipt.handle, real, imag, err)\n"
                functionStr += "\tif err.error_code != 0:\n"
                functionStr += "\t\traise HelicsException(\"[\" + str(err.error_code) + \"] \" + ffi.string(err.message).decode())\n"
                functionStr += "\treturn complex(real[0], imag[0])\n\n\n"
            elif functionDict.get("spelling", "") == "helicsInputGetComplexObject":
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
                functionStr = "def helicsInputGetComplexObject(ipt: HelicsInput) -> complex:\n"
                functionStr += "\t\"\"\"\n"
                functionStr += "\t\tGet a complex value from an input object.\n\n"
                functionStr += "\t\t@param ipt The input to get the data for.\n\n"
                functionStr += "\t\t@return  A complex number.\n"
                functionStr += "\t\"\"\"\n"
                functionStr += "\tfn = getattr(lib, \"helicsInputGetComplexObject\")\n"
                functionStr += "\terr = helicsErrorInitialize()\n"
                functionStr += "\tresult = fn(ipt.handle, err)\n"
                functionStr += "\tif err.error_code != 0:\n"
                functionStr += "\t\traise HelicsException(\"[\" + str(err.error_code) + \"] \" + ffi.string(err.message).decode())\n"
                functionStr += "\treturn complex(result.real, result.imag)\n\n\n"
            elif functionDict.get("spelling", "") == "helicsInputGetNamedPoint":
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
                functionStr = "def helicsInputGetNamedPoint(ipt: HelicsInput) -> Tuple[str, float]:\n"
                functionStr += "\t\"\"\"\n"
                functionStr += "\t\tGet a named point from a subscription.\n\n"
                functionStr += "\t\t@param ipt The input to get the result for.\n\n"
                functionStr += "\t\t@return a tuple of a string and a double value for the named point\n"
                functionStr += "\t\"\"\"\n"
                functionStr += "\tfn = getattr(lib, \"helicsInputGetNamedPoint\")\n"
                functionStr += "\tmaxStringLen = helicsInputGetStringSize(ipt) + 1024\n"
                functionStr += "\toutputString = ffi.new(f\"char[{maxStringLen}]\")\n"
                functionStr += "\tactualLength = ffi.new(\"int[1]\")\n"
                functionStr += "\tval = ffi.new(\"double[1]\")\n"
                functionStr += "\terr = helicsErrorInitialize()\n"
                functionStr += "\tfn(ipt.handle, outputString, maxStringLen, actualLength, val, err)\n"
                functionStr += "\tif err.error_code != 0:\n"
                functionStr += "\t\traise HelicsException(\"[\" + str(err.error_code) + \"] \" + ffi.string(err.message).decode())\n"
                functionStr += "\treturn (ffi.string(outputString, maxlen=actualLength[0]).decode(), val[0])\n\n\n"
            elif functionDict.get("spelling", "") == "helicsInputGetString":
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
                functionStr = "def helicsInputGetString(ipt: HelicsInput) -> str:\n"
                functionStr += "\t\"\"\"\n"
                functionStr += "\t\tGet a string value from a subscription.\n\n"
                functionStr += "\t\t@param ipt The input to get the string for.\n\n"
                functionStr += "\t\t@return  the string value.\n"
                functionStr += "\t\"\"\"\n"
                functionStr += "\tfn = getattr(lib, \"helicsInputGetString\")\n"
                functionStr += "\tmaxStringLength = helicsInputGetStringSize(ipt) + 1024\n"
                functionStr += "\toutputString = ffi.new(f\"char[{maxStringLength}]\")\n"
                functionStr += "\tactualLength = ffi.new(\"int[1]\")\n"
                functionStr += "\terr = helicsErrorInitialize()\n"
                functionStr += "\tfn(ipt.handle, outputString, maxStringLength, actualLength, err)\n"
                functionStr += "\tif err.error_code != 0:\n"
                functionStr += "\t\traise HelicsException(\"[\" + str(err.error_code) + \"] \" + ffi.string(err.message).decode())\n"
                functionStr += "\treturn ffi.string(outputString, maxlen=actualLength[0]).decode()\n\n\n"
            elif functionDict.get("spelling", "") == "helicsInputGetVector":
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
                functionStr = "def helicsInputGetVector(ipt: HelicsInput) -> List[float]:\n"
                functionStr += "\t\"\"\"\n"
                functionStr += "\t\tGet a vector from a subscription.\n\n"
                functionStr += "\t\t@param ipt The input to get the vector for.\n\n"
                functionStr += "\t\t@return  a list of floating point values.\n"
                functionStr += "\t\"\"\"\n"
                functionStr += "\tfn = getattr(lib, \"helicsInputGetVector\")\n"
                functionStr += "\tmaxLength = helicsInputGetVectorSize(ipt) + 1024\n"
                functionStr += "\tdata = ffi.new(f\"double[{maxLength}]\")\n"
                functionStr += "\tactualSize = ffi.new(\"int[1]\")\n"
                functionStr += "\terr = helicsErrorInitialize()\n"
                functionStr += "\tfn(ipt.handle, data, maxLength, actualSize, err)\n"
                functionStr += "\tif err.error_code != 0:\n"
                functionStr += "\t\traise HelicsException(\"[\" + str(err.error_code) + \"] \" + ffi.string(err.message).decode())\n"
                functionStr += "\treturn [x for x in data][0 : actualSize[0]]\n\n\n"
            elif functionDict.get("spelling", "") == "helicsInputSetDefaultBytes":
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
                functionStr = "def helicsInputSetDefaultBytes(ipt: HelicsInput, data: bytes):\n"
                functionStr += "\t\"\"\"\n"
                functionStr += "\t\tSet the default as a raw data array.\n\n"
                functionStr += "\t\t@param ipt The input to set the default for.\n"
                functionStr += "\t\t@param data A pointer to the raw data to use for the default.\n"
                functionStr += "\t\"\"\"\n"
                functionStr += "\tfn = getattr(lib, \"helicsInputSetDefaultBytes\")\n"
                functionStr += "\tif isinstance(data, str):\n"
                functionStr += "\t\tdata = data.encode()\n"
                functionStr += "\tif not isinstance(data, bytes):\n"
                functionStr += "\t\traise Exception(\"\"\"data must be of type `bytes`. Got {t} instead. Try converting it to bytes (e.g. `\"hello world\".encode()`\"\"\".format(t=type(data)))\n"
                functionStr += "\tinputDataLength = len(data)\n"
                functionStr += "\terr = helicsErrorInitialize()\n"
                functionStr += "\tfn(ipt.handle, data, inputDataLength, err)\n"
                functionStr += "\tif err.error_code != 0:\n"
                functionStr += "\t\traise HelicsException(\"[\" + str(err.error_code) + \"] \" + ffi.string(err.message).decode())\n\n\n"
            elif functionDict.get("spelling", "") == "helicsInputSetDefaultComplex":
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
                functionStr = "def helicsInputSetDefaultComplex(ipt: HelicsInput, value: complex):\n"
                functionStr += "\t\"\"\"\n"
                functionStr += "\t\tSet the default as a complex number.\n\n"
                functionStr += "\t\t@param ipt The input to get the data for.\n"
                functionStr += "\t\t@param value The default complex value.\n"
                functionStr += "\t\"\"\"\n"
                functionStr += "\tfn = getattr(lib, \"helicsInputSetDefaultComplex\")\n"
                functionStr += "\terr = helicsErrorInitialize()\n"
                functionStr += "\tfn(ipt.handle, value.real, value.imag, err)\n"
                functionStr += "\tif err.error_code != 0:\n"
                functionStr += "\t\traise HelicsException(\"[\" + str(err.error_code) + \"] \" + ffi.string(err.message).decode())\n\n\n"
            elif functionDict.get("spelling", "") == "helicsInputSetDefaultVector":
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
                functionStr = "def helicsInputSetDefaultVector(ipt: HelicsInput, vectorInput: List[float]):\n"
                functionStr += "\t\"\"\"\n"
                functionStr += "\t\tSet the default as a list of floats.\n\n"
                functionStr += "\t\t@param ipt The input to get the data for.\n"
                functionStr += "\t\t@param vectorInput The default list of floating point values.\n"
                functionStr += "\t\"\"\"\n"
                functionStr += "\tfn = getattr(lib, \"helicsInputSetDefaultVector\")\n"
                functionStr += "\tvectorLength = len(vectorInput)\n"
                functionStr += "\terr = helicsErrorInitialize()\n"
                functionStr += "\tfn(ipt.handle, vectorInput, vectorLength, err)\n"
                functionStr += "\tif err.error_code != 0:\n"
                functionStr += "\t\traise HelicsException(\"[\" + str(err.error_code) + \"] \" + ffi.string(err.message).decode())\n\n\n"
            elif functionDict.get("spelling", "") == "helicsMessageAppendData":
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
                functionStr = "def helicsMessageAppendData(message: HelicsMessage, data: bytes):\n"
                functionStr += "\t\"\"\"\n"
                functionStr += "\t\tAppend data to the payload.\n\n"
                functionStr += "\t\t@param message The message object in question.\n"
                functionStr += "\t\t@param data A string containing the message data to append.\n"
                functionStr += "\t\"\"\"\n"
                functionStr += "\tfn = getattr(lib, \"helicsMessageAppendData\")\n"
                functionStr += "\tif isinstance(data, str):\n"
                functionStr += "\t\tdata = data.encode()\n"
                functionStr += "\tif not isinstance(data, bytes):\n"
                functionStr += "\t\traise Exception(\"\"\"data must be of type `bytes`. Got {t} instead. Try converting it to bytes (e.g. `\"hello world\".encode()`\"\"\".format(t=type(data)))\n"
                functionStr += "\tinputDataLength = len(data)\n"
                functionStr += "\terr = helicsErrorInitialize()\n"
                functionStr += "\tfn(message.handle, data, inputDataLength, err)\n"
                functionStr += "\tif err.error_code != 0:\n"
                functionStr += "\t\traise HelicsException(\"[\" + str(err.error_code) + \"] \" + ffi.string(err.message).decode())\n\n\n"
            elif functionDict.get("spelling", "") == "helicsMessageGetBytes":
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
                functionStr = "def helicsMessageGetBytes(message: HelicsMessage) -> bytes:\n"
                functionStr += "\t\"\"\"\n"
                functionStr += "\t\tGet the raw data for a message object.\n\n"
                functionStr += "\t\t@param message A message object to get the data for.\n\n"
                functionStr += "\t\t@return Raw string data.\n"
                functionStr += "\t\"\"\"\n"
                functionStr += "\tfn = getattr(lib, \"helicsMessageGetBytes\")\n"
                functionStr += "\tmaxMessageLength = helicsMessageGetByteCount(message) + 1024\n"
                functionStr += "\tdata = ffi.new(f\"char[{maxMessageLength}]\")\n"
                functionStr += "\tactualSize = ffi.new(\"int[1]\")\n"
                functionStr += "\terr = helicsErrorInitialize()\n"
                functionStr += "\tfn(message.handle, data, maxMessageLength, actualSize, err)\n"
                functionStr += "\tif err.error_code != 0:\n"
                functionStr += "\t\traise HelicsException(\"[\" + str(err.error_code) + \"] \" + ffi.string(err.message).decode())\n"
                functionStr += "\treturn ffi.unpack(data, length=actualSize[0])\n\n\n"
            elif functionDict.get("spelling", "") == "helicsMessageSetData":
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
                functionStr = "def helicsMessageSetData(message: HelicsMessage, data: bytes):\n"
                functionStr += "\t\"\"\"\n"
                functionStr += "\t\tSet the data payload of a message as raw data.\n\n"
                functionStr += "\t\t@param message The message object in question.\n"
                functionStr += "\t\t@param data A string containing the message data.\n"
                functionStr += "\t\"\"\"\n"
                functionStr += "\tfn = getattr(lib, \"helicsMessageSetData\")\n"
                functionStr += "\tif isinstance(data, str):\n"
                functionStr += "\t\tdata = data.encode()\n"
                functionStr += "\tif not isinstance(data, bytes):\n"
                functionStr += "\t\traise Exception(\"\"\"data must be of type `bytes`. Got {t} instead. Try converting it to bytes (e.g. `\"hello world\".encode()`\"\"\".format(t=type(data)))\n"
                functionStr += "\tinputDataLength = len(data)\n"
                functionStr += "\terr = helicsErrorInitialize()\n"
                functionStr += "\tfn(message.handle, data, inputDataLength, err)\n"
                functionStr += "\tif err.error_code != 0:\n"
                functionStr += "\t\traise HelicsException(\"[\" + str(err.error_code) + \"] \" + ffi.string(err.message).decode())\n\n\n"
            elif functionDict.get("spelling", "") == "helicsPublicationPublishBytes":
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
                functionStr = "def helicsPublicationPublishBytes(pub: HelicsPublication, data: bytes):\n"
                functionStr += "\t\"\"\"\n"
                functionStr += "\t\tPublish raw data.\n\n"
                functionStr += "\t\t@param pub The publication to publish for.\n"
                functionStr += "\t\t@param data the raw byte data to publish.\n"
                functionStr += "\t\"\"\"\n"
                functionStr += "\tfn = getattr(lib, \"helicsPublicationPublishBytes\")\n"
                functionStr += "\tif isinstance(data, str):\n"
                functionStr += "\t\tdata = data.encode()\n"
                functionStr += "\tif not isinstance(data, bytes):\n"
                functionStr += "\t\traise Exception(\"\"\"data must be of type `bytes`. Got {t} instead. Try converting it to bytes (e.g. `\"hello world\".encode()`\"\"\".format(t=type(data)))\n"
                functionStr += "\tinputDataLength = len(data)\n"
                functionStr += "\terr = helicsErrorInitialize()\n"
                functionStr += "\tfn(pub.handle, data, inputDataLength, err)\n"
                functionStr += "\tif err.error_code != 0:\n"
                functionStr += "\t\traise HelicsException(\"[\" + str(err.error_code) + \"] \" + ffi.string(err.message).decode())\n\n\n"
            elif functionDict.get("spelling", "") == "helicsPublicationPublishComplex":
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
                functionStr = "def helicsPublicationPublishComplex(pub: HelicsPublication, value: complex):\n"
                functionStr += "\t\"\"\"\n"
                functionStr += "\t\tPublish a complex number.\n\n"
                functionStr += "\t\t@param pub The publication to publish for.\n"
                functionStr += "\t\t@param value The complex number.\n"
                functionStr += "\t\"\"\"\n"
                functionStr += "\tfn = getattr(lib, \"helicsPublicationPublishComplex\")\n"
                functionStr += "\terr = helicsErrorInitialize()\n"
                functionStr += "\tfn(pub.handle, value.real, value.imag, err)\n"
                functionStr += "\tif err.error_code != 0:\n"
                functionStr += "\t\traise HelicsException(\"[\" + str(err.error_code) + \"] \" + ffi.string(err.message).decode())\n\n\n"
            elif functionDict.get("spelling", "") == "helicsPublicationPublishVector":
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
                functionStr = "def helicsPublicationPublishVector(pub: HelicsPublication, vectorInput: List[float]):\n"
                functionStr += "\t\"\"\"\n"
                functionStr += "\t\tPublish a vector of doubles.\n\n"
                functionStr += "\t\t@param pub The publication to publish for.\n"
                functionStr += "\t\t@param vectorInput The list of floating point values.\n"
                functionStr += "\t\"\"\"\n"
                functionStr += "\tfn = getattr(lib, \"helicsPublicationPublishVector\")\n"
                functionStr += "\tvectorLength = len(vectorInput)\n"
                functionStr += "\terr = helicsErrorInitialize()\n"
                functionStr += "\tfn(pub.handle, vectorInput, vectorLength, err)\n"
                functionStr += "\tif err.error_code != 0:\n"
                functionStr += "\t\traise HelicsException(\"[\" + str(err.error_code) + \"] \" + ffi.string(err.message).decode())\n\n\n"
            elif functionDict.get("spelling", "") == "helicsQueryBufferFill":
                #check to see if function signiture changed
                argNum = len(functionDict.get("arguments", {}).keys())
                if argNum != 4:
                    raise RuntimeError("the function signature for helicsQueryBufferFill has changed!")
                arg0 = functionDict.get("arguments", {}).get(0, {})
                if arg0.get("spelling","") != "buffer" or arg0.get("type", "") != "HelicsQueryBuffer":
                    raise RuntimeError("the function signature for helicsQueryBufferFill has changed!")
                arg1 = functionDict.get("arguments", {}).get(1, {})
                if arg1.get("spelling","") != "str" or arg1.get("pointer_type", "") != "Char_S_*":
                    raise RuntimeError("the function signature for helicsQueryBufferFill has changed!")
                arg2 = functionDict.get("arguments", {}).get(2, {})
                if arg2.get("spelling","") != "strSize" or arg2.get("type", "") != "Int":
                    raise RuntimeError("the function signature for helicsQueryBufferFill has changed!")
                arg3 = functionDict.get("arguments", {}).get(3, {})
                if arg3.get("spelling","") != "err" or arg3.get("pointer_type", "") != "HelicsError_*":
                    raise RuntimeError("the function signature for helicsQueryBufferFill has changed!")
                functionStr = "def helicsQueryBufferFill(buffer: HelicsQueryBuffer, string: str):\n"
                functionStr += "\t\"\"\"\n"
                functionStr += "\t\tSet the data for a query callback.\n\n"
                functionStr += "\t\t@details There are many queries that HELICS understands directly, but it is occasionally useful to have a federate be able to respond to specific queries with answers specific to a federate.\n\n"
                functionStr += "\t\t@param buffer The buffer received in a helicsQueryCallback.\n"
                functionStr += "\t\t@param string The string with the data to fill the buffer with.\n"
                functionStr += "\t\"\"\"\n"
                functionStr += "\tfn = getattr(lib, \"helicsQueryBufferFill\")\n"
                functionStr += "\tstrLen = len(string)\n"
                functionStr += "\terr = helicsErrorInitialize()\n"
                functionStr += "\tfn(buffer.handle, ffi.new(\"char[]\",string.encode()), strLen, err)\n"
                functionStr += "\tif err.error_code != 0:\n"
                functionStr += "\t\traise HelicsException(\"[\" + str(err.error_code) + \"] \" + ffi.string(err.message).decode())\n\n\n"
            elif functionDict.get("spelling", "") == "helicsGetPropertyIndex":
                #check to see if function signiture changed
                argNum = len(functionDict.get("arguments", {}).keys())
                if argNum != 1:
                    raise RuntimeError("the function signature for helicsGetPropertyIndex has changed!")
                arg0 = functionDict.get("arguments", {}).get(0, {})
                if arg0.get("spelling","") != "val" or arg0.get("pointer_type", "") != "Char_S_*":
                    raise RuntimeError("the function signature for helicsGetPropertyIndex has changed!")
                functionStr = "def helicsGetPropertyIndex(val: str) -> HelicsProperties:\n"
                functionStr += "\t\"\"\"\n"
                functionStr += "\t\tGet a property index for use in /ref helicsFederateInfoSetFlagOption, /ref helicsFederateInfoSetTimeProperty,\n"
                functionStr += "\t\tor /ref helicsFederateInfoSetIntegerProperty\n"
                functionStr += "\t\t@param val A string with the property name.\n"
                functionStr += "\t\t@return An HelicsProperties with the property code or (-1) if not a valid property.\n"
                functionStr += "\t\"\"\"\n"
                functionStr += "\tfn = getattr(lib, \"helicsGetPropertyIndex\")\n"
                functionStr += "\tresult = fn(ffi.new(\"char[]\",val.encode()))\n"
                functionStr += "\tif result == -1 or result == -101:\n"
                functionStr += "\t\traise HelicsException(f\"[-1] Unknown index for HelicsProperties {val}\")\n"
                functionStr += "\telse:\n"
                functionStr += "\t\treturn HelicsProperties(result)\n\n\n"
            elif functionDict.get("spelling", "") == "helicsGetFlagIndex":
                #check to see if function signiture changed
                argNum = len(functionDict.get("arguments", {}).keys())
                if argNum != 1:
                    raise RuntimeError("the function signature for helicsGetFlagIndex has changed!")
                arg0 = functionDict.get("arguments", {}).get(0, {})
                if arg0.get("spelling","") != "val" or arg0.get("pointer_type", "") != "Char_S_*":
                    raise RuntimeError("the function signature for helicsGetFlagIndex has changed!")
                functionStr = "def helicsGetFlagIndex(val: str) -> HelicsFederateFlags:\n"
                functionStr += "\t\"\"\"\n"
                functionStr += "\t\tGet a property index for use in /ref helicsFederateInfoSetFlagOption, /ref helicsFederateSetFlagOption,\n"
                functionStr += "\t\t@param val A string with the option name.\n"
                functionStr += "\t\t@return An HelicsFederateFlags with the property code or (-1) if not a valid property.\n"
                functionStr += "\t\"\"\"\n"
                functionStr += "\tfn = getattr(lib, \"helicsGetFlagIndex\")\n"
                functionStr += "\tresult = fn(ffi.new(\"char[]\",val.encode()))\n"
                functionStr += "\tif result == -1 or result == -101:\n"
                functionStr += "\t\traise HelicsException(f\"[-1] Unknown index for HelicsFederateFlags {val}\")\n"
                functionStr += "\telse:\n"
                functionStr += "\t\treturn HelicsFederateFlags(result)\n\n\n"
            elif functionDict.get("spelling", "") == "helicsGetOptionIndex":
                #check to see if function signiture changed
                argNum = len(functionDict.get("arguments", {}).keys())
                if argNum != 1:
                    raise RuntimeError("the function signature for helicsGetOptionIndex has changed!")
                arg0 = functionDict.get("arguments", {}).get(0, {})
                if arg0.get("spelling","") != "val" or arg0.get("pointer_type", "") != "Char_S_*":
                    raise RuntimeError("the function signature for helicsGetOptionIndex has changed!")
                functionStr = "def helicsGetOptionIndex(val: str) -> HelicsHandleOptions:\n"
                functionStr += "\t\"\"\"\n"
                functionStr += "\t\tGet an option index for use in /ref helicsPublicationSetOption, /ref helicsInputSetOption, /ref helicsEndpointSetOption,\n"
                functionStr += "\t\t/ref helicsFilterSetOption, and the corresponding get functions.\n"
                functionStr += "\t\t@param val A string with the option name.\n"
                functionStr += "\t\t@return An HelicsHandleOptions with the property code or (-1) if not a valid property.\n"
                functionStr += "\t\"\"\"\n"
                functionStr += "\tfn = getattr(lib, \"helicsGetOptionIndex\")\n"
                functionStr += "\tresult = fn(ffi.new(\"char[]\",val.encode()))\n"
                functionStr += "\tif result == -1 or result == -101:\n"
                functionStr += "\t\traise HelicsException(f\"[-1] Unknown index for HelicsHandleOptions {val}\")\n"
                functionStr += "\telse:\n"
                functionStr += "\t\treturn HelicsHandleOptions(result)\n\n\n"
            elif functionDict.get("spelling", "") == "helicsGetOptionValue":
                #check to see if function signiture changed
                argNum = len(functionDict.get("arguments", {}).keys())
                if argNum != 1:
                    raise RuntimeError("the function signature for helicsGetOptionValue has changed!")
                arg0 = functionDict.get("arguments", {}).get(0, {})
                if arg0.get("spelling","") != "val" or arg0.get("pointer_type", "") != "Char_S_*":
                    raise RuntimeError("the function signature for helicsGetOptionValue has changed!")
                functionStr = "def helicsGetOptionValue(val: str) -> int:\n"
                functionStr += "\t\"\"\"\n"
                functionStr += "\t\tGet an option index for use in /ref helicsPublicationSetOption, /ref helicsInputSetOption, /ref helicsEndpointSetOption,\n"
                functionStr += "\t\tor /ref helicsFederateInfoSetIntegerProperty\n"
                functionStr += "\t\t@param val A string with the option name.\n"
                functionStr += "\t\t@return An int with the property code or (-1) if not a valid property.\n"
                functionStr += "\t\"\"\"\n"
                functionStr += "\tfn = getattr(lib, \"helicsGetOptionValue\")\n"
                functionStr += "\tresult = fn(ffi.new(\"char[]\",val.encode()))\n"
                functionStr += "\tif result == -1 or result == -101:\n"
                functionStr += "\t\traise HelicsException(f\"[-1] Unknown option value for flag {val}\")\n"
                functionStr += "\telse:\n"
                functionStr += "\t\treturn result\n\n\n"
            else:
                raise RuntimeError(f"an unknown function is being asked to be modified! function name is {functionDict.get('spelling','')}")
            return functionStr
         
        
        helicsBindingStr = ""
        helicsBindingStr += createBoilerPlate()
        for cu in self.parsedInfo.keys():
            if self.parsedInfo[cu]["kind"] == cidx.CursorKind.ENUM_DECL.name:
                helicsBindingStr += createEnum(self.parsedInfo[cu])
        for cu in self.parsedInfo.keys():
            if self.parsedInfo[cu]["kind"] == cidx.CursorKind.TYPEDEF_DECL.name and 'struct' not in self.parsedInfo[cu]["type"]:
                helicsBindingStr += createTypeDef(self.parsedInfo[cu])
        for cu in self.parsedInfo.keys():
            if self.parsedInfo[cu]["kind"] == cidx.CursorKind.STRUCT_DECL.name:
                helicsBindingStr += createStruct(self.parsedInfo[cu])
        for cu in self.parsedInfo.keys():
            if self.parsedInfo[cu]["kind"] == cidx.CursorKind.MACRO_DEFINITION.name:
                helicsBindingStr += createMacro(self.parsedInfo[cu])
        for cu in self.parsedInfo.keys():
            if self.parsedInfo[cu]["kind"] == cidx.CursorKind.VAR_DECL.name:
                helicsBindingStr += createVarDeclaration(self.parsedInfo[cu])
        for cu in self.parsedInfo.keys():
            if self.parsedInfo[cu]["kind"] == cidx.CursorKind.FUNCTION_DECL.name:
                helicsBindingStr += createFunctionDeclaration(self.parsedInfo[cu])
        return helicsBindingStr 
    
            
    def createMatlabBindings(self) -> None:
        """
            Function that creates the HELICS MATLAB Bindings
        """
        def createBoilerPlate(headerFiles: List[str], helicsElementMapTuples) -> str:
            boilerPlateStr = ""
            for h in headerFiles:
                boilerPlateStr = f"#include \"helics/helics.h\"\n"
            boilerPlateStr += "#include <mex.h>\n"
            boilerPlateStr += "#include <stdexcept>\n"
            boilerPlateStr += "#include <string>\n"
            boilerPlateStr += "#include <unordered_map>\n\n"
            boilerPlateStr += "static int mexFunctionCalled = 0;\n\n"
            boilerPlateStr += "static void throwHelicsMatlabError(HelicsError *err) {\n"
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
        
        
        def createEnum(enumDict: dict()) -> None:
            """
                Create Matlab Binding Enumerations for each C Enumeration
            """
            clangLogger.debug(f"creating MATLAB enum definition for:\n{json.dumps(enumDict,indent=4,sort_keys=True)}")
            enumSpelling = enumDict.get('spelling','')
            enumComment = enumDict.get('brief_comment','')
            with open(f"+helics/{enumSpelling}.m","w") as enumMFile:
                enumMFile.write(f"classdef {enumSpelling} < uint64\n")
                enumMFile.write("\t%{\n"+f"\t{enumComment}\n\n\tAttributes:")
                docStrBody = ""
                enumStrBody = ""
                for enumKey in enumDict.get('enumerations',{}).keys():
                    keywordSpelling = enumDict.get('enumerations',{}).get(enumKey,{}).get('spelling','')
                    keywordValue = enumDict.get('enumerations',{}).get(enumKey,{}).get('value')
                    keywordComment = enumDict.get('enumerations',{}).get(enumKey,{}).get('brief_comment','')
                    if keywordComment == None:
                        keywordComment = ''
                    docStrBody += f"\n\t\t{keywordSpelling}: value:{keywordValue}\t{keywordComment}"
                    enumStrBody += f"\n\t\t{keywordSpelling} ({keywordValue})"
                enumMFile.write(docStrBody)
                enumMFile.write("\n\t%}\n\tenumeration")
                enumMFile.write(enumStrBody)
                enumMFile.write("\n\tend\nend")
                
        
        
        def createMacro(macroDict: dict(), cursorIdx: int):
            clangLogger.debug(f"creating MATLAB macro definition for:\n{json.dumps(macroDict,indent=4,sort_keys=True)}")
            macroSpelling = macroDict.get("spelling","")
            macroComment = macroDict.get("brief_comment","")
            macroValue = macroDict.get("value")
            macroWrapperStr = ""
            macroMainFunctionElementStr = ""
            macroMapTuple = None
            if isinstance(macroValue, str):
                with open(f"+helics/{macroSpelling}.m", "w") as macroFile:
                    if macroComment != None:
                        macroFile.write("%{\n")
                        macroFile.write(f"{macroComment}\n")
                        macroFile.write("%}\n")
                    macroFile.write(f"function v = {macroSpelling}()\n")
                    macroFile.write("\tpersistent vInitialized;\n")
                    macroFile.write("\tif isempty(vInitialized)\n")
                    macroFile.write(f"\t\tvInitialized = helicsMex(\"{macroSpelling}\");\n")
                    macroFile.write("\tend\n")
                    macroFile.write("\tv = vInitialized;\n")
                    macroFile.write("end\n")
                macroWrapperStr += f"void _wrap_{macroSpelling}(int resc, mxArray *resv[], int argc, mxArray *argv[])" + "{\n"
                macroWrapperStr += "\tmxArray *out = mxCreateNumericMatrix(1,1,mxINT64_CLASS,mxREAL);\n"
                macroWrapperStr += f"\t*((int64_T*)mxGetData(out)) = (int64_t){macroSpelling};\n"
                macroWrapperStr += "\tresv[0] = out;\n"
                macroWrapperStr += "}\n\n"
                macroMainFunctionElementStr += f"\tcase {cursorIdx}:\n"
                macroMainFunctionElementStr += f"\t\t_wrap_{macroSpelling}(resc, resv, argc, (mxArray**)(argv));\n"
                macroMainFunctionElementStr += f"\t\tbreak;\n"
                macroMapTuple = (macroSpelling, cursorIdx)
            elif isinstance(macroValue, float) or isinstance(macroValue, int):
                with open(f"+helics/{macroSpelling}.m", "w") as macroFile:
                    if macroComment != None:
                        macroFile.write("%{\n")
                        macroFile.write(f"{macroComment}\n")
                        macroFile.write("%}\n")
                    macroFile.write(f"function v = {macroSpelling}()\n")
                    macroFile.write("\tpersistent vInitialized;\n")
                    macroFile.write("\tif isempty(vInitialized)\n")
                    macroFile.write(f"\t\tvInitialized = {macroValue};\n")
                    macroFile.write("\tend\n")
                    macroFile.write("\tv = vInitialized;\n")
                    macroFile.write("end\n")
            return macroWrapperStr, macroMainFunctionElementStr, macroMapTuple
        
        
        def createVar(varDict: dict(), cursorIdx: int):
            clangLogger.debug(f"creating MATLAB var definition for:\n{json.dumps(varDict,indent=4,sort_keys=True)}")
            varSpelling = varDict.get("spelling","")
            varComment = varDict.get("brief_comment","")
            varValue = varDict.get("value")
            if isinstance(varValue, str):
                with open(f"+helics/{varSpelling}.m", "w") as varFile:
                    if varComment != None:
                        varFile.write("%{\n")
                        varFile.write(f"{varComment}\n")
                        varFile.write("%}\n")
                    varFile.write(f"function v = {varSpelling}()\n")
                    varFile.write("\tpersistent vInitialized;\n")
                    varFile.write("\tif isempty(vInitialized)\n")
                    varFile.write(f"\t\tvInitialized = {varValue}();\n")
                    varFile.write("\tend\n")
                    varFile.write("\tv = vInitialized;\n")
                    varFile.write("end\n")
            elif isinstance(varValue, float) or isinstance(varValue, int):
                with open(f"+helics/{varSpelling}.m", "w") as varFile:
                    if varComment != None:
                        varFile.write("%{\n")
                        varFile.write(f"{varComment}\n")
                        varFile.write("%}\n")
                    varFile.write(f"function v = {varSpelling}()\n")
                    varFile.write("\tpersistent vInitialized;\n")
                    varFile.write("\tif isempty(vInitialized)\n")
                    varFile.write(f"\t\tvInitialized = {varValue};\n")
                    varFile.write("\tend\n")
                    varFile.write("\tv = vInitialized;\n")
                    varFile.write("end\n")
        
        
        def createFunction(functionDict: dict(), cursorIdx: int):
            clangLogger.debug(f"creating MATLAB function definition for:\n{json.dumps(functionDict,indent=4,sort_keys=True)}")
            modifiedMatlabFunctionList = [
                "helicsCreateBrokerFromArgs",
                "helicsCreateCoreFromArgs",
                "helicsDoubleToBytes",
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
                "helicsIntToBytes",
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
                "helicsStringToBytes",
                "helicsBoolToBytes",
                "helicsCharToBytes",
                "helicsTimeToBytes",
                "helicsComplexToBytes",
                "helicsVectorToBytes"
            ]
            functionsToIgnore = ["helicsErrorInitialize", "helicsErrorClear"]
            functionName = functionDict.get("spelling")
            if functionName in modifiedMatlabFunctionList:
                return createModifiedMatlabFunction(functionDict, cursorIdx)
            if functionName not in functionsToIgnore:
                functionComment = functionDict.get("raw_comment")
                functionComment = functionComment.replace("/**", '%{')
                functionComment = functionComment.replace("\n */", "\n")
                functionComment = functionComment.replace("\n * ", "\n\t")
                functionComment = functionComment.replace(" *", "")
                if "@forcpponly" in functionComment:
                    funComPart = functionComment.partition("\t@forcpponly")
                    funComPart1 = funComPart[2].partition("@endforcpponly\n")
                    if "@forcpponly in funComPart1[2]":
                        funComPart2 = funComPart1[2].partition("\t@forcpponly")
                        functionComment = funComPart[0] + funComPart2[0] + '%}\n'
                    else:
                        functionComment = funComPart[0] + funComPart1[2] + '\n%}\n'
                else:
                    functionComment = functionComment + '%}\n'
                with open(f"+helics/{functionName}.m", "w") as functionMFile:
                    functionMFile.write(functionComment)
                    functionMFile.write(f"function varargout = {functionName}(varargin)\n")
                    functionMFile.write("\t[varargout{1:nargout}] = helicsMex(" + f"\"{functionName}\", varargin" + "{:});\n")
                    functionMFile.write("end\n")
                functionWrapperStr = ""
                functionMainElements = ""
                functionMainElements += f"\tcase {cursorIdx}:\n"
                functionMainElements += f"\t\t_wrap_{functionName}(resc, resv, argc, (mxArray**)(argv));\n"
                functionMainElements += f"\t\tbreak;\n"
                functionWrapperStr += f"void _wrap_{functionName}(int resc, (mxArray**resv, argc, (mxArray**)(argv))" + "{\n"
                for a in functionDict.get("arguments",{}).keys():
                    functionWrapperStr += getArgInitializationStr(functionDict.get("arguments",{}).get(a,{}), int(a))
                if functionDict.get("result_type","") != "Void":
                    functionWrapperStr += f"{getFunctionReturnInitializationStr(functionDict)} = {functionName}("
                    for a in functionDict.get("arguments",{}).keys():
                        functionWrapperStr += getArgFunctionCallStr(functionDict.get("arguments",{}).get(a,{}), int(a))
                    functionWrapperStr += ");\n\n"
                    functionWrapperStr += getFunctionReturnConversionStr(functionDict)
                    functionWrapperStr += "\tif(_out){\n"
                    functionWrapperStr += "\t\t--resc;\n"
                    functionWrapperStr += "\t\t*resv++ = _out;\n"
                    functionWrapperStr += "\t}"
                    for a in functionDict.get("arguments",{}).keys():
                        functionWrapperStr += "\n\n"
                        functionWrapperStr += getArgFunctionCleanUpStr(functionDict.get("arguments",{}).get(a,{}))
                    functionWrapperStr += "\n}\n\n\n"
                else:
                    functionWrapperStr += f"\t{functionName}("
                    for a in functionDict.get("arguments",{}).keys():
                        functionWrapperStr += getArgFunctionCallStr(functionDict.get("arguments",{}).get(a,{}), int(a))
                    functionWrapperStr += ");\n\n"
                    functionWrapperStr += "\tmxArray _out = (mxArray *)0;\n\n"
                    functionWrapperStr += "\tif(_out){\n"
                    functionWrapperStr += "\t\t--resc;\n"
                    functionWrapperStr += "\t\t*resv++ = _out;\n"
                    functionWrapperStr += "\t}"
                    for a in functionDict.get("arguments",{}).keys():
                        functionWrapperStr += "\n\n"
                        functionWrapperStr += getArgFunctionCleanUpStr(functionDict.get("arguments",{}).get(a,{}))
                    functionWrapperStr += "\n}\n\n\n"                        
                return functionWrapperStr, functionMainElements, (functionName, cursorIdx)
            else:
                return "","", None

        
        def getArgInitializationStr(argDict: dict(), argPos: int) -> str:
            argInitTextMap = {
                "Char_S": initializeArgChar(argDict.get("spelling",""), argPos),
                "Char_S_*": initializeArgChar(argDict.get("spelling",""), argPos),
                "Double": initializeArgDouble(argDict.get("spelling",""), argPos),
                "Int": initializeArgInt(argDict.get("spelling",""), argPos),
                "Void_*": initializeArgVoidPtr(argDict.get("spelling",""), argPos),
                "HelicsBool": initializeArgHelicsBool(argDict.get("spelling",""), argPos),
                "HelicsBroker": initializeArgHelicsClass("HelicsBroker", argDict.get("spelling",""), argPos),
                "HelicsCore": initializeArgHelicsClass("HelicsCore", argDict.get("spelling",""), argPos),
                "HelicsDataBuffer": initializeArgHelicsClass("HelicsDataBuffer", argDict.get("spelling",""), argPos),
                "HelicsDataTypes": initializeArgHelicsEnum("HelicsDataTypes", argDict.get("spelling",""), argPos),
                "HelicsEndpoint": initializeArgHelicsClass("HelicsEndpoint", argDict.get("spelling",""), argPos),
                "HelicsError_*": initializeArgHelicsErrorPtr(argDict.get("spelling","")),
                "HelicsFederate": initializeArgHelicsClass("HelicsFederate", argDict.get("spelling",""), argPos),
                "HelicsFederateInfo": initializeArgHelicsClass("HelicsFederateInfo", argDict.get("spelling",""), argPos),
                "HelicsFilter": initializeArgHelicsClass("HelicsFilter", argDict.get("spelling",""), argPos),
                "HelicsFilterTypes": initializeArgHelicsEnum("HelicsFilterTypes", argDict.get("spelling",""), argPos),
                "HelicsInput": initializeArgHelicsClass("HelicsInput", argDict.get("spelling",""), argPos),
                "HelicsIterationRequest": initializeArgHelicsEnum("HelicsIterationRequest", argDict.get("spelling",""), argPos),
                "HelicsIterationResult_*": initializeArgHelicsIterationResultPtr(argDict.get("spelling","")), 
                "HelicsMessage": initializeArgHelicsClass("HelicsMessage", argDict.get("spelling",""), argPos),
                "HelicsPublication": initializeArgHelicsClass("HelicsPublication", argDict.get("spelling",""), argPos),
                "HelicsQuery": initializeArgHelicsClass("HelicsQuery", argDict.get("spelling",""), argPos),
                "HelicsQueryBuffer": initializeArgHelicsClass("HelicsQueryBuffer", argDict.get("spelling",""), argPos),
                "HelicsTime": initializeArgHelicsTime(argDict.get("spelling",""), argPos),
                "HelicsTranslator": initializeArgHelicsClass("HelicsTranslator", argDict.get("spelling",""), argPos),
                "HelicsTranslatorTypes": initializeArgHelicsEnum("HelicsTranslatorTypes", argDict.get("spelling",""), argPos),
                "int32_t": initializeArgInt32_t(argDict.get("spelling",""), argPos),
                "int64_t": initializeArgInt64_t(argDict.get("spelling",""), argPos)
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
        
        
        def getFunctionReturnInitializationStr(functionDict: dict()) -> str:
            returnTextMap = {
                "Char_S": "\tchar result",
                "Char_S_*": "\tchar *result",
                "Double": "\tdouble result",
                "Int": "\tint result",
                "Void_*": "\tvoid *reault",
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
        
        
        def getArgFunctionCallStr(argDict: dict(), position: int) -> str:
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
                "HelicsIterationResult_*": argDefaultFunctionCall(argDict.get("spelling",""), position), 
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
        
        
        def getFunctionReturnConversionStr(functionDict: dict()) -> str:
            returnTextMap = {
                "Char_S": returnCharTomxArray(),
                "Char_S_*": returnCharPtrTomxArray(),
                "Double": returnDoubleTomxArray(),
                "Int": returnIntTomxArray(),
                "Void_*": returnVoidPtrTomxArray(),
                "HelicsBool": returnIntTomxArray(),
                "HelicsBroker": returnVoidPtrTomxArray(),
                "HelicsCore": returnVoidPtrTomxArray(),
                "HelicsDataBuffer": returnVoidPtrTomxArray(),
                "HelicsEndpoint": returnVoidPtrTomxArray(),
                "HelicsFederate": returnVoidPtrTomxArray(),
                "HelicsFederateInfo": returnVoidPtrTomxArray(),
                "HelicsFederateState": returnIntTomxArray(),
                "HelicsFilter": returnVoidPtrTomxArray(),
                "HelicsInput": returnVoidPtrTomxArray(),
                "HelicsIterationResult": returnIntTomxArray(),
                "HelicsMessage": returnVoidPtrTomxArray(),
                "HelicsPublication": returnVoidPtrTomxArray(),
                "HelicsQuery": returnVoidPtrTomxArray(),
                "HelicsTime": returnDoubleTomxArray(),
                "HelicsTranslator": returnVoidPtrTomxArray(),
                "int32_t": returnIntTomxArray(),
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
        
        
        def getArgFunctionCleanUpStr(argDict: dict()) -> str:
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
        
        
        def initializeArgChar(argName: str, position: int) -> str:
            retStr = f"\tchar *{argName};\n"
            retStr += f"\tsize_t {argName}Length;\n"
            retStr += f"\tint {argName}Status;\n"
            retStr += f"\t{argName}Length = mxGetN(argv[{position}]) + 1;\n"
            retStr += f"\t{argName} = (char *)mxMalloc({argName}Length);\n"
            retStr += f"\t{argName}Status = mxGetString(argv[{position}], {argName}, {argName}Length);\n\n"
            return retStr
        
        
        def initializeArgDouble(argName: str, position: int) -> str:
            retStr = f"\tdouble {argName} = mxGetScalar(argv[{position}]);\n\n"
            return retStr
        
        
        def initializeArgInt(argName: str, position: int) -> str:
            retStr = f"\tint {argName} = (int)(mxGetScalar(argv[{position}]));\n\n"
            return retStr
        
        
        def initializeArgVoidPtr(argName: str, position: int) -> str:
            retStr = f"\tvoid *{argName} = mxGetData(argv[{position}]);\n\n"
            return retStr
        
        
        def initializeArgHelicsBool(argName: str, position: int) -> str:
            retStr = f"\tHelicsBool {argName} = (HelicsBool)(mxGetScalar(argv[{position}]));\n\n"
            return retStr
        
        
        def initializeArgHelicsClass(helicsClass: str, argName: str, position: int) -> str:
            retStr = f"\t{helicsClass} {argName} = ({helicsClass})(mxGetData(argv[{position}]));\n\n"
            return retStr
        
        
        def initializeArgHelicsEnum(helicsEnum: str, argName: str, position: int) -> str:
            retStr = f"\t{helicsEnum} {argName} = ({helicsEnum})(mxGetScalar(argv[{position}]));\n\n"
            return retStr
        
        
        def initializeArgHelicsIterationResultPtr(argName: str) -> str:
            retStr = f"\tHelicsIterationResult *{argName} = (HelicsIterationResult *)0;\n\n"
            return retStr
        
        
        def initializeArgHelicsTime(argName: str, position: int) -> str:
            retStr = f"\tHelicsTime {argName} = (HelicsTime)(mxGetScalar(argv[{position}]));\n\n"
            return retStr
        
        
        def initializeArgInt32_t(argName: str, position: int) -> str:
            retStr = f"\tint32_t {argName} = *((int32_t *)mxGetData(argv[{position}]));\n\n"
            return retStr
        
        
        def initializeArgInt64_t(argName: str, position: int) -> str:
            retStr = f"\tint64_t {argName} = *((int64_t *)mxGetData(argv[{position}]));\n\n"
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
                retStr = f"(char const *){argName}"
            else:
                retStr = f", (char const *){argName}"
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
            retStr = "\tmxArray *_out = mxCreateString((const char *)(&result));\n\n"
            return retStr
        
        
        def returnCharPtrTomxArray() -> str:
            retStr = "\tmxArray *_out = mxCreateString((const char *)result);\n\n"
            return retStr
        
        
        def returnDoubleTomxArray() -> str:
            retStr = "\tmxArray *_out = mxCreateDoubleScalar(result);\n\n"
            return retStr
        
        
        def returnIntTomxArray() -> str:
            retStr = "\tmxArray *_out = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);\n"
            retStr += "\t*((int64_T*)mxGetData(out)) = (int64_T)result;\n\n"
            return retStr
        
        
        def returnVoidPtrTomxArray() -> str:
            retStr = "\tmxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);\n"
            retStr += "\t*((uint64_T*)mxGetData(_out)) = (uint64_T)result;\n\n"
            return retStr
        
        
        def argCharPostFunctionCall(argName: str) -> str:
            return f"\tmxFree({argName});"
        
        
        def argHelicsErrorPtrPostFunctionCall(argName: str) -> str:
            retStr = f"\tif({argName}.error_code != HELICS_OK)" + "{\n"
            retStr += f"\t\tthrowHelicsMatlabError(&{argName});\n"
            retStr += "\t}"
            return retStr
        
        
        def HelicsIterationResultPtrPostFunctionCall(argName: str) -> str:
            retStr = f"\tmxArray *{argName}Mx = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);\n"
            retStr += f"\t*((int64_T*)mxGetData({argName}Mx)) = (int64_T){argName};\n"
            retStr += "\tif(--resc >= 0) {\n"
            retStr += f"\t\t*resv++ = *{argName}Mx;\n"
            retStr += "\t}"
            return retStr
        
        
        def createModifiedMatlabFunction(functionDict: dict(), cursorIdx: int):
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
                "helicsIntToBytes": helicsIntToBytesMatlabWrapper,
                "helicsDoubleToBytes": helicsDoubleToBytesMatlabWrapper,
                "helicsStringToBytes": helicsStringToBytesMatlabWrapper,
                "helicsBoolToBytes": helicsBoolToBytesMatlabWrapper,
                "helicsCharToBytes": helicsCharToBytesMatlabWrapper,
                "helicsTimeToBytes": helicsTimeToBytesMatlabWrapper,
                "helicsComplexToBytes": helicsComplexToBytesMatlabWrapper,
                "helicsVectorToBytes": helicsVectorToBytesMatlabWrapper
            }
            functionComment, functionWrapper, functionMainElement = modifiedPythonFunctionList[functionDict.get("spelling","")](functionDict, cursorIdx)
            
            with open(f'+helics/{functionDict.get("spelling","")}.m', "w") as functionMFile:
                functionMFile.write(functionComment)
                functionMFile.write(f'function varargout = {functionDict.get("spelling","")}(varargin)\n')
                functionMFile.write("\t[varargout{1:nargout}] = helicsMex(" + f'\"{functionDict.get("spelling","")}\", varargin' + "{:});\n")
                functionMFile.write("end\n")
            return functionWrapper, functionMainElement, (functionDict.get("spelling",""), cursorIdx)
        
        
        def helicsCreateCoreFromArgsMatlabWrapper(functionDict: dict(), cursorIdx: int):
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
            functionComment = "%{\n"
            functionComment += "\tCreate a core object by passing command line arguments.\n\n"
            functionComment += "\t@param type The type of the core to create.\n"
            functionComment += "\t@param name The name of the core.\n"
            functionComment += "\t@param arguments The list of string values from a command line.\n\n"
            functionComment += "\t@return A HelicsCore object.\n"
            functionComment += "%}\n"
            functionWrapper = f"void _wrap_{functionName}(int resc, mxArray *resv[], int argc, const mxArray *argv[])" + "{\n"
            functionWrapper += initializeArgChar("type", 0)
            functionWrapper += initializeArgChar("name", 1)
            functionWrapper += "\tint arg3 = 0;\n"
            functionWrapper += "\tchar **arg4 = (char **)0;\n"
            functionWrapper += "\tint ii;\n"
            functionWrapper += "\targ3 = static_cast<int>(mxGetNumberOfElements(argv[2]));\n"
            functionWrapper += "\targ4 = (char **)mxMalloc((arg3)*sizeof(char *));\n"
            functionWrapper += "\tfor (ii=0;ii<arg3;ii++){\n"
            functionWrapper += "\t\tmxArray *cellElement=mxGetCell(argv[2], ii);\n"
            functionWrapper += "\t\tint len = mxGetN(cellElement) + 1;\n"
            functionWrapper += "\t\targ4[ii] = (char *)mxMalloc(len);\n"
            functionWrapper += "\t\tint flag = mxGetString(cellElement, arg4[ii], len);\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += initializeArgHelicsErrorPtr("err")
            functionWrapper += f"\tHelicsCore result = {functionName}(type, name, arg3, arg4, &err);\n\n"
            functionWrapper += returnVoidPtrTomxArray()
            functionWrapper += "\tif(_out){\n"
            functionWrapper += "\t\t--resc;\n"
            functionWrapper += "\t\t*resv++ = _out;\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += f'{argCharPostFunctionCall("type")}\n\n'
            functionWrapper += f'{argCharPostFunctionCall("name")}\n\n'
            functionWrapper += f'{argCharPostFunctionCall("arg4")}\n\n'
            functionWrapper += f'{argHelicsErrorPtrPostFunctionCall("err")}\n'
            functionWrapper += "}\n\n\n"
            functionMainElements = f"\tcase {cursorIdx}:\n"
            functionMainElements += f"\t\t_wrap_{functionName}(resc, resv, argc, (mxArray**)(argv));\n"
            functionMainElements += f"\t\tbreak;\n"
            return functionComment, functionWrapper, functionMainElements
            
            
        def helicsCreateBrokerFromArgsMatlabWrapper(functionDict: dict(), cursorIdx: int):
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
            functionComment = "%{\n"
            functionComment += "\tCreate a broker object by passing command line arguments.\n\n"
            functionComment += "\t@param type The type of the core to create.\n"
            functionComment += "\t@param name The name of the core.\n"
            functionComment += "\t@param arguments The list of string values from a command line.\n\n"
            functionComment += "\t@return A HelicsBroker object.\n"
            functionComment += "%}\n"
            functionWrapper = f"void _wrap_{functionName}(int resc, mxArray *resv[], int argc, const mxArray *argv[])" + "{\n"
            functionWrapper += initializeArgChar("type", 0)
            functionWrapper += initializeArgChar("name", 1)
            functionWrapper += "\tint arg3 = 0;\n"
            functionWrapper += "\tchar **arg4 = (char **)0;\n"
            functionWrapper += "\tint ii;\n"
            functionWrapper += "\targ2 = static_cast<int>(mxGetNumberOfElements(argv[2]));\n"
            functionWrapper += "\targ3 = (char **)mxMalloc((arg2)*sizeof(char *));\n"
            functionWrapper += "\tfor (ii=0;ii<arg3;ii++){\n"
            functionWrapper += "\t\tmxArray *cellElement=mxGetCell(argv[2], ii);\n"
            functionWrapper += "\t\tint len = mxGetN(cellElement) + 1;\n"
            functionWrapper += "\t\targ4[ii] = (char *)mxMalloc(len);\n"
            functionWrapper += "\t\tint flag = mxGetString(cellElement, arg4[ii], len);\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += initializeArgHelicsErrorPtr("err")
            functionWrapper += f"\tHelicsBroker result = {functionName}(type, name, arg3, arg4, &err);\n\n"
            functionWrapper += returnVoidPtrTomxArray()
            functionWrapper += "\tif(_out){\n"
            functionWrapper += "\t\t--resc;\n"
            functionWrapper += "\t\t*resv++ = _out;\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += f'{argCharPostFunctionCall("type")}\n\n'
            functionWrapper += f'{argCharPostFunctionCall("name")}\n\n'
            functionWrapper += f'{argCharPostFunctionCall("arg4")}\n\n'
            functionWrapper += f'{argHelicsErrorPtrPostFunctionCall("err")}\n'
            functionWrapper += "}\n\n\n"
            functionMainElements = f"\tcase {cursorIdx}:\n"
            functionMainElements += f"\t\t_wrap_{functionName}(resc, resv, argc, (mxArray**)(argv));\n"
            functionMainElements += f"\t\tbreak;\n"
            return functionComment, functionWrapper, functionMainElements
        
        
        def helicsFederateInfoLoadFromArgsMatlabWrapper(functionDict: dict(), cursorIdx: int):
            #check to see if function signiture changed
            argNum = len(functionDict.get("arguments", {}).keys())
            if argNum != 4:
                raise RuntimeError("the function signature for helicsFederateInfoLoadFromArgs has changed!")
            arg0 = functionDict.get("arguments", {}).get(0, {})
            if arg0.get("spelling","") != "fi" or arg0.get("type", "") != "HelicsFederateInfo":
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
            functionComment = "%{\n"
            functionComment += "\tLoad federate info from command line arguments.\n\n"
            functionComment += "\t@param fi A federateInfo object.\n"
            functionComment += "\t@param arguments A list of strings from the command line.\n"
            functionComment += "%}\n"
            functionWrapper = f"void _wrap_{functionName}(int resc, mxArray *resv[], int argc, const mxArray *argv[])" + "{\n"
            functionWrapper += initializeArgHelicsClass("HelicsFederateInfo", "fi", 0)
            functionWrapper += "\tint arg2 = 0;\n"
            functionWrapper += "\tchar **arg3 = (char **)0;\n"
            functionWrapper += "\tint ii;\n"
            functionWrapper += "\targ2 = static_cast<int>(mxGetNumberOfElements(argv[1]));\n"
            functionWrapper += "\targ3 = (char **)mxMalloc((arg2)*sizeof(char *));\n"
            functionWrapper += "\tfor (ii=0;ii<arg2;ii++){\n"
            functionWrapper += "\t\tmxArray *cellElement=mxGetCell(argv[1], ii);\n"
            functionWrapper += "\t\tint len = mxGetN(cellElement) + 1;\n"
            functionWrapper += "\t\targ3[ii] = (char *)mxMalloc(len);\n"
            functionWrapper += "\t\tint flag = mxGetString(cellElement, arg3[ii], len);\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += initializeArgHelicsErrorPtr("err")
            functionWrapper += f"\t{functionName}(fi, arg2, arg3, &err);\n\n"
            functionWrapper += "\tmxArray *_out = (mxArray *)0;\n"
            functionWrapper += "\tif(_out){\n"
            functionWrapper += "\t\t--resc;\n"
            functionWrapper += "\t\t*resv++ = _out;\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += f'{argCharPostFunctionCall("arg3")}\n\n'
            functionWrapper += f'{argHelicsErrorPtrPostFunctionCall("err")}\n'
            functionWrapper += "}\n\n\n"
            functionMainElements = f"\tcase {cursorIdx}:\n"
            functionMainElements += f"\t\t_wrap_{functionName}(resc, resv, argc, (mxArray**)(argv));\n"
            functionMainElements += f"\t\tbreak;\n"
            return functionComment, functionWrapper, functionMainElements
        
        
        def helicsEndpointSendBytesMatlabWrapper(functionDict: dict(), cursorIdx: int):
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
            functionComment = "%{\n"
            functionComment += "\tSend a message to the targeted destinations.\n\n"
            functionComment += "\t@param endpoint The endpoint to send the data from.\n"
            functionComment += "\t@param data The data to send.\n"
            functionComment += "%}\n"
            functionWrapper = f"void _wrap_{functionName}(int resc, mxArray *resv[], int argc, const mxArray *argv[])" + "{\n"
            functionWrapper += initializeArgHelicsClass("HelicsEndpoint", "endpoint", 0)
            functionWrapper += initializeArgChar("data", 1)
            functionWrapper += "\tint inputDataLength = (int)(dataLength - 1);\n\n"
            functionWrapper += initializeArgHelicsErrorPtr("err")
            functionWrapper += f"\t{functionName}(endpoint, (void *)data, inputDataLength, &err);\n\n"
            functionWrapper += "\tmxArray *_out = (mxArray *)0;\n"
            functionWrapper += "\tif(_out){\n"
            functionWrapper += "\t\t--resc;\n"
            functionWrapper += "\t\t*resv++ = _out;\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += f'{argCharPostFunctionCall("data")}\n\n'
            functionWrapper += f'{argHelicsErrorPtrPostFunctionCall("err")}\n'
            functionWrapper += "}\n\n\n"
            functionMainElements = f"\tcase {cursorIdx}:\n"
            functionMainElements += f"\t\t_wrap_{functionName}(resc, resv, argc, (mxArray**)(argv));\n"
            functionMainElements += f"\t\tbreak;\n"
            return functionComment, functionWrapper, functionMainElements
        
        
        def helicsEndpointSendBytesAtMatlabWrapper(functionDict: dict(), cursorIdx: int):
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
            functionComment = "%{\n"
            functionComment += "\tSend a message to the targeted destinations at a specified time.\n\n"
            functionComment += "\t@param endpoint The endpoint to send the data from.\n"
            functionComment += "\t@param data The data to send.\n"
            functionComment += "\t@param time The time to send the message at.\n"
            functionComment += "%}\n"
            functionWrapper = f"void _wrap_{functionName}(int resc, mxArray *resv[], int argc, const mxArray *argv[])" + "{\n"
            functionWrapper += initializeArgHelicsClass("HelicsEndpoint", "endpoint", 0)
            functionWrapper += initializeArgChar("data", 1)
            functionWrapper += "\tint inputDataLength = (int)(dataLength - 1);\n\n"
            functionWrapper += initializeArgHelicsTime("time", 2)
            functionWrapper += initializeArgHelicsErrorPtr("err")
            functionWrapper += f"\t{functionName}(endpoint, (void *)data, inputDataLength, time, &err);\n\n"
            functionWrapper += "\tmxArray *_out = (mxArray *)0;\n"
            functionWrapper += "\tif(_out){\n"
            functionWrapper += "\t\t--resc;\n"
            functionWrapper += "\t\t*resv++ = _out;\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += f'{argCharPostFunctionCall("data")}\n\n'
            functionWrapper += f'{argHelicsErrorPtrPostFunctionCall("err")}\n'
            functionWrapper += "}\n\n\n"
            functionMainElements = f"\tcase {cursorIdx}:\n"
            functionMainElements += f"\t\t_wrap_{functionName}(resc, resv, argc, (mxArray**)(argv));\n"
            functionMainElements += f"\t\tbreak;\n"
            return functionComment, functionWrapper, functionMainElements
        
        
        def helicsEndpointSendBytesToMatlabWrapper(functionDict: dict(), cursorIdx: int):
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
            functionComment = "%{\n"
            functionComment += "\tSend a message to the specified destination.\n\n"
            functionComment += "\t@param endpoint The endpoint to send the data from.\n"
            functionComment += "\t@param data The data to send.\n"
            functionComment += "\t@param dst The destination to send the message to.\n"
            functionComment += "%}\n"
            functionWrapper = f"void _wrap_{functionName}(int resc, mxArray *resv[], int argc, const mxArray *argv[])" + "{\n"
            functionWrapper += initializeArgHelicsClass("HelicsEndpoint", "endpoint", 0)
            functionWrapper += initializeArgChar("data", 1)
            functionWrapper += "\tint inputDataLength = (int)(dataLength - 1);\n\n"
            functionWrapper += initializeArgChar("dst", 2)
            functionWrapper += initializeArgHelicsErrorPtr("err")
            functionWrapper += f"\t{functionName}(endpoint, (void *)data, inputDataLength, dst, &err);\n\n"
            functionWrapper += "\tmxArray *_out = (mxArray *)0;\n"
            functionWrapper += "\tif(_out){\n"
            functionWrapper += "\t\t--resc;\n"
            functionWrapper += "\t\t*resv++ = _out;\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += f'{argCharPostFunctionCall("data")}\n\n'
            functionWrapper += f'{argCharPostFunctionCall("dst")}\n\n'
            functionWrapper += f'{argHelicsErrorPtrPostFunctionCall("err")}\n'
            functionWrapper += "}\n\n\n"
            functionMainElements = f"\tcase {cursorIdx}:\n"
            functionMainElements += f"\t\t_wrap_{functionName}(resc, resv, argc, (mxArray**)(argv));\n"
            functionMainElements += f"\t\tbreak;\n"
            return functionComment, functionWrapper, functionMainElements
        
        
        def helicsEndpointSendBytesToAtMatlabWrapper(functionDict: dict(), cursorIdx: int):
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
            functionComment = "%{\n"
            functionComment += "\tSend a message to the specified destination at a specified time.\n\n"
            functionComment += "\t@param endpoint The endpoint to send the data from.\n"
            functionComment += "\t@param data The data to send.\n"
            functionComment += "\t@param dst The destination to send the message to.\n"
            functionComment += "\t@param time The time to send the message at.\n"
            functionComment += "%}\n"
            functionWrapper = f"void _wrap_{functionName}(int resc, mxArray *resv[], int argc, const mxArray *argv[])" + "{\n"
            functionWrapper += initializeArgHelicsClass("HelicsEndpoint", "endpoint", 0)
            functionWrapper += initializeArgChar("data", 1)
            functionWrapper += "\tint inputDataLength = (int)(dataLength - 1);\n\n"
            functionWrapper += initializeArgChar("dst", 2)
            functionWrapper += initializeArgHelicsTime("time", 3)
            functionWrapper += initializeArgHelicsErrorPtr("err")
            functionWrapper += f"\t{functionName}(endpoint, (void *)data, inputDataLength, dst, time, &err);\n\n"
            functionWrapper += "\tmxArray *_out = (mxArray *)0;\n"
            functionWrapper += "\tif(_out){\n"
            functionWrapper += "\t\t--resc;\n"
            functionWrapper += "\t\t*resv++ = _out;\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += f'{argCharPostFunctionCall("data")}\n\n'
            functionWrapper += f'{argCharPostFunctionCall("dst")}\n\n'
            functionWrapper += f'{argHelicsErrorPtrPostFunctionCall("err")}\n'
            functionWrapper += "}\n\n\n"
            functionMainElements = f"\tcase {cursorIdx}:\n"
            functionMainElements += f"\t\t_wrap_{functionName}(resc, resv, argc, (mxArray**)(argv));\n"
            functionMainElements += f"\t\tbreak;\n"
            return functionComment, functionWrapper, functionMainElements
        
        
        def helicsFederateRequestTimeIterativeMatlabWrapper(functionDict: dict(), cursorIdx: int):
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
            functionComment = "%{\n"
            functionComment += "\tRequest an iterative time.\n\n"
            functionComment += "\t@details This call allows for finer grain control of the iterative process than /ref helicsFederateRequestTime. It takes a time and and\n"
            functionComment += "\titeration request, and returns a time and iteration status.\n\n"
            functionComment += "\t@param fed The federate to make the request of.\n"
            functionComment += "\t@param requestTime The next desired time.\n"
            functionComment += "\t@param iterate The requested iteration mode.\n\n"
            functionComment += "\t@return granted time and HelicsIterationResult.\n"
            functionComment += "%}\n"
            functionWrapper = f"void _wrap_{functionName}(int resc, mxArray *resv[], int argc, const mxArray *argv[])" + "{\n"
            functionWrapper += initializeArgHelicsClass("HelicsFederate", "fed", 0)
            functionWrapper += initializeArgHelicsTime("requestTime", 1)
            functionWrapper += f"\tHelicsIterationRequest iterate = (HelicsIterationRequest)(mxGetScalar(argv[2]));\n\n"
            functionWrapper += initializeArgHelicsIterationResultPtr("outIteration")
            functionWrapper += initializeArgHelicsErrorPtr("err")
            functionWrapper += f"\tHelicsTime result = {functionName}(fed, requestTime, iterate, outIteration, &err);\n\n"
            functionWrapper += returnDoubleTomxArray()
            functionWrapper += "\tif(_out){\n"
            functionWrapper += "\t\t--resc;\n"
            functionWrapper += "\t\t*resv++ = _out;\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += "\tif(--resc>=0){\n"
            functionWrapper += "\t\tmxArray *_out1 = mxCreateNumericMatrix(1,1,mxINT64_CLASS,mxREAL);\n"
            functionWrapper += "\t\t*((int64_T*)mxGetData(_out1)) = (int64_T)outIteration;\n"
            functionWrapper += "\t\t*resv++ = _out1;\n"
            functionWrapper += "\t}\n\n"            
            functionWrapper += f'{argHelicsErrorPtrPostFunctionCall("err")}\n'
            functionWrapper += "}\n\n\n"
            functionMainElements = f"\tcase {cursorIdx}:\n"
            functionMainElements += f"\t\t_wrap_{functionName}(resc, resv, argc, (mxArray**)(argv));\n"
            functionMainElements += f"\t\tbreak;\n"
            return functionComment, functionWrapper, functionMainElements
        
        
        def helicsFederateRequestTimeIterativeCompleteMatlabWrapper(functionDict: dict(), cursorIdx: int):
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
            functionComment = "%{\n"
            functionComment += "\tComplete an iterative time request asynchronous call.\n\n"
            functionComment += "\t@param fed The federate to make the request of.\n\n"
            functionComment += "\t@return tuple of HelicsTime and HelicsIterationResult.\n"
            functionComment += "%}\n"
            functionWrapper = f"void _wrap_{functionName}(int resc, mxArray *resv[], int argc, const mxArray *argv[])" + "{\n"
            functionWrapper += initializeArgHelicsClass("HelicsFederate", "fed", 0)
            functionWrapper += initializeArgHelicsTime("requestTime", 1)
            functionWrapper += initializeArgHelicsIterationResultPtr("outIteration")
            functionWrapper += initializeArgHelicsErrorPtr("err")
            functionWrapper += f"\tHelicsTime result = {functionName}(fed, requestTime, outIteration, &err);\n\n"
            functionWrapper += returnDoubleTomxArray()
            functionWrapper += "\tif(_out){\n"
            functionWrapper += "\t\t--resc;\n"
            functionWrapper += "\t\t*resv++ = _out;\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += "\tif(--resc>=0){\n"
            functionWrapper += "\t\tmxArray *_out1 = mxCreateNumericMatrix(1,1,mxINT64_CLASS,mxREAL);\n"
            functionWrapper += "\t\t*((int64_T*)mxGetData(_out1)) = (int64_T)outIteration;\n"
            functionWrapper += "\t\t*resv++ = _out1;\n"
            functionWrapper += "\t}\n\n"            
            functionWrapper += f'{argHelicsErrorPtrPostFunctionCall("err")}\n'
            functionWrapper += "}\n\n\n"
            functionMainElements = f"\tcase {cursorIdx}:\n"
            functionMainElements += f"\t\t_wrap_{functionName}(resc, resv, argc, (mxArray**)(argv));\n"
            functionMainElements += f"\t\tbreak;\n"
            return functionComment, functionWrapper, functionMainElements
        
        
        def helicsInputGetBytesMatlabWrapper(functionDict: dict(), cursorIdx: int):
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
            functionComment = "%{\n"
            functionComment += "\tGet the raw data for the latest value of a subscription.\n\n"
            functionComment += "\t@param ipt The input to get the data for.\n\n"
            functionComment += "\t@return  raw Bytes of the value, the value is uninterpreted raw bytes.\n"
            functionComment += "%}\n"
            functionWrapper = f"void _wrap_{functionName}(int resc, mxArray *resv[], int argc, const mxArray *argv[])" + "{\n"
            functionWrapper += initializeArgHelicsClass("HelicsInput", "ipt", 0)
            functionWrapper += "\tint maxDataLen = helicsInputGetByteCount(ipt) + 2;\n\n"
            functionWrapper += "\tvoid *data = mxMalloc(maxDataLen);\n\n"
            functionWrapper += "\tint *actualSize = (int *)0;\n\n"
            functionWrapper += initializeArgHelicsErrorPtr("err")
            functionWrapper += f"\t{functionName}(ipt, data, maxDatalen, actualSize, &err);\n\n"
            functionWrapper += "\tmwSize dims[2] = {1, *actualSize};\n"
            functionWrapper += "\tmxArray *_out = mxCreateCharArray(2, dims);\n"
            functionWrapper += "\tmxChar *out_data = (mxChar *)mxGetData(_out);\n"
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
            functionMainElements += f"\t\t_wrap_{functionName}(resc, resv, argc, (mxArray**)(argv));\n"
            functionMainElements += f"\t\tbreak;\n"
            return functionComment, functionWrapper, functionMainElements
        
        
        def helicsInputGetComplexObjectMatlabWrapper(functionDict: dict(), cursorIdx: int):
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
            functionComment = "%{\n"
            functionComment += "\tGet a complex value from an input object.\n\n"
            functionComment += "\t@param ipt The input to get the data for.\n\n"
            functionComment += "\t@return  A complex number.\n"
            functionComment += "%}\n"
            functionWrapper = f"void _wrap_{functionName}(int resc, mxArray *resv[], int argc, const mxArray *argv[])" + "{\n"
            functionWrapper += initializeArgHelicsClass("HelicsInput", "ipt", 0)
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
            functionMainElements += f"\t\t_wrap_{functionName}(resc, resv, argc, (mxArray**)(argv));\n"
            functionMainElements += f"\t\tbreak;\n"
            return functionComment, functionWrapper, functionMainElements
        
        
        def helicsInputGetComplexMatlabWrapper(functionDict: dict(), cursorIdx: int):
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
            functionComment = "%{\n"
            functionComment += "\tGet a complex value from an input object.\n\n"
            functionComment += "\t@param ipt The input to get the data for.\n\n"
            functionComment += "\t@return  A complex number.\n"
            functionComment += "%}\n"
            functionWrapper = f"void _wrap_{functionName}(int resc, mxArray *resv[], int argc, const mxArray *argv[])" + "{\n"
            functionWrapper += initializeArgHelicsClass("HelicsInput", "ipt", 0)
            functionWrapper += "\tdouble values[2];\n\n"
            functionWrapper += initializeArgHelicsErrorPtr("err")
            functionWrapper += f"\t{functionName}(ipt, &(vaules[0]), &(values[1]), &err);\n\n"
            functionWrapper += "\tmxComplexDouble complex_result;\n"
            functionWrapper += "\tcomplex_result.real = values[0];\n"
            functionWrapper += "\tcomplex_result.imag = values[1];\n"
            functionWrapper += "\tmxArray *_out = mxCreateDoubleMatrix(1,1,mxCOMPLEX);\n"
            functionWrapper += "\tint status = mxSetComplexDoubles(_out, &complex_result);\n\n"
            functionWrapper += "\tif(_out){\n"
            functionWrapper += "\t\t--resc;\n"
            functionWrapper += "\t\t*resv++ = _out;\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += f'{argHelicsErrorPtrPostFunctionCall("err")}\n'
            functionWrapper += "}\n\n\n"
            functionMainElements = f"\tcase {cursorIdx}:\n"
            functionMainElements += f"\t\t_wrap_{functionName}(resc, resv, argc, (mxArray**)(argv));\n"
            functionMainElements += f"\t\tbreak;\n"
            return functionComment, functionWrapper, functionMainElements
        
        
        def helicsInputGetNamedPointMatlabWrapper(functionDict: dict(), cursorIdx: int):
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
            functionComment = "%{\n"
            functionComment += "\tGet a named point from a subscription.\n\n"
            functionComment += "\t@param ipt The input to get the result for.\n\n"
            functionComment += "\t@return a string and a double value for the named point\n"
            functionComment += "%}\n"
            functionWrapper = f"void _wrap_{functionName}(int resc, mxArray *resv[], int argc, const mxArray *argv[])" + "{\n"
            functionWrapper += initializeArgHelicsClass("HelicsInput", "ipt", 0)
            functionWrapper += "\tint maxStringLen = helicsInputGetStringSize(ipt) + 2;\n\n"
            functionWrapper += "\tchar *outputString = (char *)mxMalloc(maxStringLen);\n\n"
            functionWrapper += "\tint *actualLength = (int *)0;\n\n"
            functionWrapper += "\tdouble *val = (double *)0;\n\n"
            functionWrapper += initializeArgHelicsErrorPtr("err")
            functionWrapper += f"\t{functionName}(ipt, outputString, maxStringLen, actualLength, val, &err);\n\n"
            functionWrapper += "\tmwSize dims[2] = {1, *actualLength};\n"
            functionWrapper += "\tmxArray *_out = mxCreateCharArray(2, dims);\n"
            functionWrapper += "\tmxChar *out_data = (mxChar *)mxGetData(_out);\n"
            functionWrapper += "\tfor(int i=0; i<actualLength; ++i){\n"
            functionWrapper += "\t\tout_data[i] = outputString[i];\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += "\tif(_out){\n"
            functionWrapper += "\t\t--resc;\n"
            functionWrapper += "\t\t*resv++ = _out;\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += "\tif(--resc>=0){\n"
            functionWrapper += "\t\tmxArray *_out1 = mxCreateScalar(*val);\n"
            functionWrapper += "\t\t*resv++ = _out1;\n"
            functionWrapper += "\t}\n\n" 
            functionWrapper += f'{argCharPostFunctionCall("outputString")}\n\n'
            functionWrapper += f'{argHelicsErrorPtrPostFunctionCall("err")}\n'
            functionWrapper += "}\n\n\n"
            functionMainElements = f"\tcase {cursorIdx}:\n"
            functionMainElements += f"\t\t_wrap_{functionName}(resc, resv, argc, (mxArray**)(argv));\n"
            functionMainElements += f"\t\tbreak;\n"
            return functionComment, functionWrapper, functionMainElements
        
        
        def helicsInputGetStringMatlabWrapper(functionDict: dict(), cursorIdx: int):
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
            functionComment = "%{\n"
            functionComment += "\tGet a string value from a subscription.\n\n"
            functionComment += "\t@param ipt The input to get the string for.\n\n"
            functionComment += "\t@return the string value.\n"
            functionComment += "%}\n"
            functionWrapper = f"void _wrap_{functionName}(int resc, mxArray *resv[], int argc, const mxArray *argv[])" + "{\n"
            functionWrapper += initializeArgHelicsClass("HelicsInput", "ipt", 0)
            functionWrapper += "\tint maxStringLen = helicsInputGetStringSize(ipt) + 2;\n\n"
            functionWrapper += "\tchar *outputString = (char *)mxMalloc(maxStringLen);\n\n"
            functionWrapper += "\tint *actualLength = (int *)0;\n\n"
            functionWrapper += initializeArgHelicsErrorPtr("err")
            functionWrapper += f"\t{functionName}(ipt, outputString, maxStringLen, actualLength, val, &err);\n\n"
            functionWrapper += "\tmwSize dims[2] = {1, *actualLength};\n"
            functionWrapper += "\tmxArray *_out = mxCreateCharArray(2, dims);\n"
            functionWrapper += "\tmxChar *out_data = (mxChar *)mxGetData(_out);\n"
            functionWrapper += "\tfor(int i=0; i<actualLength; ++i){\n"
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
            functionMainElements += f"\t\t_wrap_{functionName}(resc, resv, argc, (mxArray**)(argv));\n"
            functionMainElements += f"\t\tbreak;\n"
            return functionComment, functionWrapper, functionMainElements
        
        
        def helicsInputGetVectorMatlabWrapper(functionDict: dict(), cursorIdx: int):
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
            functionComment = "%{\n"
            functionComment += "\tGet a vector from a subscription.\n\n"
            functionComment += "\t@param ipt The input to get the vector for.\n\n"
            functionComment += "\t@return  a list of floating point values.\n"
            functionComment += "%}\n"
            functionWrapper = f"void _wrap_{functionName}(int resc, mxArray *resv[], int argc, const mxArray *argv[])" + "{\n"
            functionWrapper += initializeArgHelicsClass("HelicsInput", "ipt", 0)
            functionWrapper += "\tint maxLength = helicsInputGetVectorSize(ipt);\n\n"
            functionWrapper += "\tdouble data[maxLength];\n\n"
            functionWrapper += "\tint *actualSize = (int *)0;\n\n"
            functionWrapper += initializeArgHelicsErrorPtr("err")
            functionWrapper += f"\t{functionName}(ipt, data, maxLength, actualSize, &err);\n\n"
            functionWrapper += "\tmxDouble result_data[*actualSize];\n"
            functionWrapper += "\tfor(int i=0; i<(*actualSize); ++i){\n"
            functionWrapper += "\t\tresult_data[i] = (mxDouble)data[i];\n"
            functionWrapper += "\t}\n"
            functionWrapper += "\tmxArray *_out = mxCreateDoubleMatrix(*actualSize, 1, mxREAL);\n"
            functionWrapper += "\tint status = mxSetDoubles(_out, &(resutl_data[0]));\n\n"
            functionWrapper += "\tif(_out){\n"
            functionWrapper += "\t\t--resc;\n"
            functionWrapper += "\t\t*resv++ = _out;\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += f'{argHelicsErrorPtrPostFunctionCall("err")}\n'
            functionWrapper += "}\n\n\n"
            functionMainElements = f"\tcase {cursorIdx}:\n"
            functionMainElements += f"\t\t_wrap_{functionName}(resc, resv, argc, (mxArray**)(argv));\n"
            functionMainElements += f"\t\tbreak;\n"
            return functionComment, functionWrapper, functionMainElements
        
        
        def helicsInputGetComplexVectorMatlabWrapper(functionDict: dict(), cursorIdx: int):
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
            functionComment = "%{\n"
            functionComment += "\tGet a compelx vector from a subscription.\n\n"
            functionComment += "\t@param ipt The input to get the vector for.\n\n"
            functionComment += "\t@return a list of complex values.\n"
            functionComment += "%}\n"
            functionWrapper = f"void _wrap_{functionName}(int resc, mxArray *resv[], int argc, const mxArray *argv[])" + "{\n"
            functionWrapper += initializeArgHelicsClass("HelicsInput", "ipt", 0)
            functionWrapper += "\tint maxLength = helicsInputGetVectorSize(ipt);\n\n"
            functionWrapper += "\tdouble data[maxLength];\n\n"
            functionWrapper += "\tint *actualSize = (int *)0;\n\n"
            functionWrapper += initializeArgHelicsErrorPtr("err")
            functionWrapper += f"\t{functionName}(ipt, data, maxLength, actualSize, &err);\n\n"
            functionWrapper += "\tmxComplexDouble result_data[*actualSize/2];\n"
            functionWrapper += "\tfor(int i=0; i<(*actualSize/2); ++i){\n"
            functionWrapper += "\t\tresult_data[i].real = data[2*(i)];\n"
            functionWrapper += "\t\tresult_data[i].imag = data[2*(i) + 1];\n"
            functionWrapper += "\t}\n"
            functionWrapper += "\tmxArray *_out = mxCreateDoubleMatrix(*actualSize/2, 1, mxCOMPLEX);\n"
            functionWrapper += "\tint status = mxSetComplexDoubles(_out, &(resutl_data[0]));\n\n"
            functionWrapper += "\tif(_out){\n"
            functionWrapper += "\t\t--resc;\n"
            functionWrapper += "\t\t*resv++ = _out;\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += f'{argHelicsErrorPtrPostFunctionCall("err")}\n'
            functionWrapper += "}\n\n\n"
            functionMainElements = f"\tcase {cursorIdx}:\n"
            functionMainElements += f"\t\t_wrap_{functionName}(resc, resv, argc, (mxArray**)(argv));\n"
            functionMainElements += f"\t\tbreak;\n"
            return functionComment, functionWrapper, functionMainElements
        
        
        def helicsInputSetDefaultBytesMatlabWrapper(functionDict: dict(), cursorIdx: int):
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
            functionComment = "%{\n"
            functionComment += "\tSet the default as a raw data array.\n\n"
            functionComment += "\t@param ipt The input to set the default for.\n"
            functionComment += "\t@param raw data to use for the default.\n"
            functionComment += "%}\n"
            functionWrapper = f"void _wrap_{functionName}(int resc, mxArray *resv[], int argc, const mxArray *argv[])" + "{\n"
            functionWrapper += initializeArgHelicsClass("HelicsInput", "ipt", 0)
            functionWrapper += initializeArgChar("data", 1)
            functionWrapper += "\tint inputDataLength = dataLength - 1;\n\n"
            functionWrapper += initializeArgHelicsErrorPtr("err")
            functionWrapper += f"\t{functionName}(ipt, (void *)data, inputDataLength, &err);\n\n"
            functionWrapper += "\tmxArray *_out = (mxArray *)0;\n"
            functionWrapper += "\tif(_out){\n"
            functionWrapper += "\t\t--resc;\n"
            functionWrapper += "\t\t*resv++ = _out;\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += f'{argCharPostFunctionCall("data")}\n\n'
            functionWrapper += f'{argHelicsErrorPtrPostFunctionCall("err")}\n'
            functionWrapper += "}\n\n\n"
            functionMainElements = f"\tcase {cursorIdx}:\n"
            functionMainElements += f"\t\t_wrap_{functionName}(resc, resv, argc, (mxArray**)(argv));\n"
            functionMainElements += f"\t\tbreak;\n"
            return functionComment, functionWrapper, functionMainElements
        
        
        def helicsInputSetDefaultComplexMatlabWrapper(functionDict: dict(), cursorIdx: int):
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
            functionComment = "%{\n"
            functionComment += "\tSet the default as a complex number.\n\n"
            functionComment += "\t@param ipt The input to get the data for.\n"
            functionComment += "\t@param value The default complex value.\n"
            functionComment += "%}\n"
            functionWrapper = f"void _wrap_{functionName}(int resc, mxArray *resv[], int argc, const mxArray *argv[])" + "{\n"
            functionWrapper += initializeArgHelicsClass("HelicsInput", "ipt", 0)
            functionWrapper += "\tmxComplexDouble *value = xmGetComplexDoubles(argv[1]);\n\n"
            functionWrapper += initializeArgHelicsErrorPtr("err")
            functionWrapper += f"\t{functionName}(ipt, (double)value->real, (double)value->imag, &err);\n\n"
            functionWrapper += "\tmxArray *_out = (mxArray *)0;\n"
            functionWrapper += "\tif(_out){\n"
            functionWrapper += "\t\t--resc;\n"
            functionWrapper += "\t\t*resv++ = _out;\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += f'{argCharPostFunctionCall("data")}\n\n'
            functionWrapper += f'{argHelicsErrorPtrPostFunctionCall("err")}\n'
            functionWrapper += "}\n\n\n"
            functionMainElements = f"\tcase {cursorIdx}:\n"
            functionMainElements += f"\t\t_wrap_{functionName}(resc, resv, argc, (mxArray**)(argv));\n"
            functionMainElements += f"\t\tbreak;\n"
            return functionComment, functionWrapper, functionMainElements
        
        
        def helicsInputSetDefaultVectorMatlabWrapper(functionDict: dict(), cursorIdx: int):
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
            functionComment = "%{\n"
            functionComment += "\tSet the default as a list of floats.\n\n"
            functionComment += "\t@param ipt The input to get the data for.\n"
            functionComment += "\t@param vectorInput The default list of floating point values.\n"
            functionComment += "%}\n"
            functionWrapper = f"void _wrap_{functionName}(int resc, mxArray *resv[], int argc, const mxArray *argv[])" + "{\n"
            functionWrapper += initializeArgHelicsClass("HelicsInput", "ipt", 0)
            functionWrapper += "\tint vectorLength =  (int)mxGetNumberOfElements(argv[1]);\n\n"
            functionWrapper += "\tdouble *vectorInput =  (double *)mxGetDoubles(argv[1]);\n\n"
            functionWrapper += initializeArgHelicsErrorPtr("err")
            functionWrapper += f"\t{functionName}(ipt, (const double *)vectorInput, vectorLength, &err);\n\n"
            functionWrapper += "\tmxArray *_out = (mxArray *)0;\n"
            functionWrapper += "\tif(_out){\n"
            functionWrapper += "\t\t--resc;\n"
            functionWrapper += "\t\t*resv++ = _out;\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += f'{argHelicsErrorPtrPostFunctionCall("err")}\n'
            functionWrapper += "}\n\n\n"
            functionMainElements = f"\tcase {cursorIdx}:\n"
            functionMainElements += f"\t\t_wrap_{functionName}(resc, resv, argc, (mxArray**)(argv));\n"
            functionMainElements += f"\t\tbreak;\n"
            return functionComment, functionWrapper, functionMainElements
        
        
        def helicsInputSetDefaultComplexVectorMatlabWrapper(functionDict: dict(), cursorIdx: int):
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
            functionComment = "%{\n"
            functionComment += "\tSet the default as a list of floats.\n\n"
            functionComment += "\t@param ipt The input to get the data for.\n"
            functionComment += "\t@param vectorInput The default list of complex values.\n"
            functionComment += "%}\n"
            functionWrapper = f"void _wrap_{functionName}(int resc, mxArray *resv[], int argc, const mxArray *argv[])" + "{\n"
            functionWrapper += initializeArgHelicsClass("HelicsInput", "ipt", 0)
            functionWrapper += "\tint vectorLength =  (int)mxGetN(argv[1])*2;\n\n"
            functionWrapper += "\tdouble vectorInput[vectorLength];\n"
            functionWrapper += "\tmxComplexDouble *vals = mxGetComplexDoubles(argv[1]);\n"
            functionWrapper += "\tfor(int i=0; i<vectorLength/2; ++i){\n"
            functionWrapper += "\t\tvectorInput[2*i] = vals[i].real;\n"
            functionWrapper += "\t\tvectorInput[2*i + 1] = vals[i].imag;\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += initializeArgHelicsErrorPtr("err")
            functionWrapper += f"\t{functionName}(ipt, (const double *)vectorInput, vectorLength, &err);\n\n"
            functionWrapper += "\tmxArray *_out = (mxArray *)0;\n"
            functionWrapper += "\tif(_out){\n"
            functionWrapper += "\t\t--resc;\n"
            functionWrapper += "\t\t*resv++ = _out;\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += f'{argHelicsErrorPtrPostFunctionCall("err")}\n'
            functionWrapper += "}\n\n\n"
            functionMainElements = f"\tcase {cursorIdx}:\n"
            functionMainElements += f"\t\t_wrap_{functionName}(resc, resv, argc, (mxArray**)(argv));\n"
            functionMainElements += f"\t\tbreak;\n"
            return functionComment, functionWrapper, functionMainElements
        
        
        def helicsMessageAppendDataMatlabWrapper(functionDict: dict(), cursorIdx: int):
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
            functionComment = "%{\n"
            functionComment += "\tAppend data to the payload.\n\n"
            functionComment += "\t@param message The message object in question.\n"
            functionComment += "\t@param data A string containing the message data to append.\n"
            functionComment += "%}\n"
            functionWrapper = f"void _wrap_{functionName}(int resc, mxArray *resv[], int argc, const mxArray *argv[])" + "{\n"
            functionWrapper += initializeArgHelicsClass("HelicsMessage", "message", 0)
            functionWrapper += initializeArgChar("data", 1)
            functionWrapper += "\tint inputDataLength = dataLength - 1;\n\n"
            functionWrapper += initializeArgHelicsErrorPtr("err")
            functionWrapper += f"\t{functionName}(message, (void *)data, inputDataLength, &err);\n\n"
            functionWrapper += "\tmxArray *_out = (mxArray *)0;\n"
            functionWrapper += "\tif(_out){\n"
            functionWrapper += "\t\t--resc;\n"
            functionWrapper += "\t\t*resv++ = _out;\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += f'{argHelicsErrorPtrPostFunctionCall("err")}\n'
            functionWrapper += "}\n\n\n"
            functionMainElements = f"\tcase {cursorIdx}:\n"
            functionMainElements += f"\t\t_wrap_{functionName}(resc, resv, argc, (mxArray**)(argv));\n"
            functionMainElements += f"\t\tbreak;\n"
            return functionComment, functionWrapper, functionMainElements
        
        
        def helicsMessageGetBytesMatlabWrapper(functionDict: dict(), cursorIdx: int):
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
            functionComment = "%{\n"
            functionComment += "\tGet the raw data for a message object.\n\n"
            functionComment += "\t@param message A message object to get the data for.\n\n"
            functionComment += "\t@return Raw string data.\n"
            functionComment += "%}\n"
            functionWrapper = f"void _wrap_{functionName}(int resc, mxArray *resv[], int argc, const mxArray *argv[])" + "{\n"
            functionWrapper += initializeArgHelicsClass("HelicsMessage", "message", 0)
            functionWrapper += "\tint maxMessageLength = helicsMessageGetByteCount(message) + 2;\n\n"
            functionWrapper += "\tchar *data = (char *)mxMalloc(maxMessageLength);\n\n"
            functionWrapper += "\tint *actualSize = (int *)0;\n\n"
            functionWrapper += initializeArgHelicsErrorPtr("err")
            functionWrapper += f"\t{functionName}(message, (void *)data, maxMessageLength, actualSize, &err);\n\n"
            functionWrapper += "\tmwSize dims[2] = {1,(mwSize)*actualSize};\n"
            functionWrapper += "\tmxArray *_out = mxCreateCharArray(2,dims);\n"
            functionWrapper += "\tmxChar *out_data = (mxChar *)mxGetData(_out);\n"
            functionWrapper += "\tfor(int i=0; i<*actualSize; ++i){\n"
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
            functionMainElements += f"\t\t_wrap_{functionName}(resc, resv, argc, (mxArray**)(argv));\n"
            functionMainElements += f"\t\tbreak;\n"
            return functionComment, functionWrapper, functionMainElements
        
        
        def helicsMessageSetDataMatlabWrapper(functionDict: dict(), cursorIdx: int):
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
            functionComment = "%{\n"
            functionComment += "\tSet the data payload of a message as raw data.\n\n"
            functionComment += "\t@param message The message object in question.\n"
            functionComment += "\t@param data A string containing the message data.\n"
            functionComment += "%}\n"
            functionWrapper = f"void _wrap_{functionName}(int resc, mxArray *resv[], int argc, const mxArray *argv[])" + "{\n"
            functionWrapper += initializeArgHelicsClass("HelicsMessage", "message", 0)
            functionWrapper += initializeArgChar("data", 1)
            functionWrapper += "\tint inputDataLength = dataLength - 1;\n\n"
            functionWrapper += initializeArgHelicsErrorPtr("err")
            functionWrapper += f"\t{functionName}(message, (void *)data, inputDataLength, &err);\n\n"
            functionWrapper += "\tmxArray *_out = (mxArray *)0;\n\n"
            functionWrapper += "\tif(_out){\n"
            functionWrapper += "\t\t--resc;\n"
            functionWrapper += "\t\t*resv++ = _out;\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += f'{argHelicsErrorPtrPostFunctionCall("err")}\n'
            functionWrapper += "}\n\n\n"
            functionMainElements = f"\tcase {cursorIdx}:\n"
            functionMainElements += f"\t\t_wrap_{functionName}(resc, resv, argc, (mxArray**)(argv));\n"
            functionMainElements += f"\t\tbreak;\n"
            return functionComment, functionWrapper, functionMainElements
        
        
        def helicsPublicationPublishBytesMatlabWrapper(functionDict: dict(), cursorIdx: int):
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
            functionComment = "%{\n"
            functionComment += "\tPublish raw data.\n\n"
            functionComment += "\t@param pub The publication to publish for.\n"
            functionComment += "\t@param data the raw byte data to publish.\n"
            functionComment += "%}\n"
            functionWrapper = f"void _wrap_{functionName}(int resc, mxArray *resv[], int argc, const mxArray *argv[])" + "{\n"
            functionWrapper += initializeArgHelicsClass("HelicsPublication", "pub", 0)
            functionWrapper += initializeArgChar("data", 1)
            functionWrapper += "\tint inputDataLength = dataLength - 1;\n\n"
            functionWrapper += initializeArgHelicsErrorPtr("err")
            functionWrapper += f"\t{functionName}(pub, (void *)data, inputDataLength, &err);\n\n"
            functionWrapper += "\tmxArray *_out = (mxArray *)0;\n\n"
            functionWrapper += "\tif(_out){\n"
            functionWrapper += "\t\t--resc;\n"
            functionWrapper += "\t\t*resv++ = _out;\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += f'{argHelicsErrorPtrPostFunctionCall("err")}\n'
            functionWrapper += "}\n\n\n"
            functionMainElements = f"\tcase {cursorIdx}:\n"
            functionMainElements += f"\t\t_wrap_{functionName}(resc, resv, argc, (mxArray**)(argv));\n"
            functionMainElements += f"\t\tbreak;\n"
            return functionComment, functionWrapper, functionMainElements
        
        
        def helicsPublicationPublishComplexMatlabWrapper(functionDict: dict(), cursorIdx: int):
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
            functionComment = "%{\n"
            functionComment += "\tPublish a complex number.\n\n"
            functionComment += "\t@param pub The publication to publish for.\n"
            functionComment += "\t@param value The complex number.\n"
            functionComment += "%}\n"
            functionWrapper = f"void _wrap_{functionName}(int resc, mxArray *resv[], int argc, const mxArray *argv[])" + "{\n"
            functionWrapper += initializeArgHelicsClass("HelicsPublication", "pub", 0)
            functionWrapper += "\tmxComplexDouble *complexValue = mxGetComplexDouble(argv[1]);\n"
            functionWrapper += "\tdouble value[2] = {complexValue[0].real, complexValue[0].imag};\n\n"
            functionWrapper += initializeArgHelicsErrorPtr("err")
            functionWrapper += f"\t{functionName}(pub, value[0], value[1], &err);\n\n"
            functionWrapper += "\tmxArray *_out = (mxArray *)0;\n\n"
            functionWrapper += "\tif(_out){\n"
            functionWrapper += "\t\t--resc;\n"
            functionWrapper += "\t\t*resv++ = _out;\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += f'{argHelicsErrorPtrPostFunctionCall("err")}\n'
            functionWrapper += "}\n\n\n"
            functionMainElements = f"\tcase {cursorIdx}:\n"
            functionMainElements += f"\t\t_wrap_{functionName}(resc, resv, argc, (mxArray**)(argv));\n"
            functionMainElements += f"\t\tbreak;\n"
            return functionComment, functionWrapper, functionMainElements
        
        
        def helicsPublicationPublishVectorMatlabWrapper(functionDict: dict(), cursorIdx: int):
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
            functionComment = "%{\n"
            functionComment += "\tPublish a vector of doubles.\n\n"
            functionComment += "\t@param pub The publication to publish for.\n"
            functionComment += "\t@param vectorInput The list of floating point values.\n"
            functionComment += "%}\n"
            functionWrapper = f"void _wrap_{functionName}(int resc, mxArray *resv[], int argc, const mxArray *argv[])" + "{\n"
            functionWrapper += initializeArgHelicsClass("HelicsPublication", "pub", 0)
            functionWrapper += "\tint vectorLength =  (int)mxGetNumberOfElements(argv[1]);\n\n"
            functionWrapper += "\tdouble *vectorInput =  (double *)mxGetDoubles(argv[1]);\n\n"
            functionWrapper += initializeArgHelicsErrorPtr("err")
            functionWrapper += f"\t{functionName}(pub, (const double *)vectorInput, vectorLength, &err);\n\n"
            functionWrapper += "\tmxArray *_out = (mxArray *)0;\n"
            functionWrapper += "\tif(_out){\n"
            functionWrapper += "\t\t--resc;\n"
            functionWrapper += "\t\t*resv++ = _out;\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += f'{argHelicsErrorPtrPostFunctionCall("err")}\n'
            functionWrapper += "}\n\n\n"
            functionMainElements = f"\tcase {cursorIdx}:\n"
            functionMainElements += f"\t\t_wrap_{functionName}(resc, resv, argc, (mxArray**)(argv));\n"
            functionMainElements += f"\t\tbreak;\n"
            return functionComment, functionWrapper, functionMainElements
        
        
        def helicsPublicationPublishComplexVectorMatlabWrapper(functionDict: dict(), cursorIdx: int):
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
            functionComment = "%{\n"
            functionComment += "\tPublish a vector of doubles.\n\n"
            functionComment += "\t@param pub The publication to publish for.\n"
            functionComment += "\t@param vectorInput The list of complex values.\n"
            functionComment += "%}\n"
            functionWrapper = f"void _wrap_{functionName}(int resc, mxArray *resv[], int argc, const mxArray *argv[])" + "{\n"
            functionWrapper += initializeArgHelicsClass("HelicsPublication", "pub", 0)
            functionWrapper += "\tint vectorLength =  (int)mxGetN(argv[1])*2;\n\n"
            functionWrapper += "\tdouble vectorInput[vectorLength];\n"
            functionWrapper += "\tmxComplexDouble *vals = mxGetComplexDoubles(argv[1]);\n"
            functionWrapper += "\tfor(int i=0; i<vectorLength/2; ++i){\n"
            functionWrapper += "\t\tvectorInput[2*i] = vals[i].real;\n"
            functionWrapper += "\t\tvectorInput[2*i + 1] = vals[i].imag;\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += initializeArgHelicsErrorPtr("err")
            functionWrapper += f"\t{functionName}(pub, (const double *)vectorInput, vectorLength, &err);\n\n"
            functionWrapper += "\tmxArray *_out = (mxArray *)0;\n"
            functionWrapper += "\tif(_out){\n"
            functionWrapper += "\t\t--resc;\n"
            functionWrapper += "\t\t*resv++ = _out;\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += f'{argHelicsErrorPtrPostFunctionCall("err")}\n'
            functionWrapper += "}\n\n\n"
            functionMainElements = f"\tcase {cursorIdx}:\n"
            functionMainElements += f"\t\t_wrap_{functionName}(resc, resv, argc, (mxArray**)(argv));\n"
            functionMainElements += f"\t\tbreak;\n"
            return functionComment, functionWrapper, functionMainElements
        
        
        def helicsQueryBufferFillMatlabWrapper(functionDict: dict(), cursorIdx: int):
            #check to see if function signiture changed
            argNum = len(functionDict.get("arguments", {}).keys())
            if argNum != 4:
                raise RuntimeError("the function signature for helicsQueryBufferFill has changed!")
            arg0 = functionDict.get("arguments", {}).get(0, {})
            if arg0.get("spelling","") != "buffer" or arg0.get("type", "") != "HelicsQueryBuffer":
                raise RuntimeError("the function signature for helicsQueryBufferFill has changed!")
            arg1 = functionDict.get("arguments", {}).get(1, {})
            if arg1.get("spelling","") != "str" or arg1.get("pointer_type", "") != "Char_S_*":
                raise RuntimeError("the function signature for helicsQueryBufferFill has changed!")
            arg2 = functionDict.get("arguments", {}).get(2, {})
            if arg2.get("spelling","") != "strSize" or arg2.get("type", "") != "Int":
                raise RuntimeError("the function signature for helicsQueryBufferFill has changed!")
            arg3 = functionDict.get("arguments", {}).get(3, {})
            if arg3.get("spelling","") != "err" or arg3.get("pointer_type", "") != "HelicsError_*":
                raise RuntimeError("the function signature for helicsQueryBufferFill has changed!")
            functionName = functionDict.get("spelling","")
            functionComment = "%{\n"
            functionComment += "\tSet the data for a query callback.\n\n"
            functionComment += "\t@details There are many queries that HELICS understands directly, but it is occasionally useful to have a federate be able to respond to specific queries with answers specific to a federate.\n\n"
            functionComment += "\t@param buffer The buffer received in a helicsQueryCallback.\n"
            functionComment += "\t@param string The string with the data to fill the buffer with.\n"
            functionComment += "%}\n"
            functionWrapper = f"void _wrap_{functionName}(int resc, mxArray *resv[], int argc, const mxArray *argv[])" + "{\n"
            functionWrapper += initializeArgHelicsClass("HelicsQueryBuffer", "buffer", 0)
            functionWrapper += initializeArgChar("str", 1)
            functionWrapper += "\tint strSize = strLength - 1;\n\n"
            functionWrapper += initializeArgHelicsErrorPtr("err")
            functionWrapper += f"\t{functionName}(buffer, str, strSize, &err);\n\n"
            functionWrapper += "\tmxArray *_out = (mxArray *)0;\n"
            functionWrapper += "\tif(_out){\n"
            functionWrapper += "\t\t--resc;\n"
            functionWrapper += "\t\t*resv++ = _out;\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += f'{argHelicsErrorPtrPostFunctionCall("err")}\n'
            functionWrapper += "}\n\n\n"
            functionMainElements = f"\tcase {cursorIdx}:\n"
            functionMainElements += f"\t\t_wrap_{functionName}(resc, resv, argc, (mxArray**)(argv));\n"
            functionMainElements += f"\t\tbreak;\n"
            return functionComment, functionWrapper, functionMainElements
        
        
        def helicsLoadSignalHandlerCallbackMatlabWrapper(functionDict: dict(), cursorIdx: int):
            return "","",""
        
        
        def helicsLoadSignalHandlerCallbackNoExitMatlabWrapper(functionDict: dict(), cursorIdx: int):
            return "","",""
        
        
        def helicsBrokerSetLoggingCallbackMatlabWrapper(functionDict: dict(), cursorIdx: int):
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
            functionComment = "%{\n"
            functionComment += "\tSet the logging callback to a broker\n\n"
            functionComment += "\t@details Add a logging callback function to a broker.\n\t\tThe logging callback will be called when\n\t\ta message flows into a broker from the core or from a broker.\n\n"
            functionComment += "\t@param broker The broker object in which to set the callback.\n"
            functionComment += "\t@param logger A function handle with the signature void(int loglevel, string identifier, string message).\n"
            functionComment += "%}\n"
            functionWrapper = "void matlabBrokerLoggingCallback(int loglevel, const char* identifier, const char* message, void* userData){\n"
            functionWrapper += "\tmxArray *lhs;\n"
            functionWrapper += "\tmxArray *rhs[4];"
            functionWrapper += "\trhs[0] = const_cast<mxArray *>(userData);\n"
            functionWrapper += "\trhs[1] = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);\n"
            functionWrapper += "\t*((int64_T*)mxGetData(out)) = (int64_T)loglevel;\n"
            functionWrapper += "\trhs[2] = mxCreateString(identifier);\n"
            functionWrapper += "\trhs[3] = mxCreateString(message);\n"
            functionWrapper += '\tint status = mexCallMATLAB(0,&lhs,4,rhs,"feval");\n'
            functionWrapper += "\tmxDestroyArray(lhs);\n"
            functionWrapper += "\tmxDestroyArray(rhs[1]);\n"
            functionWrapper += "\tmxDestroyArray(rhs[2]);\n"
            functionWrapper += "\tmxDestroyArray(rhs[3]);\n"
            functionWrapper += "}\n\n"
            functionWrapper += f"void _wrap_{functionName}(int resc, mxArray *resv[], int argc, const mxArray *argv[])" + "{\n"
            functionWrapper += initializeArgHelicsClass("HelicsBroker", "broker", 0)
            functionWrapper += "\tvoid *userData = mxGetData(argv[1]);\n"
            functionWrapper += initializeArgHelicsErrorPtr("err")
            functionWrapper += f"\t{functionName}(broker, &matlabBrokerLoggingCallback, userData, &err);\n\n"
            functionWrapper += "\tmxArray *_out = (mxArray *)0;\n"
            functionWrapper += "\tif(_out){\n"
            functionWrapper += "\t\t--resc;\n"
            functionWrapper += "\t\t*resv++ = _out;\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += f'{argHelicsErrorPtrPostFunctionCall("err")}\n'
            functionWrapper += "}\n\n\n"
            functionMainElements = f"\tcase {cursorIdx}:\n"
            functionMainElements += f"\t\t_wrap_{functionName}(resc, resv, argc, (mxArray**)(argv));\n"
            functionMainElements += f"\t\tbreak;\n"
            return functionComment, functionWrapper, functionMainElements
        
        
        def helicsCoreSetLoggingCallbackMatlabWrapper(functionDict: dict(), cursorIdx: int):
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
            functionComment = "%{\n"
            functionComment += "\tSet the logging callback to a core\n\n"
            functionComment += "\t@details Add a logging callback function to a core.\n\t\tThe logging callback will be called when\n\t\ta message flows into a core from the core or from a broker.\n\n"
            functionComment += "\t@param core The core object in which to set the callback.\n"
            functionComment += "\t@param logger A function handle with the signature void(int loglevel, string identifier, string message).\n"
            functionComment += "%}\n"
            functionWrapper = "void matlabCoreLoggingCallback(int loglevel, const char* identifier, const char* message, void* userData){\n"
            functionWrapper += "\tmxArray *lhs;\n"
            functionWrapper += "\tmxArray *rhs[4];"
            functionWrapper += "\trhs[0] = const_cast<mxArray *>(userData);\n"
            functionWrapper += "\trhs[1] = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);\n"
            functionWrapper += "\t*((int64_T*)mxGetData(out)) = (int64_T)loglevel;\n"
            functionWrapper += "\trhs[2] = mxCreateString(identifier);\n"
            functionWrapper += "\trhs[3] = mxCreateString(message);\n"
            functionWrapper += '\tint status = mexCallMATLAB(0,&lhs,4,rhs,"feval");\n'
            functionWrapper += "\tmxDestroyArray(lhs);\n"
            functionWrapper += "\tmxDestroyArray(rhs[1]);\n"
            functionWrapper += "\tmxDestroyArray(rhs[2]);\n"
            functionWrapper += "\tmxDestroyArray(rhs[3]);\n"
            functionWrapper += "}\n\n"
            functionWrapper += f"void _wrap_{functionName}(int resc, mxArray *resv[], int argc, const mxArray *argv[])" + "{\n"
            functionWrapper += initializeArgHelicsClass("HelicsCore", "core", 0)
            functionWrapper += "\tvoid *userData = mxGetData(argv[1]);\n"
            functionWrapper += initializeArgHelicsErrorPtr("err")
            functionWrapper += f"\t{functionName}(core, &matlabCoreLoggingCallback, userData, &err);\n\n"
            functionWrapper += "\tmxArray *_out = (mxArray *)0;\n"
            functionWrapper += "\tif(_out){\n"
            functionWrapper += "\t\t--resc;\n"
            functionWrapper += "\t\t*resv++ = _out;\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += f'{argHelicsErrorPtrPostFunctionCall("err")}\n'
            functionWrapper += "}\n\n\n"
            functionMainElements = f"\tcase {cursorIdx}:\n"
            functionMainElements += f"\t\t_wrap_{functionName}(resc, resv, argc, (mxArray**)(argv));\n"
            functionMainElements += f"\t\tbreak;\n"
            return functionComment, functionWrapper, functionMainElements
        
        
        def helicsFederateSetLoggingCallbackMatlabWrapper(functionDict: dict(), cursorIdx: int):
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
            functionComment = "%{\n"
            functionComment += "\tSet the logging callback for a federate\n\n"
            functionComment += "\t@details Add a logging callback function for a federate.\n\t\tThe logging callback will be called when\n\t\ta message flows into a federate from the core or from a federate.\n\n"
            functionComment += "\t@param fed The federate object in which to set the callback.\n"
            functionComment += "\t@param logger A function handle with the signature void(int loglevel, string identifier, string message).\n"
            functionComment += "%}\n"
            functionWrapper = "void matlabFederateLoggingCallback(int loglevel, const char* identifier, const char* message, void* userData){\n"
            functionWrapper += "\tmxArray *lhs;\n"
            functionWrapper += "\tmxArray *rhs[4];"
            functionWrapper += "\trhs[0] = const_cast<mxArray *>(userData);\n"
            functionWrapper += "\trhs[1] = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);\n"
            functionWrapper += "\t*((int64_T*)mxGetData(out)) = (int64_T)loglevel;\n"
            functionWrapper += "\trhs[2] = mxCreateString(identifier);\n"
            functionWrapper += "\trhs[3] = mxCreateString(message);\n"
            functionWrapper += '\tint status = mexCallMATLAB(0,&lhs,4,rhs,"feval");\n'
            functionWrapper += "\tmxDestroyArray(lhs);\n"
            functionWrapper += "\tmxDestroyArray(rhs[1]);\n"
            functionWrapper += "\tmxDestroyArray(rhs[2]);\n"
            functionWrapper += "\tmxDestroyArray(rhs[3]);\n"
            functionWrapper += "}\n\n"
            functionWrapper += f"void _wrap_{functionName}(int resc, mxArray *resv[], int argc, const mxArray *argv[])" + "{\n"
            functionWrapper += initializeArgHelicsClass("HelicsFederate", "fed", 0)
            functionWrapper += "\tvoid *userData = mxGetData(argv[1]);\n"
            functionWrapper += initializeArgHelicsErrorPtr("err")
            functionWrapper += f"\t{functionName}(fed, &matlabFederateLoggingCallback, userData, &err);\n\n"
            functionWrapper += "\tmxArray *_out = (mxArray *)0;\n"
            functionWrapper += "\tif(_out){\n"
            functionWrapper += "\t\t--resc;\n"
            functionWrapper += "\t\t*resv++ = _out;\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += f'{argHelicsErrorPtrPostFunctionCall("err")}\n'
            functionWrapper += "}\n\n\n"
            functionMainElements = f"\tcase {cursorIdx}:\n"
            functionMainElements += f"\t\t_wrap_{functionName}(resc, resv, argc, (mxArray**)(argv));\n"
            functionMainElements += f"\t\tbreak;\n"
            return functionComment, functionWrapper, functionMainElements
        
        
        def helicsFilterSetCustomCallbackMatlabWrapper(functionDict: dict(), cursorIdx: int):
            #check to see if function signiture changed
            argNum = len(functionDict.get("arguments", {}).keys())
            if argNum != 4:
                raise RuntimeError("the function signature for helicsFederateSetLoggingCallback has changed!")
            arg0 = functionDict.get("arguments", {}).get(0, {})
            if arg0.get("spelling","") != "filter" or arg0.get("type", "") != "HelicsFilter":
                raise RuntimeError("the function signature for helicsFederateSetLoggingCallback has changed!")
            arg1 = functionDict.get("arguments", {}).get(1, {})
            if arg1.get("spelling","") != "filtCall" or arg1.get("pointer_type", "") != "FunctionProto_*":
                raise RuntimeError("the function signature for helicsFederateSetLoggingCallback has changed!")
            arg2 = functionDict.get("arguments", {}).get(2, {})
            if arg2.get("spelling","") != "userdata" or arg2.get("pointer_type", "") != "Void_*":
                raise RuntimeError("the function signature for helicsFederateSetLoggingCallback has changed!")
            arg3 = functionDict.get("arguments", {}).get(3, {})
            if arg3.get("spelling","") != "err" or arg3.get("pointer_type", "") != "HelicsError_*":
                raise RuntimeError("the function signature for helicsFederateSetLoggingCallback has changed!")
            functionName = functionDict.get("spelling","")
            functionComment = "%{\n"
            functionComment += "\tSet a general callback for a custom filter.\n\n"
            functionComment += "\t@details Add a custom filter callback function for creating a custom filter operation in the c shared library.\n\n"
            functionComment += "\t@param filter The filter object in which to set the callback.\n"
            functionComment += "\t@param filtCall A function handle with the signature HelicsMessage(HelicsMessage message).\n"
            functionComment += "%}\n"
            functionWrapper = "HelicsMessage matlabFilterCustomCallback(HelicsMessage message, void* userData){\n"
            functionWrapper += "\tmxArray *lhs;\n"
            functionWrapper += "\tmxArray *rhs[2];"
            functionWrapper += "\trhs[0] = const_cast<mxArray *>(userData);\n"
            functionWrapper += "\trhs[1] = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);\n"
            functionWrapper += "\t*((unit64_T*)mxGetData(rhs[1])) = (uint64_T)message;\n"
            functionWrapper += '\tint status = mexCallMATLAB(1,&lhs,2,rhs,"feval");\n'
            functionWrapper += "\tHelicsMessage rv = (HelicsMessage)(mxGetData(lhs[0]));\n"
            functionWrapper += "\tmxDestroyArray(rhs[1]);\n"
            functionWrapper += "\treturn rv;\n"
            functionWrapper += "}\n\n"
            functionWrapper += f"void _wrap_{functionName}(int resc, mxArray *resv[], int argc, const mxArray *argv[])" + "{\n"
            functionWrapper += initializeArgHelicsClass("HelicsFilter", "filter", 0)
            functionWrapper += "\tvoid *userData = mxGetData(argv[1]);\n"
            functionWrapper += initializeArgHelicsErrorPtr("err")
            functionWrapper += f"\t{functionName}(filter, &matlabFilterCustomCallback, userData, &err);\n\n"
            functionWrapper += "\tmxArray *_out = (mxArray *)0;\n"
            functionWrapper += "\tif(_out){\n"
            functionWrapper += "\t\t--resc;\n"
            functionWrapper += "\t\t*resv++ = _out;\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += f'{argHelicsErrorPtrPostFunctionCall("err")}\n'
            functionWrapper += "}\n\n\n"
            functionMainElements = f"\tcase {cursorIdx}:\n"
            functionMainElements += f"\t\t_wrap_{functionName}(resc, resv, argc, (mxArray**)(argv));\n"
            functionMainElements += f"\t\tbreak;\n"
            return functionComment, functionWrapper, functionMainElements
        
        
        def helicsFederateSetQueryCallbackMatlabWrapper(functionDict: dict(), cursorIdx: int):
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
            functionComment = "%{\n"
            functionComment += "\tSet callback for queries executed against a federate.\n\n"
            functionComment += "\t@details There are many queries that HELICS understands directly, but it is occasionally useful to a have a federate be able to respond\n\ttospecific queries with answers specific to a federate.\n\n"
            functionComment += "\t@param fed The federate object in which to set the callback.\n"
            functionComment += "\t@param queryAnswer A function handle with the signature const char*(const char *query).\n"
            functionComment += "%}\n"
            functionWrapper = "HelicsMessage matlabFederateQueryCallback(const char* query, int querySize, HelicsQueryBuffer buffer, void* userData){\n"
            functionWrapper += "\tmxArray *lhs;\n"
            functionWrapper += "\tmxArray *rhs[2];"
            functionWrapper += "\mxSize dims[2] = {1, querySize};\n"
            functionWrapper += "\trhs[0] = const_cast<mxArray *>(userData);\n"
            functionWrapper += "\trhs[1] = mxCreateCharArray(2, dims);\n"
            functionWrapper += "\tmxChar *pQuery = (mxChar *)mxGetData(rhs[1]);\n"
            functionWrapper += "\tfor(int i=0; i<querySize; ++i){\n"
            functionWrapper += "\t\tpQuery[i] = query[i];\n"
            functionWrapper += "\t}\n"
            functionWrapper += '\tint status = mexCallMATLAB(1,&lhs,2,rhs,"feval");\n'
            functionWrapper += "\tchar *rStr;]n"
            functionWrapper += "\tsize_t rStrSize = mxGetN(lhs[0]) + 1;\n"
            functionWrapper += "\trStr = (char *)mxMalloc(rStrSize);\n"
            functionWrapper += "int status = mxGetString(lhs[0], (const char *)rStr, rStrSize);\n"
            functionWrapper += initializeArgHelicsErrorPtr("err")
            functionWrapper += "helicsQueryBufferFill(buffer, (const char *)rStr, (int)rStrSize, err);\n"
            functionWrapper += "\tmxDestroyArray(rhs[1]);\n"
            functionWrapper += "\tmxFree(rStr);\n"
            functionWrapper += f'{argHelicsErrorPtrPostFunctionCall("err")}\n'
            functionWrapper += "}\n\n"
            functionWrapper += f"void _wrap_{functionName}(int resc, mxArray *resv[], int argc, const mxArray *argv[])" + "{\n"
            functionWrapper += initializeArgHelicsClass("HelicsFilter", "filter", 0)
            functionWrapper += "\tvoid *userData = mxGetData(argv[1]);\n"
            functionWrapper += initializeArgHelicsErrorPtr("err")
            functionWrapper += f"\t{functionName}(filter, &matlabFilterCustomCallback, userData, &err);\n\n"
            functionWrapper += "\tmxArray *_out = (mxArray *)0;\n"
            functionWrapper += "\tif(_out){\n"
            functionWrapper += "\t\t--resc;\n"
            functionWrapper += "\t\t*resv++ = _out;\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += f'{argHelicsErrorPtrPostFunctionCall("err")}\n'
            functionWrapper += "}\n\n\n"
            functionMainElements = f"\tcase {cursorIdx}:\n"
            functionMainElements += f"\t\t_wrap_{functionName}(resc, resv, argc, (mxArray**)(argv));\n"
            functionMainElements += f"\t\tbreak;\n"
            return functionComment, functionWrapper, functionMainElements
        
        
        def helicsFederateSetTimeUpdateCallbackMatlabWrapper(functionDict: dict(), cursorIdx: int):
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
            functionComment = "%{\n"
            functionComment += "\tSet callback for the federate time update.n\n"
            functionComment += "\t@details This callback will be executed every time the simulation time is updated starting on entry to executing mode.\n\n"
            functionComment += "\t@param fed The federate object in which to set the callback.\n"
            functionComment += "\t@param timeUpdate A function handle with the signature void(double newTime, int iterating).\n"
            functionComment += "%}\n"
            functionWrapper = "void matlabFederateTimeUpdateCallback(HelicsTime newTime, HelicsBool iterating, void* userData){\n"
            functionWrapper += "\tmxArray *lhs;\n"
            functionWrapper += "\tmxArray *rhs[3];"
            functionWrapper += "\trhs[0] = const_cast<mxArray *>(userData);\n"
            functionWrapper += "\trhs[1] = mxCreateDoubleScalar((double)newTime);\n"
            functionWrapper += "\trhs[2] = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);\n"
            functionWrapper += "\t*((int64_T*)mxGetData(rhs[2]) =  (int46_T)iterating;\n"
            functionWrapper += '\tint status = mexCallMATLAB(0,&lhs,3,rhs,"feval");\n'
            functionWrapper += "\tmxDestroyArray(lhs);\n"
            functionWrapper += "\tmxDestroyArray(rhs[1]);\n"
            functionWrapper += "\tmxDestroyArray(rhs[2]);\n"
            functionWrapper += "}\n\n"
            functionWrapper += f"void _wrap_{functionName}(int resc, mxArray *resv[], int argc, const mxArray *argv[])" + "{\n"
            functionWrapper += initializeArgHelicsClass("HelicsFederate", "fed", 0)
            functionWrapper += "\tvoid *userData = mxGetData(argv[1]);\n"
            functionWrapper += initializeArgHelicsErrorPtr("err")
            functionWrapper += f"\t{functionName}(fed, &matlabFederateTimeUpdateCallback, userData, &err);\n\n"
            functionWrapper += "\tmxArray *_out = (mxArray *)0;\n"
            functionWrapper += "\tif(_out){\n"
            functionWrapper += "\t\t--resc;\n"
            functionWrapper += "\t\t*resv++ = _out;\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += f'{argHelicsErrorPtrPostFunctionCall("err")}\n'
            functionWrapper += "}\n\n\n"
            functionMainElements = f"\tcase {cursorIdx}:\n"
            functionMainElements += f"\t\t_wrap_{functionName}(resc, resv, argc, (mxArray**)(argv));\n"
            functionMainElements += f"\t\tbreak;\n"
            return functionComment, functionWrapper, functionMainElements
        
        
        def helicsFederateSetStateChangeCallbackMatlabWrapper(functionDict: dict(), cursorIdx: int):
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
            functionComment = "%{\n"
            functionComment += "\tSet callback for the federate mode change.n\n"
            functionComment += "\t@details This callback will be executed every time the operating mode of the federate changes.\n\n"
            functionComment += "\t@param fed The federate object in which to set the callback.\n"
            functionComment += "\t@param stateChange A function handle with the signature void(int newState, int oldState).\n"
            functionComment += "%}\n"
            functionWrapper = "void matlabFederateSetStateChangeCallback(HelicsFederateState newState, HelicsFederateState oldState, void* userData){\n"
            functionWrapper += "\tmxArray *lhs;\n"
            functionWrapper += "\tmxArray *rhs[3];"
            functionWrapper += "\trhs[0] = const_cast<mxArray *>(userData);\n"
            functionWrapper += "\trhs[1] = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);\n"
            functionWrapper += "\t*((int64_T*)mxGetData(rhs[2]) =  (int46_T)newState;\n"
            functionWrapper += "\trhs[2] = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);\n"
            functionWrapper += "\t*((int64_T*)mxGetData(rhs[2]) =  (int46_T)oldState;\n"
            functionWrapper += '\tint status = mexCallMATLAB(0,&lhs,3,rhs,"feval");\n'
            functionWrapper += "\tmxDestroyArray(lhs);\n"
            functionWrapper += "\tmxDestroyArray(rhs[1]);\n"
            functionWrapper += "\tmxDestroyArray(rhs[2]);\n"
            functionWrapper += "}\n\n"
            functionWrapper += f"void _wrap_{functionName}(int resc, mxArray *resv[], int argc, const mxArray *argv[])" + "{\n"
            functionWrapper += initializeArgHelicsClass("HelicsFederate", "fed", 0)
            functionWrapper += "\tvoid *userData = mxGetData(argv[1]);\n"
            functionWrapper += initializeArgHelicsErrorPtr("err")
            functionWrapper += f"\t{functionName}(fed, &matlabFederateSetStateChangeCallback, userData, &err);\n\n"
            functionWrapper += "\tmxArray *_out = (mxArray *)0;\n"
            functionWrapper += "\tif(_out){\n"
            functionWrapper += "\t\t--resc;\n"
            functionWrapper += "\t\t*resv++ = _out;\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += f'{argHelicsErrorPtrPostFunctionCall("err")}\n'
            functionWrapper += "}\n\n\n"
            functionMainElements = f"\tcase {cursorIdx}:\n"
            functionMainElements += f"\t\t_wrap_{functionName}(resc, resv, argc, (mxArray**)(argv));\n"
            functionMainElements += f"\t\tbreak;\n"
            return functionComment, functionWrapper, functionMainElements
        
        
        def helicsFederateSetTimeRequestEntryCallbackMatlabWrapper(functionDict: dict(), cursorIdx: int):
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
            functionComment = "%{\n"
            functionComment += "\tSet callback for the federate time request.n\n"
            functionComment += "\t@details This callback will be executed when a valid time request is made.\n"
            functionComment += "\tIt is intended for the possibility of embedded data grabbers in a callback to simplify user code.\n\n"
            functionComment += "\t@param fed The federate object in which to set the callback.\n"
            functionComment += "\t@param requestTime A callback with the signature void(double currentTime, double requestTime, bool iterating).\n"
            functionComment += "%}\n"
            functionWrapper = "void matlabFederateSetTimeRequestEntryCallback(HelicsTime currentTime, HelicsTime requestTime, HelicsBool iterating, void* userData){\n"
            functionWrapper += "\tmxArray *lhs;\n"
            functionWrapper += "\tmxArray *rhs[4];"
            functionWrapper += "\trhs[0] = const_cast<mxArray *>(userData);\n"
            functionWrapper += "\trhs[1] = mxCreateDoubleScalar(currentTime);\n"
            functionWrapper += "\trhs[2] = mxCreateDoubleScalar(requestTime);\n"
            functionWrapper += "\trhs[3] = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);\n"
            functionWrapper += "\t*((int64_T*)mxGetData(rhs[2]) =  (int46_T)iterating;\n"
            functionWrapper += '\tint status = mexCallMATLAB(0,&lhs,4,rhs,"feval");\n'
            functionWrapper += "\tmxDestroyArray(lhs);\n"
            functionWrapper += "\tmxDestroyArray(rhs[1]);\n"
            functionWrapper += "\tmxDestroyArray(rhs[2]);\n"
            functionWrapper += "\tmxDestroyArray(rhs[3]);\n"
            functionWrapper += "}\n\n"
            functionWrapper += f"void _wrap_{functionName}(int resc, mxArray *resv[], int argc, const mxArray *argv[])" + "{\n"
            functionWrapper += initializeArgHelicsClass("HelicsFederate", "fed", 0)
            functionWrapper += "\tvoid *userData = mxGetData(argv[1]);\n"
            functionWrapper += initializeArgHelicsErrorPtr("err")
            functionWrapper += f"\t{functionName}(fed, &matlabFederateSetTimeRequestEntryCallback, userData, &err);\n\n"
            functionWrapper += "\tmxArray *_out = (mxArray *)0;\n"
            functionWrapper += "\tif(_out){\n"
            functionWrapper += "\t\t--resc;\n"
            functionWrapper += "\t\t*resv++ = _out;\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += f'{argHelicsErrorPtrPostFunctionCall("err")}\n'
            functionWrapper += "}\n\n\n"
            functionMainElements = f"\tcase {cursorIdx}:\n"
            functionMainElements += f"\t\t_wrap_{functionName}(resc, resv, argc, (mxArray**)(argv));\n"
            functionMainElements += f"\t\tbreak;\n"
            return functionComment, functionWrapper, functionMainElements
        
        
        def helicsFederateSetTimeRequestReturnCallbackMatlabWrapper(functionDict: dict(), cursorIdx: int):
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
            functionComment = "%{\n"
            functionComment += "\tSet callback for the federate time request return.n\n"
            functionComment += "\t@details This callback will be executed when after all other callbacks for a time request return.\n"
            functionComment += "\tThis callback will be the last thing executed before returning control the user program.\n\n"
            functionComment += "\t@param fed The federate object in which to set the callback.\n"
            functionComment += "\t@param requestTimeReturn A callback with the signature void(double newTime, bool iterating).\n"
            functionComment += "%}\n"
            functionWrapper = "void matlabFederateSetTimeRequestReturnCallback(HelicsTime newTime, HelicsBool iterating, void* userData){\n"
            functionWrapper += "\tmxArray *lhs;\n"
            functionWrapper += "\tmxArray *rhs[3];"
            functionWrapper += "\trhs[0] = const_cast<mxArray *>(userData);\n"
            functionWrapper += "\trhs[1] = mxCreateDoubleScalar(newTime);\n"
            functionWrapper += "\trhs[2] = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);\n"
            functionWrapper += "\t*((int64_T*)mxGetData(rhs[2]) =  (int46_T)iterating;\n"
            functionWrapper += '\tint status = mexCallMATLAB(0,&lhs,3,rhs,"feval");\n'
            functionWrapper += "\tmxDestroyArray(lhs);\n"
            functionWrapper += "\tmxDestroyArray(rhs[1]);\n"
            functionWrapper += "\tmxDestroyArray(rhs[2]);\n"
            functionWrapper += "}\n\n"
            functionWrapper += f"void _wrap_{functionName}(int resc, mxArray *resv[], int argc, const mxArray *argv[])" + "{\n"
            functionWrapper += initializeArgHelicsClass("HelicsFederate", "fed", 0)
            functionWrapper += "\tvoid *userData = mxGetData(argv[1]);\n"
            functionWrapper += initializeArgHelicsErrorPtr("err")
            functionWrapper += f"\t{functionName}(fed, &matlabFederateSetTimeRequestReturnCallback, userData, &err);\n\n"
            functionWrapper += "\tmxArray *_out = (mxArray *)0;\n"
            functionWrapper += "\tif(_out){\n"
            functionWrapper += "\t\t--resc;\n"
            functionWrapper += "\t\t*resv++ = _out;\n"
            functionWrapper += "\t}\n\n"
            functionWrapper += f'{argHelicsErrorPtrPostFunctionCall("err")}\n'
            functionWrapper += "}\n\n\n"
            functionMainElements = f"\tcase {cursorIdx}:\n"
            functionMainElements += f"\t\t_wrap_{functionName}(resc, resv, argc, (mxArray**)(argv));\n"
            functionMainElements += f"\t\tbreak;\n"
            return functionComment, functionWrapper, functionMainElements
        
        
        def helicsIntToBytesMatlabWrapper(functionDict: dict(), cursorIdx: int):
            #check to see if function signiture changed
            argNum = len(functionDict.get("arguments", {}).keys())
            if argNum != 2:
                raise RuntimeError("the function signature for helicsIntToBytes has changed!")
            arg0 = functionDict.get("arguments", {}).get(0, {})
            if arg0.get("spelling","") != "value" or arg0.get("type", "") != "int64_t":
                raise RuntimeError("the function signature for helicsIntToBytes has changed!")
            arg1 = functionDict.get("arguments", {}).get(1, {})
            if arg1.get("spelling","") != "data" or arg1.get("type", "") != "HelicsDataBuffer":
                raise RuntimeError("the function signature for helicsIntToBytes has changed!")
            functionName = functionDict.get("spelling","")
            functionComment = "%{\n"
            functionComment += "\tconvert an int to serialized bytes.\n\n"
            functionComment += "\t@param value The integer.\n"
            functionComment += "\t@return HelicsDataBuffer.\n"
            functionComment += "%}\n"
            functionWrapper = f"void _wrap_{functionName}(int resc, mxArray *resv[], int argc, const mxArray *argv[])" + "{\n"
            functionWrapper += initializeArgInt64_t('value',0)
            functionWrapper += "\t HelicsDataBuffer data = helicsCreateDataBuffer(sizeof(int));\n\n"
            functionWrapper += f"\tint32_t result = {functionName}(value, data);\n\n"
            functionWrapper += "\tmxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);\n"
            functionWrapper += "\t*((uint64_T*)mxGetData(_out)) = (uint64_T)data;\n\n"
            functionWrapper += "\tif(_out){\n"
            functionWrapper += "\t\t--resc;\n"
            functionWrapper += "\t\t*resv++ = _out;\n"
            functionWrapper += "\t}\n"
            functionWrapper += "}\n\n\n"
            functionMainElements = f"\tcase {cursorIdx}:\n"
            functionMainElements += f"\t\t_wrap_{functionName}(resc, resv, argc, (mxArray**)(argv));\n"
            functionMainElements += f"\t\tbreak;\n"
            return functionComment, functionWrapper, functionMainElements
        
        
        def helicsDoubleToBytesMatlabWrapper(functionDict: dict(), cursorIdx: int):
            #check to see if function signiture changed
            argNum = len(functionDict.get("arguments", {}).keys())
            if argNum != 2:
                raise RuntimeError("the function signature for helicsDoubleToBytes has changed!")
            arg0 = functionDict.get("arguments", {}).get(0, {})
            if arg0.get("spelling","") != "value" or arg0.get("type", "") != "Double":
                raise RuntimeError("the function signature for helicsDoubleToBytes has changed!")
            arg1 = functionDict.get("arguments", {}).get(1, {})
            if arg1.get("spelling","") != "data" or arg1.get("type", "") != "HelicsDataBuffer":
                raise RuntimeError("the function signature for helicsDoubleToBytes has changed!")
            functionName = functionDict.get("spelling","")
            functionComment = "%{\n"
            functionComment += "\tconvert a double to serialized bytes.\n\n"
            functionComment += "\t@param value The double.\n"
            functionComment += "\t@return HelicsDataBuffer.\n"
            functionComment += "%}\n"
            functionWrapper = f"void _wrap_{functionName}(int resc, mxArray *resv[], int argc, const mxArray *argv[])" + "{\n"
            functionWrapper += initializeArgDouble('value',0)
            functionWrapper += "\t HelicsDataBuffer data = helicsCreateDataBuffer(sizeof(double));\n\n"
            functionWrapper += f"\tint32_t result = {functionName}(value, data);\n\n"
            functionWrapper += "\tmxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);\n"
            functionWrapper += "\t*((uint64_T*)mxGetData(_out)) = (uint64_T)data;\n\n"
            functionWrapper += "\tif(_out){\n"
            functionWrapper += "\t\t--resc;\n"
            functionWrapper += "\t\t*resv++ = _out;\n"
            functionWrapper += "\t}\n"
            functionWrapper += "}\n\n\n"
            functionMainElements = f"\tcase {cursorIdx}:\n"
            functionMainElements += f"\t\t_wrap_{functionName}(resc, resv, argc, (mxArray**)(argv));\n"
            functionMainElements += f"\t\tbreak;\n"
            return functionComment, functionWrapper, functionMainElements
        
        
        def helicsStringToBytesMatlabWrapper(functionDict: dict(), cursorIdx: int):
            #check to see if function signiture changed
            argNum = len(functionDict.get("arguments", {}).keys())
            if argNum != 2:
                raise RuntimeError("the function signature for helicsStringToBytes has changed!")
            arg0 = functionDict.get("arguments", {}).get(0, {})
            if arg0.get("spelling","") != "str" or arg0.get("pointer_type", "") != "Char_S_*":
                raise RuntimeError("the function signature for helicsStringToBytes has changed!")
            arg1 = functionDict.get("arguments", {}).get(1, {})
            if arg1.get("spelling","") != "data" or arg1.get("type", "") != "HelicsDataBuffer":
                raise RuntimeError("the function signature for helicsStringToBytes has changed!")
            functionName = functionDict.get("spelling","")
            functionComment = "%{\n"
            functionComment += "\tconvert a string to serialized bytes.\n\n"
            functionComment += "\t@param str The string.\n"
            functionComment += "\t@return HelicsDataBuffer.\n"
            functionComment += "%}\n"
            functionWrapper = f"void _wrap_{functionName}(int resc, mxArray *resv[], int argc, const mxArray *argv[])" + "{\n"
            functionWrapper += initializeArgChar('str',0)
            functionWrapper += "\t HelicsDataBuffer data = helicsCreateDataBuffer(strLength);\n\n"
            functionWrapper += f"\tint32_t result = {functionName}(str, data);\n\n"
            functionWrapper += "\tmxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);\n"
            functionWrapper += "\t*((uint64_T*)mxGetData(_out)) = (uint64_T)data;\n\n"
            functionWrapper += "\tif(_out){\n"
            functionWrapper += "\t\t--resc;\n"
            functionWrapper += "\t\t*resv++ = _out;\n"
            functionWrapper += "\t}\n"
            functionWrapper += "}\n\n\n"
            functionMainElements = f"\tcase {cursorIdx}:\n"
            functionMainElements += f"\t\t_wrap_{functionName}(resc, resv, argc, (mxArray**)(argv));\n"
            functionMainElements += f"\t\tbreak;\n"
            return functionComment, functionWrapper, functionMainElements
        
        
        def helicsBoolToBytesMatlabWrapper(functionDict: dict(), cursorIdx: int):
            #check to see if function signiture changed
            argNum = len(functionDict.get("arguments", {}).keys())
            if argNum != 2:
                raise RuntimeError("the function signature for helicsBoolToBytes has changed!")
            arg0 = functionDict.get("arguments", {}).get(0, {})
            if arg0.get("spelling","") != "value" or arg0.get("type", "") != "HelicsBool":
                raise RuntimeError("the function signature for helicsBoolToBytes has changed!")
            arg1 = functionDict.get("arguments", {}).get(1, {})
            if arg1.get("spelling","") != "data" or arg1.get("type", "") != "HelicsDataBuffer":
                raise RuntimeError("the function signature for helicsBoolToBytes has changed!")
            functionName = functionDict.get("spelling","")
            functionComment = "%{\n"
            functionComment += "\tconvert a HelicsBool to serialized bytes.\n\n"
            functionComment += "\t@param value The HelicsBool.\n"
            functionComment += "\t@return HelicsDataBuffer.\n"
            functionComment += "%}\n"
            functionWrapper = f"void _wrap_{functionName}(int resc, mxArray *resv[], int argc, const mxArray *argv[])" + "{\n"
            functionWrapper += initializeArgHelicsBool('value',0)
            functionWrapper += "\t HelicsDataBuffer data = helicsCreateDataBuffer(sizeof(HelicsBool);\n\n"
            functionWrapper += f"\tint32_t result = {functionName}(value, data);\n\n"
            functionWrapper += "\tmxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);\n"
            functionWrapper += "\t*((uint64_T*)mxGetData(_out)) = (uint64_T)data;\n\n"
            functionWrapper += "\tif(_out){\n"
            functionWrapper += "\t\t--resc;\n"
            functionWrapper += "\t\t*resv++ = _out;\n"
            functionWrapper += "\t}\n"
            functionWrapper += "}\n\n\n"
            functionMainElements = f"\tcase {cursorIdx}:\n"
            functionMainElements += f"\t\t_wrap_{functionName}(resc, resv, argc, (mxArray**)(argv));\n"
            functionMainElements += f"\t\tbreak;\n"
            return functionComment, functionWrapper, functionMainElements
        
        
        def helicsCharToBytesMatlabWrapper(functionDict: dict(), cursorIdx: int):
            #check to see if function signiture changed
            argNum = len(functionDict.get("arguments", {}).keys())
            if argNum != 2:
                raise RuntimeError("the function signature for helicsCharToBytes has changed!")
            arg0 = functionDict.get("arguments", {}).get(0, {})
            if arg0.get("spelling","") != "value" or arg0.get("type", "") != "Char_S":
                raise RuntimeError("the function signature for helicsCharToBytes has changed!")
            arg1 = functionDict.get("arguments", {}).get(1, {})
            if arg1.get("spelling","") != "data" or arg1.get("type", "") != "HelicsDataBuffer":
                raise RuntimeError("the function signature for helicsCharToBytes has changed!")
            functionName = functionDict.get("spelling","")
            functionComment = "%{\n"
            functionComment += "\tconvert a char to serialized bytes.\n\n"
            functionComment += "\t@param value The char.\n"
            functionComment += "\t@return HelicsDataBuffer.\n"
            functionComment += "%}\n"
            functionWrapper = f"void _wrap_{functionName}(int resc, mxArray *resv[], int argc, const mxArray *argv[])" + "{\n"
            functionWrapper += initializeArgChar('value',0)
            functionWrapper += "\t HelicsDataBuffer data = helicsCreateDataBuffer(valueLength);\n\n"
            functionWrapper += f"\tint32_t result = {functionName}(value, data);\n\n"
            functionWrapper += "\tmxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);\n"
            functionWrapper += "\t*((uint64_T*)mxGetData(_out)) = (uint64_T)data;\n\n"
            functionWrapper += "\tif(_out){\n"
            functionWrapper += "\t\t--resc;\n"
            functionWrapper += "\t\t*resv++ = _out;\n"
            functionWrapper += "\t}\n"
            functionWrapper += "}\n\n\n"
            functionMainElements = f"\tcase {cursorIdx}:\n"
            functionMainElements += f"\t\t_wrap_{functionName}(resc, resv, argc, (mxArray**)(argv));\n"
            functionMainElements += f"\t\tbreak;\n"
            return functionComment, functionWrapper, functionMainElements
        
        
        def helicsTimeToBytesMatlabWrapper(functionDict: dict(), cursorIdx: int):
            #check to see if function signiture changed
            argNum = len(functionDict.get("arguments", {}).keys())
            if argNum != 2:
                raise RuntimeError("the function signature for helicsTimeToBytes has changed!")
            arg0 = functionDict.get("arguments", {}).get(0, {})
            if arg0.get("spelling","") != "value" or arg0.get("type", "") != "HelicsTime":
                raise RuntimeError("the function signature for helicsTimeToBytes has changed!")
            arg1 = functionDict.get("arguments", {}).get(1, {})
            if arg1.get("spelling","") != "data" or arg1.get("type", "") != "HelicsDataBuffer":
                raise RuntimeError("the function signature for helicsTimeToBytes has changed!")
            functionName = functionDict.get("spelling","")
            functionComment = "%{\n"
            functionComment += "\tconvert a HelicsTime to serialized bytes.\n\n"
            functionComment += "\t@param value The HelicsTime value.\n"
            functionComment += "\t@return HelicsDataBuffer.\n"
            functionComment += "%}\n"
            functionWrapper = f"void _wrap_{functionName}(int resc, mxArray *resv[], int argc, const mxArray *argv[])" + "{\n"
            functionWrapper += initializeArgHelicsTime('value',0)
            functionWrapper += "\t HelicsDataBuffer data = helicsCreateDataBuffer(sizeof(HelicsTime));\n\n"
            functionWrapper += f"\tint32_t result = {functionName}(value, data);\n\n"
            functionWrapper += "\tmxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);\n"
            functionWrapper += "\t*((uint64_T*)mxGetData(_out)) = (uint64_T)data;\n\n"
            functionWrapper += "\tif(_out){\n"
            functionWrapper += "\t\t--resc;\n"
            functionWrapper += "\t\t*resv++ = _out;\n"
            functionWrapper += "\t}\n"
            functionWrapper += "}\n\n\n"
            functionMainElements = f"\tcase {cursorIdx}:\n"
            functionMainElements += f"\t\t_wrap_{functionName}(resc, resv, argc, (mxArray**)(argv));\n"
            functionMainElements += f"\t\tbreak;\n"
            return functionComment, functionWrapper, functionMainElements
        
        
        def helicsComplexToBytesMatlabWrapper(functionDict: dict(), cursorIdx: int):
            #check to see if function signiture changed
            argNum = len(functionDict.get("arguments", {}).keys())
            if argNum != 3:
                raise RuntimeError("the function signature for helicsComplexToBytes has changed!")
            arg0 = functionDict.get("arguments", {}).get(0, {})
            if arg0.get("spelling","") != "real" or arg0.get("type", "") != "Double":
                raise RuntimeError("the function signature for helicsComplexToBytes has changed!")
            arg1 = functionDict.get("arguments", {}).get(1, {})
            if arg1.get("spelling","") != "imag" or arg1.get("type", "") != "Double":
                raise RuntimeError("the function signature for helicsComplexToBytes has changed!")
            arg2 = functionDict.get("arguments", {}).get(2, {})
            if arg2.get("spelling","") != "data" or arg2.get("type", "") != "HelicsDataBuffer":
                raise RuntimeError("the function signature for helicsComplexToBytes has changed!")
            functionName = functionDict.get("spelling","")
            functionComment = "%{\n"
            functionComment += "\tconvert a complex to serialized bytes.\n\n"
            functionComment += "\t@param value The complex value.\n"
            functionComment += "\t@return HelicsDataBuffer.\n"
            functionComment += "%}\n"
            functionWrapper = f"void _wrap_{functionName}(int resc, mxArray *resv[], int argc, const mxArray *argv[])" + "{\n"
            functionWrapper += "\tmxComplexDouble *value = xmGetComplexDoubles(argv[1]);\n\n"
            functionWrapper += "\t HelicsDataBuffer data = helicsCreateDataBuffer(2*sizeof(double));\n\n"
            functionWrapper += f"\tint32_t result = {functionName}(value->real, value->imag, data);\n\n"
            functionWrapper += "\tmxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);\n"
            functionWrapper += "\t*((uint64_T*)mxGetData(_out)) = (uint64_T)data;\n\n"
            functionWrapper += "\tif(_out){\n"
            functionWrapper += "\t\t--resc;\n"
            functionWrapper += "\t\t*resv++ = _out;\n"
            functionWrapper += "\t}\n"
            functionWrapper += "}\n\n\n"
            functionMainElements = f"\tcase {cursorIdx}:\n"
            functionMainElements += f"\t\t_wrap_{functionName}(resc, resv, argc, (mxArray**)(argv));\n"
            functionMainElements += f"\t\tbreak;\n"
            return functionComment, functionWrapper, functionMainElements
        
        
        def helicsVectorToBytesMatlabWrapper(functionDict: dict(), cursorIdx: int):
            #check to see if function signiture changed
            argNum = len(functionDict.get("arguments", {}).keys())
            if argNum != 3:
                raise RuntimeError("the function signature for helicsVectorToBytes has changed!")
            arg0 = functionDict.get("arguments", {}).get(0, {})
            if arg0.get("spelling","") != "value" or arg0.get("pointer_type", "") != "Double_*":
                raise RuntimeError("the function signature for helicsVectorToBytes has changed!")
            arg1 = functionDict.get("arguments", {}).get(1, {})
            if arg1.get("spelling","") != "dataSize" or arg1.get("type", "") != "Int":
                raise RuntimeError("the function signature for helicsVectorToBytes has changed!")
            arg2 = functionDict.get("arguments", {}).get(2, {})
            if arg2.get("spelling","") != "data" or arg2.get("type", "") != "HelicsDataBuffer":
                raise RuntimeError("the function signature for helicsVectorToBytes has changed!")
            functionName = functionDict.get("spelling","")
            functionComment = "%{\n"
            functionComment += "\tconvert a real vector to serialized bytes.\n\n"
            functionComment += "\t@param value The vector of doubles.\n"
            functionComment += "\t@return HelicsDataBuffer.\n"
            functionComment += "%}\n"
            functionWrapper = f"void _wrap_{functionName}(int resc, mxArray *resv[], int argc, const mxArray *argv[])" + "{\n"
            functionWrapper += "\tint dataSize =  (int)mxGetNumberOfElements(argv[0]);\n\n"
            functionWrapper += "\tdouble *value =  (double *)mxGetDoubles(argv[0]);\n\n"
            functionWrapper += "\t HelicsDataBuffer data = helicsCreateDataBuffer(dataSize*sizeof(double));\n\n"
            functionWrapper += f"\tint32_t result = {functionName}((const double *)value, dataSize, data);\n\n"
            functionWrapper += "\tmxArray *_out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);\n"
            functionWrapper += "\t*((uint64_T*)mxGetData(_out)) = (uint64_T)data;\n\n"
            functionWrapper += "\tif(_out){\n"
            functionWrapper += "\t\t--resc;\n"
            functionWrapper += "\t\t*resv++ = _out;\n"
            functionWrapper += "\t}\n"
            functionWrapper += "}\n\n\n"
            functionMainElements = f"\tcase {cursorIdx}:\n"
            functionMainElements += f"\t\t_wrap_{functionName}(resc, resv, argc, (mxArray**)(argv));\n"
            functionMainElements += f"\t\tbreak;\n"
            return functionComment, functionWrapper, functionMainElements
        
        def createMexMain() -> str:
            mexMainStr = "void mexFunction(int resc, mxArray *resv[], int argc, const mxArray *argv[]) {\n"
            mexMainStr += "\tif(--argc < 0 || !mxIsString(*argv)){\n"
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
        
        
        if not os.path.exists("+helics"):
            os.makedirs("+helics")
        helicsMexStr = ""
        helicsMexWrapperFunctions = []
        helicsMexMainFunctionElements = []
        helicsMapTuples = []
        for cu in self.parsedInfo.keys():
            if self.parsedInfo[cu]["kind"] == cidx.CursorKind.ENUM_DECL.name:
                createEnum(self.parsedInfo[cu])
            if self.parsedInfo[cu]["kind"] == cidx.CursorKind.MACRO_DEFINITION.name:
                macroMexWrapperFunctionStr, macroMexMainFunctionElementStr, macroMapTuple= createMacro(self.parsedInfo[cu],int(cu))
                helicsMexWrapperFunctions.append(macroMexWrapperFunctionStr)
                helicsMexMainFunctionElements.append(macroMexMainFunctionElementStr)
                if macroMapTuple != None:
                    helicsMapTuples.append(macroMapTuple)
            if self.parsedInfo[cu]["kind"] == cidx.CursorKind.VAR_DECL.name:
                createVar(self.parsedInfo[cu],int(cu))
            if self.parsedInfo[cu]["kind"] == cidx.CursorKind.FUNCTION_DECL.name:
                functionMexWrapperFunctionStr, functionMexMainFunctionElementStr, functionMapTuple= createFunction(self.parsedInfo[cu],int(cu))
                helicsMexWrapperFunctions.append(functionMexWrapperFunctionStr)
                helicsMexMainFunctionElements.append(functionMexMainFunctionElementStr)
                if functionMapTuple != None:
                    helicsMapTuples.append(functionMapTuple)
        helicsMexStr += createBoilerPlate(self.headerFiles, helicsMapTuples)
        for wrapperStr in helicsMexWrapperFunctions:
            helicsMexStr += wrapperStr
        helicsMexStr += createMexMain()
        for element in helicsMexMainFunctionElements:
            helicsMexStr += element
        helicsMexStr += closeBoilerPlate()
        with open("helicsMex.cpp", "w") as helicsMexFile:
            helicsMexFile.write(helicsMexStr)
        clangLogger.info("MATLAB HELICS API successfully created!")


def main(helicsHeaders: List[str], createPythonBindings: bool, createMatlabBindings: bool) -> None:
    """
    Main function for building Non-native language bindings of the HELICS C library
    """
    helicsParser = HelicsHeaderParser(helicsHeaders)
    #===========================================================================
    # with open("parsedHeaderDict.json","w") as pFile:
    #         pFile.write(json.dumps(helicsParser.parsedInfo, indent=4, sort_keys=True))
    #         pFile.write(json.dumps(helicsParser._types, indent=4, sort_keys=True))
    #===========================================================================
    if createPythonBindings:
        helicsPythonBindingStr = helicsParser.createPythonBindings()
        with open("capi.py","w") as pythonBindingFile:
            pythonBindingFile.write(helicsPythonBindingStr)
        clangLogger.info("Python bindings successfully created!")
    if createMatlabBindings:
        helicsParser.createMatlabBindings()
        #clangLogger.info("Matlab binding successfully created!")
            

if __name__ == '__main__':
    userInputParser = ArgumentParser()
    userInputParser.add_argument("-p","--create_python_binding", action="store_true",
        default=False, help="Set to true to create the helics python binding.")
    userInputParser.add_argument("-m","--create_matlab_binding", action="store_true",
        default=False, help="Set to true to create the helics matlab binding.")
    userInputParser.add_argument("headers", default=[], nargs="+",
        help="list of helics header files to parse")
    userArgs = userInputParser.parse_args()
    main(userArgs.headers, userArgs.create_python_binding, userArgs.create_matlab_binding)
    

