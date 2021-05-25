# CMakeList.txt : CMake project for CilDataFlowAnalyzer, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
add_executable (CilDataFlowAnalyzer   "TestHarness.cpp" "TestHarness.h"   "CilParser.h" "PlatformHeaders.h" "CustomTypes.h" "CilBasicBlockParser.h" "CilBasicBlockParser.cpp" "TestInput/ITestInput.h" "TestInput/Test_FillAttributeInfo.h" "CilParser.cpp" "opdef.h" "BasicBlock.h" "SwitchOperation.h" "OpCodeHelper.h" "OpCodeHelper.cpp" "BasicBlockTree.h"  "BasicBlockTree.cpp"   "ExceptionHandlingClause.h" "InsertedBytes.h" "InsertedBytes.cpp"  "TestInput/Test_DirectoryExists.h" "CilDataFlowAnalyzer.h" "CilDataFlowAnalyzer.cpp"  "BasicBlockRoute.h" "BasicBlockRoute.cpp")

# TODO: Add tests and install targets if needed.
