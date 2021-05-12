#pragma once
#include "cor.h"

class SwitchOperation
{
public:
	SwitchOperation(int numberOfTargets, DWORD* pTargets)
	{
		NumberOfTargets = numberOfTargets;
		Targets = new DWORD[numberOfTargets];
		for(int i = 0; i < numberOfTargets;i++)
		{
			Targets[i] = pTargets[i];
		}
	}

	~SwitchOperation()
	{
		delete[] Targets;
	}
	
	int NumberOfTargets;
	DWORD* Targets;
};