#ifndef BOOST_ARCHIVE_PICOJSON_WRAPPER_H
#define BOOST_ARCHIVE_PICOJSON_WRAPPER_H

// C++ Standard Library
#include <istream>
#include <stack>
#include <stdexcept>
#include <type_traits>

// Boost
#include <boost/archive/basic_archive.hpp>
#include <boost/fusion/container/map.hpp>
#include <boost/fusion/container/set.hpp>
#include <boost/fusion/include/at_key.hpp>
#include <boost/fusion/include/has_key.hpp>
#include <boost/fusion/include/value_at_key.hpp>
#include <boost/serialization/collection_size_type.hpp>
#include <boost/serialization/item_version_type.hpp>

// Picojson
#include <picojson/picojson.h>

// Forward Declarations
namespace std
{

template <class T, std::size_t N> class array;
template <class T, class AllocatorT> class vector;

}  // namespace std

namespace boost
{
namespace archive
{

using picojson_real_number_type = double;

class picojson_wrapper
{
public:
  explicit picojson_wrapper(std::istream& is);

  picojson_wrapper();

  ~picojson_wrapper() = default;

protected:
  void ctx_start(const char* tag);

  void ctx_end(const char* tag);

  void ctx_push(picojson::value& value);

  void ctx_pop();

  void object_start();

  constexpr void object_end() const {}

  void array_start(const std::size_t reserve);

  constexpr void array_end() const {}

  void array_push();

  void array_next();

  picojson::value& active();

  std::stack<picojson::value*> ctx_stack_;
  picojson::value root_;
};

using picojson_native_types = fusion::set<bool, picojson_real_number_type, std::string>;

using picojson_conversions = fusion::map<
  fusion::pair<short, picojson_real_number_type>,
  fusion::pair<unsigned short, picojson_real_number_type>,
  fusion::pair<int, picojson_real_number_type>,
  fusion::pair<unsigned int, picojson_real_number_type>,
  fusion::pair<long, picojson_real_number_type>,
  fusion::pair<unsigned long, picojson_real_number_type>,
  fusion::pair<signed char, picojson_real_number_type>,
  fusion::pair<unsigned char, picojson_real_number_type>,
  fusion::pair<wchar_t, picojson_real_number_type>,
  fusion::pair<float, picojson_real_number_type>>;

using meta_type_conversions = fusion::map<
  fusion::pair<archive::class_id_type, picojson_real_number_type>,
  fusion::pair<archive::class_id_optional_type, picojson_real_number_type>,
  fusion::pair<archive::class_id_reference_type, picojson_real_number_type>,
  fusion::pair<archive::object_id_type, picojson_real_number_type>,
  fusion::pair<archive::object_reference_type, picojson_real_number_type>,
  fusion::pair<archive::version_type, picojson_real_number_type>,
  fusion::pair<archive::tracking_type, bool>,
  fusion::pair<archive::class_name_type, std::string>>;

using meta_type_names_t = fusion::map<
  fusion::pair<archive::class_id_type, char const*>,
  fusion::pair<archive::class_id_optional_type, char const*>,
  fusion::pair<archive::class_id_reference_type, char const*>,
  fusion::pair<archive::object_id_type, char const*>,
  fusion::pair<archive::object_reference_type, char const*>,
  fusion::pair<archive::version_type, char const*>,
  fusion::pair<archive::tracking_type, char const*>,
  fusion::pair<archive::class_name_type, char const*>>;

const meta_type_names_t meta_type_names{fusion::make_pair<archive::class_id_type>("_class_id"),
                                        fusion::make_pair<archive::class_id_optional_type>("_class_id_optional"),
                                        fusion::make_pair<archive::class_id_reference_type>("_class_id_reference"),
                                        fusion::make_pair<archive::object_id_type>("_object_id"),
                                        fusion::make_pair<archive::object_reference_type>("_object_reference"),
                                        fusion::make_pair<archive::version_type>("_version"),
                                        fusion::make_pair<archive::tracking_type>("_tracking"),
                                        fusion::make_pair<archive::class_name_type>("_class_name")};

class json_archive_exception final : public std::exception
{
public:
  explicit json_archive_exception(std::string reason) : reason_{std::move(reason)} {}

  const char* what() const noexcept override { return reason_.c_str(); };

private:
  std::string reason_;
};

namespace detail
{

template <typename T> struct is_std_vector_bool : std::integral_constant<bool, false>
{};

template <typename... OtherTs>
struct is_std_vector_bool<std::vector<bool, OtherTs...>> : std::integral_constant<bool, true>
{};

template <typename T> struct is_fixed_size_array : std::integral_constant<bool, std::is_array<T>::value>
{};

template <typename T, std::size_t N> struct is_fixed_size_array<std::array<T, N>> : std::integral_constant<bool, true>
{};

template <typename T> struct is_std_vector : std::integral_constant<bool, false>
{};

template <typename T, typename... OtherTs>
struct is_std_vector<std::vector<T, OtherTs...>> : std::integral_constant<bool, true>
{};

template <typename T> struct is_element_native_convertible : std::integral_constant<bool, false>
{};

template <typename T, std::size_t N>
struct is_element_native_convertible<T[N]> : std::integral_constant<
                                               bool,
                                               (fusion::result_of::has_key<picojson_native_types, T>::type::value or
                                                fusion::result_of::has_key<picojson_conversions, T>::type::value)>
{};

template <typename T, std::size_t N>
struct is_element_native_convertible<std::array<T, N>>
    : std::integral_constant<
        bool,
        (fusion::result_of::has_key<picojson_native_types, T>::type::value or
         fusion::result_of::has_key<picojson_conversions, T>::type::value)>
{};

template <typename T, typename... OtherTs>
struct is_element_native_convertible<std::vector<T, OtherTs...>>
    : std::integral_constant<
        bool,
        (fusion::result_of::has_key<picojson_native_types, T>::type::value or
         fusion::result_of::has_key<picojson_conversions, T>::type::value)>
{};

}  // namespace detail

}  // archive
}  // boost

#endif  // BOOST_ARCHIVE_PICOJSON_WRAPPER_H
