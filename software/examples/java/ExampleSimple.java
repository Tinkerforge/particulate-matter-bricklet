import com.tinkerforge.IPConnection;
import com.tinkerforge.BrickletParticulateMatter;
import com.tinkerforge.BrickletParticulateMatter.PMConcentration;

public class ExampleSimple {
	private static final String HOST = "localhost";
	private static final int PORT = 4223;

	// Change XYZ to the UID of your Particulate Matter Bricklet
	private static final String UID = "XYZ";

	// Note: To make the example code cleaner we do not handle exceptions. Exceptions
	//       you might normally want to catch are described in the documentation
	public static void main(String args[]) throws Exception {
		IPConnection ipcon = new IPConnection(); // Create IP connection
		BrickletParticulateMatter pm =
		  new BrickletParticulateMatter(UID, ipcon); // Create device object

		ipcon.connect(HOST, PORT); // Connect to brickd
		// Don't use device before ipcon is connected

		// Get current PM concentration
		PMConcentration pmConcentration = pm.getPMConcentration(); // Can throw com.tinkerforge.TimeoutException

		System.out.println("PM 1.0: " + pmConcentration.pm10 + " µg/m³");
		System.out.println("PM 2.5: " + pmConcentration.pm25 + " µg/m³");
		System.out.println("PM 10.0: " + pmConcentration.pm100 + " µg/m³");

		System.out.println("Press key to exit"); System.in.read();
		ipcon.disconnect();
	}
}
