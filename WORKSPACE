# Drop-in replacement for deprecated git_repository.
load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")

# Google testing and mocking framework for C++.
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
http_archive(
    name = "com_google_googletest",
    urls = ["https://github.com/google/googletest/archive/release-1.8.1.zip"],
    strip_prefix = "googletest-release-1.8.1",
    sha256 = "927827c183d01734cc5cfef85e0ff3f5a92ffe6188e0d18e909c5efebf28a0c7",
)

# PlatformIO Bazel rules.
git_repository(
    name = "platformio_rules",
    remote = "https://github.com/mum4k/platformio_rules.git",
    tag = "v0.0.3",
)
