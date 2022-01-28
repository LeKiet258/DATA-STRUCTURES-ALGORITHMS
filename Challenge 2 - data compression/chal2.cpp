#include<iostream>
#include<vector>
#include<fstream>
#include <map> 
#include <string> 
#pragma warning(disable : 4996);

using namespace std;

pair<string, map<int, string>> decompress(string filePath) {
	ifstream fin(filePath, ios::binary);
	vector<int> codeVec;
	int temp = -1;

	//=========READ EACH NUMBER FROM FILE==========
	while (fin.read(reinterpret_cast<char*>(&temp), sizeof(temp))) {
		if (temp == NULL)
			break;

		codeVec.push_back(temp);
	}

	// Build the dictionary.
	int tableSz = 256;
	map<int, string> table;

	//init a table
	for (int i = 0; i < 256; i++)
		table[i] = string(1, i);

	//deal with the first item in code[]
	string s = table[codeVec[0]];
	string result = s; //use w/ entry to store info of new last position of the table
	string entry; //help store new slot in table

	//translate codeVec to text msg
	for (int i = 1; i < codeVec.size(); i++) {
		int code = codeVec[i];

		//if code exists in table
		if (table.count(code))
			entry = table[code];
		//if code does NOT exist in table
		else
			entry = s + s[0];

		result += entry;

		// Add (cur + entry[0]) to the table.
		table[tableSz++] = s + entry[0];
		s = entry;
	}

	fin.close();
	return make_pair(result, table);
}

pair<vector<int>, map<int, string>> compress(string filePath) {
	ifstream fin(filePath);
	int tableSz = 256;
	map<string, int> table;
	vector<int> v; //save encoded symbols
	string s = ""; //init sequence s
	char c;

	//init a table
	for (int i = 0; i < 256; i++) {
		//string(a,char) -> init /char/ /a/ times 
		table[string(1, i)] = i;
	}

	//read each charater
	while (fin.get(c)) {
		char nextByte = c;

		//if (s + nextByte) is in table
		if (table.count(s + nextByte) != 0)
			s += nextByte;
		//if (s + nextByte) is NOT in table
		else {
			v.push_back(table.find(s)->second);

			//Add (s + nextByte) to the table.
			table[s + nextByte] = tableSz++;
			s = nextByte;
		}
	}

	// Output the code for s.
	if (!s.empty())
		v.push_back(table.find(s)->second);

	//flip 2 types of the current table to the unique form with decoding function
	map<int, string> flipTable;

	for (auto i = table.begin(); i != table.end(); ++i)
		flipTable[i->second] = i->first;

	fin.close();
	return make_pair(v, flipTable);
}

int main(int argc, char* argv[]) {
	map<int, string> table;
	float input_size = 0;
	float output_size = 0;

	if (argc >= 4) {
		// Get infomation from cmd
		string action = argv[1];
		string opInfo = (argc == 5 ? argv[4] : "");

#pragma region Action
		//encode phase
		if (action == "-e") {
			ofstream fout(argv[3], ios::binary);
			pair<vector<int>, map<int, string>> afterCompress = compress(argv[2]);

			//take the code after compressing
			vector<int> code = afterCompress.first;
			for (auto each : code)
				fout.write(reinterpret_cast<char*>(&each), sizeof(each));

			//update table
			table = afterCompress.second;

			fout.close();

			//read file txt to get size
			fstream inFile(argv[2], ios::in);

			inFile.seekg(0L, ios::end);
			input_size = inFile.tellg() * 8;
			inFile.close();

			//read file lzw to get size
			fstream outFile(argv[3], ios::in | ios::binary);
			vector<int> codeVec;
			int temp;

			while (outFile.read(reinterpret_cast<char*>(&temp), sizeof(temp))) {
				if (temp == NULL)
					break;

				codeVec.push_back(temp);
			}

			for (int i = 0; i < codeVec.size(); i++) {
				if (codeVec.at(i) >= 0 && codeVec.at(i) < 256)
					output_size += 8;
				else if (codeVec.at(i) >= 256 && codeVec.at(i) < 512)
					output_size += 9;
				else if (codeVec.at(i) >= 512 && codeVec.at(i) < 1024)
					output_size += 10;
				else if (codeVec.at(i) >= 1024 && codeVec.at(i) < 2048)
					output_size += 11;
				else if (codeVec.at(i) >= 2048 && codeVec.at(i) < 4096)
					output_size += 12;
			}

			outFile.close();
		}

		//decode phase
		else if (action == "-d") {
			ofstream fout(argv[3]);
			pair<string, map<int, string>> afterDecompress = decompress(argv[2]);

			//write to file
			fout << afterDecompress.first;
			//update table
			table = afterDecompress.second;

			fout.close();
		}
#pragma endregion

#pragma region OutputInfo
		//OutputInfo
		if (opInfo == "-d") {
			auto i = table.begin();

			for (advance(i, 256); i != table.end(); ++i) {
				cout << i->second << ": " << i->first << endl;
			}
		}
		else if (opInfo == "-i") {
			float saved = (input_size - output_size) / input_size;

			cout << "Input size :" << input_size << " bits\n";
			cout << "Output size:" << output_size << " bits\n";

			cout << "Saved:" << saved * 100 << " %\n";
		}
		else if (opInfo == "-ind") {
			auto i = table.begin();

			for (advance(i, 256); i != table.end(); ++i) {
				cout << i->second << ": " << i->first << endl;
			}

			float saved = (input_size - output_size) / input_size;

			cout << "Input size :" << input_size << " bits\n";
			cout << "Output size:" << output_size << " bits\n";

			cout << "Saved:" << saved * 100 << " %\n";
		}
#pragma endregion

	}
	else
		cout << " Lack of commands !\n";


	return 0;
}