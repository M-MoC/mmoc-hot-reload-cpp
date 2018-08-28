#include <string>
#include <vector>

#define mmocPORT __declspec( dllexport )

extern "C"
{

mmocPORT std::string func(std::vector<int>& intvec)
{
	int accum=0;
	for(int i=0;i<intvec.size();++i)
		accum+=intvec[i];
	if(accum<0)
		accum*=-1;
	std::string s;
	for(int i=0;i<accum;++i)
		{ s+=(i%50?"":"\n"); s+=(i%5?"":" "); s+="*"; }
	return s;
}

} //end of extern "C"
