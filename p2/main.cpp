#include<iostream>
#include "huffman.hpp"

int main()
{
    huffNode* nodes;
    huffNode* root = makeHuffmanTree("tale.txt",nodes);
    std::map<char, std::string> encode;
    makeEncodingMap(encode,root,"");
    writeFile(encodeFile("tale.txt",encode));
    printThings(encode);
    delete[] nodes;
    return 0;
}
