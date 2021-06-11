drawhisto:	drawhisto.cpp
	`root-config --cxx --cflags` -o drawhisto drawhisto.cpp `root-config --glibs`

sourcecheck:	sourcecheck.cpp
	`root-config --cxx --cflags` -o sourcecheck sourcecheck.cpp `root-config --glibs`