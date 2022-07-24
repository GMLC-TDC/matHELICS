
function buildHelicsInterface(targetPath,makePackage)
if (nargin==0)
    targetPath=fileparts(mfilename('fullpath'));
end
if (nargin<2)
    makePackage=false;
end
if (~exist(targetPath,'dir'))
    mkdir(targetPath);
end
if (makePackage)
    if (isequal(targetPath,fileparts(mfilename('fullpath'))))
        warning('cannot makePackage if target location is the same as the file origin');
        makePackage=false;
    end
end
inputPath=fileparts(mfilename('fullpath'));
HelicsVersion='3.2.1';
% set up platform specific names and locations
targetTarFile=fullfile(targetPath,['helicsTar',HelicsVersion,'.tar.gz']);
if ismac
    basePath=fullfile(targetPath,['Helics-',HelicsVersion,'-macOS-universal2']);
    baseFile=['Helics-shared-',HelicsVersion,'-macOS-universal2.tar.gz'];
    targetFile=fullfile(basePath,'lib','libhelics.dylib');
    % download the helics library if needed
    if (~exist(fullfile(basePath,'include/helics/helics.h'),'file'))
        if (~exist(targetTarFile,'file'))
            websave(targetTarFile,['https://github.com/GMLC-TDC/HELICS/releases/download/v',HelicsVersion,'/',baseFile]);
        end
        system(['tar xf ',targetTarFile,' -C ',targetPath]);
    end
    %actually build the mex file
    mex('-lhelics','-R2018a',['-I',basePath,'/include/'],['-L',basePath,'/lib'],['LDFLAGS=$LDFLAGS -Wl,-rpath,$ORIGIN/lib,-rpath,',basePath,'/lib',',-rpath,',basePath,'/lib64'],fullfile(inputPath,'helicsMex.cpp'),'-outdir',targetPath)
elseif isunix
    basePath=fullfile(targetPath,['Helics-',HelicsVersion,'-Linux-x86_64']);
    baseFile=['Helics-shared-',HelicsVersion,'-Linux-x86_64.tar.gz'];
    targetFile=fullfile(basePath,'lib64','libhelics.so');
    % download the helics library if needed
    if (~exist(fullfile(basePath,'include/helics/helics.h'),'file'))
        if (~exist(targetTarFile,'file'))
            websave(targetTarFile,['https://github.com/GMLC-TDC/HELICS/releases/download/v',HelicsVersion,'/',baseFile]);
        end
        system(['tar xf ',targetTarFile,' -C ',targetPath]);
    end
    %actually build the mex file
    mex('-lhelics','-R2018a',['-I',basePath,'/include/'],['-L',basePath,'/lib'],['-L',basePath,'/lib64'],['LDFLAGS=$LDFLAGS -Wl,-rpath,$ORIGIN/lib,-rpath,',basePath,'/lib',',-rpath,',basePath,'/lib64'],fullfile(inputPath,'helicsMex.cpp'),'-outdir',targetPath)
elseif ispc
    if isequal(computer,'PCWIN64')
        basePath=fullfile(targetPath,['Helics-',HelicsVersion,'-win64']);
        baseFile=['Helics-shared-',HelicsVersion,'-win64.tar.gz'];
        targetFile='helics.dll';
    else
        basePath=fullfile(targetPath,['Helics-',HelicsVersion,'-win32']);
        baseFile=['Helics-shared-',HelicsVersion,'-win32.tar.gz'];
        targetFile='helics.dll';
    end
    % download the helics library if needed
    if (~exist(fullfile(basePath,'include/helics/helics.h'),'file'))
        if (~exist(targetTarFile,'file'))
            websave(targetTarFile,['https://github.com/GMLC-TDC/HELICS/releases/download/v',HelicsVersion,'/',baseFile]);
        end
        untar(targetTarFile,targetPath);

    end
    %actually build the mex file
    mex('-lhelics','-R2018a',['-I',basePath,'/include/'],['-L',basePath,'/lib'],['-L',basePath,'/bin'],fullfile(inputPath,'helicsMex.cpp'),'-outdir',targetPath)
    %copy the needed dll file if on windows
    if ispc
        if (~exist(fullfile(targetPath,targetFile),'file'))
            copyfile(fullfile(basePath,'bin',targetFile),fullfile(targetPath,targetFile));
            copyfile(fullfile(basePath,'bin','*.dll'),targetPath);
        end
    end
else
    error('Platform not supported');
end

%% now build the interface directory and copy files
copyfile(fullfile(inputPath,'matlabBindings','+helics'),fullfile(targetPath,'+helics'));
copyfile(fullfile(inputPath,'extra_m_codes'),fullfile(targetPath,'+helics'));
% copy the include directory with the C headers
mkdir(fullfile(targetPath,'include'));
copyfile(fullfile(inputPath,'helics_minimal.h'),fullfile(targetPath,'include','helics_minimal.h'));
if (ismac)
    [status, result]=system(['cp -R ',fullfile(basePath,'lib'),' ',fullfile(targetPath,'lib')])
elseif (isunix)
    [status, result]=system(['cp -R ',fullfile(basePath,'lib64'),' ',fullfile(targetPath,'lib')])
else
    copyfile(fullfile(basePath,'bin'),fullfile(targetPath,'bin'));
end

%% generate a startup script to load the library

    fid=fopen(fullfile(targetPath,'helicsStartup.m'),'w');
    fprintf(fid,'function helicsStartup(libraryName,headerName)\n');
    fprintf(fid,'%% function to load the helics library prior to execution\n');
    fprintf(fid,'if (nargin==0)\n');
    fprintf(fid,'\tcpath=fileparts(mfilename(''fullpath''));\n');
    if (ispc)
        fprintf(fid,'\tlibraryName=''%s'';\n',targetFile);
    else
       [~,tname,ext]=fileparts(targetFile);
       fprintf(fid,'\tlibraryName=fullfile(cpath,''lib'',''%s%s'');\n',tname,ext);
    end
    fprintf(fid,'end\n\n');
    fprintf(fid,'');

    fprintf(fid,'if (nargin<2)\n');
    fprintf(fid,'\theaderName=fullfile(cpath,''include'',''helics_minimal.h'');');
    fprintf(fid,'end\n\n');
    fprintf(fid,'');

    fprintf(fid,'if (~isempty(libraryName))\n');
    fprintf(fid,'\tif ~libisloaded(''libHelics'')\n');
    fprintf(fid,'\t\tloadlibrary(libraryName,headerName);\n');
    fprintf(fid,'\tend\n');
    fprintf(fid,'else\n');
    fprintf(fid,'\tdisp(''Unable to find library for HELICS'')\n');
    fprintf(fid,'end\n');
    fclose(fid);

if (makePackage)
    rmdir(basePath,'s');
    delete(targetTarFile);
    if ismac || isunix
        system(['tar czvf matHELICS.tar.gz ',fullfile(targetPath,'*')]);
    elseif ispc
        zip('matHELICS','*',targetPath);
    else
        warning('unrecognized platform for making package');
    end
end



