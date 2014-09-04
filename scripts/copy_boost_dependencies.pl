#!/usr/bin/perl

use strict; 
use warnings;

# Gotchangpdf test script #
print "Running test script\n";

# Directory containing test cases
my $testdir = '../test';

opendir (DIR, $src) or die $!;
while (my $file = readdir(DIR)) {

	# Path relative to this directory
	my $file_path = "$testdir/$file";
	
    # We only want files
    next unless (-f "$file_path");

    # Use a regular expression to find files ending in .h
    next unless ($file =~ m/\.h$/);
	
	    # Use a regular expression to find files ending in .h
    next unless ($file =~ m/\.c$/);
	
	print $log "Found test case named \"$file\"...  ";
	
	system("$path $file_path > NUL");
	
	if ($? == 0) {
		print $log "Passed!\n";
	}
	else {
		print $log "Failed!\n";
		$failed_count++;
	}
}

if ($failed_count == 0) {
	print "All tests have finished successfully";
}
else {
	print "$failed_count test scenarios failed";
}

closedir(DIR);
close($log);
exit 0;