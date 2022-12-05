//\==============================================================================
// Advent of code 2022 Day 5
// Question: https://adventofcode.com/2022/day/5
// Gist:  Unpacking the cargo, towers of hanoi style problem
// Input data: picture of stacks, [1] [2] [3] ... [n] with crates on each
//\==============================================================================
#include "days.h"


#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>
#include <sstream>
#include <stack>
#include <string>
#include <vector>

//======================================================================
//Structure to hold instructions in the form of:
// n: number of crates to move
// x: stack to move from
// y: stack to move to.
//======================================================================
struct instructions
{
	int32_t n;
	int32_t x;
	int32_t y;
};

//Function for reading how many stacks there are from the file
//This could have been done by simply counting the number of spaces on a line it turns out. But hey I want to make use of that
//line from the input file
int32_t get_number_of_dock_stacks( std::ifstream& file_stream )
{
	for ( std::string line; std::getline( file_stream, line ); )
	{
		//does the line have numbers on it?
		//The line indicating stack count starts with "  1"
		if ( line.length() > 0 && line[1] == '1' )
		{
			//we have found the line indicating number of stacks
			int32_t stack_count{};
			//Just use stringstream conversion to get the stack count as an integer
			std::stringstream( line.substr( line.find_last_not_of( " \n" ) ) ) >> stack_count;
			//seek back to the beginning of the file
			file_stream.seekg( 0, std::ios::beg );
			return stack_count;
		}
	}
	return 0;
}

//Read the file and push the correct items on to the correct stacks
void populate_dock_stacks( std::vector< std::stack< char > >& dock_stacks, int32_t number_of_stacks, std::ifstream& file_stream )
{
	//populate stack vector
	for ( int32_t i = 0; i < number_of_stacks; ++i )
	{
		dock_stacks.emplace_back( std::stack< char >() );
	}

	for ( std::string line; std::getline( file_stream, line ); )
	{
		if ( line.length() > 0 && line[1] == '1' )
		{
			//if we get to this point we are back at the dock number stacks
			break;
		}
		int32_t stack_id{};
		for ( int32_t i = 1; i < line.length(); ++stack_id, i += 4 )
		{
			if ( line[i] != ' ' )
			{
				dock_stacks[stack_id].push( line[i] );
			}
		}
	}
	//each stack is in the reverse order so loop through each stack and reverse the order of items
	for( int32_t i = 0; i < dock_stacks.size(); ++i )
	{
		std::stack< char > temp;
		while ( !dock_stacks[i].empty() )
		{
			temp.push( dock_stacks[i].top() );
			dock_stacks[i].pop();
		}
		dock_stacks[i] = temp;
	}
}

//get all of the instructions out of the file and into our instructions structure style.
void populate_instructions( std::vector< instructions >& crane_instructions, std::ifstream& file_stream )
{
	for ( std::string line; std::getline( file_stream, line ); )
	{
		if ( line.length() )
		{
			//instructions take the form of "move n from x to y"
			int32_t n{}, x{}, y{};
			//find the first number in the string
			size_t num_start = line.find_first_of( "1234567890" );
			size_t num_end = line.find_first_of( ' ', num_start );
			std::stringstream( line.substr( num_start, num_end ) ) >> n;
			num_start = line.find_first_of( "1234567890", num_end );
			num_end = line.find_first_of( ' ', num_start );
			std::stringstream( line.substr( num_start, num_end ) ) >> x;
			num_start = line.find_first_of( "1234567890", num_end );
			std::stringstream( line.substr( num_start ) ) >> y;

			crane_instructions.push_back( { n, x, y } );
		}
	}
}


//process the instructions
void crate_mover_9000( std::vector< std::stack< char > >& dock_stacks, const std::vector< instructions >& crane_instructions )
{
	for ( const auto& instruction : crane_instructions )
	{
		int32_t n = instruction.n;
		while ( n )
		{
			dock_stacks[instruction.y - 1].push( dock_stacks[instruction.x - 1].top() );
			dock_stacks[instruction.x - 1].pop();
			--n;
		}		
	}
}

//process the instructions for part_2
void crate_mover_9001( std::vector< std::stack< char > >& dock_stacks, const std::vector< instructions >& crane_instructions )
{
	for ( const auto& instruction : crane_instructions )
	{
		int32_t n = instruction.n;
		std::stack<char> temp{};
		while ( n )
		{
			temp.push( dock_stacks[instruction.x - 1].top() );
			dock_stacks[instruction.x - 1].pop();
			--n;
		}
		while ( !temp.empty() )
		{
			dock_stacks[instruction.y - 1].push( temp.top() );
			temp.pop();
		}
	}
}


void aoc::day_05()
{
	// a vector of stacks
	std::vector< std::stack< char > > dock_stacks{};
	std::vector< std::stack< char > > dock_stacks_part_2{};
	//vector containing all the instructions to process
	std::vector< instructions > crane_instructions{};
	//open the input file for reading
	std::ifstream file_input;
	file_input.open( "./data/day_05_input.txt" );
	if ( file_input.is_open() )
	{
		//get the number of stacks required from the input file
		const int32_t number_of_stacks = get_number_of_dock_stacks( file_input );

		//populate the number of stacks with data from the input file
		populate_dock_stacks( dock_stacks, number_of_stacks, file_input );

		//gather the instructions
		populate_instructions( crane_instructions, file_input );
		//create a duplicate of the dock_stack for part 2
		dock_stacks_part_2 = dock_stacks;

		//Now we can process the instructions from the input file
		crate_mover_9000( dock_stacks, crane_instructions );

		//Part two move multiple crates in the same stack order
		crate_mover_9001( dock_stacks_part_2, crane_instructions );

		//close the input file
		file_input.close();
	}

	//Part 1 - What crates are on the top of each stack
	std::cout << "Part 1 - Items on top of stacks: "; for( auto& stack : dock_stacks ) { std::cout << stack.top(); 	} std::cout << std::endl;

	//Part 1 - What crates are on the top of each stack
	std::cout << "Part 2 - Items on top of stacks: "; for ( auto& stack : dock_stacks_part_2 ) {	std::cout << stack.top(); }	std::cout << std::endl;
}
