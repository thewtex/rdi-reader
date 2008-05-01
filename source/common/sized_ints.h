// From: Daniel Lohmann <daniel dot lohmann at informatik dot uni-erlangen dot de>
// http://gcc.gnu.org/ml/gcc-help/2007-06/msg00259.html
// The only assumption we make is that a character contains 1 byte and a byte is a unit for 8 bits
//

#ifndef SIZED_INTS_H
#define SIZED_INTS_H

#ifndef CHAR_BIT
  #define CHAR_BIT 8
#endif

namespace internal {
	class ERROR_inttype_not_available
	{
		ERROR_inttype_not_available();
	};

	template< bool FIRST, class A, class B > struct select
	{
		typedef A T;
	};
	template< class A, class B > struct select< false, A, B >
	{
		typedef B T;
	};

}	// namespace internal

// Retrieves a signed/unsigned integer type with sizeof( T ) == BYTES
template< unsigned BYTES, bool SIGNED >
struct int_t
{
	typedef typename internal::select< sizeof( signed char ) * CHAR_BIT == 8 * BYTES,  signed char,
		    typename internal::select< sizeof( signed short ) * CHAR_BIT == 8 * BYTES, signed short,
			typename internal::select< sizeof( signed int ) * CHAR_BIT == 8 * BYTES, signed int,
			typename internal::select< sizeof( signed long ) * CHAR_BIT == 8 * BYTES, signed long,
			typename internal::select< sizeof( signed long long ) * CHAR_BIT == 8 * BYTES, signed long long,
			internal::ERROR_inttype_not_available >::T >::T >::T >::T >::T T;
};
template< unsigned BYTES >
struct int_t< BYTES, false>
{
	typedef typename internal::select< sizeof( unsigned char ) * CHAR_BIT == 8 * BYTES, unsigned char,
		    typename internal::select< sizeof( unsigned short ) * CHAR_BIT == 8 * BYTES, unsigned short,
			typename internal::select< sizeof( unsigned int ) * CHAR_BIT == 8 * BYTES, unsigned int,
			typename internal::select< sizeof( unsigned long ) * CHAR_BIT == 8 * BYTES, unsigned long,
			typename internal::select< sizeof( unsigned long long ) * CHAR_BIT == 8 * BYTES, unsigned long long,
			internal::ERROR_inttype_not_available >::T >::T >::T >::T >::T T;
};

// Retrieves the smallest unsigned integer type with sizeof( T ) >= BYTES
template< unsigned BYTES >
struct uint_t_min
{
	typedef typename internal::select< sizeof( unsigned char ) * CHAR_BIT >= 8 * BYTES, unsigned char,
		    typename internal::select< sizeof( unsigned short ) * CHAR_BIT >= 8 * BYTES, unsigned short,
			typename internal::select< sizeof( unsigned int ) * CHAR_BIT >= 8 * BYTES, unsigned int,
			typename internal::select< sizeof( unsigned long ) * CHAR_BIT >= 8 * BYTES, unsigned long,
			typename internal::select< sizeof( unsigned long long ) * CHAR_BIT >= 8 * BYTES, unsigned long long,
			internal::ERROR_inttype_not_available >::T >::T >::T >::T >::T T;
};



// Machine independent definition of sized integer types
typedef int_t< 1, true >::T		Int8;
typedef int_t< 2, true >::T		Int16;
typedef int_t< 4, true >::T		Int32;
typedef int_t< 8, true >::T		Int64;
typedef int_t< 1, false >::T	UInt8;
typedef int_t< 2, false >::T	UInt16;
typedef int_t< 4, false >::T	UInt32;
typedef int_t< 8, false >::T	UInt64;

#endif // inclusion guard
