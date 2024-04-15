#include <iostream>
#include <iomanip>
#include <fstream>
#include <cctype>
#include <string>
#include <ctime>
#include <cstring>
#include <cstdlib>

using namespace std;

/*======================= function prototypes ========================*/
int Read_questions(ifstream&, string[][5]);
int Read_answers(ifstream&, string[]);
int Play_game(int, string[][5], string[], string);
bool Linear_search(int[], int, int);
void Show_questions(int, string[][5], int, string, int);
string Player_try();
void Sort_score(fstream&, string, int);

/*=====================================================================*/


/*=====================================================================
Function: Main()
Description: Controls main functions of program and error checks files
	to be opened and modified. Has a do-while loop so user can continue
	to play game without quiting program.
Parameters: Command line arguments. There is three in total. First is the 
	program executable, then the question file and last the answer file. 
======================================================================*/ 
int main(int argc, char* argv[])
{

	char ch;					//This is hold Y or N values
	char choice[10];			//Helps determine if single value
	const int COL = 5;			//2nd dimension in array questions 
	const int SIZE = 50;		//1st dimension in array questions
	fstream scoreFile;			//object to store scores and names
	int is_single;				//determines single value
	int player_score = 0;		//variable that holds player score
	ifstream answerFile;		//input filestream object for answers
	ifstream questionFile;		//input filestream object for questions
	string answers[SIZE];		//initializes array for answers
	string player_name;			//variable to hold player name
	string questions[SIZE][COL];//initializes array for questions
	
	//If argc does not contain 3 elements, then it does not have 
	//all the required files. 
	if ( argc != 3 )
	{
		cout << "Please include two file names on the command line.";
		return 0;
	}

	//opening questions.txt and answers.txt through pointers
	questionFile.open( argv[1], ios::in );
	answerFile.open( argv[2], ios::in );

	//If either of the files have errors opening then program quits.
	if ( !questionFile || !answerFile )
	{
		cout << "ERROR: Cannot open one or both of the files.\n";
		return 0;
	}

	//Calls the Read_questions function and returns the number of 
	//questions that were read into the array. 
	const int num_questions = Read_questions( questionFile, questions );
	
	//The number of questions are compared to the number of answers.
	//If they do not match, program terminates. 
	if ( num_questions != Read_answers( answerFile, answers ))
	{
		cout << "The number of questions and answers do not equal. \
			Terminating program.";
		return 0;
	}

	questionFile.close();
	answerFile.close();


	do
	{

		cout << "Please enter your name: ";
		getline( cin, player_name );

		if ( player_name.empty() ) //Assigns No name incase no name is entered.
			player_name = "No Name"; 
		
		//once a full game is played, the function returns the score to player_score.
		player_score = Play_game( num_questions, questions, answers, player_name );
		cout << "\n\nYou earned a score of " << player_score << "!\n\n";

		scoreFile.open( "summary.txt", ios::out | ios::app ); //opens in append mode

		if ( !scoreFile )
		{
			cout << "ERROR: Cannot open summary.txt. Program exiting.";
			return 0;
		}

		//writes player name and player score into the summary file. 
		scoreFile << player_name << endl;
		scoreFile << player_score << endl;
		scoreFile.close();
		scoreFile.open( "summary.txt", ios::in );//opens in input mode

		if ( !scoreFile )
		{
			cout << "ERROR: Cannot open summary.txt. Program exiting.";
			return 0;
		}

		//Calls sort_score function to sort and display highscores.
		Sort_score( scoreFile, player_name, player_score );
		scoreFile.close();

		
		//Simple input validation to keep player from entering more than one
		//character and entering eroneous responces. 
		cout << "\n\nWould you like to play again? Y for yes or N for no.> ";
		cin >> choice;
		is_single = strlen( choice );
		while ( is_single > 1 )
		{
			cout << "You entered more than one character! Try again.> ";
			cin >> choice;
			is_single = strlen( choice );
		}

		ch = choice[0];
		ch = toupper( ch );

		while ( ch != 'Y' && ch != 'N' )
		{
			cout << "Invalid responce. Enter Y for yes or N for no.> ";
			cin >> choice;
			is_single = strlen( choice );
			while ( is_single > 1 )
			{
				cout << "You entered more than one character! Try again.> ";
				cin >> choice;
				is_single = strlen( choice );
			}
			ch = choice[0];
			ch = toupper( ch );
		}

		cin.ignore(); //To absorb the space cause by the enter key when 
					  //entering a choice and going straight to 
					  //entering a name if player chooses to play again.

	} while ( ch == 'Y' );






	return 0;
}

/*=====================================================================
Function: Read_questions()	
Description: Reads in the question file and sorts out the questions into
	one dimension of the question array, and sorts the responces into the 
	second dimension of the array. Example: questions[0][0] will hold the 
	first question string. questions[0][1,2,3,4] will hold the responces 
	assigned to that question.
Parameters: ifstream &file - the question file to read from.
	string questions[0][5] - the array the questions will be read into.
	Returns i which tells program how many lines it read into array.
======================================================================*/ 
int Read_questions( ifstream& file, string questions[][5] )
{

	int i = 0;   //First dimension of index.
	int j = 0;   //Second dimension of index.
	string line; //String to read each line of file in before assigning 
				 //it to an array.
	
	while ( !file.eof() ) //Loops until end of file.
	{
		getline( file, line );

		if ( !line.empty() ) //If line is empty, will not read it into 
		{					 //the array.
			questions[i][j] = line; //J is always zero, i is incrementing
									//by one to assign only the questions.
			for ( int n = 1; n < 5; n++ ) //assigns the responces to the 
			{							  //same index as it's 
				getline( file, questions[i][n] );//questions.
			}
			i++;
		}
	}

	if ( i == 0 )						  //i will never increment if 
	{                                     //there is no content in file.
		cout << "The question file is empty! Hit any key to quit.";
		getline( cin, line );			  //Pause for user to respond.
		exit( 0 );
	}

	return i;
}

/*=====================================================================
Function: Read_answers()
Description: Reads in from answer file the answers to an array named
	answers[]. If the file is empty then program will exit. 
Parameters:ifstream &file - the answers.txt file that the function reads
	from. string answers[]- the array the strings will be read into.
	Returns i which tells program how many lines it read into array. 
======================================================================*/ 
int Read_answers( ifstream& file, string answers[50] )
{

	int i = 0;						//First dimension of index
	string line;					//string to read into first before
									//array. 
	
	while ( !file.eof() )			//executes until end of file
	{
		line = " ";
		getline( file, line );

		if ( line.length() != 0 )   //If line is not blank then will 
		{							//read line into array. 
			answers[i] = line;
			i++;
		}

	}

	if (i == 0)						//If no content in file, program 
	{								//will exit.
		cout << "The answer file is empty! Hit any key to quit.";
		getline( cin, line );		//Pause for user to respond.
		exit( 0 );
	}

	return i;
}





/*=====================================================================
Function:Play_game()
Description:Handles the majority of gameplay. Presents the game title,
assigns random questions, validates yes or no inputs from user, calls
Linear_search(), Show_questions(), and Player_try() functions. Also 
assigns points based on point scheme. 
Parameters: int num_questions - number of questions that was read into
array. string questions[][]- question array holding questions. 
string answers[]- answer array holding answers, string player_name-
holds the player name for display and scoring purposes. The return
of this function is the player_points that were awarded. 
======================================================================*/ 
int Play_game( int num_questions, string questions[][5], string answers[50],
	string player_name )
{
	bool found = false;						//flag for linear search                   
	char ch;								//Holds Y or N
	char choice[10];						//character validation purposes
	const int INDEX = num_questions - 1;	//minus one to assign to index
	int is_single = 0;						//for string length
	int last_points = 0;					//last points user earned
	int player_points = 0;					//Running total of points
	int* question_array;                    //pointer to array
	int question_case = 0;					//for switch in show_questions()
	int question_index = 0;					//Questions array index
	question_array = new int [INDEX]; 		//Dynamic allocated array
	srand( time (NULL) );					//Seeding rand with time
	string player_answer; 					//first answer choice
	string player_answer2;					//second answer choice 
	
	
	cout << "\n**********************************************************\n";
	cout << "\n                 Welcome to Study Quizlet!\n";
	cout << "************************************************************\n";
	cout << "\nYou will be presented with a series of questions that will"
		<< " gain you points.\n";
	cout << "Do your best and your highscore will be totaled at the end"
		<< " of the game. Let's go!\n\n";

	//This for loop executes 6 times for 6 questions. It assigns a random 
	//number to the question_index that is between 1 and the number of 
	//questions that were read from the file. Num_questions
	//holds the value of the number of questions. It then puts that random
	//number into the dynamic allocated array we initialized based on the 
	//loop count.  
	for ( int i = 0; i < 6; i++ )
	{

		question_index = ( rand() % (( num_questions - 1 ) - 0 + 1 ));
		found = Linear_search( question_array, num_questions, question_index );
		question_array[i] = question_index;
	//Now if the found variable was triggered to true, the while loop executes. 
	//This means that the random number we just put into the array already exsisted
	//in the array. This would cause duplicate questions to display. Back into the 
	//while loop, it will execute until it finds a number it has not put into the 
	//question_array.
		while ( found )
		{
			question_index = ( rand() % (( num_questions - 1 ) - 0 + 1 ));
			found = Linear_search( question_array, num_questions, question_index );
		}
	//Once out of the while loop, we have a new number not picked yet so it gets 
	//assigned to the question_array. We set question_case to one to display the 
	//first instance of a question without any responces blocked out. This is sent
	//to the Show_questions function along with the question index to display the 
	//randomly selected question. i is sent to help display which question number
	//is displaying. 
		question_array[i] = question_index;
		question_case = 1;
		Show_questions( question_index, questions, question_case, player_name, i );
	//After questions are displayed, the Player_try function is called and that 
	//returns the players validated choice and assigns it to player_answer.
		player_answer = Player_try();



	//This if statement determines if the player answer matches the answer we got
	//from the text file. If not, offers choice to skip or try again. 
		if ( player_answer != answers[question_index])
		{
			cout << "\nSorry! That was an incorrect responce. Would you like"
				<< " to attempt it again?\n";
			cout << "\nY for yes or N for no.> ";
			cin >> choice;
			is_single = strlen( choice ); //takes the length of the answer
			while ( is_single > 1 )       //if it is greater than one character
			{							 //it stays in while loop.
				cout << "You entered more than one character! Try again.> ";
				cin >> choice;
				is_single = strlen( choice );
			}

			ch = choice[0];
			ch = toupper( ch ); //Assigns character to ch and uppercases it.
			
			//Must have single character to enter into while loop. Stays in
			//while loop until character is Y or N. 
			while ( ch != 'Y' && ch != 'N' ) //if not Y and not N then invalid.
			{
				cout << "Invalid responce. Enter Y for yes or N for no.> ";
				cin >> choice;
				is_single = strlen( choice ); //makes sure only one character
				while ( is_single > 1 )		  //is assigned again.
				{
					cout << "You entered more than one character! Try again.> ";
					cin >> choice;
					is_single = strlen( choice );
				}
				ch = choice[0];
				ch = toupper( ch ); 
			}
			
			//If player wants to try again, based on their previous answer, 
			//question_case determines which responce is blocked out from 
			//displaying when it goes to the Show_questions function.
			
			if ( ch == 'Y' )
			{
				if ( player_answer == "A" )
					question_case = 2;

				else if ( player_answer == "B" )
					question_case = 3;

				else if ( player_answer == "C" )
					question_case = 4;

				else if ( player_answer == "D" )
					question_case = 5;
				
				//Displaying second try question.
				Show_questions( question_index, questions, question_case,
					player_name, i );
			
				//New variable to hold second answer.
				player_answer2 = Player_try();
			
				//If second answer matches previous answer then try again.
				while ( player_answer2 == player_answer )
				{
					cout << "\nYou already selected that answer! Try again.";
					player_answer2 = Player_try();
				}

				//If second answer matches correct answer then player is 
				//awarded points. The if statement is the senario if 
				//the player has previous points to mulitply from.
				//it takes the last points earned and multiplies it by 10
				//but since this is second attempt it divides it by 2.
				if ( player_answer2 == answers[question_index] )
				{
					cout << "\nCorrect!\n";
					if ( player_points != 0 )
					{
						player_points += ( last_points * 10 ) / 2;
						last_points = ( last_points * 10 ) / 2;
					}
				//This else statement is for the senario if the player
				//has no previous points earned to mulitply off of. 
				//First points awarded is 10 but since this is second
				//attempt then only half is awarded.
					else
					{
						last_points = 5;
						player_points = 5;
					}
				}
				//In the senario if player got second attempt wrong,
				//then game over, all points lost. 
				else
				{
					cout << "\nIncorrect! All points lost!\n" <<
						"\nGAME OVER";
					player_points = 0;
					break;
				}

			}
				//If the player elects not to try a second attempt,
				//then zero points are awarded and next question is 
				//displayed.
			else
				cout << "\nZero points awarded!\n\n";

		}
		
		//If player get questions right on first attempt then program
		//goes into this else statement.
		else
		{
			cout << "\nCorrect!\n";
			if ( player_points != 0 ) //Senario if player has points 
			{
				player_points += last_points * 10;
				last_points = last_points * 10;
			}
			else                     //Senario if player has no points.
			{
				player_points = 10;
				last_points = 10;
			}
		}
	}
	delete[] question_array;        //Delete dynamic array
	question_array = 0;				//set pointer to zero
	return player_points;			//returns points to main()
}



/*=====================================================================
Function: Linear_search()
Description: Searches the question_array for any repeating random numbers.
If it finds none it returns false.
Parameters: int question_array[]-the array it searches. int size - 
the size of the array it's searching. int value - the number it is 
searching for in the array. If found it returns true. 
======================================================================*/ 
bool Linear_search( int question_array[], int size, int value )
{
	
	bool found = false;            
	int i = 0;
	
	//only loops the amount of times equal to the array size. If not 
	//found by then, function returns bool value of false. 
	while ( i < size && !found )
	{
	//iterates through indexes of the array searching for the value
		if ( question_array[i] == value )
		{
			found = true;

		}
		i++;
	}
	return found;
}





/*=====================================================================
Function: Show_questions
Description: This function handles the display of the questions and 
responces. from case 1, this is base zero of the questions. all 
responces are displayed. The rest of the cases have a responce missing.
Parameters: int question_index - this is the random number that determines
which question and responces get displayed. This is why it is assigned
to the first dimension of the array. String questions[][]- the array 
that hold the questions and responces. int choice - this holds the case
number that was determined in the play_game function. This decides which 
case is displayed. string name - holds player name to display in the 
question. int num - passes the number of which question is being displayed.
i.e. question 1, or question 3. 
======================================================================*/ 
void Show_questions( int question_index, string questions[][5], int choice,
	string name, int num )
{
	switch ( choice )
	{
	case 1:// Original question block with no blocked out answers
		cout << "\n" << name << "  Here's Question Number " << num + 1;
		cout << "\n" << questions[question_index][0] << "\n";
		cout << "A. " << questions[question_index][1] << "\n";
		cout << "B. " << questions[question_index][2] << "\n";
		cout << "C. " << questions[question_index][3] << "\n";
		cout << "D. " << questions[question_index][4] << "\n";
		break;

	case 2:// Answer block with choice A removed
		cout << "\n" << name << "  Here's Question Number " <<
			num + 1 << " (second try)";
		cout << "\n" << questions[question_index][0] << "\n";
		cout << "\n";
		cout << "B. " << questions[question_index][2] << "\n";
		cout << "C. " << questions[question_index][3] << "\n";
		cout << "D. " << questions[question_index][4] << "\n";
		break;

	case 3:// Answer block with choice B removed
		cout << "\n" << name << "  Here's Question Number " <<
			num + 1 << " (second try)";
		cout << "\n" << questions[question_index][0] << "\n";
		cout << "A. " << questions[question_index][1] << "\n";
		cout << "\n";
		cout << "C. " << questions[question_index][3] << "\n";
		cout << "D. " << questions[question_index][4] << "\n";;
		break;

	case 4:// Answer block with choice C removed
		cout << "\n" << name << "  Here's Question Number " <<
			num + 1 << " (second try)";
		cout << "\n" << questions[question_index][0] << "\n";
		cout << "A. " << questions[question_index][1] << "\n";
		cout << "B. " << questions[question_index][2] << "\n";
		cout << "\n";
		cout << "D. " << questions[question_index][4] << "\n";
		break;

	case 5:// Answer block with choice D removed
		cout << "\n" << name << "  Here's Question Number " <<
			num + 1 << " (second try)";
		cout << "\n" << questions[question_index][0] << "\n";
		cout << "A. " << questions[question_index][1] << "\n";
		cout << "B. " << questions[question_index][2] << "\n";
		cout << "C. " << questions[question_index][3] << "\n";
		cout << "\n";
		break;
	}
}



/*=====================================================================
Function:Player_try()
Description: A choice validating function. Ensures that only a responce
in the form of A,B,C, or D is accepted. 
Parameters:None, returns a string of either a,b,c,or d so it can be 
compared directly to the answer[] array. 
======================================================================*/ 
string Player_try()
{
	char choice[10];
	char ch;
	int is_single = 0;
	
	
	//This block of code is responsible for ensuring that only a single
	//character can be input by the player. If we only had the lower 
	//part of the code starting at the second while loop, then the 
	//user would be able to input "ae" and the function would accept
	//that as a valid answer because only the "a" would get mapped to 
	//the choice variable. 
	cout << "\n\nYour choice?> ";
	cin >> choice;
	is_single = strlen( choice );
	while ( is_single > 1 )
	{
		cout << "You entered more than one character! Try again.> ";
		cin >> choice;
		is_single = strlen( choice );
	}

	ch = choice[0];
	ch = toupper( ch ); //uppercases the input.

	//While the answer is not A,B,C, or D, the while loop will be active 
	//until the user only inputs one character and it meets the requirements
	//of being A-D. 
	while (( ch != 'A' ) && ( ch != 'B' ) && ( ch != 'C' ) && ( ch != 'D' ))
	{
		cout << "Please select a valid letter responce!> ";
		cin >> choice;
		is_single = strlen( choice );
		while ( is_single > 1 )
		{
			cout << "You entered more than one character! Try again.> ";
			cin >> choice;
			is_single = strlen( choice );
		}
		ch = choice[0];
		ch = toupper( ch );
	}
	
	//This converts one character from the ch variable from character to 
	//a string so it can be compared directly to the answer array. 
	string answer( 1, ch );

	return answer;


}




/*=====================================================================
Function: Sort_score
Description: This function uses a bubble sort algorithm to sort the 
scores that are stored in the summary.txt file. It then determines 
which name and score belong to the player that just finished a game
and gives them an arrow pointing to their name and the place they 
ranked compared to the other scores. It displays the scores from 
highest to lowest. 
Parameters: fstream& file- the summary file that is used to read 
the names and scores that are stored in it. String player_name - 
the current player's name. int player_score - the current player's
score. Both the name and score are used to determine the current player. 
======================================================================*/ 
void Sort_score( fstream& file, string player_name, int player_score )
{
	
	bool swap = false;		//bool variable for sort 
	int i = 0;				//index for moving through array
	int score[50];			//creates array for the scores
	int temp_score[1];		//temp array to swap for sort
	string buffer;			//buffer used to change str to int
	string name[50];		//array for the names read in
	string temp_name[1];	//temp array for swapping
	
	
	while ( !file.eof() ) //while file is not at end
	{
		getline( file, name[i] ); 
		getline( file, buffer ); 
		
		//converts the string of a number to an int so that they 
		//can be properly compared and sorted. Name[i] and score[i]
		//belong to the same set so they must be kept together when 
		//sorting.
		score[i] = atoi( buffer.c_str() ); 

		i++;

	}


	
	

	do
	{

		swap = false;
		for ( int index = 0; index < ( i - 2 ); index++ )
		{
			
			//if the score is less than the score to the right of
			//it, then the name and score of the first index gets
			//stored into the temp arrays and are replaced with 
			//the score of greater value. There is 6 swaps for 
			//every pass.
			if ( score[index] < score[index + 1] )
			{
				temp_name[0] = name[index];
				temp_score[1] = score[index];
				name[index] = name[index + 1];
				score[index] = score[index + 1];
				name[index + 1] = temp_name[0];
				score[index + 1] = temp_score[1];
				swap = true;


			}
		}

	} while  (swap ); //The loop continues until no more swaps


	cout << "\n\n***********************************\n";
	cout << "            HIGHSCORES                  \n";
	cout << "***********************************\n\n";
	
	// The variable i here is the number of lines that were read from the summary
	//file. 
	for ( int y = 0; y < i - 1; y++ )
	{
		//If the player name and score match the current name and score then 
		//most likely this is the current player. The if and else if statements
		// determine which index that name and score is stored into. Adding one
		//to the index gives us the place they came in compared to the other 
		//score. Only up to 3 had to be written out because after 3 every number
		//after ends with "th". 
		if ( player_name == name[y] && player_score == score[y] )
		{
			if ( y == 0 )
				cout << ( y + 1 ) << ". " << name[y] << " ------ " << score[y] <<
				" <---- You scored 1st!" << endl;

			else if ( y == 1 )
				cout << ( y + 1 ) << ". " << name[y] << " ------ " << score[y] <<
				" <---- You scored 2nd!" << endl;

			else if ( y == 2 )
				cout << ( y + 1 ) << ". " << name[y] << " ------ " << score[y] <<
				" <---- You scored 3rd!" << endl;

			else
				cout << ( y + 1 ) << ". " << name[y] << " ------ " << score[y] <<
				" <---- You scored " << ( y + 1 ) << "th!" << endl;

		}
		//Every other score that is not our current player gets displayed as
		//normal.
		else
			cout << ( y + 1 ) << ". " << name[y] << " ------ " << score[y] << endl;
	}

}
	











































































