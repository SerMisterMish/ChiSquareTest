#include "pch.h"
#include "DiscreteDistribution.h"

DiscreteDistribution::DiscreteDistribution(size_t n, bool uniform) : Distribution(n)
{
	if (uniform)
	{
		double p = 1.0 / _states;
		for (size_t i{}; i < n; ++i)
		{
			_probabilities[i] = p;
		}
	}
}
