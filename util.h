#ifndef SANDBOX_UTIL_H
#define SANDBOX_UTIL_H

/**
 * Quick helper function to capitalize an entire std::string.
**/
static void upper(std::string& s){
	for(unsigned i=0;i<s.length();++i){
		s[i]=toupper(s[i]);
	}
}

#endif
