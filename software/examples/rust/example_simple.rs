use std::{error::Error, io};

use tinkerforge::{ip_connection::IpConnection, particulate_matter_bricklet::*};

const HOST: &str = "localhost";
const PORT: u16 = 4223;
const UID: &str = "XYZ"; // Change XYZ to the UID of your Particulate Matter Bricklet.

fn main() -> Result<(), Box<dyn Error>> {
    let ipcon = IpConnection::new(); // Create IP connection.
    let pm = ParticulateMatterBricklet::new(UID, &ipcon); // Create device object.

    ipcon.connect((HOST, PORT)).recv()??; // Connect to brickd.
                                          // Don't use device before ipcon is connected.

    // Get current PM concentration.
    let pm_concentration = pm.get_pm_concentration().recv()?;

    println!("PM 1.0: {} µg/m³", pm_concentration.pm10);
    println!("PM 2.5: {} µg/m³", pm_concentration.pm25);
    println!("PM 10.0: {} µg/m³", pm_concentration.pm100);

    println!("Press enter to exit.");
    let mut _input = String::new();
    io::stdin().read_line(&mut _input)?;
    ipcon.disconnect();
    Ok(())
}
