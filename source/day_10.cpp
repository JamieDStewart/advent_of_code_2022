//\==============================================================================
// Advent of code 2022 Day 8
// Question: https://adventofcode.com/2022/day/10
// Gist:  work out the signal strength of items from noop/addx opcodes and
// render some characters
// Input data: sequence of opcodes noop/addx
//\==============================================================================
#include "days.h"


#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace opcodes
{
	const std::string noop{ "noop" };
	const std::string addx{ "addx" };
};


int32_t get_signal_strength( std::vector<int32_t>& register_values, int32_t cycle_count )
{
	return cycle_count * register_values[cycle_count];
}

void render_image(const std::vector<int32_t>& x_register )
{
	constexpr int32_t pix_per_line = 40;
	constexpr int32_t total_pixels = pix_per_line * 6;

	int32_t offset = 0;

	for( int i = 1; i <= total_pixels;  ++i)
	{
		const int32_t sprite_pos = x_register[i];
		const int32_t crt_pos = i - offset;
		const char out_char = (crt_pos >= (sprite_pos) && crt_pos < (sprite_pos + 3)) ? '#' : ' ';
		std::cout << out_char;

		if ( i % 40 == 0 )
		{
			offset += pix_per_line;
			std::cout << std::endl;
		}

	}
}

void aoc::day_10()
{
	
	int32_t line_count{};
	int32_t x_value{ 1 };
	std::vector<int32_t> x_register;
	x_register.push_back( { 1 } );
	std::ifstream file_input;
	file_input.open( "./data/day_10_input.txt" );
	if ( file_input.is_open() )
	{
		for ( std::string line; std::getline( file_input, line ); )
		{
			size_t separator_location = line.find_first_of( ' ' );
			std::string op_code = line.substr( 0, separator_location );
			//Carry out an operation
			if( op_code == opcodes::noop )
			{
				//add 1 cycle x value remains
				x_register.push_back( x_value );
			}
			else if( op_code == opcodes::addx )
			{
				int32_t value{};
				std::stringstream( line.substr( separator_location + 1 ) ) >> value;
				x_register.push_back( x_value );
				x_register.push_back( x_value );
				x_value += value;

			}
			line_count++;
		}

		file_input.close();
	}

	//number of lines in file
	std::cout << "Line Count: " << line_count << std::endl;
	std::cout << "20th cycle, register X has the value: " <<  x_register[ 20] << " signal strength: " << get_signal_strength( x_register, 20) << std::endl;
	std::cout << "60th cycle, register X has the value: " <<  x_register[ 60] << " signal strength: " << get_signal_strength( x_register, 60 ) << std::endl;
	std::cout << "100th cycle, register X has the value: " << x_register[100] << " signal strength: " << get_signal_strength( x_register, 100 ) << std::endl;
	std::cout << "140th cycle, register X has the value: " << x_register[140] << " signal strength: " << get_signal_strength( x_register, 140 ) << std::endl;
	std::cout << "180th cycle, register X has the value: " << x_register[180] << " signal strength: " << get_signal_strength( x_register, 180 ) << std::endl;
	std::cout << "220th cycle, register X has the value: " << x_register[220] << " signal strength: " << get_signal_strength( x_register, 220 ) << std::endl;
	std::cout << "Sum of the signal strengths: " << get_signal_strength( x_register, 20 ) +
		get_signal_strength( x_register, 60 ) +
		get_signal_strength( x_register, 100 ) +
		get_signal_strength( x_register, 140 ) +
		get_signal_strength( x_register, 180 ) +
		get_signal_strength( x_register, 220 ) << std::endl;

	render_image( x_register );

}
