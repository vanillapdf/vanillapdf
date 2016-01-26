#!/usr/bin/perl

use strict; 
use warnings;

# Build zlib
system("build_zlib.pl") == 0 or die $!;