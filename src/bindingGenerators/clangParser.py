'''
Copyright (c) 2017-2022,
Battelle Memorial Institute; Lawrence Livermore National Security, LLC; Alliance for Sustainable Energy, LLC.  See
the top-level NOTICE for additional details. All rights reserved.
SPDX-License-Identifier: BSD-3-Clause
'''
import json
import logging
import os
from typing import List

import clang.cindex as cidx


clangLogger = logging.getLogger(__name__)
clangLogger.setLevel(logging.DEBUG)
logFormatter = logging.Formatter('%(asctime)s - %(name)s - %(levelname)s - %(message)s')
clangLogFileHandler = logging.FileHandler('clangParserLog.log',mode='w',encoding='utf-8')
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
        ignoredMacros = ["HELICS_C_API_H_", "HELICS_EXPORT", "HELICS_DEPRECATED"]
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