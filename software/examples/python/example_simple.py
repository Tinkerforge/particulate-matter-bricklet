#!/usr/bin/env python
# -*- coding: utf-8 -*-

HOST = "localhost"
PORT = 4223
UID = "XYZ" # Change XYZ to the UID of your Particulate Matter Bricklet

from tinkerforge.ip_connection import IPConnection
from tinkerforge.bricklet_particulate_matter import BrickletParticulateMatter

if __name__ == "__main__":
    ipcon = IPConnection() # Create IP connection
    pm = BrickletParticulateMatter(UID, ipcon) # Create device object

    ipcon.connect(HOST, PORT) # Connect to brickd
    # Don't use device before ipcon is connected

    # Get current PM concentration
    pm10, pm25, pm100 = pm.get_pm_concentration()

    print("PM 1.0: " + str(pm10) + " µg/m³")
    print("PM 2.5: " + str(pm25) + " µg/m³")
    print("PM 10.0: " + str(pm100) + " µg/m³")

    input("Press key to exit\n") # Use raw_input() in Python 2
    ipcon.disconnect()
