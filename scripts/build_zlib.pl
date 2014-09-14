#!/usr/bin/perl

use strict; 
use warnings;
use gotchangpdf;

use File::Copy;

my $os = $^O;
my $make;

if ($os eq "MSWin32") {
	# Running windows
	print "MS Windows\n";
	
	$make = 'nmake';
}
elsif ($os eq "LinuxTODO") {
	# Running linux
	print "Linux\n";
	
	$make = 'make';
}
else {
	# Unknown platform
	die "Unknown platform $os";
}

# Change working directory to zlib dir
chdir $gotchangpdf::zlib_dir;

# Build all dependent libraries
system("$make -f win32\\Makefile.msc zlib1.dll") == 0 or die $!;