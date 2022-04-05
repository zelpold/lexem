#include "statemachine.h"


#include <QDebug>
StateMachine::StateMachine() {


    for (int i = 0; i < 73; i++)
    {
        mapTable[static_cast<int>(alphabet[i])] = groups[i];
    }
}

void StateMachine::work(QString text) {

    std::string buffer;
    std::string std_text = text.toStdString()+" ";
    qDebug() << std_text.c_str();
    int state = 2;
    int line = 0;
    int pos = 0;
    int count = 0;
    for(auto s:std_text) {
        if (buffer == "\n" || buffer == " " || buffer == "\0") buffer="";
        int new_state = getNewState(state, s);

        if (new_state == 0 && state == 3 ){

            emit errorSignal("Ошибка: незакрытый коментарий!");
            return ;
        }
        qDebug() << s << count << std_text.length();
        if (new_state == 0 && state == 4 && count == std_text.length()-1) {
            emit errorSignal("не законченный оператор присваивания!");
            return ;
        }
        if (new_state == 0)
        {
            emit errorSignal("Неожиданный симол '"+QString(s)+"' в позиции: "
                                                            +QString::number(pos) + " на строке: "
                                                            +QString::number(line));
            return;
        }
        if ((state == 5 || state == 7 || state == 9  || state == 10
                || state == 12 || state == 13 || state == 15 || state == 16)
                && (new_state == 3 ||  new_state == 1 || new_state == 4))
        {
            addLexem(buffer,line,pos-buffer.length(), "идентификатор");

            buffer="";
        }
        if ((state == 8 || state == 11 || state == 14 || state == 17)
                && (new_state == 3 ||  new_state == 1 || new_state == 4))
        {
            addLexem(buffer,line,pos-buffer.length(), "ключевое слово");
            buffer="";
        }
        if (state == 6 && (new_state == 3 ||  new_state == 1 || new_state == 4) )
        {
            addLexem(buffer,line,pos-buffer.length(),"константа");
            buffer="";
        }
        buffer += s;

        if (state == 4 && new_state == 1){
            addLexem(":=",line,pos-1,"знак присвоения");
            buffer = "";
        }
        if (mapTable[static_cast<int>(s)] == 14 && new_state == 1)
        {
            addLexem(buffer,line,pos,"Разделяющий символ");
            buffer="";
        }
        if (state==3) buffer="";
        state = new_state;

        if (state == 1) state = 2;
        pos++;
        if (s =='\n')
        {
            line++;
            pos = 0;
        }
        count++;
    }
    if (state == 3) emit errorSignal("Ошибка: незакрытый коментарий!");
    if (state == 4) emit errorSignal("не законченный оператор присваивания!");

}



int StateMachine::getNewState(int state, char s) {
    int n_group = mapTable[static_cast<int>(s)];
    int new_state = transition_table[state][n_group];
    return new_state;
}

void StateMachine::addLexem(std::string buffer,int line, int pos, std::string type)
{
    Lexem lexem;
    lexem.name = buffer;
    lexem.type = type;
    lexem.line = line;
    lexem.pos = pos;
    lexems.append(lexem);
}

QList<Lexem> StateMachine::getLexemList()
{
    return lexems;
}


