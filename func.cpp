#include <string>
#include <vector>

#define mmocPORT __declspec( dllexport )

extern "C"
{

mmocPORT std::string func(std::vector<int>& intvec)
{
	int accum=0;for(int i=0;i<intvec.size();++i)
		accum+=intvec[i];
	
	if(accum<0)
		accum*=-1;
	
	std::string s;
	for(int i=0;i<accum;++i)
		s += "*" += (i%5?"":" ") += (i%20?"":"\n");
	
	return s;
}

} //end of extern "C"
