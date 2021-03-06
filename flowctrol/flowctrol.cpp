// flowctrol.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <vector>
#include "flowctrol.h"
#include<windows.h>  

#define	NUMSOFMODULE 71
using namespace std;

moduleManager moduleManager::m_instance;

bool moduleManager::getAllModule(set<uint>& set_module)
{
	set_module.clear();
	set_module.insert(1);
	for (size_t i = 0; i < NUMSOFMODULE; i++)
	{
		set_module.insert(i);
	}
	
	return true;
}

flowCtrlCounter::flowCtrlCounter(uint totalTPS, uint timesPerSec, uint moduleNumber)
	:m_totalTPS(totalTPS),m_timesPerSec(timesPerSec),m_moduleNumber(moduleNumber)
{
	m_moduleOrder = 0;
	m_base = 0;
	m_remainderK = 0;
	m_remainderN = 0;
	m_currentSec = 0;
	m_currentLoop = 0;
	calVariables();
}

void flowCtrlCounter::calVariables()
{
	set<uint> set_module;
	moduleManager& manager = moduleManager::getInstance();
	manager.getAllModule(set_module);
	//cout << "number of module is " << set_module.size() << endl;
	m_numbersOfModule = set_module.size();

	m_base = m_totalTPS / m_numbersOfModule / m_timesPerSec;
	m_remainderN = m_totalTPS % m_numbersOfModule;
	m_remainderK = m_totalTPS / m_numbersOfModule % m_timesPerSec;
	//cout << m_remainderN << " " << m_remainderK << endl;
	uint cnt = 0;
	auto it = set_module.begin();
	for (; it != set_module.end(); it++, cnt++)
	{
		if (*it == m_moduleNumber)
		{
			m_moduleOrder = cnt;
			//cout << "m_moduleOrder=" << m_moduleOrder << endl;
			break;
		}
	}
}



uint flowCtrlCounter::getSndTimesThisLoop()
{
	bool ret = false;
	ret = inLCRORegionMode(m_currentLoop, m_moduleOrder, m_moduleOrder + m_remainderK, m_timesPerSec);
	uint res = m_base;
	if (ret)
	{
		res = m_base + 1;
		//cout << "s1" << endl;
	}

	ret = inLCRORegionMode(m_moduleOrder, m_currentSec*m_remainderN, (m_currentSec + 1)*m_remainderN, m_numbersOfModule);
	if (ret && m_currentLoop == (m_moduleOrder + m_remainderK) % m_timesPerSec)
	{
		res = m_base + 1;
		//cout << "s2" << endl;
	}

	if (++m_currentLoop == m_timesPerSec)
	{
		m_currentLoop = 0;
		if (++m_currentSec == m_numbersOfModule)
		{
			m_currentSec = 0;
		}
	}

	return res;
}

uint getAllModuleTPS(vector<flowCtrlCounter>& vec)
{
	uint sum = 0;
	for (auto it =vec.begin(); it != vec.end(); it++)
	{
		sum += it->getSndTimesThisLoop();
	}
	return sum;
}

int main()
{
	uint tps = 250;
	vector<flowCtrlCounter> vec_counter;
	for (size_t i = 0; i < NUMSOFMODULE; i++)
	{
		vec_counter.push_back(flowCtrlCounter(tps,10,i));
	}
	uint seq = 0;
	while (true)
	{
		cout << "seq=" << seq << ", times=" << getAllModuleTPS(vec_counter) << endl;
		Sleep(100);
		seq++;
	}
}