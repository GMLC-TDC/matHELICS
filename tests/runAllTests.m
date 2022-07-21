helicsStartup
results=runtests('valueFedTests.m');
assertSuccess(results);
results=runtests('messageFedTests.m');
assertSuccess(results);
results=runtests('messageFilterTests.m');
assertSuccess(results);
