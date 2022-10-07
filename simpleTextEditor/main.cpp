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
    int position;
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
        rowNodePointer->position = lines;
        rowHead = rowNodePointer;
    }else{
        rowNodePointer->next = (struct RowNode*) malloc(sizeof(struct RowNode));
        rowNodePointer = rowNodePointer->next;
        rowNodePointer->head = (struct CharNode*) malloc(sizeof(struct CharNode));
        lines++;
        rowNodePointer->position = lines;
    }
}//kk
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
}//kk
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
}//kk
void printAll(){
    struct RowNode* currentRow = rowHead;
    while(true){
        struct CharNode* currentChar = currentRow->head;
        while(true){
            if (currentChar->data == 0){
                if (currentChar->next == nullptr){
                    break;
                }
                currentChar = currentChar->next;
                continue;
            }
            char a = currentChar->data;
            std::cout << a;
            if (currentChar->next == nullptr){
                break;
            }
            currentChar = currentChar->next;
        }
        std::cout << "\n";
        if (currentRow->next == nullptr) break;
        currentRow = currentRow->next;
    }
}
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
}
void loadFile(std::string filename){
    filename = filename + ".txt";
    std::ifstream infile(filename);
    std::string line;
    free(rowHead);
    rowHead = nullptr;
    start = false;
    while (getline(infile, line))
    {
        newLine();
        std::istringstream iss(line);
        append(line);
        if (!(iss >> line)) { break; } // error
    }
}
void search(std::string input){
    bool answerNotFound = true;
    int rowAnswer;
    int charAnswer = 0;
    struct CharNode* currChar;
    struct RowNode* currRow;
    currRow = rowHead;
    currChar = rowHead->head;
    while(answerNotFound){
        if (currChar->data != input[0]){
            if (currChar->next != nullptr){
                currChar = currChar->next;
                charAnswer++;
                continue;
            }
            else{
                if (currRow->next != nullptr){
                    currRow = currRow->next;
                    charAnswer = 0;
                    currChar = currRow->head;
                }
                else{
                    break;
                }
            }
        }
        else{
            char currToCheckWith = input[0];
            int n = 0;
            while(answerNotFound){
                if (currChar->data == currToCheckWith){
                    n++;
                    if (input[n] == '\0'){
                        rowAnswer = currRow->position;
                        answerNotFound = false;
                    }
                    currToCheckWith = input[n];
                    if (currChar->next != nullptr){
                        currChar = currChar->next;
                        continue;
                    } else{
                        break;
                    }
                } else {
                    break;
                }
            }
        }
    }
    std::cout << rowAnswer;
    std::cout << "  ";
    std::cout << charAnswer << std::endl;
}
int main() {
    fflush(stdin);
    while (true) {
        int n;
        try
        {
            std::cout << "Chose the command: ";
            fflush(stdin);
            std::string s;
            getline( std::cin, s );
            n = std::stoi( s );
            if (n < 0) throw 1;
        }
        catch (...)
        {
            std::cerr << "You must answer with a whole number >= 0." << std::endl;
            continue;
        }
        fflush(stdin);
        if (!(n >= 0 && n <= 8)){
            std::cout << "The command is not implemented!" << std::endl;
            continue;
        }
        system("clear");
        if (n == 1){
            std::string input;
            std::cout << "enter text: ";
            std::cin>> input;
            append(input);
            continue;
        }
        if (n == 2){
            newLine();
            continue;
        }
        if (n == 3) {
            std::string input;
            std::cout <<"Enter the file name for saving: ";
            std::cin>> input;
            saveFile(input);
            continue;
        }
        if (n == 4){
            std::string input;
            std::cout <<"Enter the file name for loading: ";
            std::cin>> input;
            loadFile(input);
            continue;
        }
        if (n == 5){
            printAll();
            continue;
        }
        if (n == 6){
            std::string row;
            std::string character;
            std::string input;
            std::cout <<"Enter the row index: ";
            std::cin>> row;
            std::cout <<"Enter the symbol index: ";
            std::cin>> character;
            std::cout <<"Enter text: ";
            std::cin>> input;
            insert(std::stoi(row), std::stoi(character), input);
            continue;
        }
        if (n == 7){
            std::string input;
            std::cout << "enter text: ";
            std::cin>> input;
            search(input);
            continue;
        }
        if (n == 8){
            break;
        }
    }
}
