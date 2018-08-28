#ifndef mmocPROCESSES_HPP
#define mmocPROCESSES_HPP
#if mmocbuildPROCESSES_HPP
	#define mmocPORT __declspec( dllexport )
#else
	#define mmocPORT __declspec( dllimport )
#endif
#include <memory> //unique_ptr

#include <string>

namespace mmoc
{

namespace priv
{
	class Process;
	class Dll;
}

class mmocPORT Process
{
	std::unique_ptr<priv::Process> impl;
	std::string pathname,args;
public:
	Process(); //for std::unique_ptr
	~Process(); //for std::unique_ptr
	
	bool launch(std::string);
	bool launch(std::string,std::string);
	
	const std::string& get_pathname() const;
	const std::string& get_args() const;
	
	static unsigned StillActive();
	unsigned status(); //only use if process exists; otherwise undefined return value
	
	bool close();
	bool terminate(unsigned);
	//return value: 0 = concluded, ~0 = wait function failed, [other value] = wait timed out
	unsigned wait_for_process_conclusion(Process&,unsigned = -1);
};

class mmocPORT Dll
{
	std::string pathname;
	std::unique_ptr<priv::Dll> impl;
public:
	Dll(); //for std::unique_ptr
	~Dll(); //for std::unique_ptr
	
	bool load(std::string);
	const std::string& get_pathname() const;
	void* get(std::string);
	bool close();
};

} //end of namespace mmoc

#undef mmocDLL
#endif