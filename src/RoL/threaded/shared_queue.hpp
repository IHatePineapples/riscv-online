#pragma once

#include "RoL/threaded/queue.hpp"

/**
 * \file Defines shared variables and memory accross compilation units.
 * 
 * \note Placed in the `./threaded` directory because that's the main use case.
*/

namespace emulation{
    class emulator;
}

using job_t = std::pair<unsigned char /** id */, emulation::emulator /** state */>;
using job_queue = concurrent_queue<job_t>;

extern job_queue in_jq_;
extern job_queue out_jq_;
