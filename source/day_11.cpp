//\==============================================================================
// Advent of code 2022 Day 11
// Question: https://adventofcode.com/2022/day/11
// Gist: monkeys throw some items about 
// Input data: monkey id, items held, operation, outcome
//\==============================================================================


#include "days.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>
#include <queue>
#include <sstream>
#include <string>
#include <vector>



struct monkey
{

	std::queue<int64_t> items{};
	int64_t test_value{};
	int64_t operation_value;
	int64_t throw_to_monkey[2];
	enum operation_symbol
	{
		ADD,
		MUL,
		SQR
	};
	operation_symbol op_symbol{};

	int64_t inspection_count{};

	[[nodiscard]] int64_t operation( const int64_t old ) const
	{
		switch( op_symbol )
		{
		case ADD:
			return old + operation_value;
		case MUL:
			return old * operation_value;
		case SQR:
			return old * old;
		
		}
	}

	[[nodiscard]] bool test( const int64_t value ) const
	{
		return ((value % test_value) == 0);
	}
};

namespace keys
{
	static const std::string monkey = "monkey";
	
}
//if the worry factor is old * old this will find all positions of the word 'old' within the supplied string.
//If this value is greater than 1 then string is old * old or old^2.
bool is_old_squared( std::string& equation_rhs )
{
	//search first on old * old by finding two instances of the old string
	size_t pos{};
	int8_t old_count{};
	while ( (pos = equation_rhs.find( "old", pos )) != std::string::npos ) {
		++old_count;
		pos += 3;
	}
	return (old_count > 1);
}

//Find the position of the last operation code within a string
bool find_operation_symbol( const char op_code, std::string& equation_rhs )
{
	const size_t pos = equation_rhs.find( op_code );
	return  (pos != std::string::npos);
}
//processes input file returns number of lines read from file
int64_t process_input( const std::string& input_file, std::vector<monkey>& monkeys )
{
	int64_t line_count{};
	std::ifstream file_input;
	file_input.open( input_file );
	if ( file_input.is_open() )
	{
		for ( std::string line; std::getline( file_input, line ); )
		{
			std::transform( line.begin(), line.end(), line.begin(),
							[]( unsigned char c ){ return std::tolower( c ); } );

			std::string key = line.substr( 0, line.find_first_of( " :" ) );
			if ( key == keys::monkey )
			{
				//append a new monkey to the list
				monkeys.push_back( {} );
				//Next line in the file is the items the monkey is carrying
				if ( std::string items; std::getline( file_input, items ) )
				{
					//list of comma separated numbers populate the monkey's item vector
					std::string numbers_string = items.substr( items.find_last_of( ':' ) + 1 );
					//as string stream is designed to work with spaces as separation characters in streaming to values replace ',' with spaces
					std::replace( numbers_string.begin(), numbers_string.end(), ',', ' ' );
					std::stringstream ss( numbers_string );
					int64_t item_id{};
					while ( ss >> item_id )
					{
						monkeys.back().items.push( item_id );
					}
				}
				//Next comes the equation
				if ( std::string equation; std::getline( file_input, equation ) )
				{
					std::string equation_rhs = equation.substr( equation.find_last_of( '=' ) + 1 );
					//find the occurance of symbols in the substring
					if ( is_old_squared( equation_rhs ) )
					{
						monkeys.back().op_symbol = monkey::SQR;
					}
					else if ( find_operation_symbol( '+', equation_rhs ) )
					{
						monkeys.back().op_symbol = monkey::ADD;
						std::stringstream( equation_rhs.substr( equation_rhs.find_last_of( '+' ) + 1 ) ) >> monkeys.back().operation_value;
					}
					else if ( find_operation_symbol( '*', equation_rhs ) )
					{
						monkeys.back().op_symbol = monkey::MUL;
						std::stringstream( equation_rhs.substr( equation_rhs.find_last_of( '*' ) + 1 ) ) >> monkeys.back().operation_value;
					}
				}
				//Next line is evaluation test
				if ( std::string test; std::getline( file_input, test ) )
				{
					std::stringstream( test.substr( test.find_last_of( ' ' ) + 1 ) ) >> monkeys.back().test_value;
				}
				//next is the outcomes for the test
				for ( auto const i : {0, 1} )
				{
					if ( std::string throw_to_monkey; std::getline( file_input, throw_to_monkey ) )
					{
						std::stringstream( throw_to_monkey.substr( throw_to_monkey.find_last_of( ' ' ) + 1 ) ) >> monkeys.back().throw_to_monkey[i];
					}
				}

			}
			line_count++;
		}

		file_input.close();
	}
	return line_count;
	
}

int64_t carry_out_monkey_business_for_rounds( int64_t num_rounds, std::vector<monkey> monkeys, int64_t relief )
{
	for ( int64_t r{}; r < num_rounds; ++r )
	{
		//now perform the round for each monkey
		//for each
		//   Monkey inspects item: worry level = operation(item_value)
		//	 worry level / relief
		//	 monkey evaluates worry level if test true throw_to_monkey[0] else throw_to_monkey[1] (throw worry level for item)
		for ( auto& monkey : monkeys )
		{
			//for each item
			while ( !monkey.items.empty() )
			{
				++monkey.inspection_count;
				int64_t worry_level{};
				if ( relief == 3 ) //Part 1 - there is relief when monkey finishes with item
				{
					worry_level = monkey.operation( monkey.items.front() ) / relief;
				}
				else // part 2 - there is no relief, use mod with relief value to prevent overflow
				{
					worry_level = monkey.operation( monkey.items.front() ) % relief;
				}
				monkey.items.pop();

				monkeys[monkey.throw_to_monkey[!monkey.test( worry_level )]].items.push( worry_level );
				

			}
		}
	}
	//sort the monkeys based on inspection count
	std::sort( monkeys.begin(), monkeys.end(), []( const monkey& a, const monkey& b ){ return a.inspection_count > b.inspection_count; } );
	return monkeys[0].inspection_count * monkeys[1].inspection_count;

}

void aoc::day_11()
{
	std::vector<monkey> monkeys;
	//process input file and populate the monkey vector
	const int64_t line_count = process_input( "./data/day_11_input.txt", monkeys );
	//Part 1 - relief is used to reduce the level of worry by 1/3rd 
	const int64_t level_of_monkey_business_after_20_rounds = carry_out_monkey_business_for_rounds( 20, monkeys, 3 );
	//Part 2 - There is no relief, but item worry amounts will soar so find lowest common multiple and use mod to keep the worry amount within 64 bit boundaries.
	//work out mod value for part 2 to limit worry -- get lowest common multiple of test values
	// as values are all prime simply multiply values together.
	int64_t mod = std::accumulate( monkeys.begin(), monkeys.end(), 1ll, []( const int64_t & a, const monkey& b ){ return a * b.test_value; } );
	const int64_t level_of_monkey_business_after_10000_rounds = carry_out_monkey_business_for_rounds( 10000, monkeys, mod );


	//number of lines in file
	std::cout << "Line Count: " << line_count << std::endl;
	std::cout << "Level of monkey business after 20 rounds: " << level_of_monkey_business_after_20_rounds << std::endl;
	std::cout << "Level of monkey business after 10000 rounds: " << level_of_monkey_business_after_10000_rounds << std::endl;

}
