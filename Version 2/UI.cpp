#include "huffman.h"
#include <iostream>

using namespace std;

int main(int argc, char const *argv[])
{
    while(true){
	Huffman test;
	test.Compress();
	test.Extract();
    }
	return 0;
}
