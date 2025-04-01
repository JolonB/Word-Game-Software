#include <string>
#include <iostream>
#include <fstream>
#include <vector>

#include "trienode.h"

std::vector<TrieNode> trie;

bool addNode(int index, char* line, size_t strlen)
{
    // TODO add node if it doesn't already exist

    TrieNode* node;
    node = &trie[index];

    char firstChar = line[0];
    int nChildren = node->nChildren;
    int firstChild = node->firstChild;

    int childIndex = 0;
    for (childIndex = 0; childIndex < nChildren; childIndex++)
    {
        int nextIndex = firstChild + childIndex;
        TrieNode* nextNode = &trie[nextIndex];
        if (nextNode->letter == firstChar)
        {
            addNode(nextIndex, line+1, strlen-1);
        }
    }
}

bool parseFile(std::string filename)
{
   std::ifstream file(filename);
   if (!file)
   {
    std::cerr << "Error opening file" << std::endl;
    return false;
   }

   std::string line;
   while (std::getline(file, line))
   {
    std::cout << line << std::endl;
   }

   file.close();
   return true;
}

int main()
{
    TrieNode root;
    trie.push_back(root);
    return !parseFile("wordlist.txt");
}