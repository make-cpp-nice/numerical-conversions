#pragma once

#ifndef XNR_CONVERSIONS_H
#define XNR_CONVERSIONS_H

#ifndef XNR_CONVERSION_OVERFLOW_CHECKING
	#ifdef _DEBUG
		#define XNR_CONVERSION_OVERFLOW_CHECKING 1
	#else
		#define XNR_CONVERSION_OVERFLOW_CHECKING 0
	#endif
#endif

/***************************************************************************/
//  to_signed
/***************************************************************************/
template <class FixDest, class FixSrc = std::false_type, class DeducedSrc,
	class = std::enable_if_t<
	(std::is_same_v<FixSrc, std::false_type>
		|| std::is_same_v<FixSrc, DeducedSrc>)
	&&	std::is_signed_v<FixDest> && std::is_unsigned_v<DeducedSrc>
	>
>
constexpr auto to_signed(DeducedSrc arg)
{
	const FixDest ts = static_cast<FixDest>(arg);
#if XNR_CONVERSION_OVERFLOW_CHECKING > 0
	if(ts!=arg)
		throw std::overflow_error("conversion to signed overflow");
#endif
	return ts;
	
}

template <int = 0, class DeducedSrc, 
	class = std::enable_if_t<std::is_unsigned_v<DeducedSrc>>
>
constexpr auto to_signed(DeducedSrc arg)
{
	const auto ts = static_cast<std::make_signed_t<DeducedSrc>>(arg);
#if XNR_CONVERSION_OVERFLOW_CHECKING > 0
	if (ts < 0)
		throw std::overflow_error("conversion to signed overflow");
#endif	
	return ts;
}


/***************************************************************************/
//  to_signed_cast
/***************************************************************************/
template <class FixDest, class FixSrc = std::false_type, class DeducedSrc,
	class = std::enable_if_t<
	(std::is_same_v<FixSrc, std::false_type>
		|| std::is_same_v<FixSrc, DeducedSrc>)
	&& std::is_signed_v<FixDest>&& std::is_unsigned_v<DeducedSrc>
	>
>
constexpr auto to_signed_cast(DeducedSrc arg)
{
	return static_cast<FixDest>(arg);
}

template <int = 0, class DeducedSrc,
	class = std::enable_if_t<std::is_unsigned_v<DeducedSrc>>
>
constexpr auto to_signed_cast(DeducedSrc arg)
{
	return static_cast<std::make_signed_t<DeducedSrc>>(arg);
}


/***************************************************************************/
//  to_unsigned
/***************************************************************************/
template <class FixDest, class FixSrc = std::false_type, class DeducedSrc,
	class = std::enable_if_t<
	(std::is_same_v<FixSrc, std::false_type>
		|| std::is_same_v<FixSrc, DeducedSrc>)
	&& std::is_signed_v<DeducedSrc>
	>
>
constexpr auto to_unsigned(DeducedSrc arg)
{
	FixDest tus = static_cast<std::make_unsigned_t<FixDest>>(arg);
#if XNR_CONVERSION_OVERFLOW_CHECKING > 0
	if(tus != arg)
		throw std::overflow_error("signed to unsigned overflow");
#endif
	return tus;
}

template <int = 0, class DeducedSrc,
	class = std::enable_if_t<std::is_signed_v<DeducedSrc>>
>
constexpr auto to_unsigned(DeducedSrc arg)
{
#if XNR_CONVERSION_OVERFLOW_CHECKING > 0
	if(arg<0)
		throw std::overflow_error("signed to unsigned overflow");
#endif	
	return static_cast<std::make_unsigned_t<DeducedSrc>>(arg);
}

/***************************************************************************/
//  to_unsigned_cast
/***************************************************************************/

template <class FixDest, class FixSrc = std::false_type, class DeducedSrc,
	class = std::enable_if_t<
	(std::is_same_v<FixSrc, std::false_type>
		|| std::is_same_v<FixSrc, DeducedSrc>)
	&& std::is_signed_v<DeducedSrc>
	>
>
constexpr auto to_unsigned_cast(DeducedSrc arg)
{
	return static_cast<std::make_unsigned_t<FixDest>>(arg);
}

template <int = 0, class DeducedSrc,
	class = std::enable_if_t<std::is_signed_v<DeducedSrc>>
>
constexpr auto to_unsigned_cast(DeducedSrc arg)
{
	return static_cast<std::make_unsigned_t<DeducedSrc>>(arg);
}

/***************************************************************************/
//  narrow_to
/***************************************************************************/

template <class Dest, class FixSrc = std::false_type,
	class DeducedSrc, class = std::enable_if_t<
	(std::is_same_v<FixSrc, std::false_type>
		|| std::is_same_v<FixSrc, DeducedSrc>)
	&& std::is_integral_v<Dest>
	&& std::is_integral_v<DeducedSrc>
	&& std::is_signed_v<Dest> == std::is_signed_v<DeducedSrc>
	&& (sizeof(Dest) < sizeof(DeducedSrc))>
	>
constexpr Dest narrow_to(DeducedSrc arg)
{
	Dest nt = static_cast<Dest>(arg);
#if XNR_CONVERSION_OVERFLOW_CHECKING > 0
	if (nt !=arg)
			throw std::overflow_error("integer narrowing overflow");
#endif
	return nt;
}

/***************************************************************************/
//  narrow_cast_to
/***************************************************************************/

template <class Dest, class FixSrc = std::false_type,
	class DeducedSrc, class = std::enable_if_t<
	(std::is_same_v<FixSrc, std::false_type>
		|| std::is_same_v<FixSrc, DeducedSrc>)
	&& std::is_integral_v<Dest>
	&& std::is_integral_v<DeducedSrc>
	&& (sizeof(Dest) < sizeof(DeducedSrc))>
	>
	constexpr Dest narrow_cast_to(DeducedSrc arg)
{
	return static_cast<Dest>(arg);
}

/***************************************************************************/
// truncate_to
/***************************************************************************/
template <class Dest = decltype(1), class FixSrc = std::false_type,
	class DeducedSrc, class = std::enable_if_t<
	(std::is_same_v<FixSrc, std::false_type>
		|| std::is_same_v<FixSrc, DeducedSrc>)
	&& std::is_floating_point_v<DeducedSrc>
	&& !std::is_floating_point_v<Dest>>
	>
constexpr Dest truncate_to(DeducedSrc arg)
{
#if XNR_CONVERSION_OVERFLOW_CHECKING > 0
	if (
		std::numeric_limits<Dest>::max() < arg
		||
		std::numeric_limits<Dest>::min() > arg)
		throw std::overflow_error
		("number too large for conversion");
#endif
	return static_cast<Dest>(arg);
}

/***************************************************************************/
// round_to
/***************************************************************************/

template <class Dest = decltype(1), class FixSrc = std::false_type,
	class DeducedSrc, class = std::enable_if_t<
	(std::is_same_v<FixSrc, std::false_type>
		|| std::is_same_v<FixSrc, DeducedSrc>)
	&& std::is_floating_point_v<DeducedSrc>
	&& !std::is_floating_point_v<Dest>>
	>
constexpr Dest round_to(DeducedSrc arg)
{
#if XNR_CONVERSION_OVERFLOW_CHECKING > 0
	if (
		std::numeric_limits<Dest>::max() < arg
		||
		std::numeric_limits<Dest>::min() > arg
		)
			throw std::overflow_error("number too large for conversion");
#endif
	return static_cast<const Dest>((std::is_signed_v<Dest> && arg < 0) ?
		arg - 0.5 : arg + 0.5);
}

/***************************************************************************/
// approx_to
/***************************************************************************/
template <class Dest, class FixSrc = std::false_type,
	class DeducedSrc, class = std::enable_if_t<
	(std::is_same_v<FixSrc, std::false_type> || std::is_same_v<FixSrc, DeducedSrc>)
	&& std::is_floating_point_v<Dest>
	&& 
	(
		(std::is_floating_point_v<DeducedSrc> && (sizeof(Dest) < sizeof(DeducedSrc)))
		||
		(std::is_integral_v<DeducedSrc> && (sizeof(Dest) <= sizeof(DeducedSrc)))
	)
	>
>
constexpr Dest approx_to(DeducedSrc arg)
{
	return static_cast<Dest>(arg);
}

/***************************************************************************/
// promote_to
/***************************************************************************/
template <class Dest, class FixSrc = std::false_type,
	class DeducedSrc, class = std::enable_if_t<
		(std::is_same_v<FixSrc, std::false_type> || std::is_same_v<FixSrc, DeducedSrc>)
		&& (sizeof(Dest) > sizeof(DeducedSrc))
		&& 
		(
			(std::is_floating_point_v<Dest> && std::is_arithmetic_v<DeducedSrc> )
			||
			(std::is_integral_v<Dest> && std::is_signed_v<Dest> && std::is_integral_v<DeducedSrc>)	
			|| 
			(std::is_unsigned_v<Dest> && std::is_unsigned_v<DeducedSrc>)
		)
	>
>
constexpr Dest promote_to(DeducedSrc arg)
{
	return static_cast<Dest>(arg);
}

/***************************************************************************/
// promote_here
/***************************************************************************/
template <class Dest, class FixSrc = std::false_type, 
	class = std::enable_if_t<std::is_arithmetic_v<Dest>>
>
class promote_here
{
	Dest v;
public:
	template <class DeducedSrc, class = std::enable_if_t<
		(std::is_same_v<FixSrc, std::false_type> || std::is_same_v<FixSrc, DeducedSrc>)
		&& (sizeof(Dest) > sizeof(DeducedSrc) || std::is_same_v<Dest , DeducedSrc>)
		&&
		(
			std::is_same_v<Dest, DeducedSrc>
			||
			(std::is_floating_point_v<Dest>&& std::is_arithmetic_v<DeducedSrc>)
			||
			(std::is_integral_v<Dest> && std::is_signed_v<Dest> && std::is_integral_v<DeducedSrc>)
			||
			(std::is_unsigned_v<Dest> && std::is_unsigned_v<DeducedSrc>)
		)
		>
	>																
	explicit inline promote_here(DeducedSrc uv) : v(static_cast<Dest>(uv))
	{}
	
	//Disallowing copying will stop some unintended use 
	inline promote_here(promote_here const& _v) = delete;
	inline promote_here(promote_here const&& _v) = delete;

	inline auto operator = (promote_here const& _v) = delete;
	inline auto operator = (promote_here const&& _v) = delete;

	inline promote_here() = delete;

	inline operator Dest() const {

		return v;
	}

	inline Dest operator ()() const {

		return v;
	}

	//This will catch attempts to initialise wrong numerical types
		//and not alow them to compile
	template<class T,
		class = std::enable_if_t<std::is_arithmetic_v<T>>>
	inline operator const T() const {
		const int f = 0;
		f = 6; //intentionally prevents compilation
		return v;
	}
};

/***************************************************************************/
// aliases using promote_here
/***************************************************************************/

using double_here = promote_here<double>;
using long_double_here = promote_here<long double>;
using long_long_here = promote_here<long long>;

#endif //XNR_CONVERSIONS_H
