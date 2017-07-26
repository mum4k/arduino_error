# Copyright 2017 Google Inc.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

load("@platformio_rules//platformio:platformio.bzl", "platformio_library")

package(
    default_visibility = ["//visibility:public"],
)

cc_library(
    name = "error",
    srcs = ["error.cc"],
    hdrs = ["error.h"],
    defines = ["NATIVE_BUILD"],
)

cc_test(
    name = "error_test",
    srcs = ["error_test.cc"],
    deps = [
        ":error",
        "//external:gtest_main",
    ],
)

cc_library(
    name = "error_or",
    hdrs = ["error_or.h"],
    defines = ["NATIVE_BUILD"],
    deps = [
        ":error",
        "//external:gtest",
    ],
)

cc_test(
    name = "error_or_test",
    srcs = ["error_or_test.cc"],
    deps = [
        ":error_or",
        "//external:gtest_main",
    ],
)

cc_library(
    name = "error_macros",
    hdrs = ["error_macros.h"],
    defines = ["NATIVE_BUILD"],
)

cc_test(
    name = "error_macros_test",
    srcs = ["error_macros_test.cc"],
    deps = [
        ":error",
        ":error_macros",
        ":error_or",
        "//testing:error_matchers",
        "//external:gtest_main",
    ],
)

platformio_library(
    name = "Error",
    src = "error.cc",
    hdr = "error.h",
)

platformio_library(
    name = "Error_or",
    hdr = "error_or.h",
    deps = [
        ":Error",
    ],
)

platformio_library(
    name = "Error_macros",
    hdr = "error_macros.h",
)
