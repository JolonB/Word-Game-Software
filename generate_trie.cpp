#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <queue>

#include "trienode.h"

// std::vector<TrieNode> trie;

TrieNode* findChild(const TrieNode * const node, char childLetter)
{
    for (TrieNode* child : node->children)
    {
        if (child->letter == childLetter)
        {
            return child;
        }
    }
    return nullptr;
}

TrieNode* createChild(TrieNode* parent, char letter, bool isWord)
{
    TrieNode* node = new TrieNode();
    node->letter = letter;
    node->isWord = isWord;
    parent->children.push_back(node);
    return node;
}

bool addNode(TrieNode* node, const char* line, size_t strlen)
{
    char firstChar = line[0];
    // If at the last character, create a node
    if (strlen == 1)
    {
        createChild(node, firstChar, true);
        return true;
    }

    TrieNode* nextChild = findChild(node, firstChar);
    // Create child if it doesn't exist
    if (nextChild == nullptr)
    {
        nextChild = createChild(node, firstChar, false);
    }
    addNode(nextChild, line+1, strlen-1);

    return true;
}

void trimString(std::string& str)
{
    // Remove \n and \r from string
    size_t strLen = str.size();
    if (!str.empty() && str[strLen - 1] == '\n')
        str.erase(strLen - 1);
    if (!str.empty() && str[strLen - 1] == '\r')
        str.erase(strLen - 1);
}

bool parseFile(std::string filename, TrieNode* const root)
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
    trimString(line);
    // std::cout << line << std::endl;
    addNode(root, line.c_str(), line.length());
   }

   file.close();
   return true;
}

void printTree(const TrieNode* const root, int level=0)
{
    for (int i = 0; i < level; i++) std::cout << " ";
    std::cout << root->letter << " " << (root->isWord ? "-" : "") << level << std::endl;
    for (const TrieNode* const node : root->children)
    {
        printTree(node, level+1);
    }
}

bool isWordInTree(const TrieNode* const root, const char* word, int strlen)
{
    const TrieNode* const child = findChild(root, word[0]);
    if (child == nullptr) {
        return false;
    }
    if (strlen == 1 && child->isWord)
    {
        return true;
    }
    return isWordInTree(child, word+1, strlen-1);
}

bool isWordInTree(const TrieNode* const root, std::string word)
{
    return isWordInTree(root, word.c_str(), word.length());
}

void writeNode(std::ofstream& file, const TrieNode* const node, int childOffset)
{
    int nChildren = node->children.size();
    // Convert TrieNode to StaticTrieNode
    StaticTrieNode stnode;
    stnode.letterID = node->letter - START_CHAR;
    stnode.nChildren = nChildren;
    stnode.firstChild = (nChildren > 0) ? childOffset : 0;
    stnode.isWord = node->isWord;
    file.write(reinterpret_cast<char*>(&stnode), sizeof(StaticTrieNode));
}

void writeBreadthFirst(std::ofstream& file, const TrieNode* const root)
{
    std::queue<const TrieNode*> inputNodes;
    inputNodes.push(root);

    int maxLen = 0;
    while (!inputNodes.empty())
    {
        // Write first node in queue
        const TrieNode* node = inputNodes.front();
        writeNode(file, node, inputNodes.size());
        if (inputNodes.size() > maxLen) maxLen = inputNodes.size();
        // Remove from queue now that it's been processed
        inputNodes.pop();

        // Add children to queue
        for (const TrieNode* const child : node->children)
        {
            inputNodes.push(child);
        }
    }
    std::cout << "Max len: " << maxLen << std::endl;
}

void writeTrieToDisk(std::string filename, const TrieNode* const root)
{
    std::ofstream file(filename, std::ios::binary);
    writeBreadthFirst(file, root);

    file.close();
}

bool checkWord(std::ifstream& trie, std::string word)
{
    // Return to start of trie
    trie.seek(0);
    // Loop over each child until we find the one we want
    // TODO
    return false;
}

#define ISWORD(x) std::cout << (x) << " " << isWordInTree(&root, (x)) << std::endl
#define ISWORDDISK(x) std::cout << (x) << " " << checkWord(trie, (x)) << std::endl

int main()
{
    TrieNode root;
    // trie.push_back(root);
    bool parseSuccess = parseFile("twl.txt", &root);
    // printTree(&root);
    ISWORD("hello");
    ISWORD("cat");
    ISWORD("car");
    ISWORD("cart");
    ISWORD("bedding");
    ISWORD("jolon");
    ISWORD("bet");
    ISWORD("bets");
    ISWORD("aa");
    ISWORD("a");
    ISWORD("i");
    ISWORD("aaron");
    writeTrieToDisk("trie.bin", &root);

    std::ifstream trie("trie.bin", std::ios::binary);
    ISWORDDISK("hello");
    return !parseSuccess;
}