use std::{error::Error, io, thread};
use tinkerforge::{ipconnection::IpConnection, particulate_matter_bricklet::*};

const HOST: &str = "127.0.0.1";
const PORT: u16 = 4223;
const UID: &str = "XYZ"; // Change XYZ to the UID of your Particulate Matter Bricklet

fn main() -> Result<(), Box<dyn Error>> {
    let ipcon = IpConnection::new(); // Create IP connection
    let particulate_matter_bricklet = ParticulateMatterBricklet::new(UID, &ipcon); // Create device object

    ipcon.connect(HOST, PORT).recv()??; // Connect to brickd
                                        // Don't use device before ipcon is connected

    //Create listener for PM concentration events.
    let pm_concentration_listener = particulate_matter_bricklet.get_pm_concentration_receiver();
    // Spawn thread to handle received events. This thread ends when the particulate_matter_bricklet
    // is dropped, so there is no need for manual cleanup.
    thread::spawn(move || {
        for event in pm_concentration_listener {
            println!("PM 1.0: {}{}", event.pm10, " µg/m³");
            println!("PM 2.5: {}{}", event.pm25, " µg/m³");
            println!("PM 10.0: {}{}", event.pm100, " µg/m³");
            println!();
        }
    });

    // Set period for PM concentration callback to 1s (1000ms)
    particulate_matter_bricklet.set_pm_concentration_callback_configuration(1000, false);

    println!("Press enter to exit.");
    let mut _input = String::new();
    io::stdin().read_line(&mut _input)?;
    ipcon.disconnect();
    Ok(())
}
