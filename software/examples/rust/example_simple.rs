use std::{error::Error, io};

use tinkerforge::{ipconnection::IpConnection, particulate_matter_bricklet::*};

const HOST: &str = "127.0.0.1";
const PORT: u16 = 4223;
const UID: &str = "XYZ"; // Change XYZ to the UID of your Particulate Matter Bricklet

fn main() -> Result<(), Box<dyn Error>> {
    let ipcon = IpConnection::new(); // Create IP connection
    let particulate_matter_bricklet = ParticulateMatterBricklet::new(UID, &ipcon); // Create device object

    ipcon.connect(HOST, PORT).recv()??; // Connect to brickd
                                        // Don't use device before ipcon is connected

    // Get current PM concentration
    let get_pm_concentration_result = particulate_matter_bricklet.get_pm_concentration().recv()?;

    println!("PM 1.0: {}{}", get_pm_concentration_result.pm10, " µg/m³");
    println!("PM 2.5: {}{}", get_pm_concentration_result.pm25, " µg/m³");
    println!("PM 10.0: {}{}", get_pm_concentration_result.pm100, " µg/m³");

    println!("Press enter to exit.");
    let mut _input = String::new();
    io::stdin().read_line(&mut _input)?;
    ipcon.disconnect();
    Ok(())
}
