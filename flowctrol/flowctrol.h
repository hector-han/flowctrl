#pragma once
#include<set>

typedef unsigned int uint;

bool inLCRORegionMode(uint target, uint left, uint right, uint mod)
{
	//std::cout << target << " " << left << " " << right << " " << mod << std::endl;
	if (target < left)
	{
		uint diff = left - target;
		uint tmp = diff % mod == 0 ? diff / mod : diff / mod + 1;
		target += mod * tmp;
	}
	if (left <= target && target < right)
	{
		return true;
	}
	else
	{
		return false;
	}
}
class moduleManager
{
public:
	static moduleManager& getInstance()
	{
		return m_instance;
	}

	bool getAllModule(std::set<uint>&);
private:
	moduleManager() {}
	moduleManager(const moduleManager&) {}
	moduleManager& operator=(const moduleManager&) {}
	static moduleManager m_instance;
};

class flowCtrlCounter
{
public:
	flowCtrlCounter(uint totalTPS, uint timesPerSec, uint moduleNumber);
	uint getSndTimesThisLoop();

private:
	void calVariables();
	uint m_totalTPS;
	uint m_timesPerSec;

	uint m_moduleOrder;
	uint m_numbersOfModule;
	uint m_base;
	uint m_remainderN;
	uint m_remainderK;
	uint m_currentSec;
	uint m_currentLoop;
	uint m_moduleNumber;
};