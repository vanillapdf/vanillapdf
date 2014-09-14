#!/usr/bin/perl

use strict; 
use warnings;
use gotchangpdf;

my $os = $^O;
my $bootstrap;

if ($os eq "MSWin32") {
	# Running windows
	print "MS Windows\n";
	
	$bootstrap = 'bootstrap.bat';
}
elsif ($os eq "LinuxTODO") {
	# Running linux
	print "Linux\n";
	
	$bootstrap = 'bootstrap.sh';
}
else {
	# Unknown platform
	die "Unknown platform $os";
}

if (! -d $gotchangpdf::boost_local_dir) die "Boost local directory does not exist, call copy boost at first";

# Change working directory to boost dir
chdir $gotchangpdf::boost_local_dir;

if (! -f "b2.exe") {
	# Build boost build
	system("$bootstrap") == 0 or die $!;
}

# Build all dependent libraries
system("b2 link=static threading=multi -sZLIB_SOURCE=\"$gotchangpdf::zlib_dir\"") == 0 or die $!;