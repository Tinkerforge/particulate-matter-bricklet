#!/usr/bin/perl

use strict;
use Tinkerforge::IPConnection;
use Tinkerforge::BrickletParticulateMatter;

use constant HOST => 'localhost';
use constant PORT => 4223;
use constant UID => 'XYZ'; # Change XYZ to the UID of your Particulate Matter Bricklet

my $ipcon = Tinkerforge::IPConnection->new(); # Create IP connection
my $pm = Tinkerforge::BrickletParticulateMatter->new(&UID, $ipcon); # Create device object

$ipcon->connect(&HOST, &PORT); # Connect to brickd
# Don't use device before ipcon is connected

# Get current PM concentration
my ($pm10, $pm25, $pm100) = $pm->get_pm_concentration();

print "PM 1.0: $pm10 µg/m³\n";
print "PM 2.5: $pm25 µg/m³\n";
print "PM 10.0: $pm100 µg/m³\n";

print "Press key to exit\n";
<STDIN>;
$ipcon->disconnect();
