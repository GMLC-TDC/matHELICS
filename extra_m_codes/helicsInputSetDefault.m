function  helicsInputSetDefault(varargin)
%set the default value for a HELICS input
%helicsInputSetDefault(inp,value)
%inp is the input object
% value is the data value to set as the default can be a char,integer, double, vector, logical, or string. 

    pubdata=varargin{2};
    import helics.*
   switch (class(pubdata))
       case {'char'}
           if (length(pubdata)==1)
               helicsInputSetDefaultChar(varargin{:});
           else
               helicsInputSetDefaultString(varargin{:});
           end
       case {'int8'}
           helicsInputSetDefaultChar(varargin{:});
       case {'double'}
           if (isreal(pubdata))
               if (numel(pubdata)>1)
                   helicsInputSetDefaultVector(varargin{:});
               else
                   helicsInputSetDefaultDouble(varargin{:});
               end
           else
               if (numel(pubdata)>1)
                   helicsInputSetDefaultVector(varargin{:});
               else
                   helicsInputSetDefaultComplex(varargin{:});
               end
           end
       case {'single'}
           if (isreal(pubdata))
                if (numel(pubdata)>1)
                   helicsInputSetDefaultVector(varargin{1},double(pubdata));
               else
                   helicsInputSetDefaultDouble(varargin{1}, double(pubdata));
               end
           else
               if (numel(pubdata)>1)
                   helicsInputSetDefaultVector(varargin{:});
               else
                   helicsInputSetDefaultComplex(varargin{1},double(pubdata));
               end
           end
       case {'int32','uint64','uint32','int16','uint16'}
           helicsInputSetDefaultInteger(varargin{1},int64(pubdata));
       case {'int64'}
           helicsInputSetDefaultInteger(varargin{:});
       case {'logical'}
               helicsInputSetDefaultBoolean(varargin{1},pubdata);
   end
end
