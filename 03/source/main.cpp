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


int main( int argc, char* argv[] )
{
	//Vector to store the priority values for each common type
	std::vector< uint32_t > priority_values;
	uint32_t total_priority_values = 0;
	uint32_t total_badge_priority_sum = 0;

	//open the input file for reading
	std::ifstream file_input;
	file_input.open( "./data/input.txt" );
	if ( file_input.is_open() )
	{
		//Keep a map to score the characters as they appear in the input string
		uint32_t full_inventory_map[256]{};
		
		//process each line of the file
		for ( std::string line; std::getline( file_input, line ); )
		{
			constexpr uint32_t lower_map_start = 'a';
			constexpr uint32_t upper_map_start = 'A';
			//======================================================================================
			// Part 1 - find the common characters in each compartment  of the elves bag
			//======================================================================================
			//Keep a map to score the characters as they appear in each half of the input string
			uint32_t inventory_map[256]{};
			//split each line in half, 1st half first compartment, 2nd half 2nd compartment
			std::string first_compartment = line.substr( 0, line.length() >> 1 );
			std::string second_compartment = line.substr( line.length() >> 1 );
			//iterate over the first string and add a 1 to the index in the character map for each letter that occurs
			for ( const unsigned char c : first_compartment )
			{
				inventory_map[static_cast<uint32_t>(c)] = 1;
			}
			//iterate over the second string and if an index already has a value of 1 in it then we have found the misplaced item
			for ( const unsigned char c : second_compartment )
			{
				if ( inventory_map[static_cast<uint32_t>(c)] == 1 )
				{
					uint32_t offset = upper_map_start;
					uint32_t base_score = 27;
					if ( c > lower_map_start ) //this is a lower case letter
					{
						offset = lower_map_start;
						base_score = 1;
					}
					const uint32_t priority_score = static_cast<uint32_t>(c) - offset + base_score;
					priority_values.push_back( priority_score );
					total_priority_values += priority_score;
					break;
				}
			}
			//======================================================================================
			// Part 2 - Find the badges and get the badge priority
			//======================================================================================
			//Get only the unique characters in a line -- strip out any re-occurring characters
			std::sort( line.begin(), line.end() );	//sort the string first
			//std unique will move only unique characters to the front of the string and returns an index to the end of the unique characters
			const auto index = std::unique( line.begin(), line.end() );
			const std::string unique_chars_line = line.substr( 0, index - line.begin() );
			//map each character into the full inventory string
			for ( const unsigned char c : unique_chars_line )
			{
				full_inventory_map[static_cast<uint32_t>(c)] += 1;
				if( full_inventory_map[static_cast<uint32_t>(c)] == 3 ) //we have found the item in common
				{
					
					uint32_t offset = upper_map_start;
					uint32_t base_score = 27;
					if ( c > lower_map_start ) //this is a lower case letter
					{
						offset = lower_map_start;
						base_score = 1;
					}
					const uint32_t priority_score = static_cast<uint32_t>(c) - offset + base_score;
					total_badge_priority_sum += priority_score;
					memset( full_inventory_map, 0, sizeof( uint32_t ) * 256 );
					break;
				}
			}
			
			
		}
		//close the input file
		file_input.close();
	}

	//if we collected any input then let's sort the array to find the elf with the most calories
	if ( !priority_values.empty() )
	{
		std::cout << "Number of packs incorrectly packed: " << priority_values.size() << std::endl;
		//Part 1 - What is the priority score for items in the wrong compartments
		std::cout << "The total priority score: " << total_priority_values << std::endl;
		//Part 2 - what is the value of the badge priority items for the elves in groups?
		std::cout << "The sum of the three elves badge priority is: " << total_badge_priority_sum << std::endl;

	}


	return 0;
}
