
// C++ Standard Library
#include <optional>
#include <sstream>
#include <string>

// GTest
#include <gtest/gtest.h>

// Boost Archive JSON
#include <boost/archive/json_oarchive.h>

class json_oarchive_test_suite : public ::testing::Test
{
public:
  json_oarchive_test_suite() : buffer{}, ar{std::in_place, buffer, false} {}

  struct TestStruct
  {
    int m = 111;

    TestStruct() = default;

    template <typename ArchiveT> inline void serialize(ArchiveT& ar, const unsigned int file_version)
    {
      ar& BOOST_SERIALIZATION_NVP(m);
    }
  };

  struct NestedTestStruct
  {
    TestStruct first;
    TestStruct second;

    NestedTestStruct() = default;

    template <typename ArchiveT> inline void serialize(ArchiveT& ar, const unsigned int file_version)
    {
      ar& BOOST_SERIALIZATION_NVP(first);
      ar& BOOST_SERIALIZATION_NVP(second);
    }
  };

  void SetUp() override {}

  void TearDown() override {}

  std::ostringstream buffer;
  std::optional<boost::archive::json_oarchive> ar;
};

TEST_F(json_oarchive_test_suite, SerializeBool)
{
  const bool value = true;
  ASSERT_NO_THROW((*ar) & boost::serialization::make_nvp("bool", value));

  // Call destructor to flush to output stream
  ar.reset();

  static const char* SERIALIZED = "{\"bool\":true}";
  ASSERT_EQ(buffer.str(), SERIALIZED);
}

TEST_F(json_oarchive_test_suite, SerializeFloat)
{
  const float value = 123.0f;
  ASSERT_NO_THROW((*ar) & boost::serialization::make_nvp("float", value));

  // Call destructor to flush to output stream
  ar.reset();

  static const char* SERIALIZED = "{\"float\":123}";
  ASSERT_EQ(buffer.str(), SERIALIZED);
}

TEST_F(json_oarchive_test_suite, SerializeDouble)
{
  const double value = 123.456;
  ASSERT_NO_THROW((*ar) & boost::serialization::make_nvp("double", value));

  // Call destructor to flush to output stream
  ar.reset();

  static const char* SERIALIZED = "{\"double\":123.456}";
  ASSERT_EQ(buffer.str(), SERIALIZED);
}

TEST_F(json_oarchive_test_suite, SerializeInt)
{
  const int value = 99;
  ASSERT_NO_THROW((*ar) & boost::serialization::make_nvp("int", value));

  // Call destructor to flush to output stream
  ar.reset();

  static const char* SERIALIZED = "{\"int\":99}";
  ASSERT_EQ(buffer.str(), SERIALIZED);
}

TEST_F(json_oarchive_test_suite, SerializeString)
{
  const std::string value = "hello";
  ASSERT_NO_THROW((*ar) & boost::serialization::make_nvp("string", value));

  // Call destructor to flush to output stream
  ar.reset();

  static const char* SERIALIZED = "{\"string\":\"hello\"}";
  ASSERT_EQ(buffer.str(), SERIALIZED);
}

TEST_F(json_oarchive_test_suite, SerializeStruct)
{
  const TestStruct value;
  ASSERT_NO_THROW((*ar) & boost::serialization::make_nvp("struct", value));

  // Call destructor to flush to output stream
  ar.reset();

  // clang-format off
  static const char* SERIALIZED =
    "{"
      "\"struct\":{"
        "\"_class_id_optional\":0,"
        "\"_tracking\":false,"
        "\"_version\":0,"
        "\"m\":111"
      "}"
    "}";
  // clang-format on

  ASSERT_EQ(buffer.str(), SERIALIZED);
}

TEST_F(json_oarchive_test_suite, SerializeNestedStruct)
{
  const NestedTestStruct value;
  ASSERT_NO_THROW((*ar) & boost::serialization::make_nvp("nested_struct", value));

  // Call destructor to flush to output stream
  ar.reset();

  // clang-format off
  static const char* SERIALIZED =
    "{"
        "\"nested_struct\":{"
        "\"_class_id_optional\":0,"
        "\"_tracking\":false,"
        "\"_version\":0,"
        "\"first\":{"
          "\"_class_id_optional\":1,"
          "\"_tracking\":false,"
          "\"_version\":0,"
          "\"m\":111"
        "},"
        "\"second\":{"
          "\"m\":111"
        "}"
      "}"
    "}";
  // clang-format on

  ASSERT_EQ(buffer.str(), SERIALIZED);
}

TEST_F(json_oarchive_test_suite, SerializeBoolStdVector)
{
  std::vector<bool> bool_array_value{true, false, true, false};
  ASSERT_NO_THROW((*ar) & boost::serialization::make_nvp("bool_array", bool_array_value));

  // Call destructor to flush to output stream
  ar.reset();

  static const char* SERIALIZED = "{\"bool_array\":[true,false,true,false]}";

  ASSERT_EQ(buffer.str(), SERIALIZED);
}

TEST_F(json_oarchive_test_suite, SerializeIntStdVector)
{
  std::vector<int> int_array_value{1, 2, 3, 4};
  ASSERT_NO_THROW((*ar) & boost::serialization::make_nvp("int_array", int_array_value));

  // Call destructor to flush to output stream
  ar.reset();

  static const char* SERIALIZED = "{\"int_array\":[1,2,3,4]}";

  ASSERT_EQ(buffer.str(), SERIALIZED);
}

TEST_F(json_oarchive_test_suite, SerializeFloatStdVector)
{
  std::vector<float> float_array_value{1.0, 2.0, 3.0, 4.0};
  ASSERT_NO_THROW((*ar) & boost::serialization::make_nvp("float_array", float_array_value));

  // Call destructor to flush to output stream
  ar.reset();

  static const char* SERIALIZED = "{\"float_array\":[1,2,3,4]}";

  ASSERT_EQ(buffer.str(), SERIALIZED);
}

TEST_F(json_oarchive_test_suite, SerializeDoubleStdVector)
{
  std::vector<double> double_array_value{1.0, 2.0, 3.0, 4.0};
  ASSERT_NO_THROW((*ar) & boost::serialization::make_nvp("double_array", double_array_value));

  // Call destructor to flush to output stream
  ar.reset();

  static const char* SERIALIZED = "{\"double_array\":[1,2,3,4]}";

  ASSERT_EQ(buffer.str(), SERIALIZED);
}

TEST_F(json_oarchive_test_suite, SerializeStringStdVector)
{
  std::vector<std::string> string_array_value{"p", "i", "c", "o"};
  ASSERT_NO_THROW((*ar) & boost::serialization::make_nvp("string_array", string_array_value));

  // Call destructor to flush to output stream
  ar.reset();

  static const char* SERIALIZED = "{\"string_array\":[\"p\",\"i\",\"c\",\"o\"]}";

  ASSERT_EQ(buffer.str(), SERIALIZED);
}

TEST_F(json_oarchive_test_suite, SerializeStructStdVector)
{
  std::vector<TestStruct> struct_array_value{TestStruct{}, TestStruct{}};
  ((*ar) & boost::serialization::make_nvp("struct_array", struct_array_value));

  // Call destructor to flush to output stream
  ar.reset();

  // clang-format off
  static const char* SERIALIZED =
    "{"
      "\"struct_array\":["
        "{\"_class_id_optional\":0,\"_tracking\":false,\"_version\":0,\"m\":111},"
        "{\"m\":111}"
      "]"
    "}";
  // clang-format on

  ASSERT_EQ(buffer.str(), SERIALIZED);
}

TEST_F(json_oarchive_test_suite, SerializeBoolStdArray)
{
  std::array<bool, 4> bool_array_value{true, false, true, false};
  ASSERT_NO_THROW((*ar) & boost::serialization::make_nvp("bool_array", bool_array_value));

  // Call destructor to flush to output stream
  ar.reset();

  static const char* SERIALIZED = "{\"bool_array\":[true,false,true,false]}";

  ASSERT_EQ(buffer.str(), SERIALIZED);
}

TEST_F(json_oarchive_test_suite, SerializeIntStdArray)
{
  std::array<int, 4> int_array_value{1, 2, 3, 4};
  ASSERT_NO_THROW((*ar) & boost::serialization::make_nvp("int_array", int_array_value));

  // Call destructor to flush to output stream
  ar.reset();

  static const char* SERIALIZED = "{\"int_array\":[1,2,3,4]}";

  ASSERT_EQ(buffer.str(), SERIALIZED);
}

TEST_F(json_oarchive_test_suite, SerializeFloatStdArray)
{
  std::array<float, 4> float_array_value{1.0, 2.0, 3.0, 4.0};
  ASSERT_NO_THROW((*ar) & boost::serialization::make_nvp("float_array", float_array_value));

  // Call destructor to flush to output stream
  ar.reset();

  static const char* SERIALIZED = "{\"float_array\":[1,2,3,4]}";

  ASSERT_EQ(buffer.str(), SERIALIZED);
}

TEST_F(json_oarchive_test_suite, SerializeDoubleStdArray)
{
  std::array<double, 4> double_array_value{1.0, 2.0, 3.0, 4.0};
  ASSERT_NO_THROW((*ar) & boost::serialization::make_nvp("double_array", double_array_value));

  // Call destructor to flush to output stream
  ar.reset();

  static const char* SERIALIZED = "{\"double_array\":[1,2,3,4]}";

  ASSERT_EQ(buffer.str(), SERIALIZED);
}

TEST_F(json_oarchive_test_suite, SerializeStringStdArray)
{
  std::array<std::string, 4> string_array_value{"p", "i", "c", "o"};
  ASSERT_NO_THROW((*ar) & boost::serialization::make_nvp("string_array", string_array_value));

  // Call destructor to flush to output stream
  ar.reset();

  static const char* SERIALIZED = "{\"string_array\":[\"p\",\"i\",\"c\",\"o\"]}";

  ASSERT_EQ(buffer.str(), SERIALIZED);
}

TEST_F(json_oarchive_test_suite, SerializeStructStdArray)
{
  std::array<TestStruct, 2> struct_array_value{TestStruct{}, TestStruct{}};
  ((*ar) & boost::serialization::make_nvp("struct_array", struct_array_value));

  // Call destructor to flush to output stream
  ar.reset();

  // clang-format off
  static const char* SERIALIZED =
    "{"
      "\"struct_array\":["
        "{\"_class_id_optional\":0,\"_tracking\":false,\"_version\":0,\"m\":111},"
        "{\"m\":111}"
      "]"
    "}";
  // clang-format on

  ASSERT_EQ(buffer.str(), SERIALIZED);
}
