#!/usr/bin/env python3
# -*- coding: utf-8 -*-

HOST = "localhost"
PORT = 4223
UID = "2aAkrW" # Change XYZ to the UID of your Particulate Matter Bricklet

import time

from tinkerforge.ip_connection import IPConnection
from tinkerforge.bricklet_particulate_matter import BrickletParticulateMatter

def cb_pm_concentration(pm10, pm25, pm100):
    print(time.time(), 'concentration', pm10, pm25, pm100)

def cb_pm_count(greater03um, greater05um, greater10um, greater25um, greater50um, greater100um):
    print(time.time(), 'count', greater03um, greater05um, greater10um, greater25um, greater50um, greater100um)

if __name__ == "__main__":
    ipcon = IPConnection() # Create IP connection
    pm = BrickletParticulateMatter(UID, ipcon) # Create device object

    ipcon.connect(HOST, PORT) # Connect to brickd
    # Don't use device before ipcon is connected

    pm.register_callback(pm.CALLBACK_PM_CONCENTRATION, cb_pm_concentration)
    pm.set_pm_concentration_callback_configuration(0, True)

    pm.register_callback(pm.CALLBACK_PM_COUNT, cb_pm_count)
    pm.set_pm_count_callback_configuration(500, True)

    raw_input("Press key to exit\n")
    ipcon.disconnect()
