function  helicsPublicationPublish(varargin)
%publish a value through HELICS
%helicsPublicationPublish(pub,value)
%pub is the publication object
% value is the data value to set as the default can be a char,integer, double, vector, logical, or string. 
    pubdata=varargin{2};
    import helics.*
   switch (class(pubdata))
       case {'char'}
           if (length(pubdata)==1)
               helicsPublicationPublishChar(varargin{:});
           else
               helicsPublicationPublishString(varargin{:});
           end
       case {'int8'}
           helicsPublicationPublishChar(varargin{:});
       case {'double'}
           if (isreal(pubdata))
               if (numel(pubdata)>1)
                   helicsPublicationPublishVector(varargin{:});
               else
                   helicsPublicationPublishDouble(varargin{:});
               end
           else
               if (numel(pubdata)>1)
                   helicsPublicationPublishVector(varargin{:});
               else
                   helicsPublicationPublishComplex(varargin{:});
               end
           end
       case {'single'}
           if (isreal(pubdata))
                if (numel(pubdata)>1)
                   helicsPublicationPublishVector(varargin{1},double(pubdata));
               else
                   helicsPublicationPublishDouble(varargin{1}, double(pubdata));
               end
           else
               if (numel(pubdata)>1)
                   helicsPublicationPublishVector(varargin{:});
               else
                   helicsPublicationPublishComplex(varargin{1},double(pubdata));
               end
           end
       case {'int32','uint64','uint32','int16','uint16'}
           helicsPublicationPublishInteger(varargin{1},int64(pubdata));
       case {'int64'}
           helicsPublicationPublishInteger(varargin{:});
       case {'logical'}
               helicsPublicationPublishBoolean(varargin{1},pubdata);
   end
end
