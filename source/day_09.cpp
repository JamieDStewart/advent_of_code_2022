//\==============================================================================
// Advent of code 2022 Day 9
// Question: https://adventofcode.com/2022/day/9
// Gist:  Head and tail of a moving rope, work out which unique positions the tail visits
// Input data: Rope head movement directions in the form of D 10 where D is one of the four cardinal directions
//\==============================================================================
#include "days.h"


#include <fstream>
#include <iostream>
#include <numeric>
#include <sstream>
#include <string>
#include <set>
#include <vector>

struct vec2
{
	int32_t x = 0;
	int32_t y = 0;

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

	vec2 operator +=( const vec2& rhs )
	{
		*this = *this + rhs;
		return *this;
	}

	vec2 operator -( const vec2& rhs )const
	{
		return { x - rhs.x, y - rhs.y };
	}


};

vec2 get_direction( const char command )
{
	//possible commands
	constexpr char up = 'U';
	constexpr char down = 'D';
	constexpr char left = 'L';
	constexpr char right = 'R';

	switch ( command )
	{
	case up:
		{
			return { 0, 1 };
		}
	case down:
		{
			return { 0, -1 };
		}
	case left:
		{
			return { -1,0 };
		}
	case right:
		{
			return { 1,0 };
		}
	default:
		return { 0,0 };
	}
}

void move_rope( vec2 direction, std::vector<vec2>& rope, std::set<std::pair<int32_t, int32_t>>& tail_visited_locations )
{	
	*(rope.begin()) += direction;
	for ( auto iter = rope.begin() + 1; iter != rope.end(); ++iter )
	{
		const vec2 distance = (*(iter - 1)) - (*iter);
		//if the distance is greater than 1 in a direction then move the tail in that direction
		if ( abs( distance.x ) > 1 || abs( distance.y ) > 1 )
		{
			if ( distance.x ) { (*iter).x += (distance.x < 0) ? -1 : 1; }
			if ( distance.y ) { (*iter).y += (distance.y < 0) ? -1 : 1; }
			//tail has moved add now position to

		}
	}
	tail_visited_locations.insert( { (*(rope.end() - 1)).x, (*(rope.end() - 1)).y } );

}


void aoc::day_09()
{
	int32_t line_count{};

	//Part 1 rope with two segments a head and a tail
	std::vector<vec2> part_1_rope( 2 );
	std::vector<vec2> part_2_rope( 10 );
	//set vectors for the head and tail
	std::set<std::pair<int32_t, int32_t>> part_1_tail_visited_locations;
	std::set<std::pair<int32_t, int32_t>> part_2_tail_visited_locations;
	part_1_tail_visited_locations.insert( { part_1_rope[part_1_rope.size()-1].x, part_1_rope[part_1_rope.size() - 1].y});
	part_2_tail_visited_locations.insert( { part_2_rope[part_2_rope.size() - 1].x, part_2_rope[part_2_rope.size() - 1].y } );
	std::ifstream file_input;
	file_input.open( "./data/day_09_input.txt" );
	if ( file_input.is_open() )
	{
		for ( std::string line; std::getline( file_input, line ); )
		{
			//lines take the form of 'D' 'C' where D is direction and C is count
			const char command = line[0];
			int32_t count{};
			std::stringstream( line.substr( line.find_last_of( ' ' ) + 1 ) ) >> count;
			std::cout << "Command: " << command << " Count: " << count << std::endl;
			const vec2 direction = get_direction( command );
			for ( int i = 0; i < count; ++i )
			{
				move_rope( direction, part_1_rope, part_1_tail_visited_locations );
				move_rope( direction, part_2_rope, part_2_tail_visited_locations );
			}
			

			line_count++;
		}		

		file_input.close();
	}

	//number of lines in file
	std::cout << "Line Count: " << line_count << std::endl;
	std::cout << "Number of positions tail visited: " << part_1_tail_visited_locations.size() << std::endl;
	std::cout << "Number of positions tail visited: " << part_2_tail_visited_locations.size() << std::endl;

}
