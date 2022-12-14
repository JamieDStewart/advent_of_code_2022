//\==============================================================================
// Advent of code 2022 Day 8
// Question: https://adventofcode.com/2022/day/8
// Gist:  Count the number of visible trees in a grid
// Input data: A matrix of tree heights represented by a single character 0-9
//\==============================================================================
#include "days.h"

#include <algorithm>
#include <bitset>
#include <fstream>
#include <iostream>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>

void set_stream_to_start( std::ifstream& stream )
{
	stream.clear();
	stream.seekg( 0, std::ios::beg );
}

//gets the number of lines in an input stream, sets the read marker back to the start of the stream
int32_t get_number_of_lines_in_stream( std::ifstream& stream )
{
	//ensure stream is set to 0 pos.
	set_stream_to_start( stream );
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
	set_stream_to_start( stream );
	return total_rows;
}

int32_t get_number_of_characters_in_row( std::ifstream& stream )
{
	set_stream_to_start( stream );
	if ( std::string line; std::getline( stream, line ) )
	{
		set_stream_to_start( stream );
		return static_cast<int32_t>(line.length());
	}
	return 0;
}

int8_t* initialise_forest_array( int32_t rows, int32_t cols )
{
	auto* forest_array = new int8_t[rows*cols];
	return forest_array;
}

void populate_forest( int8_t* forest, int32_t cols, std::ifstream& stream )
{

	int32_t current_row = 0;
	//process each line of the file and populate the forest grid
	for ( std::string line; std::getline( stream, line ); )
	{
		int32_t current_column{};
		for ( const char c : line )
		{
			const int8_t tree_height = std::atoi( &c );
			forest[current_column + (current_row * cols)] = tree_height;
			++current_column;
		}
		++current_row;
	}
}


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

};


void scan_through_grid_section( const int8_t* grid, const vec2 start_pos, const vec2 direction, const vec2 size, std::vector<uint8_t>& visible_trees )
{
	vec2 current_pos = start_pos;
	int8_t current_height_max = -1;
	
	//do this until we reach an edge or find a tree that's too tall
	do
	{
		const int32_t index = current_pos.y + (current_pos.x * size.y);
		const int8_t tree_height = grid[index];
		if( tree_height > current_height_max )
		{
			current_height_max = tree_height;
			visible_trees[index] = 1;
			if ( current_height_max == 9 ){ break; }
		}
		current_pos = current_pos + direction;

	}
	while ( current_pos.x >= 0 && current_pos.y >= 0 && current_pos.x < size.x && current_pos.y < size.y );
}

void scan_through_grid( const int8_t* grid, const vec2 size, std::vector<uint8_t>& visible_trees )
{
	//for all rows
	for ( int32_t r = 0; r < size.x; ++r )
	{
		scan_through_grid_section( grid, { r,0 }, { 0,1 }, size, visible_trees );
		scan_through_grid_section( grid, { size.x - 1 - r, size.y - 1 }, { 0,-1 }, size, visible_trees );
	}
	//for all columns
	for ( int32_t c = 0; c < size.y; ++c )
	{
		scan_through_grid_section( grid, { 0,c }, { 1,0 }, size, visible_trees );
		scan_through_grid_section( grid, { size.x - 1, size.y - 1 - c }, { -1,0 }, size, visible_trees );
	}
}

int32_t trees_visible_from_location_in_direction( const int8_t* grid, const vec2 start_pos, const vec2 direction, const vec2 size )
{
	int32_t visible_trees = 0;
	vec2 current_pos = start_pos;
	const int8_t start_tree_height = grid[start_pos.y + (start_pos.x * size.y)];
	int8_t tree_height = 0;
	//do this until we reach an edge or find a tree that's too tall
	do
	{
		current_pos = current_pos + direction;
		if ( current_pos.x < 0 || current_pos.y < 0 || current_pos.x >= size.x || current_pos.y >= size.y )
		{
			break;
		}
		visible_trees++;

		const int32_t index = current_pos.y + (current_pos.x * size.y);
		tree_height = grid[index];

	}
	while ( tree_height < start_tree_height );
	return visible_trees;
}

int32_t get_location_with_highest_visibility( const int8_t* grid, const vec2 size)
{
	int32_t max_visibility = -1;
	//for all rows
	for ( int32_t r = 0; r < size.x; ++r )
	{
		//for all columns
		for ( int32_t c = 0; c < size.y; ++c )
		{
			const int32_t north = trees_visible_from_location_in_direction( grid, { r,c }, { -1,0 }, size );
			const int32_t east = trees_visible_from_location_in_direction( grid, { r,c }, { 0,1 }, size );
			const int32_t south = trees_visible_from_location_in_direction( grid, { r,c }, { 1,0 }, size );
			const int32_t west = trees_visible_from_location_in_direction( grid, { r,c }, { 0,-1 }, size );

			const int32_t visibility = north * south * east * west;
			if ( visibility > max_visibility )
			{
				max_visibility = visibility;
			}

		}
	}
	return max_visibility;
	
}

void aoc::day_08()
{
	//forest 2D array of uint8_t values
	int8_t* forest_grid = nullptr;
	int32_t rows{};
	int32_t cols{};

	std::ifstream file_input;
	file_input.open( "./data/day_08_input.txt" );
	if ( file_input.is_open() )
	{
		
		//Get the total number of lines in the input file
		rows = get_number_of_lines_in_stream( file_input );
		std::cout << "Number of rows: " << rows << std::endl;
		//get the total number of columns in the input file
		cols = get_number_of_characters_in_row( file_input );
		std::cout << "Number of columns: " << cols << std::endl;
		//Create a 2D array of num_rows by num_cols
		forest_grid = initialise_forest_array( rows, cols );
		populate_forest( forest_grid, cols, file_input );
		
		file_input.close();
	}

	//use a vector as a bitset to enable single values as visible trees
	std::vector< uint8_t > visible_trees( rows * cols );
	//Go through the grid and mark trees as being visible
	scan_through_grid( forest_grid, { rows,cols }, visible_trees );

	
	const int32_t visible_count = std::accumulate( visible_trees.begin(), visible_trees.end(), 0 );
	std::cout << "Trees Visible: " << visible_count << std::endl;

	const int32_t max_visibility = get_location_with_highest_visibility( forest_grid, { rows,cols } );
	std::cout << "Max Trees Visible: " << max_visibility << std::endl;
	
}
