#!/usr/bin/perl

use strict; 
use warnings;

use File::Path;
use File::Copy;
use File::Basename;

my $outdir = "..\\lib\\boost_1_55_0";
my $srcdir = "..\\src";
my $boostdir = "C:\\boost_1_55_0";
my $bcp = "..\\lib\\boost_1_55_0\\tools\\bcp.exe";

my @files = find_files($srcdir);
foreach(@files) {
    copy_file($_, $outdir);
}

sub copy_file {
    my $source = $_[0];
    my $dest = $_[1];

	open (my $fh, "<", "$srcdir\\$source") || die $!;
    while ( <$fh> ) {
        if (/#.*include <(boost.*)>/) {
			print "found $1\n";
            system("$bcp --boost=$boostdir $1 $outdir") == 0 or die $!;
        }
    }
    close($fh);
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
        print "$found\n";
       
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