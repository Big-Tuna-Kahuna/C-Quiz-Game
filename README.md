# C-Quiz-Game
 Simple C++ console application designed as a quiz game. Players answer a series of multiple-choice questions, and their scores are recorded and sorted to display high scores at the end of each game session.

Features
Question and Answer System: The game reads questions and their corresponding answers from external text files.
Scoring System: Players earn points for each correct answer. Points increase progressively if consecutive answers are correct. Incorrect answers in a second attempt lead to a penalty.
Repeat Gameplay: Players can choose to play the game multiple times without restarting the program.
High Score Tracking: At the end of each game session, scores are saved to a file and sorted to display high scores.
Files
main.cpp: Contains the main logic of the game, including file handling, game loops, and user interaction.
questions.txt: Text file containing the quiz questions.
answers.txt: Text file containing the correct answers to the quiz questions.
summary.txt: Text file used to record and sort player scores.

Prerequisites
To run this program, ensure that you have a C++ compiler installed on your system, such as GCC, Clang, or MSVC.

Compilation and Execution
To compile and run the game, use the following commands:
# Compile the program
g++ main.cpp -o quizGame

# Run the program
./quizGame questions.txt answers.txt
Ensure that questions.txt and answers.txt are in the same directory as the executable, or provide the appropriate paths to them when executing the command.

How to Play
Start the program using the command line with the appropriate question and answer files.
Enter your name when prompted.
Answer the questions presented by typing A, B, C, or D and then pressing enter.
After each question, decide whether to attempt a second answer if the first is incorrect.
At the end of the game, view your score and high scores from previous games.
Choose whether to play again.
