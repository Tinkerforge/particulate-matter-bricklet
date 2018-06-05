<?php

require_once('Tinkerforge/IPConnection.php');
require_once('Tinkerforge/BrickletParticulateMatter.php');

use Tinkerforge\IPConnection;
use Tinkerforge\BrickletParticulateMatter;

const HOST = 'localhost';
const PORT = 4223;
const UID = 'XYZ'; // Change XYZ to the UID of your Particulate Matter Bricklet

// Callback function for PM concentration callback
function cb_pmConcentration($pm10, $pm25, $pm100)
{
    echo "PM 1.0: $pm10 µg/m³\n";
    echo "PM 2.5: $pm25 µg/m³\n";
    echo "PM 10.0: $pm100 µg/m³\n";
    echo "\n";
}

$ipcon = new IPConnection(); // Create IP connection
$pm = new BrickletParticulateMatter(UID, $ipcon); // Create device object

$ipcon->connect(HOST, PORT); // Connect to brickd
// Don't use device before ipcon is connected

// Register PM concentration callback to function cb_pmConcentration
$pm->registerCallback(BrickletParticulateMatter::CALLBACK_PM_CONCENTRATION,
                      'cb_pmConcentration');

// Set period for PM concentration callback to 1s (1000ms)
$pm->setPMConcentrationCallbackConfiguration(1000, FALSE);

echo "Press ctrl+c to exit\n";
$ipcon->dispatchCallbacks(-1); // Dispatch callbacks forever

?>
