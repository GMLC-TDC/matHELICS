helicsStartup
results=runtests('messageFilterTests.m');
assertSuccess(results);
results=runtests('messageFedTests.m');
assertSuccess(results);
results=runtests('valueFedTests.m');
assertSuccess(results);
