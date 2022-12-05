//\==============================================================================
// Advent of code 2022 Day 1
// Question: https://adventofcode.com/2022/day/1
// Gist:  Elves on the expedition carry x number of calories, which elf(ves) have the most
// Input data: calorie values, elves separated by blank line
//\==============================================================================


#include "days.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>


void aoc::day_01()
{
	//vector to store the total quantity of calories that an elf is carrying
	std::vector< uint32_t > calories;

	//open the input file for reading
	std::ifstream file_input;
	file_input.open( "./data/day_01_input.txt" );
	if ( file_input.is_open() )
	{
		//keep track of the total number of calories for the current elf
		uint32_t total_calories = 0;

		//process each line of the file
		for ( std::string line; std::getline( file_input, line ); )
		{

			if ( line.length() > 0 )
			{
				//get the value from the string stream as an integer
				std::stringstream ss( line );
				uint32_t val = 0;
				ss >> val;
				//add this value to the total calories
				total_calories += val;
			}
			else
			{
				//if the line is empty then we're onto the next elf
				//so stick the calorie total into the vector and reset the counter
				calories.push_back( total_calories );
				total_calories = 0;
			}
		}
		//close the input file
		file_input.close();
	}

	//if we collected any input then let's sort the array to find the elf with the most calories
	if ( !calories.empty() )
	{
		std::cout << "Elves accompanying you on journey: " << calories.size() << std::endl;
		std::sort( calories.begin(), calories.end(), std::greater<>() );
		//Part 1 - How many calories does the elf with the highest amount of calories have
		std::cout << "The Elf with the most calories has: " << calories[0] << std::endl;
		//Part 2 - what is the calorie total for the three elves with the most calories.
		//accumulate the first three elements of the vector
		std::cout << "The sum of the three elves with the highest calories is: " <<
			std::accumulate( calories.begin(), calories.begin() + 3, 0u ) << std::endl;

	}
}
