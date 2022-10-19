#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <utility>

enum Commands {
    COMMAND_APPEND = 1,
    COMMAND_NEWLINE = 2,
    COMMAND_SAVE = 3,
    COMMAND_LOAD = 4,
    COMMAND_PRINT = 5,
    COMMAND_INSERT = 6,
    COMMAND_SEARCH = 7,
    COMMAND_DELETE = 8,
    COMMAND_COPY = 9,
    COMMAND_PASTE = 10,
    COMMAND_CUT = 11,
    COMMAND_UNDO = 12,
    COMMAND_REDO = 13,
    COMMAND_CLOSE = 14,
    COMMAND_FREE_LINE = 15
};
class CharNode {
public:
    char data;
    CharNode *next;
    void append(char chr = 0){
        if (next == nullptr){
            next = new CharNode;
            next->data = chr;
        } else {
            CharNode temp;
            temp = *next;
            next = new CharNode;
            next->data = chr;
            next->next = &temp;
        }
    }
};

class RowNode {
public:
    CharNode *head;
    RowNode *next;
    int rowN;

    void append() {
        if (next == nullptr) {
            next = new RowNode;
        } else {
            RowNode temp;
            temp = *next;
            next = new RowNode;
            next->next = &temp;
        }
    }
    void saveFile(std::string filename){
        filename = filename + ".txt";
        //std::ofstream MyFile(filename);
        std::fstream MyFile;
        RowNode* currentRowNode = this;
        MyFile.open (filename);
        if (MyFile.is_open())std::cout << "error opening" << std::endl;
        while(true){
            std::string currentRow;
            CharNode* currentCharNode = currentRowNode->head;
            while(true){
                currentRow += currentCharNode->data;
                if (currentCharNode->next == nullptr) break;
                else{
                    currentCharNode = currentCharNode->next;
                }
            }
            currentRow = currentRow.substr(1);
            MyFile << currentRow << std::endl;
            if (currentRowNode->next == nullptr) break;
            else{
                currentRowNode = currentRowNode->next;
            }
        }
        MyFile.close();
    }//3
    void recursiveFree(RowNode* currentRow, CharNode* currentChar, CharNode* lastNode = nullptr){
        RowNode* nextRow;
        CharNode* nextChar;
        if (lastNode == currentChar){
            return;
        }
        if(currentChar->next != nullptr){
            nextChar = currentChar->next;
            free(currentChar);
            nextRow = currentRow;
        } else {
            if (currentRow->next != nullptr){
                nextRow = currentRow->next;
                free(currentRow);
                free(currentChar);
                if (nextRow->head == nullptr){
                    free(nextRow);
                    return;
                }
                nextChar = nextRow->head;
            } else  {
                free(currentChar);
                free(currentRow);
                return;
            }
        }
        recursiveFree(nextRow, nextChar, lastNode);
    }

    void recursivePrint(RowNode *currentRow, CharNode *currentChar) {
        RowNode *nextRow;
        CharNode *nextChar;
        if (currentChar->data != 0) std::cout << currentChar->data;
        if (currentChar->next != nullptr) {
            nextChar = currentChar->next;
            nextRow = currentRow;
        } else if (currentRow->next != nullptr) {
            std::cout << "" << std::endl;
            nextRow = currentRow->next;
            nextChar = currentRow->next->head;
        } else {
            std::cout << "" << std::endl;
            return;
        }
        recursivePrint(nextRow, nextChar);
    };

    void insert(int row, int character, const std::string& input) {
        RowNode *currentRow = this;
        for (int i = 0; i < row; i++) {
            if (currentRow->next != nullptr) currentRow = currentRow->next;
        }
        CharNode *currentChar = currentRow->head;
        for (int i = 0; i < character; i++) {
            if (currentChar->next != nullptr) currentChar = currentChar->next;
        }
        CharNode *nextChar = currentChar->next;

        for (char i: input) {
            currentChar->next = new CharNode;
            currentChar = currentChar->next;
            currentChar->data = i;
        }
        currentChar->next = nextChar;
    }
    void deleteText(int row, int character, int nOf) {
        RowNode *currentRow = this;
        for (int i = 0; i < row; i++) {
            if (currentRow->next != nullptr) currentRow = currentRow->next;
        }
        CharNode *currentChar = currentRow->head;
        for (int i = 0; i < character; i++) {
            if (currentChar->next != nullptr) currentChar = currentChar->next;
        }
        CharNode *nextChar = currentChar->next;
        for (int i = 0; i < nOf; i++) {
            if (nextChar->next != nullptr) nextChar = nextChar->next;
            else {
                recursiveFree(currentRow, currentChar->next, nextChar);
                currentChar->next = nullptr;
                return;
            }
        }
        recursiveFree(currentRow, currentChar->next, nextChar);
        currentChar->next = nextChar;
    }
    void copyText(int row, int character, int nOf, std::string &cache) {
        RowNode *currentRow = this;
        for (int i = 0; i < row; i++) {
            if (currentRow->next != nullptr) currentRow = currentRow->next;
        }
        CharNode *currentChar = currentRow->head;
        for (int i = 0; i < character; i++) {
            if (currentChar->next != nullptr) currentChar = currentChar->next;
        }
        CharNode *nextChar = currentChar->next;

        for (int i = 0; i < nOf; i++) {
            if (nextChar->data != 0) cache += nextChar->data;
            if (nextChar->next != nullptr) nextChar = nextChar->next;
        }
    }
    bool localRecursiveSearch(int n, CharNode *currChar, std::string currString) {
        CharNode *nextChar;
        if (currChar->next != nullptr) nextChar = currChar->next; else return false;
        n++;
        if (nextChar->data == currString[n - 1]){
            if (n == currString.length()) return true; else {
                return localRecursiveSearch(n, nextChar, currString);
            }
        } else return false;
    }
    void recursiveSearch(std::string str, RowNode* currRow, CharNode* currChar, int nRow, int nStr){
        RowNode* nextRow;
        CharNode* nextChar;
        if (currChar->data == str[0]){
            if (localRecursiveSearch(1, currChar, str)){
                std::cout << nRow << " " << nStr - 1 << std::endl;
            }
        }
        if(currChar->next != nullptr){
            nextChar = currChar->next;
            nStr++;
            nextRow = currRow;
        } else if (currRow->next != nullptr){
            nextRow = currRow->next;
            nRow++;
            nStr = 1;
            nextChar = nextRow->head;
        } else {
            return;
        }
        recursiveSearch(str, nextRow, nextChar, nRow, nStr);
    }//7

};

class LastCommands{
public:
    Commands firstCommand;
    std::string firstCommandParameter1;
    std::string firstCommandParameter2;
    std::string firstCommandParameter3;
    Commands secondCommand;
    std::string secondCommandParameter1;
    std::string secondCommandParameter2;
    std::string secondCommandParameter3;
    Commands thirdCommand;
    std::string thirdCommandParameter1;
    std::string thirdCommandParameter2;
    std::string thirdCommandParameter3;
    std::string currentCommand;

    void ClearCommands(){
        free(this);
    }
    void AddCommand(Commands CommandName,std::string CommandParameter1 = "",std::string CommandParameter2 = "",
                    std::string CommandParameter3 = ""){
        if (currentCommand.empty()){
            firstCommand = CommandName;
            firstCommandParameter1 = std::move(CommandParameter1);
            firstCommandParameter2 = std::move(CommandParameter2);
            firstCommandParameter3 = std::move(CommandParameter3);
            currentCommand = "1";
        }
        else if (currentCommand == "1"){
            secondCommand = CommandName;
            secondCommandParameter1 = std::move(CommandParameter1);
            secondCommandParameter2 = std::move(CommandParameter2);
            secondCommandParameter3 = std::move(CommandParameter3);
            currentCommand = "2";
        } else if (currentCommand == "2"){
            thirdCommand = CommandName;
            thirdCommandParameter1 = std::move(CommandParameter1);
            thirdCommandParameter2 = std::move(CommandParameter2);
            thirdCommandParameter3 = std::move(CommandParameter3);
            currentCommand = "3";
        } else if (currentCommand == "3"){
            shiftCommands(CommandName,CommandParameter1,CommandParameter2, CommandParameter3);
        }
    }
    void shiftCommands(Commands CommandName,std::string CommandParameter1 = "",std::string CommandParameter2 = "",
                       std::string CommandParameter3 = ""){
        firstCommand = secondCommand;
        firstCommandParameter1 = secondCommandParameter1;
        firstCommandParameter2 = secondCommandParameter2;
        firstCommandParameter3 = secondCommandParameter3;
        secondCommand = thirdCommand;
        secondCommandParameter1 = thirdCommandParameter1;
        secondCommandParameter2 = thirdCommandParameter2;
        secondCommandParameter3 = thirdCommandParameter3;
        thirdCommand = CommandName;
        thirdCommandParameter1 = std::move(CommandParameter1);
        thirdCommandParameter2 = std::move(CommandParameter2);
        thirdCommandParameter3 = std::move(CommandParameter3);
    }

    void callUndo(RowNode currentRow){
        if (currentCommand.empty()){
            std::cout<<"no data"<<std::endl;
            return;
        }
        if (currentCommand == "1"){
            callReverseCommand(firstCommand, currentRow, firstCommandParameter1,
                               firstCommandParameter2, firstCommandParameter3);
            currentCommand = "";
            return;
        }
        if (currentCommand == "2"){
            callReverseCommand(secondCommand, currentRow, secondCommandParameter1,
                               secondCommandParameter2, secondCommandParameter3);
            currentCommand = "1";
            return;
        }
        if (currentCommand == "3"){
            callReverseCommand(thirdCommand, currentRow, thirdCommandParameter1,
                               thirdCommandParameter2, thirdCommandParameter3);
            currentCommand = "2";
            return;
        }

    }

    void callRedo(RowNode currentRow){
        if (currentCommand == "3"){
            std::cout<<"no data"<<std::endl;
            return;
        }
        if (currentCommand.empty()){
            callCommand(firstCommand, currentRow, firstCommandParameter1,
                               firstCommandParameter2, firstCommandParameter3);
            currentCommand = "1";
            return;
        }
        if (currentCommand == "1"){
            callCommand(secondCommand, currentRow, secondCommandParameter1,
                               secondCommandParameter2, secondCommandParameter3);
            currentCommand = "2";
            return;
        }
        if (currentCommand == "2"){
            callCommand(thirdCommand, currentRow, thirdCommandParameter1,
                               thirdCommandParameter2, thirdCommandParameter3);
            currentCommand = "3";
            return;
        }

    }

    void callReverseCommand(Commands CommandName, RowNode currentRow, std::string CommandParameter1 = "",
                            std::string CommandParameter2 = "", std::string CommandParameter3 = ""){
        if (CommandName == COMMAND_INSERT){
            currentRow.deleteText(std::stoi(CommandParameter1),std::stoi(CommandParameter2), CommandParameter3.length());
        }
        if (CommandName == COMMAND_DELETE){
            currentRow.insert(std::stoi(CommandParameter1),std::stoi(CommandParameter2), CommandParameter3);
        }
    }
    void callCommand(Commands CommandName, RowNode currentRow, std::string CommandParameter1 = "",
                            std::string CommandParameter2 = "", std::string CommandParameter3 = ""){
        if (CommandName == COMMAND_INSERT){
            currentRow.insert(std::stoi(CommandParameter1),std::stoi(CommandParameter2), CommandParameter3);
        }
        if (CommandName == COMMAND_DELETE){
            currentRow.deleteText(std::stoi(CommandParameter1),std::stoi(CommandParameter2), CommandParameter3.length());
        }
    }
};


int main() {
    bool start = false;
    std::string cache;
    RowNode *Head;
    RowNode *CurrentRow;
    CharNode *CurrentChar;
    LastCommands CommandsCache;
    fflush(stdin);
    while (true) {
        int n;
        try
        {
            std::string s; std::cout << "Chose the command: "; getline( std::cin, s );
            n = std::stoi( s );
            if (n < 0) throw 1;
        }
        catch (...)
        {
            std::cerr << "You must answer with a whole number >= 0." << std::endl;
            continue;
        }
        fflush(stdin);
        if (n > 14){
            std::cout << "The command is not implemented!" << std::endl;
            continue;
        }
        system("clear");
        if (n == COMMAND_APPEND){
            std::string input; std::cout << "enter text: "; getline( std::cin, input );
            if (!start){
                Head = new RowNode;
                CurrentRow = Head;
                CurrentRow->head = new CharNode;
                CurrentChar = CurrentRow->head;
                start = true;
            }
            int character = 0;
            CharNode *localCurrent = CurrentRow->head;
            while(localCurrent != CurrentChar){
                if (localCurrent->next != nullptr) localCurrent = localCurrent->next;
                else break;
                character++;
            }
            Head->insert(CurrentRow->rowN, character, input);
            while(true){
                if (CurrentChar->next != nullptr) CurrentChar = CurrentChar->next;
                else break;
            }
            CommandsCache.AddCommand(COMMAND_INSERT,std::to_string(CurrentRow->rowN),
                                     std::to_string(character),input);
            continue;
        }
        if (n == COMMAND_NEWLINE){
            if (!start){
                Head = new RowNode;
                CurrentRow = Head;
                CurrentRow->head = new CharNode;
                CurrentChar = CurrentRow->head;
                CurrentRow->rowN = 1;
                start = true;
            }
            CurrentRow->next = new RowNode;
            CurrentRow->next->rowN = CurrentRow->rowN + 1;
            CurrentRow = CurrentRow->next;
            CurrentRow->head = new CharNode;
            CurrentChar = CurrentRow->head;
            continue;
        }
        if (n == COMMAND_SAVE) {
            std::string input; std::cout <<"Enter the file name for saving: "; getline( std::cin, input );
            Head->saveFile(input);
            CommandsCache.ClearCommands();
            continue;
        }
        if (n == COMMAND_LOAD){
            std::string input; std::cout <<"Enter the file name for loading: "; getline( std::cin, input );
            std::string filename = input + ".txt";
            std::ifstream infile(filename);
            std::string line;
            if (Head != nullptr){
                Head->recursiveFree(Head, Head->head);
                start = false;
            }
            Head = new RowNode;
            while (getline(infile, line)) {
                std::istringstream iss(line);
                if (!start){
                    start = true;
                    CurrentRow = Head;
                    CurrentRow->head = new CharNode;
                    CurrentChar = CurrentRow->head;
                } else {
                    CurrentRow->next = new RowNode;
                    CurrentRow = CurrentRow->next;
                    CurrentRow->head = new CharNode;
                    CurrentChar = CurrentRow->head;
                }
                for (char i: line) {
                    CurrentChar->append(i);
                    if (CurrentChar->next != nullptr) {
                        CurrentChar = CurrentChar->next;
                    }
                }
                if (!(iss >> line)) { break; } // error
            }
            CommandsCache.ClearCommands();
            CommandsCache = *new LastCommands;
            continue;
        }
        if (n == COMMAND_PRINT){
            if (Head == nullptr){
                std::cout << "No data";
                continue;
            }
            Head->recursivePrint(Head, Head->head);
            continue;
        }
        if (n == COMMAND_INSERT){
            std::string row; std::cout <<"Enter the row index: "; std::cin>> row;
            fflush(stdin);
            std::string character; std::cout <<"Enter the symbol index: "; std::cin>> character;
            fflush(stdin);
            std::string input; std::cout <<"Enter text: "; getline( std::cin, input );
            Head->insert(std::stoi(row), std::stoi(character), input);
            CommandsCache.AddCommand(COMMAND_INSERT,std::to_string(std::stoi(row)),
                                     character,input);
            continue;
        }
        if (n == COMMAND_SEARCH){
            std::string input; std::cout << "enter text: "; getline( std::cin, input );
            if (Head == nullptr){
                std::cout << "No data"<<std::endl;
                continue;
            }
            Head->recursiveSearch(input, Head, Head->head, 1, 1);
            continue;
        }
        if (n == COMMAND_DELETE){
            std::string row; std::cout <<"Enter the row index: "; std::cin>> row;
            fflush(stdin);
            std::string character; std::cout <<"Enter the symbol index: "; std::cin>> character;
            fflush(stdin);
            std::string nOf; std::cout << "Enter n of symbols:  "; getline(std::cin, nOf );
            std::string  deletedText;
            RowNode *currentRow = Head;
            for (int i = 0; i < std::stoi(row); i++) {
                if (currentRow->next != nullptr) currentRow = currentRow->next;
            }
            CharNode *currentChar = currentRow->head;
            for (int i = 0; i < std::stoi(character); i++) {
                if (currentChar->next != nullptr) currentChar = currentChar->next;
            }
            CharNode *nextChar = currentChar->next;
            for (int i = 0; i < std::stoi(nOf); i++) {
                deletedText += nextChar->data;
                if (nextChar->next != nullptr) nextChar = nextChar->next;
            }
            Head->deleteText(std::stoi(row), std::stoi(character), std::stoi(nOf));
            CommandsCache.AddCommand(COMMAND_DELETE,row,character,
                                     deletedText);
            continue;
        }
        if (n == COMMAND_COPY){
            std::string row; std::cout <<"Enter the row index: "; std::cin>> row;
            fflush(stdin);
            std::string character; std::cout <<"Enter the symbol index: "; std::cin>> character;
            fflush(stdin);
            std::string nOf; std::cout << "Enter n of symbols:  "; getline(std::cin, nOf );
            cache = "";
            Head->copyText(std::stoi(row), std::stoi(character), std::stoi(nOf),
                           cache);
            std::cout<<(cache)<<std::endl;
            continue;
        }
        if (n == COMMAND_PASTE){
            std::string row; std::cout <<"Enter the row index: "; std::cin>> row;
            fflush(stdin);
            std::string character; std::cout <<"Enter the symbol index: "; std::cin>> character;
            fflush(stdin);
            Head->insert(std::stoi(row), std::stoi(character), cache);
            CommandsCache.AddCommand(COMMAND_INSERT,std::to_string(std::stoi(row)),character,cache);
            continue;
        }
        if (n == COMMAND_CUT){
            std::string row; std::cout <<"Enter the row index: "; std::cin>> row;
            fflush(stdin);
            std::string character; std::cout <<"Enter the symbol index: "; std::cin>> character;
            fflush(stdin);
            std::string nOf; std::cout << "Enter n of symbols:  "; getline(std::cin, nOf );
            cache = "";
            Head->copyText(std::stoi(row), std::stoi(character), std::stoi(nOf),
                           cache);
            std::string  deletedText;
            RowNode *currentRow = Head;
            for (int i = 0; i < std::stoi(row); i++) {
                if (currentRow->next != nullptr) currentRow = currentRow->next;
            }
            CharNode *currentChar = currentRow->head;
            for (int i = 0; i < std::stoi(character); i++) {
                if (currentChar->next != nullptr) currentChar = currentChar->next;
            }
            CharNode *nextChar = currentChar->next;
            for (int i = 0; i < std::stoi(nOf); i++) {
                deletedText += nextChar->data;
                if (nextChar->next != nullptr) nextChar = nextChar->next;
            }
            Head->deleteText(std::stoi(row), std::stoi(character), std::stoi(nOf));
            CommandsCache.AddCommand(COMMAND_DELETE,row,character,
                                     deletedText);
            continue;
        }
        if (n == COMMAND_UNDO){
            CommandsCache.callUndo(*Head);
        }
        if (n == COMMAND_REDO){
            CommandsCache.callRedo(*Head);
        }
        if (n == COMMAND_CLOSE){
            if (Head != nullptr){
                Head->recursiveFree(Head, Head->head);
            }
            Head = nullptr;
            start = false;
            break;
        }
    }
}
