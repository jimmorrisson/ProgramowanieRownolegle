#pragma once

#define FROM_MAIN 1
#define FROM_WORKER 2

#define CONTINUE_RUNNING 0
#define STOP_RUNNING 1

namespace config
{
	extern int workersNumber;
}