#ifndef BOOST_ARCHIVE_JSON_IARCHIVE_H
#define BOOST_ARCHIVE_JSON_IARCHIVE_H

// C++ Standard Library
#include <iterator>
#include <istream>
#include <sstream>

// Boost
#include <boost/archive/detail/register_archive.hpp>

// Boost
#include <boost/archive/detail/common_iarchive.hpp>
#include <boost/archive/detail/register_archive.hpp>

// Boost Archive JSON
#include <boost/archive/picojson_wrapper.h>
#include <boost/serialization/array.hpp>
#include <boost/serialization/item_version_type.hpp>
#include <boost/serialization/nvp.hpp>

namespace boost
{
namespace archive
{

class json_iarchive : public picojson_wrapper, public detail::common_iarchive<json_iarchive>
{
public:
  explicit json_iarchive(std::istream& is);

  ~json_iarchive() = default;

  inline void load_start(const char* tag) { picojson_wrapper::ctx_start(tag); }

  inline void load_end(const char* tag) { picojson_wrapper::ctx_end(tag); }

  template <typename T> void load_override(const boost::serialization::nvp<T>& kv)
  {
    try
    {
      picojson_wrapper::ctx_start(kv.name());
      this->load(kv.value());
      picojson_wrapper::ctx_end(kv.name());
    }
    catch (const std::runtime_error& err)
    {
      std::ostringstream oss;
      oss << '[' << kv.name() << "] : " << err.what();
      throw json_archive_exception{oss.str()};
    }
    // ctx_start --> std::logic_error intentionally not caught
  }

  template <typename T>
  std::enable_if_t<fusion::result_of::has_key<meta_type_conversions, T>::type::value> load_override(T& value)
  {
    load(value);
  }

  template <typename T>
  std::enable_if_t<!fusion::result_of::has_key<meta_type_conversions, T>::type::value> load_override(T& value)
  {
    detail::common_iarchive<json_iarchive>::load_override(value);
  }

  template <typename T> void load(T& value)
  {
    if constexpr (fusion::result_of::has_key<picojson_native_types, T>::type::value)
    {
      value = picojson_wrapper::active().get<T>();
    }
    else if constexpr (fusion::result_of::has_key<picojson_conversions, T>::type::value)
    {
      using load_type = typename fusion::result_of::value_at_key<picojson_conversions, T>::type;
      value = static_cast<T>(picojson_wrapper::active().template get<load_type>());
    }
    else if constexpr (std::is_same<class_name_type, T>::value)
    {
      value = class_name_type{picojson_wrapper::active().template get<std::string>().c_str()};
    }
    else if constexpr (std::is_same<serialization::collection_size_type, T>::value)
    {
      value = static_cast<std::size_t>(picojson_wrapper::active().template get<picojson_real_number_type>());
    }
    else if constexpr (fusion::result_of::has_key<meta_type_conversions, T>::type::value)
    {
      /// ???
    }
    else if constexpr (detail::is_fixed_size_array<T>::value)
    {
      auto& read_value_array = picojson_wrapper::active().get<picojson::array>();
      auto witr = std::begin(value);

      for (auto& v : read_value_array)
      {
        picojson_wrapper::ctx_push(v);
        this->load(*witr++);
        picojson_wrapper::ctx_pop();
      }
    }
    else if constexpr (detail::is_std_vector_bool<T>::value)
    {
      auto& read_value_array = picojson_wrapper::active().get<picojson::array>();
      value.reserve(read_value_array.size());

      for (auto& v : read_value_array)
      {
        picojson_wrapper::ctx_push(v);
        bool dst;
        this->load(dst);
        value.push_back(dst);
        picojson_wrapper::ctx_pop();
      }
    }
    else if constexpr (detail::is_std_vector<T>::value)
    {
      auto& read_value_array = picojson_wrapper::active().get<picojson::array>();
      value.resize(read_value_array.size());

      auto witr = value.begin();
      for (auto& v : read_value_array)
      {
        picojson_wrapper::ctx_push(v);
        this->load(*witr++);
        picojson_wrapper::ctx_pop();
      }
    }
    else
    {
      detail::common_iarchive<json_iarchive>::load_override(value);
    }
  }
};

}  // archive
}  // boost

BOOST_SERIALIZATION_REGISTER_ARCHIVE(boost::archive::json_iarchive)

#endif  // BOOST_ARCHIVE_JSON_IARCHIVE_H