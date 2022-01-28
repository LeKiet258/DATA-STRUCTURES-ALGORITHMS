#include<iostream>
#include<fstream>
#include<string>
#include<vector>

using namespace std;

struct Poultry
{
	string type;
	float weight;
	int eggProduction;
};

struct NODE
{
	Poultry data;
	NODE* pNext;
};

struct List
{
	NODE* pHead;
	NODE* pTail;
};

// Aid functions----------------------------------------------------------------------------
NODE* createNODE(Poultry data) {
	NODE* res = new NODE;

	if (!res)
		return NULL;

	res->data = data;
	res->pNext = NULL;

	return res;
}

//init a list
List* createList(NODE* p_NODE) {
	if (p_NODE == NULL)
		return NULL;

	List * res = new List;

	if (!res)
		return NULL;

	res->pHead = res->pTail = p_NODE;

	return res;
}

bool addHead(List * &L, Poultry data) {
	NODE* item = createNODE(data);

	if (!item)
		return false;

	if (L == NULL)
		L = createList(item);
	else {
		item->pNext = L->pHead;
		L->pHead = item;
	}

	return true;
}

bool addTail(List * &L, Poultry data) {
	NODE* item = createNODE(data);

	if (!item)
		return false;

	if (L == NULL)
		L = createList(item);
	else {
		L->pTail->pNext = item;
		L->pTail = item;
	}

	return true;
}


bool exist(vector<string> a, string type) {
	for (string i : a)
		if (i == type)
			return true;
	return false;
}

vector<string> extractAllTypes(List* L) {
	vector<string> res;
	NODE* t = L->pHead;

	while (t) {
		if (!exist(res, t->data.type))
			res.push_back(t->data.type);

		t = t->pNext;
	}

	return res;
}


void removeDuplicates(List*& L) {
	NODE* ptr1 = L->pHead, * ptr2, * dup;

	/* Pick elements one by one */
	while (ptr1 != NULL && ptr1->pNext != NULL)
	{
		ptr2 = ptr1;

		/* Compare the picked element with rest
		   of the elements */
		while (ptr2->pNext != NULL)
		{
			/* If duplicate then delete it */
			if (ptr1->data.type == ptr2->pNext->data.type && 
				ptr1->data.weight == ptr2->pNext->data.weight
				&& ptr1->data.eggProduction == ptr2->data.eggProduction)
			{
				dup = ptr2->pNext;
				ptr2->pNext = ptr2->pNext->pNext;
				delete(dup);
			}
			else
				ptr2 = ptr2->pNext;
		}
		ptr1 = ptr1->pNext;
	}
}

List* findType(List* L, string type) {
	List* res = NULL;
	NODE* t = L->pHead;

	while (t) {
		if (t->data.type == type)
			addTail(res, t->data);

		t = t->pNext;
	}

	return res;
}


int length(List* L) {
	int num = 0;
	NODE* t = L->pHead;

	while (t) {
		num++;
		t = t->pNext;
	}

	return num;
}
// Your work ----------------------------------------------------------------------------


/* Read fileand store into Linked list
- Input: fileName <string>: the name of input file
- Output : return <List*>
*/

List * readData(string fileName)
{
	ifstream fin(fileName);
	List* res = NULL;

	if (fin.is_open()) {
		string lineInfo;
		Poultry animal;
		string fNum;

		//get the first line, which is title
		getline(fin, lineInfo);

		while (!fin.eof()) {
			getline(fin, animal.type, ' ');

			getline(fin, fNum, ' ');
			animal.weight = stof(fNum);

			fin >> animal.eggProduction;

			//add to list
			if (animal.type == "chicken")
				addHead(res, animal);
			else
				addTail(res, animal);
		}
	}

	fin.close();
	return res;
}

/*
Display content of Linked list into Console
- Input: L <List*>: address of Linked list
- Output: <void>
*/
void displayList(List* L)
{
	// TODO
	NODE* tmp = L->pHead;

	while (tmp != NULL) {
		if (tmp != NULL && tmp->data.type[0] == '\n')
			tmp->data.type.erase(tmp->data.type.begin());

		cout << "Type: " << tmp->data.type << endl;
		cout << "Weight: " << tmp->data.weight << endl;
		cout << "Egg production: " << tmp->data.eggProduction<< endl;
		cout << "------------------------\n";

		tmp = tmp->pNext;
	}
}

/*
splitAndStore
- Input:
	L <List*>: address of Linked list
	outputFile <string>: name of output file
- Output: <void>
*/
void splitAndStore(List* L, string outputFile) {
	// TODO
	ofstream fout(outputFile);
	removeDuplicates(L);

	if (fout.is_open()) {
		vector<string> totalAnimals = extractAllTypes(L);
		fout << totalAnimals.size() << endl;

		for (int i = 0; i < totalAnimals.size(); i++) {
			List* newAnimal = findType(L, totalAnimals[i]);

			//write to file
			fout << totalAnimals[i] << " " << length(newAnimal) << endl;

			NODE* hold = newAnimal->pHead;

			while (hold) {
				fout << hold->data.weight << " " << hold->data.eggProduction << endl;
				hold = hold->pNext;
			}
		}
	}

	fout.close();
}

/*
Delete Linked list
- Input: L <List*>: address of Linked list
- Output: <void>
*/
void deleteList(List*& L)
{
	// TODO
	NODE* cur;

	while (L->pHead != NULL) {
		cur = L->pHead;
		L->pHead = L->pHead->pNext;
		delete cur;
	}
}

// Main function ----------------------------------------------------------------------------
int main()
{
	//NOP BAI
	List* L = readData("dataset.txt");

	cout << "The content of List: \n";
	displayList(L);

	splitAndStore(L, "output.txt");

	deleteList(L);

	return 0;
}