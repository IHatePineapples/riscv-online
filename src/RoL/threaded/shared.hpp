#pragma once

#include "RoL/threaded/queue.hpp"
#include "RoL/threaded/vector.hpp"

/**
 * \file Defines shared variables and memory accross compilation units.
 *
 * \note Placed in the `./threaded` directory because that's the main use case.
 */

namespace emulation
{
    class emulator;
}

using job_t = std::pair<unsigned char /** id */, emulation::emulator /** state */>;
using job_queue = concurrent_queue<job_t>;
using job_vector = concurrent_vector<job_t>;

extern job_queue in_jq_;
extern job_vector out_jq_;
extern job_t current_job_;
