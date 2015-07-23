#pragma once
#include<iostream>
#include<algorithm>
#include<string>
#include<vector>
#include<map>
#include<fstream>
#include<cassert>


/* Huffman Node. */
struct huffNode {
    char symbol;
    int weight;
    huffNode* left;
    huffNode* right;
    huffNode() {left = 0; right = 0;}
};

/* Comparator object for the heap functions to compare the weights and make a min heap */
struct huffComp {
	bool operator()(huffNode* n1, huffNode* n2) {return (n1->weight > n2->weight);}
};

/* Creates the huffman tree and returns the root of the tree. Most of the code and logic was from
 * an assignment from CSC 212 Professor Wes' class.*/
huffNode* makeHuffmanTree(std::string fileName,huffNode*& nodes) {
    std::map<char, int> freqTable;
    std::ifstream file(fileName.c_str());
    std::string line;

    /* Populates the frequency table */
    while(getline(file,line)) {
        for(int i = 0; i < line.length(); i++)
            freqTable[line[i]]++;
        /* Adds a next line char everytime getline finishes */
        freqTable['\n']++;
    }
    /* Added a null character for end of file indicator */
    freqTable['\0']++;

    /* Creates the node of the trees. */
    nodes = new huffNode[2*freqTable.size()];
    int index = 0;

    /* Fills the array with the symbols and weights of the nodes according to the frequency table */
    for (std::map<char,int>::const_iterator i = freqTable.begin(); i != freqTable.end(); ++i) {
       nodes[index].symbol = i->first;
       nodes[index].weight = i->second;
       index++;
    }

    /* Priority queue of node pointers. We used node pointers instead of nodes because nodes move around
     * due to the heap operations so it will break the left and right pointers in the node*/
    std::vector<huffNode*> pQueue;
    for(int i = 0; i < freqTable.size(); i++) {
        pQueue.push_back(nodes + i);
    }

    /* Create the comparator object */
    huffComp comp;

    /* Build the heap using the STL's make_heap function. Originally builds a max heap but with the comparator, it makes a min heap */
    std::make_heap(pQueue.begin(),pQueue.end(),comp);

    /* Pops the first two nodes off the priority queue since they are the smallest weights, add the weights and create an inner node of this weight.
     * Reinsert it into the priority queue. The STL functions helps maintain the heap property */
    while(pQueue.size() > 1) {

        /* Pops the two nodes */
        std::pop_heap(pQueue.begin(),pQueue.end(),comp);
        huffNode* v1 = pQueue.back();
        pQueue.pop_back();
        std::pop_heap(pQueue.begin(),pQueue.end(),comp);
        huffNode* v2 = pQueue.back();
        pQueue.pop_back();

        /* Adds the weights and set the weight of an inner node to that weight */
        nodes[index].weight = v1->weight + v2->weight;
        nodes[index].left = v1;
        nodes[index].right = v2;

        /* Reinserts it into the queue */
        pQueue.push_back(nodes + index);
        std::push_heap(pQueue.begin(),pQueue.end(),comp);
        index++;
    }

    /* Returns the root of the tree */
    return pQueue[0];

}

/* Traverses the tree and gets the code and stores it inside a map for encoding */
void makeEncodingMap(std::map<char,std::string>& encode, huffNode* source, std::string bitString) {

    /* Base case. When we reach a leaf nodes, it stores the code */
    if(source->left == 0 && source->right== 0) {
        encode[source->symbol] = bitString;
        return;
    }

    /* Traverses left and right childs if they exist */
    if(source->left !=0)
        makeEncodingMap(encode,source->left,bitString + "0");
    if(source->right !=0)
        makeEncodingMap(encode,source->right,bitString + "1");
}

/* Reads the file and encodes it. Code is similar to putting the file into a frequency table */
std::string encodeFile(std::string fileName, std::map<char, std::string> encodingMap) {
    std::ifstream file(fileName.c_str());
    std::string line;
    std::string encodedString = "";
    while(getline(file,line)) {
        for(int i = 0; i < line.length(); i++) {
            encodedString += encodingMap[line[i]];
        }
        encodedString += encodingMap['\n'];
    }
    encodedString += encodingMap['\0'];
    return encodedString;
}

/* Takes the string created from encodeFile and converts it into bits and writes it */
void writeFile(std::string encodedString) {
    std::ofstream file;
    file.open("encodedFile",std::ios::binary);

    /* Since operating systems deal with bytes and not bits, I packaged 8 bits into 1 byte before writing it out */
    if(file.is_open()) {

        /* A char is a 1 byte in size. Initalizing it to zero gets you one bit */
        unsigned char buff = 0;

        /* bitCount keeps track of how many bits have been packaged. */
        int bitCount = 1;

        /* Goes through the string one char at a time. */
        for(auto c : encodedString) {
            /* If the char is 1, we'll or the buffer with a 1 */
            if(c == '1') {
                buff |= 1;
            }

            /* We then bit shift it to the left one bit */
            buff <<= 1;

            /* Increment bitCount since we just added a bit to our buffer */
            bitCount++;

            /* If bitCount is 8, we have a 1 byte char and we can write it to a file */
            if(bitCount == 8) {
                file.write((char*)(&buff),1);

                /* Resets bitCount and buffer */
                bitCount = 1;
                buff = 0;
            }
        }
        /* After we're done, with the string, we still might have one more byte left that hasn't reached 8 bits yet. */
        if(bitCount > 0) {

            /* If this is the case, we bit shift it to the left 8 - bitCount times. This pads the end with zeroes and write it to file */
            buff <<= 8 - bitCount;
            file.write((char*)(&buff),1);
        }
    }
    file.close();
}

/* Writes the prefix code to a file as well as calculating the compression rate */
void printThings(std::map<char,std::string> encode) {
    std::ofstream file;
    file.open("huffman.txt");
    if(file.is_open()) {
        for(std::map<char,std::string>::const_iterator i = encode.begin(); i != encode.end(); ++i) {
            file << i->first << " " << i->second << std::endl;
        }
    }

    /* tellg() gives us the file size so we store the file size of the files we need and then calculate compression and print it */
    std::ifstream infile("huffman.txt",std::ios::binary | std::ios::ate);
    double huffman = infile.tellg();
    infile.close();

    infile.open("encodedFile",std::ios::binary | std::ios::ate);
    double encodedFile = infile.tellg();
    infile.close();

    infile.open("tale.txt",std::ios::binary | std::ios::ate);
    double taleFile = infile.tellg();
    infile.close();

    std::cout << "Compresion rate is " << 100.0 - (huffman + encodedFile) / taleFile * 100 << "%" << std::endl;
}
