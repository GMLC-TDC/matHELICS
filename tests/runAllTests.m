% The mex interface (helicsMex) does not need helicsStartup. Only call the
% loadlibrary-based startup when the mex file is not available.
if ~exist('helicsMex','file')
    helicsStartup
end
results=runtests('messageFilterTests.m');
assertSuccess(results);
results=runtests('messageFedTests.m');
assertSuccess(results);
results=runtests('valueFedTests.m');
assertSuccess(results);
