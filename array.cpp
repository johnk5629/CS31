#include <iostream>
#include <string>
#include <cassert>

using namespace std;

int appendToAll(string a[], int n, string value);
int lookup(const string a[], int n, string target);
int positionOfMax(const string a[], int n);
int rotateLeft(string a[], int n, int pos);
int countRuns(const string a[], int n);
int flip(string a[], int n);
int differ(const string a1[], int n1, const string a2[], int n2);
int subsequence(const string a1[], int n1, const string a2[], int n2);
int lookupAny(const string a1[], int n1, const string a2[], int n2);
int split(string a[], int n, string splitter);


int main()
{
	string h[7] = { "selina", "reed", "diana", "tony", "", "logan", "peter" };
	assert(lookup(h, 7, "logan") == 5);
	assert(lookup(h, 7, "diana") == 2);
	assert(lookup(h, 2, "diana") == -1);
	assert(positionOfMax(h, 7) == 3);

	string g[4] = { "selina", "reed", "peter", "sue" };
	assert(differ(h, 2, g, 1) == 1);
	
	assert(appendToAll(g, 4, "?") == 4 && g[0] == "selina?" && g[3] == "sue?");
	assert(rotateLeft(g, 4, 1) == 1 && g[1] == "peter?" && g[3] == "reed?");

	string e[4] = { "diana", "tony", "", "logan" };
	assert(subsequence(h, 7, e, 4) == 2);

	string d[5] = { "reed", "reed", "reed", "tony", "tony" };
	assert(countRuns(d, 5) == 2);

	string f[3] = { "peter", "diana", "steve" };
	assert(lookupAny(h, 7, f, 3) == 2);
	assert(flip(f, 3) == 3 && f[0] == "steve" && f[2] == "peter");

	assert(split(h, -2, "z") == -1);

	string w[2] = { "sue", "steve" };
	assert(split(w, 2, "sue") == 1);
	
	cout << "All tests succeeded" << endl;


	return 0;
}


int appendToAll(string a[], int n, string value)
{
	if (n >= 0) //0 elements do not matter
	{
		for (int i = 0; i < n; i++)
		{
			a[i] += value;
		}
		return n;
	}
	else 
	{
		return -1;
	}
}

int lookup(const string a[], int n, string target)
{
	if (n >= 0) //returns -1 if target isn't found anyways; whether there is an element or not is not significant
	{
		for (int i = 0; i < n; i++)
		{
			if (a[i] == target)
			{
				return i;
			}
		}
		return -1;
	}
	return -1;
}

int positionOfMax(const string a[], int n)
{
	if (n > 0) //has to have at least one element
	{
		int greatestString = 0;
		for (int i = 0; i < n; i++)
		{
			if (a[i] >= a[greatestString])
			{
				if (a[i] != a[greatestString])
				{
					greatestString = i;
				}
			}
		}
		return greatestString;
	}
	return -1;
}

int rotateLeft(string a[], int n, int pos)
{
	if (pos >= n) //cannot rotate anything that's outside the boundary of the array
	{
		return -1;
	}

	if (n > 0 && pos >= 0) //cannot rotate an array with no elements and position must be >= 0
	{
		string copy = a[pos]; //set a copy of the original string

		for (int i = pos; i < (n - 1); i++)
		{
			a[i] = a[i + 1]; //move the strings to the left
		}
		a[n - 1] = copy; // set the original at the end of the array
		return pos;
	}
	return -1;
}

int countRuns(const string a[], int n)
{
	if (n == 0) // no elements = no runs
	{
		return 0;
	}
	else if (n > 0)
	{
		int runs = 1; //with at least one element, one run is guaranteed
		for (int i = 0; i < (n - 1); i++) //evaluates only up to "< (n -1)" to prevent the index from going out of bounds in the next if statement
		{
			if (a[i] != a[i + 1]) //for every non identical consecutive elements, add a run
			{
				runs++;
			}
		}
		return runs;
	}
	return -1;
}

int flip(string a[], int n)
{
	if (n > 0) //cannot flip a function with 0 or less elements
	{
		for (int i = 0; i < n; i++)
		{
			if (i < ((n - 1) - i)) //prevents from doing the algorithm to the second half of the array, which would just flip it back
			{
				string copy = a[(n - 1) - i];
				a[(n - 1) - i] = a[i];
				a[i] = copy;
			}
			else
				break;
		}
		return n;
	}
	return -1;
}

int differ(const string a1[], int n1, const string a2[], int n2)
{
	if (n1 >= 0 && n2 >= 0) //arrays with no elements can be compared
	{
		if (n1 <= n2)
		{
			for (int i = 0; i < n1; i++)
			{
				if (a1[i] != a2[i])
				{
					return i;
				}
			}

			return n1;
		}
		else // (n2 < n1)
		{
			for (int i = 0; i < n2; i++)
			{
				if (a1[i] != a2[i])
				{
					return i;
				}
			}
			return n2;
		}
	}
	return -1;
}

int subsequence(const string a1[], int n1, const string a2[], int n2)
{
	if (n1 >= 0 && n2 >= 0 && n1 >= n2) //cannot have a subsequence bigger than the original array
	{
		if (n2 == 0) //spec specified this case
			return 0;

		int n2Counter = 0;
		int startOfSubsequence = 0;

		for (int i = 0; i < n1; i++)
		{
			if (a1[i] == a2[n2Counter]) 
			{
				if ((i + n2) < n1) //make sure the array won't go out of bounds
				{
					startOfSubsequence = i;
					while (a1[i + n2Counter] == a2[n2Counter]) //adding n2Counter to i makes sure the index of a1 also moves
					{
						n2Counter++;
						if (n2Counter == n2) //once n2Counter reaches to the max number of elements of a2
						{
							return startOfSubsequence;
						}
					}
					n2Counter = 0;
				}
				else
					break;
			}
		}
		return -1;
	}
	return -1;
}

int lookupAny(const string a1[], int n1, const string a2[], int n2)
{
	if (n1 > 0 && n2 > 0) //cannot compare the elements of the two arrays if there are no elements
	{
		for (int i = 0; i < n1; i++)
		{
			for (int j = 0; j < n2; j++)
			{
				if (a1[i] == a2[j])
				{
					return i;
				}
			}
		}
		return -1;
	}
	return -1;
}


int split(string a[], int n, string splitter)
{
	if (n > 0) //cannot split an array with no elements
	{
		int loop = 0;
		int i = 0;
		int counter = 0;

		while (loop < n) //organizes the array relative to "splitter"
		{
			if (a[i] < splitter)
			{
				//basically a rotateRight algorithm
				string copy = a[i];

				for (int j = i; j > 0; j--)
				{
					a[j] = a[j - 1];
				}
				a[0] = copy;

				i++; //add the i to analyze the next element
				loop++;
			}
			else if (a[i] > splitter)
			{
				rotateLeft(a, n, i); //the previous algorithm seemed useful for this; though the retrun value is irrelevant, the array is altered

				//leave the index alone to analyze the new element that's been moved to this position
				loop++;
			}
			else
			{
				i++; //add the i to analyze the next element
				loop++;
			}
		}

		for (int k = 0; k < n; k++) //goes through the organized array to find the spot
		{
			if (a[k] < splitter)
			{
				counter++;
			}
			else
				break;
		}
		return counter;
	}
	return -1;
}
