//\==============================================================================
// Advent of code 2022 Day 2
// Question: https://adventofcode.com/2022/day/2
// Gist:  Play Rock, Paper, Scissors with elves, you have a cheat sheet to winning
// Input data: A = Rock, B = Paper, C = Scissors
//             X, Y, Z dependent on part of the question 
//\==============================================================================

#include <fstream>
#include <iostream>
#include <map>
#include <string>


int main( int argc, char* argv[] )
{
	std::map< std::string, int32_t > part_1_hand_values;
	//They Choose Rock
	part_1_hand_values["A X"] = 1 + 3;	//You choose rock,    3 points for draw
	part_1_hand_values["A Y"] = 2 + 6;	//You choose paper,   6 points for win
	part_1_hand_values["A Z"] = 3 + 0;	//You choose scissors,0 points for loss
	//They Choose Paper
	part_1_hand_values["B X"] = 1 + 0;	//You choose rock,    0 points for loss
	part_1_hand_values["B Y"] = 2 + 3;	//You choose paper,   3 points for draw
	part_1_hand_values["B Z"] = 3 + 6;	//You choose scissors,6 points for win
	//They Choose Scissors
	part_1_hand_values["C X"] = 1 + 6;	//You choose rock,    6 points for win
	part_1_hand_values["C Y"] = 2 + 0;	//You choose paper,   0 points for loss
	part_1_hand_values["C Z"] = 3 + 3;	//You choose scissors,3 points for draw

	std::map< std::string, int32_t > part_2_hand_values;
	//They Choose Rock
	part_2_hand_values["A X"] = 3 + 0;	//You Need to lose 3 points for choosing scissors
	part_2_hand_values["A Y"] = 1 + 3;	//You need to draw 1 point  for choosing rock
	part_2_hand_values["A Z"] = 2 + 6;	//You need to win  2 points for choosing paper
	//They Choose Paper
	part_2_hand_values["B X"] = 1 + 0;	//You Need to lose 1 point for choosing rock
	part_2_hand_values["B Y"] = 2 + 3;	//You need to draw 2 points for choosing paper
	part_2_hand_values["B Z"] = 3 + 6;	//You need to win  3 points for choosing scissors
	//They Choose Scissors
	part_2_hand_values["C X"] = 2 + 0;	//You Need to lose 2 points for choosing paper
	part_2_hand_values["C Y"] = 3 + 3;	//You need to draw 3 points for choosing scissors
	part_2_hand_values["C Z"] = 1 + 6;	//You need to win  1 point for choosing rock

	uint32_t number_of_hands_played = 0;
	// uint32 to keep track of the running total for scores
	uint32_t part_1_scores = 0;
	uint32_t part_2_scores = 0;

	//open the input file for reading
	std::ifstream file_input;
	file_input.open( "./data/input.txt" );
	if ( file_input.is_open() )
	{
		//process each line of the file
		for ( std::string line; std::getline( file_input, line ); )
		{
			if ( line.length() > 0 )
			{
				number_of_hands_played++;
				//use the line string as a key into a map of possible scores
				part_1_scores += part_1_hand_values[line];
				part_2_scores += part_2_hand_values[line];
			}
		}
		//close the input file
		file_input.close();
	}

	std::cout << "Rounds of Rock, Paper, Scissors played: " << number_of_hands_played << std::endl;
	//Part 1 - Using map of scores for X/Y/Z for part 1 to sum the scores
	std::cout << "The sum of the scores for part_1 is: " << part_1_scores << std::endl;
	//Part 2 - Using map of scores for X/Y/Z for part 2 to sum the scores
	std::cout << "The sum of the scores for part_2 is: " << part_2_scores << std::endl;

	return 0;
}
