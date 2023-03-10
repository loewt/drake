/* clang-format off to disable clang-format-includes */
#include "drake/common/autodiff.h"
#include "drake/common/ad/test/standard_operations_test.h"
/* clang-format on */

namespace drake {
namespace test {
namespace {

TEST_F(AutoDiffXdTest, Atan2) {
  CHECK_BINARY_FUNCTION_ADS_ADS(atan2, x, y, 0.1);
  CHECK_BINARY_FUNCTION_ADS_ADS(atan2, x, y, -0.1);
  CHECK_BINARY_FUNCTION_ADS_ADS(atan2, y, x, 0.4);
  CHECK_BINARY_FUNCTION_ADS_ADS(atan2, y, x, -0.4);
}

}  // namespace
}  // namespace test
}  // namespace drake
