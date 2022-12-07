//\==============================================================================
// Advent of code 2022 Day 7
// Question: https://adventofcode.com/2022/day/7
// Gist:  find size of directories on comms device that tally to x in size
// Input data: series of lines of commands and their output
//\==============================================================================
#include "days.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>


//Simple file structure
struct file_s
{
	const std::string name;
	const int32_t size;
};

//Create a simple node structure
struct dir_s
{
	dir_s( const std::string& a_name, const dir_s* a_parent ) : name( a_name ), parent( a_parent ){};
	~dir_s()
	{
		//delete the directory tree
		if( !directories.empty() )
		{
			for( auto iter = directories.begin(); iter != directories.end(); ++iter )
			{
				delete (*iter);
				(*iter) = nullptr;
			}
		}
	}
	dir_s( dir_s& ) = delete;
	dir_s( dir_s&& ) = delete;

	dir_s* get_directory( const std::string& dir_name ) const
	{
		for( auto dir : directories )
		{
			if( dir->name == dir_name )
			{
				return dir;
			}
		}
		return nullptr;
	}

	dir_s* move_up_dir()
	{
		return const_cast<dir_s*>(parent);
	}

	void calc_directory_size()
	{
		size = std::accumulate( files.begin(), files.end(), 0, []( int i, const file_s& f ){ return f.size + i; } );
		for( auto* dir : directories )
		{
			dir->calc_directory_size();
			size += dir->size;
		}
	}

	void walk_directories_sum_directory_sizes_below( const int32_t size_limit, int32_t& value_summation ) const
	{
		if( size <= size_limit )
		{
			value_summation += size;
		}
		for ( auto* dir : directories )
		{
			dir->walk_directories_sum_directory_sizes_below( size_limit, value_summation );
		}
	}

	int32_t find_smallest_directory_size_to_free_up_size_of( int32_t size_to_free )
	{
		int32_t current_free_size = size;
		for ( auto* dir : directories )
		{
			const int32_t amount = dir->find_smallest_directory_size_to_free_up_size_of( size_to_free );
			if( amount >= size_to_free && amount <= current_free_size )
			{
				current_free_size = amount;
			}
		}
		return current_free_size;
	}

	std::vector<file_s>	files;
	std::vector<dir_s*> directories;
	const std::string name{};
	int32_t size{ 0 };
	const dir_s* parent{ nullptr };
};

void aoc::day_07()
{
	//Commands
	const std::string change_dir = "cd";
	const std::string root = "/";
	const std::string up_dir = "..";
	const std::string list = "ls";

	//set up a base directory
	dir_s* current_dir{nullptr};
	dir_s* root_dir{ nullptr };

	int32_t line_count{};
	std::ifstream file_input;
	file_input.open( "./data/day_07_input.txt" );
	if ( file_input.is_open() )
	{
		//process each line of the file
		for ( std::string line; std::getline( file_input, line ); )
		{
			//lines take the form of a command, start with $, or a output from a command
			if( line[0] == '$' )
			{
				//get command string
				const auto start_of_command = line.find_first_of( ' ' )+1;
				const auto end_of_command = line.find_first_of( ' ', start_of_command )- start_of_command;
				std::string command = line.substr( start_of_command, end_of_command );
				std::cout << "Command: " << command << std::endl;

				if( command == change_dir )
				{
					//we are changing directory
					//get directory name
					std::string dir_name = line.substr( line.find_last_of(' ')+1);
					//if current_dir is null this is the first time we have run the "cd" command
					if ( nullptr == current_dir )
					{
						
						//make a new directory and set it to our current director
						current_dir = new dir_s( dir_name, nullptr );
						root_dir = current_dir;
					}
					else
					{
						if( dir_name == root )
						{
							current_dir = root_dir;
						}
						if( dir_name == up_dir )
						{
							//move up a directory by getting the parent directory
							if ( current_dir->move_up_dir() )
							{
								current_dir = current_dir->move_up_dir();								
							}							
						}
						else
						{
							dir_s* n = current_dir->get_directory( dir_name );
							if ( nullptr != n ) //we can change to this directory
							{
								current_dir = n;
							}
						}
					}
				}
				else if ( command == list )
				{
					//we have run the list command this can be ignored as there's nothing for us to process
				}
			}
			else
			{
				//this is not a command this is output we need to record
				//output takes the form of:
				// "dir _name_" or "123456 file.name"
				std::string first_token = line.substr( 0, line.find_first_of( ' ' ) );
				std::cout << first_token << std::endl;
				//test for directory first, if it's not a directory it's a file
				if( first_token == "dir" )
				{
					std::string dir_name = line.substr( line.find_last_of( ' ' ) + 1 );
					current_dir->directories.push_back( new dir_s( dir_name, current_dir ) );
				}
				else
				{
					int32_t file_size{};
					std::stringstream( first_token ) >> file_size;
					std::string file_name = line.substr( line.find_last_of( ' ' ) + 1 );
					current_dir->files.push_back( { file_name, file_size } );
					
				}
			}
			line_count++;
		}
		file_input.close();
	}
	std::cout << "Number of lines in input file: " << line_count << std::endl;

	current_dir = root_dir;
	//do a directory walk and populate all directories with a size
	if( nullptr != root_dir )
	{
		root_dir->calc_directory_size();
		int32_t total_summation_below_100000{};
		root_dir->walk_directories_sum_directory_sizes_below( 100000, total_summation_below_100000 );
		std::cout << "Summation of directories below 100000: " << total_summation_below_100000 << std::endl;

		std::cout << "Total size of data on system: " << root_dir->size << std::endl;
		int32_t free_space = 70000000 - root_dir->size;
		int32_t size_of_data_to_free = 30000000 - free_space;
		std::cout << "Size of data to free: " << size_of_data_to_free << std::endl;

		int32_t free_size = root_dir->find_smallest_directory_size_to_free_up_size_of( size_of_data_to_free );
		std::cout << "Size of directory to free up space: " << free_size << std::endl;
	}
	delete root_dir;
}
