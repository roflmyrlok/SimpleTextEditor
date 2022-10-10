#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>

struct CharNode{
    char data;
    struct CharNode* next;
};

struct RowNode {
    struct CharNode* head;
    struct RowNode* next;
};


struct RowNode* rowNodePointer;
struct RowNode* rowHead = nullptr;
bool start = false;
int lines = 0;


void newLine(){
    if (start == 0) {
        start = true;
        rowNodePointer = (struct RowNode*) malloc(sizeof(struct RowNode));
        rowNodePointer->head = (struct CharNode*) malloc(sizeof(struct CharNode));
        lines = 1;
        rowHead = rowNodePointer;
    }else{
        rowNodePointer->next = (struct RowNode*) malloc(sizeof(struct RowNode));
        rowNodePointer = rowNodePointer->next;
        rowNodePointer->head = (struct CharNode*) malloc(sizeof(struct CharNode));
        lines++;
    }
}//2
void append(const std::string& input){
    if (start == 0){
        newLine();
    }
    struct CharNode* current = rowNodePointer->head;
    while (current->next != nullptr){
        current = current->next;
    }
    for (char i : input) {
        current->next = (struct CharNode*) malloc(sizeof(struct CharNode));
        current = current->next;
        current->data = i;
    }
}//1
void saveFile(std::string filename){
    filename = filename + ".txt";
    std::fstream MyFile;
    //std::ofstream MyFile(filename);
    struct RowNode* currentRowNode = rowHead;
    MyFile.open (filename);
    if (MyFile.is_open())std::cout << "error opening" << std::endl;
    while(true){
        std::string currentRow;
        struct CharNode* currentCharNode = currentRowNode->head;
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
void recursiveFree(struct RowNode* currentRow, struct CharNode* currentChar){
    struct RowNode* nextRow;
    struct CharNode* nextChar;
    if(currentChar->next != nullptr){
        nextChar = currentChar->next;
        free(currentChar);
        nextRow = currentRow;
    } else {
        if (currentRow->next != nullptr){
            nextRow = currentRow->next;
            free(currentRow);
            nextChar = nextRow->head;
        } else  {
            free(currentChar);
            free(currentRow);
            return;
        }
    }
    recursiveFree(nextRow, nextChar);
}//loadFile support func
void loadFile(std::string filename){
    filename = filename + ".txt";
    std::ifstream infile(filename);
    std::string line;
    if (rowHead != nullptr){
        recursiveFree(rowHead, rowHead->head);
    }
    rowHead = nullptr;
    start = false;
    while (getline(infile, line))
    {
        newLine();
        std::istringstream iss(line);
        append(line);
        if (!(iss >> line)) { break; } // error
    }
}//4
void recursivePrint(struct RowNode* currentRow, struct CharNode* currentChar){
    struct RowNode* nextRow;
    struct CharNode* nextChar;
    if(currentChar->next != nullptr){
        nextChar = currentChar->next;
        if (currentChar->data != 0) std::cout << currentChar->data;
        nextRow = currentRow;
    } else if (currentRow->next != nullptr){
        if (currentChar->data != 0) std::cout << currentChar->data;
        std::cout << "" << std::endl;
        nextRow = currentRow->next;
        nextChar = nextRow->head;
    } else {
        std::cout << currentChar->data << std::endl;
        return;
    }
    recursivePrint(nextRow, nextChar);
}//5
void insert(int row, int character, const std::string& input){
    struct RowNode* currentRow = rowHead;
    for (int i = 1; i < row; i++) {
        if (currentRow->next != nullptr) currentRow = currentRow->next;
    }
    struct CharNode* currentChar = currentRow->head;
    for (int i = 1; i < character; i++) {
        if (currentChar->next != nullptr) currentChar = currentChar->next;
    }
    struct CharNode* nextChar = currentChar->next;

    for (char i : input) {
        currentChar->next = (struct CharNode*) malloc(sizeof(struct CharNode));
        currentChar = currentChar->next;
        currentChar->data = i;
    }
    currentChar->next = nextChar;
}//6
bool localRecursiveSearch(int n, struct CharNode* currChar, std::string currString) {
    struct CharNode *nextChar;
    if (currChar->next != nullptr) nextChar = currChar->next; else return false;
    n++;
    if (nextChar->data == currString[n - 1]){
        if (n == currString.length()) return true; else {
            return localRecursiveSearch(n, nextChar, currString);
        }
    } else return false;
}
void recursiveSearch(std::string str, struct RowNode* currRow, struct CharNode* currChar, int nRow, int nStr){
    struct RowNode* nextRow;
    struct CharNode* nextChar;
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
            append(input);
            continue;
        }
        if (n == 2){
            newLine();
            continue;
        }
        if (n == 3) {
            std::string input; std::cout <<"Enter the file name for saving: "; getline( std::cin, input );
            saveFile(input);
            continue;
        }
        if (n == 4){
            std::string input; std::cout <<"Enter the file name for loading: "; getline( std::cin, input );
            loadFile(input);
            continue;
        }
        if (n == 5){
            if (rowHead == nullptr){
                std::cout << "No data";
                continue;
            }
            recursivePrint(rowHead, rowHead->head);
            continue;
        }
        if (n == 6){
            std::string row; std::cout <<"Enter the row index: "; std::cin>> row;
            fflush(stdin);
            std::string character; std::cout <<"Enter the symbol index: "; std::cin>> character;
            fflush(stdin);
            std::string input; std::cout <<"Enter text: "; getline( std::cin, input );
            insert(std::stoi(row), std::stoi(character), input);
            continue;
        }
        if (n == 7){
            std::string input; std::cout << "enter text: "; getline( std::cin, input );
            if (rowHead == nullptr){
                std::cout << "No data";
                continue;
            }
            recursiveSearch(input, rowHead, rowHead->head, 1, 1);
            continue;
        }
        if (n == 8){
            if (rowHead != nullptr){
                recursiveFree(rowHead, rowHead->head);
            }
            rowHead = nullptr;
            start = false;
            break;
        }
    }
}
