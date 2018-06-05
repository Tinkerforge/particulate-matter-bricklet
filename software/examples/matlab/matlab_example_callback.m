function matlab_example_callback()
    import com.tinkerforge.IPConnection;
    import com.tinkerforge.BrickletParticulateMatter;

    HOST = 'localhost';
    PORT = 4223;
    UID = 'XYZ'; % Change XYZ to the UID of your Particulate Matter Bricklet

    ipcon = IPConnection(); % Create IP connection
    pm = handle(BrickletParticulateMatter(UID, ipcon), 'CallbackProperties'); % Create device object

    ipcon.connect(HOST, PORT); % Connect to brickd
    % Don't use device before ipcon is connected

    % Register PM concentration callback to function cb_pm_concentration
    set(pm, 'PMConcentrationCallback', @(h, e) cb_pm_concentration(e));

    % Set period for PM concentration callback to 1s (1000ms)
    pm.setPMConcentrationCallbackConfiguration(1000, false);

    input('Press key to exit\n', 's');
    ipcon.disconnect();
end

% Callback function for PM concentration callback
function cb_pm_concentration(e)
    fprintf('PM 1.0: %i µg/m³\n', e.pm10);
    fprintf('PM 2.5: %i µg/m³\n', e.pm25);
    fprintf('PM 10.0: %i µg/m³\n', e.pm100);
    fprintf('\n');
end
