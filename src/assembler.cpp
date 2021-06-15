#include <bits/stdint-uintn.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <map>

#include "parsing.cpp"


class Output;



// File structure
void parseLine(EatableString *in, Output* output);

// Line types
void parseAction(EatableString *in, Output* output);
bool parseLabel(EatableString *in, Output* output);
bool parseInstruction(EatableString *in, Output* output);

// Instruction components
uint8_t parseRegisters(EatableString *in);
enum LiteralType { STRING, U8, U16, U32, BOOLEAN };
union LiteralValue {
    std::string string; uint8_t u8; uint16_t u16; uint32_t u32; 
    
    LiteralValue() { /*...*/ }
    LiteralValue(const LiteralValue&c) { /*...*/ }
    
    ~LiteralValue() {}
};
// Had to create this struct because std::pair is a cancer
// and I can't figure out how to initialize it in one line
struct Literal {
    LiteralType type;
    LiteralValue value;
    
    Literal(LiteralType type, LiteralValue value) : type(type), value(value) {}
    
    ~Literal() {}
};
Literal parseLiteral(EatableString *in);

// Action functions
void actionStack(EatableString *in, Output* output);
void actionText(EatableString *in, Output* output);

// Action map
std::map<std::string, void(*)(EatableString *in, Output* output)> actions = {
    {"stack", actionStack},
    {"text", actionText}
};


enum Arg {
    Reg,
    Imm,
    Mem,
    Imm8,
    Imm16,
    Imm32
};

class Instruction {
    public:
    
    uint32_t microInstructionCode = 0;
    std::vector<Arg> arguments;
    
    Instruction(uint32_t microInstructionCode, std::vector<Arg> arguments) : 
            microInstructionCode(microInstructionCode),
            arguments(arguments) {
        
    }
    
    Instruction() {
        
    }
};

std::map<std::string, Instruction> instructions = {
    // Stop execution 
    {"HALT", Instruction(0x000, {})},
    // Rd = S
    {"MOV", Instruction(0x001, {Arg::Reg, Arg::Reg})},
    // Rd = S
    {"MOV", Instruction(0x002, {Arg::Reg, Arg::Imm8})},
    // Rd = S
    {"MOV", Instruction(0x003, {Arg::Reg, Arg::Imm16})},
    // Rd = S
    {"MOV", Instruction(0x004, {Arg::Reg, Arg::Imm32})},
    // Rd = Ms
    {"LDA", Instruction(0x005, {Arg::Reg, Arg::Mem})},
    // Md = Rs
    {"STA", Instruction(0x006, {Arg::Mem, Arg::Reg})},
    // Md = Rs
    {"STA", Instruction(0x007, {Arg::Mem, Arg::Imm8})},
    // Rd = Rd + S
    {"ADD", Instruction(0x008, {Arg::Reg, Arg::Reg})},
    // Rd = Rd + S
    {"ADD", Instruction(0x009, {Arg::Reg, Arg::Imm8})},
    // Rd = Rd + S
    {"ADD", Instruction(0x00a, {Arg::Reg, Arg::Imm16})},
    // Rd = Rd + S
    {"ADD", Instruction(0x00b, {Arg::Reg, Arg::Imm32})},
    // Rd = Rd - S
    {"SUB", Instruction(0x00c, {Arg::Reg, Arg::Reg})},
    // Rd = Rd - S
    {"SUB", Instruction(0x00d, {Arg::Reg, Arg::Imm8})},
    // Rd = Rd - S
    {"SUB", Instruction(0x00e, {Arg::Reg, Arg::Imm16})},
    // Rd = Rd - S
    {"SUB", Instruction(0x00f, {Arg::Reg, Arg::Imm32})},
    // Rd = Rd * S
    {"MUL", Instruction(0x010, {Arg::Reg, Arg::Reg})},
    // Rd = Rd * S
    {"MUL", Instruction(0x011, {Arg::Reg, Arg::Imm8})},
    // Rd = Rd * S
    {"MUL", Instruction(0x012, {Arg::Reg, Arg::Imm16})},
    // Rd = Rd * S
    {"MUL", Instruction(0x013, {Arg::Reg, Arg::Imm32})},
    // Rd = Rd | S
    {"OR", Instruction(0x014, {Arg::Reg, Arg::Reg})},
    // Rd = Rd | S
    {"OR", Instruction(0x015, {Arg::Reg, Arg::Imm8})},
    // Rd = Rd | S
    {"OR", Instruction(0x016, {Arg::Reg, Arg::Imm16})},
    // Rd = Rd | S
    {"OR", Instruction(0x017, {Arg::Reg, Arg::Imm32})},
    // Rd = Rd & S
    {"AND", Instruction(0x018, {Arg::Reg, Arg::Reg})},
    // Rd = Rd & S
    {"AND", Instruction(0x019, {Arg::Reg, Arg::Imm8})},
    // Rd = Rd & S
    {"AND", Instruction(0x01a, {Arg::Reg, Arg::Imm16})},
    // Rd = Rd & S
    {"AND", Instruction(0x01b, {Arg::Reg, Arg::Imm32})},
    // Rd = Rd ^ S
    {"XOR", Instruction(0x01c, {Arg::Reg, Arg::Reg})},
    // Rd = Rd ^ S
    {"XOR", Instruction(0x01d, {Arg::Reg, Arg::Imm8})},
    // Rd = Rd ^ S
    {"XOR", Instruction(0x01e, {Arg::Reg, Arg::Imm16})},
    // Rd = Rd ^ S
    {"XOR", Instruction(0x01f, {Arg::Reg, Arg::Imm32})},
    // Rd = !Rd
    {"NOT", Instruction(0x020, {Arg::Reg})},
    // Rd = Rd + 1
    {"INC", Instruction(0x021, {Arg::Reg})},
    // Rd = Rd - 1
    {"DEC", Instruction(0x022, {Arg::Reg})},
    // Jump to
    {"J", Instruction(0x023, {Arg::Mem})},
    // Jump to if zero-flag is 1
    {"JZ", Instruction(0x024, {Arg::Mem})},
    // Jump to if zero-flag is 0
    {"JNZ", Instruction(0x025, {Arg::Mem})},
    // Jump forward 
    {"JF", Instruction(0x026, {Arg::Imm8})},
    // Jump forward 
    {"JF", Instruction(0x027, {Arg::Imm16})},
    // Jump forward 
    {"JF", Instruction(0x028, {Arg::Imm32})},
    // Jump forward if zero-flag is 1
    {"JFZ", Instruction(0x029, {Arg::Imm8})},
    // Jump forward if zero-flag is 1
    {"JFZ", Instruction(0x02a, {Arg::Imm16})},
    // Jump forward if zero-flag is 1
    {"JFZ", Instruction(0x02b, {Arg::Imm32})},
    // Jump forward if zero-flag is 0
    {"JFNZ", Instruction(0x02c, {Arg::Imm8})},
    // Jump forward if zero-flag is 0
    {"JFNZ", Instruction(0x02d, {Arg::Imm16})},
    // Jump forward if zero-flag is 0
    {"JFNZ", Instruction(0x02e, {Arg::Imm32})},
    // Jump backward 
    {"JB", Instruction(0x02f, {Arg::Imm8})},
    // Jump backward 
    {"JB", Instruction(0x030, {Arg::Imm16})},
    // Jump backward 
    {"JB", Instruction(0x031, {Arg::Imm32})},
    // Jump backward if zero-flag is 1
    {"JBZ", Instruction(0x032, {Arg::Imm8})},
    // Jump backward if zero-flag is 1
    {"JBZ", Instruction(0x033, {Arg::Imm16})},
    // Jump backward if zero-flag is 1
    {"JBZ", Instruction(0x034, {Arg::Imm32})},
    // Jump backward if zero-flag is 0
    {"JBNZ", Instruction(0x035, {Arg::Imm8})},
    // Jump backward if zero-flag is 0
    {"JBNZ", Instruction(0x036, {Arg::Imm16})},
    // Jump backward if zero-flag is 0
    {"JBNZ", Instruction(0x037, {Arg::Imm32})}
};

std::map<std::string, uint8_t> registers = {
    {"a",       1} ,
    {"b",       1},
    {"c",       1},
    {"d",       1},
    {"x",       1},
    {"y",       1},
    {"z",       1},
    {"w",       1},
    {"i",       2},
    {"j",       2},
    {"k",       2},
    {"l",       2},
    {"ax",      2},
    {"by",      2},
    {"cz",      2},
    {"dw",      2},
    {"axi",     4},
    {"byj",     4},
    {"czk",     4},
    {"dwl",     4},
    {"pc",      4},
    {"mar",     4},
    {"mdr",     4},
    {"sp",      4},
    {"fc",      2},
};

enum InstructionArgType {
    REGISTER,
    LITERAL, 
    REF_LABEL
};

class InstructionArg {
    public:
    
    InstructionArgType type;
    union {
        struct {
            LiteralType literalType;
            LiteralValue literalValue;
        } literal;
        
        std::string registerName;
        
        std::string labelName;
    };
    
    InstructionArg() {
        
    }
    
    ~InstructionArg() {
        
    }
};

enum SectionType {
    NULL_SECTION,
    INSTRUCTION,
    TEXT,
    STACK,
    BUFFER,
    LABEL
};

struct Section {
    SectionType type = NULL_SECTION;
    
    union {
        struct {
            std::string name;
            std::vector<InstructionArg> args;
        } instruction;
        
        struct {
            std::vector<std::string> strings;
        } text;
        
        struct {
            uint32_t fixedSize;
        } stack;
        
        struct {
            uint32_t fixedSize;
        } buffer;
        
        struct {
            std::string name;
        } label;
    };
    
    Section() {
        
    }
    
    ~Section() {
        
    }
};

class Output {
    public:
    
    // This vector will be compared with 'usedLabels' at the end to see if any undeclared labels are used
    std::vector<std::string> knownLabels;
    // This vector will be checked if there aren't any duplicate labels
    std::vector<std::string> usedLabels;
    
    // Abstract representation of the code
    std::vector<Section> sections;
};

char* readEntireFile(const char* name) {

    FILE *file = fopen(name, "rb");
    
    if(file == 0) {
        printf("file doesnt' exist\n");
        return 0;
    }

    // Find file size
    fseek(file, 0, SEEK_END);
    long fsize = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Allocate big enough buffer (+ null-termiator)
    char* buffer = (char*)malloc(fsize + 1);
    fread(buffer, 1, fsize, file);
    buffer[fsize] = 0;

    fclose(file);

    return buffer;
}

int main(int argc, const char** args) {
    if(argc < 2) {
        printf("Missing parameters\n");
        return 1;
    }
    
    if(argc > 2) {
        printf("Too many parameters\n");
        return 1;
    }
    
    char* file = readEntireFile(args[0]);
    EatableString in = EatableString(file);
    Output output = Output();
    
    in.trim();
    while(in.c != 0) {
        parseLine(&in, &output);
        in.trim();
    }
}

// File structure
void parseLine(EatableString *in, Output* output) {
    in->trim();
    
    if(*in->c == '.') {
        parseAction(in, output);
    } else if(*in->c == '#') {
        while(*in->c != 0 && *in->c != '\n') in->c++;
    } else {
        if(!parseLabel(in, output)) 
            if(!parseInstruction(in, output))
                printf("err: syntax error on line %d\n", in->line);
    }
}

// Line types
void parseAction(EatableString *in, Output* output) {
    
}

bool parseLabel(EatableString *in, Output* output) {
    std::string labelName = readWord(in);
    if(labelName.empty()) return false;
    
    if(!in->next(':')) return false;
    
    Section section;
    section.type = LABEL;
    section.label.name = labelName;
    output->sections.push_back(section);
    
    output->knownLabels.push_back(labelName);
    
    return true;
}

bool parseInstruction(EatableString *in, Output* output) {
    std::string instructionName = readWord(in);
    if(instructionName.empty()) {
        printf("expected instruction name or label name\n");
        std::exit(1);
        return false;
    }
    
    bool found = false;
    Instruction foundInstruction;
    for(std::map<std::string, Instruction>::iterator iter = instructions.begin(); 
            iter != instructions.end(); 
            ++iter) {
        if(iter->first == instructionName) {
            found = true;
            break;
        }
    }
    
    if(!found) {
        printf("err: unknown instructions: %s\n", instructionName.c_str());
        std::exit(1);
    }    
    
    Section section;
    section.type = INSTRUCTION;
    section.instruction.name = instructionName;
    
    for(std::vector<Arg>::iterator iter = foundInstruction.arguments.begin(); 
            iter != foundInstruction.arguments.end(); 
            ++iter) {
        
        
        switch (*iter.base()) {
            case Reg:
                {
                    if(!in->eat('$')) {
                        printf("err: expected a register\n");
                        std::exit(1);
                        return false;
                    }
                    
                    std::string registerName = readWord(in);
                    if(registerName.empty()) {
                        printf("err: expected a register name\n");
                        std::exit(1);
                        return false;
                    }
        
                    bool registerFound = false;
                    Instruction foundInstruction;
                    for(std::map<std::string, uint8_t>::iterator iter = registers.begin(); 
                            iter != registers.end(); 
                            ++iter) {
                        if(iter->first == registerName) {
                            registerFound = true;
                            break;
                        }
                    }
                    
                    if(!registerFound) {
                        printf("err: unknown register\n");
                        std::exit(1);
                        return false;
                    }
                    
                    InstructionArg arg;
                    arg.type = REGISTER;
                    arg.registerName = registerName;
                    section.instruction.args.push_back(arg);
                }
                break;
            case Imm:
                {
                    Literal literal = parseLiteral();
                }
                break;
            case Mem:
                break;
            case Imm8:
                break;
            case Imm16:
                break;
            case Imm32:
                break;
        }
    }
    
    output->sections.push_back(section);
    
    return false;
}

Literal smallestFittingType(uint32_t num) {
    if(num < 0x100) {
        return Literal( U8, {(uint8_t)num} );
    } else if(num < 0x10000) {
        return { U16 };
    } else {
        return { U32 };
    }
}

Literal parseLiteral(EatableString *in) {
    
    // H E X
    if(*in->c == '0' && *(in->c + 1) == 'x') {
        uint32_t integerValue = 0;
        
        in->nextChar();
        in->nextChar();
        
        while((*in->c >= '0' && *in->c <= '9') || (*in->c >= 'a' && *in->c <= 'f') || (*in->c >= 'A' && *in->c <= 'F')) {
            integerValue <<= 4;
            if(*in->c >= '0' && *in->c <= '9') {
                integerValue += *in->c - '0';
            }
            if(*in->c >= 'a' && *in->c <= 'f') {
                integerValue += *in->c - 'a' + 10;
            }
            if(*in->c >= 'A' && *in->c <= 'F') {
                integerValue += *in->c - 'A' + 10;
            }
            
            in->nextChar();
        }
        
        return { smallestFittingType(integerValue), {  } };
    } else 
    
    // B I N A R Y
    if(*in->c == '0' && *(in->c + 1) == 'b') {
        uint32_t integerValue = 0;
        
        in->nextChar();
        in->nextChar();
        
        while(*in->c >= '0' && *in->c <= '1') {
            integerValue <<= 1;
            integerValue += *in->c - '0';
            
            in->nextChar();
        }
    
        return (new Node("literal"))
                ->append("type", "integer")
                ->append("value", (uint32_t)integerValue);
    } else 
    
    // C H A R A C T E R
    if(*in->c == '\'') {
        uint32_t integerValue = 0;
        
        in->nextChar();
        integerValue = *in->c;
        in->nextChar();
        
        if(!in->eat('\'')) ERR("expected ' after character literal");
    
        return (new Node("literal"))
                ->append("type", "integer")
                ->append("value", (uint32_t)integerValue);
    } else 
    
    // S T R I N G
    if(*in->c == '"') {
        in->nextChar();
        
        std::string stringValue;
        
        while(*in->c != '"') {
            if(*in->c == '\\') {
                in->nextChar();
                switch (*in->c) {
                    case '\a':
                        stringValue += 0x07;
                        break;
                    case '\b':
                        stringValue += 0x08;
                        break;
                    case '\e':
                        stringValue += 0x1B;
                        break;
                    case '\f':
                        stringValue += 0x0C;
                        break;
                    case '\n':
                        stringValue += 0x0A;
                        break;
                    case '\r':
                        stringValue += 0x0D;
                        break;
                    case '\t':
                        stringValue += 0x09;
                        break;
                    case '\v':
                        stringValue += 0x0B;
                        break;
                    default:
                        stringValue += *in->c;
                        break;
                }
            } else {
                stringValue += *in->c;
            }
            in->nextChar();
        }
        
        if(!in->eat('"')) ERR("expected \" after string literal");
    
        return (new Node("literal"))
                ->append("type", "string")
                ->append("value", stringValue);
    } else 
    
    // I N T E G E R   &   F L O A T
    if(*in->c >= '0' && *in->c <= '9') {
        uint32_t integerValue = 0;
        float floatValue = 0;
        
        bool isFloat = false;
        
        while(*in->c >= '0' && *in->c <= '9') {
            integerValue += *in->c - '0';
            integerValue *= 10;
            
            in->nextChar();
        }
        
        if(*in->c == '.') {
            isFloat = true;
            float fractionPart = 0;
            floatValue = integerValue;
        
            while(*in->c >= '0' && *in->c <= '9') {
                fractionPart /= 10.0;
                fractionPart += (*in->c - '0') / 10.0;
                
                in->nextChar();
            }
            
            floatValue += fractionPart;
    
            return (new Node("literal"))
                    ->append("type", "float")
                    ->append("value", floatValue);
        }
    
        return (new Node("literal"))
                ->append("type", "integer")
                ->append("value", (uint32_t)integerValue);
    }
}


// Instruction components
uint8_t parseRegisters(EatableString *in) {
    return 0;
}