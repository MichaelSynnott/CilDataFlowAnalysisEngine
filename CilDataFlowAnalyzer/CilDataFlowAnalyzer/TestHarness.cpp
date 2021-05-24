// TestHarness.cpp : Defines the entry point for the application.
//

#include "TestHarness.h"

#include "CilDataFlowAnalyzer.h"
#include "TestInput/ITestInput.h"
#include "TestInput/Test_DirectoryExists.h"

using namespace std;

int main()
{

	ITestInput* testInput = new Test_DirectoryExists();
	
	CilDataFlowAnalyzer analyzer{ testInput->GetTestBytes()};
	analyzer.Init();
	
	auto x = analyzer.GetStackStatusAtOffset(0x27);
	int j = 0;
	
}
