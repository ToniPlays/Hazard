#pragma once

#include "Core.h"

namespace Hazard {
	class Analyzer 
	{
	public:
		Analyzer() 
		{
			Reset();
		}
		void Reset() 
		{
			m_Average = 0;
			m_Count = 0;
			m_Total = 0;
		}
		void AddSample(double value) 
		{
			if (value > m_Highest) m_Highest = value;
			else if (value < m_Lowest) m_Lowest = value;

			m_Count++;
			m_Average += (value - m_Average) / m_Count;
		}

		double Average() { return m_Average; }
		double Highest() { return m_Highest; }
		double Lowers() { return m_Lowest; }
	private:
		double m_Average;
		uint32_t m_Count;
		double m_Total;

		double m_Highest;
		double m_Lowest;
	};
}