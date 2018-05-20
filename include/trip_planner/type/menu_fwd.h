#ifndef TRIP_PLANNER_MENUFWD_H
#define TRIP_PLANNER_MENUFWD_H

#include <memory>

namespace trip_planner {

	class ibehaviour;

	/** Existing menus */
	typedef enum { DEFAULT, CRITERIA, ALGORITHM, SEARCH, SELECTION, GRAPH }	menu_t;

	/** Existing search modes */
	typedef enum { BY_STOP, BY_LINE }										search_t;

	/** Smart Pointer to a class that implements ibehaviour */
	typedef std::shared_ptr<ibehaviour>										behaviour_t;
}
  
#endif