/*
 * Copyright 2017 Google Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

// Googletest matchers for the error::Error and error::ErrorOr types.
#ifndef ARDUINO_ERROR_TESTING_ERROR_MATCHERS_H
#define ARDUINO_ERROR_TESTING_ERROR_MATCHERS_H

#include <ostream>
#include <string>

#include "error.h"
#include "error_or.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

using ::testing::MakePolymorphicMatcher;
using ::testing::MatchResultListener;
using ::testing::PolymorphicMatcher;
using ::testing::StringMatchResultListener;

namespace testing {
namespace error {
namespace internal {

// A matcher that verifies that the provided error object matches the expected
// one.
// Works with both the ::error:Error type and the ::error::ErrorOr type.
// Optionally matches the library number, error number, and subcode as well.
class ErrorMatcher {
public:
  explicit ErrorMatcher(::error::Error::Code expected_code);
  ErrorMatcher(::error::Error::Code expected_code, int expected_library_number);
  ErrorMatcher(::error::Error::Code expected_code, int expected_library_number,
               int expected_error_number);
  ErrorMatcher(::error::Error::Code expected_code, int expected_library_number,
               int expected_error_number, int expected_subcode);
  ~ErrorMatcher();

  template <typename ErrorType>
  bool MatchAndExplain(ErrorType error, MatchResultListener *listener) const;
  void DescribeTo(::std::ostream *os) const;
  void DescribeNegationTo(::std::ostream *os) const;

private:
  // Helper methods used to determine how many of the optional error attributes
  // are being matched alongside with the canonical code.
  bool IgnoreAllOptional() const;
  bool IgnoreSubcode() const;
  bool IgnoreLibraryErrorAndSubcode() const;

  ::error::Error expected_error_;
};

//
// Implementation details of the ErrorMatcher.
//

inline ErrorMatcher::ErrorMatcher(::error::Error::Code expected_code,
                                  int expected_library_number,
                                  int expected_error_number,
                                  int expected_subcode)
    : expected_error_(expected_code, expected_library_number,
                      expected_error_number, expected_subcode) {}

inline ErrorMatcher::ErrorMatcher(::error::Error::Code expected_code,
                                  int expected_library_number,
                                  int expected_error_number)
    : ErrorMatcher(expected_code, expected_library_number,
                   expected_error_number, ::error::kUnspecified) {}

inline ErrorMatcher::ErrorMatcher(::error::Error::Code expected_code,
                                  int expected_library_number)
    : ErrorMatcher(expected_code, expected_library_number,
                   ::error::kUnspecified, ::error::kUnspecified) {}

inline ErrorMatcher::ErrorMatcher(::error::Error::Code expected_code)
    : ErrorMatcher(expected_code, ::error::kUnspecified, ::error::kUnspecified,
                   ::error::kUnspecified) {}

inline ErrorMatcher::~ErrorMatcher() {}

template <typename ErrorType>
inline bool ErrorMatcher::MatchAndExplain(ErrorType error_type,
                                          MatchResultListener *listener) const {
  const ::error::Error &error = error_type.GetError();
  ::error::Error expected_error = expected_error_;

  if (IgnoreAllOptional()) {
    expected_error =
        ::error::Error(expected_error_.CanonicalCode(), error.LibraryNumber(),
                       error.ErrorNumber(), error.Subcode());
  } else if (IgnoreLibraryErrorAndSubcode()) {
    expected_error = ::error::Error(expected_error_.CanonicalCode(),
                                    expected_error_.LibraryNumber(),
                                    error.ErrorNumber(), error.Subcode());
  } else if (IgnoreSubcode()) {
    expected_error = ::error::Error(
        expected_error_.CanonicalCode(), expected_error_.LibraryNumber(),
        expected_error_.ErrorNumber(), error.Subcode());
  }

  return expected_error == error;
}

inline void ErrorMatcher::DescribeTo(::std::ostream *os) const {
  *os << "matches error " << ::testing::PrintToString(expected_error_);
}

inline void ErrorMatcher::DescribeNegationTo(::std::ostream *os) const {
  *os << "does not match error " << ::testing::PrintToString(expected_error_);
}

inline bool ErrorMatcher::IgnoreAllOptional() const {
  return (expected_error_.LibraryNumber() == ::error::kUnspecified &&
          expected_error_.ErrorNumber() == ::error::kUnspecified &&
          expected_error_.Subcode() == ::error::kUnspecified);
}

inline bool ErrorMatcher::IgnoreSubcode() const {
  return (expected_error_.LibraryNumber() != ::error::kUnspecified &&
          expected_error_.ErrorNumber() != ::error::kUnspecified &&
          expected_error_.Subcode() == ::error::kUnspecified);
}

inline bool ErrorMatcher::IgnoreLibraryErrorAndSubcode() const {
  return (expected_error_.LibraryNumber() != ::error::kUnspecified &&
          expected_error_.ErrorNumber() == ::error::kUnspecified &&
          expected_error_.Subcode() == ::error::kUnspecified);
}

// A matcher that matches the value of type T in an error::ErrorOr<T> type using
// the provided inner matcher.
template <typename InnerMatcher> class ErrorOrValueMatcher {
public:
  explicit ErrorOrValueMatcher(InnerMatcher inner_matcher);
  ~ErrorOrValueMatcher();

  template <typename T>
  bool MatchAndExplain(::error::ErrorOr<T> error_or,
                       MatchResultListener *listener) const;

  void DescribeTo(::std::ostream *os) const;
  void DescribeNegationTo(::std::ostream *os) const;

private:
  const InnerMatcher inner_matcher_;
};

//
// Implementation details of the ErrorOrValueMatcher.
//

template <typename InnerMatcher>
inline ErrorOrValueMatcher<InnerMatcher>::ErrorOrValueMatcher(
    InnerMatcher inner_matcher)
    : inner_matcher_(inner_matcher) {}

template <typename InnerMatcher>
inline ErrorOrValueMatcher<InnerMatcher>::~ErrorOrValueMatcher() {}

template <typename InnerMatcher>
template <typename T>
inline bool ErrorOrValueMatcher<InnerMatcher>::MatchAndExplain(
    ::error::ErrorOr<T> error_or, MatchResultListener *listener) const {
  if (!error_or.Ok()) {
    return false;
  }

  StringMatchResultListener inner_listener;
  Matcher<T> matcher = inner_matcher_;

  *listener << "inner_matcher: ";
  matcher.DescribeTo(listener->stream());

  const bool match =
      matcher.MatchAndExplain(error_or.ValueOrDie(), &inner_listener);
  const ::std::string &inner_message = inner_listener.str();
  if (inner_message != "") {
    *listener << ", " << inner_message;
  }
  return match;
}

template <typename InnerMatcher>
inline void
ErrorOrValueMatcher<InnerMatcher>::DescribeTo(::std::ostream *os) const {
  *os << "is ok and holds value that satisfies the inner matcher";
}

template <typename InnerMatcher>
inline void ErrorOrValueMatcher<InnerMatcher>::DescribeNegationTo(
    ::std::ostream *os) const {
  *os << "is not ok or holds value that does not satisfy the inner matcher";
}

} // namespace internal

// The ErrorIs matcher matches an ::error::Error or an ::error::ErrorOr<T>
// instance to the expected error.
// Matches any or all error attributes, depending on the number of expected
// attributes provided in the call.
inline PolymorphicMatcher<internal::ErrorMatcher>
ErrorIs(::error::Error::Code expected_code) {
  return MakePolymorphicMatcher(internal::ErrorMatcher(expected_code));
}

inline PolymorphicMatcher<internal::ErrorMatcher>
ErrorIs(::error::Error::Code expected_code, int expected_library_number) {
  return MakePolymorphicMatcher(
      internal::ErrorMatcher(expected_code, expected_library_number));
}

inline PolymorphicMatcher<internal::ErrorMatcher>
ErrorIs(::error::Error::Code expected_code, int expected_library_number,
        int expected_error_number) {
  return MakePolymorphicMatcher(internal::ErrorMatcher(
      expected_code, expected_library_number, expected_error_number));
}

inline PolymorphicMatcher<internal::ErrorMatcher>
ErrorIs(::error::Error::Code expected_code, int expected_library_number,
        int expected_error_number, int expected_subcode) {
  return MakePolymorphicMatcher(
      internal::ErrorMatcher(expected_code, expected_library_number,
                             expected_error_number, expected_subcode));
}

// The IsOk matcher verifies that an ::error::Error or an ::error::ErrorOr<T>
// instance contains the canonical code Error::OK.
// This is essentially a shortcut for ErrorIs(error::Error::OK);
inline PolymorphicMatcher<internal::ErrorMatcher> IsOk() {
  return MakePolymorphicMatcher(internal::ErrorMatcher(::error::Error::OK));
}

// Macros allowing to verify that a function returning error::Error or
// error::ErrorOr<T> succeeded.
#define EXPECT_OK(expression) EXPECT_THAT(expression, ::testing::error::IsOk())
#define ASSERT_OK(expression) ASSERT_THAT(expression, ::testing::error::IsOk())

// IsOkAndHolds is a matcher that verifies that the provided ::error::ErrorOr<T>
// contains the canonical code ::error::Error::OK and a value of type T that
// matches the provided inner matcher.
template <typename InnerMatcher>
inline PolymorphicMatcher<internal::ErrorOrValueMatcher<InnerMatcher>>
IsOkAndHolds(InnerMatcher inner_matcher) {
  return MakePolymorphicMatcher(
      internal::ErrorOrValueMatcher<InnerMatcher>(inner_matcher));
}

} // namespace error
} // namespace testing

#endif // ARDUINO_ERROR_TESTING_ERROR_MATCHERS_H
