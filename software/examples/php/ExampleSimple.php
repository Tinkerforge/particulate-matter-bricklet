<?php

require_once('Tinkerforge/IPConnection.php');
require_once('Tinkerforge/BrickletParticulateMatter.php');

use Tinkerforge\IPConnection;
use Tinkerforge\BrickletParticulateMatter;

const HOST = 'localhost';
const PORT = 4223;
const UID = 'XYZ'; // Change XYZ to the UID of your Particulate Matter Bricklet

$ipcon = new IPConnection(); // Create IP connection
$pm = new BrickletParticulateMatter(UID, $ipcon); // Create device object

$ipcon->connect(HOST, PORT); // Connect to brickd
// Don't use device before ipcon is connected

// Get current PM concentration
$pm_concentration = $pm->getPMConcentration();

echo "PM 1.0: " . $pm_concentration['pm10'] . " µg/m³\n";
echo "PM 2.5: " . $pm_concentration['pm25'] . " µg/m³\n";
echo "PM 10.0: " . $pm_concentration['pm100'] . " µg/m³\n";

echo "Press key to exit\n";
fgetc(fopen('php://stdin', 'r'));
$ipcon->disconnect();

?>
