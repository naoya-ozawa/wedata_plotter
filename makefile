drawhisto:	drawhisto.cpp
	`root-config --cxx --cflags` -o drawhisto drawhisto.cpp `root-config --glibs`

slicecount:	slicecount.cpp
	`root-config --cxx --cflags` -o slicecount slicecount.cpp `root-config --glibs`

sourcecheck:	sourcecheck.cpp
	`root-config --cxx --cflags` -o sourcecheck sourcecheck.cpp `root-config --glibs`

timewindow_comparison:	timewindow_comparison.cpp
	`root-config --cxx --cflags` -o timewindow_comparison timewindow_comparison.cpp `root-config --glibs`

runcomparison:	runcomparison.cpp
	`root-config --cxx --cflags` -o runcomparison runcomparison.cpp `root-config --glibs`
