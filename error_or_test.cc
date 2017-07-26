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

#include "error_or.h"
#include "gtest/gtest.h"

namespace error {
namespace {

ErrorOr<int> NoErrorFromCode() { return Error::OK; }

ErrorOr<int> NoErrorFromOkError() { return Error(Error::OK); }

ErrorOr<int> InternalError() { return Error(Error::INTERNAL_ERROR); }

const int kReturnValue = 63;

ErrorOr<int> Value() { return kReturnValue; }

TEST(ErrorOrTest, DefaultConstructsUnknownError) {
  ErrorOr<int> error_or_int;
  EXPECT_FALSE(error_or_int.Ok());
  EXPECT_EQ(Error::UNKNOWN, error_or_int.GetError().CanonicalCode());
}

TEST(ErrorOrTest, OkConvertedToUnknownFromErrorCode) {
  ErrorOr<int> error_or_int = NoErrorFromCode();
  EXPECT_FALSE(error_or_int.Ok());
  EXPECT_EQ(Error::UNKNOWN, error_or_int.GetError().CanonicalCode());
}

TEST(ErrorOrTest, OkConvertedToUnknownFromError) {
  ErrorOr<int> error_or_int = NoErrorFromOkError();
  EXPECT_FALSE(error_or_int.Ok());
  EXPECT_EQ(Error::UNKNOWN, error_or_int.GetError().CanonicalCode());
}

TEST(ErrorOrTest, ReturnsTheContainedError) {
  ErrorOr<int> error_or_int = InternalError();
  EXPECT_FALSE(error_or_int.Ok());
  EXPECT_EQ(Error::INTERNAL_ERROR, error_or_int.GetError().CanonicalCode());
}

TEST(ErrorOrTest, ReturnsTheContainedValue) {
  ErrorOr<int> error_or_int = Value();
  EXPECT_TRUE(error_or_int.Ok());
  EXPECT_EQ(Error::OK, error_or_int.GetError().CanonicalCode());
  int value = error_or_int.ValueOrDie();
  EXPECT_EQ(kReturnValue, value);
}

TEST(ErrorOrTest, ReturnsTheContainedValueByReference) {
  ErrorOr<int> error_or_int = Value();
  EXPECT_TRUE(error_or_int.Ok());
  EXPECT_EQ(Error::OK, error_or_int.GetError().CanonicalCode());
  const int &value = error_or_int.ValueOrDie();
  EXPECT_EQ(kReturnValue, value);
}

} // namespace
} // namespace error
