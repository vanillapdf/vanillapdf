#!/usr/bin/perl

package gotchangpdf;

use Cwd;
use File::Basename;

# TODO find root directory of project
chdir "..";

our $root_dir = getcwd;
our $src_dir = "$root_dir\\src";

our $boost_original_dir = "C:\\boost_1_57_0";
our $boost_local_dir = "$root_dir\\lib\\boost";
our $boost_build_dir = "$boost_local_dir\\stage";

our $zlib_dir = "$root_dir\\lib\\zlib";

chdir "scripts";

1;