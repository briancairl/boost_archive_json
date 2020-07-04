#ifndef BOOST_ARCHIVE_JSON_OARCHIVE_H
#define BOOST_ARCHIVE_JSON_OARCHIVE_H

// C++ Standard Library
#include <iterator>
#include <ostream>
#include <sstream>
#include <type_traits>

// Boost
#include <boost/archive/detail/common_oarchive.hpp>
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

class json_oarchive : public detail::common_oarchive<json_oarchive>
{
public:
  explicit json_oarchive(std::ostream& os, const bool prettify = false);

  ~json_oarchive();

  inline void save_start(const char* tag) { json_.ctx_start(tag); }

  inline void save_end(const char* tag) { json_.ctx_end(tag); }

  template <typename T> void save_override(const boost::serialization::nvp<T>& kv)
  {
    try
    {
      json_.ctx_start(kv.name());
      json_.object_start();
      this->save(kv.const_value());
      json_.object_end();
      json_.ctx_end(kv.name());
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
  std::enable_if_t<fusion::result_of::has_key<meta_type_conversions, T>::type::value> save_override(const T& value)
  {
    save(value);
  }

  template <typename T>
  std::enable_if_t<!fusion::result_of::has_key<meta_type_conversions, T>::type::value> save_override(T& value)
  {
    detail::common_oarchive<json_oarchive>::save_override(value);
  }

  template <typename T> void save(const T& value)
  {
    if constexpr (fusion::result_of::has_key<picojson_native_types, T>::type::value)
    {
      json_.active() = picojson::value{value};
    }
    else if constexpr (fusion::result_of::has_key<picojson_conversions, T>::type::value)
    {
      using cast_type = typename fusion::result_of::value_at_key<picojson_conversions, T>::type;
      json_.active() = picojson::value{static_cast<cast_type>(value)};
    }
    else if constexpr (fusion::result_of::has_key<meta_type_conversions, T>::type::value)
    {
      using cast_type = typename fusion::result_of::value_at_key<meta_type_conversions, T>::type;
      auto cast_value = static_cast<cast_type>(value);
      save_override(boost::serialization::make_nvp(fusion::at_key<T>(meta_type_names), cast_value));
    }
    else if constexpr (detail::is_std_vector<T>::value or detail::is_fixed_size_array<T>::value)
    {
      json_.array_start(std::distance(std::begin(value), std::end(value)));

      for (const auto& element : value)
      {
        json_.array_push();
        if constexpr (!detail::is_element_native_convertible<T>::value)
        {
          json_.object_start();
        }
        this->save(element);
        json_.object_end();
      }

      json_.array_end();
    }
    else
    {
      detail::common_oarchive<json_oarchive>::save_override(value);
    }
  }

private:
  picojson_wrapper json_;
  std::ostream* os_;
  bool prettify_;
};

}  // archive
}  // boost

BOOST_SERIALIZATION_REGISTER_ARCHIVE(boost::archive::json_oarchive)

#endif  // BOOST_ARCHIVE_JSON_IARCHIVE_H
