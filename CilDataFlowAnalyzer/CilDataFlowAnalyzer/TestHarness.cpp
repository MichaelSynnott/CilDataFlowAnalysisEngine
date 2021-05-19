// TestHarness.cpp : Defines the entry point for the application.
//

#include "TestHarness.h"

#include "CilDataFlowAnalyzer.h"
#include "TestInput/ITestInput.h"
#include "TestInput/Test_FillAttributeInfo.h"

using namespace std;

int main()
{

	ITestInput* testInput = new Test_FillAttributeInfo();
	
	CilDataFlowAnalyzer analyzer{ testInput->GetTestBytes(), testInput->GetTestLength() };
	analyzer.Init();
	
	auto x = analyzer.GetStackStatusAtOffset(0x40);
	
}
