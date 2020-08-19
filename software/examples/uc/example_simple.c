

#include "bindings/hal_common.h"
#include "bindings/bricklet_particulate_matter.h"

#define UID "XYZ" // Change XYZ to the UID of your Particulate Matter Bricklet

void check(int rc, const char* msg);




TF_ParticulateMatter pm;

void example_setup(TF_HalContext *hal) {
	// Create device object
	check(tf_particulate_matter_create(&pm, UID, hal), "create device object");


	// Get current PM concentration
	uint16_t pm10, pm25, pm100;
	check(tf_particulate_matter_get_pm_concentration(&pm, &pm10, &pm25,
	                                                 &pm100), "get PM concentration");

	tf_hal_printf("PM 1.0: %u µg/m³\n", pm10);
	tf_hal_printf("PM 2.5: %u µg/m³\n", pm25);
	tf_hal_printf("PM 10.0: %u µg/m³\n", pm100);

}

void example_loop(TF_HalContext *hal) {
	// Poll for callbacks
	tf_hal_callback_tick(hal, 0);
}
