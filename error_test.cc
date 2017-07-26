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
#include "gtest/gtest.h"

namespace error {
namespace {

TEST(ErrorTest, IsOkWhenOk) {
  Error error(Error::OK);
  EXPECT_TRUE(error.Ok());
}

TEST(ErrorTest, IsOkWithDefaultConstructor) {
  Error error;
  EXPECT_TRUE(error.Ok());
}

TEST(ErrorTest, IsNotOkWithCanonicalCode) {
  Error error(Error::INTERNAL_ERROR);
  EXPECT_FALSE(error.Ok());
}

TEST(ErrorTest, AttributesDefaultToUnspecified) {
  Error error;
  EXPECT_EQ(kUnspecified, error.LibraryNumber());
  EXPECT_EQ(kUnspecified, error.ErrorNumber());
  EXPECT_EQ(kUnspecified, error.Subcode());
}

const int kLibraryNumber = 1;
const int kErrorNumber = 2;

TEST(ErrorTest, InitializesWithLibraryNumberAndError) {
  Error error(Error::OK, kLibraryNumber, kErrorNumber);
  EXPECT_EQ(kLibraryNumber, error.LibraryNumber());
  EXPECT_EQ(kErrorNumber, error.ErrorNumber());
  EXPECT_EQ(kUnspecified, error.Subcode());
}

const int kSubcode = 3;

TEST(ErrorTest, InitializesWithAllProvided) {
  Error error(Error::OK, kLibraryNumber, kErrorNumber, kSubcode);
  EXPECT_EQ(kLibraryNumber, error.LibraryNumber());
  EXPECT_EQ(kErrorNumber, error.ErrorNumber());
  EXPECT_EQ(kSubcode, error.Subcode());
}

TEST(ErrorTest, ComparisonOperatorsWork) {
  Error error(Error::OK, kLibraryNumber, kErrorNumber, kSubcode);
  Error same_error(Error::OK, kLibraryNumber, kErrorNumber, kSubcode);
  Error different_error_code(Error::UNKNOWN, kLibraryNumber, kErrorNumber,
                             kSubcode);
  Error different_module_number(Error::OK, kLibraryNumber + 1, kErrorNumber,
                                kSubcode);
  Error different_module_error_number(Error::OK, kLibraryNumber,
                                      kErrorNumber + 1, kSubcode);
  Error different_subcode(Error::OK, kLibraryNumber, kErrorNumber,
                          kSubcode + 1);

  // Same errors.
  EXPECT_TRUE(error == same_error);
  EXPECT_FALSE(error != same_error);

  // Different errors, operator==.
  EXPECT_FALSE(error == different_error_code);
  EXPECT_FALSE(error == different_module_number);
  EXPECT_FALSE(error == different_module_error_number);
  EXPECT_FALSE(error == different_subcode);

  // Different errors, operator!=.
  EXPECT_TRUE(error != different_error_code);
  EXPECT_TRUE(error != different_module_number);
  EXPECT_TRUE(error != different_module_error_number);
  EXPECT_TRUE(error != different_subcode);
}

TEST(ErrorTest, ReturnsItself) {
  Error error(Error::OK, kLibraryNumber, kErrorNumber, kSubcode);
  EXPECT_TRUE(error == error.GetError());
}

} // namespace
} // namespace error
