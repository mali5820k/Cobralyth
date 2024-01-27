#include "lexer.hpp"
#include <filesystem>
#include <fstream>
#include <format>

Token::Token(int tokenLine, std::string tokenValue, TokenType tokenType) {
    this->tokenLine = tokenLine;
    this->tokenValue = tokenValue;
    this->tokenType = tokenType;
}

Token::~Token() {}

std::string Token::toString() {
    std::string representation("Token line: " + std::to_string(this->tokenLine) + "\nToken type: "+ std::to_string(this->tokenType) + "\nToken value:" + this->tokenValue + "\n\n");
    return representation;
}

Lexer::Lexer() {
    this->currentLineCharIndex = 0;
    this->lineCounter = 0;
}

Lexer::~Lexer() {}

std::vector<std::string> Lexer::scannedContents() {
    std::vector<std::string> returnVal = {};

    return returnVal;
}

std::vector<Token> Lexer::tokenList() {
    std::vector<Token> returnVal = {};

    return returnVal;
}

// Scans the provided file
bool Lexer::scanFile(std::string filename) {
    this->filename = filename;
    // Check if file extension is not of .clyth
    if (std::filesystem::path(this->filename).extension() != ".clyth") {
        return false;
    }

    std::ifstream startingFile(this->filename);
    std::string curFileLine;
    while (std::getline (startingFile, curFileLine)) {
        this->fileLine = curFileLine;
        this->lexLine();
        this->lineCounter++;
    }

    startingFile.close();

    return true;
}

void Lexer::consumeChar() {
    if (this->currentLineCharIndex + 1 < this->fileLine.length()) {
        this->currentLineCharIndex += 1;
        this->currentChar = this->nextChar;
        this->nextChar = this->fileLine[this->currentLineCharIndex];
        this->endOfFileReached = false;
        return;
    }
    this->endOfFileReached = true;
    return;
}

void Lexer::checkMatch(std::vector<std::string> keywordList) {
    // Check for number-literal, string-literal, function/class/method/variable signatures.

    std::string matchedKeyword = "";

    // Check each word to see if the currentCharacter matches the current index of that word.
    // If not, we exit that loop iteration and skip past the matched letters by checking with the
    // current matchedKeyword's contents.
    // Starting case is that the matchedKeyword buffer is empty, in which case we want to scan using the current character
    // Otherwise, we want to exhaust all the characters in the matchedKeyword buffer and then continue scanning with the currentChar.

    // TODO: Create the looping logic that implements the above ^^^^^
    for (auto & word : keywordList) {
        for (auto & letter : word) {
            if (letter == this->currentChar) {

            }
        }
    }

    // IF match is found, create a token for that type and append to this->scannedTokens;
    // ELSE, create token of type identifier
}

// It'd be better to make a trie that contains all of the key-words in a large tree, so the "testConsume" function parses the list of words until it finds one
void Lexer::lexLine() {
    std::string currentBuffer = "";
    // Check for longest match on each keyword and clear if match found.
    while (!this->endOfFileReached) {
        switch (this->currentChar) {
            case '+':
            case '-':
            case '*':
            case '/':
            case '^':
            case '=':
            case '>':
            case '<':
            case '|':
            case '{':
            case '[':
            case '(':
            case ',':
            case '.':
            case '%':
            case ' ':
                // We consume all whitespace since it's not in a string if we haven't hit a single or double-quote
                consumeChar();
                break;
            case '"':
                // Essentially consume all characters until the end of the string is encountered.
                int startingStringLine = this->lineCounter;
                while (this->currentChar != '"') {
                    if (this->endOfFileReached) {
                        std::cout << std::format("\nERROR: EOF reached without closure to string starting at line {}!!\n", startingStringLine);
                    }
                    currentBuffer += this->currentChar;
                    consumeChar();
                }
                currentBuffer += this->currentChar;
                consumeChar();
                break;
            default:
                std::vector<std::string> keywordList = {
                    "and",
                };
                
                checkMatch(keywordList);
                break;
        }
    }
}