#ifndef SMART_NUMERIC_CASTS
#define SMART_NUMERIC_CASTS

#include <stdexcept>


#ifndef SIZE_MAX
#define SIZE_MAX 0xffffffff
#endif

#ifndef INT_MAX
#define INT_MAX 2147483647
#endif

template <class FromType, class To_Type>
class _smart_cast_base
{
protected:
	const FromType v;
	inline _smart_cast_base(_smart_cast_base const& _v) = delete;

	explicit inline _smart_cast_base(To_Type _v) : v(_v)
	{}
public:
	explicit inline _smart_cast_base(FromType _v) : v(_v)
	{}
};

class to_signed : public _smart_cast_base<size_t, int>
{
public:
	using _smart_cast_base::_smart_cast_base;

	inline operator const int() const {
#ifdef _DEBUG
		if (static_cast<const int>(v) < 0)
			throw std::overflow_error(
				"unsigned int too large to convert to signed");
#endif
		return static_cast<const int>(v);
	}
};

class to_unsigned : public _smart_cast_base<int, size_t>
{
public:
	using _smart_cast_base::_smart_cast_base;

	inline operator const size_t() const {
#ifdef _DEBUG
		if (v < 0)
			throw std::overflow_error(
				"initialisation by negative integer");
#endif
		return static_cast<const size_t>(v);
	}
};

class trunc_to_int : public _smart_cast_base<double, int>
{
public:
	using _smart_cast_base::_smart_cast_base;

	inline operator const int() const {
#ifdef _DEBUG
		if (v > INT_MAX || v < -INT_MAX)
			throw std::overflow_error(
				"double too large to convert to int");
#endif
		return static_cast<const int>(v);
	}
};

class round_to_int : public _smart_cast_base<double, int>
{
public:
	using _smart_cast_base::_smart_cast_base;

	inline operator const int() const {
#ifdef _DEBUG
		if (v > INT_MAX || v < -INT_MAX)
			throw std::overflow_error(
				"double too large to convert to int");
#endif
		return static_cast<const int>((v < 0) ? v - 0.5 : v + 0.5);
	}
};

class trunc_to_unsigned : public _smart_cast_base<double, size_t>
{
public:
	using _smart_cast_base::_smart_cast_base;

	inline operator const size_t() const {
#ifdef _DEBUG
		if (v > SIZE_MAX || v + 0.5 < 0)
			throw std::overflow_error(
				"double is out of range of size_t");
#endif
		return static_cast<const size_t>(v);
	}
};
class round_to_unsigned : public _smart_cast_base<double, size_t>
{
public:
	using _smart_cast_base::_smart_cast_base;

	inline operator const size_t() const {
#ifdef _DEBUG
		if (v > SIZE_MAX || v + 0.5 < 0)
			throw std::overflow_error(
				"double is out of range of size_t");
#endif
		return static_cast<const size_t>(v + 0.5);
	}
};

class to_real : public _smart_cast_base<int, double>
{
public:
	using _smart_cast_base::_smart_cast_base;

	inline operator const double() const {
		return static_cast<const double>(v);
	}
};

class unsigned_to_real : public _smart_cast_base<size_t, double>
{
public:
	using _smart_cast_base::_smart_cast_base;

	inline operator const double() const {
		return static_cast<const double>(v);
	}
};

#endif//SMART_NUMERIC_CASTS