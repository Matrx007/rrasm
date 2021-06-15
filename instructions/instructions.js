#!/bin/node

String.prototype.zeroPad = function(size) {
    let str = String(this);
    while (str.length < (size || 2)) {str = "0" + str;}
    return str;
}

const Arg = {
    Reg: 0,
    Imm: 1,
    Mem: 2,
    
    Imm8: 3,
    Imm16: 4,
    Imm32: 5
}

const instructions = [
    [`HALT`,   [], `Stop execution `],
    
    [`MOV`,    [Arg.Reg, [Arg.Reg, Arg.Imm]], `Rd = S`],
    // [`MOV`,    [Arg.Reg, Arg.Reg], `Rd = Rs`],
    // [`MVI`,    [Arg.Reg, Arg.Imm], `Rd = I`],
    
    [`LDA`,    [Arg.Reg, Arg.Mem], `Rd = Ms`],
    [`STA`,    [Arg.Mem, [Arg.Reg, Arg.Imm8]], `Md = Rs`],
    
    [`ADD`,    [Arg.Reg, [Arg.Reg, Arg.Imm]], `Rd = Rd + S`],
    // [`ADD`,    [Arg.Reg, Arg.Reg], `Rd = Rd + Rs`],
    // [`ADDI`,   [Arg.Reg, Arg.Imm], `Rd = Rd + I`],
    [`SUB`,    [Arg.Reg, [Arg.Reg, Arg.Imm]], `Rd = Rd - S`],
    // [`SUB`,    [Arg.Reg, Arg.Reg], `Rd = Rd - Rs`],
    // [`SUBI`,   [Arg.Reg, Arg.Imm], `Rd = Rd - I`],
    [`MUL`,    [Arg.Reg, [Arg.Reg, Arg.Imm]], `Rd = Rd * S`],
    // [`MUL`,    [Arg.Reg, Arg.Reg], `Rd = Rd * Rs`],
    // [`MULI`,   [Arg.Reg, Arg.Imm], `Rd = Rd * I`],
    [`OR`,     [Arg.Reg, [Arg.Reg, Arg.Imm]], `Rd = Rd | S`],
    // [`OR`,     [Arg.Reg, Arg.Reg], `Rd = Rd | Rs`],
    // [`ORI`,    [Arg.Reg, Arg.Imm], `Rd = Rd | I`],
    [`AND`,    [Arg.Reg, [Arg.Reg, Arg.Imm]], `Rd = Rd & S`],
    // [`AND`,    [Arg.Reg, Arg.Reg], `Rd = Rd & Rs`],
    // [`ANDI`,   [Arg.Reg, Arg.Imm], `Rd = Rd & I`],
    [`XOR`,    [Arg.Reg, [Arg.Reg, Arg.Imm]], `Rd = Rd ^ S`],
    // [`XOR`,    [Arg.Reg, Arg.Reg], `Rd = Rd ^ Rs`],
    // [`XORI`,   [Arg.Reg, Arg.Imm], `Rd = Rd ^ I`],
    
    [`NOT`,    [Arg.Reg], `Rd = !Rd`],
    
    [`INC`,    [Arg.Reg], `Rd = Rd + 1`],
    [`DEC`,    [Arg.Reg], `Rd = Rd - 1`],
    
    [`J`,      [Arg.Mem], `Jump to`],
    [`JZ`,     [Arg.Mem], `Jump to if zero-flag is 1`],
    [`JNZ`,    [Arg.Mem], `Jump to if zero-flag is 0`],
    
    [`JF`,     [Arg.Imm], `Jump forward `],
    [`JFZ`,    [Arg.Imm], `Jump forward if zero-flag is 1`],
    [`JFNZ`,   [Arg.Imm], `Jump forward if zero-flag is 0`],
    
    [`JB`,     [Arg.Imm], `Jump backward `],
    [`JBZ`,    [Arg.Imm], `Jump backward if zero-flag is 1`],
    [`JBNZ`,   [Arg.Imm], `Jump backward if zero-flag is 0`]
];

const registers = [
    // name, size in bytes
    [`a`,       1] ,
    [`b`,       1],
    [`c`,       1],
    [`d`,       1],
    [`x`,       1],
    [`y`,       1],
    [`z`,       1],
    [`w`,       1],
    [`i`,       2],
    [`j`,       2],
    [`k`,       2],
    [`l`,       2],
    [`ax`,      2],
    [`by`,      2],
    [`cz`,      2],
    [`dw`,      2],
    [`axi`,     4],
    [`byj`,     4],
    [`czk`,     4],
    [`dwl`,     4],
    
    [`pc`,      4],
    [`mar`,     4],
    [`mdr`,     4],
    [`sp`,      4],
    
    [`fc`,      2]
];

function findAllCombinations(arr) {
    let results = [];
    
    function gatherResults(finalArrayIndex, currentIndexes) {
        let row = [];
        for(let i = 0; i < finalArrayIndex; i++) {
            row.push(arr[i][currentIndexes[i]]);
        }
        results.push(row);
    };
    
    function nestedForLoop(arrayIndex, currentIndexes) {
        for(let i = 0; i < arr[arrayIndex].length; i++) {
            if(arrayIndex == arr.length-1) {
                gatherResults(arrayIndex+1, [...currentIndexes, i]);
            } else {
                nestedForLoop(arrayIndex+1, [...currentIndexes, i]);   
            }
        }   
    };
    
    nestedForLoop(0, []);
    
    return results;
}

let microInstructions = [];

instructions.forEach((instruction) => {
    if(instruction[1].length == 0) {
        microInstructions.push(instruction);
    } else {
        let possibilities = [];
        
        instruction[1].forEach((arg) => {
            if(Array.isArray(arg)) {
                let expandedPossibilities = [];
                
                arg.forEach((argPossibility) => {
                    switch (argPossibility) {
                        case Arg.Imm:
                            expandedPossibilities.push(Arg.Imm8, Arg.Imm16, Arg.Imm32);
                            break;
                        default:
                            expandedPossibilities.push(argPossibility);
                            break;
                    }
                });
                
                possibilities.push(expandedPossibilities);
            } else {
                switch (arg) {
                    case Arg.Imm:
                        possibilities.push([Arg.Imm8, Arg.Imm16, Arg.Imm32]);
                        break;
                    default:
                        possibilities.push([arg]);
                        break;
                }
            }
        });
        
        let results = findAllCombinations(possibilities);
        
        results.forEach((result) => {
            microInstructions.push([instruction[0], result, instruction[2]]); 
        });
    }
});

// ################################
// ### PRINT MICRO-INSTRUCTIONS ###
// ################################
    
console.log("##############################");
console.log("### CPU micro-instructions ###");
console.log("##############################");

microInstructions.forEach((microInstruction, opcode) => {
    let args = '';
    let __isFirstArg = true;
    
    microInstruction[1].forEach((arg) => {
        if(__isFirstArg) {
            __isFirstArg = false;
        } else {
            args += ', ';
        }
        
        switch (arg) {
            case Arg.Reg:       args += 'Reg';      break;
            case Arg.Imm:       args += 'Imm';      break;
            case Arg.Mem:       args += 'Mem';      break;
            case Arg.Imm8:      args += 'Imm8';     break;
            case Arg.Imm16:     args += 'Imm16';    break;
            case Arg.Imm32:     args += 'Imm32';    break;
        }
    });
    
    console.log("// "+microInstruction[2]);
    console.log(`#define INST_${microInstruction[0].padEnd(10)} 0x${opcode.toString(16).zeroPad(3)} // args: [${args}]`);
});


    
console.log("#################################");
console.log("### Assembler instruction map ###");
console.log("#################################");

microInstructions.forEach((microInstruction, opcode) => {
    let args = '';
    let __isFirstArg = true;
    
    microInstruction[1].forEach((arg) => {
        if(__isFirstArg) {
            __isFirstArg = false;
        } else {
            args += ', ';
        }
        
        switch (arg) {
            case Arg.Reg:       args += 'Arg::Reg';      break;
            case Arg.Imm:       args += 'Arg::Imm';      break;
            case Arg.Mem:       args += 'Arg::Mem';      break;
            case Arg.Imm8:      args += 'Arg::Imm8';     break;
            case Arg.Imm16:     args += 'Arg::Imm16';    break;
            case Arg.Imm32:     args += 'Arg::Imm32';    break;
        }
    });
    
    console.log("// "+microInstruction[2]);
    console.log(`{"${microInstruction[0]}", Instruction(0x${opcode.toString(16).zeroPad(3)}, {${args}})},`);
});

