using System;
using Tinkerforge;

class Example
{
	private static string HOST = "localhost";
	private static int PORT = 4223;
	private static string UID = "XYZ"; // Change XYZ to the UID of your Particulate Matter Bricklet

	static void Main()
	{
		IPConnection ipcon = new IPConnection(); // Create IP connection
		BrickletParticulateMatter pm =
		  new BrickletParticulateMatter(UID, ipcon); // Create device object

		ipcon.Connect(HOST, PORT); // Connect to brickd
		// Don't use device before ipcon is connected

		// Get current PM concentration
		int pm10, pm25, pm100;
		pm.GetPMConcentration(out pm10, out pm25, out pm100);

		Console.WriteLine("PM 1.0: " + pm10 + " µg/m³");
		Console.WriteLine("PM 2.5: " + pm25 + " µg/m³");
		Console.WriteLine("PM 10.0: " + pm100 + " µg/m³");

		Console.WriteLine("Press enter to exit");
		Console.ReadLine();
		ipcon.Disconnect();
	}
}
