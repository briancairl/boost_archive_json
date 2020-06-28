#ifndef BOOST_ARCHIVE_JSON_OARCHIVE_H
#define BOOST_ARCHIVE_JSON_OARCHIVE_H

// C++ Standard Library
#include <algorithm>
#include <functional>
#include <iterator>
#include <ostream>
#include <string>
#include <type_traits>

// Boost
#include <boost/archive/detail/common_oarchive.hpp>
#include <boost/archive/detail/register_archive.hpp>

// Boost Archive JSON
#include <boost/archive/detail/abi_prefix.hpp>
#include <boost/archive/picojson_wrapper.h>
#include <boost/serialization/array.hpp>
#include <boost/serialization/item_version_type.hpp>
#include <boost/serialization/nvp.hpp>

namespace boost
{
namespace archive
{

class json_oarchive : public picojson_wrapper, public detail::common_oarchive<json_oarchive>
{
public:
  explicit json_oarchive(std::ostream& os, const bool prettify = false);

  ~json_oarchive();

  inline void save_start(const char* tag) { picojson_wrapper::ctx_start(tag); }

  inline void save_end(const char* tag) { picojson_wrapper::ctx_end(tag); }

  template <typename T> void save_override(const boost::serialization::nvp<T>& kv)
  {
    picojson_wrapper::ctx_start(kv.name());
    picojson_wrapper::object_start();
    this->save(kv.const_value());
    picojson_wrapper::object_end();
    picojson_wrapper::ctx_end(kv.name());
  }

  template <typename T>
  std::enable_if_t<fusion::result_of::has_key<meta_type_conversions, T>::type::value> save_override(const T& value)
  {
    save(value);
  }

  template <typename T> void save(const T& value)
  {
    if constexpr (fusion::result_of::has_key<picojson_native_types, T>::type::value)
    {
      picojson_wrapper::active() = picojson::value{value};
    }
    else if constexpr (fusion::result_of::has_key<picojson_conversions, T>::type::value)
    {
      using cast_type = typename fusion::result_of::value_at_key<picojson_conversions, T>::type;
      picojson_wrapper::active() = picojson::value{static_cast<cast_type>(value)};
    }
    else if constexpr (fusion::result_of::has_key<meta_type_conversions, T>::type::value)
    {
      using cast_type = typename fusion::result_of::value_at_key<meta_type_conversions, T>::type;
      auto cast_value = static_cast<cast_type>(value);
      save_override(boost::serialization::make_nvp(fusion::at_key<T>(meta_type_names), cast_value));
    }
    else if constexpr (detail::is_array_like<T>::value)
    {
      picojson_wrapper::array_start(std::distance(std::begin(value), std::end(value)));

      for (const auto& element : value)
      {
        picojson_wrapper::array_push();
        if constexpr (!detail::is_element_native_convertible<T>::value)
        {
          picojson_wrapper::object_start();
        }
        this->save(element);
        picojson_wrapper::object_end();
      }

      picojson_wrapper::array_end();
    }
    else
    {
      detail::common_oarchive<json_oarchive>::save_override(value);
    }
  }

private:
  std::reference_wrapper<std::ostream> os_;
  bool prettify_;
};

}  // archive
}  // boost

BOOST_SERIALIZATION_REGISTER_ARCHIVE(boost::archive::json_oarchive)

#endif  // BOOST_ARCHIVE_JSON_IARCHIVE_H
