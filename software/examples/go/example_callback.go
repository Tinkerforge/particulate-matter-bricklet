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

	pm.RegisterPMConcentrationCallback(func(pm10 uint16, pm25 uint16, pm100 uint16) {
		fmt.Printf("PM 1.0: %d µg/m³\n", pm10)
		fmt.Printf("PM 2.5: %d µg/m³\n", pm25)
		fmt.Printf("PM 10.0: %d µg/m³\n", pm100)
		fmt.Println()
	})

	// Set period for PM concentration callback to 1s (1000ms).
	pm.SetPMConcentrationCallbackConfiguration(1000, false)

	fmt.Print("Press enter to exit.")
	fmt.Scanln()

}
