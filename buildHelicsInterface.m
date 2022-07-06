
HelicsVersion='3.2.1';
% set up platform specific names and locations
if ismac
    basePath=['Helics-',HelicsVersion,'-macOS-universal2'];
    baseFile=['Helics-shared-',HelicsVersion,'-macOS-universal2.tar.gz'];
    % download the helics library if needed
    if (~exist(fullfile(basePath,'include/helics/helics.h'),'file'))
        if (~exist('helicsTar.tar.gz','file'))
            websave('helicsTar.tar.gz',['https://github.com/GMLC-TDC/HELICS/releases/download/v',HelicsVersion,'/',baseFile]);
        end
        !tar xf helicsTar.tar.gz
    end
     %actually build the mex file
    mex('-lhelics','-R2018a',['-I./',basePath,'/include/'],['-L./',basePath,'/lib'],'helicsMex.cpp')
elseif isunix
    basePath=['Helics-',HelicsVersion,'-Linux-x86_64'];
    baseFile=['Helics-shared-',HelicsVersion,'-Linux-x86_64.tar.gz'];
    % download the helics library if needed
    if (~exist(fullfile(basePath,'include/helics/helics.h'),'file'))
        if (~exist('helicsTar.tar.gz','file'))
            websave('helicsTar.tar.gz',['https://github.com/GMLC-TDC/HELICS/releases/download/v',HelicsVersion,'/',baseFile]);
        end
        !tar xf helicsTar.tar.gz
    end
     %actually build the mex file
    mex('-lhelics','-R2018a',['-I./',basePath,'/include/'],['-L./',basePath,'/lib'],'helicsMex.cpp')
elseif ispc
    if isequal(computer,'PCWIN64')
        basePath=['Helics-',HelicsVersion,'-win64'];
        baseFile=['Helics-shared-',HelicsVersion,'-win64.tar.gz'];
        targetFile='helics.dll';
    else
        basePath=['Helics-',HelicsVersion,'-win32'];
        baseFile=['Helics-shared-',HelicsVersion,'-win32.tar.gz'];
        targetFile='helics.dll';
    end
    % download the helics library if needed
    if (~exist(fullfile(basePath,'include/helics/helics.h'),'file'))
        if (~exist('helicsTar.tar.gz','file'))
            websave('helicsTar.tar.gz',['https://github.com/GMLC-TDC/HELICS/releases/download/v',HelicsVersion,'/',baseFile]);
        end
        untar('helicsTar.tar.gz');

    end
    %actually build the mex file
    mex('-lhelics','-R2018a',['-I./',basePath,'/include/'],['-L./',basePath,'/lib'],['-L./',basePath,'/bin'],'helicsMex.cpp')
    %copy the needed dll file if on windows
    if ispc
        if (~exist(targetFile,'file'))
            copyfile(fullfile(basePath,'bin',targetFile),'.');
        end
    end
else
    error('Platform not supported');
end






