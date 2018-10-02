#include <iostream>
#include <string>
using namespace std;

int main()
{
	cout << "Age of rider: ";
	int age;
	cin >> age;
	cin.ignore(10000, '\n');

	cout << "Student? (y/n): ";
	string student;
	getline(cin, student);

	cout << "Destination: ";
	string dest;
	getline(cin, dest);

	cout << "Number of zone boundaries crossed: ";
	int zone;
	cin >> zone;

	cout << "---\n";

	//start of checking for errors
	if (age < 0) {
		cout << "The age must not be negative\n";
		return 1; //program stops to prevent further error message
	}

	if (student != "y" && student != "n") {
		cout << "You must enter y or n\n";
		return 1;
	}

	if (dest == "") {
		cout << "You must enter a destination\n";
		return 1;
	}

	if (zone < 0) {
		cout << "The number of zone boundaries crossed must not be negative\n";
		return 1;
	}
	//end of checking for errors

	double fare = 1.35;
	double perZone = 0.55;
	double totalFare = 0; //initialized to 0 for future use and preventing undefined behavior

	cout.setf(ios::fixed);
	cout.precision(2); //only prints 2 digits after the decimal point

	//start of discount calculation
	if (age >= 65) {
		fare = 0.55;
		perZone = 0.25;
		if (zone == 0) {
			totalFare = 0.45;
		}
	}

	if (((age < 18 || student == "y") && zone <= 1) && totalFare != 0.45) {
		totalFare = 0.65; //under 18 or student discount for 1 or less zone crossed
	}
	//end of discount calculation

	//reason why totalFare was initialized to 0
	if (totalFare != 0) {
		cout << "The fare to " << dest << " is " << "$" << totalFare << "\n";
	}
	else {
		totalFare = fare + (perZone * zone); //calculating totalFare
		cout << "The fare to " << dest << " is " << "$" << totalFare << "\n";
	}

	return 0;
}