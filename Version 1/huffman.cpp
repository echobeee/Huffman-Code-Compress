//From bee

#include <iostream>
#include <string>
#include <fstream>

using namespace std;

const int maxNode = 1 << 10;
const int INF = 65535;
//节点结构
struct Node
{
	int weight;
	string code;
	unsigned char value;
	int lfchild, rtchild, parent;
};

struct character
{
	unsigned char value;
	int weight;
};

Node huffTree[maxNode];
int n;

//初始化
void initiateHuffTree(int num) {
    n = num;
	for(int i = 0; i < 2*n-1; i++)
	{
		huffTree[i].weight = 0;
		huffTree[i].code = "";
		huffTree[i].lfchild = -1;
		huffTree[i].rtchild = -1;
		huffTree[i].parent = -1;
	}
}

// void startHuffTree(unsigned char ch[], double weight[]) {//有num个字符，及其相应字符和频率
// 	for (int i = 0; i < n; i++){
// 		huffTree[i].value = ch[i];
// 		huffTree[i].weight = weight[i];
// 	}
// }

//构建huffman树
void huffmanTree() {
	int lnode, rnode;
	int min1, min2;
	for (int i = n; i < 2*n-1; i++){
		//每要构造一个新的结点就要初始化一次
		lnode = rnode = -1;//记录下标
		min1 = min2 = INF;

		for(int j = 0; j < i; j++){//遍历所有node
			if(huffTree[j].parent == -1){

				if(huffTree[j].weight < min1 )//小于最小
				{
					min2 = min1;
					rnode = lnode;
					lnode = j;
					min1 = huffTree[j].weight;
				}
				else if(huffTree[j].weight < min2)//小于次小大于最小
				{
					min2 = huffTree[j].weight;
					rnode = j;
				}
			}
		}

		huffTree[i].weight = huffTree[lnode].weight + huffTree[rnode].weight;
		huffTree[i].lfchild = lnode;
		huffTree[i].rtchild = rnode;
		huffTree[lnode].parent = i;
		huffTree[rnode].parent = i;
	}
}

//生成huffman编码
void huffCode() {
	int parent;
	string code;
	for(int i = 0; i < n; i++){
		code = "";
		int k = i;
		while (huffTree[k].parent != -1){
			parent = huffTree[k].parent;
			if(huffTree[parent].lfchild == k){
				code = code + "0";
			}
			else code = code + "1";
			k = parent;
		}
        for(int x = code.size()-1; x>=0; x--)
		huffTree[i].code += code[x];
	}
	//字符串的huffman编码

}

void huffDecode(string a) {
    bool flag = true;
    string output;
    string ch;
    for(int i = 0; i < a.size(); i++){
        ch = ch + a[i];
        for(int j = 0; j < n; j++){
            if(ch == huffTree[j].code)
            {
                output += huffTree[j].value;
                ch = "";
                break;
            }
            else if((i == a.size()-1)&& (j == n-1) && ch != "" ){
                cout << "error\n";
                flag = false;
                break;
            }
        }
    }
    if(flag)
    cout << "Decode: " << output << endl;
}

// void clearHuffTree() {

// }

string scanFile() {
	cout << "To Zip:";
	string filename;
	cin >> filename;
	ifstream in;
	in.open(filename.c_str(), ios::binary);//以二进制的格式打开文件
	cout << "zipping...\n";
	if(!in){
		cout << "No such file!\n\n";
		return "NULL";
	}
	unsigned char temp = '\0'; //用一个8位的无符号变量来依次读取源文件的信息
	character *ch = new character[256];//临时数组
	for(int i = 0; i < 256; i++){
        ch[i].weight = 0;
	}
	while(true)//读取文件
	{
		temp = in.get();
		if(in.eof())break;
		ch[temp].value = temp;
		ch[temp].weight++;
	}
	in.close();
	int num = 0;
	for(int i = 0; i < 256; i++){
		if(ch[i].weight != 0)
		{
			num++;
		}
	}
	initiateHuffTree(num);
	int j = 0;
	for(int i = 0; i < 256; i++){
		if(ch[i].weight != 0)//赋值给结点
		{
			huffTree[j].value = ch[i].value;
			huffTree[j].weight = ch[i].weight;
			j++;
		}
	}
	if(num == 0){
		cout << "Empty File!\n\n";
		return "NULL";
	}
	delete []ch;
	return filename;
}

void Compress() {
	string file = scanFile();
	if(file == "NULL") return;
	huffmanTree();
	huffCode();
	string filename = file + ".comp";
	ofstream out(filename.c_str(), ios::binary);
	if(!out){
		cout << "File open failed.." << endl;
		return;
	}
	out.write((char*)&n, sizeof(int));
    for(int i = 0; i < n; i++){
	 	out.write((char*)&(huffTree[i].value), sizeof(unsigned char));
	 	out.write((char*)&(huffTree[i].weight), sizeof(int));
	 }
	ifstream in(file.c_str(), ios::binary);//再读一次原文件
	if(!in)
	{
		cout << "To zip file open failed.. \n\n";
		return;
	}
	unsigned char temp = '\0';
	string code = "";
	while(true)//得到huffman编码
	{
		temp = in.get();
		if(in.eof())break;
		for(int i = 0; i < n; i++){
			if(temp == huffTree[i].value){
				code += huffTree[i].code;
				break;
			}
		}
	}
	while(code.length() >= 8)
	{
		temp = '\0';
		for(int i = 0; i < 8; i++){
			temp = temp << 1;//左移
			if(code[i] == '1')
				temp = temp | 1;
		}
		string::iterator p = code.begin();
		code.erase(p, p + 8);//清空code中的前八位编码
		out.write((char*)&temp, sizeof(unsigned char));//再将刚刚存储在unsigned char中的编码写入文件
	}
	if(code.length() > 0){//处理最后不足八位的哈夫曼编码,移位补0
		temp = '\0';
		for(int i = 0; i < code.length(); i++){
			temp = temp << 1;
			if(code[i] == '1')
				temp = temp | 1;
		}
		for(int i = 0; i < (8 - code.length()); i++)
			temp = temp << 1;//移位补0
		out.write((char*)&temp, sizeof(unsigned char));
	}
	in.close();
	out.close();
	//清空huffman树
	//clearHuffTree();
	cout << "Zip success!\n";
}


void Extract() {
	string filename;
	cout << "To Extract:";
	cin >> filename;
	ifstream in(filename.c_str(), ios::binary);
	if(!in){
		cout << "File open failed..\n";
		return;
	}
	int num = 0;
	in.read((char*)&num, sizeof(int));
	initiateHuffTree(num);
	for(int i = 0; i < n; i++){
		in.read((char*)&(huffTree[i].value), sizeof(unsigned char));
		in.read((char*)&(huffTree[i].weight), sizeof(int));
	}

	huffmanTree();

	unsigned char temp = '\0';
	unsigned long long length = 0;//原文件的长度
	for(int i = 0; i < n; i++){
		length += huffTree[i].weight;
	}
	string outfile;
	cout << "Extracted name:";
	cin >> outfile;
	ofstream out(outfile.c_str(), ios::binary);
	if(!out)
	{
		cout << "file open failed.." << endl;
		return;
	}
	int top = 2*n-2;//找到根结点
	while(length)
	{
		temp = in.get();
		//string subcode;
		for(int i = 0; i < 8; i++)
		{
			if(temp & 128)
			{//subcode[i] = 1;为1,右子结点
				top = huffTree[top].rtchild;
			}
			else //subcode[i] = 0;为0,左子结点
			{
				top = huffTree[top].lfchild;
			}
			//如果一个字符的huffman编码大于8位,则不会进入这个语句,会继续读取文件,top继续往下循环
			if(top < n)//为叶子结点
			{
				out << huffTree[top].value;//输出源字符
				length--;//字符数减少
				if(!length)break;//若此时字符数为0,则直接退出
				top = 2*n-2;//重置top,回到根结点
			}
			temp = temp << 1;
		}
	}
	in.close();
	out.close();
	cout << "Extract success!\n";
	//清除。

}


int main(int argc, char const *argv[])
{
    Compress();
    Extract();
	return 0;
}
