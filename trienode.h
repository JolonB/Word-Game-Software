
struct TrieNode
{
    char letter ='\0';
    uint8_t nChildren = 0;
    uint32_t firstChild = 1;
    uint8_t isWord = 0;  // bool
};