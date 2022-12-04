//\==============================================================================
// Advent of code 2022 Day 3
// Question: https://adventofcode.com/2022/day/3
// Gist:  Backpacks with items in each compartment, find common items and sum their value
//        part 2 - every 3 elves have an item in common find the priority for that item
// Input data: calorie values, elves separated by blank line
//\==============================================================================

#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>

struct cleaning_area
{
	int32_t start;
	int32_t end;
	int32_t range;
};

bool compare_by_start( const cleaning_area& a, const cleaning_area& b )
{
	//if the cleaning ranges start on the same area sort based off the one that covers the largest range
	if( a.start == b.start ) 
	{
		return a.range > b.range;
	}
	return a.start < b.start;
}

int main( int argc, char* argv[] )
{
	//Vector to store the priority values for each common type
	uint32_t number_of_completely_overlapped_areas{};
	uint32_t partially_overlapped_areas{};
	uint32_t line_count{};
	//open the input file for reading
	std::ifstream file_input;
	file_input.open( "./data/input.txt" );
	if ( file_input.is_open() )
	{
		//process each line of the file
		for ( std::string line; std::getline( file_input, line ); )
		{
			line_count++;
			std::vector<cleaning_area> cleaning_areas{};
			std::string temp_string{};
			std::stringstream input_line( line );
			//split the input string on the ',' character
			while ( std::getline( input_line, temp_string, ',') )
			{
				//string is now in format xx-xx where x are numbers
				int32_t start{}, end{};
				std::size_t dash_pos = temp_string.find( '-' );
				std::stringstream ( temp_string.substr( 0, dash_pos ) ) >> start;
				std::stringstream ( temp_string.substr( dash_pos+1) ) >> end;
				cleaning_areas.push_back( { start, end, end - start } );
			}
			//sorting the vector will put the cleaning areas in for form of min -> max for starting values
			std::sort( cleaning_areas.begin(), cleaning_areas.end(), compare_by_start );
			//as sorted based on minimum we already know that cleaning_areas[0].start <= cleaning_areas[1].start for all areas
			//Basic box collision test if area 0 fully contains area 1
			if( cleaning_areas[0].end   >= cleaning_areas[1].end  )
			{
				//fully overlapped
				number_of_completely_overlapped_areas++;
			}
			//test for any overlap if area 1 starts before area 0 ends
			if ( cleaning_areas[1].start <= cleaning_areas[0].end )
			{
				partially_overlapped_areas++;
			}
			
		}
		//close the input file
		file_input.close();
	}

	std::cout << "Number of lines in input file: " << line_count << std::endl;
	//Part 1 - How many cleaning areas are overlapped
	std::cout << "Number of completely overlapped cleaning areas: " << number_of_completely_overlapped_areas << std::endl;
	//part 2 - partially overlapped areas
	std::cout << "Number of partially overlapped cleaning areas: " << partially_overlapped_areas << std::endl;
	

	return 0;
}
