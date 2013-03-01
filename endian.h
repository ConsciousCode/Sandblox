/**
 * Fix endian issues by providing endian-related helper functions.
**/

#ifndef ENDIAN_H
#define ENDIAN_H

#include <stdint.h>
#include <cstdio>

#if defined (__GLIBC__)
	#if (__BYTE_ORDER == __LITTLE_ENDIAN)
		#define LITTLE_ENDIAN
	#elif (__BYTE_ORDER == __BIG_ENDIAN)
		#define BIG_ENDIAN
	#elif (__BYTE_ORDER == __PDP_ENDIAN)
		#define PDP_ENDIAN
	#else
		#error Unknown machine endianness detected.
	#endif
#elif defined(_BIG_ENDIAN) && !defined(_LITTLE_ENDIAN)
	#define BIG_ENDIAN
#elif defined(_LITTLE_ENDIAN) && !defined(_BIG_ENDIAN)
	#define LITTLE_ENDIAN
#elif defined(__sparc) || defined(__sparc__) \
   || defined(_POWER) || defined(__powerpc__) \
   || defined(__ppc__) || defined(__hpux) \
   || defined(_MIPSEB) || defined(_POWER) \
   || defined(__s390__)
	#define BIG_ENDIAN
		#warning 5
#elif defined(__i386__) || defined(__alpha__) \
   || defined(__ia64) || defined(__ia64__) \
   || defined(_M_IX86) || defined(_M_IA64) \
   || defined(_M_ALPHA) || defined(__amd64) \
   || defined(__amd64__) || defined(_M_AMD64) \
   || defined(__x86_64) || defined(__x86_64__) \
   || defined(_M_X64) || defined(__bfin__)

	#define LITTLE_ENDIAN
#else
	#error Unsupported CPU type.
#endif

namespace endian{
	template<class T,size_t SIZE>
	struct _swapifier{
		static T _swap(const T val){}
	};

	template<class T>
	struct _swapifier<T,1>{
		inline static T _swap(const T v){
			return v;
		}
	};

	template<class T>
	struct _swapifier<T,2>{
		inline static T _swap(const T v){
			short a=*((short*)&v);
			return ((a&0xff)<<8)+
			((a>>8)&0xff);
		}
	};

	template<class T>
	struct _swapifier<T,4>{
		inline static T _swap(const T v){
			long a=*((long*)&v);
			return (((a&0xff)<<24)+
			(((a>>8)&0xff)<<16)+
			(((a>>16)&0xff)<<8)+
			((a>>24)&0xff));
		}
	};

	template<class T>
	struct _swapifier<T,8>{
		inline static T _swap(const T v){
			int64_t a=*((int64_t*)&v);
			return ((a&0xff)<<56)+
			(((a>>8)&0xff)<<48)+
			(((a>>16)&0xff)<<40)+
			(((a>>24)&0xff)<<32)+
			(((a>>32)&0xff)<<24)+
			(((a>>40)&0xff)<<16)+
			(((a>>48)&0xff)<<8)+
			((a>>56)&0xff);
		}
	};

	template<class T>
	inline T _swapper(T v){
		return _swapifier<T,sizeof(T)>::_swap(v);
	}

	/**
	 * A wrapper template class containing a little-endian representation of the given type.
	**/
	template<class T>
	struct lil{
		protected:
			template<class U>
			U swap(U v){
				return _swapifier<U,sizeof(U)>::_swap(v);
			}
		public:
			T value;
			lil(){}
		#ifdef LITTLE_ENDIAN
			#define MAKE_LIL_OP(op)\
				template<class U>\
				lil<T>& operator op##= (U other){\
					value op##= other;\
					return *this;\
				}

			template<class U>
			lil(U v):value(v){}

			operator T(){
				return value;
			}

			template<class U>
			lil<T>& operator=(U other){
				value=other;
				return *this;
			}

			MAKE_LIL_OP(+)
			MAKE_LIL_OP(-)
			MAKE_LIL_OP(*)
			MAKE_LIL_OP(/)
			MAKE_LIL_OP(%)
			MAKE_LIL_OP(<<)
			MAKE_LIL_OP(>>)
			MAKE_LIL_OP(&)
			MAKE_LIL_OP(|)
			MAKE_LIL_OP(^)

			#undef MAKE_LIL_OP
		#elif defined BIG_ENDIAN
			#define MAKE_LIL_OP(op)\
				template<class U>\
				lil<T>& operator op##= (U other){\
					value=swap(swap(value) op other);\
					return *this;\
				}

			template<class U>
			lil(U v):value(swap(v)){}

			operator T(){
				return value;
			}

			template<class U>
			lil<T>& operator=(U other){
				printf("Test %i\n",other);
				value=swap(other);
				printf("Now %i\n",value);
				return *this;
			}

			MAKE_LIL_OP(+)
			MAKE_LIL_OP(-)
			MAKE_LIL_OP(*)
			MAKE_LIL_OP(/)
			MAKE_LIL_OP(%)
			MAKE_LIL_OP(<<)
			MAKE_LIL_OP(>>)
			MAKE_LIL_OP(&)
			MAKE_LIL_OP(|)
			MAKE_LIL_OP(^)

			#undef MAKE_LIL_OP
		#endif
	};

	/**
	 * A wrapper template class containing a big-endian representation of the given type.
	**/
	template<class T>
	struct big{
		protected:
			T value;

			template<class U>
			U swap(U v){
				return _swapifier<U,sizeof(U)>::_swap(v);
			}
		public:
			big(){}

		#ifdef BIG_ENDIAN
			#define MAKE_BIG_OP(op)\
				template<class U>\
				big<T>& operator op##= (U other){\
					value op##= other;\
					return *this;\
				}

			template<class U>
			big(U v):value(v){}

			operator T(){
				return value;
			}

			template<class U>
			big<T>& operator=(U other){
				value=other;
				return *this;
			}

			MAKE_BIG_OP(+)
			MAKE_BIG_OP(-)
			MAKE_BIG_OP(*)
			MAKE_BIG_OP(/)
			MAKE_BIG_OP(%)
			MAKE_BIG_OP(<<)
			MAKE_BIG_OP(>>)
			MAKE_BIG_OP(&)
			MAKE_BIG_OP(|)
			MAKE_BIG_OP(^)

			#undef MAKE_BIG_OP
		#elif defined LITTLE_ENDIAN
			#define MAKE_BIG_OP(op)\
				template<class U>\
				big<T>& operator op##= (U other){\
					value = swap(swap(value) op other);\
					return *this;\
				}

			template<class U>
			big(U v):value(swap(v)){}

			operator T(){
				return value;
			}

			template<class U>
			big<T>& operator=(U other){
				value=other;
				return *this;
			}

			MAKE_BIG_OP(+)
			MAKE_BIG_OP(-)
			MAKE_BIG_OP(*)
			MAKE_BIG_OP(/)
			MAKE_BIG_OP(%)
			MAKE_BIG_OP(<<)
			MAKE_BIG_OP(>>)
			MAKE_BIG_OP(&)
			MAKE_BIG_OP(|)
			MAKE_BIG_OP(^)

			#undef MAKE_BIG_OP
		#endif
	};

	#ifdef BIG_ENDIAN
		template<class T>
		inline T& big2sys(T& v){
			return v;
		}

		template<class T>
		inline T lil2sys(const T& v){
			return _swapper(v);
		}

		typedef lil<signed char> lil8_t;
		typedef lil<unsigned char> ulil8_t;
		typedef lil<short> lil16_t;
		typedef lil<unsigned short> ulil16_t;
		typedef lil<int> lil32_t;
		typedef lil<unsigned> ulil32_t;

		typedef signed char big8_t;
		typedef unsigned char ubig8_t;
		typedef short big16_t;
		typedef unsigned short ubig16_t;
		typedef int big32_t;
		typedef unsigned ubig32_t;
	#elif defined LITTLE_ENDIAN
		template<class T>
		inline T& lil2sys(T& v){
			return v;
		}

		template<class T>
		inline T big2sys(const T& v){
			return _swapper(v);
		}

		typedef big<signed char> lil8_t;
		typedef big<unsigned char> ulil8_t;
		typedef big<short> lil16_t;
		typedef big<unsigned short> ulil16_t;
		typedef big<int> lil32_t;
		typedef big<unsigned> ulil32_t;

		typedef signed char lil8_t;
		typedef unsigned char ulil8_t;
		typedef short lil16_t;
		typedef unsigned short ulil16_t;
		typedef int lil32_t;
		typedef unsigned ulil32_t;
	#endif

	typedef lil32_t lil_t;
	typedef ulil32_t ulil_t;

	typedef big32_t big_t;
	typedef ubig32_t ubig_t;
}

#endif
