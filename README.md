# `boost::archive::json*`

[![Build Status](https://travis-ci.com/briancairl/boost_archive_json.svg?branch=master)](https://travis-ci.com/briancairl/boost_archive_json)

## Background

This library provides JSON input/output archive objects usable with [boost serialization](https://www.boost.org/doc/libs/1_71_0/libs/serialization/doc/serialization.html) module.

It was (heavily) inspired by [boost_mongo](https://github.com/ignatz/boost_mongo), and essentially serves as stripped-back version for the isolated human-readable JSON serialization/de-serialization use case with minimal external dependencies. As such, the `mongo` C++ client library is not required.

Instead, this library makes use of the header-only [picojson](https://github.com/kazuho/picojson/blob/master/picojson.h) library as a backbone for serialization. This is a outstandingly easy to used C++ JSON library in its own right.


## Usage


### `boost::archive::json_oarchive`

```c++
// C++ Standard Library
#include <fstream>  // or your favorite std::ostream derivative (e.g. std::cout)

// Boost Archive JSON
#include <boost/archive/json_oarchive.h>
...

struct Serializable
{
  int m = 111;

  Serializable() = default;

  template<typename ArchiveT>
  inline void serialize(ArchiveT& ar, const unsigned int file_version)
  {
    ar & BOOST_SERIALIZATION_NVP(m);

    // same as
    //
    // ar & boost::serialization::make_nvp("m", m);
  }
};

...

std::ofstream ofs{"serialized.json"};

boost::archive::json_oarchive ar{ofs};

Serializable object{};

ar & BOOST_SERIALIZATION_NVP(object);

// same as:
//
//   ar & boost::serialization::make_nvp("object", object);

/// contents of `ar` are flushed to `ofs` when `ar` is destroyed
```


### `boost::archive::json_iarchive`

```c++
// C++ Standard Library
#include <fstream>  // or your favorite std::istream derivative (e.g. std::cout)

// Boost Archive JSON
#include <boost/archive/json_iarchive.h>

...

struct Serializable
{
  int m = 111;

  Serializable() = default;

  template<typename ArchiveT>
  inline void serialize(ArchiveT& ar, const unsigned int file_version)
  {
    ar & BOOST_SERIALIZATION_NVP(m);

    // same as
    //
    // ar & boost::serialization::make_nvp("m", m);
  }
};

...

std::ofstream ifs{"serialized.json"};

boost::archive::json_iarchive ar{ifs};

Serializable object;

ar & BOOST_SERIALIZATION_NVP(object);

// object is now filled with data from deserialized JSON if there weren't any exceptions!

// same as:
//
//   ar & boost::serialization::make_nvp("object", object);
```

## Running unit tests

From repository root
```
bazel test ... --test_output=all

```


## Requirements
- C++17
- [boost](https://www.boost.org/)
- [gtest](https://github.com/google/googletest) [tests only]
- [bazel](https://bazel.build/)


## Notes

### Legacy C++ support

This library is written in C++17, but usage of syntax/STL features is fairly limited. Namely,
there is liberal usage of `if constexpr` over the older `enable_if` tricks for clarity.

If there is a demonstrated need for C++11/14 support, getting things in shape would be very doable


### Build System Requirements

I've been using [bazel](https://bazel.build/) for most of my personal projects, though I understand its not the prevailing "go-to" for most people.
A CMake build should be easy to concoct as well for anyway who wants to do it. Though I have no plans to make one, myself, it would be an appreciated contribution.
