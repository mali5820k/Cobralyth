#include "lexer.hpp"
#include <filesystem>
#include <fstream>

Token::Token(int tokenLine, std::string tokenValue, TokenType tokenType) {
    this->tokenLine = tokenLine;
    this->tokenValue = tokenValue;
    this->tokenType = tokenType;
}

std::string Token::toString() {
    std::string representation("Token line: " + std::to_string(this->tokenLine) + "\nToken type: "+ std::to_string(this->tokenType) + "\nToken value:" + this->tokenValue + "\n\n");
    return representation;
}

Lexer::Lexer(std::string filename, std::map<std::string, bool> importTable) {
    this->importTableReference = &importTable;
    this->filename = filename;
    this->currentLineCharIndex = 0;
    this->lineCounter = 0;
}

Lexer::~Lexer() {}

std::vector<std::string> Lexer::scannedContents() {

}

std::vector<Token> Lexer::tokenList() {

}

bool Lexer::startScanning() {
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
}

bool Lexer::updateFilename(std::string filename) {
    this->filename = filename;
}

// Returns 0 if not found, positive integer for last index of string parsed.
int Lexer::testConsume(std::string term, int index, std::string fileLine) {
    int retVal = 0;
    for (int i = index; i < term.length(); i++) {

    }

    return retVal;
}

bool Lexer::consumeChar() {
    if (this->currentLineCharIndex + 1 < this->fileLine.length()) {
        this->currentLineCharIndex += 1;
        this->currentChar = this->nextChar;
        this->nextChar = this->fileLine[this->currentLineCharIndex];
        return true;
    }

    return false;
}

// It'd be better to make a trie that contains all of the key-words in a large tree, so the "testConsume" function parses the list of words until it finds one
void Lexer::lexLine() {
    bool inString = false;
    std::string currentBuffer = "";
    // Check for longest match on each keyword and clear if match found.
    // Otherwise, check for number-literal, string-literal, function/class/method/variable signatures.
    // Lastly, could simply be an unsupported bit in the language, so throw an error.
    switch (currentChar) {
        case 'a':
            
            // int part1 = testConsume("array", i, fileLine);
            // if (part1) {
            //     // Parse part2:
            //     int part2 = testConsume("<", part1, fileLine);
            //     if (part2) {
                    
            //     }
            //     // Create new token
            //     this->scannedTokens.push_back(Token(*lineCounter, "array", TokenType.ARRAY));
            //     // Clear buffer since match was found
            // }
            
            break;
        case 'b':
        case 'c':
        case 'd':
        case 'e':
        case 'f':
        case 'i':
        case 'l':
        case 'm':
        case 'n':
        case 'o':
        case 'p':
        case 'r':
        case 's':
        case 'u':
        case 'v':
        case 'w':
        case 'x':
        case ' ':
            // We consume all whitespace since it's not in a string if we haven't hit a single or double-quote
            consumeChar();
            break;
        case '"':
        case '\'':
            // It's a character/string, so just read it all in until you hit another single-quote
            while (this->currentChar != '\'') {
                currentBuffer += this->currentChar;
                consumeChar();
            }
        default:
            break;
        
    }
}