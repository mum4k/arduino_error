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

#include "testing/error_matchers.h"

#include "error.h"
#include "error_or.h"
#include "gtest/gtest.h"

namespace testing {
namespace error {
namespace {

using ::error::Error;
using ::error::ErrorOr;

TEST(ErrorMatcherTest, IsOkWorks) {
  Error ok(Error::OK);
  Error error(Error::INTERNAL_ERROR);
  EXPECT_THAT(ok, IsOk());
  EXPECT_OK(ok);
  ASSERT_OK(ok);
  EXPECT_THAT(error, Not(IsOk()));
}

TEST(ErrorMatcherTest, MatchesCode) {
  Error error(Error::INTERNAL_ERROR);
  EXPECT_THAT(error, ErrorIs(Error::INTERNAL_ERROR));
  EXPECT_THAT(error, Not(ErrorIs(Error::OK)));
}

const int kLibraryNumber = 13;

TEST(ErrorMatcherTest, MatchesCodeAndLibraryNumber) {
  Error error(Error::INTERNAL_ERROR, kLibraryNumber);
  EXPECT_THAT(error, ErrorIs(Error::INTERNAL_ERROR, kLibraryNumber));
  EXPECT_THAT(error, Not(ErrorIs(Error::OK, kLibraryNumber)));
  EXPECT_THAT(error, Not(ErrorIs(Error::INTERNAL_ERROR, kLibraryNumber + 1)));
}

const int kErrorNumber = 42;

TEST(ErrorMatcherTest, MatchesCodeLibraryAndErrorNumber) {
  Error error(Error::INTERNAL_ERROR, kLibraryNumber, kErrorNumber);
  EXPECT_THAT(error,
              ErrorIs(Error::INTERNAL_ERROR, kLibraryNumber, kErrorNumber));
  EXPECT_THAT(error, Not(ErrorIs(Error::OK, kLibraryNumber, kErrorNumber)));
  EXPECT_THAT(error, Not(ErrorIs(Error::INTERNAL_ERROR, kLibraryNumber + 1,
                                 kErrorNumber)));
  EXPECT_THAT(error, Not(ErrorIs(Error::INTERNAL_ERROR, kLibraryNumber,
                                 kErrorNumber + 1)));
}

const int kSubcode = 66;

TEST(ErrorMatcherTest, MatchesAllAttributes) {
  Error error(Error::INTERNAL_ERROR, kLibraryNumber, kErrorNumber, kSubcode);
  EXPECT_THAT(error, ErrorIs(Error::INTERNAL_ERROR, kLibraryNumber,
                             kErrorNumber, kSubcode));
  EXPECT_THAT(error,
              Not(ErrorIs(Error::OK, kLibraryNumber, kErrorNumber, kSubcode)));
  EXPECT_THAT(error, Not(ErrorIs(Error::INTERNAL_ERROR, kLibraryNumber + 1,
                                 kErrorNumber, kSubcode)));
  EXPECT_THAT(error, Not(ErrorIs(Error::INTERNAL_ERROR, kLibraryNumber,
                                 kErrorNumber + 1, kSubcode)));
  EXPECT_THAT(error, Not(ErrorIs(Error::INTERNAL_ERROR, kLibraryNumber,
                                 kErrorNumber, kSubcode + 1)));
}

TEST(ErrorMatcherTest, IgnoresUnspecified) {
  Error error(Error::INTERNAL_ERROR, kLibraryNumber, kErrorNumber, kSubcode);
  EXPECT_THAT(error, ErrorIs(Error::INTERNAL_ERROR));
  EXPECT_THAT(error, ErrorIs(Error::INTERNAL_ERROR, kLibraryNumber));
  EXPECT_THAT(error,
              ErrorIs(Error::INTERNAL_ERROR, kLibraryNumber, kErrorNumber));
}

ErrorOr<int> InternalError() { return Error::INTERNAL_ERROR; }

ErrorOr<int> InternalErrorWithLibraryNumber() {
  return Error(Error::INTERNAL_ERROR, kLibraryNumber);
}

ErrorOr<int> InternalErrorWithLibraryAndErrorNumber() {
  return Error(Error::INTERNAL_ERROR, kLibraryNumber, kErrorNumber);
}

ErrorOr<int> InternalErrorWithAllAttributes() {
  return Error(Error::INTERNAL_ERROR, kLibraryNumber, kErrorNumber, kSubcode);
}

const int kValue = 666;

ErrorOr<int> ReturnValue() { return kValue; }

TEST(ErrorOrMatcherTest, MatchesCode) {
  auto error = InternalError();
  EXPECT_THAT(error, ErrorIs(Error::INTERNAL_ERROR));
  EXPECT_THAT(error, Not(ErrorIs(Error::OK)));
}

TEST(ErrorOrMatcherTest, MatchesCodeAndLibraryNumber) {
  auto error = InternalErrorWithLibraryNumber();
  EXPECT_THAT(error, ErrorIs(Error::INTERNAL_ERROR, kLibraryNumber));
  EXPECT_THAT(error, Not(ErrorIs(Error::OK, kLibraryNumber)));
  EXPECT_THAT(error, Not(ErrorIs(Error::INTERNAL_ERROR, kLibraryNumber + 1)));
}

TEST(ErrorOrMatcherTest, MatchesCodeLibraryAndErrorNumber) {
  auto error = InternalErrorWithLibraryAndErrorNumber();
  EXPECT_THAT(error,
              ErrorIs(Error::INTERNAL_ERROR, kLibraryNumber, kErrorNumber));
  EXPECT_THAT(error, Not(ErrorIs(Error::OK, kLibraryNumber, kErrorNumber)));
  EXPECT_THAT(error, Not(ErrorIs(Error::INTERNAL_ERROR, kLibraryNumber + 1,
                                 kErrorNumber)));
  EXPECT_THAT(error, Not(ErrorIs(Error::INTERNAL_ERROR, kLibraryNumber,
                                 kErrorNumber + 1)));
}

TEST(ErrorOrMatcherTest, MatchesAllAttributes) {
  auto error = InternalErrorWithAllAttributes();
  EXPECT_THAT(error, ErrorIs(Error::INTERNAL_ERROR, kLibraryNumber,
                             kErrorNumber, kSubcode));
  EXPECT_THAT(error,
              Not(ErrorIs(Error::OK, kLibraryNumber, kErrorNumber, kSubcode)));
  EXPECT_THAT(error, Not(ErrorIs(Error::INTERNAL_ERROR, kLibraryNumber + 1,
                                 kErrorNumber, kSubcode)));
  EXPECT_THAT(error, Not(ErrorIs(Error::INTERNAL_ERROR, kLibraryNumber,
                                 kErrorNumber + 1, kSubcode)));
  EXPECT_THAT(error, Not(ErrorIs(Error::INTERNAL_ERROR, kLibraryNumber,
                                 kErrorNumber, kSubcode + 1)));
}

TEST(ErrorOrMatcherTest, IgnoresUnspecified) {
  auto error = InternalErrorWithAllAttributes();
  EXPECT_THAT(error, ErrorIs(Error::INTERNAL_ERROR));
  EXPECT_THAT(error, ErrorIs(Error::INTERNAL_ERROR, kLibraryNumber));
  EXPECT_THAT(error,
              ErrorIs(Error::INTERNAL_ERROR, kLibraryNumber, kErrorNumber));
}

TEST(ErrorOrMatcherTest, WorksWhenErrorOrHoldsValue) {
  auto error = ReturnValue();
  EXPECT_THAT(error, Not(ErrorIs(Error::INTERNAL_ERROR, kLibraryNumber,
                                 kErrorNumber, kSubcode)));
}

TEST(ErrorOrValueMatcher, MatchesValue) {
  auto error_or_value = ReturnValue();
  EXPECT_THAT(error_or_value, IsOkAndHolds(kValue));
  EXPECT_THAT(error_or_value, IsOkAndHolds(Ge(kValue)));
  EXPECT_THAT(error_or_value, Not(IsOkAndHolds(Eq(0))));
}

} // namespace
} // namespace error
} // namespace testing
