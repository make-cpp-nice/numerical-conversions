# Numerical conversions to use instead of static_cast

A set of dedicated numerical conversions that are a better choice than the canonical application static_cast. 

static_cast is used as a safety measure to prevent the accidental conversion of a pointer by integer types. However  it is not helpful to tar all numerical conversions with the same brush and even more so when that same brush is also used for pointer casting. 

Provided here is a more fine grained and restrictive set of dedicated numerical conversions to use instead. Their names describe what they do and that allows them to sit much more comfortably in the code in which they are used. 

They do come with in-built overflow checking enabled in Debug builds by default but that is not their main purpose. They are intended most of all to improve the semantic role that explicit conversions play in the code in which they sit so that they help to tell its story rather than impede its readability, as shown in the code fragments below. I hope the names are well enough chosen that you can comfortably read this right now with no further introduction.

//I want my yPixel to be the best integer representation 
int yPixel = round_to<int>(GetAverageAt(xPixel)); 	//GetAverageAt returns a double

//I am only interested in full sacks
int Num10KgSacks = truncate_to<int>(KgsOfGrainInSilo/10); // KgsOfGrainInSilo is a double

//Bounds checking before use – make the size_t signed before comparison to avoid warning
if(i >-1 && i< to_signed(my_std_vector.size())) {
	return my_std_vector[i];
}

//Bounds checking before use – more optimal deliberately exploiting overflow
if(to_unsigned_cast(i) < my_std_vector.size()) { 
	return my_std_vector[i];		
}

//assuring double precision to avoid integer division
double ratio_of_integers = (double_here)x/y;

//Pythagoras Theorem without loosing precision 
double c = approx_to<double>(sqrt((long_double_here)a*a + (long_double_here)b*b));

Every conversion is zero overhead when overflow checking is not enabled, except for round_to which performs rounding. Like static_cast, they will not allow an integer to swallow a pointer but go beyond that by being tightly typed to only accept template parameters and argument types that correspond to the conversion they represent. Here is the full list:

T to_signed(arg) arg must be unsigned and T will be be the signed version of arg type.
Optionally T may be declared explicitly to_signed<T>(arg) where T must be signed and not narrower than the arg type. Overflow will throw if checking enabled.
T to_unsigned(arg) arg must be signed and T will be be the unsigned version of arg. Optionally T may be declared explicitly to_unsigned<T>(arg) where T must be unsigned and not narrower than the arg type. Overflow will throw if checking enabled.
T narrow_to<T>(arg) Must represent a narrowing of integer type width and may also include conversion between signed and unsigned in either direction. Overflow will throw if checking enabled.

The variants to_signed_cast,  to_unsigned_cast, narrow_cast_to which do the same as the above but indicate that an exploitation of the defined results of overflow is intended and they will not throw if one occurs.

T round_to<T>(arg) Must represent a conversion from floating point to integer and will return the best integer representation of arg. Overflow is undefined and will throw if checking enabled.
T truncate_to<T>(arg) Must represent a conversion from floating point to integer and will truncate arg to the nearest whole number. Overflow is undefined and will throw if checking enabled.

T approx_to<T>(arg) T must be floating point and arg may be a wider floating point type or an integer type whose full precision cannot be represented by T (the same width or wider). Although precision may be lost, overflow will not occur.

T promote_to<T>(arg) Must represent a promotion that is guaranteed to succeed without overflow or loss of precision. There must be a promotion.

T promote_here<T>(arg) Does the same as promote_to but can also be applied redundantly making it equivalent to brace initialisation – it will not narrow. It is designed specifically for in place forced promotions as the name suggests. It is implemented as a type of functor that permits prefix application (promote_as<T>) arg which remains useful for forced promotions within expressions, although you will probably prefer to use one of the aliases defined below:

Using double_here = promote_here<double>;
Using long_double_here = promote_here<long double>;
Using long_long_here = promote_here<long long>;

e.g. 
double ratio_of_integers = (double_here)x/y; //equivalent to double{x}/y
	

All of these conversions can optionally be more tightly typed using a second template parameter to fix the type of the argument. e.g 

int xPixel = round_to<int, double>(arg); //will only compile if arg is a double.

They cover and characterise every possible conversion between numerical types. The characterisation is important because each conversion performs a different action and may carry  distinct hazards. This is something you should be aware of as you write them and it is what you need to see when you read them.

Which to use where?  The use of some of them will be mandated by the need to resolve unwanted warnings:
“signed/unsigned mismatch” - to_signed and to_unsigned 
“possible loss of data” - narrow_to 
“possible loss of precision” - round_to, truncate_to, approx_to
But warnings cannot be relied on to flag up every potentially hazardous conversion. If you can't apply promote_to or promote_here then you will need one of the above and only one of them will fit. 

Overflow checking is a pre-compiler option. By default it is enabled in Debug builds and absent in Release builds but you can override this by defining XNR_CONVERSION_OVERFLOW_CHECKING before including numerical_conversions.h. 
#define XNR_CONVERSION_OVERFLOW_CHECKING 1 
to enable checking, or 
#define XNR_CONVERSION_OVERFLOW_CHECKING 0 
to disable it,
Regardless of this setting the conversions to_signed_cast,  to_unsigned_cast, narrow_cast_to will not throw because they represent an intention to allow overflow to occur and exploit it. 

There is not much of a learning curve to using these conversions instead of static_cast and the result is something more readable, more informative, more type safe and that keeps hazards to numeric integrity more visible.
