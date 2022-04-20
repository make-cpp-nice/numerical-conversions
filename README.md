# Smart-numeric-conversion-casts
Smart numeric conversion casts that are correctly applied using the C style cast syntax but are more strongly typed and resilient to coding errors than a static_cast. 

Smart numeric conversion casts that are correctly applied using the C style cast syntax, have appropriately verbose and descriptive names and are more strongly typed and resilient to coding errors than a static_cast. In debug builds, exceptions will catch conversion overflows. In release builds, they have zero overhead.

They are:

(to_signed) - converts a size_t to an int

(trunc_to_int) or (round_to_int) - converts a double to an int

(to_unsigned) - converts an int to a size_t 

(trunc_to_unsigned) or (round_to_unsigned)  - converts a double to a size_t

(to_real) - converts an int to a double

(unsigned_to_real) - converts a size_t to a double

They can only be used in their correct context so they will always correctly describe the conversion taking place, loudly proclaiming transitions between signed & unsigned and integer & real. That can be quite an orientation aide when reading your code.

Where a conversion has a potential to produce incorrect results (e.g., signed to unsigned), debug builds will check that numbers do not exceed the boundaries that cause this and throw a run - time exception if they do.

These smart cast are dedicated value casts and always operate as defined conversions.
They will not work as pointer casts:

