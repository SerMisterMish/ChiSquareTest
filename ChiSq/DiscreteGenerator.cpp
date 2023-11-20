#include "pch.h"
#include <exception>
#include "DiscreteGenerator.h"

DiscreteGenerator::DiscreteGenerator(const long long* sample, size_t states_number) : _distribution(states_number,
	false), _sample_size(0), _generator_type(generator_type::UNDEF)
{
	_sample = new long long[states_number];
	for (size_t i{}; i < states_number; ++i)
	{
		_sample_size += _sample[i] = sample[i];
	}
}

DiscreteGenerator::DiscreteGenerator(const DiscreteGenerator& DG) : _generator_type(DG._generator_type),
                                                                    _distribution(DG._distribution),
                                                                    _sample_size(DG._sample_size)
{
	_sample = new long long[_sample_size];
	for (size_t i{}; i < _sample_size; ++i)
	{
		_sample[i] = DG._sample[i];
	}
}

DiscreteGenerator& DiscreteGenerator::operator=(const DiscreteGenerator& DG)
{
	if (this == &DG)
		return *this;
	_distribution = DG._distribution;
	_sample_size = DG._sample_size;
	delete[] _sample;
	_sample = new long long[_sample_size];
	for (size_t i{}; i < _sample_size; ++i)
	{
		_sample[i] = DG._sample[i];
	}
	_generator_type = DG._generator_type;

	return *this;
}

DiscreteGenerator& DiscreteGenerator::operator=(DiscreteGenerator&& DG) noexcept
{
	if (this == &DG)
		return *this;
	_distribution = std::move(DG._distribution);
	_sample_size = DG._sample_size;
	_generator_type = DG._generator_type;
	delete[] _sample;
	_sample = DG._sample;
	DG._sample = nullptr;
	DG._sample_size = 0;
	return *this;
}

void DiscreteGenerator::SetSample(const long long* sample, size_t states_number)
{
	delete[] _sample;
	_distribution.SetStatesNumber(states_number);
	_sample = new long long[states_number];
	_sample_size = 0;
	for (size_t i{}; i < states_number; ++i)
	{
		_sample_size += _sample[i] = sample[i];
	}
}

void DiscreteGenerator::Simulate() const
{
	if (_generator_type == generator_type::UNDEF)
		throw std::exception("Generator is not implemented");

	size_t states = _distribution.GetStatesNumber();

	for (size_t j{}; j < states; ++j)
	{
		_sample[j] = 0;
	}

	for (size_t i{}; i < _sample_size; ++i)
	{
		++_sample[GetRandomValue()];
	}
}
