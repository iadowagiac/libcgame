#include <cgame.h>
#include "../assets.h"

float z = 6.2;

static void
on_load_scene(void)
{
    spawn_entity(EN_CAMERA, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0);
    spawn_entity(EN_PLAYER, 0.0, 0.0, 7.0, 0.0, 0.0, 0.0, 100.0 * CENTIMETERS_TO_METERS);

    // A cube goes from (-1,-1,-1) to (1, 1, 1).
    // This means that it's 2x2x2 meters in size.
    // If we scale it to 10.0 * CENTIMETERS_TO_METERS,
    // it should be 20x20x20 centimers in size.

    spawn_entity(EN_PLAYER, -1.0 + 10.0 * CENTIMETERS_TO_METERS,
                            -1.0 + 10.0 * CENTIMETERS_TO_METERS, z,
                             0.0, 0.0, 0.0,
                            10.0 * CENTIMETERS_TO_METERS);
    spawn_entity(EN_PLAYER, -1.0 + 30.0 * CENTIMETERS_TO_METERS,
                            -1.0 + 10.0 * CENTIMETERS_TO_METERS, z,
                             0.0, 0.0, 0.0,
                            10.0 * CENTIMETERS_TO_METERS);
    spawn_entity(EN_PLAYER, -1.0 + 50.0 * CENTIMETERS_TO_METERS,
                            -1.0 + 10.0 * CENTIMETERS_TO_METERS, z,
                             0.0, 0.0, 0.0,
                            10.0 * CENTIMETERS_TO_METERS);
    spawn_entity(EN_PLAYER, -1.0 + 70.0 * CENTIMETERS_TO_METERS,
                            -1.0 + 10.0 * CENTIMETERS_TO_METERS, z,
                             0.0, 0.0, 0.0,
                            10.0 * CENTIMETERS_TO_METERS);
    spawn_entity(EN_PLAYER, -1.0 + 90.0 * CENTIMETERS_TO_METERS,
                            -1.0 + 10.0 * CENTIMETERS_TO_METERS, z,
                             0.0, 0.0, 0.0,
                            10.0 * CENTIMETERS_TO_METERS);
    spawn_entity(EN_PLAYER, -1.0 + 110.0 * CENTIMETERS_TO_METERS,
                            -1.0 + 10.0 * CENTIMETERS_TO_METERS, z,
                             0.0, 0.0, 0.0,
                            10.0 * CENTIMETERS_TO_METERS);
    spawn_entity(EN_PLAYER, -1.0 + 130.0 * CENTIMETERS_TO_METERS,
                            -1.0 + 10.0 * CENTIMETERS_TO_METERS, z,
                             0.0, 0.0, 0.0,
                            10.0 * CENTIMETERS_TO_METERS);
    spawn_entity(EN_PLAYER, -1.0 + 150.0 * CENTIMETERS_TO_METERS,
                            -1.0 + 10.0 * CENTIMETERS_TO_METERS, z,
                             0.0, 0.0, 0.0,
                            10.0 * CENTIMETERS_TO_METERS);
    spawn_entity(EN_PLAYER, -1.0 + 170.0 * CENTIMETERS_TO_METERS,
                            -1.0 + 10.0 * CENTIMETERS_TO_METERS, z,
                             0.0, 0.0, 0.0,
                            10.0 * CENTIMETERS_TO_METERS);
    spawn_entity(EN_PLAYER, -1.0 + 190.0 * CENTIMETERS_TO_METERS,
                            -1.0 + 10.0 * CENTIMETERS_TO_METERS, z,
                             0.0, 0.0, 0.0,
                            10.0 * CENTIMETERS_TO_METERS);


    spawn_entity(EN_PLAYER, -1.0 + 10.0 * CENTIMETERS_TO_METERS,
                            -1.0 + 30.0 * CENTIMETERS_TO_METERS,
                             z,
                             0.0, 0.0, 0.0,
                            10.0 * CENTIMETERS_TO_METERS);
    spawn_entity(EN_PLAYER, -1.0 + 10.0 * CENTIMETERS_TO_METERS,
                            -1.0 + 50.0 * CENTIMETERS_TO_METERS,
                             z,
                             0.0, 0.0, 0.0,
                            10.0 * CENTIMETERS_TO_METERS);
    spawn_entity(EN_PLAYER, -1.0 + 10.0 * CENTIMETERS_TO_METERS,
                            -1.0 + 70.0 * CENTIMETERS_TO_METERS,
                             z,
                             0.0, 0.0, 0.0,
                            10.0 * CENTIMETERS_TO_METERS);
    spawn_entity(EN_PLAYER, -1.0 + 10.0 * CENTIMETERS_TO_METERS,
                            -1.0 + 90.0 * CENTIMETERS_TO_METERS,
                             z,
                             0.0, 0.0, 0.0,
                            10.0 * CENTIMETERS_TO_METERS);
    spawn_entity(EN_PLAYER, -1.0 + 10.0 * CENTIMETERS_TO_METERS,
                            -1.0 + 110.0 * CENTIMETERS_TO_METERS,
                             z,
                             0.0, 0.0, 0.0,
                            10.0 * CENTIMETERS_TO_METERS);
    spawn_entity(EN_PLAYER, -1.0 + 10.0 * CENTIMETERS_TO_METERS,
                            -1.0 + 130.0 * CENTIMETERS_TO_METERS,
                             z,
                             0.0, 0.0, 0.0,
                            10.0 * CENTIMETERS_TO_METERS);
    spawn_entity(EN_PLAYER, -1.0 + 10.0 * CENTIMETERS_TO_METERS,
                            -1.0 + 150.0 * CENTIMETERS_TO_METERS,
                             z,
                             0.0, 0.0, 0.0,
                            10.0 * CENTIMETERS_TO_METERS);
    spawn_entity(EN_PLAYER, -1.0 + 10.0 * CENTIMETERS_TO_METERS,
                            -1.0 + 170.0 * CENTIMETERS_TO_METERS,
                             z,
                             0.0, 0.0, 0.0,
                            10.0 * CENTIMETERS_TO_METERS);
    spawn_entity(EN_PLAYER, -1.0 + 10.0 * CENTIMETERS_TO_METERS,
                            -1.0 + 190.0 * CENTIMETERS_TO_METERS,
                             z,
                             0.0, 0.0, 0.0,
                            10.0 * CENTIMETERS_TO_METERS);
}

const struct scene sc_first =
{
    .on_load_scene = on_load_scene,
};
