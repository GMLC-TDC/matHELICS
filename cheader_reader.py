from argparse import ArgumentParser
import json
import platform
from typing import ClassVar, List, Dict, Any

import clang.cindex as cidx
from twisted.cred import strcred
from pickle import TRUE, NONE

class HelicsHeaderParser (object):
    """
        Class that will parse the HELICS C API headers and create other language bindings
        
        @ivar parsedInfo: a dictionary containing all the parsed cursors found in the HELICS C API headers
        
    """
    _types = {}
    parsedInfo: dict #Dictionary of parsed value 
    def __init__(self, helicsHeaders: List[str]):
        """
            Constructor
        """
        self._types["functions"] = {} 
        self.parsedInfo = {}
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
                if cursorInfoDict.get("result_type","") not in self._types.keys():
                    self._types[cursorInfoDict.get("result_type","")] = [cursorInfoDict.get("spelling","")]
                else:
                    self._types.get(cursorInfoDict.get("result_type",""),[]).append(cursorInfoDict.get("spelling",""))
            if cursorInfoDict.get("pointer_type","") != "":
                if cursorInfoDict.get("pointer_type","") not in self._types.keys():
                    self._types[cursorInfoDict.get("pointer_type","")] = [cursorInfoDict.get("spelling","")]
                else:
                    self._types.get(cursorInfoDict.get("pointer_type",""),[]).append(cursorInfoDict.get("spelling",""))
            if cursorInfoDict.get("double_pointer_type","") != "":
                if cursorInfoDict.get("double_pointer_type","") not in self._types.keys():
                    self._types[cursorInfoDict.get("double_pointer_type","")] = [cursorInfoDict.get("spelling","")]
                else:
                    self._types.get(cursorInfoDict.get("double_pointer_type",""),[]).append(cursorInfoDict.get("spelling",""))
            cursorInfoDict["arguments"] = {}
            argNum = 0
            for arg in node.get_arguments():
                cursorInfoDict["arguments"][argNum] = self._cursorInfo(arg)
                argNum += 1
            self._types["functions"][cursorInfoDict.get("spelling","")] = cursorInfoDict["arguments"]
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
                if cursorInfoDict.get("type","") not in self._types.keys():
                    self._types[cursorInfoDict.get("type","")] = [cursorInfoDict.get("spelling","")]
                else:
                    self._types.get(cursorInfoDict.get("type",""),[]).append(cursorInfoDict.get("spelling",""))
            if cursorInfoDict.get("pointer_type","") != "":
                if cursorInfoDict.get("pointer_type","") not in self._types.keys():
                    self._types[cursorInfoDict.get("pointer_type","")] = [cursorInfoDict.get("spelling","")]
                else:
                    self._types.get(cursorInfoDict.get("pointer_type",""),[]).append(cursorInfoDict.get("spelling",""))
            if cursorInfoDict.get("double_pointer_type","") != "":
                if cursorInfoDict.get("double_pointer_type","") not in self._types.keys():
                    self._types[cursorInfoDict.get("double_pointer_type","")] = [cursorInfoDict.get("spelling","")]
                else:
                    self._types.get(cursorInfoDict.get("double_pointer_type",""),[]).append(cursorInfoDict.get("spelling",""))                  
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
            value = ''
            for t in node.get_tokens():
                value = t.spelling
            cursorInfoDict["value"] = json.loads(value)
        if node.kind == cidx.CursorKind.STRUCT_DECL:
            cursorInfoDict["members"] = {}
            memberNum = 0
            for i in node.get_children():
                cursorInfoDict["members"][memberNum] = self._cursorInfo(i)
                memberNum += 1
        return cursorInfoDict
        
        
    def parseHelicsHeaderFiles(self, helicsHeaders: List[str]) -> None:
        """
            Function that parses the HELICS C header files
            @param helicsHeaders: A list of the HELICS C header files to parse
        """
        idx = cidx.Index.create()
        cursorNum = 0
        for headerFile in helicsHeaders:
            tu = idx.parse(headerFile)
            for c in tu.cursor.get_children():
                if c.location.file.name == headerFile:
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

    
    
    def createPythonBindings(self) -> str:
        """
            Function that creates the HELICS Python Bindings
        """
        def createBoilerPlate() -> str:
            """
                Create Python Binding boiler plate string 
            """
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
            return boilerPlateStr
        
        
        def createEnum(enumDict: dict()) -> str:
            """
                Create Python Binding Enumerations for each C Enumeration
            """
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
            typedefSpelling = typedefDict.get('spelling','')
            typedefComment = typedefDict.get('brief_comment','')
            if typedefSpelling != "HelicsComplex" and typedefSpelling != "HelicsError":
                if typedefDict.get('type', '') == "void *":
                    typedefStr = f"class {typedefSpelling}(_HelicsCHandle):\n"
                    typedefStr += f"\t\"\"\"\n\t{typedefComment}\n\t\"\"\"\n"
                    typedefStr += "\tpass\n\n\n"
                    return  typedefStr
                elif typedefDict.get('type', '') == "double":
                    return f"{typedefSpelling} = float\t#{typedefComment}\n\n\n"
                elif typedefDict.get('type', '') == "int":
                    return f"{typedefSpelling} = int\t#{typedefComment}\n\n\n"
                else:
                    raise RuntimeError(f"unhandled typedef type for {typedefSpelling}. type is {typedefDict.get('type','unknown')}")
            return ""
            
            
        def createStruct(structDict: dict()) -> str:
            """
            """
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
            varSpelling = varDict.get('spelling','')
            varValue = varDict.get('value')
            varComment = varDict.get('brief_comment','')
            if varSpelling == "HELICS_INVALID_OPTION_INDEX":
                return f"{varSpelling} = -{varValue}\n\n\n"
            else:
                return f"{varSpelling} = {varValue}\t#{varComment}\n\n\n"
            
                
        def createFunctionDeclaration(functionDict: dict()) -> str:
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
                functionStr += "\treturn ffi.string(data, maxlen=actualSize[0])\n\n\n"
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
                functionStr += "\treturn ffi.string(data, maxlen=actualSize[0])\n\n\n"
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
            if self.parsedInfo[cu]["kind"] == cidx.CursorKind.TYPEDEF_DECL.name and 'struct' not in self.parsedInfo[cu]["type"]:
                helicsBindingStr += createTypeDef(self.parsedInfo[cu])
            if self.parsedInfo[cu]["kind"] == cidx.CursorKind.STRUCT_DECL.name:
                helicsBindingStr += createStruct(self.parsedInfo[cu])
            if self.parsedInfo[cu]["kind"] == cidx.CursorKind.VAR_DECL.name:
                helicsBindingStr += createVarDeclaration(self.parsedInfo[cu])
            if self.parsedInfo[cu]["kind"] == cidx.CursorKind.FUNCTION_DECL.name:
                helicsBindingStr += createFunctionDeclaration(self.parsedInfo[cu])
        return helicsBindingStr 
            

def main(helicsHeaders: List[str], createPythonBindings: bool) -> None:
    """
    Main function for building Non-native language bindings of the HELICS C library
    """
    helicsParser = HelicsHeaderParser(helicsHeaders)
    with open("parsedHeaderDict.json","w") as pFile:
            pFile.write(json.dumps(helicsParser.parsedInfo, indent=4, sort_keys=True))
            pFile.write(json.dumps(helicsParser._types, indent=4, sort_keys=True))
    if createPythonBindings:
        helicsPythonBindingStr = helicsParser.createPythonBindings()
        with open("capi.py","w") as pythonBindingFile:
            pythonBindingFile.write(helicsPythonBindingStr)
        print("python bindings successfully created!")
            

if __name__ == '__main__':
    userInputParser = ArgumentParser()
    userInputParser.add_argument("-p","--create_python_binding", action="store_true",
        default=False, help="Set to true to create the helics python binding.")
    userInputParser.add_argument("headers", default=[], nargs="+",
        help="list of helics header files to parse")
    userArgs = userInputParser.parse_args()
    main(userArgs.headers, userArgs.create_python_binding)
    

