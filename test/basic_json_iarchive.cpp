
// C++ Standard Library
#include <optional>
#include <sstream>
#include <string>

// GTest
#include <gtest/gtest.h>

// Boost Archive JSON
#include <boost/archive/json_iarchive.h>

class json_iarchive_test_suite : public ::testing::Test
{
public:
  json_iarchive_test_suite() : buffer{} {}

  struct TestStruct
  {
    int m = 111;

    TestStruct() = default;

    template <typename ArchiveT> inline void serialize(ArchiveT& ar, const unsigned int file_version)
    {
      ar& BOOST_SERIALIZATION_NVP(m);
    }

    inline bool operator==(const TestStruct& other) const { return other.m == this->m; }
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

    inline bool operator==(const NestedTestStruct& other) const
    {
      return other.first == this->first and other.second == this->second;
    }
  };

  void create_iarchive(const char* serialized)
  {
    buffer << serialized;
    ar.emplace(buffer);
  }

  void SetUp() override {}

  void TearDown() override {}

  std::stringstream buffer;
  std::optional<boost::archive::json_iarchive> ar;
};

TEST_F(json_iarchive_test_suite, DeserializeThrowOnEmpty)
{
  static const char* SERIALIZED = "";
  this->create_iarchive(SERIALIZED);

  bool value;
  ASSERT_THROW(
    ((*ar) & boost::serialization::make_nvp("bool", value)),
    boost::archive::json_archive_exception
  );
}

TEST_F(json_iarchive_test_suite, DeserializeThrowOnMissing)
{
  static const char* SERIALIZED = "{\"bool\":true}";
  this->create_iarchive(SERIALIZED);

  bool value;
  ASSERT_THROW(
    ((*ar) & boost::serialization::make_nvp("not_bool", value)),
    boost::archive::json_archive_exception
  );
}


TEST_F(json_iarchive_test_suite, DeserializeBool)
{
  static const char* SERIALIZED = "{\"bool\":true}";
  this->create_iarchive(SERIALIZED);

  bool value;
  ((*ar) & boost::serialization::make_nvp("bool", value));

  ASSERT_EQ(value, true);
}

TEST_F(json_iarchive_test_suite, DeserializeFloat)
{
  static const char* SERIALIZED = "{\"float\":123}";
  this->create_iarchive(SERIALIZED);

  float value;
  ((*ar) & boost::serialization::make_nvp("float", value));

  ASSERT_EQ(value, 123.0f);
}

TEST_F(json_iarchive_test_suite, DeserializeDouble)
{
  static const char* SERIALIZED = "{\"double\":123.456}";
  this->create_iarchive(SERIALIZED);

  double value;
  ((*ar) & boost::serialization::make_nvp("double", value));

  ASSERT_EQ(value, 123.456);
}

TEST_F(json_iarchive_test_suite, DeserializeInt)
{
  static const char* SERIALIZED = "{\"int\":99}";
  this->create_iarchive(SERIALIZED);

  int value;
  ((*ar) & boost::serialization::make_nvp("int", value));

  ASSERT_EQ(value, 99);
}

TEST_F(json_iarchive_test_suite, DeserializeString)
{
  static const char* SERIALIZED = "{\"string\":\"hello\"}";
  this->create_iarchive(SERIALIZED);

  std::string value;
  ((*ar) & boost::serialization::make_nvp("string", value));

  ASSERT_EQ(value, "hello");
}

TEST_F(json_iarchive_test_suite, DeserializeStruct)
{
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
  this->create_iarchive(SERIALIZED);

  TestStruct value;
  ((*ar) & boost::serialization::make_nvp("struct", value));

  ASSERT_EQ(value, TestStruct{});
}

TEST_F(json_iarchive_test_suite, DeserializeNestedStruct)
{
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
  this->create_iarchive(SERIALIZED);

  NestedTestStruct value;
  ((*ar) & boost::serialization::make_nvp("nested_struct", value));

  ASSERT_EQ(value, NestedTestStruct{});
}

TEST_F(json_iarchive_test_suite, DeserializeBoolStdVector)
{
  static const char* SERIALIZED = "{\"bool_array\":[true,false,true,false]}";
  this->create_iarchive(SERIALIZED);

  std::vector<bool> value;
  ((*ar) & boost::serialization::make_nvp("bool_array", value));

  const std::vector<bool> bool_array_value_target{true, false, true, false};
  ASSERT_EQ(value, bool_array_value_target);
}

TEST_F(json_iarchive_test_suite, DeserializeIntStdVector)
{
  static const char* SERIALIZED = "{\"int_array\":[1,2,3,4]}";
  this->create_iarchive(SERIALIZED);

  std::vector<int> value;
  ((*ar) & boost::serialization::make_nvp("int_array", value));

  const std::vector<int> int_array_value_target{1, 2, 3, 4};
  ASSERT_EQ(value, int_array_value_target);
}

TEST_F(json_iarchive_test_suite, DeserializeFloatStdVector)
{
  static const char* SERIALIZED = "{\"float_array\":[1,2,3,4]}";
  this->create_iarchive(SERIALIZED);

  std::vector<float> value;
  ((*ar) & boost::serialization::make_nvp("float_array", value));

  const std::vector<float> float_array_value_target{1.0f, 2.0f, 3.0f, 4.0f};
  ASSERT_EQ(value, float_array_value_target);
}

TEST_F(json_iarchive_test_suite, DeserializeDoubleStdVector)
{
  static const char* SERIALIZED = "{\"double_array\":[1,2,3,4]}";
  this->create_iarchive(SERIALIZED);

  std::vector<double> value;
  ((*ar) & boost::serialization::make_nvp("double_array", value));

  const std::vector<double> double_array_value_target{1.0, 2.0, 3.0, 4.0};
  ASSERT_EQ(value, double_array_value_target);
}

TEST_F(json_iarchive_test_suite, DeserializeStringStdVector)
{
  static const char* SERIALIZED = "{\"string_array\":[\"p\",\"i\",\"c\",\"o\"]}";
  this->create_iarchive(SERIALIZED);

  std::vector<std::string> value;
  ((*ar) & boost::serialization::make_nvp("string_array", value));

  const std::vector<std::string> string_array_value_target{"p", "i", "c", "o"};
  ASSERT_EQ(value, string_array_value_target);
}

TEST_F(json_iarchive_test_suite, DeserializeStructStdVector)
{
  // clang-format off
  static const char* SERIALIZED =
    "{"
      "\"struct_array\":["
        "{\"_class_id_optional\":0,\"_tracking\":false,\"_version\":0,\"m\":111},"
        "{\"m\":111}"
      "]"
    "}";
  // clang-format on
  this->create_iarchive(SERIALIZED);

  std::vector<TestStruct> value;
  ((*ar) & boost::serialization::make_nvp("struct_array", value));

  const std::vector<TestStruct> struct_array_value_target{TestStruct{}, TestStruct{}};
  ASSERT_EQ(value, struct_array_value_target);
}

TEST_F(json_iarchive_test_suite, DeserializeBoolStdArray)
{
  static const char* SERIALIZED = "{\"bool_array\":[true,false,true,false]}";
  this->create_iarchive(SERIALIZED);

  std::array<bool, 4> value;
  ((*ar) & boost::serialization::make_nvp("bool_array", value));

  const std::array<bool, 4> bool_array_value_target{true, false, true, false};
  ASSERT_EQ(value, bool_array_value_target);
}

TEST_F(json_iarchive_test_suite, DeserializeIntStdArray)
{
  static const char* SERIALIZED = "{\"int_array\":[1,2,3,4]}";
  this->create_iarchive(SERIALIZED);

  std::array<int, 4> value;
  ((*ar) & boost::serialization::make_nvp("int_array", value));

  const std::array<int, 4> int_array_value_target{1, 2, 3, 4};
  ASSERT_EQ(value, int_array_value_target);
}

TEST_F(json_iarchive_test_suite, DeserializeFloatStdArray)
{
  static const char* SERIALIZED = "{\"float_array\":[1,2,3,4]}";
  this->create_iarchive(SERIALIZED);

  std::array<float, 4> value;
  ((*ar) & boost::serialization::make_nvp("float_array", value));

  const std::array<float, 4> float_array_value_target{1.0f, 2.0f, 3.0f, 4.0f};
  ASSERT_EQ(value, float_array_value_target);
}

TEST_F(json_iarchive_test_suite, DeserializeDoubleStdArray)
{
  static const char* SERIALIZED = "{\"double_array\":[1,2,3,4]}";
  this->create_iarchive(SERIALIZED);

  std::array<double, 4> value;
  ((*ar) & boost::serialization::make_nvp("double_array", value));

  const std::array<double, 4> double_array_value_target{1.0, 2.0, 3.0, 4.0};
  ASSERT_EQ(value, double_array_value_target);
}

TEST_F(json_iarchive_test_suite, DeserializeStringStdArray)
{
  static const char* SERIALIZED = "{\"string_array\":[\"p\",\"i\",\"c\",\"o\"]}";
  this->create_iarchive(SERIALIZED);

  std::array<std::string, 4> value;
  ((*ar) & boost::serialization::make_nvp("string_array", value));

  const std::array<std::string, 4> string_array_value_target{"p", "i", "c", "o"};
  ASSERT_EQ(value, string_array_value_target);
}

TEST_F(json_iarchive_test_suite, DeserializeStructStdArray)
{
  // clang-format off
  static const char* SERIALIZED =
    "{"
      "\"struct_array\":["
        "{\"_class_id_optional\":0,\"_tracking\":false,\"_version\":0,\"m\":111},"
        "{\"m\":111}"
      "]"
    "}";
  // clang-format on
  this->create_iarchive(SERIALIZED);

  std::array<TestStruct, 2> value;
  ((*ar) & boost::serialization::make_nvp("struct_array", value));

  const std::array<TestStruct, 2> struct_array_value_target{TestStruct{}, TestStruct{}};
  ASSERT_EQ(value, struct_array_value_target);
}
