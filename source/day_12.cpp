//\==============================================================================
// Advent of code 2022 Day 11
// Question: https://adventofcode.com/2022/day/12
// Gist: The inevitable A* problem from point S reach Point E 
// Input data: a map of letter values from a-z indicating height with start point S
// and end point E. Can only move if adjacent height equal to or lower than 1
// a -> b is ok, a-> c is not allowed. c -> a is ok too.
//\==============================================================================


#include "days.h"
#include "vec2.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>
#include <queue>
#include <stack>
#include <string>
#include <vector>
#include <Windows.h>


void reset_stream( std::ifstream& stream )
{
	stream.clear();
	stream.seekg( 0, std::ios::beg );
}

//gets the number of lines in an input stream, sets the read marker back to the start of the stream
int32_t get_row_count( std::ifstream& stream )
{
	//ensure stream is set to 0 pos.
	reset_stream( stream );
	//get the size of the file for populating a 2D array
	auto total_rows = static_cast<int32_t>(std::count( std::istreambuf_iterator<char>( stream ), std::istreambuf_iterator<char>(), '\n' ));
	//slight issue if the line doesn't end with a \n character we may need to add one to the total_rows
	if ( stream.tellg() != 0 )
	{
		//get the last  character that we parsed and check to see if it was a '\n' if it wasn't then add 1 to rows
		stream.unget();
		if ( stream.get() != '\n' )
		{
			++total_rows;
		}
	}
	//reset stream
	reset_stream( stream );
	return total_rows;
}

int32_t get_column_count( std::ifstream& stream )
{
	reset_stream( stream );
	if ( std::string line; std::getline( stream, line ) )
	{
		reset_stream( stream );
		return static_cast<int32_t>(line.length());
	}
	return 0;
}


void populate_map( std::vector<int8_t>& map, int32_t rows, int32_t cols, std::ifstream& stream )
{
	//reserve the maps capacity
	map.reserve( rows * cols );
	//process each line of the file and populate the forest grid
	for ( std::string line; std::getline( stream, line ); )
	{
		for ( const char c : line )
		{
			map.push_back( c );
		}
	}
}




void locate_start_and_end_points( const std::vector<int8_t>& map, int32_t width, vec2& start_pos, vec2& end_pos )
{
	for( int32_t index{}; index < map.size(); ++index )
	{
		if( map[index] == 'S' )
		{
			start_pos = vec2{ index % width, index / width };
		}
		if ( map[index] == 'E' )
		{
			end_pos = vec2{ index % width, index / width };
		}
		//early out of the loop if both values have been set.
		if( start_pos.x != -1 && end_pos.x != -1 )
		{
			break;
		}
	}
}

struct a_star_node
{
	vec2 pos{};

	int32_t f{};
	int32_t g{};
	int32_t h{};

	a_star_node* parent;
	
};

bool is_neighbour_valid( const std::vector<int8_t>& map, const int32_t rows, const int32_t cols, const vec2& neighbour, const vec2& parent, int8_t& height_diff )
{
	if (neighbour.x >= 0 && neighbour.x < cols && neighbour.y >= 0 && neighbour.y < rows)
	{
		int8_t neighbour_value = map[neighbour.x + (neighbour.y * cols)];
		int8_t parent_value = map[parent.x + (parent.y * cols)];
		//E has elevation z
		if(  neighbour_value == 'E' )
		{
			neighbour_value = 'z';
		}
		else if ( neighbour_value == 'S' )
		{
			neighbour_value = 'a';
		}
		//if this is the first node the S == a;
		else if( parent_value == 'S' )
		{
			parent_value = 'a';
		}
		else if ( parent_value == 'E' )
		{
			parent_value = 'z';
		}

		height_diff = neighbour_value - parent_value;
		if ( height_diff >=  -1 )
		{
			return true;
		}
	}
	return false;
}

void print_path( a_star_node* path_end, const std::vector<a_star_node*>& closed_graph, int32_t rows, int32_t cols, int32_t first_low_point )
{
	system( "cls" );
	std::vector<std::string> lines{};

	for ( int32_t r = 0; r < rows; ++r )
	{
		std::string row{};
		for ( int32_t c = 0; c < cols; ++c )
		{
			vec2 pos{ c, r };
			if ( std::find_if( closed_graph.begin(), closed_graph.end(), [&pos]( const a_star_node* node ){ return pos == node->pos; } ) != closed_graph.end() )
			{
				row.push_back( ' ' );
			}
			else
			{
				row.push_back( '.' );
			}

		}
		lines.push_back( row );
	}

	a_star_node* path_point = path_end;
	int8_t val = 'E';
	while ( path_point != nullptr )
	{
		lines[path_point->pos.y][path_point->pos.x] = val;
		
		if ( path_point->parent )
		{
			if ( path_point->g == first_low_point )
			{
				val = '@';
			}
			else
			{
				vec2 direction = path_point->pos - path_point->parent->pos;
				if ( direction.x )
				{
					val = direction.x > 0 ? '>' : '<';
				}
				if ( direction.y )
				{
					val = direction.y > 0 ? 'V' : '^';
				}
			}
		}
		path_point = path_point->parent;
	}

	for ( auto line : lines )
	{
		std::cout << line << std::endl;
	}
	
}

a_star_node* perform_astar_routine( const std::vector<int8_t>& map, int32_t rows, int32_t cols, const vec2& start, const vec2& end, std::vector<a_star_node*>& closed_graph, int32_t& steps_to_first_location_at_height_a )
{
	std::vector<a_star_node*> open_list;
	//push the start node into the openlist
	open_list.push_back( new a_star_node{ start, 0, 0, 0, nullptr } );
;	while( !open_list.empty() )
	{
		//sort the vector to put the item with the lowest score at the rear
		std::sort( open_list.begin(), open_list.end(), []( const a_star_node* l, const a_star_node* r ) {  return l->f > r->f; } );
		//get the first item from the queue then pop it
		a_star_node* current_node = open_list.back();
		open_list.pop_back();
		//is this item the end item?
		if( current_node->pos == end )
		{
			return current_node;
		}
		
		//add this position to the closed list
		closed_graph.push_back( current_node );
		//if not then get NESW neighbours and add to queue
		std::vector<vec2> neighbours = { current_node->pos - vec2{ 0, -1 }, current_node->pos - vec2{ 1, 0 }, current_node->pos - vec2{ 0, 1 }, current_node->pos - vec2{ -1, 0 } };

		for( vec2& neighbour : neighbours )
		{
			int8_t height_diff{};
			//test to see if this neighbour is in our map
			if( is_neighbour_valid( map, rows, cols, neighbour, current_node->pos, height_diff ) )
			{
				//calculate g h & f
				const int32_t g = current_node->g + 1;
				const int32_t h = end.manhattan_dist(end) + height_diff;
				const int32_t f = g + h;
				//find out if this location is at height 'a'
				int8_t neighbour_node_value = map[neighbour.x + (neighbour.y * cols)];
				if ( neighbour_node_value == 'a' && g < steps_to_first_location_at_height_a  )
				{
					steps_to_first_location_at_height_a = g;
				}

				auto already_closed = std::find_if( closed_graph.begin(), closed_graph.end(), [&neighbour]( const a_star_node* node ){ return neighbour == node->pos; } );
				//if the neighbour isn't on the closed list
				if ( already_closed == closed_graph.end() )
				{
					//is the location in the open list already
					auto it = std::find_if( open_list.begin(), open_list.end(), [&neighbour]( const a_star_node* node ){ return neighbour == node->pos; } );
					if( it != open_list.end() )
					{
						//our item is on the open list already if our f score is lower then we should replace
						if ( f < (*it)->f )
						{
							(*it)->f = f;
							(*it)->g = g;
							(*it)->h = h;
							(*it)->parent = current_node;
						}
					}
					else
					{
					
						//first time visiting this square push back
						open_list.push_back( new a_star_node{ neighbour, f, g, h, current_node } );
					}
				}
				
			}
		}
		//print_path( current_node, closed_graph, rows, cols );
		//Sleep( 2 );
	}
	//return an nullptr if no path is found
	return nullptr;
}



void aoc::day_12()
{
	int32_t rows{}, cols{};
	std::vector<int8_t> map{};

	std::ifstream file_input;
	file_input.open( "./data/day_12_input.txt" );
	if ( file_input.is_open() )
	{
		//populate the map from the input
		rows = get_row_count( file_input );
		cols = get_column_count( file_input );
		populate_map( map, rows, cols, file_input );
		file_input.close();
	}

	if( !map.empty() )
	{
		//locate start and end points of map
		vec2 start_pos{-1,-1}, end_pos{-1,-1};
		locate_start_and_end_points( map, cols, start_pos, end_pos );
		//Perform A* routine
		std::vector<a_star_node*> closed_graph{};
		int32_t steps_to_first_location_at_height_a = 100000;
		a_star_node* path = perform_astar_routine( map, rows, cols, end_pos, start_pos, closed_graph, steps_to_first_location_at_height_a);
		if ( path != nullptr )
		{
			print_path( path, closed_graph, rows, cols, steps_to_first_location_at_height_a );
			std::cout << "Path Length: " << path->g << std::endl;
			std::cout << "Steps to first A location: " << steps_to_first_location_at_height_a << std::endl;
		}

		//clean up all nodes in graph
		for( a_star_node* node : closed_graph )
		{
			delete node;
			node = nullptr;
		}
		closed_graph.clear();
	}
	//number of lines in file
	std::cout << "Line Count: " << rows << std::endl;
	

}
