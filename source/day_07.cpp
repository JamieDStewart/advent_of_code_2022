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
	dir_s( const std::string& a_name, const dir_s* a_parent ) : name( a_name ), parent( a_parent ) {};

	~dir_s()
	{
		//delete the directory tree
		if ( !directories.empty() )
		{
			for ( auto iter = directories.begin(); iter != directories.end(); ++iter )
			{
				delete ( *iter );
				( *iter ) = nullptr;
			}
		}
	}

	//Don't need copying or move constructors
	dir_s( dir_s& ) = delete;
	dir_s( dir_s&& ) = delete;
	bool operator== ( dir_s& ) = delete;
	bool operator== ( dir_s&& ) = delete;

	//Gets an immediate subdirectory with the supplied name or returns null
	dir_s* get_sub_directory_with_name( const std::string& dir_name ) const
	{
		for ( auto dir : directories )
		{
			if ( dir->name == dir_name )
			{
				return dir;
			}
		}
		return nullptr;
	}

	//gets parent directory
	dir_s* get_parent_dir() const
	{
		return const_cast< dir_s* >( parent );
	}

	//calculates the directory size taking into account any sub_directories
	void calc_directory_size()
	{
		size = std::accumulate( files.begin(), files.end(), 0, []( int i, const file_s& f ) { return f.size + i; } );
		//recursively calculate sub_directory size and add this into the current directory size
		for ( auto* dir : directories )
		{
			dir->calc_directory_size();
			size += dir->size;
		}
	}

	//get current directory and all sub-directories below a given size
	std::vector< const dir_s* > get_directories_below_size( const int32_t size_limit ) const 
	{
		std::vector< const dir_s* > directories_below_size_limit{};
		if ( size <= size_limit )
		{
			directories_below_size_limit.push_back( this );
		}
		for ( auto* dir : directories )
		{
			std::vector< const dir_s* > sub_directories_below_size = dir->get_directories_below_size( size_limit );
			if ( !sub_directories_below_size.empty() )
			{
				directories_below_size_limit.insert( directories_below_size_limit.end(), sub_directories_below_size.begin(), sub_directories_below_size.end() );
			}
		}
		return directories_below_size_limit;
	}

	//get current directory and all sub-directories below a given size
	std::vector< const dir_s* > get_directories_above_size( const int32_t size_limit ) const 
	{
		std::vector< const dir_s* > directories_above_size_limit{};
		if ( size >= size_limit )
		{
			directories_above_size_limit.push_back( this );
		}
		for ( auto* dir : directories )
		{
			std::vector< const dir_s* > sub_directories_below_size = dir->get_directories_above_size( size_limit );
			if ( !sub_directories_below_size.empty() )
			{
				directories_above_size_limit.insert( directories_above_size_limit.end(), sub_directories_below_size.begin(), sub_directories_below_size.end() );
			}
		}
		return directories_above_size_limit;
	}

	std::vector< file_s > files;
	std::vector< dir_s* > directories;
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
	dir_s* current_dir{ nullptr };
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
			if ( line[0] == '$' )
			{
				//get command string
				const auto start_of_command = line.find_first_of( ' ' ) + 1;
				const auto end_of_command = line.find_first_of( ' ', start_of_command ) - start_of_command;
				std::string command = line.substr( start_of_command, end_of_command );

				if ( command == change_dir )
				{
					//we are changing directory
					//get directory name
					std::string dir_name = line.substr( line.find_last_of( ' ' ) + 1 );
					//if current_dir is null this is the first time we have run the "cd" command
					if ( nullptr == current_dir )
					{
						//make a new directory and set it to our current director
						current_dir = new dir_s( dir_name, nullptr );
						root_dir = current_dir;
					}
					else
					{
						if ( dir_name == root )
						{
							current_dir = root_dir;
						}
						if ( dir_name == up_dir )
						{
							//move up a directory by getting the parent directory
							if ( current_dir->get_parent_dir() )
							{
								current_dir = current_dir->get_parent_dir();
							}
						}
						else
						{
							dir_s* n = current_dir->get_sub_directory_with_name( dir_name );
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
				//test for directory first, if it's not a directory it's a file
				if ( first_token == "dir" )
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
	if ( nullptr != root_dir )
	{
		root_dir->calc_directory_size();
		int32_t total_summation_below_100000{};
		std::vector< const dir_s* > directories_below_size = root_dir->get_directories_below_size( 100000 );
		int32_t accumulated_size = std::accumulate( directories_below_size.begin(), directories_below_size.end(), 0, []( int i, const dir_s* d ) { return d->size + i; } );

		std::cout << "Summation of directories below 100000: " << accumulated_size << std::endl;

		std::cout << "Total size of data on system: " << root_dir->size << std::endl;
		int32_t free_space = 70000000 - root_dir->size;
		int32_t size_of_data_to_free = 30000000 - free_space;
		std::cout << "Size of data to free: " << size_of_data_to_free << std::endl;

		std::vector< const dir_s* > directories_above_size = root_dir->get_directories_above_size( size_of_data_to_free );
		std::sort( directories_above_size.begin(), directories_above_size.end(), []( const dir_s* i, const dir_s* d ) { return i->size < d->size; } );
		std::cout << "Size of directory to free up space: " << directories_above_size[0]->size << std::endl;
		
	}
	delete root_dir;
}
