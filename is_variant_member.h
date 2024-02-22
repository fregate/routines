#include <type_traits>
#include <variant>

template <typename T, typename V>
struct is_variant_member;

template <typename T, typename... Vars>
struct is_variant_member<T, std::variant<Vars...>> : public std::disjunction<std::is_same<T, Vars>...>
{
};

/* usage
struct EnableVariantCtor
{
  using DataV = std::variant<int8_t, int16_t, int32_t, int64_t, double, std::string>;
  DataV data;

  // this ctor will use only for int8_t, int16_t, int32_t, int64_t, double and std::string
  template <typename T, typename = std::enable_if<is_variant_member<T, DataV>::value, T>::type>
  explicit EnableVariantCtor(T d)
    : data{std::move(d)}
  {}

  // and this class could have other ctors:
  EnableVariantCtor(char* str, size_t len)
    : data{std::string(str, len)}
  {}

  EnableVariantCtor(std::string_view str)
    : data{std::string(str.begin(), str.end())}
  {
  }
};

int main(int argc, const char * argv[])
{
  auto check_int64 = std::make_shared<EnableVariantCtor>(static_cast<int64_t>(123));
  auto check_string = std::make_shared<EnableVariantCtor>(std::string{"123"});
  auto check_view = std::make_shared<EnableVariantCtor>("123");
  return 0;
}
*/
