#include <conio.h>

#include "Grid.h"
#include "Loader.h"

void printHelp();
void printAlgorithms(); 
int chooseAlgorithm();

int main(int argc, char *argv[]) {

	vector<string> accepted_args{ "-d", "-s", "-t", "-p", "-r" };

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

	int algorithm = chooseAlgorithm();

	Grid g;
	g.run(argv[1], argv[2], argv[3], algorithm);

	return 0;
}

void printHelp() {
	cout << " Usage: SOURCE DESTINATION [-arg]" << endl << " Use \"-help\" for list of arguments. " << endl;
	cout << " List of arguments (best path by): " << endl;
	cout << " Distance: " << setw(8) << "-d" << endl;
	cout << " Less stops: " << setw(6) << "-s" << endl;
	cout << " Time: " << setw(12) << "-t" << endl;
	cout << " Price: " << setw(11) << "-p" << endl;
	cout << " Realistic path: " << setw(2) << "-r" << endl;
}

void printAlgorithms() {
	cout << endl << " Choose one of the algorithm: " << endl;
	cout << " [1] Dijkstra's algorithm. " << endl;
	cout << " [2] Bellman-Ford's algorithm. " << endl;
	cout << " [3] Johnson's algorithm. " << endl;
	cout << " [4] A* algorithm. " << endl;
}

int chooseAlgorithm() {

	printAlgorithms();

	int choice;
	cin >> choice;

	while (cin.fail() || choice < 0 || choice > 5) {
			
		cin.clear();
		cin.ignore();

		cout << " Input not valid. Please re-enter: ";
		cin >> choice;
	}

	cout << endl;

	return choice - 1;
}