//\==============================================================================
// Advent of code 2022 Day 11
// Question: https://adventofcode.com/2022/day/12
// Gist: The inevitable A* problem from point S reach Point E 
// Input data: a map of letter values from a-z indicating height with start point S
// and end point E. Can only move if adjacent height equal to or lower than 1
// a -> b is ok, a-> c is not allowed. c -> a is ok too.
//\==============================================================================


#include "days.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>
#include <string>
#include <sstream>
#include <vector>

class packet_node
{
public:
	virtual ~packet_node() = default;
	packet_node* get_next_child(const packet_node* sibling)
	{
		if( sibling == nullptr && !children.empty() )
		{
			return children.front();
		}
		const auto pos = std::find( children.begin(), children.end(), sibling );
		if( pos != children.end() && (pos+1) != children.end() )
		{
			return *(pos + 1);
		}
		return nullptr;
	}

	virtual void print_children()
	{
		std::cout << "[ ";
		for( const auto child : children )
		{
			child->print_children();
			if ( child != children.back() )
			{
				std::cout << ",";
			}
		}
		std::cout << " ]";;
	}
	packet_node* parent{ nullptr };
	std::vector<packet_node*> children{};
};

class packet_data_node final : public packet_node
{
public:
	packet_data_node( packet_node* p, const int32_t v )
	{
		parent = p;
		value = v;
	}

	void print_children() override
	{
		std::cout << value;
	}
	int32_t value{};
};

class packet
{
public:
	packet() : root( new packet_node() ) {};

	
	packet_node* root;	
};


packet* process_packet( const std::string& packet_string )
{
	packet* p = new packet();
	packet_node* current_node = p->root;
	std::string current_value{};

	//lambda for adding data nodes
	auto add_data_node = []( packet_node* p, std::string& data )
	{
		//process the current_value string into an integer type
		int32_t value;
		std::stringstream( data ) >> value;
		data.clear();
		auto* pdn = new packet_data_node( p, value );
		p->children.push_back( pdn );
	};

	//start from the 2nd character in the string
	for( auto iter = packet_string.begin()+1; iter != packet_string.end(); ++iter )
	{
		const char c = (*iter);
		
		if( c == '[' )
		{
			//if we [ we have the start of a new child node, create node set the parent and make new one current
			current_node->children.push_back( new packet_node() );
			current_node->children.back()->parent = current_node;
			current_node = current_node->children.back();
		}
		//signifies end of list, return to parent node
		else if( c == ']' )
		{
			//current_value may still contain data capture this data and append new packet data node before moving to parent
			if ( current_value.length() > 0 )
			{
				add_data_node( current_node, current_value );
			}
			current_node = current_node->parent;
		}
		//signifies data value separator
		else if( c == ',' )
		{
			if ( current_value.length() > 0 )
			{
				add_data_node( current_node, current_value );
			}
		}
		else
		{
			//this must be a value type append into current_value
			current_value.push_back( c );
		}
	}
	return p;
}

int32_t compare_data_values( int32_t l, int32_t r)
{
	//std::cout << "Compare " << l << " vs " <<r << std::endl;
	if ( l == r )
	{
		//std::cout << "Inputs are Even" << std::endl;
		return 0;
	}
	if ( l < r )
	{
		//std::cout << "Left side is smaller, so inputs are in the right order" << std::endl;
		return 1;
	}
	//std::cout << "Right side is smaller, so inputs are NOT in the right order" << std::endl;
	return -1;
}

packet_node* box_value_node( int32_t value )
{
	auto* parent = new packet_node();
	auto* boxed_value = new packet_data_node( parent, value );
	parent->children.push_back( boxed_value );
	return parent;
}

void cleanup_boxed_value_node(packet_node* boxed_value)
{
	delete boxed_value->children.back();
	delete boxed_value;
}

int32_t compare_packets(  packet_node* ld, packet_node* rd)
{
	//iterate over the node's hierarchy until one of them is empty
	while ( ld != nullptr && rd != nullptr )
	{
		//std::cout << "Comparing: " << std::endl;
		//ld->print_children(); std::cout << std::endl;
		//rd->print_children(); std::cout << std::endl;

		const auto* ldn = dynamic_cast<packet_data_node*>(ld);
		const auto* rdn = dynamic_cast<packet_data_node*>(rd);
		int32_t val{};
		if( ldn && rdn ) //both are value types
		{
			val = compare_data_values( ldn->value, rdn->value );
		}
		else if( ldn && !rdn ) //this is value vs. list
		{
			//box the value into a list
			auto* boxed_value = box_value_node( ldn->value );
			val = compare_packets( boxed_value->get_next_child(nullptr), rd->get_next_child(nullptr) );
			cleanup_boxed_value_node( boxed_value );
			
		}
		else if( !ldn && rdn ) //this is list vs. value
		{
			//box the value into a list
			auto* boxed_value = box_value_node( rdn->value );
			val = compare_packets( ld->get_next_child(nullptr), boxed_value->get_next_child( nullptr ) );
			cleanup_boxed_value_node( boxed_value );

		}
		else
		{
			//list vs list
			val = compare_packets( ld->get_next_child( nullptr ), rd->get_next_child( nullptr ) );
		}
		//If val isn't 0 we have a solution
		if ( val != 0 ) { return val; }
		//otherwise advance the nodes to their next sibling.
		ld = ld->parent->get_next_child( ld );
		rd = rd->parent->get_next_child( rd );
		

	}
	if ( !ld && !rd ) //both list ran out of items at the same time so they are equal
	{
		return 0;
	}

	if( !ld ) //left side ran out first
	{
		//std::cout << "Left side ran out of items, so inputs are in the right order" << std::endl;
		return 1;
	}
	if( !rd ) // right side ran out first
	{
		//std::cout << "Right side ran out of items, so inputs are not in the right order" << std::endl;
		return -1;
	}
	
	return 1;
}

void aoc::day_13()
{
	std::ifstream file_input;
	int32_t packet_index{};
	std::vector<packet_node*> sorted_array{};

	file_input.open( "./data/day_13_input.txt" );
	if ( file_input.is_open() )
	{
		int32_t correct_index_sum{};
		while ( !file_input.eof() )
		{
			++packet_index;
			std::string left_packet{};
			std::getline( file_input, left_packet );
			std::string right_packet{};
			std::getline( file_input, right_packet );
			file_input.ignore(); //move a character to the next line
			//std::cout << "== Pair " << packet_index << " ==" << std::endl;
			//std::cout << "Left Input:  " << left_packet << std::endl;
			const packet* left = process_packet( left_packet );
			sorted_array.push_back( left->root );

			//std::cout << "Right Input: " << right_packet << std::endl;
			const packet* right = process_packet( right_packet );
			sorted_array.push_back( right->root );

			//part 1 - find the sum of the packets that are in order
			if( const int32_t val = compare_packets( left->root, right->root); val == 1 )
			{
				correct_index_sum += packet_index;
				//std::cout << "Correct Order" << std::endl;
			}
			//std::cout << std::endl;
			
		}
		file_input.close();

		//number of lines in file
		std::cout << "Sum of Correct indices: " << correct_index_sum << std::endl;
		//part 2 - sort the list of packets
		//create divider packets and insert them into the array
		packet_node* first_divider = new packet_node();
		first_divider->children.push_back( new packet_node() );
		first_divider->children.back()->parent = first_divider;
		first_divider->children.back()->children.push_back( new packet_data_node( first_divider->children.back(), 2 ) );
		first_divider->print_children(); std::cout << std::endl;
		sorted_array.push_back( first_divider );

		packet_node* second_divider = new packet_node();
		second_divider->children.push_back( new packet_node() );
		second_divider->children.back()->parent = second_divider;
		second_divider->children.back()->children.push_back( new packet_data_node( second_divider->children.back(), 6 ) );
		second_divider->print_children(); std::cout << std::endl;
		sorted_array.push_back( second_divider );

		std::sort( sorted_array.begin(), sorted_array.end(), [](  packet_node* a,  packet_node* b ){ return (compare_packets( a, b ) == 1) ? true : false; } );

		int32_t index{ 1 };
		int32_t value{1};
		for( auto p : sorted_array )
		{
			if( p == first_divider || p == second_divider )
			{
				std::cout << index << ": ";
				p->print_children(); std::cout << std::endl;
				value *= index;
			}
			++index;
		}

		std::cout << "Decoder Key Value: " << value << std::endl;
	}

	
	


}

