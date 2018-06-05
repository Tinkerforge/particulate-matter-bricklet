#!/usr/bin/perl

use strict;
use Tinkerforge::IPConnection;
use Tinkerforge::BrickletParticulateMatter;

use constant HOST => 'localhost';
use constant PORT => 4223;
use constant UID => 'XYZ'; # Change XYZ to the UID of your Particulate Matter Bricklet

# Callback subroutine for PM concentration callback
sub cb_pm_concentration
{
    my ($pm10, $pm25, $pm100) = @_;

    print "PM 1.0: $pm10 µg/m³\n";
    print "PM 2.5: $pm25 µg/m³\n";
    print "PM 10.0: $pm100 µg/m³\n";
    print "\n";
}

my $ipcon = Tinkerforge::IPConnection->new(); # Create IP connection
my $pm = Tinkerforge::BrickletParticulateMatter->new(&UID, $ipcon); # Create device object

$ipcon->connect(&HOST, &PORT); # Connect to brickd
# Don't use device before ipcon is connected

# Register PM concentration callback to subroutine cb_pm_concentration
$pm->register_callback($pm->CALLBACK_PM_CONCENTRATION, 'cb_pm_concentration');

# Set period for PM concentration callback to 1s (1000ms)
$pm->set_pm_concentration_callback_configuration(1000, 0);

print "Press key to exit\n";
<STDIN>;
$ipcon->disconnect();
