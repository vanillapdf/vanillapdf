#!/usr/bin/perl

use strict; 
use warnings;
use gotchangpdf;

my $bcp;

my $os = $^O;

if ($os eq "MSWin32") {
	# Running windows
	print "MS Windows\n";
	$bcp = "$gotchangpdf::root_dir\\os\\Win32\\bcp.exe";
}
elsif ($os eq "LinuxTODO") {
	# Running linux
	print "Linux\n";
	
	$bcp = "$gotchangpdf::root_dir\\os\\Linux\\bcp.exe";
}
else {
	# Unknown platform
	die "Unknown platform $os";
}

# Check for existing output directory
if (-d $gotchangpdf::boost_local_dir) {
	rmdir($gotchangpdf::boost_local_dir);
}

mkdir($gotchangpdf::boost_local_dir);

# Find all our source files
my @files = find_files($gotchangpdf::src_dir);

# Include boost build utility to output
my @includes = ("build");

# Find all file includes
foreach(@files) {
	push(@includes, find_includes($_, $gotchangpdf::boost_local_dir));
}

# Copy all dependencies to output directory
system("$bcp --boost=$gotchangpdf::boost_original_dir @includes $gotchangpdf::boost_local_dir") == 0 or die $!;

sub find_includes {
    my $source = $_[0];
    my $dest = $_[1];
	
	my @ress;

	open (my $fh, "<", "$source") || die $!;
    while ( <$fh> ) {
        if (/#.*include <(boost.*)>/) {
			push(@ress, $1);
        }
    }
    close($fh);
	
	return @ress;
}

sub find_files {
    my $path = $_[0];
    my @res;
   
    if (! -d $path) {
        print "Input path is not a directory\n";
        return @res;
    }
   
    opendir(my $dh, $path) || die;   
    while(readdir $dh) {
        next if(/^\.{1,2}/);
   
        my $found = "$path\\$_";
       
        if (-d $found) {
            push(@res, find_files($found));
        }       
        elsif (-f $found) {
			# Use a regular expression to find files ending in .h or .c
			next unless (/\.h|\.cpp$/);
			
            push(@res, $found);
        }
        else {
            print "Unknown file type \"$found\"\n";
        }
    }
   
    closedir $dh;
   
    return @res;
}