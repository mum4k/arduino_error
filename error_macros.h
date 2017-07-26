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

// Macros for working with the ::error::Error and ::error::ErrorOr types.
#ifndef ARDUINO_ERROR_ERROR_MACROS_H
#define ARDUINO_ERROR_ERROR_MACROS_H

#ifdef NATIVE_BUILD

// Use this in the global namespace in the header file of each library that
// wants to use library numbers in its errors. This ensures that no two
// libraries used in the same build accidentally define the same library
// numbers.  This only generates code if running in native C++ environment
// (-DNATIVE_BUILD) and has no effect on the code compiled for the Arduino
// platform.
#define CHECK_UNIQUE_LIBRARY_NUMBER(id)                                        \
  const bool kUniqueLibraryNumber##id = true;

#else // NATIVE_BUILD

#define CHECK_UNIQUE_LIBRARY_NUMBER(id)

#endif

// Evaluates the provided expression, which must result in either an
// ::error::Error or ::error::ErrorOr<T>.
// If the returned error doesn't contain the canonical code ::error::Error::OK,
// it returns the same error instance from the current function.
//
// Example use:
//   Error Foo() { ... }
//
//   RETURN_IF_ERROR(Foo());
#define RETURN_IF_ERROR(expression)                                            \
  {                                                                            \
    auto error = expression;                                                   \
    if (!error.Ok()) {                                                         \
      return error;                                                            \
    }                                                                          \
  }

// Evaluates the provided expression which must result in an
// ::error::ErrorOr<T>.
// If the returned error doesn't contain the canonical code ::error::Error::OK,
// it returns the same error instance from the current function.
// Otherwise it extracts the value and assigns it to the provided variable name.
//
// Example use:
//   ErrorOr<int> Foo() { ... }
//
//   ASSIGN_OR_RETURN(int value, Foo());
//
// Alternatively you can declare the variable before using the macro:
//   int value;
//   ASSIGN_OR_RETURN(value, Foo());
#define ASSIGN_OR_RETURN(type_variable_name, expression)                       \
  ASSIGN_OR_RETURN_IMPL(APPEND_NUMBER(error_or_value, __LINE__),               \
                        type_variable_name, expression)

//
// Implementation details for ASSIGN_OR_RETURN.
//

#define ASSIGN_OR_RETURN_IMPL(error_or_value, type_variable_name, expression)  \
  auto error_or_value = expression;                                            \
  if (!error_or_value.Ok()) {                                                  \
    return error_or_value.GetError();                                          \
  }                                                                            \
  type_variable_name = error_or_value.ValueOrDie();

// Appends the number to the expression. Used to make sure that
// multiple ASSIGN_OR_RETURN statements can be used in the same scope.
// They declare a local variable names error_or_valueN, where N is the number.
#define APPEND_NUMBER(expression, number)                                      \
  APPEND_NUMBER_INNER(expression, number)

// Inner macro to force the preprocessor to expand the __LINE__ macro.
#define APPEND_NUMBER_INNER(expression, number) expression##number

#endif // ARDUINO_ERROR_ERROR_MACROS_H
