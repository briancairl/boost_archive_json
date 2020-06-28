// C++ Standard Library
#include <algorithm>
#include <iterator>

// Boost
#include <boost/archive/detail/archive_serializer_map.hpp>
#include <boost/archive/impl/archive_serializer_map.ipp>

// Boost Archive JSON
#include <boost/archive/json_oarchive.h>

namespace boost
{
namespace archive
{

json_oarchive::json_oarchive(std::ostream& os, const bool prettify) : picojson_wrapper{}, os_{os}, prettify_{prettify}
{}

json_oarchive::~json_oarchive() { picojson_wrapper::root_.serialize(std::ostream_iterator<char>(os_), prettify_); }

template class detail::archive_serializer_map<json_oarchive>;

}  // namespace archive
}  // namespace boost
