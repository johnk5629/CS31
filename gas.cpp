// gas.cpp

// Portions you are to complete are marked with a TODO: comment.
// We've provided some incorrect return statements (so indicated) just
// to allow this skeleton program to compile and run, albeit incorrectly.
// The first thing you probably want to do is implement the trivial
// functions (marked TRIVIAL).  Then get City::display going.  That gives
// you more flexibility in the order you tackle the rest of the functionality.
// As you finish implementing each TODO: item, remove its TODO: comment.

#include <iostream>
#include <string>
#include <random>
#include <utility>
#include <cstdlib>
#include <cassert>

#define CHECKTYPE(f, t) { (void)(t)(f); }
using namespace std;

///////////////////////////////////////////////////////////////////////////
// Manifest constants
///////////////////////////////////////////////////////////////////////////

const int MAXROWS = 20;               // max number of rows in a city
const int MAXCOLS = 30;               // max number of columns in a city
const int MAXFLATULANS = 100;         // max number of Flatulans allowed

const int UP = 0;
const int DOWN = 1;
const int LEFT = 2;
const int RIGHT = 3;
const int NUMDIRS = 4;

///////////////////////////////////////////////////////////////////////////
// Type definitions
///////////////////////////////////////////////////////////////////////////

class City;  // This is needed to let the compiler know that City is a
			 // type name, since it's mentioned in the Flatulan declaration.

class Flatulan
{
public:
	// Constructor
	Flatulan(City* cp, int r, int c);

	// Accessors
	int  row() const;
	int  col() const;

	// Mutators
	void move();
	bool possiblyGetConverted();

private:
	City* m_city;
	int   m_row;
	int   m_col;
};

class Player
{
public:
	// Constructor
	Player(City *cp, int r, int c);

	// Accessors
	int  row() const;
	int  col() const;
	int  age() const;
	int  health() const;
	bool isPassedOut() const;

	// Mutators
	void  preach();
	void  move(int dir);
	void  getGassed();

private:
	City* m_city;
	int   m_row;
	int   m_col;
	int   m_age;
	int   m_health;
};

class City
{
public:
	// Constructor/destructor
	City(int nRows, int nCols);
	~City();

	// Accessors
	int     rows() const;
	int     cols() const;
	Player* player() const;
	int     flatulanCount() const;
	int     nFlatulansAt(int r, int c) const;
	bool    determineNewPosition(int& r, int& c, int dir) const;
	void    display() const;

	// Mutators
	bool  addFlatulan(int r, int c);
	bool  addPlayer(int r, int c);
	void  preachToFlatulansAroundPlayer();
	void  moveFlatulans();

private:
	int       m_rows;
	int       m_cols;
	Player  * m_player;
	Flatulan* m_flatulans[MAXFLATULANS];
	int       m_nFlatulans;

	// Helper functions
	bool isInBounds(int r, int c) const;
};

class Game
{
public:
	// Constructor/destructor
	Game(int rows, int cols, int nFlatulans);
	~Game();

	// Mutators
	void play();

private:
	City* m_city;
};

///////////////////////////////////////////////////////////////////////////
//  Auxiliary function declarations
///////////////////////////////////////////////////////////////////////////

int decodeDirection(char dir);
int randInt(int min, int max);
void clearScreen();

///////////////////////////////////////////////////////////////////////////
//  Flatulan implementation
///////////////////////////////////////////////////////////////////////////

Flatulan::Flatulan(City* cp, int r, int c)
{
	if (cp == nullptr)
	{
		cout << "***** A Flatulan must be created in some City!" << endl;
		exit(1);
	}
	if (r < 1 || r > cp->rows() || c < 1 || c > cp->cols())
	{
		cout << "***** Flatulan created with invalid coordinates (" << r << ","
			<< c << ")!" << endl;
		exit(1);
	}
	m_city = cp;
	m_row = r;
	m_col = c;
}

int Flatulan::row() const
{
	return m_row;
}

int Flatulan::col() const
{
	return m_col;
}

void Flatulan::move()
{
	// Attempt to move in a random direction; if we can't move, don't move.
	// If the player is there, don't move.
	int dir = randInt(0, NUMDIRS - 1);  // dir is now UP, DOWN, LEFT, or RIGHT

	switch (dir)
	{
		case UP:
			if (!(m_city->player()->row() == (m_row - 1) && m_city->player()->col() == m_col))	//checks if player is above
			{
				m_city->determineNewPosition(m_row, m_col, dir);	//moves Flatulan
			}
			break;
		case DOWN:
			if (!(m_city->player()->row() == (m_row + 1) && m_city->player()->col() == m_col))	//checks if player is below
			{
				m_city->determineNewPosition(m_row, m_col, dir);
			}
			break;
		case LEFT:
			if (!(m_city->player()->col() == (m_col - 1) && m_city->player()->row() == m_row))	//checks if player is to the left
			{
				m_city->determineNewPosition(m_row, m_col, dir);
			}
			break;
		case RIGHT:
			if (!(m_city->player()->col() == (m_col + 1) && m_city->player()->row() == m_row))	//checks if player is to the right
			{
				m_city->determineNewPosition(m_row, m_col, dir);
			}
			break;
		default:
			break;
	}
}

bool Flatulan::possiblyGetConverted()
{
	return randInt(1, 3) >= 2;		//generates random int from 1-3, return true if the result is 2 or 3 (2/3 chance since 2 numbers out of 3 choices)
}

///////////////////////////////////////////////////////////////////////////
//  Player implementations
///////////////////////////////////////////////////////////////////////////

Player::Player(City* cp, int r, int c)
{
	if (cp == nullptr)
	{
		cout << "***** The player must be created in some City!" << endl;
		exit(1);
	}
	if (r < 1 || r > cp->rows() || c < 1 || c > cp->cols())
	{
		cout << "**** Player created with invalid coordinates (" << r
			<< "," << c << ")!" << endl;
		exit(1);
	}
	m_city = cp;
	m_row = r;
	m_col = c;
	m_age = 0;
	m_health = 10;
}

int Player::row() const
{
	return m_row;
}

int Player::col() const
{
	return m_col;
}

int Player::age() const
{
	return m_age;
}

int Player::health() const
{
	return m_health;
}

bool Player::isPassedOut() const
{
	return (m_health <= 0);
}

void Player::preach()
{
	m_age++;
	m_city->preachToFlatulansAroundPlayer();
}

void Player::move(int dir)
{
	m_age++;
	switch (dir)
	{
		case UP:
			if (m_city->nFlatulansAt(m_row - 1, m_col) == 0)	//checks if Flatulans are above
			{
				m_city->determineNewPosition(m_row, m_col, dir);	//moves player
			}
			break;
		case DOWN:
			if (m_city->nFlatulansAt(m_row + 1, m_col) == 0)	//checks if Flatulans are below
			{
				m_city->determineNewPosition(m_row, m_col, dir);
			}
			break;
		case LEFT:
			if (m_city->nFlatulansAt(m_row, m_col - 1) == 0)	//checks if Flatulans are to the left
			{
				m_city->determineNewPosition(m_row, m_col, dir);
			}
			break;
		case RIGHT:
			if (m_city->nFlatulansAt(m_row, m_col + 1) == 0)	//checks if Flatulans are to the right
			{
				m_city->determineNewPosition(m_row, m_col, dir);
			}
			break;
		default:
			break;
	}
}

void Player::getGassed()
{
	m_health--;
}

///////////////////////////////////////////////////////////////////////////
//  City implementations
///////////////////////////////////////////////////////////////////////////

City::City(int nRows, int nCols)
{
	if (nRows <= 0 || nCols <= 0 || nRows > MAXROWS || nCols > MAXCOLS)
	{
		cout << "***** City created with invalid size " << nRows << " by "
			<< nCols << "!" << endl;
		exit(1);
	}
	m_rows = nRows;
	m_cols = nCols;
	m_player = nullptr;
	m_nFlatulans = 0;
}

City::~City()
{
	delete m_player;
	for (int i = 0; i < m_nFlatulans; i++)
	{
		delete m_flatulans[i];
	}
	m_nFlatulans = 0;
}

int City::rows() const
{
	return m_rows;
}

int City::cols() const
{
	return m_cols;
}

Player* City::player() const
{
	return m_player;
}

int City::flatulanCount() const
{
	return m_nFlatulans;
}

int City::nFlatulansAt(int r, int c) const
{
	int flatulanCount = 0;

	for (int f = 0; f < m_nFlatulans; f++)
	{
		if (m_flatulans[f]->row() == r && m_flatulans[f]->col() == c)	//check for row and column of flatulan
		{
			flatulanCount++;	//increase flatulan count
		}
	}

	return flatulanCount;
}

bool City::determineNewPosition(int& r, int& c, int dir) const
{
	switch (dir)
	{
	case UP:
		if ((r - 1) < 1)		//check if the move would go past the boundaries
		{
			return false;
		}
		else
		{
			r--;				//moves row number down since it goes from 1-end from top down
		}
		break;
	case DOWN:
		if ((r + 1) > m_rows)	//check if the move would go past the boundaries
		{
			return false;
		}
		else
		{
			r++;
		}
		break;
	case LEFT:
		if ((c - 1) < 1)		//check if the move would go past the boundaries
		{
			return false;
		}
		else
		{
			c--;
		}
		break;
	case RIGHT:
		if ((c + 1) > m_cols)	//check if the move would go past the boundaries
		{
			return false;
		}
		else
		{
			c++;
		}
		break;
	default:
		return false;
	}
	return true;
}

void City::display() const
{
	// Position (row,col) in the city coordinate system is represented in
	// the array element grid[row-1][col-1]
	char grid[MAXROWS][MAXCOLS];
	int r, c;

	// Fill the grid with dots
	for (r = 0; r < rows(); r++)
		for (c = 0; c < cols(); c++)
			grid[r][c] = '.';


	for (r = 1; r <= rows(); r++)
		for (c = 1; c <= cols(); c++)
		{
			switch (nFlatulansAt(r, c))
			{
				case 0:
					grid[r - 1][c - 1] = '.';
					break;
				case 1:
					grid[r - 1][c - 1] = 'F';
					break;
				case 2:
					grid[r - 1][c - 1] = '2';
					break;
				case 3:
					grid[r - 1][c - 1] = '3';
					break;
				case 4:
					grid[r - 1][c - 1] = '4';
					break;
				case 5:
					grid[r - 1][c - 1] = '5';
					break;
				case 6:
					grid[r - 1][c - 1] = '6';
					break;
				case 7:
					grid[r - 1][c - 1] = '7';
					break;
				case 8:
					grid[r - 1][c - 1] = '8';
					break;
				case 9:
					grid[r - 1][c - 1] = '9';
					break;
				default:
					grid[r - 1][c - 1] = '9';
					break;
			}
		}

	// Indicate player's position
	if (m_player != nullptr)
	{
		// Set the char to '@', unless there's also a Flatulan there
		// (which should never happen), in which case set it to '*'.
		char& gridChar = grid[m_player->row() - 1][m_player->col() - 1];
		if (gridChar == '.')
			gridChar = '@';
		else
			gridChar = '*';
	}

	// Draw the grid
	clearScreen();
	for (r = 0; r < rows(); r++)
	{
		for (c = 0; c < cols(); c++)
			cout << grid[r][c];
		cout << endl;
	}
	cout << endl;

	// Write message, Flatulan, and player info
	cout << "There are " << m_nFlatulans << " unconverted Flatulans remaining." << endl;
	if (m_player == nullptr)
		cout << "There is no player." << endl;
	else
	{
		if (m_player->age() > 0)
			cout << "The player has lasted " << m_player->age() << " steps." << endl;
		if (m_player->isPassedOut())
			cout << "The player has passed out." << endl;
		else
			cout << "The player's health level is " << m_player->health() << endl;
	}
}

bool City::addFlatulan(int r, int c)
{
	if (!isInBounds(r, c))
		return false;

	// Don't add a Flatulan on a spot with a player
	if (m_player != nullptr  &&  m_player->row() == r  &&  m_player->col() == c)
		return false;

	if (m_nFlatulans == MAXFLATULANS)
	{
		return false;
	}
	else
	{
		m_flatulans[m_nFlatulans] = new Flatulan(this, r, c);	//dynamically allowcate new Flatulan
		m_nFlatulans++;
	}

	return true;
}

bool City::addPlayer(int r, int c)
{
	if (!isInBounds(r, c))
		return false;

	// Don't add a player if one already exists
	if (m_player != nullptr)
		return false;

	// Don't add a player on a spot with a Flatulan
	if (nFlatulansAt(r, c) > 0)
		return false;

	// Dynamically allocate a new Player and add it to the city
	m_player = new Player(this, r, c);
	return true;
}

void City::preachToFlatulansAroundPlayer()
{
	// Preach to Flatulans orthogonally or diagonally adjacent to player.
	// If a Flatulan is converted, destroy it and remove it from the city,
	// since we have no further need to display it or have it interact with
	// the player.

	for (int i = 0; i < m_nFlatulans; i++)
	{
		if (m_nFlatulans != 0 && m_player->row() + 1 == m_flatulans[i]->row() && m_player->col() == m_flatulans[i]->col() && m_flatulans[i]->possiblyGetConverted())	//checks bottom
		{
			delete m_flatulans[i];
			for (int k = i; k < m_nFlatulans; k++)
			{
				m_flatulans[k] = m_flatulans[k + 1];	//shift the flatulan array by one to eliminate the pointer that got its object deleted
			}
			m_nFlatulans--;				//reduce m_nFlatulans to make sure the array is not looped through meaningless items

			if (i > 0)	//have to make sure that i does not become a negative number
			{
				i--;	//gotta shift i since the array of Flatulan pointer have been shifted down by 1
			}
		}
		if (m_nFlatulans != 0 && m_player->row() + 1 == m_flatulans[i]->row() && m_player->col() == m_flatulans[i]->col() + 1 && m_flatulans[i]->possiblyGetConverted())	//checks bottom-right
		{
			delete m_flatulans[i];
			for (int k = i; k < m_nFlatulans; k++)
			{
				m_flatulans[k] = m_flatulans[k + 1];
			}
			m_nFlatulans--;

			if (i > 0)
			{
				i--;
			}
		}
		if (m_nFlatulans != 0 && m_player->row() + 1 == m_flatulans[i]->row() && m_player->col() == m_flatulans[i]->col() - 1 && m_flatulans[i]->possiblyGetConverted())	//checks bottom-left
		{
			delete m_flatulans[i];
			for (int k = i; k < m_nFlatulans; k++)
			{
				m_flatulans[k] = m_flatulans[k + 1];
			}
			m_nFlatulans--;

			if (i > 0)
			{
				i--;
			}
		}
		if (m_nFlatulans != 0 && m_player->row() - 1 == m_flatulans[i]->row() && m_player->col() == m_flatulans[i]->col() && m_flatulans[i]->possiblyGetConverted())	//checks top
		{
			delete m_flatulans[i];
			for (int k = i; k < m_nFlatulans; k++)
			{
				m_flatulans[k] = m_flatulans[k + 1];
			}
			m_nFlatulans--;

			if (i > 0)
			{
				i--;
			}
		}
		if (m_nFlatulans != 0 && m_player->row() - 1 == m_flatulans[i]->row() && m_player->col() == m_flatulans[i]->col() + 1 && m_flatulans[i]->possiblyGetConverted())	//checks top-right
		{
			delete m_flatulans[i];
			for (int k = i; k < m_nFlatulans; k++)
			{
				m_flatulans[k] = m_flatulans[k + 1];
			}
			m_nFlatulans--;

			if (i > 0)
			{
				i--;
			}
		}
		if (m_nFlatulans != 0 && m_player->row() - 1 == m_flatulans[i]->row() && m_player->col() == m_flatulans[i]->col() - 1 && m_flatulans[i]->possiblyGetConverted())	//checks top-left
		{
			delete m_flatulans[i];
			for (int k = i; k < m_nFlatulans; k++)
			{
				m_flatulans[k] = m_flatulans[k + 1];
			}
			m_nFlatulans--;

			if (i > 0)
			{
				i--;
			}
		}
		if (m_nFlatulans != 0 && m_player->row() == m_flatulans[i]->row() && m_player->col() == m_flatulans[i]->col() + 1 && m_flatulans[i]->possiblyGetConverted())	//checks right
		{
			delete m_flatulans[i];
			for (int k = i; k < m_nFlatulans; k++)
			{
				m_flatulans[k] = m_flatulans[k + 1];
			}
			m_nFlatulans--;

			if (i > 0)
			{
				i--;
			}
		}
		if (m_nFlatulans != 0 && m_player->row() == m_flatulans[i]->row() && m_player->col() == m_flatulans[i]->col() - 1 && m_flatulans[i]->possiblyGetConverted())	//checks left
		{
			delete m_flatulans[i];
			for (int k = i; k < m_nFlatulans; k++)
			{
				m_flatulans[k] = m_flatulans[k + 1];
			}
			m_nFlatulans--;

			if (i > 0)
			{
				i--;
			}
		}
	}


	// TODO:  Implement this.
}

void City::moveFlatulans()
{
	for (int k = 0; k < m_nFlatulans; k++)
	{
		m_flatulans[k]->move();


		if (m_player->row() == m_flatulans[k]->row() + 1 && m_player->col() == m_flatulans[k]->col())		//checks if the player is below Flatulan
		{
			m_player->getGassed();
		}
		else if (m_player->row() == m_flatulans[k]->row() - 1 && m_player->col() == m_flatulans[k]->col())	//checks if the player is above Flatulan
		{
			m_player->getGassed();
		}
		else if (m_player->col() == m_flatulans[k]->col() + 1 && m_player->row() == m_flatulans[k]->row())	//checks if the player is to the right of Flatulan
		{
			m_player->getGassed();
		}
		else if (m_player->col() == m_flatulans[k]->col() - 1 && m_player->row() == m_flatulans[k]->row())	//checks if the player is to the left of Flatulan
		{
			m_player->getGassed();
		}
	}
}

bool City::isInBounds(int r, int c) const
{
	return (r >= 1 && r <= m_rows  &&  c >= 1 && c <= m_cols);
}

///////////////////////////////////////////////////////////////////////////
//  Game implementations
///////////////////////////////////////////////////////////////////////////

Game::Game(int rows, int cols, int nFlatulans)
{
	if (nFlatulans < 0)
	{
		cout << "***** Cannot create Game with negative number of Flatulans!" << endl;
		exit(1);
	}
	if (nFlatulans > MAXFLATULANS)
	{
		cout << "***** Trying to create Game with " << nFlatulans
			<< " Flatulans; only " << MAXFLATULANS << " are allowed!" << endl;
		exit(1);
	}
	if (rows == 1 && cols == 1 && nFlatulans > 0)
	{
		cout << "***** Cannot create Game with nowhere to place the Flatulans!" << endl;
		exit(1);
	}

	// Create city
	m_city = new City(rows, cols);

	// Add player
	int rPlayer = randInt(1, rows);
	int cPlayer = randInt(1, cols);
	m_city->addPlayer(rPlayer, cPlayer);

	// Populate with Flatulans
	while (nFlatulans > 0)
	{
		int r = randInt(1, rows);
		int c = randInt(1, cols);
		// Don't put a Flatulan where the player is
		if (r == rPlayer  &&  c == cPlayer)
			continue;
		m_city->addFlatulan(r, c);
		nFlatulans--;
	}
}

Game::~Game()
{
	delete m_city;
}

void Game::play()
{
	m_city->display();
	Player* p = m_city->player();
	if (p == nullptr)
		return;

	while (!p->isPassedOut() && m_city->flatulanCount() > 0)
	{
		cout << "Move (u/d/l/r//q): ";
		string action;
		getline(cin, action);
		if (action.size() == 0)  // player preaches
			p->preach();
		else
		{
			switch (action[0])
			{
			default:   // if bad move, nobody moves
				cout << '\a' << endl;  // beep
				continue;
			case 'q':
				return;
			case 'u':
			case 'd':
			case 'l':
			case 'r':
				p->move(decodeDirection(action[0]));
				break;
			}
		}
		m_city->moveFlatulans();
		m_city->display();
	}
	if (p->isPassedOut())
		cout << "You lose." << endl;
	else
		cout << "You win." << endl;
}

///////////////////////////////////////////////////////////////////////////
//  Auxiliary function implementations
///////////////////////////////////////////////////////////////////////////

int decodeDirection(char dir)
{
	switch (dir)
	{
	case 'u':  return UP;
	case 'd':  return DOWN;
	case 'l':  return LEFT;
	case 'r':  return RIGHT;
	}
	return -1;  // bad argument passed in!
}

// Return a random int from min to max, inclusive
int randInt(int min, int max)
{
	if (max < min)
		swap(max, min);
	static random_device rd;
	static mt19937 generator(rd());
	uniform_int_distribution<> distro(min, max);
	return distro(generator);
}


///////////////////////////////////////////////////////////////////////////
//  main()
///////////////////////////////////////////////////////////////////////////

int main()
{
	// Create a game
	// Use this instead to create a mini-game:   Game g(3, 4, 2);
	Game g(7, 8, 25);

	// Play the game
	g.play();
}


///////////////////////////////////////////////////////////////////////////
//  clearScreen implementation
///////////////////////////////////////////////////////////////////////////

// DO NOT MODIFY OR REMOVE ANY CODE BETWEEN HERE AND THE END OF THE FILE!!!
// THE CODE IS SUITABLE FOR VISUAL C++, XCODE, AND g++ UNDER LINUX.

// Note to Xcode users:  clearScreen() will just write a newline instead
// of clearing the window if you launch your program from within Xcode.
// That's acceptable.  (The Xcode output window doesn't have the capability
// of being cleared.)

#ifdef _MSC_VER  //  Microsoft Visual C++

#include <windows.h>

void clearScreen()
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(hConsole, &csbi);
	DWORD dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
	COORD upperLeft = { 0, 0 };
	DWORD dwCharsWritten;
	FillConsoleOutputCharacter(hConsole, TCHAR(' '), dwConSize, upperLeft,
		&dwCharsWritten);
	SetConsoleCursorPosition(hConsole, upperLeft);
}

#else  // not Microsoft Visual C++, so assume UNIX interface

#include <iostream>
#include <cstring>
#include <cstdlib>

void clearScreen()  // will just write a newline in an Xcode output window
{
	static const char* term = getenv("TERM");
	if (term == nullptr || strcmp(term, "dumb") == 0)
		cout << endl;
	else
	{
		static const char* ESC_SEQ = "\x1B[";  // ANSI Terminal esc seq:  ESC [
		cout << ESC_SEQ << "2J" << ESC_SEQ << "H" << flush;
	}
}

#endif