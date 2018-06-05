#!/bin/sh
# Connects to localhost:4223 by default, use --host and --port to change this

uid=XYZ # Change XYZ to the UID of your Particulate Matter Bricklet

# Handle incoming PM concentration callbacks
tinkerforge dispatch particulate-matter-bricklet $uid pm-concentration &

# Set period for PM concentration callback to 1s (1000ms)
tinkerforge call particulate-matter-bricklet $uid set-pm-concentration-callback-configuration 1000 false

echo "Press key to exit"; read dummy

kill -- -$$ # Stop callback dispatch in background
