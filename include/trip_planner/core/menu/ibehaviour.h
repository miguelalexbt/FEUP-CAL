#ifndef TRIP_PLANNER_IBEHAVIOUR_H
#define TRIP_PLANNER_IBEHAVIOUR_H

namespace trip_planner {

	class ibehaviour {
	public:
		virtual ~ibehaviour() = 0;

		/** Virtual function, determines the functionality of the menu.
		*	@return Equal to 0 if it wishes to continue to run the menus, equal 1 if it wishes to stop running the menus. */
		virtual int display() = 0;
	};
}

#endif