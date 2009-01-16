#!/usr/bin/perl

use warnings;
use strict;

die "Usage: makefixed.pl <value>\n" if (@ARGV != 1);

my $val = shift @ARGV;

printf("0x%.8x\n", $val * (1 << 10));

