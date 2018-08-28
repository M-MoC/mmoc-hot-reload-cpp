#include <iostream>
#include <vector>
#include "processes.hpp"

std::vector<std::string> split(const std::string& to_split,char split_with = ' ')
{
	std::vector<std::string> to_return;
	to_return.push_back(std::string());
	
	for(int i=0;i<to_split.size();++i)
	{
		if(to_split[i]!=split_with)
			to_return.back()+=to_split[i];
		else to_return.push_back(std::string());
	}
	
	return to_return;
};

//below is for testing global namespace isn't polluted by windows.h
float CreateProcessA(float arg1,float arg2,float arg3)
	{ return arg1*arg2+arg3; }
void* ZeroMemory(char* mem,size_t bytes)
	{ for(int i=0;i<bytes;++i) mem[i]=0; std::cout<<"ZeroMemory(...) called\n"; return mem; }
struct HMODULE
	{ int x; }
	;

int main()
{
	
	std::cout<<CreateProcessA(2,3,1)<<"\n"; //returns 2*3+1=6+1=7
	ZeroMemory(nullptr,0); //returns nullptr=0
	HMODULE thing{123};
	std::cout<<thing.x<<" = 123\n";
	
	std::string(* func)(std::vector<int>&)=nullptr;
	mmoc::Dll dll;
	
	//reloading same dll? closing nonexistent dll? THINK ABOUT
	
	auto load_func_dll=[&]()-> void
	{
		std::cout<<"loading func.dll...\n";
		if(!dll.load("func.dll"))
			{ std::cout<<"failed to load dll.\n"; return; }
		std::cout<<"success!\ngetting func...\n";
		func=reinterpret_cast<decltype(func)>(dll.get("func"));
		if(func==nullptr)
			{ std::cout<<"failed to get func.\n"; return; }
		std::cout<<"succcess! func now loaded.\n";
	};
	
	//load_func_dll();
	
	auto invoke_func_compile=[&]()-> void
	{
		std::cout<<"attempting to run \"compile func.bat\"...\n";
		mmoc::Process compile;
		if(!compile.launch("compile func.bat"))
			{ std::cout<<"failed to run \"recompile func.bat\"."; return; }
		std::cout<<"now running...\n\n";
		while(compile.status()==mmoc::Process::StillActive())
			;
		std::cout<<"\n\n";
	};
	
	bool quit=false;while(!quit)
	{
		std::string input;
		
		std::cout<<
			"enter:\n"
			" \"feed\" to feed arguments to current function\n"
			" \"compile\" to recompile its source code to func.dll\n"
			" \"load\"/\"unload\" to load/unload func.dll\n"
			" \"q\" to quit this application\n"
			"\n> "
			;
		std::getline(std::cin,input);
		
		if(input=="compile")
			invoke_func_compile();
		else if(input=="unload")
			dll.close();
		else if(input=="load")
			load_func_dll();
		else if(input=="q")
			quit=true;
		else if(input=="feed")
		{
			LABEL_retry:
			std::cout<<
				"Enter a list of space-separated integers (the arguments) "
				"or enter \"q\" to quit feeding arguments:\n> "
				;
			std::getline(std::cin,input);
			std::vector<std::string> str_args=split(input);
			if(str_args[0]!="esc" && str_args[0]!="q")
			{
				std::vector<int> args;
				bool retry=false;for(int i=0;i<str_args.size();++i)
				{
					try { args.push_back(std::stoi(str_args[i])); }
					catch(std::out_of_range& e)
					{
						std::cout<<
							"error: argument "<<args.size()<<" (\""<<str_args[i]<<"\") "
							"is out of type int's range of possible values\n"
							;
						retry=true;
					}
					catch(std::invalid_argument& e)
					{
						std::cout<<
							"error: argument "<<args.size()<<" (\""<<str_args[i]<<"\") "
							"is not a valid int\n"
							;
						retry=true;
					}
				}
				if(retry)
					{ std::cout<<"\n\n"; goto LABEL_retry; }
				//call func with args & report to user
				std::cout<<"f(";
				for(int i=0;i<args.size();++i)
					std::cout<<args[i]<<(i==args.size()-1 ?")":",");
				std::cout<<
					" =>\n"<<func(args)<<"\n\n"
					"state of argument vector after calling f(args):\n"
					;
				for(int i=0;i<args.size();++i)
					std::cout<<i<<(i==args.size()-1 ?"\n":",");
				std::cout<<"\n\n";
			}
		} else std::cout<<"not a valid command. refer to the below:\n\n";
	}
}
