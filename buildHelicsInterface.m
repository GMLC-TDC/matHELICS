function buildHelicsInterface(targetPath,makePackage)
% buildHelicsInterface(targetPath,makePackage) will generate the files
% necessary for the Matlab HELICS interface.  It will download additional
% files from github if needed.  
% buildHelicsInterface() will generate the package files in the current
% directory
% buildHelicsInterface(targetPath) will create the package files in the
% specified targetPath directory
% buildHelicsInterface('') is equivalent to buildHelicsInterface()
% buildHelicsInterface(targetPath,makePackage) will if makePackage is set
% to true generate a zip/tar.gz file with all the files that can be copied
% and extracted on a similar system.  
%
% To make the helics library accessible anywhere on the matlab path the
% targetPath folder should be added to the matlab path.  (NOTE: it should
% not be added with subfolders as all required matlab code is in the main
% folder or in the +helics folder which matlab will recognize as a
% package).
% 
% this file requires matlab 2018a or higher.  
if (nargin==0)
    targetPath=fileparts(mfilename('fullpath'));
end
if (nargin<2)
    makePackage=false;
end
if (isempty(targetPath))
    targetPath=fileparts(mfilename('fullpath'));
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
HelicsVersion='3.5.1';
% set up platform specific names and locations
targetTarFile=fullfile(targetPath,['helicsTar',HelicsVersion,'.tar.gz']);
if ismac
    basePath=fullfile(targetPath,['Helics-',HelicsVersion,'-macOS-universal2']);
    baseFile=['Helics-shared-',HelicsVersion,'-macOS-universal2.tar.gz'];
    targetFile=fullfile(basePath,'lib','libhelics.dylib');
    % download the helics library if needed
    if (~exist(fullfile(basePath,'include/helics/helics.h'),'file'))
        if (~exist(targetTarFile,'file'))
            fprintf('downloading helics binary package\n')
            websave(targetTarFile,['https://github.com/GMLC-TDC/HELICS/releases/download/v',HelicsVersion,'/',baseFile]);
        end
        fprintf('extracting helics binary package\n')
        system(['tar xf ',targetTarFile,' -C ',targetPath]);
    end
    %actually build the mex file
    fprintf('building helics mex target\n')
    mex('-lhelics','-R2018a',['-I',basePath,'/include/'],['-L',basePath,'/lib'],['LDFLAGS=$LDFLAGS -Wl,-rpath,$ORIGIN/lib,-rpath,',basePath,'/lib',',-rpath,',basePath,'/lib64'],fullfile(inputPath,'helicsMex.cpp'),'-outdir',targetPath)
elseif isunix
    basePath=fullfile(targetPath,['Helics-',HelicsVersion,'-Linux-x86_64']);
    baseFile=['Helics-shared-',HelicsVersion,'-Linux-x86_64.tar.gz'];
    targetFile=fullfile(basePath,'lib64','libhelics.so');
    % download the helics library if needed
    if (~exist(fullfile(basePath,'include/helics/helics.h'),'file'))
        if (~exist(targetTarFile,'file'))
            fprintf('downloading helics binary package\n')
            websave(targetTarFile,['https://github.com/GMLC-TDC/HELICS/releases/download/v',HelicsVersion,'/',baseFile]);
        end
        fprintf('extracting helics binary package\n')
        system(['tar xf ',targetTarFile,' -C ',targetPath]);
    end
    %actually build the mex file
    fprintf('building helics mex target\n')
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
            fprintf('downloading helics binary package\n')
            websave(targetTarFile,['https://github.com/GMLC-TDC/HELICS/releases/download/v',HelicsVersion,'/',baseFile]);
        end
        fprintf('extracting helics binary package\n')
        untar(targetTarFile,targetPath);

    end
    %actually build the mex file
    fprintf('building helics mex target\n')
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
fprintf('copying required files\n')
copyfile(fullfile(inputPath,'matlabBindings','+helics'),fullfile(targetPath,'+helics'));
copyfile(fullfile(inputPath,'extra_m_codes'),fullfile(targetPath,'+helics'));
% copy the include directory with the C headers
if (~exist(fullfile(targetPath,'include'),'dir'))
    mkdir(fullfile(targetPath,'include'));
end
copyfile(fullfile(inputPath,'helics_minimal.h'),fullfile(targetPath,'include','helics_minimal.h'));
if (ismac)
    [status, result]=system(['cp -R ',fullfile(basePath,'lib'),' ',fullfile(targetPath,'lib')]);
    if (status~=0)
        disp(result);
    end
elseif (isunix)
    [status, result]=system(['cp -R ',fullfile(basePath,'lib64'),' ',fullfile(targetPath,'lib')]);
     if (status~=0)
        disp(result);
    end
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
    fprintf(fid,'\theaderName=fullfile(cpath,''include'',''helics_minimal.h'');\n');
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
    fprintf('generating helics matlab binary package file\n')
    rmdir(basePath,'s');
    delete(targetTarFile);
    if ismac || isunix
        system(['tar czf matHELICS.tar.gz ',fullfile(targetPath,'*')]);
    elseif ispc
        zip('matHELICS','*',targetPath);
    else
        warning('unrecognized platform for making package');
    end
end



