#!/usr/bin/env python
# -*- coding: utf-8 -*-

HOST = "localhost"
PORT = 4223
UID = "XYZ" # Change XYZ to the UID of your Particulate Matter Bricklet

from tinkerforge.ip_connection import IPConnection
from tinkerforge.bricklet_particulate_matter import BrickletParticulateMatter

# Callback function for PM concentration callback
def cb_pm_concentration(pm10, pm25, pm100):
    print("PM 1.0: " + str(pm10) + " µg/m³")
    print("PM 2.5: " + str(pm25) + " µg/m³")
    print("PM 10.0: " + str(pm100) + " µg/m³")
    print("")

if __name__ == "__main__":
    ipcon = IPConnection() # Create IP connection
    pm = BrickletParticulateMatter(UID, ipcon) # Create device object

    ipcon.connect(HOST, PORT) # Connect to brickd
    # Don't use device before ipcon is connected

    # Register PM concentration callback to function cb_pm_concentration
    pm.register_callback(pm.CALLBACK_PM_CONCENTRATION, cb_pm_concentration)

    # Set period for PM concentration callback to 1s (1000ms)
    pm.set_pm_concentration_callback_configuration(1000, False)

    input("Press key to exit\n") # Use raw_input() in Python 2
    ipcon.disconnect()
