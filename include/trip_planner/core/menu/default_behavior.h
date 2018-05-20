#ifndef TRIP_PLANNER_DEFAULTBEHAVIOUR_H
#define TRIP_PLANNER_DEFAULTBEHAVIOUR_H

#include <string>
#include <iostream>

#include "trip_planner/core/menu/menu.h"
#include "trip_planner/core/menu/ibehaviour.h"
#include "trip_planner/type/menu_fwd.h"

namespace trip_planner {

	class default_behaviour : public ibehaviour {
	public:
		int display();
	};
}

#endif
