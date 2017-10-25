
#pragma once

#include <string>

#include "types.h"
#include "render.h"


/*
    Make sure the main subsystems necessary to run the
    program are initialized.
*/
bool init();


/*
    Make sure the global variables which
    update every frame are set properly,
    such as `ticks_total` and `ticks_delta`
*/
void update_globals();


