#include <cstdint>

#ifndef UTILS_H_
#define UTILS_H_


class Position {

	public:

		float x;
		float y;
		float z;

		Position(const float _x, const float _y, const float _z)
			: x(_x), y(_y), z(_z)
		{}

		bool operator<(const Position &lhs, const Position &rhs)
		{
			if (lhs.x == rhs.x) {
				if (lhs.y == rhs.y) {
					if(lhs.z == rhs.z) {
						return 0;
					} else {
						return lhs.z < rhs.z;
					}
				} else {
					return lhs.y < rhs.y;
				}
			} else {
				return lhs.x < rhs.x;
			}
		} 
};


#endif // UTILS_H_