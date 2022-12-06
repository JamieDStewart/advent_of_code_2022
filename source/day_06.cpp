//\==============================================================================
// Advent of code 2022 Day 6
// Question: https://adventofcode.com/2022/day/6
// Gist:  find a sequence of characters where non-occurence happens
// Input data: one long string of ascii characters
//\==============================================================================
#include "days.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>

//function returns true if a string contains only unique characters 
bool string_contains_only_unique_characters( std::string search_string )
{
	//Use std::sort to sort the string in to ascii order then use std::unique to strip out any repeated characters
	std::sort( search_string.begin(), search_string.end() );
	const auto unique_length = std::unique( search_string.begin(), search_string.end() );
	//if the length of the unique characters in the substring is the same as the sequence length then we've found the marker
	return (unique_length - search_string.begin() == search_string.length());
}

void aoc::day_06()
{
	std::ifstream file_input;
	file_input.open( "./data/day_06_input.txt" );
	if ( file_input.is_open() )
	{
		//process each line of the file
		for ( std::string line; std::getline( file_input, line ); )
		{
			constexpr int32_t part_1_sequence_length{ 4 };
			constexpr int32_t part_2_sequence_length{ 14 };
			//std::unique seems like the perfect candidate for this programming problem
			for( auto index_pos = line.begin(); index_pos != line.end()-(part_1_sequence_length -1); ++index_pos )
			{
				//Part 1 : Unique Sequence length 4 -- if found set found marker to true to stop looking and allow part 2 to continue
				static bool part_1_found = false;
				if ( !part_1_found && string_contains_only_unique_characters( std::string( index_pos, index_pos + part_1_sequence_length ) ) )
				{
					//Get the location of the marker
					const int32_t sequence_location = static_cast<int32_t>(index_pos - line.begin()) + part_1_sequence_length;
					std::cout << "Part 1 Sequence: " << std::string( index_pos, index_pos + part_1_sequence_length ) << " Sequence location: " << sequence_location << std::endl;
					part_1_found = true;
				}
				//Part 2 : unique sequence length is 14, so once this is found the sequence for part_1 will have been found already so stop scanning the file (break)
				if ( index_pos + part_2_sequence_length < line.end() )
				{
					if ( string_contains_only_unique_characters( std::string( index_pos, index_pos + part_2_sequence_length ) ) )
					{
						//Get the location of the marker
						const int32_t sequence_location = static_cast<int32_t>(index_pos - line.begin()) + part_2_sequence_length;
						std::cout << "Part 2 Sequence: " << std::string( index_pos, index_pos + part_2_sequence_length ) << " Sequence location: " << sequence_location << std::endl;
						break;
					}
				}
			}
			
					
		}
		file_input.close();
	}

}
