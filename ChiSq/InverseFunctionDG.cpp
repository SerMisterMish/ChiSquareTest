#include "pch.h"
#include "InverseFunctionDG.h"
#include "algorithm"
//#include "ChiSq.h"
#include "random"

InverseFunctionDG::InverseFunctionDG(DiscreteDistribution DD, size_t sample_size) : DiscreteGenerator(std::move(DD), sample_size)
{
	size_t states = _distribution.GetStatesNumber();
	const double* probabilities = _distribution.GetProbabilities();
	_marked_table = new std::pair<double, unsigned int>[states];
	for (size_t i{}; i < states; ++i)
	{
		_marked_table[i] = { probabilities[i], i };
	}
	sort(_marked_table, _marked_table + states, std::greater<>());

	_generator_type = generator_type::INVERSE_FUNCTION;
}

InverseFunctionDG::InverseFunctionDG(const InverseFunctionDG& IDG) : DiscreteGenerator(IDG)
{
	size_t states = _distribution.GetStatesNumber();
	_marked_table = new std::pair<double, unsigned int>[states];
	for (size_t i{}; i < states; ++i)
	{
		_marked_table[i] = IDG._marked_table[i];
	}
}

InverseFunctionDG::InverseFunctionDG(const DiscreteGenerator* DG) : DiscreteGenerator(*DG)
{
	if (DG->GetGenType() != generator_type::INVERSE_FUNCTION)
	{
		_marked_table = nullptr;
		_generator_type = generator_type::UNDEF;
		return;
	}
	
	size_t states = _distribution.GetStatesNumber();
	_marked_table = new std::pair<double, unsigned int>[states];
	for (size_t i{}; i < states; ++i)
	{
		_marked_table[i] = ((InverseFunctionDG*)DG)->_marked_table[i];
	}
}

InverseFunctionDG& InverseFunctionDG::operator=(const InverseFunctionDG& IDG)
{
	if (this == &IDG)
	{
		return *this;
	}
	DiscreteGenerator::operator=(IDG);
	delete[] _marked_table;
	size_t states = _distribution.GetStatesNumber();
	_marked_table = new std::pair<double, unsigned int>[states];
	for (size_t j{}; j < states; ++j)
	{
		_marked_table[j] = IDG._marked_table[j];
	}
	return *this;
}

InverseFunctionDG& InverseFunctionDG::operator=(InverseFunctionDG&& IDG) noexcept
{
	if (this == &IDG)
	{
		return *this;
	}
	DiscreteGenerator::operator=(std::move(IDG));
	delete[] _marked_table;
	_marked_table = IDG._marked_table;
	IDG._marked_table = nullptr;

	return *this;
}

size_t InverseFunctionDG::GetRandomValue() const
{
	if (_generator_type == generator_type::UNDEF)
		throw std::exception("Generator is not implemented");

	std::uniform_real_distribution<double> rng(0, 1);
	size_t k{ 0 };
	double p{ _marked_table[0].first }, alpha = rng(global_rne);
	while (alpha >= p)
	{
		p += _marked_table[++k].first;
	}
	return _marked_table[k].second;
}
