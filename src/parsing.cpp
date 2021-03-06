#pragma once

#include <cstdlib>
#include <inttypes.h>
#include <stdio.h>
#include <string>

class EatableString {
    public:

    const char *string;
    const char *c;
    uint32_t line = 1;

    EatableString(const char *string) {
        this->string = string;
        this->c = this->string; 
    }

    /*  Skip leading whitespaces and new lines */
    bool trim() {
        bool spacer = (*(this->c) == ' ' || *(this->c) == '\n');
        while(*(this->c) == ' ' || *(this->c) == '\n') {
            if(*(this->c) == '\n') line++;
            nextChar();
        }
        return spacer;
    }

    /*  Increment the pointer (char *c) */
    void nextChar() {
        if(*(this->c) != 0) {
            this->c++;
        } else {
            printf("Input end reached\n");
        }
    }

    /*  Increment the pointer (char *c) and return the new character */
    char next() {
        if(*(this->c) != 0) {
            return (*this->c++);
        } else {
            printf("Input end reached\n");
            return 0;
        }
    }

    /*  Increments pointer and returns true if next character equals T
        Skips leading whitespaces and new lines. */
    bool eat(char t) {
        trim();
        if(*this->c == t) {
            nextChar();
            return true;
        }
        return false;
    }

    /*  Returns true if next character equals T
        Skips leading whitespaces and new lines. */
    bool next(char t) {
        if(*this->c == t) {
            nextChar();
            return true;
        }
        return false;
    }

    /*  Trims the input and then checks if the next character is 
        what was expected. */
    bool is(char t) {
        trim();
        return *c == t;
    }

    /*  Trims the input and returns the following character. */
    char get() {
        trim();
        return *c;
    }

    /*  Trims the input and returns the current character and then steps forward. */
    char read() {
        trim();
        return *(c++);
    }

    const char* currentLine() {
        const char* start = c;
        while(*start != '\n' && start > string) {
            start--;
        }
        if(*start == '\n') start++;

        return start;
    }
};

#define READ_WORD_BUFFER_SIZE 64

// #########################
// ### READING FUNCTIONS ###
// #########################


bool isNameCharacter(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_';
}

bool isNameFirstCharacter(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

bool isNumberCharacter(char c) {
    return (c >= '0' && c <= '9') || c == '.';
}

bool isIntegerCharacter(char c) {
    return (c >= '0' && c <= '9');
}

// #########################
// ### PARSING FUNCTIONS ###
// #########################

std::string readWord(EatableString *in) {
    if(isNameFirstCharacter(in->get())) {
        char buffer[READ_WORD_BUFFER_SIZE];
        uint8_t bufferPointer = 0;

        while(isNameCharacter(*in->c) || isIntegerCharacter(*in->c) || *in->c == '_') {
            if(bufferPointer == READ_WORD_BUFFER_SIZE) {
                printf("err: max name length reached\n");
                std::exit(1);
            }
            buffer[bufferPointer++]= *in->c;
            in->nextChar();
        }
        buffer[bufferPointer]= 0;

        return std::string(buffer);
    } else {
        return std::string();
    }
}