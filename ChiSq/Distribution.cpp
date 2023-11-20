#include "pch.h"
#include "Distribution.h"
#include <functional>

void Distribution::SetProbabilitiesSameSize(const double* probabilities)
{
	memcpy(_probabilities, probabilities, _states * sizeof(double));
}

void Distribution::SetProbabilitiesSameSizeM(double*& probabilities)
{
	delete[] _probabilities;
	_probabilities = probabilities;
	probabilities = nullptr;
}

void Distribution::SetProbabilities(const double* probabilities, size_t n)
{
	delete[] _probabilities;
	_states = n;
	if (!_states)
	{
		_probabilities = nullptr;
		return;
	}
	_probabilities = new double[_states];
	SetProbabilitiesSameSize(probabilities);
}

void Distribution::SetProbabilitiesM(double*& probabilities, size_t n)
{
	_states = n;
	SetProbabilitiesSameSizeM(probabilities);
}

Distribution::Distribution(const double* probabilities, size_t n) : _probabilities(nullptr)
{
	SetProbabilities(probabilities, n);
	// _probabilities = new double[_states];
	// for (size_t i{}; i < n; ++i)
	// {
	// 	_probabilities[i] = probabilities[i];
	// }
}

Distribution::Distribution(const std::initializer_list<double>& probabilities) : _probabilities(nullptr)
{
	SetProbabilities(std::data(probabilities), probabilities.size());
}

Distribution::Distribution(const Distribution& D) : _probabilities(nullptr)
{
	SetProbabilities(D._probabilities, D._states);
}

Distribution::Distribution(Distribution&& D) noexcept : _probabilities(nullptr)
{
	SetProbabilitiesM(D._probabilities, D._states);
	// D._probabilities = nullptr;
	D._states = 0;
}

Distribution& Distribution::operator=(const Distribution& D)
{
	if (this == &D)
		return *this;
	SetProbabilities(D._probabilities, D._states);
	// delete[] _probabilities;
	// _states = D._states;
	// _probabilities = new double[_states];
	// for (size_t i{}; i < _states; ++i)
	// {
	// 	_probabilities[i] = D._probabilities[i];
	// }
	return *this;
}

Distribution& Distribution::operator=(Distribution&& D) noexcept
{
	if (this == &D)
		return *this;
	SetProbabilitiesM(D._probabilities, D._states);
	// delete[] _probabilities;
	// _probabilities = D._probabilities;
	// _states = D._states;
	// D._probabilities = nullptr;
	D._states = 0;
	return *this;
}