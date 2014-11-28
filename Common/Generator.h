#pragma once
#include <random>

/// \brief Generate random numbers
/// \tparam RealType Expects float, double or long double
template <typename RealType> class Generator
  {
public:
  /// \brief Create a new generator for a region between min and max.
  Generator(
      RealType min, ///< Minimum value
      RealType max) ///< Maximum value
      : m_gen(m_rd()),
        m_dis(min, max)
    {
    }

  /// \brief Generate [count] numbers into [destination].
  void generate(
      RealType *destination, ///< Destination values
      size_t count) ///< Number of values to generate
    {
    for (size_t i = 0; i < count; ++i)
      {
      destination[i] = m_dis(m_gen);
      }
    }

private:
  /// \brief Random number generator, used to seed [m_gen]
  std::random_device m_rd;
  /// \brief Mersenne twister engine used for for generation.
  std::mt19937 m_gen;
  /// \brief Floating point distribution based on random input [m_gen]
  std::uniform_real_distribution<RealType> m_dis;
  };
