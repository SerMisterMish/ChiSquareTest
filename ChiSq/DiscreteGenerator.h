#pragma once
#include "DiscreteDistribution.h"

/**
 * The DiscreteGenerator abstract class stores an object of DiscreteDistribution class and a sample
 * of random values from the given distribution, and provides an interface for the generation of samples of random values with
 * the given distribution.
 */
class DiscreteGenerator
{
public:
	DiscreteGenerator() = default;

	/**
	 * Constructs an object of DiscreteGenerator class with given DiscreteDistribution and given size of the sample.
	 *
	 * @param DD The DiscreteDistribution object, according which the samples will be generated.
	 * @param sample_size The size of the samples.
	 */
	DiscreteGenerator(DiscreteDistribution DD, size_t sample_size = 1) : _distribution(std::move(DD)), _sample_size(sample_size)
	{ _sample = new long long[_sample_size]{}; }

	/**
	 * Constructs an object of DiscreteGenerator class with an undefined DiscreteDistribution and a given sample of the given size.
	 *
	 * @param sample A pointer to an array that contains the frequncies of realizations of some random value.
	 * @param sample_size The size of the sample array.
	 */
	DiscreteGenerator(const long long* sample, size_t states_number);

	DiscreteGenerator(const DiscreteGenerator& DG);

	DiscreteGenerator(DiscreteGenerator&& DG) noexcept : _generator_type(DG._generator_type), _distribution(std::move(DG._distribution)),
	_sample_size(DG._sample_size), _sample(DG._sample)
	{ DG._sample = nullptr; DG._sample_size = 0; }

	DiscreteGenerator& operator=(const DiscreteGenerator& DG);

	DiscreteGenerator& operator=(DiscreteGenerator&& DG) noexcept;

	const long long* GetSample() const { return _sample; }

	DiscreteDistribution& GetDistribution() { return _distribution; }

	const DiscreteDistribution& GetDistribution() const { return _distribution; }

	void SetSample(const long long* sample, size_t states_number);

	size_t GetSampleSize() const { return _sample_size; }

	void SetSampleSize(size_t sample_size) { _sample_size = sample_size; delete[] _sample; _sample = new long long[_sample_size]; }

	/**
	 * Generates one random value of the given DiscreteDistribution.
	 */
	virtual size_t GetRandomValue() const = 0;

	/**
	 * Generates a sample of random values of the given DiscreteDistribution.
	 */
	void Simulate() const;

	enum class generator_type
	{
		UNDEF,
		INVERSE_FUNCTION,
		DICHOTOMY,
	};

	generator_type GetGenType() const { return _generator_type; }

	virtual ~DiscreteGenerator() { delete[] _sample; }
protected:
	/**
	 * Contains the implemented type of the generator.
	 */
	generator_type _generator_type{generator_type::UNDEF};

	/**
	 * An object of DiscreteDistribution class according to which the sample is generated.
	 */
	DiscreteDistribution _distribution;

	/**
	 * Size of the sample.
	 */
	size_t _sample_size;

	/**
	 * A Sample of realizations of random value with given DiscreteDistribution in the form of frequencies.
	 */
	long long *_sample;
};

