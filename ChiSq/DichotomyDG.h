#pragma once
#include "DiscreteGenerator.h"

/**
 * Class DichotomyDG stores distribution and sample and provides dichotomy method for generation of the sample.
 */
class DichotomyDG :
    public DiscreteGenerator
{
public:
	DichotomyDG() : DiscreteGenerator(), _accumulated_probabilities(nullptr)
	{
		_generator_type = generator_type::DICHOTOMY;
	}

	/**
	 * Constructs generator with an empty sample
	 */
	DichotomyDG(DiscreteDistribution DD, size_t sample_size = 1);
	
	DichotomyDG(const DichotomyDG& DDG);

	DichotomyDG(DichotomyDG&& DDG) noexcept : DiscreteGenerator(std::move(DDG))
	{
		_accumulated_probabilities = DDG._accumulated_probabilities;
		DDG._accumulated_probabilities = nullptr;

		_generator_type = generator_type::DICHOTOMY;
	}

	DichotomyDG(const DiscreteGenerator* DG);

	DichotomyDG& operator=(const DichotomyDG& DDG);

	DichotomyDG& operator=(DichotomyDG&& DDG) noexcept;

	/**
	 * Generates one random value of the given DiscreteDistribution using the dichotomy method.
	 */
	size_t GetRandomValue() const override;

	~DichotomyDG() override
	{
		delete[] _accumulated_probabilities;
	}

protected:

	/**
	* An array of the accumulated probabilities of the given DiscreteDistribution.
	*/
	double* _accumulated_probabilities;
};
