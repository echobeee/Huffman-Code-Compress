#include <iostream>
#include <string>

using namespace std;

class Node//huffman树中的每一个节点
{
public:
	Node(){
		weight = 0;
		code = "";
		value = '\0';
		lfchild = rtchild = parent = -1;
	}
	int weight;
	string code;
	unsigned char value;
	int lfchild, rtchild, parent;
};
