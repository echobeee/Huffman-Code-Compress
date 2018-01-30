//docx,xls,txt
//pdf,rar,jpg,mp3,exe
#include "huffman.h"
Huffman::Huffman()
{
    n_types = 0;
}

string Huffman::scanFile()
{
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
		if(in.eof())break;//读完最后一个字符就break 不读'/0'
		ch[temp].value = temp;
		ch[temp].weight++;
	}
	in.close();

	//把临时数组的值赋给huffTree
	Node temp_elem;
	for(int i = 0; i < 256; i++){
		if(ch[i].weight != 0)
		{
			n_types++;
			temp_elem.value = ch[i].value;
			temp_elem.weight = ch[i].weight;
			huffTree.push_back(temp_elem);
		}
	}

	if(n_types == 0){
		cout << "Empty File!\n\n";
		return "NULL";
	}
	delete []ch;
	return filename;
}

void Huffman::HuffmanTree()
{
	int lnode, rnode;
	int min1, min2;
	cout << "Treeing...";
	for (int i = n_types; i < 2*n_types-1; i++){
		//每要构造一个新的结点就要初始化一次
		lnode = rnode = -1;//记录下标
		min1 = min2 = INF;

		//找到树中的最小和次小
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
		Node temp_parent;
		temp_parent.weight = huffTree[lnode].weight + huffTree[rnode].weight;
		temp_parent.lfchild = lnode;
		temp_parent.rtchild = rnode;
		huffTree[lnode].parent = i;
		huffTree[rnode].parent = i;
		huffTree.push_back(temp_parent);
	}
	cout << endl;
}

void Huffman::HuffmanCode()
{
	int parent;
	string code;
	cout << "Coding...";
	for(int i = 0; i < n_types; i++){
		code = "";
		int k = i;
		//从叶到根
		while (huffTree[k].parent != -1){
			parent = huffTree[k].parent;
			if(huffTree[parent].lfchild == k){
				code = code + "0";
			}
			else code = code + "1";
			k = parent;
		}
		//反转编码
        for(int x = code.size()-1; x>=0; x--)
		huffTree[i].code += code[x];
	}
	cout << endl;
}

void Huffman::Compress()
{
	string file = scanFile();//原文件名
	if(file == "NULL") return;
	//生产huffman树和huffman编码
	HuffmanTree();
	HuffmanCode();
	string filename = file + ".comp";//输出文件名
	ofstream out(filename.c_str(), ios::binary);
	if(!out){
		cout << "File open failed.." << endl;
		return;
	}
	out.write((char*)&n_types, sizeof(int));//先写字符的类型个数
    for(int i = 0; i < n_types; i++){//再写字符及其权重
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
	while(true)//得到huffman编码 - string类型
	{//每读入一个字符就处理一个字符 大大提高了压缩速度
		temp = in.get();
		if(in.eof())break;
		for(int i = 0; i < n_types; i++){
			if(temp == huffTree[i].value){
				code += huffTree[i].code;
				break;
			}
		}
	
		while(code.length() >= 8)
		{
			temp = '\0';
			for(int i = 0; i < 8; i++){//得到huffman编码 - binary类型
				temp = temp << 1;//左移
				if(code[i] == '1')
					temp = temp | 1;
			}
			string::iterator p = code.begin();
			code.erase(p, p + 8);//清空code中的前八位编码
			out.write((char*)&temp, sizeof(unsigned char));//再将刚刚存储在unsigned char中的编码写入文件
		}	
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
	huffTree.resize(0);
	cout << "Zip success!\n";
}

void Huffman::Extract()
{
	string filename;
	cout << "To Extract:";
	cin >> filename;
	ifstream in(filename.c_str(), ios::binary);
	if(!in){
		cout << "File open failed..\n";
		return;
	}
	Node temp_elem;
	in.read((char*)&n_types, sizeof(int));
	for(int i = 0; i < n_types; i++){
		huffTree.push_back(temp_elem);
		in.read((char*)&(huffTree[i].value), sizeof(unsigned char));
		in.read((char*)&(huffTree[i].weight), sizeof(int));
	}
	//解码阶段只需要huffman树
	HuffmanTree();

	unsigned char temp = '\0';
	unsigned long long length = 0;//原文件的长度
	for(int i = 0; i < n_types; i++){
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
	int root = 2*n_types-2;//找到根结点
	while(length)//每解码一个字符，length就会-1
	{
		temp = in.get();
		for(int i = 0; i < 8; i++)
		{
			if(temp & 128)//由binary变为string
			{//subcode[i] = 1;为1,右子结点
				root = huffTree[root].rtchild;
			}
			else //subcode[i] = 0;为0,左子结点
			{
				root = huffTree[root].lfchild;
			}
			//如果一个字符的huffman编码大于8位,则不会进入这个语句,会继续读取文件,root继续往下循环
			if(root < n_types)//为叶子结点
			{
				out << huffTree[root].value;//输出源字符
				length--;//字符数减少
				if(!length)break;//若此时字符数为0,则直接退出
				root = 2*n_types-2;//重置root,回到根结点
			}
			temp = temp << 1;
		}
	}
	in.close();
	out.close();
	cout << "Extract success!\n";
	//清除。
	huffTree.resize(0);
}
