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

#ifdef NATIVE_BUILD

#include <ostream>

#include "error.h"

#else // NATIVE_BUILD

#include <Error.h>

#endif // NATIVE_BUILD

namespace error {

Error::Error(Code canonical_code, int library_number, int error_number,
             int subcode)
    : canonical_code_(canonical_code), library_number_(library_number),
      error_number_(error_number), subcode_(subcode) {}

Error::Error(Code canonical_code, int library_number, int error_number)
    : Error(canonical_code, library_number, error_number, kUnspecified) {}

Error::Error(Code canonical_code, int library_number)
    : Error(canonical_code, library_number, kUnspecified, kUnspecified) {}

Error::Error(Code canonical_code)
    : Error(canonical_code, kUnspecified, kUnspecified, kUnspecified) {}

Error::Error() : Error(Error::OK, kUnspecified, kUnspecified, kUnspecified) {}

Error::~Error() {}

bool Error::Ok() const { return canonical_code_ == Error::OK; }

Error::Code Error::CanonicalCode() const { return canonical_code_; }

const Error &Error::GetError() const { return *this; }

int Error::LibraryNumber() const { return library_number_; }

int Error::ErrorNumber() const { return error_number_; }

int Error::Subcode() const { return subcode_; }

bool Error::operator==(const Error &other) const {
  return (CanonicalCode() == other.CanonicalCode() &&
          LibraryNumber() == other.LibraryNumber() &&
          ErrorNumber() == other.ErrorNumber() && Subcode() == other.Subcode());
}

bool Error::operator!=(const Error &other) const { return !(*this == other); }

#ifdef NATIVE_BUILD

void PrintTo(const Error &error, ::std::ostream *os) {
  *os << "Error(Code:";
  switch (error.CanonicalCode()) {
  case Error::OK:
    *os << "OK";
    break;

  case Error::INVALID_ARGUMENT:
    *os << "INVALID_ARGUMENT";
    break;

  case Error::INTERNAL_ERROR:
    *os << "INTERNAL_ERROR";
    break;

  case Error::UNIMPLEMENTED:
    *os << "UNIMPLEMENTED";
    break;

  case Error::UNKNOWN:
    *os << "UNKNOWN";
    break;

  default:
    *os << error.CanonicalCode();
    break;
  }

  if (error.LibraryNumber() != kUnspecified) {
    *os << " LibraryNumber:" << error.LibraryNumber();
  }
  if (error.ErrorNumber() != kUnspecified) {
    *os << " ErrorNumber:" << error.ErrorNumber();
  }
  if (error.Subcode() != kUnspecified) {
    *os << " Subcode:" << error.Subcode();
  }
  *os << ")";
}

#endif

} // namespace error
