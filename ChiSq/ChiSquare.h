#pragma once
#include "DiscreteDistribution.h"
#include "DiscreteGenerator.h"

/**
* @mainpage
* @include main.cpp
*/

/**
 * The ChiSquare class performs the chi-square test to calculate the criterion and the p-value.
 */
class ChiSquare
{
  public:
    ChiSquare() = delete;

    /**
     * Constructs an object of the ChiSquare class with the given DiscreteGenerator and DiscreteDistribution.
     * To calculate the criteria generates a sample using the given DiscreteGenerator
     * and gets expected probabilities from the given expected DiscreteDistribution.
     *
     * @param observed_frequencies_generator A pointer to an object of either InverseFunctionDG or DichotomyDG class,
     * according to which a sample of random values is generated.
     * @param expected_distribution DiscreteDistribution of the null hypothesis.
     */
    ChiSquare(const DiscreteGenerator *observed_frequencies_generator, DiscreteDistribution expected_distribution);

    /**
     * Constructs an object of the ChiSquare class with the given sample and distribution, presented as an array of
     * probabilities. To calculate the criteria uses the given sample of observed frequencies and expected
     * probabilities.
     *
     * @param observed_sample A pointer to an array with the observed frequencies.
     * @param expected_probabilities A pointer to an array with the expected probabilities.
     * @param states_number A number of different states in the distribution.
     */
    ChiSquare(const long long *observed_sample, const double *expected_probabilities, size_t states_number);

    /**
     * Constructs an object of the ChiSquare class with the given sample and DiscreteDistribution. To calculate the
     * criteria uses given sample of observed frequencies and distribution.
     * @param observed_sample A pointer to an array with the observed frequencies.
     * @param expected_distribution Distribution of the null hypothesis.
     */
    ChiSquare(const long long *observed_sample, DiscreteDistribution expected_distribution);

    /**
     * Constructs an object of the ChiSquare class with the given DiscreteGenerator and distribution. To calculate the
     * criteria generates a sample with the given generator and gets expected probabilities from the given expected
     * probabilities.
     * @param observed_frequencies_generator A pointer to an object of either InverseFunctionDG or DichotomyDG class,
     * according to which a sample of random values is generated.
     * @param expected_probabilities A pointer to an array with the expected probabilities.
     */
    ChiSquare(const DiscreteGenerator *observed_frequencies_generator, const double *expected_probabilities);

    /**
     * Calculates the statistic criterion and p-value by performing the chi-square test.
     */
    void CalculateStatistic();

    ChiSquare(const ChiSquare &Chi);

    ChiSquare(ChiSquare &&Chi) noexcept;

    void ChangeSampleSize(size_t sample_size)
    {
        _observed->SetSampleSize(sample_size);
    }

    double GetPValue() const
    {
        return _p_value;
    }

    double GetCriterion() const
    {
        return _criterion;
    }

    bool HasGenerator() const
    {
        return has_generator;
    }

    ~ChiSquare()
    {
        delete _observed;
    }

  protected:
    /**
     * Calculated statistic criterion.
     */
    double _criterion;

    /**
     * Calculated p-value
     */
    double _p_value;

    /**
     * DiscreteGenerator, according to which a sample of random values is generated.
     * If the generator was not given, used as a holder for the sample.
     */
    DiscreteGenerator *_observed;

    /**
     * Expected DiscreteDistribution (hypothesis H0)
     */
    DiscreteDistribution _expected;

    /**
     * Checks whether a generator (true) was given or a sample (false).
     */
    bool has_generator;
};
