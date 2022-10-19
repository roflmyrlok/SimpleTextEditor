#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>


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
    void recursiveFree(RowNode* currentRow, CharNode* currentChar){
        RowNode* nextRow;
        CharNode* nextChar;
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
        recursiveFree(nextRow, nextChar);
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
        for (int i = 1; i < row; i++) {
            if (currentRow->next != nullptr) currentRow = currentRow->next;
        }
        CharNode *currentChar = currentRow->head;
        for (int i = 1; i < character; i++) {
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
bool start = false;
RowNode *Head;
RowNode *CurrentRow;
CharNode *CurrentChar;
int main() {
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
        if (n > 8){
            std::cout << "The command is not implemented!" << std::endl;
            continue;
        }
        system("clear");
        if (n == 1){
            std::string input; std::cout << "enter text: "; getline( std::cin, input );
            if (!start){
                Head = new RowNode;
                CurrentRow = Head;
                CurrentRow->head = new CharNode;
                CurrentChar = CurrentRow->head;
                start = true;
            }
            for (char i : input) {
                CurrentChar->append(i);
                if(CurrentChar->next != nullptr){
                    CurrentChar = CurrentChar->next;
                }
            }
            continue;
        }
        if (n == 2){
            if (!start){
                Head = new RowNode;
                CurrentRow = Head;
                CurrentRow->head = new CharNode;
                CurrentChar = CurrentRow->head;
                start = true;
            }
            CurrentRow->next = new RowNode;
            CurrentRow = CurrentRow->next;
            CurrentRow->head = new CharNode;
            CurrentChar = CurrentRow->head;
            continue;
        }
        if (n == 3) {
            std::string input; std::cout <<"Enter the file name for saving: "; getline( std::cin, input );
            Head->saveFile(input);
            continue;
        }
        if (n == 4){
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
            continue;
        }
        if (n == 5){
            if (Head == nullptr){
                std::cout << "No data";
                continue;
            }
            Head->recursivePrint(Head, Head->head);
            continue;
        }
        if (n == 6){
            std::string row; std::cout <<"Enter the row index: "; std::cin>> row;
            fflush(stdin);
            std::string character; std::cout <<"Enter the symbol index: "; std::cin>> character;
            fflush(stdin);
            std::string input; std::cout <<"Enter text: "; getline( std::cin, input );
            Head->insert(std::stoi(row), std::stoi(character), input);
            continue;
        }
        if (n == 7){
            std::string input; std::cout << "enter text: "; getline( std::cin, input );
            if (Head == nullptr){
                std::cout << "No data";
                continue;
            }
            Head->recursiveSearch(input, Head, Head->head, 1, 1);
            continue;
        }
        if (n == 8){
            if (Head != nullptr){
                Head->recursiveFree(Head, Head->head);
            }
            Head = nullptr;
            start = false;
            break;
        }
    }
}