// Boost
#include <boost/archive/detail/archive_serializer_map.hpp>
#include <boost/archive/impl/archive_serializer_map.ipp>
#include <boost/config.hpp>
#include <boost/detail/workaround.hpp>

// Boost Archive JSON
#include <boost/archive/json_iarchive.h>

namespace boost
{
namespace archive
{

json_iarchive::json_iarchive(std::istream& is) : json_{
  [&is] {
    picojson::value json;
    picojson::parse(json, is);
    return json;
  }()}
{}

template class detail::archive_serializer_map<json_iarchive>;

}  // namespace archive
}  // namespace boost
