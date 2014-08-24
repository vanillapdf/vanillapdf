#!/usr/bin/perl

use strict; 
use warnings;

use Time::localtime;

# Gotchangpdf test script #
print "Running test script\n";

# Determine platform
my $os = $^O;
my $path;
print "Determining platform...  ";

if ($os eq "MSWin32") {
	# Running windows
	print "MS Windows\n";
	
	$path = '..\build\gotchangpdf.test\Debug\gotchangpdf.test.exe';
}
elsif ($os eq "LinuxTODO") {
	# Running linux
	print "Linux\n";
	
	$path = '..\build\gotchangpdf.test\Debug\gotchangpdf.test';
}
else {
	# Unknown platform
	die "Unknown platform $os";
}

# Get current date
my $tm=localtime;
my ($day,$month,$year, $hour, $min, $sec)=($tm->mday,$tm->mon,$tm->year + 1900, $tm->hour, $tm->min, $tm->sec);

# Open log file
open (my $log, '>', "..\\log\\$hour-$min-$sec\_$day-$month-$year\_$os.log") or die "Could not open log file: $!";

# Directory containing test cases
my $testdir = '../test';

my $failed_count = 0;

opendir (DIR, $testdir) or die $!;
while (my $file = readdir(DIR)) {

	# Path relative to this directory
	my $file_path = "$testdir/$file";
	
    # We only want files
    next unless (-f "$file_path");

    # Use a regular expression to find files ending in .pdf
    next unless ($file =~ m/\.pdf$/);
	
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