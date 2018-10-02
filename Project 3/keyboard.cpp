#include <iostream>
#include <string>
#include <cctype>
#include <cassert>

using namespace std;

bool checkBeatEnd(string song, int& pos);

bool checkIfNote(string song, int& pos);

bool checkAccidentals(string song, int& pos);

bool checkOctave(string song, int& pos);

bool hasCorrectSyntax(string song);

char encodeNote(string noteSub);

bool isChord(string song, int pos);

int encodeSong(string song, string& instructions, int& badBeat);


int main()
{
	assert(hasCorrectSyntax("D5//D/"));
	assert(!hasCorrectSyntax("D5//Z/"));
	string instrs;
	int badb;
	instrs = "xxx"; badb = -999; // so we can detect whether these get changed
	assert(encodeSong("D5//D/", instrs, badb) == 0 && instrs == "R H"  &&  badb == -999);
	instrs = "xxx"; badb = -999; // so we can detect whether these get changed
	assert(encodeSong("D5//Z/", instrs, badb) == 1 && instrs == "xxx"  &&  badb == -999);
	assert(encodeSong("D5//D8/", instrs, badb) == 2 && instrs == "xxx"  &&  badb == 3);
	instrs = "xxx"; badb = -999;
	assert(encodeSong("G3B3DD5//G/A/A3B/C5/B3D5//G//G//CE5//C5/D5/E5/F#5/B3G5//G//G/", instrs, badb) == 0 && instrs == "[SFHR] LQ[DW]E[FR] L L [GT] ERT*[FU] L L"  &&  badb == -999);


	cerr << "All tests succeeded" << endl;

	return 0;
}

bool checkBeatEnd(string song, int& pos)
{
	//check if valid position
	if (pos < song.size())
	{
		if (song[pos] == '/')
		{
			pos++;
			return true;
		}
		else
			return false;
	}
}

bool checkIfNote(string song, int& pos)
{
	//check if valid position
	if (pos < song.size())
	{
		switch (song[pos])
		{
		case 'A':
		case 'B':
		case 'C':
		case 'D':
		case 'E':
		case 'F':
		case 'G':
			pos++; //increment pos to move on to the next character
			return true;
			break;
		default:
			return false;
			break;
		}
	}
}


//check for accidentals after the letter
bool checkAccidentals(string song, int& pos)
{
	//checkIfNote is assumed to have already been completed at this point

	//pos is changed so it should be checked if it's within the string bound
	//checks if its a valid accidental
	if (pos >= song.length() || !(song[pos] == '#' || song[pos] == 'b'))
		return false;

	pos++; //moves the pos value to get ready to check the next value
	return true; //only case left is true


	// as long as it's within bounds of the string return the value of if the accidental is valid
}

//check the octave after the letter
bool checkOctave(string song, int& pos)
{
	//checkIfNote and/or checkAccidentals should have been completed by this point

	//pos is changed so it should be checked if it's within the string bound
	if (pos >= song.length())
		return false;

	//checks if the character is a digit aka an octave
	if (isdigit(song[pos]))
	{
		pos++;
		return true;
	}
	else
	{
		return false;
	}
}

bool hasCorrectSyntax(string song)
{
	if (song.size() == 0)
		return true;

	int i = 0;
	while (i < song.size())
	{
		//checks if the last character is a '/'
		if (i == (song.size() - 1))
		{
			//last character must be a '/' or it does not have correct syntax
			if (checkBeatEnd(song, i))
				return true;
			else
				return false;
		}

		//checking the song string
		if (checkIfNote(song, i)) //checks if its a note
		{
			//checks the next character following the note
			if (checkAccidentals(song, i))
			{
				//if accidental follows check if octave also follows
				if (checkOctave(song, i))
				{
					continue; //valid, go back to beginning of loop to test next character
				}
				else
				{
					continue; //regardless of what follows just go back to the top of the loop to evaluate that character
				}
			}
			//if octave follows, by the order of characters established, "no accidentals" can be assumed
			else if (checkOctave(song, i))
				continue; //regardless of what follows just go back to the top of the loop to evaluate that character
			else
				continue; //regardless of what follows just go back to the top of the loop to evaluate that character
		}
		else if (checkBeatEnd(song, i))
		{
			continue; //go back to bebginning of loop
		}
		else
		{
			return false;
		}
	}
}


char encodeNote(string noteSub)
{
	int octave = 4; //no octave is defaulted to 4
	char noteLetter = 'C';
	char accidentalSign = ' ';

	//if the noteSubstring is blank, meaning the substring only had a "/", it should return a space for rest
	if (noteSub == "")
		return ' ';

	int counter = 0;

	//one iteration is most likely enough because the note is formatted in order (therefore no need for "continue;")
	while (counter < noteSub.size())
	{
		if (checkIfNote(noteSub, counter))
		{
			noteLetter = noteSub[counter - 1]; //counter was added 1 when the condition was met
		}

		if (checkAccidentals(noteSub, counter))
		{
			accidentalSign = noteSub[counter - 1];
		}

		if (checkOctave(noteSub, counter))
		{
			octave = noteSub[counter - 1] - '0'; //change the char casted to int (which is bad) into the int value that we want
		}
	}

	// This check is here solely to report a common CS 31 student error.
	if (octave > 9)
	{
		cerr << "********** encodeNote was called with first argument = "
			<< octave << endl;
	}

	// Convert Cb, C, C#/Db, D, D#/Eb, ..., B, B#
	//      to -1, 0,   1,   2,   3, ...,  11, 12

	int note;
	switch (noteLetter)
	{
	case 'C':  note = 0; break;
	case 'D':  note = 2; break;
	case 'E':  note = 4; break;
	case 'F':  note = 5; break;
	case 'G':  note = 7; break;
	case 'A':  note = 9; break;
	case 'B':  note = 11; break;
	default:   return ' ';
	}
	switch (accidentalSign)
	{
	case '#':  note++; break;
	case 'b':  note--; break;
	case ' ':  break;
	default:   return ' ';
	}

	// Convert ..., A#1, B1, C2, C#2, D2, ... to
	//         ..., -2,  -1, 0,   1,  2, ...

	int sequenceNumber = 12 * (octave - 2) + note;

	string keymap = "Z1X2CV3B4N5M,6.7/A8S9D0FG!H@JK#L$Q%WE^R&TY*U(I)OP";
	if (sequenceNumber < 0 || sequenceNumber >= keymap.size())
		return '?'; //return a non-"keymap" character to know an invalid note
	return keymap[sequenceNumber];
}

bool isChord(string song, int pos)
{
	int counter = 0;

	for (int i = pos; i < song.size(); i++)
	{
		if (song[i] == '/')
			break;

		if (isupper(song[i]))
			counter++;
	}

	return counter > 1;
}

int encodeSong(string song, string& instructions, int& badBeat)
{
	//not a song string --> return 1
	if (!(hasCorrectSyntax(song)))
		return 1;

	if (hasCorrectSyntax(song))
	{
		int k = 0;
		string encodedSong = "";
		int beat = 1; //starts at beat 1
		bool inChord = false; //checks if the current substring is part of a chord

		while (k < song.size())
		{
			if (isChord(song, k) && !inChord)
			{
				encodedSong += "[";
				inChord = true;

			}

			//create a substring consisting of a single note to encode
			string substring = "";
			int noteCounter = 0; //initialized outside so that it's reset to 0 every time it's called
			for (int j = k; j < song.size(); j++)
			{
				if (song[j] == '/')
				{
					break;
				}
				if (isalpha(song[j]) && noteCounter > 0) //makes sure that the substring has only one note
				{
					break;
				}

				substring += song[j];
				k++;
				noteCounter++;
			}

			if (encodeNote(substring) == '?') //checks if the song is playable
			{
				badBeat = beat;
				return 2;
			}

			encodedSong += encodeNote(substring); //encode the note and add to the encoded song string

			if (song[k] == '/')
			{
				if (inChord)
				{
					encodedSong += "]"; //closes the chord brackets
					inChord = false; //sets back to false for defaulting purposes
				}
				beat++; //increments after every successful beat
				k++; //increments k after checking
			}
		}
		instructions = encodedSong; //set instructions as the "encodedSong" variable
	}
	return 0;
}