#pragma once
#include <utility>
/**
 * The abstract Distribution class stores the theoretical information about a distribution, i.e. the probabilities and the number of states.
 */
class Distribution
{
public:
	Distribution() = default;

	/**
	 * Constructs a Distribution object with n states and undefined probabilities.
	 * 
	 * @param n Number of states.
	 */
	Distribution(size_t n) : _states(n) { _probabilities = _states ? new double[_states] : nullptr; }

	/**
	 * Constructs a Distribution object by deep copying the elements from the probabilities array.
	 * The probability of the state i is equal to probabilities[i], where i changes from 0 to n - 1.
	 * 
	 * @param probabilities A pointer to an array of probabilities. The condition that the sum of elements is equal to one is not checked.
	 * @param n A number of states in the Distribution.
	 */
	explicit Distribution(const double* probabilities, size_t n);

	/**
	 * Constructs a Distribution object by deep copying the elements from the probabilities initializer list.
	 * The probability of the state i is equal to the i-th element of the probabilities list, where i changes from 0 to the 
	 * lenght of the list.
	 * 
	 * @param probabilities An object of std::initializer_list<double> class, containing probabilities. 
	 * The condition that the sum of elements is equal to one is not checked.
	 */
	explicit Distribution(const std::initializer_list<double>& probabilities);

	Distribution(const Distribution& D);

	Distribution(Distribution&& D) noexcept;

	Distribution& operator=(const Distribution& D);

	Distribution& operator=(Distribution&& D) noexcept;

	/**
	 * Sets the probabilities of the Distribution object by deep copying the elements of the argument probabilities. Assumes that the array probabilities
	 * is of the same size as the number of states of the current Distribution object.
	 *
	 * @param probabilities A pointer to an array of probabilities. The condition that the sum of elements is equal to one is not checked.
	 */
	void SetProbabilitiesSameSize(const double* probabilities);

	/**
	 * Sets the probabilities of the Distribution object by shallow copying the argument probabilities. Assumes that the array probabilities
	 * is of the same size as the number of states of the current Distribution object. Sets the original probabilities pointer value to nullptr.
	 *
	 * @param probabilities A reference to a pointer to an array of probabilities. The condition that the sum of elements is equal to one is not checked.
	 */
	void SetProbabilitiesSameSizeM(double*& probabilities);

	/**
	 * Sets the probabilities of the Distribution object by deep copying the elements from the probabilities array of size n.
	 *
	 * @param probabilities A pointer to an array of probabilities. The condition that the sum of elements is equal to one is not checked.
	 * @param n Number of states in the Distribution.
	 */
	void SetProbabilities(const double* probabilities, size_t n);

	/**
	 * Sets the probabilities of the Distribution object by shallow copying the argument probabilities.
	 * Sets the number of states with to n. 
	 * Assumes that the array probabilities is of the same size as the number of states of the current Distribution object.
	 * Sets the original probabilities pointer value to nullptr.
	 *
	 * @param probabilities A reference to a pointer to an array of probabilities. The condition that the sum of elements is equal to one is not checked.
	 * @param n Number of states in the Distribution.
	 */
	void SetProbabilitiesM(double*& probabilities, size_t n);

	size_t GetStatesNumber() const { return _states; }

	const double* GetProbabilities() const { return _probabilities; }

	void SetStatesNumber(size_t states_number) { _states = states_number; }

	/**
	 * Returns the name of the distribution.
	 */
	virtual const char* GetName() const = 0;

	virtual ~Distribution() { delete[] _probabilities; }
protected:

	/**
	 * Pointer to an array of probability values.
	 */
	double *_probabilities;

	/**
	 * Number of possible states in Distribution.
	 */
	size_t _states;
};

