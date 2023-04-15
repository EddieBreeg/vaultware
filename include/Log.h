
#pragma once

#if DEBUG
#include <iostream>
#define DEBUG_LOG(x)	std::cout << x
#else
#define DEBUG_LOG(x)
#endif