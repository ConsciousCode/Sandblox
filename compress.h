#ifndef SANDBLOX_COMPRESS_H
#define SANDBLOX_COMPRESS_H

#include <string>
#include <vector>

namespace{
	size_t find(const std::vector& in,const std::string& item){
		for(unsigned i=0;i<in.size();++i){
			if(in[i]==item){
				return i;
			}
		}
		return -1;
	}
}

std::string lzw(std::string s){
	std::vector<std::string> table;

	for(char c=0;c<256;++c){
		table.push_back(c);
	}

	std::string out;
	std::string tmp;
	size_t pos,tpos;

	size_t bitlen=9,bit=0;

	for(unsigned i=0;i<s.length();++i){
		tmp=s[i++];
		tpos=0;
		//Find the longest matching code
		for(;;){
			tpos=find(table,tmp);
			if(tpos==-1){
				break
			}
			pos=tpos;
			tmp+=s[i++];
		}
		//Add the new entry to the table
		table.push_back(tmp);

		for(unsigned j=0;j<tmp.length();++j){
			//Fill in leftover bits from the last iteration
			out[out.length()-1]|=char(pos>>bit);
			//Add remaining bits in a new character
			out+=char(pos)<<(16-bitlen-bit);
			bit+=(bitlen-8)%8;
		}
	}
}

#endif
