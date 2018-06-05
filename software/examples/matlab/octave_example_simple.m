function octave_example_simple()
    more off;

    HOST = "localhost";
    PORT = 4223;
    UID = "XYZ"; % Change XYZ to the UID of your Particulate Matter Bricklet

    ipcon = javaObject("com.tinkerforge.IPConnection"); % Create IP connection
    pm = javaObject("com.tinkerforge.BrickletParticulateMatter", UID, ipcon); % Create device object

    ipcon.connect(HOST, PORT); % Connect to brickd
    % Don't use device before ipcon is connected

    % Get current PM concentration
    pmConcentration = pm.getPMConcentration();

    fprintf("PM 1.0: %d µg/m³\n", pmConcentration.pm10);
    fprintf("PM 2.5: %d µg/m³\n", pmConcentration.pm25);
    fprintf("PM 10.0: %d µg/m³\n", pmConcentration.pm100);

    input("Press key to exit\n", "s");
    ipcon.disconnect();
end
