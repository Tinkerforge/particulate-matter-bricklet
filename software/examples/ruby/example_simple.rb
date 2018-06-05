#!/usr/bin/env ruby
# -*- ruby encoding: utf-8 -*-

require 'tinkerforge/ip_connection'
require 'tinkerforge/bricklet_particulate_matter'

include Tinkerforge

HOST = 'localhost'
PORT = 4223
UID = 'XYZ' # Change XYZ to the UID of your Particulate Matter Bricklet

ipcon = IPConnection.new # Create IP connection
pm = BrickletParticulateMatter.new UID, ipcon # Create device object

ipcon.connect HOST, PORT # Connect to brickd
# Don't use device before ipcon is connected

# Get current PM concentration as [pm10, pm25, pm100]
pm_concentration = pm.get_pm_concentration

puts "PM 1.0: #{pm_concentration[0]} µg/m³"
puts "PM 2.5: #{pm_concentration[1]} µg/m³"
puts "PM 10.0: #{pm_concentration[2]} µg/m³"

puts 'Press key to exit'
$stdin.gets
ipcon.disconnect
