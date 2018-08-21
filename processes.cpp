#ifndef mmocPROCESSES_CPP
#define mmocPROCESSES_CPP
#define mmocPORT __declspec( dllexport )

#include <windows.h>
#include "processes.hpp"

namespace mmoc
{

namespace priv
{
	struct Process
	{
		STARTUPINFO startup_info;
		PROCESS_INFORMATION proc_info;
	};
	
	struct Dll
	{
		HMODULE handle;
	};
}

// for mmoc::Process
mmocPORT Process::Process() {}
mmocPORT Process::~Process() {}

mmocPORT bool Process::launch(std::string pathname_in)
{
	pathname=pathname_in; args="";
	
	ZeroMemory(&impl->startup_info,sizeof(impl->startup_info));
	impl->startup_info.cb=sizeof(impl->startup_info);
	
	ZeroMemory(&impl->proc_info,sizeof(impl->proc_info));
	
	return CreateProcessA( //A for ASCII
		pathname.data(), //pass pathname as const char*
		NULL, //no args
		NULL,NULL, //process & thread handle not inheritable
		FALSE, //set handle inheritance to FALSE
		0, //no creation flags
		NULL,NULL, //use parent's environment block & starting directory
		&impl->startup_info,&impl->proc_info //info
		);
}
mmocPORT bool Process::launch(std::string pathname_in,std::string args_in)
{
	pathname=pathname_in; args=args_in;
	
	ZeroMemory(&impl->startup_info,sizeof(impl->startup_info));
	impl->startup_info.cb=sizeof(impl->startup_info);
	
	ZeroMemory(&impl->proc_info,sizeof(impl->proc_info));
	
	return CreateProcessA( //A for ASCII
		pathname.data(), //pass pathname as const char*
		&args[0], //pass arg as char* (copy because needs to be mutable)
		NULL, NULL, //process & thread handle not inheritable
		FALSE, //set handle inheritance to FALSE
		0, //no creation flags
		NULL, NULL, //use parent's environment block & starting directory 
		&impl->startup_info,&impl->proc_info //info
		);
}

mmocPORT const std::string& Process::get_pathname() const
	{ return pathname; }
mmocPORT const std::string& Process::get_args() const
	{ return args; }

mmocPORT unsigned Process::StillActive()
	{ return STILL_ACTIVE; }
mmocPORT unsigned Process::status()
	{ DWORD status; GetExitCodeProcess(impl->proc_info.hProcess,&status); return status; }

mmocPORT bool Process::close()
{
	bool success=true;
	if(!CloseHandle(impl->proc_info.hProcess))
		success=false;
	if(!CloseHandle(impl->proc_info.hThread))
		success=false;
	return success;
}
mmocPORT unsigned Process::wait_for_process_conclusion(Process& process,unsigned ms)
	{ return WaitForSingleObject( process.impl->proc_info.hProcess, ms==-1 ? INFINITE : ms ); }

// for mmoc::Dll
mmocPORT Dll::Dll() {}
mmocPORT Dll::~Dll() {}

mmocPORT bool Dll::load(std::string pathname_in)
	{ pathname=pathname_in; return (impl->handle=LoadLibraryA(pathname.data()))!=NULL; }
mmocPORT const std::string& Dll::get_pathname() const
	{ return pathname; }
mmocPORT void* Dll::get(std::string name)
	{ return (void*)GetProcAddress(impl->handle,name.data()); }
mmocPORT bool Dll::close()
	{ return FreeLibrary(impl->handle); }

} //end of namespace mmoc

#endif