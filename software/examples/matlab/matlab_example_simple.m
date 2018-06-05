function matlab_example_simple()
    import com.tinkerforge.IPConnection;
    import com.tinkerforge.BrickletParticulateMatter;

    HOST = 'localhost';
    PORT = 4223;
    UID = 'XYZ'; % Change XYZ to the UID of your Particulate Matter Bricklet

    ipcon = IPConnection(); % Create IP connection
    pm = handle(BrickletParticulateMatter(UID, ipcon), 'CallbackProperties'); % Create device object

    ipcon.connect(HOST, PORT); % Connect to brickd
    % Don't use device before ipcon is connected

    % Get current PM concentration
    pmConcentration = pm.getPMConcentration();

    fprintf('PM 1.0: %i µg/m³\n', pmConcentration.pm10);
    fprintf('PM 2.5: %i µg/m³\n', pmConcentration.pm25);
    fprintf('PM 10.0: %i µg/m³\n', pmConcentration.pm100);

    input('Press key to exit\n', 's');
    ipcon.disconnect();
end
