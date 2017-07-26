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

// A class that indicates the success or failure after running a function.
#ifndef ARDUINO_ERROR_ERROR_H
#define ARDUINO_ERROR_ERROR_H

#ifdef NATIVE_BUILD

#include <ostream>

#endif // NATIVE_BUILD

namespace error {

// An error number used when no error number was specified.
const int kUnspecified = -1;

// An object that represents the result of an execution.
//
// An instance of the Error class always contains at least the canonical error
// code which indicates the overall result of the execution.
//
// An Error instance can contain additional information that indicate the
// source of the error. The library number can point to the c++ library that
// originated the error. The error number allows each library to define its own
// error space using an enum defining the possible errors. The error subcode
// can optionally carry application specific error code, like the error code
// returned by a hardware device.
class Error {
public:
  // Canonical error codes.
  enum Code {
    // The operation suceeded.
    OK,
    // The operation failed due to an invalid or incompatible argument.
    INVALID_ARGUMENT,
    // The operation failed due to an internal error.
    INTERNAL_ERROR,
    // The functionality isn't implemented.
    UNIMPLEMENTED,
    // An unknown error.
    UNKNOWN,
  };

  // The default constructor creates an error with the code Error::OK.
  Error();

  // Creates an error with the provided canonical error code.
  Error(Code canonical_code);

  // Creates an error specifying the canonical error code and the number of the
  // library that produced the error.
  Error(Code canonical_code, int library_number);

  // Creates an error specifying the canonical error code, number of the
  // library that produced the error and the error number within the library.
  Error(Code canonical_code, int library_number, int error_number);

  // Similar to the above, but also specifies a subcode.  This can be useful
  // for example when reporting an error dealing with a hardware component that
  // also specifies its own error codes.
  Error(Code canonical_code, int library_number, int error_number, int subcode);
  ~Error();

  // Determines if the operation succeeded.
  bool Ok() const;

  // Retrieves the canonical error code represented by this object.
  Code CanonicalCode() const;

  // Returns itself. This is a convenience method so that Error and ErrorOr have
  // the same interface.
  const Error &GetError() const;

  // Retrieves the library number that produced this error, or kUnspecified if
  // not set.
  int LibraryNumber() const;

  // Retrieves the error number within the library, or kUnspecified if not set.
  int ErrorNumber() const;

  // Retrieves the error subcode code or kUnspecified if not set.
  int Subcode() const;

  bool operator==(const Error &other) const;
  bool operator!=(const Error &other) const;

private:
  Code canonical_code_;
  int library_number_;
  int error_number_;
  int subcode_;
};

#ifdef NATIVE_BUILD

// Prints human readable representation of Error when running native c++ tests.
void PrintTo(const Error &error, ::std::ostream *os);

#endif

} // namespace error

#endif // ARDUINO_ERROR_ERROR_H
