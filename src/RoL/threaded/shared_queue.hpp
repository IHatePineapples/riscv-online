#pragma once

#include "RoL/threaded/queue.hpp"

/**
 * \file Defines shared variables and memory accross compilation units.
 * 
 * \note Placed in the `./threaded` directory because that's the main use case.
*/

extern job_queue in_jq_;
extern job_queue out_jq_;
