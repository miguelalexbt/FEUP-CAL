#ifndef TRIP_PLANNER_ALGORITHMBEHAVIOUR_H
#define TRIP_PLANNER_ALGORITHMBEHAVIOUR_H

#include <string>
#include <iostream>

#include "trip_planner/core/menu/menu.h"
#include "trip_planner/core/menu/ibehaviour.h"
#include "trip_planner/type/menu_fwd.h"

namespace trip_planner {

	class algorithm_behaviour : public ibehaviour {
	public:
		int display();
	};
}

#endif