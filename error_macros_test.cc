// Copyright 2017 Google Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "error.h"
#include "error_macros.h"
#include "gmock/gmock.h"
#include "testing/error_matchers.h"
#include "gtest/gtest.h"

using ::error::Error;
using ::error::ErrorOr;
using ::testing::error::ErrorIs;
using ::testing::error::IsOkAndHolds;

namespace {

// Returns an error with the provided canonical code.
Error ReturnWithCode(Error::Code code) { return code; }

// Uses the RETURN_IF_ERROR to forward the error if not OK.
// Otherwise returns Error::UNKNOWN.
Error ForwardWithCode(Error::Code code) {
  RETURN_IF_ERROR(Error(Error::OK));
  // Can be used multiple times in the same scope.
  RETURN_IF_ERROR(ReturnWithCode(code));
  return Error::UNKNOWN;
}

const int kValue = 42;

// Returns kValue if the code is Error::OK, otherwise returns the error.
ErrorOr<int> ValueOrError(Error::Code code) {
  if (code == Error::OK) {
    return kValue;
  }
  return code;
}

// Assigns the value or forwards the error.
Error AssignOrForwardError(Error::Code code) {
  ASSIGN_OR_RETURN(int value, ValueOrError(code));
  value++; // Avoid compiler warning about unused variable.
  return Error::OK;
}

// Returns the value or forwards the error.
ErrorOr<int> ReturnOrForwardError(Error::Code code) {
  ASSIGN_OR_RETURN(int value, ValueOrError(code));
  return value;
}

// Returns the value when assigning to a predefined variable.
ErrorOr<int> ReturnFromPredefined(Error::Code code) {
  int value;
  ASSIGN_OR_RETURN(
      value,
      ValueOrError(code)); // Can be used multiple times in the same scope.
  ASSIGN_OR_RETURN(value, ValueOrError(code));
  return value;
}

TEST(ReturnIfErrorTest, ForwardsError) {
  EXPECT_THAT(ForwardWithCode(Error::INTERNAL_ERROR),
              ErrorIs(Error::INTERNAL_ERROR));
}

TEST(ReturnIfErrorTest, DoesNotForwardOnOkError) {
  EXPECT_THAT(ForwardWithCode(Error::OK), ErrorIs(Error::UNKNOWN));
}

TEST(AssignOrReturnTest, ForwardsError) {
  EXPECT_THAT(AssignOrForwardError(Error::INTERNAL_ERROR),
              ErrorIs(Error::INTERNAL_ERROR));
}

TEST(AssignOrReturnTest, AssignsWhenNoError) {
  EXPECT_THAT(AssignOrForwardError(Error::OK), ErrorIs(Error::OK));
}

TEST(AssignOrReturnTest, ForwardsErrorOr) {
  EXPECT_THAT(ReturnOrForwardError(Error::INTERNAL_ERROR),
              ErrorIs(Error::INTERNAL_ERROR));
}

TEST(AssignOrReturnTest, ReturnsValueWhenNoError) {
  EXPECT_THAT(ReturnOrForwardError(Error::OK), IsOkAndHolds(kValue));
}

TEST(AssignOrReturnTest, ReturnsFromPredefined) {
  EXPECT_THAT(ReturnFromPredefined(Error::OK), IsOkAndHolds(kValue));
}

} // namespace
