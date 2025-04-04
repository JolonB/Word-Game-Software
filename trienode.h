#include <vector>

#define START_CHAR ('a' - 1);

struct TrieNode
{
    char letter = START_CHAR;
    std::vector<TrieNode*> children;
    uint8_t isWord = 0;  // bool
};

struct StaticTrieNode
{
    uint32_t letterID: 6;
    uint32_t nChildren: 5;
    uint32_t firstChild: 20;
    uint32_t isWord: 1;  // bool
};

/*
letterID = [1.2:1.7]
nChildren = [2.5:2.7 + 1.0:1.1]
firstChild = [4.1:4.7 + 3 + 2.0:2:4]
isWord = [4.0]
*/

// TODO Remember endianness matters here, so I may need to change how it is written to disk