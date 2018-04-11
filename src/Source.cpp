#include <conio.h>

#include "Database.h"

using namespace std;

void printHelp();
void printAlgorithms(); 
int chooseAlgorithm();

int main(int argc, char *argv[]) {

	vector<string> accepted_args{ "-distance", "-stop", "-time", "-price", "-real" };

	vector<string> args;
	for (int i = 0; i < argc; i++)
		args.push_back(string(argv[i]));

	if (argc != 4) {
		printHelp();
		return 1;
	}

	if (find(accepted_args.begin(), accepted_args.end(), args[3]) == accepted_args.end()) {
		cerr << " [ERROR]: unknown argument. " << endl;
		return 1;
	}

	int algorithm;
	if ((algorithm = chooseAlgorithm()) == -1)
		return 0;

	Database g;
	g.run(args[1], args[2], args[3], algorithm);

	return 0;
}

void printHelp() {
	cout << endl << " Usage: [SOURCE] [DESTINATION] [-ARGS]" << endl;
	cout << " List of arguments (best path by): " << endl;
	cout << " Distance: " << setw(15) << "-distance" << endl;
	cout << " Less stops: " << setw(9) << "-stop" << endl;
	cout << " Time: " << setw(15) << "-time" << endl;
	cout << " Price: " << setw(15) << "-price" << endl;
	cout << " Realistic path: " << setw(2) << "-real" << endl;
}

void printAlgorithms() {

	cout << endl << " Choose one of the algorithm: " << endl;
	cout << " [1] Dijkstra's algorithm. " << endl;
	cout << " [2] Bellman-Ford's algorithm. " << endl;
	cout << " [3] Johnson's algorithm. " << endl;
	cout << " [4] A* algorithm. " << endl;
	cout << " [0] Exit. " << endl;
	cout << " Input : ";
}

int chooseAlgorithm() {

	printAlgorithms();

	int choice;
	cin >> choice;

	while (cin.fail() || choice < 0 || choice > 4) {
			
		cin.clear();
		cin.ignore();

		cout << " Input not valid. Please re-enter: ";
		cin >> choice;
	}

	cout << endl;

	return choice - 1;
}