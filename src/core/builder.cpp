
#include "trip_planner/core/builder.h"

using namespace std;

namespace trip_planner {

	namespace {
		const double MAX_WALK_DISTANCE = 0.1;
		
		const double WALKING_SPEED = 5.0;
		const double BUS_SPEED = 50.0;
		const double SUBWAY_SPEED = 80.0;

		const double NO_PENALTY = 1.0;
		const double ZONE_PENALTY = 1.95;

		const double DISTANCE_PRIORITY = 0.25;
		const double TIME_PRIORITY = 0.5;
		const double PRICE_PRIORITY = 0.25;
	}

	builder::builder() {}

	builder &builder::instance() {
		static builder instance;
		return instance;
	}

	void builder::build(criteria_t option) {

		clean();

		add_vertices();

		add_edges(option);
	}

	void builder::add_vertices() {

		for (auto it = database::instance()._stops.begin(); it != database::instance()._stops.end(); ++it) {
			database::instance()._graph.add_vertex(&(*it).second);
		}
	}

	void builder::add_edges(criteria_t option) {

		auto add_route = [&option, this](route_t route, pubcode_t line_code, mode_t mode) {
			
			if (route.size() > 0) {
				for (size_t i = 0; i < route.size() - 1; ++i) {
					double distance = route[i].second;
					double weight = this->apply_modifiers(option, route[i].first, route[i + 1].first, distance, mode);

					connect(route[i].first, route[i + 1].first, line_code, weight);
				}
			}
		};

		for (auto src = database::instance()._stops.begin(); src != database::instance()._stops.end(); ++src) {

			for (auto dst = next(src, 1); dst != database::instance()._stops.end(); ++dst) {

				double distance = utility::haversine_distance((*src).second.get_coords(), (*dst).second.get_coords());

				if (distance <= MAX_WALK_DISTANCE) {
					double weight = apply_modifiers(option, (*src).first, (*dst).first, distance, WALK);
					connect2way((*src).first, (*dst).first, weight);
				}
			}
		}

		for (auto it = database::instance()._lines.begin(); it != database::instance()._lines.end(); ++it) {

			mode_t mode = (*it).second.get_mode();

			add_route((*it).second.get_route(ASCENDING), (*it).second.get_pubcode(), mode);
			add_route((*it).second.get_route(DESCENDING), (*it).second.get_pubcode(), mode);
		}
	}

	void builder::clean() {
		database::instance()._graph = graph<stop*>();
	}

	double builder::apply_modifiers(criteria_t option, scode_t start, scode_t end, double weight, mode_t mode) {

		auto time = [](double speed, double distance) {
			return distance / speed;
		};

		auto is_same_zone = [](stop start, stop end) {
			return start.get_zone() == end.get_zone();
		};

		switch (option) {
		case DISTANCE:
			return weight;
		case STOP:
			return 1;
		case TIME:
			if (mode == BUS) 
				return time(BUS_SPEED, weight);
			else if (mode == SUBWAY)
				return time(SUBWAY_SPEED, weight);
			else if (mode == WALK)
				return time(WALKING_SPEED, weight);
		case PRICE:
			if (is_same_zone(database::instance()._stops[start], database::instance()._stops[end]))
				return NO_PENALTY;
			else
				return ZONE_PENALTY;
		case REAL:
			return DISTANCE_PRIORITY * apply_modifiers(DISTANCE, start, end, weight, mode) +
				TIME_PRIORITY * apply_modifiers(TIME, start, end, weight, mode) +
				PRICE_PRIORITY * apply_modifiers(PRICE, start, end, weight, mode);
		default:
			return 0;
		}
	}

	void builder::connect(scode_t stop_src, scode_t stop_dst, pubcode_t owner, double weight) {
		database::instance()._graph.add_edge(&database::instance()._stops[stop_src], &database::instance()._stops[stop_dst], owner, weight);
	}

	void builder::connect2way(scode_t stop_src, scode_t stop_dst, double weight) {
		database::instance()._graph.add_edge(&database::instance()._stops[stop_src], &database::instance()._stops[stop_dst], "walk", weight);
		database::instance()._graph.add_edge(&database::instance()._stops[stop_dst], &database::instance()._stops[stop_src], "walk", weight);
	}
}