#define _CRT_SECURE_NO_DEPRECATE
#include <iostream>
#include <cstring>
#include <cctype>
#include "utilities.h"

using namespace std;

const int MAX_WORDS = 9000;

int runOneRound(const char words[][MAXWORDLEN + 1], int nWords, int wordnum);
bool wordInArray(char words[], const char wordArray[][MAXWORDLEN + 1]);

int main()
{
	char wordList[MAX_WORDS][MAXWORDLEN + 1];

	const char WORDFILENAME[] = "C:/Users/John/Desktop/word.txt";

	int nWords = getWords(wordList, 9000, WORDFILENAME);

	cout.setf(ios::fixed);
	cout.precision(2);

	if (nWords < 1)
	{
		cout << "No words were loaded, so I can't play the game.";
		return 1;
	}

	cout << "How many rounds do you want to play? ";
	int rounds;
	cin >> rounds;
	cin.ignore(10000, '\n');

	double total = 0;	//made double so the division can be in double
	int min = 10000000;	//set to very big so it can have an easy min set on the first round
	int max = 0;		//set to very low so it can have an easy max set on the first round

	for (int h = 1; h <= rounds; h++)
	{

		cout << "\nRound " << h << "\n";

		int score = runOneRound(wordList, nWords, randInt(0, nWords - 1));

		if (score == 1)
		{
			cout << "You got it in " << score << " try. \n";
		}
		else
		{
			cout << "You got it in " << score << " tries. \n";
		}

		if (score < min)
		{
			min = score;
		}
		if (score > max)
		{
			max = score;
		}

		total += score;

		double average = total / h;

		cout << "Average: " << average << ", minimum: " << min << ", maximum: " << max << "\n";
	}

	return 0;
}

int runOneRound(const char words[][MAXWORDLEN + 1], int nWords, int wordnum)
{
	if (nWords < 0 || wordnum < 0 || wordnum >= nWords)
	{
		return -1;
	}

	int tries = 1;

	while (true)
	{
		int stars = 0;
		int planets = 0;
		int usedLettersUser[MAXWORDLEN + 1] = { 0 };	//set the whole array to 0
		int usedLettersSecret[MAXWORDLEN + 1] = { 0 };	//set the whole array to 0

		cout << "Probe word: ";
		char userInput[100] = "";
		cin.getline(userInput, 100);

		bool allLowerCase = true;
		//checks if all the characters in the input are lowercase letters
		for (int q = 0; q < strlen(userInput); q++)
		{
			if (!islower(userInput[q]))
			{
				allLowerCase = false;
				break;
			}
		}

		//checking if the userInput is valid (the word is at least 4 letters and at most 6 and if all characters are lowercase letters
		if (strlen(userInput) > 6 || strlen(userInput) < 4 || !allLowerCase)
		{
			cout << "Your probe word must be a word of 4 to 6 lower case letters. \n";
			continue;
		}

		if (!wordInArray(userInput, words))
		{
			cout << "I don't know that word. \n";
			continue;
		}

		//compare string; if they're the same then return the number of tries it took to get correct
		if (strcmp(userInput, words[wordnum]) == 0)
		{
			return tries;
		}

		//test for stars first
		for (int i = 0; i < strlen(userInput); i++)
		{
			if (userInput[i] == words[wordnum][i])
			{
				stars++;
				usedLettersUser[i] = 1;				//creates marker for used letter in user input
				usedLettersSecret[i] = 1;		//creates marker for used letter in the secret word
			}
		}

		//test for planets after
		for (int k = 0; k < strlen(userInput); k++)
		{
			for (int l = 0; l < strlen(words[wordnum]); l++)
			{
				//check for same character, not same index, and whether the letters were used or not
				if (userInput[k] == words[wordnum][l] && k != l && usedLettersUser[k] != 1 && usedLettersSecret[l] != 1)
				{
					planets++;
					usedLettersUser[k] = 1; //set them to used
					usedLettersSecret[l] = 1; //set them to used
					break;
				}
			}
		}

		cout << "Stars: " << stars << ", Planets: " << planets << "\n";

		tries++;
	}
}

//check to see if the input word is in the text file
bool wordInArray(char words[], const char wordArray[][MAXWORDLEN + 1])
{
	bool inArray = false;

	for (int i = 0; i < MAX_WORDS; i++)
	{
		if (strcmp(words, wordArray[i]) == 0)
		{
			inArray = true;
		}
	}

	return inArray;
}