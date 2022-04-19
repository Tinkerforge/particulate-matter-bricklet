// This example is not self-contained.
// It requres usage of the example driver specific to your platform.
// See the HAL documentation.

#include "bindings/hal_common.h"
#include "bindings/bricklet_particulate_matter.h"

#define UID "XYZ" // Change XYZ to the UID of your Particulate Matter Bricklet

void check(int rc, const char* msg);

void example_setup(TF_HAL *hal);
void example_loop(TF_HAL *hal);


// Callback function for PM concentration callback
static void pm_concentration_handler(TF_ParticulateMatter *device, uint16_t pm10,
                                     uint16_t pm25, uint16_t pm100, void *user_data) {
	(void)device; (void)user_data; // avoid unused parameter warning

	tf_hal_printf("PM 1.0: %I16u µg/m³\n", pm10);
	tf_hal_printf("PM 2.5: %I16u µg/m³\n", pm25);
	tf_hal_printf("PM 10.0: %I16u µg/m³\n", pm100);
	tf_hal_printf("\n");
}

static TF_ParticulateMatter pm;

void example_setup(TF_HAL *hal) {
	// Create device object
	check(tf_particulate_matter_create(&pm, UID, hal), "create device object");

	// Register PM concentration callback to function pm_concentration_handler
	tf_particulate_matter_register_pm_concentration_callback(&pm,
	                                                         pm_concentration_handler,
	                                                         NULL);

	// Set period for PM concentration callback to 1s (1000ms)
	tf_particulate_matter_set_pm_concentration_callback_configuration(&pm, 1000, false);
}

void example_loop(TF_HAL *hal) {
	// Poll for callbacks
	tf_hal_callback_tick(hal, 0);
}
