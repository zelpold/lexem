#ifndef LEXEM_H
#define LEXEM_H
#include <string>

class Lexem
{
public:
    std::string name;
    std::string type;
    int line;
    int pos;

public:
    Lexem();
};

#endif // LEXEM_H
