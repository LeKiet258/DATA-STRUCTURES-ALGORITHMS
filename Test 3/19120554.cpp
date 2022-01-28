#include<iostream>
#include<stack>
#include<string>
#include<algorithm>
#include<vector>
#include<fstream>

using namespace std;

struct Node {
	string data;
	Node* left;
	Node* right;
	int Height = 0;
};

Node* createNode(string val) {
	Node* res = new Node;

	res->data = val;
	res->left = res->right = NULL;

	return res;
}

void Insert(Node*& root, string val) {
	if (root == NULL)
		root = createNode(val);
	else if (val < root->data)
		Insert(root->left, val);
	else if (val > root->data)
		Insert(root->right, val);
}

Node* createTree(string filePath) {
	ifstream fin(filePath);
	Node* tree = NULL;

	if (fin.is_open()) {
		string name = "";

		while (!fin.eof()) {
			fin >> name;
			Insert(tree, name);
		}
	}

	return tree;
}

void removeTree(Node*& pRoot) {
	if (pRoot == NULL) return;

	//first delete both subtrees
	removeTree(pRoot->left);
	removeTree(pRoot->right);

	//then delete the node
	delete pRoot;
}

//search 1 node
Node* Search(Node* root, string val) {
	if (root == NULL) return NULL;
	if (val == root->data) return root;

	if (val < root->data)
		return Search(root->left, val);
	if (val > root->data)
		return Search(root->right, val);
}

//search a list in a BST
string SearchList(string inFile) {
	string op = "output.txt";
	Node* root = createTree("name.txt");
	ifstream fin(inFile);
	ofstream fout(op);

	if (fin.is_open()) {
		string name = ""; //name to check

		while (!fin.eof()) {
			fin >> name;
			fout << (Search(root, name) == NULL ? 0 : 1) << " ";
		}
	}

	fin.close();
	fout.close();
	removeTree(root);

	return op;
}

Node* findMin(Node* root) {
	while (root->left != NULL)
		root = root->left;
	return root;
}

//remove 1 node 
void Remove(Node*& root, string val) {
	if (root != NULL) {
		if (val > root->data)
			Remove(root->right, val);
		else if (val < root->data)
			Remove(root->left, val);
		else { //root->data = val
			//case 1: no child
			if (root->left == NULL && root->right == NULL) {
				delete root;
				root = NULL;
			}
			//case 2: 1 child
			else if (root->left == NULL) {
				Node* tmp = root;
				root = root->right;
				delete tmp;
			}
			else if (root->right == NULL) {
				Node* tmp = root;
				root = root->left;
				delete tmp;
			}
			//case 3: 2 children
			else {
				Node* min = findMin(root->right);
				root->data = min->data;
				Remove(root->right, min->data);
			}
		}
	}
}

void NLR(Node* root, ofstream& fout) {
	if (root == NULL)
		return;

	fout << root->data << endl;
	NLR(root->left, fout);
	NLR(root->right, fout);
}

//remove a list
string RemoveList(string inFile) {
	Node* root = createTree("name.txt");
	string op = "output_ex3.txt";
	ifstream fin(inFile);
	ofstream fout(op, ios::app);

	if (fin.is_open()) {
		string name = ""; //name to rm

		while (!fin.eof()) {
			fin >> name;
			Remove(root, name);
		}

		//print NLR on fout
		NLR(root, fout);
	}

	fin.close();
	fout.close();
	removeTree(root);

	return op;
}

int main(int argc, char* argv[]) {
	Node* root = createTree("name.txt");
	string ex2 = SearchList("input.txt");
	string ex3 = RemoveList("input.txt");
	
	removeTree(root);
	return 0;
}