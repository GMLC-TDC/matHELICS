
function buildHelicsInterface(targetPath)
if (nargin==0)
    targetPath=fileparts(mfilename('fullpath'));
end
if (~exist(targetPath,'dir'))
    makedir(targetPath);
end
inputPath=fileparts(mfilename('fullpath'));
HelicsVersion='3.2.1';
% set up platform specific names and locations
targetTarFile=fullfile(targetPath,['helicsTar',HelicsVersion,'.tar.gz']);
if ismac
    basePath=fullfile(targetPath,['Helics-',HelicsVersion,'-macOS-universal2']);
    baseFile=['Helics-shared-',HelicsVersion,'-macOS-universal2.tar.gz'];
    % download the helics library if needed
    if (~exist(fullfile(basePath,'include/helics/helics.h'),'file'))
        if (~exist(targetTarFile,'file'))
            websave(targetTarFile,['https://github.com/GMLC-TDC/HELICS/releases/download/v',HelicsVersion,'/',baseFile]);
        end
        system(['tar xf ',targetTarFile,' -C ',targetPath]);
    end
     %actually build the mex file
    mex('-lhelics','-R2018a',['-I',basePath,'/include/'],['-L',basePath,'/lib'],['-Wl,-rpath,',basePath,'/lib'],fullfile(inputPath,'helicsMex.cpp'),'-outdir',targetPath)
elseif isunix
    basePath=fullfile(targetPath,['Helics-',HelicsVersion,'-Linux-x86_64']);
    baseFile=['Helics-shared-',HelicsVersion,'-Linux-x86_64.tar.gz'];
    % download the helics library if needed
    if (~exist(fullfile(basePath,'include/helics/helics.h'),'file'))
        if (~exist(targetTarFile,'file'))
            websave(targetTarFile,['https://github.com/GMLC-TDC/HELICS/releases/download/v',HelicsVersion,'/',baseFile]);
        end
        system(['tar xf ',targetTarFile,' -C ',targetPath]);
    end
     %actually build the mex file
    mex('-lhelics','-R2018a',['-I',basePath,'/include/'],['-L',basePath,'/lib'],['-Wl,-rpath,',basePath,'/lib'],fullfile(inputPath,'helicsMex.cpp'),'-outdir',targetPath)
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
        end
    end
else
    error('Platform not supported');
end

%% now build the interface directory
copyfile(fullfile(inputPath,'matlabBindings','+helics'),fullfile(targetPath,'+helics'));
copyfile(fullfile(inputPath,'extra_m_codes'),fullfile(targetPath,'+helics'));





