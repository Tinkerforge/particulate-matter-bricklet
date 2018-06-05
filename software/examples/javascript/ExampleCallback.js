var Tinkerforge = require('tinkerforge');

var HOST = 'localhost';
var PORT = 4223;
var UID = 'XYZ'; // Change XYZ to the UID of your Particulate Matter Bricklet

var ipcon = new Tinkerforge.IPConnection(); // Create IP connection
var pm = new Tinkerforge.BrickletParticulateMatter(UID, ipcon); // Create device object

ipcon.connect(HOST, PORT,
    function (error) {
        console.log('Error: ' + error);
    }
); // Connect to brickd
// Don't use device before ipcon is connected

ipcon.on(Tinkerforge.IPConnection.CALLBACK_CONNECTED,
    function (connectReason) {
        // Set period for PM concentration callback to 1s (1000ms)
        pm.setPMConcentrationCallbackConfiguration(1000, false);
    }
);

// Register PM concentration callback
pm.on(Tinkerforge.BrickletParticulateMatter.CALLBACK_PM_CONCENTRATION,
    // Callback function for PM concentration callback
    function (pm10, pm25, pm100) {
        console.log('PM 1.0: ' + pm10 + ' µg/m³');
        console.log('PM 2.5: ' + pm25 + ' µg/m³');
        console.log('PM 10.0: ' + pm100 + ' µg/m³');
        console.log();
    }
);

console.log('Press key to exit');
process.stdin.on('data',
    function (data) {
        ipcon.disconnect();
        process.exit(0);
    }
);
