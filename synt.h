#ifndef SYNT_H
#define SYNT_H

#include <QVector>
#include <QString>
#include <string>
#include <QMap>
#include <QStack>
#include "lexem.h"

class Synt
{
    char matrix[12][12]
    {
        {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','>',}, //;     0
        {'>','>','>','<','<','<','<','<','<','>',' ',' ',}, //or    1
        {'>','>','>','<','<','<','<','<','<','>',' ',' ',}, //xor   2
        {'>','>','>','>','<','<','<','<','<','>',' ',' ',}, //and   3
        {'>','>','>','>','<','<','<','<','<','>',' ',' ',}, //not   4
        {'>','>','>','>',' ',' ',' ',' ',' ','>',' ',' ',}, //T     5
        {'>','>','>','>',' ',' ',' ',' ',' ','>',' ',' ',}, //F     6
        {'>','>','>','>',' ',' ',' ',' ',' ','>','=',' ',}, //a     7
        {' ','<','<','<','<','<','<','<','<','=',' ',' ',}, //(     8
        {'>','>','>','>',' ',' ',' ',' ',' ','>',' ',' ',}, //)     9
        {'=','<','<','<','<','<','<','<','<',' ',' ',' ',}, //:=   10
        {' ',' ',' ',' ',' ',' ',' ','<',' ',' ',' ',' ',}  //n    11
       //0   1   2   3   4   5   6   7   8   9   10  11
    };// ;   or  xor and not T   F   a   (   )   :=  к
    QMap<QString, int> rules;
    QMap<QString, int> lexem_map;

    QStack<QString> stack;
    QList<int> operations;

public:
    QMap<int, QString> back_rules;
    Synt();
    void Work(QList<Lexem> lexems);
    QList<int> getOperations();

};

#endif // SYNT_H
