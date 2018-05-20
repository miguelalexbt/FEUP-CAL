#ifndef TRIP_PLANNER_CRITERIABEHAVIOUR_H
#define TRIP_PLANNER_CRITERIABEHAVIOUR_H

#include <string>
#include <iostream>

#include "trip_planner/core/menu/menu.h"
#include "trip_planner/core/menu/ibehaviour.h"

namespace trip_planner {

	class criteria_behaviour : public ibehaviour {
	public:
		int display();
	};
}

#endif