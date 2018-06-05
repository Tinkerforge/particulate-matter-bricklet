using System;
using Tinkerforge;

class Example
{
	private static string HOST = "localhost";
	private static int PORT = 4223;
	private static string UID = "XYZ"; // Change XYZ to the UID of your Particulate Matter Bricklet

	// Callback function for PM concentration callback
	static void PMConcentrationCB(BrickletParticulateMatter sender, int pm10, int pm25,
	                              int pm100)
	{
		Console.WriteLine("PM 1.0: " + pm10 + " µg/m³");
		Console.WriteLine("PM 2.5: " + pm25 + " µg/m³");
		Console.WriteLine("PM 10.0: " + pm100 + " µg/m³");
		Console.WriteLine("");
	}

	static void Main()
	{
		IPConnection ipcon = new IPConnection(); // Create IP connection
		BrickletParticulateMatter pm =
		  new BrickletParticulateMatter(UID, ipcon); // Create device object

		ipcon.Connect(HOST, PORT); // Connect to brickd
		// Don't use device before ipcon is connected

		// Register PM concentration callback to function PMConcentrationCB
		pm.PMConcentrationCallback += PMConcentrationCB;

		// Set period for PM concentration callback to 1s (1000ms)
		pm.SetPMConcentrationCallbackConfiguration(1000, false);

		Console.WriteLine("Press enter to exit");
		Console.ReadLine();
		ipcon.Disconnect();
	}
}
