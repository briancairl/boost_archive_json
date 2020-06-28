workspace(name="boost_archive_json")

load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

# Boost
git_repository(
    name="com_github_nelhage_rules_boost",
    commit="9f9fb8b2f0213989247c9d5c0e814a8451d18d7f",
    remote="https://github.com/nelhage/rules_boost",
    shallow_since="1570056263 -0700",
)
load("@com_github_nelhage_rules_boost//:boost/boost.bzl", "boost_deps")
boost_deps()

# Picojson
http_archive(
    name="picojson",
    url="https://github.com/kazuho/picojson/archive/v1.3.0.zip",
    sha256="58fff34589d93b0cef208a456788b9de6c7d04cfa9a7be576328ca7b48f10069",
    build_file="//external:picojson.BUILD",
    strip_prefix="picojson-1.3.0"
)

# GTest/GMock
http_archive(
    name="googletest",
    url="https://github.com/google/googletest/archive/release-1.8.0.zip",
    sha256="f3ed3b58511efd272eb074a3a6d6fb79d7c2e6a0e374323d1e6bcbcc1ef141bf",
    build_file="//external:googletest.BUILD",
    strip_prefix="googletest-release-1.8.0",
)
