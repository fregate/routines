#include <algorithm>
#include <initializer_list>

// requires C++20

namespace cmp {

template <typename _Type>
struct operand
{
	operand(std::initializer_list<_Type> && p)
		: params_(std::move(p))
	{
	}

	virtual bool check(_Type val) const = 0;

	const std::initializer_list<_Type> params_;
};

template <typename T, class U>
concept Derived = std::is_base_of<U, T>::value;

template <typename _Type>
struct in : public operand<_Type>
{
	using operand<_Type>::params_;

	in(std::initializer_list<_Type> list)
		: operand<_Type>(std::move(list))
	{
	}

	bool check(_Type val) const override { return std::find(params_.begin(), params_.end(), val) != params_.end(); }
};

template <typename _Type>
struct not_in : public operand<_Type>
{
	using operand<_Type>::params_;

	not_in(std::initializer_list<_Type> list)
		: operand<_Type>(std::move(list))
	{
	}

	bool check(_Type val) const override { return std::find(params_.begin(), params_.end(), val) == params_.end(); }
};

template <typename _Type, Derived<operand<_Type>> _Op>
struct is
{
	is(_Type v, _Op op)
		: v{v}
		, op{std::move(op)} {};

	_Type v;
	_Op op;

	operator bool() { return op.check(v); }
};

} // namespace cmp

// usage:
// if (is(123, in{1, 2, 3, 123, 100})) {}
