// Project4 - Huffman Code.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <queue>
#include <fstream>
#include <vector>
#include <algorithm>
#include <map>
#include <unordered_map>
using namespace std;

vector<pair<char, int>> makeList() {

	ifstream text;
	text.open("text.txt");

	map<char, int> L;
	char x;

	while (text >> x) {
		if (isalpha(x)) {
			x = tolower(x);
			if (L.find(x) == L.end())
				L.insert(std::make_pair(x, 1));
			else
				L[x]++;
		}
	}

	vector<pair<char, int>> V;

	for (const auto& pair : L) {
		V.push_back(pair);
	}
	return V;
}

class node {
public:
	node* left;
	node* right;
	char key;
	int total;

	node(int x, int y, node* l, node* r) {
		total = x + y;
		key = '#';
		left = l;
		right = r;
	}

	node(char k, int t) {
		key = k;
		total = t;
	}

	node* getLeft() {
		return left;
	}

	node* getRight() {
		return right;
	}

	const int getTotal() {
		return total;
	}

	const char getChar() {
		return key;
	}
};

struct CompareNodePtr {
	bool operator()(node* a, node* b) const {
		return a->getTotal() > b->getTotal(); // Min-heap order (lowest total frequency first)
	}
};

node* huffman(vector<pair<char, int>>& ordered) {
	priority_queue<node*, vector<node*>, CompareNodePtr> pq;

	for (auto& it : ordered) {
		node* n = new node(it.first, it.second);
		pq.push(n);
	}

	while (pq.size() > 1) {
		node* least = pq.top(); pq.pop();
		node* secLeast = pq.top(); pq.pop();

		node* dummy = new node(least->getTotal(), secLeast->getTotal(), least, secLeast);
		pq.push(dummy);
	}

	return pq.top();
}

void postOrder(node* root)
{
	if (root)
	{
		postOrder(root->left);
		postOrder(root->right);
		cout << "(" << root->key << ":" << root->total << ")";
	}
}

void generateHuffmanCodes(node* root, string code, unordered_map<char, string>& huffmanCodes) {
	if (root == nullptr)
		return;

	if (!root->getLeft() && !root->getRight()) {
		huffmanCodes[root->getChar()] = code;
	}

	generateHuffmanCodes(root->getLeft(), code + "0", huffmanCodes);

	generateHuffmanCodes(root->getRight(), code + "1", huffmanCodes);
}

void encode(unordered_map<char, string> codes) {
	ifstream input;
	input.open("text.txt");

	ofstream encode;
	encode.open("encoded.txt");

	char x;

	while (input >> x) {
		if (x == 32) {
			encode << " . ";
		}
		
		else if ((x > 64 && x < 91) || (x > 96 && x < 123)) {
			x = tolower(x);
			if (codes.find(x) == codes.end()) {
				// Handle missing code
			}
			else {
				encode << codes[x] << " ";
			}
		}
	}

	input.close();
	encode.close();
}

void decode(node* root) {
	ifstream input("encoded.txt");
	ofstream decode("decoded.txt");

	node* current = root;

	char bit;
	while (input >> bit) {
		if (bit == '.') { // Space character in encoded text
			decode << ' ';
			continue;
		}

		if (bit == '0') {
			current = current->left;
		}
		else if (bit == '1') {
			current = current->right;
		}

		if (current->left == nullptr && current->right == nullptr) {
			decode << current->key << " ";
			current = root; // Reset to the root for next character
		}
	}

	input.close();
	decode.close();
}

int main()
{
	vector<pair<char, int>> V = makeList();

	node* root = huffman(V);

	cout << "Huffman Tree Structure:" << endl;
	
	unordered_map<char, string> huffmanCodes;

	generateHuffmanCodes(root, "", huffmanCodes);

	cout << "Huffman Codes:" << endl;
	for (const auto& pair : huffmanCodes) {
		cout << pair.first << ": " << pair.second << endl;
	}

	encode(huffmanCodes);

	decode(root);

	return 0;
}
