#include "synt.h"
#include <QDebug>
Synt::Synt()
{
    rules.insert("a:=S;",1);
    rules.insert("SorS",2);
    rules.insert("SxorS",3);
    //rules.insert("S",4);
    rules.insert("SandS",5);
    //rules.insert("S",6);
    rules.insert("notS",7);
    //rules.insert("S",8);
    rules.insert("T",9);
    rules.insert("F",10);
    rules.insert("a",11);
    rules.insert("(S)",12);
    lexem_map.insert(";",0);
    lexem_map.insert("or",1);
    lexem_map.insert("xor",2);
    lexem_map.insert("and",3);
    lexem_map.insert("not",4);
    lexem_map.insert("T",5);
    lexem_map.insert("F",6);
    lexem_map.insert("a",7);
    lexem_map.insert("(",8);
    lexem_map.insert(")",9);
    lexem_map.insert(":=",10);
    lexem_map.insert("k",11);
    lexem_map.insert("n",11);
    back_rules.insert(1,"a := S ;");
    back_rules.insert(2,"S or S");
    back_rules.insert(3,"S xor S");
    back_rules.insert(5,"S and S");
    //rules.insert("S",4);
    //rules.insert("S",6);
    back_rules.insert(7,"not S");
    //rules.insert("S",8);
    back_rules.insert(9,"T");
    back_rules.insert(10,"F");
    back_rules.insert(11,"a");
    back_rules.insert(12,"( S )");
}

void Synt::Work(QList<Lexem> lexems)
{
    stack.clear();
    operations.clear();
    stack.push("n");
    Lexem end;
    end.name = "k";
    end.type = "k";
    lexems.append(end);
    auto it =lexems.cbegin();
    while (it != lexems.cend())
    {

        QString first_terminal=stack.pop();
        QList<QString> poped_items;
        poped_items.push_front(first_terminal);
        while(first_terminal == "S")
        {
            first_terminal = stack.pop();
            poped_items.push_front(first_terminal);
        }
        QString lexem = it->name.c_str();
        if (it->type == "идентификатор") lexem = "a";
        char matrix_rule = matrix[lexem_map[first_terminal]][lexem_map[lexem]];
        if(matrix_rule == '<' || matrix_rule == '='){
            ++it;
            for(auto i:poped_items) stack.push(i);
            stack.push(lexem);
        }
        else if (matrix_rule == '>'){
            QString rule="";
            for(auto i:poped_items) rule+=i;
            poped_items.clear();

            QString second_terminal=stack.pop();

            matrix_rule = matrix[lexem_map[second_terminal]][lexem_map[first_terminal]];
            while (second_terminal == "S" || matrix_rule == '=')
            {
                rule = second_terminal+rule;
                if (second_terminal != "S") first_terminal = second_terminal;
                second_terminal=stack.pop();
                matrix_rule = matrix[lexem_map[second_terminal]][lexem_map[first_terminal]];
            }
            stack.push(second_terminal);
            stack.push("S");
            if(rules.find(rule) != rules.end())
            operations.push_front(rules[rule]);
            else{
                qDebug() << "error no such rule" << rule;
                return;
            }

        }
        else if (lexem=="k" && first_terminal=="n") break;
        else{
            qDebug() << "error no math >,=,< in matrix table for: " << first_terminal <<" " <<lexem;
            return;
        }

    }
    qDebug() << "list";
    for (auto i:operations) qDebug()<<i;
}

QList<int> Synt::getOperations()
{
    return operations;
}
