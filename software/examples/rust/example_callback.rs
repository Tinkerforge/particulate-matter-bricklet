use std::{error::Error, io, thread};
use tinkerforge::{ip_connection::IpConnection, particulate_matter_bricklet::*};

const HOST: &str = "localhost";
const PORT: u16 = 4223;
const UID: &str = "XYZ"; // Change XYZ to the UID of your Particulate Matter Bricklet.

fn main() -> Result<(), Box<dyn Error>> {
    let ipcon = IpConnection::new(); // Create IP connection.
    let pm = ParticulateMatterBricklet::new(UID, &ipcon); // Create device object.

    ipcon.connect((HOST, PORT)).recv()??; // Connect to brickd.
                                          // Don't use device before ipcon is connected.

    let pm_concentration_receiver = pm.get_pm_concentration_callback_receiver();

    // Spawn thread to handle received callback messages.
    // This thread ends when the `pm` object
    // is dropped, so there is no need for manual cleanup.
    thread::spawn(move || {
        for pm_concentration in pm_concentration_receiver {
            println!("PM 1.0: {} µg/m³", pm_concentration.pm10);
            println!("PM 2.5: {} µg/m³", pm_concentration.pm25);
            println!("PM 10.0: {} µg/m³", pm_concentration.pm100);
            println!();
        }
    });

    // Set period for PM concentration callback to 1s (1000ms).
    pm.set_pm_concentration_callback_configuration(1000, false);

    println!("Press enter to exit.");
    let mut _input = String::new();
    io::stdin().read_line(&mut _input)?;
    ipcon.disconnect();
    Ok(())
}
