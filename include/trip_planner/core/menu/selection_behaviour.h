#ifndef TRIP_PLANNER_SELECTIONBEHAVIOUR_H
#define TRIP_PLANNER_SELECTIONBEHAVIOUR_H

#include <string>
#include <iostream>
#include <vector>

#include "trip_planner/core/menu/menu.h"
#include "trip_planner/core/menu/ibehaviour.h"

namespace trip_planner {

	class selection_behaviour : public ibehaviour {
	public:
		int display();
	private:
		scode_t process_response(const std::vector<scode_t> &res);
	};
}

#endif