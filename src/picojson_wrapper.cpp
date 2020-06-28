// C++ Standard Library
#include <stdexcept>

// Boost Archive JSON
#include <boost/archive/picojson_wrapper.h>

namespace boost
{
namespace archive
{

picojson_wrapper::picojson_wrapper(std::istream& is) :
    root_{[&is] {
      picojson::value json;
      picojson::parse(json, is);
      return json;
    }()}
{
  ctx_push(root_);
}

picojson_wrapper::picojson_wrapper() : root_{picojson::object{}} { ctx_push(root_); }

void picojson_wrapper::ctx_start(const char* tag)
{
  if (!active().is<picojson::object>())
  {
    throw std::logic_error{"Context must start from 'picojson::object' state"};
  }

  auto& ctx = active().get<picojson::object>();

  auto retval = ctx.emplace(std::piecewise_construct, std::forward_as_tuple(tag), std::forward_as_tuple());

  ctx_push(retval.first->second);
}

void picojson_wrapper::ctx_end(const char* tag) { ctx_pop(); }

void picojson_wrapper::ctx_push(picojson::value& value) { ctx_stack_.emplace(std::addressof(value)); }

void picojson_wrapper::ctx_pop() { ctx_stack_.pop(); }

void picojson_wrapper::array_start(const std::size_t reserve)
{
  active() = picojson::value{picojson::array{}};
  auto& arr_ctx = active().get<picojson::array>();
  arr_ctx.reserve(reserve);
  ctx_stack_.emplace(nullptr);
}

void picojson_wrapper::array_push()
{
  ctx_pop();
  auto& arr_ctx = active().get<picojson::array>();
  arr_ctx.emplace_back();
  ctx_push(arr_ctx.back());
}

void picojson_wrapper::array_next() { ctx_pop(); }

void picojson_wrapper::object_start() { active() = picojson::value{picojson::object{}}; }

picojson::value& picojson_wrapper::active()
{
  if (ctx_stack_.empty())
  {
    throw std::logic_error{"JSON value stack is empty"};
  }
  else if (ctx_stack_.top() == nullptr)
  {
    throw std::logic_error{"Forgot to call `picojson_wrapper::array_push`"};
  }
  return *ctx_stack_.top();
}

}  // namespace archive
}  // namespace boost
