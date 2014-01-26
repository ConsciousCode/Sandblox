#ifndef STR_HPP
#define STR_HPP

#include <string>
#include <tuple>
#include <cstdio>
#include <iostream>
#include <sstream>
#include <typeinfo>
#include <type_traits>
#include <stdexcept>

#ifdef __GNUG__
	#include <memory>
	#include <cstdlib>
	#include <cxxabi.h>

	namespace util{
		std::string demangle(const char* s){
			int status;
			std::unique_ptr<char,void(*)(void*)> res{
				abi::__cxa_demangle(s,NULL,NULL,&status),
				std::free
			};

			return (status==0)?res.get():s;
		}
	}
#else
	namespace util{
		std::string demangle(const char* s){
			return s;
		}
	}
#endif

namespace util{
	std::string str(bool b){
		return b?"true":"false";
	}

	std::string str(char c){
		return std::string(1,c);
	}

	std::string str(unsigned char c){
		char buf[4];
		sprintf(buf,"%u",unsigned(c));
		return buf;
	}

	std::string str(short s){
		char buf[8];
		sprintf(buf,"%i",int(s));
		return buf;
	}

	std::string str(unsigned short s){
		char buf[8];
		sprintf(buf,"%u",unsigned(s));
		return buf;
	}

	std::string str(int i){
		char buf[16];
		sprintf(buf,"%d",i);
		return buf;
	}

	std::string str(unsigned int i){
		char buf[16];
		sprintf(buf,"%u",i);
		return buf;
	}

	std::string str(long long int i){
		std::stringstream ss;
		ss<<i;
		return ss.str();
	}

	std::string str(unsigned long long int i){
		std::stringstream ss;
		ss<<i;
		return ss.str();
	}

	std::string str(float f){
		char buf[16];
		sprintf(buf,"%f",f);
		return buf;
	}

	std::string str(double d){
		char buf[32];
		sprintf(buf,"%f",d);
		return buf;
	}

	std::string str(long double d){
		std::stringstream ss;
		ss<<d;
		return ss.str();
	}

	template<typename T>
	std::string str(T* p){
		char buf[256];
		sprintf(buf,"<%s pointer to 0x%p>",demangle(typeid(T).name()).c_str(),p);
		return buf;
	}

	template<typename T>
	std::string str(T const* p){
		char buf[256];
		sprintf(buf,"<const %s pointer to 0x%p>",demangle(typeid(T).name()).c_str(),p);
		return buf;
	}

	std::string str(const char* s){
		return s;
	}

	template<typename T,size_t N>
	std::string str(T(&a)[N]){
		std::string s="{";
		T* d=a;
		T* end=d+N;

		while(d!=end){
			s+=str(*(d++));
			if(d+1!=end){
				s+=", ";
			}
		}

		s+="}";

		return s;
	}

	template<size_t N>
	std::string str(char s[N]){
		return s;
	}

	template<typename T>
	std::string str(){
		return demangle(typeid(T).name());
	}

	template<typename RET,class... ARGS>
	std::string str(RET (*func)(ARGS...)){
		char buf[512];
		sprintf(buf,"<%s function(%s) at %p>",
			demangle(typeid(RET).name()),
			str<ARGS...>().c_str(),
			func
		);
		return buf;
	}

	template<typename RET,class T,class... ARGS>
	std::string str(RET (T::*func)(ARGS...)){
		char buf[512];
		sprintf(buf,"<%s %s method(%s) at %p>",
			demangle(typeid(RET).name()),
			demangle(typeid(T).name()),
			str<ARGS...>().c_str(),
			func
		);
		return buf;
	}

	std::string str(std::string s){
		return s;
	}

	std::string remove_scope(std::string s){
		size_t pos=s.find_last_of("::");
		if(pos==std::string::npos){
			return s;
		}
		return s.substr(pos+1);
	}

	std::string str(const std::exception& e){
		return remove_scope(demangle(typeid(e).name()))+": "+e.what();
	}

	std::string str(const std::type_info& ti){
		return demangle(ti.name());
	}

	namespace{
		template<size_t X,typename... ARGS>
		std::string str_tuple(const std::tuple<ARGS...>& t){
			if(X<std::tuple_size<decltype(t)>::value){
				return str(std::get<X>(t),", ",str_tuple<X+1>(t));
			}
			return str(std::get<X>(t));
		}
	}

	template<typename... ARGS>
	std::string str(const std::tuple<ARGS...>& t){
		if(std::tuple_size<decltype(t)>::value==0){
			return "()";
		}
		if(std::tuple_size<decltype(t)>::value==1){
			return "("+str(std::get<0>(t))+",)";
		}
		return "("+str_tuple<0>(t)+")";
	}

	namespace{
		//helps determine if the type is iterable
		template<typename T>
		struct is_iterable{
			protected:
				using Yes=char[2];
				using No=char[1];

				struct Fallback{int being,end;};
				struct Derived:public T,Fallback{};

				template<class U>
				static No& test(decltype(U::begin)*,decltype(U::end)*);

				template<class U>
				static Yes& test(U*,U*);
			public:
				const static bool value=sizeof(test<Derived,Derived>(nullptr))==sizeof(Yes);
		};

		template<typename T>
		struct has_str{
			typedef char yes;
			typedef long no;

			template<typename A>
			static yes test(decltype(&A::str));
			template<typename A>
			static no test(...);

			enum{value=sizeof(test<T>(0))==sizeof(yes)};
		};

		template<bool HAS_STR,bool IS_ITER>
		struct generic_str_decider{
			template<typename T>
			static std::string str(const T& o){
				char buf[256];
				sprintf(buf,"<%s object at 0x%p>",demangle(typeid(T).name()).c_str(),&o);
				return buf;
			}
		};

		template<bool IS_ITER>
		struct generic_str_decider<true,IS_ITER>{
			template<typename T>
			static std::string str(const T& o){
				return o.str();
			}
		};

		template<>
		struct generic_str_decider<false,true>{
			template<typename T>
			static std::string str(const T& o){
				std::stringstream ss(demangle(typeid(T).name()));
				ss<<"[";
				for(auto v:o){
					ss<<str(v);
				}
				ss<<"]";

				return ss.str();
			}
		};
	}

	//delve into some black magic here
	//generic_string_decider decides between 3 different
	//possible behaviors based on attributes present
	//in the given type.
	template<typename T>
	std::string str(const T& o){
		return generic_str_decider<has_str<T>::value,is_iterable<T>::value>::str(o);
	}

	template<typename T,typename... ARGS>
	std::string str(const T& v,const ARGS&... args){
		return str(v)+str(args...);
	}

	template<typename T>
	void print(const T& v){
		std::cout<<str(v)<<std::endl;
		fflush(stdout);
	}

	template<typename FIRST,typename... ARGS>
	void print(const FIRST& v,const ARGS&... args){
		std::cout<<str(v)<<" ";
		print(args...);
	}

	template<typename T>
	void printx(const T& v){
		std::cout<<str(v)<<std::endl;
		fflush(stdout);
	}

	template<typename FIRST,typename... ARGS>
	void printx(const FIRST& v,const ARGS&... args){
		std::cout<<str(v);
		printx(args...);
	}

	/**
	 * A spin on the boost::any type that preserves stringification.
	 * For now, only really used by format(s,...)
	**/
	class any{
		protected:
			struct any_internal_base{
				std::function<std::string()> str_f;

				virtual ~any_internal_base(){}

				virtual any_internal_base* copy()=0;
				virtual const std::type_info& type() const=0;
			}*value;

			template<typename T>
			struct any_internal:public any_internal_base{
				T val;

				any_internal(const T& v):val(v){
					std::string(*f)(T)=&util::str;
					str_f=std::bind(f,val);
				}

				any_internal(T&& v):val(v){
					type=typeid(T);
					std::string(*f)(T)=&util::str;
					str_f=std::bind(f,val);
				}

				~any_internal(){}

				virtual any_internal_base* copy(){
					return new any_internal<T>(val);
				}

				virtual const std::type_info& type() const{
					return typeid(val);
				}
			};
		public:
			any(){
				value=nullptr;
			}

			any(const any& v){
				value=v.value?v.value->copy():nullptr;
			}

			any(any&& v){
				value=v.value;
				v.value=nullptr;
			}

			template<typename T>
			any(const T& v){
				value=new any_internal<T>(v);
			}

			template<typename T>
			any(T&& v){
				value=new any_internal<T>(v);
			}

			~any(){
				if(value){
					delete value;
				}
			}

			template<typename T>
			bool is_type(){
				return value?value->type()==typeid(T):false;
			}

			template<typename T>
			T& convert(){
				if(value and value->type()==typeid(T)){
					return ((any_internal<T>*)value)->val;
				}
				throw std::bad_cast();
			}

			std::string str() const{
				return value?value->str_f():"any()";
			}

			const std::type_info& type() const{
				return value?value->type():typeid(void);
			}

			any& operator=(const any& v){
				if(value){
					delete value;
				}
				value=v.value->copy();

				return *this;
			}
	};

	namespace{
		//edge case for when an argument that doesn't exist is referenced
		any get_any_arg_at(unsigned n){
			throw std::out_of_range("Format index not in arguments.");
		}

		//returns the nth argument as an any value for use in the format function (util::any preserves stringification)
		template<typename FIRST,typename... ARGS>
		any get_any_arg_at(unsigned n,const FIRST& v,const ARGS&... args){
			if(n>0){
				return get_any_arg_at(n,args...);
			}

			return v;
		}
	}

	/**
	 * An error thrown whenever a value isn't quite right.
	**/
	class ValueError:public std::runtime_error{
		public:
			ValueError(const std::string& s):std::runtime_error(s){}
	};

	/**
	 * Implements Pythonic string formatting.
	**/
	template<typename... ARGS>
	std::string format(const std::string& s,const ARGS&... args){
		std::string out;
		auto it=s.begin(),end=s.end();
		unsigned i=0;

		while(it!=end){
			if(*it=='{'){
				if(++it!=end){
					if(*it=='}'){
						out+=get_any_arg_at(i++,args...).str();
					}
					else if(*it=='{'){
						out+='{';
					}
					else if(isdigit(*it)){
						auto start=it;
						while(it!=end and isdigit(*(++it))){}
						if(it!=end and *it=='}'){
							i=atoi(std::string(start,it).c_str());
							out+=get_any_arg_at(i++,args...).str();
						}
						else{
							throw ValueError("Unmatched '{' in format string");
						}
					}
					else{
						throw ValueError(str("Unexpected character '",*it,"' in format string"));
					}

					++it;
				}
			}
			else if(*it=='}'){
				if(++it!=end){
					if(*it=='}'){
						out+='}';
					}
					else{
						throw ValueError("Unmatched '}' in format string");
					}
					++it;
				}
			}
			else{
				auto start=it;
				while(++it!=end and *it!='{' and *it!='}'){}
				out+=std::string(start,it);
			}
		}

		return out;
	}
}

#endif
