
## Numerical conversions to use instead of static_cast

A set of dedicated numerical conversions that are a better choice than the canonical application of ```static_cast```. 

```static_cast```is used as a safety measure to prevent the accidental conversion of a pointer by integer types. However  it is not helpful to tar all numerical conversions with the same brush and even more so when that same brush is also used for pointer casting. 

Provided here is a more fine grained and restrictive set of dedicated numerical conversions to use instead. Their names describe what they do and that allows them to sit much more comfortably in the code in which they are used. 

They do come with in-built overflow checking enabled in Debug builds by default but that is not their main purpose. They are intended most of all to improve the semantic role that explicit conversions play in the code in which they sit so that they help to tell its story rather than impede its readability, as shown in the code fragments below. I hope the names are well enough chosen that you can comfortably read this right now with no further introduction. The conversions used here are: **round_to**, **truncate_to**, **to_signed**, **to_unsigned_cast**, **approx_to**, **double_here** and **long_double_here**. 
```C++
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
```
I hope that it was an easier read than it would be have been had all those conversion been represented by static_cast.

Every conversion is zero overhead when overflow checking is not enabled, except for round_to which performs rounding. Like static_cast, they will not allow an integer to swallow a pointer but go beyond that by being tightly typed to only accept template parameters and argument types that correspond to the conversion they represent. Here is the full list:

+ **```T to_signed(arg)```** arg must be unsigned and T will be be the signed version of arg type.
Optionally T may be declared explicitly ```to_signed<T>(arg)``` where T must be signed and can be wider or narrower than the arg type. 
+ **```T to_unsigned(arg)```** arg must be signed and T will be be the unsigned version of arg. Optionally T may be declared explicitly ```to_unsigned<T>(arg)``` where T must be unsigned and can be wider or narrower than the arg type. 
+ **```T narrow_to<T>(arg)```** Narrowing of integer type width that does not involve change of signedness. T and arg must both be integers, both signed or both unsigned and   T must be narrower than the arg type.

+ The variants **```to_signed_cast```**,  **```to_unsigned_cast```**, **```narrow_cast_to```** which do the same as the above but indicate that an exploitation of the defined results of overflow is intended and they will not throw if one occurs.

+ **```T round_to<T>(arg)```** Must represent a conversion from floating point to integer and will return the best integer representation of arg. 
+ **```T truncate_to<T>(arg)```** Must represent a conversion from floating point to integer and will truncate arg to the nearest whole number.

+ **```T approx_to<T>(arg)```** T must be floating point and arg must be a wider floating point type or an integer type whose full precision cannot be represented by T (the same width or wider). Although precision may be lost, overflow will not occur.

+ **```T promote_to<T>(arg)```** Must represent a promotion that is guaranteed to succeed without overflow or loss of precision. There must be a promotion – it cannot be applied redundantly.

+ **```T promote_here<T>(arg)```** Does the same as promote_to but can also be applied redundantly making it equivalent to brace initialisation – it will not narrow. It is designed specifically for in place forced promotions as the name suggests. It is implemented as a type of functor that permits prefix application (promote_here<T>) arg which remains useful for forced promotions within expressions, although you will probably prefer to use one of the aliases defined below:
```C++
using double_here = promote_here<double>;
using long_double_here = promote_here<long double>;
using long_long_here = promote_here<long long>;
```
e.g. 
```C++
double ratio_of_integers = (double_here) x/y; //equivalent to double{x}/y
```	

All of these conversions can optionally be more tightly typed using a second template parameter to fix the type of the argument. e.g 
```C++
int xPixel = round_to<int, double>(arg); //will only compile if arg is a double.
```
They cover and characterise every possible conversion between numerical types. The characterisation is important because each conversion performs a different action and may carry  distinct hazards. This is something you should be aware of as you write them and it is what you need to see when you read them.

Which to use where?  The use of some of them will be mandated by the need to resolve unwanted warnings:
+ “signed/unsigned mismatch” - **to_signed** and **to_unsigned** 
+ “possible loss of data” - **narrow_to,  to_signed<T> and to_unsigned <T>**
+ “possible loss of precision” - **round_to<T>, truncate_to<T>, approx_to<T>**

But warnings cannot be relied on to flag up every potentially hazardous conversion. If you can't apply **promote_to** or **promote_here** then you will need one of the above and only one of them will fit. 

In turn each conversion that you use keeps it's potential hazards visible in its name:

+ **to_signed, to_unsigned** and **narrow_to** can all overflow and will throw if overflow checking is enabled.
+ **to_signed_cast, to_unsigned_cast** and **narrow_cast_to** can also overflow but indicate an intention to exploit the defined results of overflow. They will not throw even if overflow checking is enabled.
+ **round_to** and **truncate_to** will overflow if converting a number that is too large (more than 2 billion in the case of a 32 bit int). The result of overflow is undefined and will throw if overflow checking is enabled.
+ **approx_to** may loose precision but will not overflow (unless you are trying to estimate the number of atoms in the universe)
+ **promote_to** and **promote_here** can never overflow nor loose precision

Overflow checking is a pre-compiler option. By default it is enabled in Debug builds and absent in Release builds but you can override this by defining ```XNR_CONVERSION_OVERFLOW_CHECKING``` before including numerical_conversions.h. 
```C++
#define XNR_CONVERSION_OVERFLOW_CHECKING 1 /*to enable checking*/
//or 
#define XNR_CONVERSION_OVERFLOW_CHECKING 0 /*to disable checking*/
``` 
Regardless of this setting the conversions to_signed_cast,  to_unsigned_cast, narrow_cast_to will not throw because they represent an intention to allow overflow to occur and exploit it. 

There is not much of a learning curve to using these conversions instead of static_cast and the result is something more readable, more informative, more type safe and that keeps hazards to numeric integrity more controlled and visible.

Please post any feedback or comments on [Comment and discussion](https://github.com/make-cpp-nice/numerical-conversions/discussions/1)
