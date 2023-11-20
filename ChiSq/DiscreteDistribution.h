#pragma once
#include "Distribution.h"

/**
 * The DiscreteDistribution class stores the theoretical information about an arbitrary discrete distribution with the finite number 
 * of states, i.e. the probabilities, the number of states and the distribution name.
 */
class DiscreteDistribution :
	public Distribution
{
public:
	DiscreteDistribution() = default;

	/**
	 * Constructs a DiscreteDistribution object with n states, either uniform or undefined.
	 *
	 * @param n The number of states in the DiscreteDistribution object.
	 * @param uniform If set to true will construct a uniform discrete distribution with n states.
	 */
	explicit DiscreteDistribution(size_t n, bool uniform = true);


	/**
	 *	Constructs a DiscreteDistribution object by deep copying the probabilities array.
	 *	The probability of the state i is equal to probabilities[i], where i changes from 0 to n - 1.
	 * 
	 * @param probabilities A pointer to an array of probabilities. The condition that the sum of elements is equal to one is not checked.
	 * @param n A number of states in the DiscreteDistribution.
	 */
	explicit DiscreteDistribution(const double* probabilities, size_t n) : Distribution(probabilities, n) {}

	/**
	 * Constructs a DiscreteDistribution object by deep copying the elements from the probabilities initializer list.
	 * The probability of the state i is equal to the i-th element of the probabilities list, where i changes from 0 to the
	 * length of the list, not including.
	 *
	 * @param probabilities An object of std::initializer_list<double> class, containing probabilities.
	 * The condition that the sum of elements is equal to one is not checked.
	 */
	explicit DiscreteDistribution(const std::initializer_list<double>& probabilities) : Distribution(probabilities) {}

	DiscreteDistribution(const DiscreteDistribution& D) = default;

	DiscreteDistribution(DiscreteDistribution&& D) noexcept : Distribution(std::move(D)) {}

	DiscreteDistribution& operator=(const DiscreteDistribution& D) = default;

	DiscreteDistribution& operator=(DiscreteDistribution&& D) noexcept { Distribution::operator=(std::move(D)); return *this; }

	/**
	 * Returns the name of the Distribution.
	 */
	const char* GetName() const override { return "Discrete"; }

	~DiscreteDistribution() override = default;
};