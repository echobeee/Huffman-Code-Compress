#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "Node.h"

using namespace std;

const int INF = 65535;

struct character//临时储存读入文件的字符
{
	unsigned char value;
	int weight;
};

class Huffman
{
public:
	Huffman();
	void HuffmanTree();
	void HuffmanCode();
	string scanFile();
	void Compress();
	void Extract();

private:
	vector<Node> huffTree;
	int n_types;//字符种类

};
