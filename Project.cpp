#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <limits>
using namespace std;

// Function to simulate rolling a 6-sided dice
int rollDice() {
    return (rand() % 6) + 1;
}

// Function to check if the player has landed on a snake or a ladder
int checkSnakesAndLadders(int position) {
    switch(position) {
        // Ladders
        case 3: return 22;
        case 5: return 8;
        case 11: return 26;
        case 20: return 29;
        case 27: return 56;
        case 36: return 55;
        case 51: return 72;
        case 71: return 92;
        case 80: return 99;

        // Snakes
        case 17: return 4;
        case 19: return 7;
        case 21: return 9;
        case 28: return 1;
        case 54: return 34;
        case 62: return 18;
        case 64: return 60;
        case 87: return 24;
        case 93: return 73;
        case 95: return 75;
        case 98: return 79;

        // No snake or ladder at this position
        default: return position;
    }
}

// Function to display the welcome message and instructions
void displayWelcomeMessage() {
    cout << "==============================="<<endl;
    cout << "   Snake and Ladder Game   "<<endl;
    cout << "==============================="<<endl;
    cout << "Instructions:"<<endl;
    cout << "- Two players take turns."<<endl;
    cout << "- First to reach 100 wins."<<endl;
    cout << "- Landing on a ladder moves you up."<<endl;
    cout << "- Landing on a snake pulls you down."<<endl<<endl;
    cout << "Good luck!"<<endl<<endl;
}

// Function to display past game results
void displayGameHistory() {
    ifstream inFile("game_results.txt");

    if (inFile.is_open()) {
        cout << "\n Game History"<<endl;
        string line;
        while (inFile>>line) {
            cout << line << endl;
        }
        inFile.close();
        cout << "------------------------------"<<endl;
    } else {
        cout << "\nNo game history found."<<endl;
    }
}

// Function to save game result to a file
void saveGameResult(const string& winner, const string& player1, const string& player2, int totalTurns) {
    ofstream outFile("game_results.txt"); // append mode

    if (outFile.is_open()) {
        outFile << "Winner: " << winner << endl;
        outFile << "Player 1: " << player1 << ", Player 2: " << player2 << endl;
        outFile << "Total Turns: " << totalTurns << endl;
        outFile << "------------------------------" << endl;
        outFile.close();
    } else {
        cerr << "Error: Unable to open file to save results." << endl;
    }
}

// Function to take a turn for a player
int takeTurn(const string& playerName, int position, int turnNumber) {
    cout << "\nTurn " << turnNumber << ": " << playerName << "'s turn. Press Enter to roll the dice...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    int dice = rollDice();  // Roll the dice
    cout << playerName << " rolled a " << dice << "!\n";

    if (position + dice > 100) {
        cout << "Roll exceeds 100. No movement this turn."<<endl;
        return position;
    }

    position += dice;
    cout << playerName << " moves to " << position << ".\n";
    int newPosition = checkSnakesAndLadders(position);
    if (newPosition != position) {
        if (newPosition > position) {
            cout << playerName << " found a ladder! Climbs up to " << newPosition << ".\n";
        } else {
            cout << playerName << " got bitten by a snake! Slides down to " << newPosition << ".\n";
        }
    }

    return newPosition;
}

// Function to ask the player if they want to play again
bool playAgainPrompt() {
    char choice;
    cout << "\nWould you like to play again? (y/n): ";
    cin >> choice;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return (choice == 'y' || choice == 'Y');
}

// The main function that runs the game
int main() {
    srand(static_cast<unsigned int>(time(0))); // Seed the random number generator

    string player1, player2;
    bool playAgain = true;

    while (playAgain) {
        int pos1 = 0, pos2 = 0;
        int turnCount = 1;

        displayWelcomeMessage();
        displayGameHistory();

        // Input player names
        cout << "Enter name for Player 1: ";
        cin >> player1;
        cout << "Enter name for Player 2: ";
        cin >> player2;
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // clear input buffer

        // Prevent same name
        while (player1 == player2) {
            cout << "Both players can't have the same name. Enter a different name for Player 2: ";
            cin >> player2;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        // Start game loop
        while (true) {
            pos1 = takeTurn(player1, pos1, turnCount++);
            if (pos1 == 100) {
                cout << "\n Congratulations " << player1 << "! You reached 100 and won the game! "<<endl;
                saveGameResult(player1, player1, player2, turnCount);
                break;
            }

            pos2 = takeTurn(player2, pos2, turnCount++);
            if (pos2 == 100) {
                cout << "\n Congratulations " << player2 << "! You reached 100 and won the game! "<<endl;
                saveGameResult(player2, player1, player2, turnCount);
                break;
            }

            // Display current positions after each round
            cout << "\nCurrent Positions:\n";
            cout << player1 << ": " << pos1 << "\n";
            cout << player2 << ": " << pos2 << "\n";
            cout << "----------------------------------\n";
        }

        // Ask if they want to replay
        playAgain = playAgainPrompt();
    }

    cout << "\nThanks for playing! See you next time! "<<endl;
    return 0;
}
