CXX			= g++
CFlags		= -Wall -Wextra -Werror -std=c++14 -MMD -MF $(subst src,dep,$(<:.cpp=.d))
LFlags		= 
Libraries	= 
Objects		= $(subst src,obj,$(Sources:.cpp=.o))
Sources		= $(wildcard src/*.cpp)
Dependencies= $(subst src,dep,$(Sources:.cpp=.d))
Executable	= makeMake.exe
mode		=

vpath %.cpp	src
vpath %.hpp	src
vpath %.d	dep
vpath %.o	obj

.PHONY: build clean depClean debug change setd setb

debug:| setd change $(Executable)
	if "$(wildcard DEBUG)"=="" (echo.>DEBUG)

build:| setb change $(Executable)
	if "$(wildcard BUILD)"=="" (echo.>BUILD)

change:
	if "$(mode)"=="-g -DDEBUG" if "$(wildcard BUILD)"=="BUILD" (del $(objects) BUILD )
	if "$(mode)"=="-O2" if "$(wildcard DEBUG)"=="DEBUG" (del $(objects) DEBUG )

setd:
	$(eval mode = -g -DDEBUG)

setb:
	$(eval mode = -O2)

$(Executable):$(Objects)
	$(CXX) $(Objects) -o $(Executable) $(LFlags) $(Libraries)

obj/%.o:%.cpp
	$(CXX) -c $(CFlags) -o $(subst src,obj,$@) $<

clean:
	del $(Executable) $(subst /,\,$(Objects)) $(subst /,\,$(Dependencies))

depClean:
	del $(subst /,\,$(Dependencies))

src/%.hpp:depClean

-include $(Dependencies)

#created with makeMake v1.0.0
