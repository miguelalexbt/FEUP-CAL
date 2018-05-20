#ifndef TRIP_PLANNER_MENU_H
#define TRIP_PLANNER_MENU_H

#include "trip_planner/core/database.h"
#include "trip_planner/core/menu/menu_behaviour.h"
#include "trip_planner/type/menu_fwd.h"
#include "trip_planner/utility/io.h"

namespace trip_planner {

	class menu {

	public:
		
		/** Menu's instance. */
		static menu &instance();

		/** Starts the Menu. */
		void run();

		/** @return Criteria of the search. */
		criteria_t get_criteria() const;

		/** @return Search mode. */
		search_t get_search_mode() const;

		/** @return Algorithm to be used. */
		algorithm_t get_algorithm() const;

		/** @return Initial stop code. */
		scode_t get_source() const;

		/** @return Final stop code. */
		scode_t get_destination() const;

		/** Sets menu behaviour. */
		void set_behaviour(menu_t menu);

		/** Sets search criteria. */
		void set_criteria(criteria_t criteria);

		/** Sets search mode. */
		void set_search_mode(search_t search_mode);

		/** Sets algorithm to be used. */
		void set_algorithm(algorithm_t algorithm);

		/** Sets Initial and Final stop codes. */
		void set_stops(scode_t src, scode_t dst);

		/** Calls the display method from the current _behaviour.
		*	@return Equal to 0 if it wishes to continue to run the menus, equal 1 if it wishes to stop running the menus. */
		int display();

	private:

		/** Creates an instance of menu. */
		menu();

		/** Behaviour of the menu. */
		behaviour_t _behaviour;

		/** Search criteria. */
		criteria_t _criteria;

		/** Search mode. */
		search_t _search_mode;

		/** Search algorithm. */
		algorithm_t _algorithm;

		/** Initial stop. */
		scode_t _src;

		/** Final stop. */
		scode_t _dst;
	};
}

#endif