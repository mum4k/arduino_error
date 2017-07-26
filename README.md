# Error classes for Arduino

[![Build Status](https://travis-ci.com/mum4k/arduino_error.svg?token=jcEUXwLaBaCvEegQbsHv&branch=master)](https://travis-ci.com/mum4k/arduino_error)

A collection of classes useful for error reporting from C++ functions written for Arduino.

## Disclaimer

This is not an official Google product.

## Background

A function is supposed to perform an operation and maybe return a produced
value. However in real life the operation might fail. These error libraries
define objects that can represent a canonical error code.

A function can return the **error::Error** object to indicate if the operation
succeeded or failed.

If the function also needs to return a value back to the caller, it can return
the **error::ErrorOr\<valueT\>** to the caller. The
**error::ErrorOr\<valueT\>** instance guarantees that it will exclusively hold
either a canonical error code or the produced value.

## Overview of the libraries

*   **error.h** - provides a class that represents an error.
*   **error_or.h** - provides a class that holds a value or an error.
*   **error_macros.h** - provides macros that remove boilerplate when working
    with the error objects.
*   **testing/error_matchers.h** - provides
    [googletest](https://github.com/google/googletest) matchers that can be
    used in unit tests of functions using the error classes.

## Using the error::Error class

A function performing an operation that might fail can be defined as:

```c++
using error::Error;

Error DoOperation(int a) {
  if (a < 0) {
    return Error::INVALID_ARGUMENT;
  }
  return Error::OK;
}
```

The caller can execute the function and verify the output.

```c++
Error error = DoOperation(42);
if (!error.Ok()) {
  // The operation failed.
}

// The operation succeeded.
```

Besides the canonical error code, the **error::Error** object can also
optionally hold a library number, an error number within the library and a
sub-error code. Each of these is represented by an integer. These can be useful
when the error happens deep inside a nested library, since it can help to
identify the origin of the error. The sub-error code can be used to forward
error codes from peripheral devices like attached modems.

## Using the error::ErrorOr\<valueT\> class

A function that produces an integer value, but might fail can be defined as:

```c++
using error:ErrorOr;

ErrorOr<int> DoOperationAndProduce(int a) {
  if (a < 0) {
    return Error::INVALID_ARGUMENT;
  }
  return 42;
}
```

The caller can execute the function and verify the output.

```c++
using error::Error;

ErrorOr<int> error_or = DoOperationAndProduce(42);
if (!error_or.Ok()) {
  // The operation failed, retrieve the error.
  Error error = error_or.GetError();
}

// The operation succeeded, retrieve the value.
int value = error_or.ValueOrDie();
```

## Using the error macros

The code examples above contain a lot of boilerplate. This boilerplate can be
removed by using the provided error macros.

### The RETURN_IF_ERROR macro

The **RETURN_IF_ERROR** macro is useful when calling functions that return
**error::Error** or **error::ErrorOr\<valueT\>** from functions that also
return the same error type. The macro executes the called function and directly
returns the error if the operation failed.

```c++
using error::Error;

// A function that can fail.
Error DoOperation(int a) {
  if (a < 0) {
    return Error::INVALID_ARGUMENT;
  }
  return Error::OK;
}

// A calling function that isn't using the macro.
Error DoWithBoilerplate() {
  Error error = DoOperation(42);
  if (!error.Ok()) {
    return error;
  }
  return Error::OK;
}

// A calling function using the RETURN_IF_ERROR, this is equivalent to the
// function above.
Error DoWithMacro() {
  RETURN_IF_ERROR(DoOperation(42));
  return Error::OK;
}
```

### The ASSIGN_OR_RETURN macro

The **ASSIGN_OR_RETURN** macro is useful when retrieving the value from an
**error::ErrorOr\<valueT\>** type. The macro executes a function that returns
the **error::ErrorOr\<valueT\>** and verifies that the operation succeeded. If
it succeeded, the inner value of type **valueT** will be assigned to a local
variable.  If it failed, the macro directly returns the error.

```c++
using error::ErrorOr;

// A function that can fail.
ErrorOr<int> DoOperationAndProduce(int a) {
  if (a < 0) {
    return Error::INVALID_ARGUMENT;
  }
  return 42;
}

// A calling function that isn't using the macro.
Error DoWithBoilerplate() {
  ErrorOr<int> error_or = DoOperationAndProduce(42);
  if (!error_or.Ok()) {
    return error_or.GetError();
  }

  // The operation succeeded, retrieve the value.
  int value = error_or.ValueOrDie();
  // You can now use the value.
}

// A calling function using the ASSIGN_OR_RETURN, this is equivalent to the
// function above.
Error DoWithBoilerplate() {
  ASSIGN_OR_RETURN(int value, DoOperationAndProduce(42));
  // You can now use the value.
}
```

## Writing unit tests

The **testing/error_matchers.h** header file provides
[googletest](https://github.com/google/googletest) matchers useful when unit
testing functions that return the **error::Error** or
**error::ErrorOr\<valueT\>** types.

Two matchers are defined:

*   **IsOk()** - verifies that the actual **error::Error** contains the
    canonical code **error::Error::OK**.
*   **ErrorIs(expected)** - verifies that the actual **error::Error** matches
    the expected.
*   **IsOkAndHolds(inner_matcher)** - verifies that the actual
    **error::ErrorOr\<valueT\>** contains the canonical error code
    **error::Error::OK** and holds a value that matches the inner matcher.

Additionally two macros are defined:

*   **EXPECT_OK(expression)** - a shortcut for EXPECT_THAT(expression, IsOk()).
*   **ASSERT_OK(expression)** - a shortcut for ASSERT_THAT(expression, IsOk()).

### Using the IsOk and ErrorIs matchers

Assuming the above example of the **DoOperation()** function, one can write the
following unit test.

```c++
using error::Error;
using testing::error::ErrorIs;

TEST(DoOperationTest, Succeeds) {
  // The following expectations are equivalent.
  EXPECT_OK(DoOperation(42));
  EXPECT_THAT(DoOperation(42), IsOk());
  EXPECT_THAT(DoOperation(42), ErrorIs(Error::OK));

  // One can also write an assertion.
  ASSERT_OK(DoOperation(42));
}
```

### Using the IsOkAndHolds matcher

Assuming the above example of the **DoOperationAndProduce()** function, one can
write the following unit test.

```c++
using error::Error;
using testing::Eq;
using testing::error::IsOkAndHolds;

TEST(DoOperationAndProduceTest, ReturnsValue) {
  EXPECT_THAT(DoOperationAndProduce(42), IsOkAndHolds(42));
}
```

## More examples

Explore the unit tests files in this repository for more examples on how to use
the error objects, macros and matchers.
