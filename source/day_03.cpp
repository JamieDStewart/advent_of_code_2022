//\==============================================================================
// Advent of code 2022 Day 3
// Question: https://adventofcode.com/2022/day/3
// Gist:  Backpacks with items in each compartment, find common items and sum their value
//        part 2 - every 3 elves have an item in common find the priority for that item
// Input data: backpack items as a string of ascii characters
//\==============================================================================
#include "days.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

//strip repeat characters from string
std::string strip_repeated_characters( std::string string_to_strip )
{
	//Get only the unique characters in a line -- strip out any re-occurring characters
	std::sort( string_to_strip.begin(), string_to_strip.end() );
	//std unique will move only unique characters to the front of the string and returns an index to the end of the unique characters
	const auto index = std::unique( string_to_strip.begin(), string_to_strip.end() );
	//return the substring to strip out the repeated characters
	return string_to_strip.substr( 0, index - string_to_strip.begin() );;
}

//find a common character in a vector of strings
unsigned char find_common_item( const std::vector<std::string>& elf_group )
{
	uint8_t inventory_map[256]{};
	const auto sought_item_count = static_cast<uint8_t>(elf_group.size());
	for ( auto& items : elf_group )
	{
		for ( const uint8_t c : items )
		{
			inventory_map[c] += 1;
			if ( inventory_map[c] == sought_item_count )
			{
				return c;
			}
		}
	}
	return 0;

}
//Find the common character in each half of the string
unsigned char find_common_item_in_backpack( const std::string& input_string )
{
	std::vector<std::string> compartments{};
	//split each line in half, 1st half first compartment, 2nd half 2nd compartment
	compartments.emplace_back(  strip_repeated_characters(input_string.substr( 0, input_string.length() >> 1 )) );
	compartments.emplace_back( strip_repeated_characters(input_string.substr( input_string.length() >> 1 )) );

	return find_common_item( compartments );
}

//work out the priority score for the item provided
uint8_t get_item_priority_score( const uint8_t item )
{
	const uint8_t letter_score = (item > 'a') ? 'a' - 1 : 'A' - 27;
	return item - letter_score;
}

void aoc::day_03()
{
	//Vector to store the priority values for each common type
	uint32_t total_priority_values = 0;
	uint32_t total_badge_priority_sum = 0;

	//open the input file for reading
	std::ifstream file_input;
	file_input.open( "./data/day_03_input.txt" );
	if ( file_input.is_open() )
	{
		//hold a vector for each group of elves - 3 elves in each group
		std::vector< std::string > elf_group{};
		//process each line of the file
		for ( std::string line; std::getline( file_input, line ); )
		{
			//======================================================================================
			// Part 1 - Find the item that is common to both compartments of the backpack
			//======================================================================================
			const uint8_t misplaced_item = find_common_item_in_backpack( line );
			if( misplaced_item )
			{
				total_priority_values += get_item_priority_score( misplaced_item );
			}
			
			//======================================================================================
			// Part 2 - Find the badges and get the badge priority
			//======================================================================================
			//add the current line into the elf group
			elf_group.emplace_back( strip_repeated_characters( line ) );
			//every three elves is a group
			if( elf_group.size() == 3 )
			{
				const uint8_t common_item = find_common_item( elf_group );
				if ( common_item )
				{
					total_badge_priority_sum += get_item_priority_score( common_item );
				}
				elf_group.clear();
			}

		}
		//close the input file
		file_input.close();
	}

	//Part 1 - What is the priority score for items in the wrong compartments
	std::cout << "The total priority score: " << total_priority_values << std::endl;
	//Part 2 - what is the value of the badge priority items for the elves in groups?
	std::cout << "The sum of the three elves badge priority is: " << total_badge_priority_sum << std::endl;

	
}
