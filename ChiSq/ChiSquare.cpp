#include "pch.h"
#include "..\probdist\PROBDIST.H"
#include "ChiSquare.h"
#include "InverseFunctionDG.h"
#include "DichotomyDG.h"


ChiSquare::ChiSquare(const DiscreteGenerator* observed_frequencies_generator,
                     DiscreteDistribution expected_distribution) :
	_expected(std::move(expected_distribution)), has_generator(true)
{
	switch (observed_frequencies_generator->GetGenType())
	{
	case DiscreteGenerator::generator_type::DICHOTOMY:
		_observed = new DichotomyDG(observed_frequencies_generator);
		break;
	case DiscreteGenerator::generator_type::INVERSE_FUNCTION:
	default:
		_observed = new InverseFunctionDG(observed_frequencies_generator);
	}

	CalculateStatistic();
}

ChiSquare::ChiSquare(const long long* observed_sample, const double* expected_probabilities, size_t states_number) :
	_expected(expected_probabilities, states_number), has_generator(false)
{
	_observed = new InverseFunctionDG();
	_observed->SetSample(observed_sample, states_number);

	CalculateStatistic();
}

ChiSquare::ChiSquare(const long long* observed_sample, DiscreteDistribution expected_distribution) :
	_expected(std::move(expected_distribution)), has_generator(false)
{
	_observed = new InverseFunctionDG();
	_observed->SetSample(observed_sample, _expected.GetStatesNumber());

	CalculateStatistic();
}

ChiSquare::ChiSquare(const DiscreteGenerator* observed_frequencies_generator, const double* expected_probabilities) :
	_expected(expected_probabilities, observed_frequencies_generator->GetDistribution().GetStatesNumber()), has_generator(true)
{
	switch (observed_frequencies_generator->GetGenType())
	{
	case DiscreteGenerator::generator_type::DICHOTOMY:
		_observed = new DichotomyDG(observed_frequencies_generator);
		break;
	case DiscreteGenerator::generator_type::INVERSE_FUNCTION:
	default:
		_observed = new InverseFunctionDG(observed_frequencies_generator);
	}

	CalculateStatistic();
}

size_t group_sample(long long* sample, double* expected_probabilities, size_t outcome_n, size_t sample_size)
{
	size_t outcomes_in_sample{outcome_n};
	for (size_t i{}; i < outcome_n; ++i)
	{
		while (sample_size * expected_probabilities[i] < 5 && i < outcome_n - 1)
		{
			sample[i + 1] += sample[i];
			sample[i] = -1;
			expected_probabilities[i + 1] += expected_probabilities[i];
			expected_probabilities[i++] = 0;
			--outcomes_in_sample;
		}
		if (i == outcome_n - 1 && sample_size * expected_probabilities[i] < 5)
		{
			size_t j{i - 1};
			for (; sample[j] == -1; --j);
			sample[i] += sample[j];
			sample[j] = -1;
			expected_probabilities[i] += expected_probabilities[j];
			expected_probabilities[j] = 0;
			--outcomes_in_sample;
			break;
		}
	}
	return outcomes_in_sample;
}

double criterion(const long long* sample, size_t sample_size, const double* expected_probabilities, size_t outcome_n)
{
	double crit{};
	for (unsigned int i{}; i < outcome_n; ++i)
	{
		if (sample[i] != -1)
			crit += (sample[i] - sample_size * expected_probabilities[i]) * (sample[i] - sample_size *
					expected_probabilities[i]) /
				(sample_size * expected_probabilities[i]);
	}
	return crit;
}

void ChiSquare::CalculateStatistic()
{
	if (has_generator)
		_observed->Simulate();

	size_t states = _expected.GetStatesNumber();
	size_t sample_size = _observed->GetSampleSize();

	auto expp_copy = new double[states];
	auto sample_copy = new long long[states];
	memcpy(expp_copy, _expected.GetProbabilities(), states * sizeof(double));
	memcpy(sample_copy, _observed->GetSample(), states * sizeof(long long));

	size_t outcomes_in_sample = group_sample(sample_copy, expp_copy, states, sample_size);

	_criterion = criterion(sample_copy, sample_size, expp_copy, states);
	_p_value = 1 - pChi(_criterion, outcomes_in_sample - 1);

	delete[] expp_copy;
	delete[] sample_copy;
}

ChiSquare::ChiSquare(const ChiSquare& Chi) : _criterion(Chi._criterion), _p_value(Chi._p_value),
                                             _expected(Chi._expected), has_generator(Chi.has_generator)
{
	switch (Chi._observed->GetGenType())
	{
	case DiscreteGenerator::generator_type::DICHOTOMY:
		_observed = new DichotomyDG(Chi._observed);
		break;
	case DiscreteGenerator::generator_type::INVERSE_FUNCTION:
	default:
		_observed = new InverseFunctionDG(Chi._observed);
		break;
	}
}

ChiSquare::ChiSquare(ChiSquare&& Chi) noexcept : _criterion(Chi._criterion), _p_value(Chi._p_value),
                                                 _expected(std::move(Chi._expected)), has_generator(Chi.has_generator)
{
	_observed = Chi._observed;
	Chi._observed = nullptr;
}
