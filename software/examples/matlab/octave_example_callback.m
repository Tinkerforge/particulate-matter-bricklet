function octave_example_callback()
    more off;

    HOST = "localhost";
    PORT = 4223;
    UID = "XYZ"; % Change XYZ to the UID of your Particulate Matter Bricklet

    ipcon = javaObject("com.tinkerforge.IPConnection"); % Create IP connection
    pm = javaObject("com.tinkerforge.BrickletParticulateMatter", UID, ipcon); % Create device object

    ipcon.connect(HOST, PORT); % Connect to brickd
    % Don't use device before ipcon is connected

    % Register PM concentration callback to function cb_pm_concentration
    pm.addPMConcentrationCallback(@cb_pm_concentration);

    % Set period for PM concentration callback to 1s (1000ms)
    pm.setPMConcentrationCallbackConfiguration(1000, false);

    input("Press key to exit\n", "s");
    ipcon.disconnect();
end

% Callback function for PM concentration callback
function cb_pm_concentration(e)
    fprintf("PM 1.0: %d µg/m³\n", e.pm10);
    fprintf("PM 2.5: %d µg/m³\n", e.pm25);
    fprintf("PM 10.0: %d µg/m³\n", e.pm100);
    fprintf("\n");
end
