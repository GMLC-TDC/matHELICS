from argparse import ArgumentParser
import json
import platform
from typing import ClassVar, List, Dict, Any

import clang.cindex as cidx
from twisted.cred import strcred
from pickle import TRUE, NONE

class HelicsHeaderParser (object):
    """
    """
    _types = {}
    parsedInfo: dict #Dictionary of parsed value 
    def __init__(self, helicsHeaders: List[str]):
        """
        """
        self._types["functions"] = {} 
        self.parsedInfo = {}
        self.parseHelicsHeaderFiles(helicsHeaders)
    
    
    def _cursorInfo(self, node: cidx.Cursor) -> dict():
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
        idx = cidx.Index.create()
        cursorNum = 0
        for headerFile in  helicsHeaders:
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
        def createBoilerPlate() -> str:
            boilerPlateStr = "import enum\n"
            boilerPlateStr += "import signal\n"
            boilerPlateStr += "import sys\n"
            boilerPlateStr += "try:\n\tfrom typing import List, Tuple, Union\nexcept ImportError:\n\tpass\n\n"
            boilerPlateStr += "from . import _build\n\n\n"
            boilerPlateStr += "lib = _build.lib\n"
            boilerPlateStr += "ffi = _build.ffi\n\n\n" 
            boilerPlateStr += "def signal_handler(sig, frame):\n"
            boilerPlateStr += "\thelicsCloseLibrary()\n"
            boilerPlateStr += "\tprint(\"User pressed 'CTRL-C'. Exiting...\"\n"
            boilerPlateStr += "\tsys.exit(0)\n\n\n"
            boilerPlateStr += "signal.signal(signal.SIGINT, signal_handler)\n\n\n"
            boilerPlateStr += "class _HelicsCHandle:\n"
            boilerPlateStr += "\tdef __init__(self, handle):\n"
            boilerPlateStr += "\t\tself.handle = handle\n\n\n"
            boilerPlateStr += "class HelicsException(Exception):\n"
            boilerPlateStr += "\tpass\n\n\n"
            return boilerPlateStr
        
        
        def createEnum(enumDict: dict()) -> str:
            enumSpelling = enumDict.get('spelling','')
            enumComment = enumDict.get('brief_comment','')
            enumStr = f"class {enumSpelling}(enum.IntEnum):\n"
            enumStr += f"\t\"\"\" {enumComment}\n\n\tAttributes:"
            docStrBody = ""
            enumStrBody = ""
            for enumKey in enumDict.get('enumerations',{}).keys():
                keywordSpelling = enumDict.get('enumerations',{}).get(enumKey,{}).get('spelling','')
                keywordValue = enumDict.get('enumerations',{}).get(enumKey,{}).get('value')
                keywordComment = enumDict.get('enumerations',{}).get(enumKey,{}).get('brief_comment','')
                docStrBody += f"\n\t\t{keywordSpelling}: value:{keywordValue}\t{keywordComment}"
                enumStrBody += f"\n\t{keywordSpelling} = {keywordValue}"
            enumStr += docStrBody
            enumStr += "\n\t\"\"\"\n"
            enumStr += enumStrBody
            enumStr += "\n\n\n"
            return enumStr
        
        
        def createTypeDef(typedefDict: dict()) -> str:
            typedefSpelling = typedefDict.get('spelling','')
            typedefComment = typedefDict.get('brief_comment','')
            if typedefSpelling != "helics_complex" and typedefSpelling != "HelicsError":
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
            
                
        def createVarDeclaration(varDict: dict()) -> str:
            varSpelling = varDict.get('spelling','')
            varValue = varDict.get('value')
            varComment = varDict.get('brief_comment','')
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
                "helicsQueryBufferFill"
            ]
            if functionDict.get("spelling") in modifiedPythonFunctionList:
                return createModifiedPythonFunction(functionDict)
            functionReturnInfo = getReturnInfo(functionDict)
            argumentInfo = {}
            for a in functionDict.get('arguments',{}).keys():
                argumentInfo[a] = {}
                argumentInfo[a]["name"] = functionDict.get('arguments',{}).get(a,{}).get('spelling','')
                argumentInfo[a]["type"] = getPythonType(functionDict.get('arguments',{}).get(a,{}))
                argumentInfo[a]["arg_initialization"] = getArgInitializationText(functionDict.get('arguments',{}).get(a,{}))
                argumentInfo[a]["arg_function_call"] = getArgFunctionCallText(functionDict.get('arguments',{}).get(a,{}))
                argumentInfo[a]["arg_post_function_call"] = getArgPostFunctionCallText(functionDict.get('arguments',{}).get(a,{}))
            functionStr = f'def {functionDict.get("spelling","")}('
            for a in argumentInfo.keys():
                if a == 0:
                        functionStr += f'{argumentInfo[a].get("name","")}{argumentInfo[a].get("type","")}'
                else:
                    if argumentInfo[a].get("type","") != ": helics_error":
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
                "helics_bool": "\treturn result==1\n\n\n",
                "helics_broker": "\treturn helics_broker(result)\n\n\n",
                "helics_complex": "\treturn complex(result.real,result.imag)\n\n\n",
                "helics_core": "\treturn helics_core(result)\n\n\n",
                "helics_endpoint": "\treturn helics_endpoint(result)\n\n\n",
                "helics_error": '\treturn ffi.new("helics_error *",result)\n\n\n',
                "helics_federate": "\treturn helics_federate(result)\n\n\n",
                "helics_federate_info": "\treturn helics_federate_info(result)\n\n\n",
                "helics_federate_state": "\treturn helics_federate_state(result)\n\n\n",
                "helics_filter": "\treturn helics_filter(result)\n\n\n",
                "helics_input": "\treturn helics_input(result)\n\n\n",
                "helics_iteration_result": "\treturn helics_iteration_result(result)\n\n\n",
                "helics_message": "\treturn helics_message(result)\n\n\n",
                "helics_publication": "\treturn helics_publication(result)\n\n\n",
                "helics_query": "\treturn helics_query(result)\n\n\n",
                "helics_time": "\treturn result\n\n\n",
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
                "helics_bool": "bool",
                "helics_broker": "helics_broker",
                "helics_complex": "complex",
                "helics_core": "helics_core",
                "helics_data_type": "helics_data_type",
                "helics_endpoint": "helics_endpoint",
                "helics_error": "helics_error",
                "helics_error_*": "helics_error",
                "helics_federate": "helics_federate",
                "helics_federate_info": "helics_federate_info",
                "helics_federate_state": "helics_federate_state",
                "helics_filter": "helics_filter",
                "helics_filter_type": "helics_filter_type",
                "helics_input": "helics_input",
                "helics_iteration_request": "helics_iteration_request",
                "helics_iteration_result": "helics_iteration_result",
                "helics_iteration_result_*": "helics_iteration_result",
                "helics_message": "helics_message",
                "helics_publication": "helics_publication",
                "helics_query": "helics_query",
                "helics_query_buffer": "helics_query_buffer",
                "helics_time": "helics_time",
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
                "helics_bool": "",
                "helics_broker": "",
                "helics_core": "",
                "helics_data_type": "",
                "helics_endpoint": "",
                "helics_error_*": f'\t{argDict.get("spelling","")} = helicsErrorInitialize()\n',
                "helics_federate": "",
                "helics_federate_info": "",
                "helics_filter": "",
                "helics_filter_type": "",
                "helics_input": "",
                "helics_iteration_request": "",
                "helics_iteration_result_*": f'\t{argDict.get("spelling","")} = ffi.new("helics_iteration_result *")\n', 
                "helics_message": "",
                "helics_publication": "",
                "helics_query": "",
                "helics_query_buffer": "",
                "helics_time": "",
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
                "helics_bool": f'{argDict.get("spelling","")}',
                "helics_broker": f'{argDict.get("spelling","")}.handle',
                "helics_core": f'{argDict.get("spelling","")}.handle',
                "helics_data_type": f'helics_data_type({argDict.get("spelling","")})',
                "helics_endpoint": f'{argDict.get("spelling","")}.handle',
                "helics_error_*": f'{argDict.get("spelling","")}',
                "helics_federate": f'{argDict.get("spelling","")}.handle',
                "helics_federate_info": f'{argDict.get("spelling","")}.handle',
                "helics_filter": f'{argDict.get("spelling","")}.handle',
                "helics_filter_type": f'helics_filter_type({argDict.get("spelling","")})',
                "helics_input": f'{argDict.get("spelling","")}.handle',
                "helics_iteration_request": f'helics_filter_type({argDict.get("spelling","")})',
                "helics_iteration_result_*": f'{argDict.get("spelling","")}', 
                "helics_message": f'{argDict.get("spelling","")}.handle',
                "helics_publication": f'{argDict.get("spelling","")}.handle',
                "helics_query": f'{argDict.get("spelling","")}.handle',
                "helics_query_buffer": f'{argDict.get("spelling","")}.handle',
                "helics_time": f'{argDict.get("spelling","")}',
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
                "helics_bool": "",
                "helics_broker": "",
                "helics_core": "",
                "helics_data_type": "",
                "helics_endpoint": "",
                "helics_error_*": f'\tif {argDict.get("spelling","")}.error_code != 0:\n\t\traise HelicsException("[" + str({argDict.get("spelling","")}.error_code) + "] " + ffi.string({argDict.get("spelling","")}.message).decode())\n',
                "helics_federate": "",
                "helics_federate_info": "",
                "helics_filter": "",
                "helics_filter_type": "",
                "helics_input": "",
                "helics_iteration_request": "",
                "helics_iteration_result_*": "", 
                "helics_message": "",
                "helics_publication": "",
                "helics_query": "",
                "helics_query_buffer": "",
                "helics_time": "",
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
                if arg4.get("spelling","") != "err" or arg4.get("pointer_type", "") != "helics_error_*":
                    raise RuntimeError("the function signature for helicsCreateCoreFromArgs has changed!")
                functionStr = "def helicsCreateCoreFromArgs(type: str, name: str, arguments: List[str]) -> helics_core:\n"
                functionStr += "\t\"\"\"\n"
                functionStr += "\t\tCreate a core object by passing command line arguments.\n\n"
                functionStr += "\t\t@param type The type of the core to create.\n"
                functionStr += "\t\t@param name The name of the core. It can be an empty string to have a name automatically assigned..\n"
                functionStr += "\t\t@param arguments The list of string values from a command line.\n\n"
                functionStr += "\t\t@return A helics_core object.\n\t\"\"\"\n"
                functionStr += "\tfn = getattr(lib, \"helicsCreateCoreFromArgs\")\n"
                functionStr += "\targc = len(arguments)\n"
                functionStr += "\targv = ffi.new(f\"char*[{argc}]\")\n"
                functionStr += "\tfor i, s in enumerate(arguments):\n"
                functionStr += "\t\targv[i] = ffi.new(\"char[]\",s.encode())\n"
                functionStr += "\terr = helicsErrorInitialize()\n"
                functionStr += "\tresult = fn(ffi.new(\"char[]\",type.encode()), ffi.new(\"char[]\",name.encode()), argc, argv, err)\n"
                functionStr += "\tif err.error_code != 0:\n"
                functionStr += "\t\traise HelicsException(\"[\" + str(err.error_code) + \"] \" + ffi.string(err.message).decode())\n"
                functionStr += "\treturn helics_core(result)\n\n\n"
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
                if arg4.get("spelling","") != "err" or arg4.get("pointer_type", "") != "helics_error_*":
                    raise RuntimeError("the function signature for helicsCreateBrokerFromArgs has changed!")
                functionStr = "def helicsCreateBrokerFromArgs(type: str, name: str, arguments: List[str]) -> helics_broker:\n"
                functionStr += "\t\"\"\"\n"
                functionStr += "\t\tCreate a core object by passing command line arguments.\n\n"
                functionStr += "\t\t@param type The type of the core to create.\n"
                functionStr += "\t\t@param name The name of the core. It can be an empty string to have a name automatically assigned.\n"
                functionStr += "\t\t@param arguments The list of string values from a command line.\n\n"
                functionStr += "\t\t@return a helics_broker object.\n"
                functionStr += "\t\"\"\"\n"
                functionStr += "\tfn = getattr(lib, \"helicsCreateBrokerFromArgs\")\n"
                functionStr += "\targc = len(arguments)\n"
                functionStr += "\targv = ffi.new(f\"char*[{argc}]\")\n"
                functionStr += "\tfor i, s in enumerate(arguments):\n"
                functionStr += "\t\targv[i] = ffi.new(\"char[]\",s.encode())\n"
                functionStr += "\terr = helicsErrorInitialize()\n"
                functionStr += "\tresult = fn(ffi.new(\"char[]\",type.encode()), ffi.new(\"char[]\",name.encode()), argc, argv, err)\n"
                functionStr += "\tif err.error_code != 0:\n"
                functionStr += "\t\traise HelicsException(\"[\" + str(err.error_code) + \"] \" + ffi.string(err.message).decode())\n"
                functionStr += "\treturn helics_broker(result)\n\n\n"
            elif functionDict.get("spelling", "") == "helicsFederateInfoLoadFromArgs":
                #check to see if function signiture changed
                argNum = len(functionDict.get("arguments", {}).keys())
                if argNum != 4:
                    raise RuntimeError("the function signature for helicsFederateInfoLoadFromArgs has changed!")
                arg0 = functionDict.get("arguments", {}).get(0, {})
                if arg0.get("spelling","") != "fi" or arg0.get("type", "") != "helics_federate_info":
                    raise RuntimeError("the function signature for helicsFederateInfoLoadFromArgs has changed!")
                arg1 = functionDict.get("arguments", {}).get(1, {})
                if arg1.get("spelling","") != "argc" or arg1.get("type", "") != "Int":
                    raise RuntimeError("the function signature for helicsFederateInfoLoadFromArgs has changed!")
                arg2 = functionDict.get("arguments", {}).get(2, {})
                if arg2.get("spelling","") != "argv" or arg2.get("double_pointer_type", "") != "Char_S_**":
                    raise RuntimeError("the function signature for helicsFederateInfoLoadFromArgs has changed!")
                arg3 = functionDict.get("arguments", {}).get(3, {})
                if arg3.get("spelling","") != "err" or arg3.get("pointer_type", "") != "helics_error_*":
                    raise RuntimeError("the function signature for helicsFederateInfoLoadFromArgs has changed!")
                functionStr = "def helicsFederateInfoLoadFromArgs(fi: helics_federate_into, arguments: List[str]):\n"
                functionStr += "\t\"\"\"\n"
                functionStr += "\t\tLoad federate info from command line arguments.\n\n"
                functionStr += "\t\t@param fi A federateInfo object.\n"
                functionStr += "\t\t@param arguments A list of strings from the command line.\n"
                functionStr += "\t\"\"\"\n"
                functionStr += "\tfn = getattr(lib, \"helicsFederateInfoLoadFromArgs\")\n"
                functionStr += "\targc = len(arguments)\n"
                functionStr += "\targv = ffi.new(f\"char*[{argc}]\")\n"
                functionStr += "\tfor i, s in enumerate(arguments):\n"
                functionStr += "\t\targv[i] = ffi.new(\"char[]\",s.encode())\n"
                functionStr += "\terr = helicsErrorInitialize()\n"
                functionStr += "\tfn(fi.handle, argc, argv, err)\n"
                functionStr += "\tif err.error_code != 0:\n"
                functionStr += "\t\traise HelicsException(\"[\" + str(err.error_code) + \"] \" + ffi.string(err.message).decode())\n\n\n"
            elif functionDict.get("spelling", "") == "helicsEndpointSendBytes":
                #check to see if function signiture changed
                argNum = len(functionDict.get("arguments", {}).keys())
                if argNum != 4:
                    raise RuntimeError("the function signature for helicsEndpointSendBytes has changed!")
                arg0 = functionDict.get("arguments", {}).get(0, {})
                if arg0.get("spelling","") != "endpoint" or arg0.get("type", "") != "helics_endpoint":
                    raise RuntimeError("the function signature for helicsEndpointSendBytes has changed!")
                arg1 = functionDict.get("arguments", {}).get(1, {})
                if arg1.get("spelling","") != "data" or arg1.get("pointer_type", "") != "Void_*":
                    raise RuntimeError("the function signature for helicsEndpointSendBytes has changed!")
                arg2 = functionDict.get("arguments", {}).get(2, {})
                if arg2.get("spelling","") != "inputDataLength" or arg2.get("type", "") != "Int":
                    raise RuntimeError("the function signature for helicsEndpointSendBytes has changed!")
                arg3 = functionDict.get("arguments", {}).get(3, {})
                if arg3.get("spelling","") != "err" or arg3.get("pointer_type", "") != "helics_error_*":
                    raise RuntimeError("the function signature for helicsEndpointSendBytes has changed!")
                functionStr = "def helicsEndpointSendBytes(endpoint: helics_endpoint, data: bytes):\n"
                functionStr += "\t\"\"\"\n"
                functionStr += "\t\tSend a message to the targeted destinations.\n\n"
                functionStr += "\t\t@param endpoint The endpoint to send the data from.\n"
                functionStr += "\t\t@param data The data to send.\n"
                functionStr += "\t\"\"\"\n"
                functionStr += "\tfn = getattr(lib, \"helicsEndpointSendBytes\")\n"
                functionStr += "\tif isinstance(data, str):\n"
                functionStr += "\t\tdata = data.encode()\n"
                functionStr += "\tif not isinstance(data, bytes):\n"
                functionStr += "\t\traise Exception(\"\"\"data must be of type `bytes`. Got {t} instead. Try converting it to bytes (e.g. `\"hello world\".encode()`\"\"\".format(t=type(data))\n"
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
                if arg0.get("spelling","") != "endpoint" or arg0.get("type", "") != "helics_endpoint":
                    raise RuntimeError("the function signature for helicsEndpointSendBytesAt has changed!")
                arg1 = functionDict.get("arguments", {}).get(1, {})
                if arg1.get("spelling","") != "data" or arg1.get("pointer_type", "") != "Void_*":
                    raise RuntimeError("the function signature for helicsEndpointSendBytesAt has changed!")
                arg2 = functionDict.get("arguments", {}).get(2, {})
                if arg2.get("spelling","") != "inputDataLength" or arg2.get("type", "") != "Int":
                    raise RuntimeError("the function signature for helicsEndpointSendBytesAt has changed!")
                arg3 = functionDict.get("arguments", {}).get(3, {})
                if arg3.get("spelling","") != "time" or arg3.get("type", "") != "helics_time":
                    raise RuntimeError("the function signature for helicsEndpointSendBytesAt has changed!")
                arg4 = functionDict.get("arguments", {}).get(4, {})
                if arg4.get("spelling","") != "err" or arg4.get("pointer_type", "") != "helics_error_*":
                    raise RuntimeError("the function signature for helicsEndpointSendBytesAt has changed!")
                functionStr = "def helicsEndpointSendBytesAt(endpoint: helics_endpoint, data: bytes, time: helics_time):\n"
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
                functionStr += "\t\traise Exception(\"\"\"data must be of type `bytes`. Got {t} instead. Try converting it to bytes (e.g. `\"hello world\".encode()`\"\"\".format(t=type(data))\n"
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
                if arg0.get("spelling","") != "endpoint" or arg0.get("type", "") != "helics_endpoint":
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
                if arg4.get("spelling","") != "err" or arg4.get("pointer_type", "") != "helics_error_*":
                    raise RuntimeError("the function signature for helicsEndpointSendBytesTo has changed!")
                functionStr = "def helicsEndpointSendBytesTo(endpoint: helics_endpoint, data: bytes, dst: str):\n"
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
                functionStr += "\t\traise Exception(\"\"\"data must be of type `bytes`. Got {t} instead. Try converting it to bytes (e.g. `\"hello world\".encode()`\"\"\".format(t=type(data))\n"
                functionStr += "\tinputDataLength = len(data)\n"
                functionStr += "\terr = helicsErrorInitialize()\n"
                functionStr += "\tfn(endpoint.handle, data, inputDataLength, ffi.new(\"Char[]\",dst.encode()), err)\n"
                functionStr += "\tif err.error_code != 0:\n"
                functionStr += "\t\traise HelicsException(\"[\" + str(err.error_code) + \"] \" + ffi.string(err.message).decode())\n\n\n"
            elif functionDict.get("spelling", "") == "helicsEndpointSendBytesToAt":
                #check to see if function signiture changed
                argNum = len(functionDict.get("arguments", {}).keys())
                if argNum != 6:
                    raise RuntimeError("the function signature for helicsEndpointSendBytesToAt has changed!")
                arg0 = functionDict.get("arguments", {}).get(0, {})
                if arg0.get("spelling","") != "endpoint" or arg0.get("type", "") != "helics_endpoint":
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
                if arg4.get("spelling","") != "time" or arg4.get("type", "") != "helics_time":
                    raise RuntimeError("the function signature for helicsEndpointSendBytesToAt has changed!")
                arg5 = functionDict.get("arguments", {}).get(5, {})
                if arg5.get("spelling","") != "err" or arg5.get("pointer_type", "") != "helics_error_*":
                    raise RuntimeError("the function signature for helicsEndpointSendBytesToAt has changed!")
                functionStr = "def helicsEndpointSendBytesToAt(endpoint: helics_endpoint, data: bytes, dst: str, time: helics_time):\n"
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
                functionStr += "\t\traise Exception(\"\"\"data must be of type `bytes`. Got {t} instead. Try converting it to bytes (e.g. `\"hello world\".encode()`\"\"\".format(t=type(data))\n"
                functionStr += "\tinputDataLength = len(data)\n"
                functionStr += "\terr = helicsErrorInitialize()\n"
                functionStr += "\tfn(endpoint.handle, data, inputDataLength, ffi.new(\"Char[]\",dst.encode()), time, err)\n"
                functionStr += "\tif err.error_code != 0:\n"
                functionStr += "\t\traise HelicsException(\"[\" + str(err.error_code) + \"] \" + ffi.string(err.message).decode())\n\n\n"
            elif functionDict.get("spelling", "") == "helicsErrorClear":
                #check to see if function signiture changed
                argNum = len(functionDict.get("arguments", {}).keys())
                if argNum != 1:
                    raise RuntimeError("the function signature for helicsErrorClear has changed!")
                arg0 = functionDict.get("arguments", {}).get(0, {})
                if arg0.get("spelling","") != "err" or arg0.get("pointer_type", "") != "helics_error_*":
                    raise RuntimeError("the function signature for helicsErrorClear has changed!")
                functionStr = "def helicsErrorClear(err):\n"
                functionStr += "\t\"\"\"\n"
                functionStr += "\t\tClear an error object.\n"
                functionStr += "\t\"\"\"\n"
                functionStr += "\tf = loadSym(\"helicsErrorClear\")\n"
                functionStr += "\tf(err)\n\n\n"
            elif functionDict.get("spelling", "") == "helicsFederateRequestTimeIterative":
                #check to see if function signiture changed
                argNum = len(functionDict.get("arguments", {}).keys())
                if argNum != 5:
                    raise RuntimeError("the function signature for helicsFederateRequestTimeIterative has changed!")
                arg0 = functionDict.get("arguments", {}).get(0, {})
                if arg0.get("spelling","") != "fed" or arg0.get("type", "") != "helics_federate":
                    raise RuntimeError("the function signature for helicsFederateRequestTimeIterative has changed!")
                arg1 = functionDict.get("arguments", {}).get(1, {})
                if arg1.get("spelling","") != "requestTime" or arg1.get("type", "") != "helics_time":
                    raise RuntimeError("the function signature for helicsFederateRequestTimeIterative has changed!")
                arg2 = functionDict.get("arguments", {}).get(2, {})
                if arg2.get("spelling","") != "iterate" or arg2.get("type", "") != "helics_iteration_request":
                    raise RuntimeError("the function signature for helicsFederateRequestTimeIterative has changed!")
                arg3 = functionDict.get("arguments", {}).get(3, {})
                if arg3.get("spelling","") != "outIteration" or arg3.get("pointer_type", "") != "helics_iteration_result_*":
                    raise RuntimeError("the function signature for helicsFederateRequestTimeIterative has changed!")
                arg4 = functionDict.get("arguments", {}).get(4, {})
                if arg4.get("spelling","") != "err" or arg4.get("pointer_type", "") != "helics_error_*":
                    raise RuntimeError("the function signature for helicsFederateRequestTimeIterative has changed!")
                functionStr = "def helicsFederateRequestTimeIterative(fed: helics_federate, requestTime: helics_time, iterate: helics_iteration_request) -> Tuple[helics_time, helics_iteration_result]:\n"
                functionStr += "\t\"\"\"\n"
                functionStr += "\t\tRequest an iterative time.\n\n"
                functionStr += "\t\t@details This call allows for finer grain control of the iterative process than /ref helicsFederateRequestTime. It takes a time and and\n"
                functionStr += "\t\titeration request, and returns a time and iteration status.\n\n"
                functionStr += "\t\t@param fed The federate to make the request of.\n"
                functionStr += "\t\t@param requestTime The next desired time.\n"
                functionStr += "\t\t@param iterate The requested iteration mode.\n\n"
                functionStr += "\t\t@return tuple of helics_time and helics_iteration_result.\n"
                functionStr += "\t\"\"\"\n"
                functionStr += "\tfn = getattr(lib, \"helicsFederateRequestTimeIterative\")\n"
                functionStr += "\toutIteration = ffi.new(\"helics_iteration_result *\")\n"
                functionStr += "\terr = helicsErrorInitialize()\n"
                functionStr += "\tresult = fn(fed.handle, requestTime, helics_filter_type(iterate), outIteration, err)\n"
                functionStr += "\tif err.error_code != 0:\n"
                functionStr += "\t\traise HelicsException(\"[\" + str(err.error_code) + \"] \" + ffi.string(err.message).decode())\n"
                functionStr += "\treturn (result, helics_iteration_result(outIteration[0]))\n\n\n"
            elif functionDict.get("spelling", "") == "helicsFederateRequestTimeIterativeComplete":
                #check to see if function signiture changed
                argNum = len(functionDict.get("arguments", {}).keys())
                if argNum != 3:
                    raise RuntimeError("the function signature for helicsFederateRequestTimeIterativeComplete has changed!")
                arg0 = functionDict.get("arguments", {}).get(0, {})
                if arg0.get("spelling","") != "fed" or arg0.get("type", "") != "helics_federate":
                    raise RuntimeError("the function signature for helicsFederateRequestTimeIterativeComplete has changed!")
                arg1 = functionDict.get("arguments", {}).get(1, {})
                if arg1.get("spelling","") != "outIterate" or arg1.get("pointer_type", "") != "helics_iteration_result_*":
                    raise RuntimeError("the function signature for helicsFederateRequestTimeIterativeComplete has changed!")
                arg2 = functionDict.get("arguments", {}).get(2, {})
                if arg2.get("spelling","") != "err" or arg2.get("pointer_type", "") != "helics_error_*":
                    raise RuntimeError("the function signature for helicsFederateRequestTimeIterativeComplete has changed!")
                functionStr = "def helicsFederateRequestTimeIterativeComplete(fed: helics_federate) -> Tuple[helics_time, helics_iteration_result]:\n"
                functionStr += "\t\"\"\"\n"
                functionStr += "\t\tComplete an iterative time request asynchronous call.\n\n"
                functionStr += "\t\t@param fed The federate to make the request of.\n\n"
                functionStr += "\t\t@return tuple of helics_time and helics_iteration_result.\n"
                functionStr += "\t\"\"\"\n"
                functionStr += "\tfn = getattr(lib, \"helicsFederateRequestTimeIterativeComplete\")\n"
                functionStr += "\toutIterate = ffi.new(\"helics_iteration_result *\")\n"
                functionStr += "\terr = helicsErrorInitialize()\n"
                functionStr += "\tresult = fn(fed.handle, outIterate, err)\n"
                functionStr += "\tif err.error_code != 0:\n"
                functionStr += "\t\traise HelicsException(\"[\" + str(err.error_code) + \"] \" + ffi.string(err.message).decode())\n"
                functionStr += "\treturn (result, helics_iteration_result(outIterate[0]))\n\n\n"
            elif functionDict.get("spelling", "") == "helicsInputGetBytes":
                #check to see if function signiture changed
                argNum = len(functionDict.get("arguments", {}).keys())
                if argNum != 5:
                    raise RuntimeError("the function signature for helicsInputGetBytes has changed!")
                arg0 = functionDict.get("arguments", {}).get(0, {})
                if arg0.get("spelling","") != "ipt" or arg0.get("type", "") != "helics_input":
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
                if arg4.get("spelling","") != "err" or arg4.get("pointer_type", "") != "helics_error_*":
                    raise RuntimeError("the function signature for helicsInputGetBytes has changed!")
                functionStr = "def helicsInputGetBytes(ipt: helics_input) -> bytes:\n"
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
                if arg0.get("spelling","") != "ipt" or arg0.get("type", "") != "helics_input":
                    raise RuntimeError("the function signature for helicsInputGetComplex has changed!")
                arg1 = functionDict.get("arguments", {}).get(1, {})
                if arg1.get("spelling","") != "real" or arg1.get("pointer_type", "") != "Double_*":
                    raise RuntimeError("the function signature for helicsInputGetComplex has changed!")
                arg2 = functionDict.get("arguments", {}).get(2, {})
                if arg2.get("spelling","") != "imag" or arg2.get("pointer_type", "") != "Double_*":
                    raise RuntimeError("the function signature for helicsInputGetComplex has changed!")
                arg3 = functionDict.get("arguments", {}).get(3, {})
                if arg3.get("spelling","") != "err" or arg3.get("pointer_type", "") != "helics_error_*":
                    raise RuntimeError("the function signature for helicsInputGetComplex has changed!")
                functionStr = "def helicsInputGetComplex(ipt: helics_input) -> complex:\n"
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
                if arg0.get("spelling","") != "ipt" or arg0.get("type", "") != "helics_input":
                    raise RuntimeError("the function signature for helicsInputGetComplexObject has changed!")
                arg1 = functionDict.get("arguments", {}).get(1, {})
                if arg1.get("spelling","") != "err" or arg1.get("pointer_type", "") != "helics_error_*":
                    raise RuntimeError("the function signature for helicsInputGetComplexObject has changed!")
                functionStr = "def helicsInputGetComplexObject(ipt: helics_input) -> complex:\n"
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
                if arg0.get("spelling","") != "ipt" or arg0.get("type", "") != "helics_input":
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
                if arg5.get("spelling","") != "err" or arg5.get("pointer_type", "") != "helics_error_*":
                    raise RuntimeError("the function signature for helicsInputGetNamedPoint has changed!")
                functionStr = "def helicsInputGetNamedPoint(ipt: helics_input) -> Tuple[str, float]:\n"
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
                functionStr += "\tfn(ipt.handle, data, maxStringLen, actualLength, val, err)\n"
                functionStr += "\tif err.error_code != 0:\n"
                functionStr += "\t\traise HelicsException(\"[\" + str(err.error_code) + \"] \" + ffi.string(err.message).decode())\n"
                functionStr += "\treturn (ffi.string(outputString, maxlen=actualLength[0]).decode(), val[0])\n\n\n"
            elif functionDict.get("spelling", "") == "helicsInputGetString":
                #check to see if function signiture changed
                argNum = len(functionDict.get("arguments", {}).keys())
                if argNum != 5:
                    raise RuntimeError("the function signature for helicsInputGetString has changed!")
                arg0 = functionDict.get("arguments", {}).get(0, {})
                if arg0.get("spelling","") != "ipt" or arg0.get("type", "") != "helics_input":
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
                if arg4.get("spelling","") != "err" or arg4.get("pointer_type", "") != "helics_error_*":
                    raise RuntimeError("the function signature for helicsInputGetString has changed!")
                functionStr = "def helicsInputGetString(ipt: helics_input) -> str:\n"
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
                functionStr += "\treturn ffi.string(data, maxlen=actualSize[0]).decode()\n\n\n"
            elif functionDict.get("spelling", "") == "helicsInputGetVector":
                #check to see if function signiture changed
                argNum = len(functionDict.get("arguments", {}).keys())
                if argNum != 5:
                    raise RuntimeError("the function signature for helicsInputGetVector has changed!")
                arg0 = functionDict.get("arguments", {}).get(0, {})
                if arg0.get("spelling","") != "ipt" or arg0.get("type", "") != "helics_input":
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
                if arg4.get("spelling","") != "err" or arg4.get("pointer_type", "") != "helics_error_*":
                    raise RuntimeError("the function signature for helicsInputGetVector has changed!")
                functionStr = "def helicsInputGetVector(ipt: helics_input) -> List[float]:\n"
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
                if arg0.get("spelling","") != "ipt" or arg0.get("type", "") != "helics_input":
                    raise RuntimeError("the function signature for helicsInputSetDefaultBytes has changed!")
                arg1 = functionDict.get("arguments", {}).get(1, {})
                if arg1.get("spelling","") != "data" or arg1.get("pointer_type", "") != "Void_*":
                    raise RuntimeError("the function signature for helicsInputSetDefaultBytes has changed!")
                arg2 = functionDict.get("arguments", {}).get(2, {})
                if arg2.get("spelling","") != "inputDataLength" or arg2.get("type", "") != "Int":
                    raise RuntimeError("the function signature for helicsInputSetDefaultBytes has changed!")
                arg3 = functionDict.get("arguments", {}).get(3, {})
                if arg3.get("spelling","") != "err" or arg3.get("pointer_type", "") != "helics_error_*":
                    raise RuntimeError("the function signature for helicsInputSetDefaultBytes has changed!")
                functionStr = "def helicsInputSetDefaultBytes(ipt: helics_input, data: bytes):\n"
                functionStr += "\t\"\"\"\n"
                functionStr += "\t\tSet the default as a raw data array.\n\n"
                functionStr += "\t\t@param ipt The input to set the default for.\n"
                functionStr += "\t\t@param data A pointer to the raw data to use for the default.\n"
                functionStr += "\t\"\"\"\n"
                functionStr += "\tfn = getattr(lib, \"helicsInputSetDefaultBytes\")\n"
                functionStr += "\tif isinstance(data, str):\n"
                functionStr += "\t\tdata = data.encode()\n"
                functionStr += "\tif not isinstance(data, bytes):\n"
                functionStr += "\t\traise Exception(\"\"\"data must be of type `bytes`. Got {t} instead. Try converting it to bytes (e.g. `\"hello world\".encode()`\"\"\".format(t=type(data))\n"
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
                if arg0.get("spelling","") != "ipt" or arg0.get("type", "") != "helics_input":
                    raise RuntimeError("the function signature for helicsInputSetDefaultComplex has changed!")
                arg1 = functionDict.get("arguments", {}).get(1, {})
                if arg1.get("spelling","") != "real" or arg1.get("type", "") != "Double":
                    raise RuntimeError("the function signature for helicsInputSetDefaultComplex has changed!")
                arg2 = functionDict.get("arguments", {}).get(2, {})
                if arg2.get("spelling","") != "imag" or arg2.get("type", "") != "Double":
                    raise RuntimeError("the function signature for helicsInputSetDefaultComplex has changed!")
                arg3 = functionDict.get("arguments", {}).get(3, {})
                if arg3.get("spelling","") != "err" or arg3.get("pointer_type", "") != "helics_error_*":
                    raise RuntimeError("the function signature for helicsInputSetDefaultComplex has changed!")
                functionStr = "def helicsInputSetDefaultComplex(ipt: helics_input, value: complex):\n"
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
                if arg0.get("spelling","") != "ipt" or arg0.get("type", "") != "helics_input":
                    raise RuntimeError("the function signature for helicsInputSetDefaultVector has changed!")
                arg1 = functionDict.get("arguments", {}).get(1, {})
                if arg1.get("spelling","") != "vectorInput" or arg1.get("pointer_type", "") != "Double_*":
                    raise RuntimeError("the function signature for helicsInputSetDefaultVector has changed!")
                arg2 = functionDict.get("arguments", {}).get(2, {})
                if arg2.get("spelling","") != "vectorLength" or arg2.get("type", "") != "Int":
                    raise RuntimeError("the function signature for helicsInputSetDefaultVector has changed!")
                arg3 = functionDict.get("arguments", {}).get(3, {})
                if arg3.get("spelling","") != "err" or arg3.get("pointer_type", "") != "helics_error_*":
                    raise RuntimeError("the function signature for helicsInputSetDefaultVector has changed!")
                functionStr = "def helicsInputSetDefaultVector(ipt: helics_input, vectorInput: List[float]):\n"
                functionStr += "\t\"\"\"\n"
                functionStr += "\t\tSet the default as a list of floats.\n\n"
                functionStr += "\t\t@param ipt The input to get the data for.\n"
                functionStr += "\t\t@param vectorInput The default list of floating point values.\n"
                functionStr += "\t\"\"\"\n"
                functionStr += "\tfn = getattr(lib, \"helicsInputSetDefaultVector\")\n"
                functionStr += "\tvectorLength = len(vectorInput)"
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
                if arg0.get("spelling","") != "message" or arg0.get("type", "") != "helics_message":
                    raise RuntimeError("the function signature for helicsMessageAppendData has changed!")
                arg1 = functionDict.get("arguments", {}).get(1, {})
                if arg1.get("spelling","") != "data" or arg1.get("pointer_type", "") != "Void_*":
                    raise RuntimeError("the function signature for helicsMessageAppendData has changed!")
                arg2 = functionDict.get("arguments", {}).get(2, {})
                if arg2.get("spelling","") != "inputDataLength" or arg2.get("type", "") != "Int":
                    raise RuntimeError("the function signature for helicsMessageAppendData has changed!")
                arg3 = functionDict.get("arguments", {}).get(3, {})
                if arg3.get("spelling","") != "err" or arg3.get("pointer_type", "") != "helics_error_*":
                    raise RuntimeError("the function signature for helicsMessageAppendData has changed!")
                functionStr = "def helicsMessageAppendData(message: helics_message, data: bytes):\n"
                functionStr += "\t\"\"\"\n"
                functionStr += "\t\tAppend data to the payload.\n\n"
                functionStr += "\t\t@param message The message object in question.\n"
                functionStr += "\t\t@param data A string containing the message data to append.\n"
                functionStr += "\t\"\"\"\n"
                functionStr += "\tfn = getattr(lib, \"helicsMessageAppendData\")\n"
                functionStr += "\tif isinstance(data, str):\n"
                functionStr += "\t\tdata = data.encode()\n"
                functionStr += "\tif not isinstance(data, bytes):\n"
                functionStr += "\t\traise Exception(\"\"\"data must be of type `bytes`. Got {t} instead. Try converting it to bytes (e.g. `\"hello world\".encode()`\"\"\".format(t=type(data))\n"
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
                if arg0.get("spelling","") != "message" or arg0.get("type", "") != "helics_message":
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
                if arg4.get("spelling","") != "err" or arg4.get("pointer_type", "") != "helics_error_*":
                    raise RuntimeError("the function signature for helicsMessageGetBytes has changed!")
                functionStr = "def helicsMessageGetBytes(message: helics_message) -> bytes:\n"
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
                if arg0.get("spelling","") != "message" or arg0.get("type", "") != "helics_message":
                    raise RuntimeError("the function signature for helicsMessageSetData has changed!")
                arg1 = functionDict.get("arguments", {}).get(1, {})
                if arg1.get("spelling","") != "data" or arg1.get("pointer_type", "") != "Void_*":
                    raise RuntimeError("the function signature for helicsMessageSetData has changed!")
                arg2 = functionDict.get("arguments", {}).get(2, {})
                if arg2.get("spelling","") != "inputDataLength" or arg2.get("type", "") != "Int":
                    raise RuntimeError("the function signature for helicsMessageSetData has changed!")
                arg3 = functionDict.get("arguments", {}).get(3, {})
                if arg3.get("spelling","") != "err" or arg3.get("pointer_type", "") != "helics_error_*":
                    raise RuntimeError("the function signature for helicsMessageSetData has changed!")
                functionStr = "def helicsMessageSetData(message: helics_message, data: bytes):\n"
                functionStr += "\t\"\"\"\n"
                functionStr += "\t\tSet the data payload of a message as raw data.\n\n"
                functionStr += "\t\t@param message The message object in question.\n"
                functionStr += "\t\t@param data A string containing the message data.\n"
                functionStr += "\t\"\"\"\n"
                functionStr += "\tfn = getattr(lib, \"helicsMessageSetData\")\n"
                functionStr += "\tif isinstance(data, str):\n"
                functionStr += "\t\tdata = data.encode()\n"
                functionStr += "\tif not isinstance(data, bytes):\n"
                functionStr += "\t\traise Exception(\"\"\"data must be of type `bytes`. Got {t} instead. Try converting it to bytes (e.g. `\"hello world\".encode()`\"\"\".format(t=type(data))\n"
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
                if arg0.get("spelling","") != "pub" or arg0.get("type", "") != "helics_publication":
                    raise RuntimeError("the function signature for helicsPublicationPublishBytes has changed!")
                arg1 = functionDict.get("arguments", {}).get(1, {})
                if arg1.get("spelling","") != "data" or arg1.get("pointer_type", "") != "Void_*":
                    raise RuntimeError("the function signature for helicsPublicationPublishBytes has changed!")
                arg2 = functionDict.get("arguments", {}).get(2, {})
                if arg2.get("spelling","") != "inputDataLength" or arg2.get("type", "") != "Int":
                    raise RuntimeError("the function signature for helicsPublicationPublishBytes has changed!")
                arg3 = functionDict.get("arguments", {}).get(3, {})
                if arg3.get("spelling","") != "err" or arg3.get("pointer_type", "") != "helics_error_*":
                    raise RuntimeError("the function signature for helicsPublicationPublishBytes has changed!")
                functionStr = "def helicsPublicationPublishBytes(pub: helics_publication, data: bytes):\n"
                functionStr += "\t\"\"\"\n"
                functionStr += "\t\tPublish raw data.\n\n"
                functionStr += "\t\t@param pub The publication to publish for.\n"
                functionStr += "\t\t@param data the raw byte data to publish.\n"
                functionStr += "\t\"\"\"\n"
                functionStr += "\tfn = getattr(lib, \"helicsPublicationPublishBytes\")\n"
                functionStr += "\tif isinstance(data, str):\n"
                functionStr += "\t\tdata = data.encode()\n"
                functionStr += "\tif not isinstance(data, bytes):\n"
                functionStr += "\t\traise Exception(\"\"\"data must be of type `bytes`. Got {t} instead. Try converting it to bytes (e.g. `\"hello world\".encode()`\"\"\".format(t=type(data))\n"
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
                if arg0.get("spelling","") != "pub" or arg0.get("type", "") != "helics_publication":
                    raise RuntimeError("the function signature for helicsPublicationPublishComplex has changed!")
                arg1 = functionDict.get("arguments", {}).get(1, {})
                if arg1.get("spelling","") != "real" or arg1.get("type", "") != "Double":
                    raise RuntimeError("the function signature for helicsPublicationPublishComplex has changed!")
                arg2 = functionDict.get("arguments", {}).get(2, {})
                if arg2.get("spelling","") != "imag" or arg2.get("type", "") != "Double":
                    raise RuntimeError("the function signature for helicsPublicationPublishComplex has changed!")
                arg3 = functionDict.get("arguments", {}).get(3, {})
                if arg3.get("spelling","") != "err" or arg3.get("pointer_type", "") != "helics_error_*":
                    raise RuntimeError("the function signature for helicsPublicationPublishComplex has changed!")
                functionStr = "def helicsPublicationPublishComplex(pub: helics_publication, value: complex):\n"
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
                if arg0.get("spelling","") != "pub" or arg0.get("type", "") != "helics_publication":
                    raise RuntimeError("the function signature for helicsPublicationPublishVector has changed!")
                arg1 = functionDict.get("arguments", {}).get(1, {})
                if arg1.get("spelling","") != "vectorInput" or arg1.get("pointer_type", "") != "Double_*":
                    raise RuntimeError("the function signature for helicsPublicationPublishVector has changed!")
                arg2 = functionDict.get("arguments", {}).get(2, {})
                if arg2.get("spelling","") != "vectorLength" or arg2.get("type", "") != "Int":
                    raise RuntimeError("the function signature for helicsPublicationPublishVector has changed!")
                arg3 = functionDict.get("arguments", {}).get(3, {})
                if arg3.get("spelling","") != "err" or arg3.get("pointer_type", "") != "helics_error_*":
                    raise RuntimeError("the function signature for helicsPublicationPublishVector has changed!")
                functionStr = "def helicsPublicationPublishVector(pub: helics_publication, vectorInput: List[float]):\n"
                functionStr += "\t\"\"\"\n"
                functionStr += "\t\tPublish a vector of doubles.\n\n"
                functionStr += "\t\t@param pub The publication to publish for.\n"
                functionStr += "\t\t@param vectorInput The list of floating point values.\n"
                functionStr += "\t\"\"\"\n"
                functionStr += "\tfn = getattr(lib, \"helicsPublicationPublishVector\")\n"
                functionStr += "\tvectorLength = len(vectorInput)"
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
                if arg0.get("spelling","") != "buffer" or arg0.get("type", "") != "helics_query_buffer":
                    raise RuntimeError("the function signature for helicsQueryBufferFill has changed!")
                arg1 = functionDict.get("arguments", {}).get(1, {})
                if arg1.get("spelling","") != "str" or arg1.get("pointer_type", "") != "Char_S_*":
                    raise RuntimeError("the function signature for helicsQueryBufferFill has changed!")
                arg2 = functionDict.get("arguments", {}).get(2, {})
                if arg2.get("spelling","") != "strSize" or arg2.get("type", "") != "Int":
                    raise RuntimeError("the function signature for helicsQueryBufferFill has changed!")
                arg3 = functionDict.get("arguments", {}).get(3, {})
                if arg3.get("spelling","") != "err" or arg3.get("pointer_type", "") != "helics_error_*":
                    raise RuntimeError("the function signature for helicsQueryBufferFill has changed!")
                functionStr = "def helicsQueryBufferFill(buffer: helics_query_buffer, string: str):\n"
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
        with open("pythonBinding.py","w") as pythonBindingFile:
            pythonBindingFile.write(helicsPythonBindingStr)
        print(helicsPythonBindingStr)

if __name__ == '__main__':
    userInputParser = ArgumentParser()
    userInputParser.add_argument("-p","--create_python_binding", action="store_true",
        default=False, help="Set to true to create the helics python binding.")
    userInputParser.add_argument("headers", default=[], nargs="+",
        help="list of helics header files to parse")
    userArgs = userInputParser.parse_args()
    print(userArgs.headers)
    main(userArgs.headers, userArgs.create_python_binding)
    

