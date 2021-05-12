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
	
	CilDataFlowAnalyzer engine{ testInput->GetTestBytes(), testInput->GetTestLength() };
	engine.ScanForBasicBlocks();
	
}
