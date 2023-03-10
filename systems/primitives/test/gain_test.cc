#include "drake/systems/primitives/gain.h"

#include <memory>

#include <gtest/gtest.h>

#include "drake/common/eigen_types.h"
#include "drake/common/test_utilities/expect_no_throw.h"
#include "drake/common/test_utilities/expect_throws_message.h"
#include "drake/systems/framework/basic_vector.h"
#include "drake/systems/framework/fixed_input_port_value.h"
#include "drake/systems/framework/test_utilities/scalar_conversion.h"

using Eigen::Vector3d;
using std::make_unique;
using std::unique_ptr;

namespace drake {
namespace systems {
namespace {

template <typename T>
void TestGainSystem(const Gain<T>& gain_system,
                    const Eigen::VectorXd& input_vector,
                    const Eigen::VectorXd& expected_output) {
  auto context = gain_system.CreateDefaultContext();

  // Verifies that Gain allocates no state variables in the context.
  EXPECT_EQ(0, context->num_continuous_states());

  // Checks that the number of input ports in the Gain system and the Context
  // are consistent.
  ASSERT_EQ(1, gain_system.num_input_ports());
  ASSERT_EQ(1, context->num_input_ports());

  // Hook input of the expected size.
  gain_system.get_input_port().FixValue(context.get(), input_vector);

  // Checks the output.
  ASSERT_EQ(1, gain_system.num_output_ports());
  EXPECT_EQ(expected_output, gain_system.get_output_port().Eval(*context));
}

// Tests the ability to use a double as the gain.
GTEST_TEST(GainTest, GainScalarTest) {
  const double kGain{2.0};
  const int kSize = 3;
  const auto gain_system = make_unique<Gain<double>>(kGain, kSize);

  const Eigen::Vector3d input_vector(1.0, 3.14, 2.18);
  const Eigen::Vector3d expected_output(kGain * input_vector);

  // Verifies the gain accessors are OK.
  DRAKE_EXPECT_NO_THROW(gain_system->get_gain());
  EXPECT_EQ(gain_system->get_gain(), kGain);
  DRAKE_EXPECT_NO_THROW(gain_system->get_gain_vector());
  EXPECT_EQ(gain_system->get_gain_vector(),
            VectorX<double>::Ones(kSize) * kGain);

  // Tests ability to compute the gain of a vector.
  TestGainSystem(*gain_system, input_vector, expected_output);
}

// Tests the ability to use a vector as the gain.
GTEST_TEST(GainTest, GainVectorTest) {
  const Vector4<double> gain_values(1.0, 2.0, 3.0, 4.0);
  const auto gain_system = make_unique<Gain<double>>(gain_values);
  const Eigen::Vector4d input_vector(9.81, 5.46, 16.24, 98.12);
  const Eigen::Vector4d expected_output(gain_values.array() *
                                        input_vector.array());

  DRAKE_EXPECT_NO_THROW(gain_system->get_gain_vector());
  EXPECT_EQ(gain_system->get_gain_vector(), gain_values);

  // Tests ability to compute the gain of a vector.
  TestGainSystem(*gain_system, input_vector, expected_output);
}

GTEST_TEST(GainTest, DirectFeedthrough) {
  const int kSize = 3;
  const auto gain_system = make_unique<Gain<double>>(2.0, kSize);
  EXPECT_TRUE(gain_system->HasAnyDirectFeedthrough());
  const auto zero_gain = make_unique<Gain<double>>(0.0, kSize);
  EXPECT_FALSE(zero_gain->HasAnyDirectFeedthrough());
}

GTEST_TEST(GainTest, GainAccessorTest) {
  const Vector4<double> gain_values(1.0, 2.0, 3.0, 4.0);
  const auto gain_system = make_unique<Gain<double>>(gain_values);
  DRAKE_EXPECT_THROWS_MESSAGE(
      gain_system->get_gain(),
      ".*vector \\[1 2 3 4\\] cannot be represented as a scalar value.*");
}

GTEST_TEST(GainTest, ToAutoDiff) {
  const Gain<double> gain(2.0, 3);
  EXPECT_TRUE(is_autodiffxd_convertible(gain, [&](const auto& converted) {
    EXPECT_EQ(1, converted.num_input_ports());
    EXPECT_EQ(1, converted.num_output_ports());
    EXPECT_EQ(Vector3d::Constant(2.0), converted.get_gain_vector());
  }));
}

GTEST_TEST(GainTest, ToSymbolic) {
  const Gain<double> gain(2.0, 3);
  EXPECT_TRUE(is_symbolic_convertible(gain));
}

}  // namespace
}  // namespace systems
}  // namespace drake
