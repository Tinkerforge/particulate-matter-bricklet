#!/bin/sh
# Connects to localhost:4223 by default, use --host and --port to change this

uid=XYZ # Change XYZ to the UID of your Particulate Matter Bricklet

# Get current PM concentration
tinkerforge call particulate-matter-bricklet $uid get-pm-concentration
