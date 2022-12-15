#pragma once
#include <cstdint>
#include <cstdlib>

struct vec2
{
	int32_t x;
	int32_t y;

	bool operator == ( const vec2& rhs ) const
	{
		return (x == rhs.x && y == rhs.y);
	}

	bool operator != ( const vec2& rhs ) const
	{
		return (x != rhs.x || y != rhs.y);
	}

	vec2 operator +( const vec2& rhs )const
	{
		return { x + rhs.x, y + rhs.y };
	}

	vec2 operator -( const vec2& rhs )const
	{
		return { x - rhs.x, y - rhs.y };
	}

	[[nodiscard]]int32_t manhattan_dist( const vec2& b ) const
	{
		return abs(x - b.x) + abs(y - b.y);
	}

	[[nodiscard]] int32_t distance( const vec2& b) const
	{
		int32_t xd = x - b.x; int32_t yd = y - b.y;
		return static_cast<int32_t>(sqrt( xd * xd + yd*yd));
	}

};
