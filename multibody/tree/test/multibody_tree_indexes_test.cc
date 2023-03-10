#include "drake/multibody/tree/multibody_tree_indexes.h"

#include <gtest/gtest.h>

namespace drake {
namespace multibody {
namespace {

// Verifies the correct behavior of provided functions.
GTEST_TEST(MultibodyTreeIndexes, ConstantFunctions) {
  EXPECT_EQ(world_index(), BodyIndex(0));
  EXPECT_EQ(world_frame_index(), FrameIndex(0));
  EXPECT_EQ(world_model_instance(), ModelInstanceIndex(0));
  EXPECT_EQ(default_model_instance(), ModelInstanceIndex(1));
}

// Verifies that it is not possible to convert between two different
// index types.
GTEST_TEST(MultibodyTreeIndexes, ConversionNotAllowedBetweenDifferentTypes) {
  // Conversion is not allowed between two different index types.
  // Note: the extra set of parentheses are needed to avoid the test macro
  // getting confused with the comma inside the template brackets.
  EXPECT_FALSE((std::is_convertible_v<BodyIndex, FrameIndex>));
  // The trivial case of course is true.
  EXPECT_TRUE((std::is_convertible_v<BodyIndex, BodyIndex>));
  EXPECT_TRUE((std::is_convertible_v<FrameIndex, FrameIndex>));
}

}  // namespace
}  // namespace multibody
}  // namespace drake
