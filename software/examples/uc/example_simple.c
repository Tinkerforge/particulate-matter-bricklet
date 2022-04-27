// This example is not self-contained.
// It requires usage of the example driver specific to your platform.
// See the HAL documentation.

#include "src/bindings/hal_common.h"
#include "src/bindings/bricklet_particulate_matter.h"

void check(int rc, const char *msg);
void example_setup(TF_HAL *hal);
void example_loop(TF_HAL *hal);

static TF_ParticulateMatter pm;

void example_setup(TF_HAL *hal) {
	// Create device object
	check(tf_particulate_matter_create(&pm, NULL, hal), "create device object");

	// Get current PM concentration
	uint16_t pm10, pm25, pm100;
	check(tf_particulate_matter_get_pm_concentration(&pm, &pm10, &pm25,
	                                                 &pm100), "get PM concentration");

	tf_hal_printf("PM 1.0: %I16u µg/m³\n", pm10);
	tf_hal_printf("PM 2.5: %I16u µg/m³\n", pm25);
	tf_hal_printf("PM 10.0: %I16u µg/m³\n", pm100);
}

void example_loop(TF_HAL *hal) {
	// Poll for callbacks
	tf_hal_callback_tick(hal, 0);
}
