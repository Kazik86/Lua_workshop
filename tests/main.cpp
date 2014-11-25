#include <UnitTest++/UnitTest++.h>
#include <UnitTest++/TestReporterStdout.h>
#include <cstring>

/* This 'main' is slightly modified in comparison with the original one. My
 * version allows you to run _single_ test or _single_ suite via command
 * line. It is written based on answer from 'stijn' with patch from 'Geoffrey T
 * Falk'. Both can be found on StackOverflow:
 * http://stackoverflow.com/questions/3546054/how-do-i-run-a-single-test-with-unittest
 * invoke with arguments to run a single test:
 *   > myexe MyTestName
 * or single suite:
 *   > myexe suite MySuite
 */
int main( int argc, char** argv )
{
    using UnitTest::TestList;
    using UnitTest::Test;
    using UnitTest::TestReporterStdout;
    using UnitTest::TestRunner;

    if( argc > 1 )
    {
	//if first arg is "suite", we search for suite names instead of test names
	const bool bSuite = strcmp( "suite", argv[ 1 ] ) == 0;

	//walk list of all tests, add those with a name that
	//matches one of the arguments  to a new TestList
	const TestList& allTests( Test::GetTestList() );
	TestList selectedTests;
	Test* p = allTests.GetHead();
	while( p )
	{
	    for( int i = 1 ; i < argc ; ++i )
		if( strcmp( bSuite ? p->m_details.suiteName
			    : p->m_details.testName, argv[ i ] ) == 0 )
		    selectedTests.Add( p );
	    Test* q = p;
	    p = p->m_nextTest;
	    q->m_nextTest = 0;
	}

	//run selected test(s) only
	TestReporterStdout reporter;
	TestRunner runner( reporter );
	return runner.RunTestsIf( selectedTests, 0, UnitTest::True(), 0 );
    }
    else
    {
	return UnitTest::RunAllTests();
    }
}
