#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>

int main(int argc, char* args[])
{
	if(argc == 1)
	{
		//print usage
		return 0;
	}

	bool twoMode		= false;
	bool bPriority		= false;
	bool buildDirs		= false;
	bool windowed		= false;
	std::string exeName	= "";

	std::vector<std::string> argv(args + 1, args + argc);

	for(auto s:argv)
	{
		if(s == "-db")
		{
			if(twoMode)
			{
				std::cerr << "Too many arguments" << std::endl;
				return 1;
			}

			twoMode		= true;
		}
		else if(s == "-bd")
		{
			if(twoMode)
			{
				std::cerr << "Too many arguments" << std::endl;
				return 1;
			}

			twoMode		= true;
			bPriority	= true;
		}
		else if(s == "-r")
		{
			buildDirs	= true;
		}
		else if(s == "-w")
		{
			windowed = true;
		}
		else if(exeName == "")
		{
			exeName 	= s;
		}
		else
		{
			std::cerr << "Too many arguments" << std::endl;
			return 1;
		}
	}

	if(exeName == "")
	{
		std::cerr << "Too few arguments, requires an executable name" << std::endl;
	}

	if(buildDirs)
	{
		system("mkdir src dep obj");
	}

	std::stringstream makefile;
	makefile << "CXX			= g++\n"
			 << "CFlags		= -Wall -Wextra -Werror -std=c++14 -MMD -MF $(subst src,dep,$(<:.cpp=.d))\n"
			 << "LFlags		= \n"
			 << "Libraries	= \n"
			 << "Objects		= $(subst src,obj,$(Sources:.cpp=.o))\n"
			 << "Sources		= $(wildcard src/*.cpp)\n"
			 << "Dependencies= $(subst src,dep,$(Sources:.cpp=.d))\n"
			 << "Executable	= " << exeName << "\n"
			 << (twoMode?"mode		=\n":"")
			 << "\n"

			 << "vpath \%.cpp	src\n"
			 << "vpath \%.hpp	src\n"
			 << "vpath \%.d	dep\n"
			 << "vpath \%.o	obj\n\n"

			 << ".PHONY: build clean";

	if(twoMode)
	{
		makefile << " debug change setd setb";
	}

	makefile << "\n\n";

	if(twoMode)
	{
		if(bPriority)
		{
			makefile << "build:| setb change $(Executable)\n"
					 << "	if \"$(wildcard BUILD)\"==\"\" (echo.>BUILD)\n\n"

					 << "debug:| setd change $(Executable)\n"
					 << "	if \"$(wildcard DEBUG)\"==\"\" (echo.>DEBUG)\n\n";
		}
		else
		{
			makefile << "debug:| setd change $(Executable)\n"
					 << "	if \"$(wildcard DEBUG)\"==\"\" (echo.>DEBUG)\n\n"
					 
					 << "build:| setb change $(Executable)\n"
					 << "	if \"$(wildcard BUILD)\"==\"\" (echo.>BUILD)\n\n";
		}
		
		makefile << "change:\n"
				 << "	if \"$(mode)\"==\"-g -DDEBUG\" if \"$(wildcard BUILD)\"==\"BUILD\" (del $(objects) BUILD )\n"
				 << "	if \"$(mode)\"==\"-O2\" if \"$(wildcard DEBUG)\"==\"DEBUG\" (del $(objects) DEBUG )\n\n"

				 << "setd:\n"
				 << "	$(eval mode = -g -DDEBUG)\n\n"

				 << "setb:\n"
				 << "	$(eval mode = -O2)\n"
				 << (windowed?"	$(eval LFlags = -Wl,-subsystem,windows)\n\n":"\n");

	}
	
	makefile << "$(Executable):$(Objects)\n"
			 << "	$(CXX) $(Objects) -o $(Executable) $(LFlags) $(Libraries)\n\n"

			 << "obj/\%.o:\%.cpp\n"
			 << "	$(CXX) -c $(CFlags) -o $(subst src,obj,$@) $<\n\n"

			 << "clean:\n"
			 << "	del $(Executable) $(subst /,\\,$(Objects)) $(subst /,\\,$(Dependencies))\n\n"

			<< "-include $(Dependencies)\n";

	std::ofstream mfile;
	mfile.open("makefile");
	mfile << makefile.str();
	mfile.close();

	return 0;
}
