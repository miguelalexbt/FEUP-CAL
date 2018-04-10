#include <conio.h>

#include "Database.h"
#include "Loader.h"

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

	if (find(accepted_args.begin(), accepted_args.end(), argv[3]) == accepted_args.end()) {
		cerr << " [ERROR]: unknown argument. " << endl;
		return 1;
	}

	int algorithm;
	if ((algorithm = chooseAlgorithm()) == -1)
		return 0;

	Database g;
	g.run(argv[1], argv[2], argv[3], algorithm);

	return 0;
}

void printHelp() {
	cout << " Usage: SOURCE DESTINATION [-arg]" << endl << " Use \"-help\" for list of arguments. " << endl;
	cout << " List of arguments (best path by): " << endl;
	cout << " Distance: " << setw(8) << "-distance" << endl;
	cout << " Less stops: " << setw(6) << "-stop" << endl;
	cout << " Time: " << setw(12) << "-time" << endl;
	cout << " Price: " << setw(11) << "-price" << endl;
	cout << " Realistic path: " << setw(2) << "-real" << endl;
}

void printAlgorithms() {
	cout << endl << " Choose one of the algorithm: " << endl;
	cout << " [1] Dijkstra's algorithm. " << endl;
	cout << " [2] Bellman-Ford's algorithm. " << endl;
	cout << " [3] Johnson's algorithm. " << endl;
	cout << " [4] A* algorithm. " << endl;
	cout << " [0] Exit. " << endl;
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