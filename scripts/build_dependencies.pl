#!/usr/bin/perl

use strict; 
use warnings;

# Build boost
system("build_boost.pl") == 0 or die $!;

# Build zlib
system("build_zlib.pl") == 0 or die $!;