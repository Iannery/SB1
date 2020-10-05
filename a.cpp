#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

int main(int argc, char* argv[]) {
    string a = "+1";
    int b = 100;

    b = stoi(a);
    cout << b << endl;
    b+= 5;
    cout << b << endl;
	return 0;
}