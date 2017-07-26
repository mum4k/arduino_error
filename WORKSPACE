# Google testing and mocking framework for C++.
new_git_repository(
    name = "googletest",
    build_file = "external_deps/gmock.BUILD",
    remote = "https://github.com/google/googletest",
    tag = "release-1.8.0",
)

bind(
    name = "gtest",
    actual = "@googletest//:gtest",
)

bind(
    name = "gtest_main",
    actual = "@googletest//:gtest_main",
)

# PlatformIO Bazel rules.
git_repository(
    name = "platformio_rules",
    remote = "https://github.com/mum4k/platformio_rules.git",
    tag = "v0.0.1",
)
