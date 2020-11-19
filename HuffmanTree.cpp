#include "HuffmanTree.hpp"
#include "HuffmanBase.hpp"
#include "HeapQueue.hpp"
#include "TestStrings.hpp"
#include <string>
#include <map>
#include <stack>
#include <iostream>
#include <algorithm>

using std::string;

string HuffmanTree::compress(string inputStr)
{

    HeapQueue<HuffmanNode* , HuffmanNode::Compare> hq;
    string s = "";
    std::map<char, string> m;
    std::map<char, int> frequency;

    //Scan input string adding each character with its frequency to map
    for(string::iterator i = inputStr.begin(); i != inputStr.end(); i++)
    {
        
        frequency[*i]++; 
    } 

    //Create a new node for each pair from the map and store in a priority queue, sorting by frequency, then lexicographically
    for(std::map<char, int>::iterator it = frequency.begin(); it != frequency.end(); it++)
    {
        HuffmanNode *h = new HuffmanNode(it -> first, it -> second);
        hq.insert(h);
    }

    //Until there is one item left in the priority queue...
    while(hq.size() != 1)
    {
        int freq_total = 0;

        std::stack<HuffmanNode*> s;

        freq_total += hq.min() -> getFrequency(); //Add frequency of node to frequency total
        s.push(hq.min()); //Push node onto stack
        hq.removeMin(); //Remove node from priority queue

        freq_total += hq.min() -> getFrequency(); //Add frequenct of node to frequency total
        s.push(hq.min()); //Push node onto stack
        hq.removeMin(); //Remove node from priority queue

        //Create a null parent node and assign node on top of stack to its right child
        HuffmanNode *parent = new HuffmanNode('\0', freq_total, nullptr, nullptr, s.top());
        
        s.pop(); //Pop node from top of stack
        parent -> left = s.top(); //Assign node on top of stack to left child of parent

        hq.insert(parent); //Push parent node back into priority queue



    }

    
    HuffmanNode *root = new HuffmanNode('\0', 0, nullptr, nullptr, nullptr); //Create root node
    root = hq.min(); //Assign final node on stack to the root of the tree

    tree = root; //Save tree

    getPrefixes(root, s, m); //Call getPrefixes to map each character to a prefix code

    //Scan input string and convert each character to prefix code
    for(string::iterator i = inputStr.begin(); i != inputStr.end(); i++)
    {
        s += m[*i];
    }

    return s;
}

string HuffmanTree::serializeTree() const
{

    string s = "";
    
    serializeString(tree, s); //Call recursive serializeString function to create a serialized string from the Huffman Tree

    removeNodes(tree); //Destroy all nodes of Huffman tree

    return s;
}

string HuffmanTree::decompress(const std::string inputCode, const std::string serializedTree)
{

    string decomp_str = "";
    std::stack<HuffmanNode*> s;

    
    for(int i = 0; i < serializedTree.length(); i++) //Iterate through each character in the string
    {

        if((serializedTree[i]) == 'L') //If character is leaf indicator...
        {
            i++; //Move to next character
            HuffmanNode *child = new HuffmanNode(serializedTree[i], 0); //Create new child node

            s.push(child); //Add node to the stack
        }
        else if(serializedTree[i] == 'B') //If character is a branch indicator...
        {
            HuffmanNode* right = s.top(); //Save top of stack into right node
            s.pop(); //Remove node from stack
            HuffmanNode* left = s.top(); //Copy top of stack into left node
            s.pop(); //Remove node from front of stack

            HuffmanNode *parent = new HuffmanNode('\0', 0, nullptr, left, right); //Create new parent node, assiging left right nodes as children

            s.push(parent); //Add parent node to stack
        }
    }

    HuffmanNode *root = new HuffmanNode('\0', 0, nullptr, nullptr, nullptr); //Create root node
    root = s.top(); //Assign top of stack to root node

    tree = root; //Save new Huffman Tree

    //Scan each character in encoded string
    for(int i = 0; i < inputCode.size(); i++)
    {   
        if(inputCode[i] == '0') //If character is 0...
        {
            root = root -> left; //Move left
        }
        if(inputCode[i] == '1') //If character is 1...
        {
            root = root -> right; //Move right
        }

        if(!root -> left && !root -> right) //If node is a leaf node...
        {
            decomp_str += root ->getCharacter(); //Add character to decoded string
            root = tree; //Move back to root of tree
        }

    }

    removeNodes(root); //Destroy Huffman Tree

    return decomp_str; //Return decompressed string
}

void HuffmanTree::getPrefixes(HuffmanNode* root, string s, std::map<char, string> &huffmanCode)
{
    if(root == nullptr)
    {
        return;
    }


    if(!root -> left && !root -> right) //If leaf node...
    {
        huffmanCode[root -> getCharacter()] = s; //Add character and prefix code to map
    }

    if(root -> left) {getPrefixes(root -> left, s + "0", huffmanCode);} //Traverse left tree adding 0 to the encoded string when a left node is visited
    if(root -> right) {getPrefixes(root -> right, s + "1", huffmanCode);} //Traverse right tree adding 1 to the encoded string when a right node is visited

    return;
}

void HuffmanTree::removeNodes(HuffmanNode* root) const
{
    if(root == nullptr) {return;}

    removeNodes(root -> left); //Traverse left subtree
    removeNodes(root -> right); //Traverse right subtree

    delete root; //Destroy node

}

void HuffmanTree::serializeString(HuffmanNode* root, string &s) const
{
    if(root == nullptr) {return;}

        
    serializeString(root -> left, s); //Traverse left subtree

    serializeString(root -> right, s); //Traverse right subtree

    if(!root -> left && !root -> right) //If node is a leaf node
    {
        s += "L"; //Add L to serialized string
        s += root ->getCharacter(); //Add character to serialized string
    }

    if(root ->left) //If node is a branch...
    {s += "B";} //Add B to serialized string
    return;
}