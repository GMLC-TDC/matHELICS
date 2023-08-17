function buildOctaveHelicsInterface(targetPath,makePackage)
% buildHelicsInterface(targetPath,makePackage) will generate the files
% necessary for the Octave HELICS interface.  It will download additional
% files from github if needed.
% buildHelicsInterface() will generate the package files in the current
% directory
% buildHelicsInterface(targetPath) will create the package files in the
% specified targetPath directory
% buildHelicsInterface("") is equivalent to buildHelicsInterface()
% buildHelicsInterface(targetPath,makePackage) will if makePackage is set
% to true generate a zip/tar.gz file with all the files that can be copied
% and extracted on a similar system.
%
% To make the helics library accessible anywhere on the octave path the
% targetPath folder should be added to the octave path.  (NOTE: it should
% not be added with subfolders as all required matlab code is in the main
% folder or in the +helics folder which matlab will recognize as a
% package).
%
% this file requires octave 8.2 or higher.
if (nargin==0)
    targetPath=fileparts(mfilename("fullpath"));
end
if (nargin<2)
    makePackage=false;
end
if (isempty(targetPath))
    targetPath=fileparts(mfilename("fullpath"));
end
if (~exist(targetPath,"dir"))
    mkdir(targetPath);
end
if (makePackage)
    if (isequal(targetPath,fileparts(mfilename("fullpath"))))
        warning("cannot makePackage if target location is the same as the file origin");
        makePackage=false;
    end
end
inputPath=fileparts(mfilename("fullpath"));
HelicsVersion="3.4.0";
% set up platform specific names and locations
targetTarFile=fullfile(targetPath,["helicsTar",HelicsVersion,".tar.gz"]);
if ismac
    basePath=fullfile(targetPath,["Helics-",HelicsVersion,"-macOS-universal2"]);
    baseFile=["Helics-shared-",HelicsVersion,"-macOS-universal2.tar.gz"];
    targetFile=fullfile(basePath,"lib","libhelics.dylib");
    % download the helics library if needed
    if (~exist(fullfile(basePath,"include/helics/helics.h"),"file"))
        if (~exist(targetTarFile,"file"))
            fprintf("downloading helics binary package\n")
            urlwrite(["https://github.com/GMLC-TDC/HELICS/releases/download/v",HelicsVersion,"/",baseFile],targetTarFile);
        end
        fprintf("extracting helics binary package\n")
        system(["tar xf ",targetTarFile," -C ",targetPath]);
    end
    %actually build the mex file
    fprintf("building helics mex target\n")
    mex("-lhelics","-DMX_HAS_INTERLEAVED_COMPLEX",["-I",basePath,"/include/"],["-L",basePath,"/lib"],["-Wl,-rpath,$ORIGIN/lib:",basePath,"/lib",":",basePath,"/lib64"],"-o helicsMex.mex",fullfile(inputPath,"helicsMex.cpp"))
elseif isunix
    basePath=fullfile(targetPath,["Helics-",HelicsVersion,"-Linux-x86_64"]);
    baseFile=["Helics-shared-",HelicsVersion,"-Linux-x86_64.tar.gz"];
    targetFile=fullfile(basePath,"lib64","libhelics.so");
    % download the helics library if needed
    if (~exist(fullfile(basePath,"include/helics/helics.h"),"file"))
        if (~exist(targetTarFile,"file"))
            fprintf("downloading helics binary package\n")
            urlwrite(["https://github.com/GMLC-TDC/HELICS/releases/download/v",HelicsVersion,"/",baseFile],targetTarFile);
        end
        fprintf("extracting helics binary package\n")
        system(["tar xf ",targetTarFile," -C ",targetPath]);
    end
    %actually build the mex file
    fprintf("building helics mex target\n")
    mex("-lhelics","-DMX_HAS_INTERLEAVED_COMPLEX",["-I",basePath,"/include/"],["-L",basePath,"/lib"],["-L",basePath,"/lib64"],["-Wl,-rpath,$ORIGIN/lib:",basePath,"/lib:",basePath,"/lib64"],"-o helicsMex.mex",fullfile(inputPath,"helicsMex.cpp"))
elseif ispc
    if isequal(computer,"PCWIN64")
        basePath=fullfile(targetPath,["Helics-",HelicsVersion,"-win64"]);
        baseFile=["Helics-shared-",HelicsVersion,"-win64.tar.gz"];
        targetFile="helics.dll";
    else
        basePath=fullfile(targetPath,["Helics-",HelicsVersion,"-win32"]);
        baseFile=["Helics-shared-",HelicsVersion,"-win32.tar.gz"];
        targetFile="helics.dll";
    end
    % download the helics library if needed
    if (~exist(fullfile(basePath,"include/helics/helics.h"),"file"))
        if (~exist(targetTarFile,"file"))
            fprintf("downloading helics binary package\n")
            urlwrite(["https://github.com/GMLC-TDC/HELICS/releases/download/v",HelicsVersion,"/",baseFile],targetTarFile);
        end
        fprintf("extracting helics binary package\n")
        untar(targetTarFile,targetPath);

    end
    %actually build the mex file
    fprintf("building helics mex target\n")
    mex("-lhelics","-DMX_HAS_INTERLEAVED_COMPLEX",["-I",basePath,"/include/"],["-L",basePath,"/lib"],["-L",basePath,"/bin"],"-o helicsMex.mex",fullfile(inputPath,"helicsMex.cpp"))
    %copy the needed dll file if on windows
    if ispc
        if (~exist(fullfile(targetPath,targetFile),"file"))
            copyfile(fullfile(basePath,"bin",targetFile),fullfile(targetPath,targetFile));
            copyfile(fullfile(basePath,"bin","*.dll"),targetPath);
        end
    end
else
    error("Platform not supported");
end

%% now build the interface directory and copy files
fprintf("copying required files\n")
copyfile(fullfile(inputPath,"matlabBindings","+helics"),fullfile(targetPath,"+helics"));
copyfile(fullfile(inputPath,"extra_m_codes/*"),fullfile(targetPath,"+helics"));

if (makePackage)
    fprintf("generating helics matlab binary package file\n")
    rmdir(basePath,"s");
    delete(targetTarFile);
    if ismac || isunix
        system(["tar czf matHELICS.tar.gz ",fullfile(targetPath,"*")]);
    elseif ispc
        zip("matHELICS","*",targetPath);
    else
        warning("unrecognized platform for making package");
    end
end



