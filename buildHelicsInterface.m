
HelicsVersion='3.2.0';
if ismac
    basePath=['Helics-',HelicsVersion','-macOS-universal2'];
    baseFile=['Helics-shared-',HelicsVersion','-macOS-universal2.tar.gz'];
    targetFile='helics.dll';
elseif isunix
    basePath=['Helics-',HelicsVersion','-Linux-x86_64'];
    baseFile=['Helics-shared-',HelicsVersion','Linux-x86_64.tar.gz'];
    targetFile='helics.dll';
elseif ispc
    if isequal(computer,'PCWIN64')
        basePath=['Helics-',HelicsVersion','-win64'];
        baseFile=['Helics-shared-',HelicsVersion','-win64.tar.gz'];
        targetFile='helics.dll';
    else
        basePath=['Helics-',HelicsVersion','-win32'];
        baseFile=['Helics-shared-',HelicsVersion','-win32.tar.gz'];
        targetFile='helics.dll';
    end
else
    error('Platform not supported');
end



if (~exist(fullfile(basePath,'include/helics/helics.h'),'file'))
    if (~exist('helicsTar.tar.gz','file'))
        websave('helicsTar.tar.gz',['https://github.com/GMLC-TDC/HELICS/releases/download/v',HelicsVersion,'/',baseFile]);
    end
    untar('helicsTar.tar.gz');
end
mex('-lhelics','-R2018a',['-I./',basePath,'/include/'],['-L./',basePath,'/lib'],['-L./',basePath,'/bin'],'helicsMex.cpp')
copyfile(fullfile(basePath,'bin',targetFile),'.');

