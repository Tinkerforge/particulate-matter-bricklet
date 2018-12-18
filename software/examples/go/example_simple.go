package main

import (
	"fmt"
	"github.com/tinkerforge/go-api-bindings/ipconnection"
	"github.com/tinkerforge/go-api-bindings/particulate_matter_bricklet"
)

const ADDR string = "localhost:4223"
const UID string = "XYZ" // Change XYZ to the UID of your Particulate Matter Bricklet.

func main() {
	ipcon := ipconnection.New()
	defer ipcon.Close()
	pm, _ := particulate_matter_bricklet.New(UID, &ipcon) // Create device object.

	ipcon.Connect(ADDR) // Connect to brickd.
	defer ipcon.Disconnect()
	// Don't use device before ipcon is connected.

	// Get current PM concentration.
	pm10, pm25, pm100, _ := pm.GetPMConcentration()

	fmt.Printf("PM 1.0:  µg/m³\n", pm10)
	fmt.Printf("PM 2.5:  µg/m³\n", pm25)
	fmt.Printf("PM 10.0:  µg/m³\n", pm100)

	fmt.Print("Press enter to exit.")
	fmt.Scanln()

}
