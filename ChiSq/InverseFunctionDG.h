#pragma once
#include <algorithm>
#include <random>

#include "DiscreteGenerator.h"

/**
 * Class InverseFunctionDG stores an object of the class DiscreteDistribution and a sample and provides the inverse function
 * method for the generation of the sample.
 */
class InverseFunctionDG :
    public DiscreteGenerator
{
public:
    InverseFunctionDG() : DiscreteGenerator(), _marked_table(nullptr)
    {
		_generator_type = generator_type::INVERSE_FUNCTION;
    }

	/**
	 * Constructs an object of InverseFunctionDG class with the given DiscreteDitribution and an empty sample.
	 */
	InverseFunctionDG(DiscreteDistribution DD, size_t sample_size = 1);

	InverseFunctionDG(const InverseFunctionDG& IDG);

	InverseFunctionDG(InverseFunctionDG&& IDG) noexcept : DiscreteGenerator(std::move(IDG)), _marked_table(IDG._marked_table)
	{
		IDG._marked_table = nullptr;
		_generator_type = generator_type::INVERSE_FUNCTION;
	}

	InverseFunctionDG(const DiscreteGenerator* DG);

	InverseFunctionDG& operator=(const InverseFunctionDG& IDG);

	InverseFunctionDG& operator=(InverseFunctionDG&& IDG) noexcept;

	/**
	 * Generates one random value of the given DiscreteDistribution using the inverse function method.
	 */
    size_t GetRandomValue() const override;

	~InverseFunctionDG() override
	{
		delete[] _marked_table;
	}
protected:

	/**
	* An array of probability-state pairs, sorted by probabilities in descending order.
	*/
	std::pair<double, unsigned int>* _marked_table;
};

