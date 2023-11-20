#include "pch.h"
#include "DichotomyDG.h"
//#include "ChiSq.h"
#include "random"

DichotomyDG::DichotomyDG(DiscreteDistribution DD, size_t sample_size) : DiscreteGenerator(std::move(DD), sample_size)
{
	size_t states = _distribution.GetStatesNumber();
	const double* probabilities = _distribution.GetProbabilities();

	_accumulated_probabilities = new double[states];

	_accumulated_probabilities[0] = probabilities[0];
	for (size_t i{ 1 }; i < states; ++i)
	{
		_accumulated_probabilities[i] = _accumulated_probabilities[i - 1] + probabilities[i];
	}

	_generator_type = generator_type::DICHOTOMY;
}

DichotomyDG::DichotomyDG(const DichotomyDG& DDG) : DiscreteGenerator(DDG)
{
	size_t states = _distribution.GetStatesNumber();
	_accumulated_probabilities = new double[states];

	for (size_t i{}; i < states; ++i)
	{
		_accumulated_probabilities[i] = DDG._accumulated_probabilities[i];
	}
}

DichotomyDG::DichotomyDG(const DiscreteGenerator* DG) : DiscreteGenerator(*DG)
{
	if (DG->GetGenType() != generator_type::DICHOTOMY)
	{
		_accumulated_probabilities = nullptr;
		_generator_type = generator_type::DICHOTOMY;
		return;
	}

	size_t states = _distribution.GetStatesNumber();
	_accumulated_probabilities = new double[states];

	for (size_t i{}; i < states; ++i)
	{
		_accumulated_probabilities[i] = ((DichotomyDG*)DG)->_accumulated_probabilities[i];
	}
}

DichotomyDG& DichotomyDG::operator=(const DichotomyDG& DDG)
{
	if (this == &DDG)
	{
		return *this;
	}
	DiscreteGenerator::operator=(DDG);

	delete[] _accumulated_probabilities;

	size_t states = _distribution.GetStatesNumber();
	_accumulated_probabilities = new double[states];

	for (size_t j{}; j < states; ++j)
	{
		_accumulated_probabilities = DDG._accumulated_probabilities;
	}

	return *this;
}

DichotomyDG& DichotomyDG::operator=(DichotomyDG&& DDG) noexcept
{
	if (this == &DDG)
	{
		return *this;
	}

	DiscreteGenerator::operator=(std::move(DDG));
	_accumulated_probabilities = DDG._accumulated_probabilities;
	DDG._accumulated_probabilities = nullptr;

	return *this;
}

size_t DichotomyDG::GetRandomValue() const
{
	if (_generator_type == generator_type::UNDEF)
		throw std::exception("Generator is not implemented");

	size_t states = _distribution.GetStatesNumber();
	
	std::uniform_real_distribution<double> rng(0, 1);
	size_t left{}, right{ states - 1 };
	double alpha = rng(global_rne);

	while (left < right)
	{
		size_t mid = (right + left) / 2;
		if (alpha < _accumulated_probabilities[mid])
			right = mid;
		else
			left = mid + 1;
	}

	return left;
}
