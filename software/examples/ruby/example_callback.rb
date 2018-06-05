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

# Register PM concentration callback
pm.register_callback(BrickletParticulateMatter::CALLBACK_PM_CONCENTRATION) do |pm10, pm25,
                                                                               pm100|
  puts "PM 1.0: #{pm10} µg/m³"
  puts "PM 2.5: #{pm25} µg/m³"
  puts "PM 10.0: #{pm100} µg/m³"
  puts ''
end

# Set period for PM concentration callback to 1s (1000ms)
pm.set_pm_concentration_callback_configuration 1000, false

puts 'Press key to exit'
$stdin.gets
ipcon.disconnect
