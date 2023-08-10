#!/usr/bin/env perl

use v5.34;
use strict;
use warnings;
use List::Util qw(max any);

sub ifexists {
	my @if = `ip link show` =~ /[0-9]+:\sgensok0/;
	return (@if > 0) ? 1 : 0;
}

sub dummy_loaded {
	open(my $procmod, "<", "/proc/modules") or die("open(/proc/modules): $!\n");
	while (my $mod = (<$procmod> =~ /^([[:alpha:]]+)/)[0]) {
		return 1 if ($mod eq "dummy");
	}
	return 0;
}

sub load_dummy {
	return if (dummy_loaded);
	system("modprobe", "dummy");
	die("system(modprobe) failed: $!\n") if ($? == -1);
	die("modprobe exited with non-zero (" . ($? >> 8) . ")\n") if ($? >> 8 != 0);
}

sub ifcreate {
	return if (ifexists);
	
	load_dummy;
	
	system("ip", "link", "add", "gensok0",
		# NSOKYO (fucking unicast addresses can't start with an odd octet)
		address => "4e:53:4f:4b:59:4f",
		type    => "dummy");
	die("system(ip) failed: $!\n") if ($? == -1);
	die("ip exited with non-zero (" . ($? >> 8) . ")\n") if ($? >> 8 != 0);
}

sub ifdelete {
	return if (!ifexists);

	system("ip", "link", "del", "gensok0", "type", "dummy");
	die("system(ip) failed: $!\n") if ($? == -1);
	die("ip exited with non-zero (" . ($? >> 8) . ")\n") if ($? >> 8 != 0);
}

if ($> != 0) {
	say(STDERR "you must be root to manage kernel modules and virtual network interfaces");
	exit(1);
}

my $USAGE = "usage: $0 create|delete\n";
die($USAGE) if (@ARGV < 1);

if ($ARGV[0] eq "create") {
	ifcreate;
} elsif ($ARGV[0] eq "delete") {
	ifdelete;
} else {
	die($USAGE);
}
