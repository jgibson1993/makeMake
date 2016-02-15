This utility is the first I'm writing for myself, I'm frankly sick of having to write out makefiles
every time I try and write a program, so I'm making a thing that will (hopefully) simply produce
makefiles for me

USAGE:
	makeMake [Executable name]

with switches:
	-db		setup separate rules for building debug versions of things, debug default
	-bd		ditto on above, but build is default
	-r		sets up folders for sources and objects, writes default rules accordingly
