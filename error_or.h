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

// A class that holds an error or the return value after running a function.
#ifndef ARDUINO_ERROR_ERROR_OR_H
#define ARDUINO_ERROR_ERROR_OR_H

#include <stdlib.h>

#ifdef NATIVE_BUILD

#include <ostream>

#include "error.h"
#include "gmock/gmock.h"

#else // NATIVE_BUILD

#include <Error.h>

#endif // NATIVE_BUILD

namespace error {

// An object that exclusively holds either an error code, or the return value.
template <typename T> class ErrorOr {
public:
  // Creates an ErrorOr instance that will hold the provided error and no value.
  // If the provided Error holds canonical Error::OK, it will be changed to
  // Error::UNKNOWN to keep the guarantee that this objects holds either an
  // error or a value.
  ErrorOr(Error error);
  ErrorOr(Error::Code error_code);

  // Creates an ErrorOr instance with the canonical error code Error::UNKNOWN.
  ErrorOr();

  // Creates an ErrorOr instance with the provided value.
  // When created using this constructor, calls to Ok() will return true and
  // calls to GetError() will return an error with the canonical code Error::OK.
  // Calls to ValueOrDie() will return the value.
  ErrorOr(T value);
  ~ErrorOr();

  // Determines if the operation succeeded, in which case this object holds the
  // promised return value.
  bool Ok() const;

  // Returns the error stored in this object.
  const Error &GetError() const;

  // Returns the value or dies if called when the object contains an error.
  const T &ValueOrDie() const;
  T &ValueOrDie();

private:
  Error error_;
  T value_;
};

//
// Implementation details of the ErrorOr class.
//

template <typename T> inline ErrorOr<T>::ErrorOr(Error error) : error_(error) {
  if (error_.Ok()) {
    error_ = Error(Error::UNKNOWN);
  }
}

template <typename T> inline ErrorOr<T>::ErrorOr() : ErrorOr(Error::UNKNOWN) {}

template <typename T>
inline ErrorOr<T>::ErrorOr(Error::Code error_code)
    : ErrorOr(Error(error_code)) {}

template <typename T>
inline ErrorOr<T>::ErrorOr(T value) : error_(Error(Error::OK)), value_(value) {}

template <typename T> inline ErrorOr<T>::~ErrorOr() {}

template <typename T> inline bool ErrorOr<T>::Ok() const { return error_.Ok(); }

template <typename T> inline const Error &ErrorOr<T>::GetError() const {
  return error_;
}

template <typename T> inline const T &ErrorOr<T>::ValueOrDie() const {
  if (!Ok()) {
    abort();
  }
  return value_;
}

template <typename T> inline T &ErrorOr<T>::ValueOrDie() {
  if (!Ok()) {
    abort();
  }
  return value_;
}

#ifdef NATIVE_BUILD

// Prints a human readable representation of ErrorOr for use in tests.
template <typename T>
void PrintTo(const ErrorOr<T> &error_or, ::std::ostream *os) {
  *os << "ErrorOr<T>(";
  if (error_or.Ok()) {
    *os << "with value " << testing::PrintToString(error_or.ValueOrDie())
        << ")";
  } else {
    *os << "with error " << testing::PrintToString(error_or.GetError());
    *os << ")";
  }
}

#endif

} // namespace

#endif // ARDUINO_ERROR_ERROR_OR_H
