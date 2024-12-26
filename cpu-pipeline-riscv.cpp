#include <bits/stdc++.h>
using namespace std;

class Instruction
{
public:
    string opcode;
    string func3;
    string func7;

    Instruction(string opc, string f3 = "", string f7 = "") : opcode(opc), func3(f3), func7(f7) {}

    virtual void toBinaryArray(bitset<32> &instruction) const = 0;
};

int regToNum(const string &reg)
{
    return stoi(reg.substr(1));
}

int imm1ToNum(const string &imm1)
{
    int value = stoi(imm1, nullptr, 0);
    // Sign-extend if negative
    if (value < 0)
        value |= 0xFFFFFFFF << 20; // Adjust bit length as needed
    return value;
}

class Rtype : public Instruction
{
public:
    string rd, rs1, rs2;
    Rtype(string opc, string f3, string f7, string rd, string rs1, string rs2)
        : Instruction(opc, f3, f7), rd(rd), rs1(rs1), rs2(rs2) {}

    void toBinaryArray(bitset<32> &instruction) const override
    {
        instruction = bitset<32>(0x00000000);
        instruction |= bitset<32>(stoi(opcode, nullptr, 2)) << 0;
        instruction |= bitset<32>(regToNum(rd)) << 7;
        instruction |= bitset<32>(stoi(func3, nullptr, 2)) << 12;
        instruction |= bitset<32>(regToNum(rs1)) << 15;
        instruction |= bitset<32>(regToNum(rs2)) << 20;
        instruction |= bitset<32>(stoi(func7, nullptr, 2)) << 25;
    }
};

class Itype : public Instruction
{
public:
    string rd, rs1, imm1;
    Itype(string opc, string f3, string rd, string rs1, string imm1)
        : Instruction(opc, f3), rd(rd), rs1(rs1), imm1(imm1) {}

    void toBinaryArray(bitset<32> &instruction) const override
    {
        instruction = bitset<32>(0x00000000);
        instruction |= bitset<32>(stoi(opcode, nullptr, 2)) << 0;
        instruction |= bitset<32>(regToNum(rd)) << 7;
        instruction |= bitset<32>(stoi(func3, nullptr, 2)) << 12;
        instruction |= bitset<32>(regToNum(rs1)) << 15;
        instruction |= bitset<32>(imm1ToNum(imm1)) << 20;
    }
};

class Btype : public Instruction
{
public:
    string rs1, rs2, imm1;
    Btype(string opc, string f3, string rs1, string rs2, string imm1) : Instruction(opc, f3), rs1(rs1), rs2(rs2), imm1(imm1) {}

    void toBinaryArray(bitset<32> &instruction) const override
    {
        instruction = bitset<32>(0x00000000);
        int imm1Val = imm1ToNum(imm1);
        int imm111 = (imm1Val >> 11) & 0x1;    // imm1ediate[11]
        int imm14to1 = (imm1Val >> 1) & 0xF;   // imm1ediate[4:1]
        int imm110to5 = (imm1Val >> 5) & 0x3F; // imm1ediate[10:5]
        int imm112 = (imm1Val >> 12) & 0x1;    // imm1ediate[12] (sign bit)

        instruction |= bitset<32>(stoi(opcode, nullptr, 2)) << 0;
        instruction |= bitset<32>(imm14to1) << 8;
        instruction |= bitset<32>(imm111) << 7;
        instruction |= bitset<32>(stoi(func3, nullptr, 2)) << 12;
        instruction |= bitset<32>(regToNum(rs1)) << 15;
        instruction |= bitset<32>(regToNum(rs2)) << 20;
        instruction |= bitset<32>(imm110to5) << 25;
        instruction |= bitset<32>(imm112) << 31;
    }
};

class Jtype : public Instruction
{
public:
    string rd, imm1;
    Jtype(string opc, string rd, string imm1) : Instruction(opc), rd(rd), imm1(imm1) {}

    void toBinaryArray(bitset<32> &instruction) const override
    {
        instruction = bitset<32>(0x00000000);
        int imm1Val = imm1ToNum(imm1);
        int imm20 = (imm1Val >> 20) & 0x1;       // imm1ediate[20]
        int imm110to1 = (imm1Val >> 1) & 0x3FF;  // imm1ediate[10:1]
        int imm111 = (imm1Val >> 11) & 0x1;      // imm1ediate[11]
        int imm119to12 = (imm1Val >> 12) & 0xFF; // imm1ediate[19:12]

        instruction |= bitset<32>(stoi(opcode, nullptr, 2)) << 0;
        instruction |= bitset<32>(regToNum(rd)) << 7;
        instruction |= bitset<32>(imm119to12) << 12;
        instruction |= bitset<32>(imm111) << 20;
        instruction |= bitset<32>(imm110to1) << 21;
        instruction |= bitset<32>(imm20) << 31;
    }
};

class Utype : public Instruction
{
public:
    string rd, imm1;
    Utype(string opc, string rd, string imm1) : Instruction(opc), rd(rd), imm1(imm1) {}

    void toBinaryArray(bitset<32> &instruction) const override
    {
        instruction = bitset<32>(0x00000000);
        instruction |= bitset<32>(stoi(opcode, nullptr, 2)) << 0;
        instruction |= bitset<32>(regToNum(rd)) << 7;
        instruction |= bitset<32>(imm1ToNum(imm1)) << 12;
    }
};

class Stype : public Instruction
{
public:
    string rs1, rs2, imm1;
    Stype(string opc, string f3, string rs1, string rs2, string imm1)
        : Instruction(opc, f3), rs1(rs1), rs2(rs2), imm1(imm1) {}

    void toBinaryArray(bitset<32> &instruction) const override
    {
        instruction = bitset<32>(0x00000000);
        int imm1Val = imm1ToNum(imm1);
        int imm1Hi = (imm1Val >> 5) & 0x7F; // High 7 bits
        int imm1Lo = imm1Val & 0x1F;        // Low 5 bits

        instruction |= bitset<32>(stoi(opcode, nullptr, 2)) << 0;
        instruction |= bitset<32>(imm1Lo) << 7;
        instruction |= bitset<32>(stoi(func3, nullptr, 2)) << 12;
        instruction |= bitset<32>(regToNum(rs1)) << 15;
        instruction |= bitset<32>(regToNum(rs2)) << 20;
        instruction |= bitset<32>(imm1Hi) << 25;
    }
};

class Nop : public Itype
{
public:
    Nop() : Itype("0010011", "000", "x0", "x0", "0") {}
};

class assembler
{
public:
    assembler()
    {
        opcodeMap["ADD"] = "0110011";
        opcodeMap["SUB"] = "0110011";
        opcodeMap["MUL"] = "0110011";
        opcodeMap["DIV"] = "0110011";
        opcodeMap["REM"] = "0110011";
        opcodeMap["SLT"] = "0110011";
        opcodeMap["SLTU"] = "0110011";
        opcodeMap["AND"] = "0110011";
        opcodeMap["OR"] = "0110011";
        opcodeMap["XOR"] = "0110011";
        opcodeMap["SLL"] = "0110011";
        opcodeMap["SRL"] = "0110011";
        opcodeMap["SRA"] = "0110011";
        opcodeMap["ADDI"] = "0010011";
        opcodeMap["SLTI"] = "0010011";
        opcodeMap["SLTIU"] = "0010011";
        opcodeMap["ANDI"] = "0010011";
        opcodeMap["ORI"] = "0010011";
        opcodeMap["XORI"] = "0010011";
        opcodeMap["SLLI"] = "0010011";
        opcodeMap["SRLI"] = "0010011";
        opcodeMap["SRAI"] = "0010011";
        opcodeMap["LW"] = "0000011";
        opcodeMap["SW"] = "0100011";
        opcodeMap["LB"] = "0000011";
        opcodeMap["LH"] = "0000011";
        opcodeMap["LBU"] = "0000011";
        opcodeMap["LHU"] = "0000011";
        opcodeMap["SB"] = "0100011";
        opcodeMap["SH"] = "0100011";
        opcodeMap["BEQ"] = "1100011";
        opcodeMap["BNE"] = "1100011";
        opcodeMap["BLT"] = "1100011";
        opcodeMap["BLTU"] = "1100011";
        opcodeMap["BGE"] = "1100011";
        opcodeMap["BGEU"] = "1100011";
        opcodeMap["JAL"] = "1101111";
        opcodeMap["JALR"] = "1100111";
        opcodeMap["LUI"] = "0110111";
        opcodeMap["AUIPC"] = "0010111";
        opcodeMap["NOP"] = "0010011";

        // Func3 Map
        func3Map["ADD"] = "000";
        func3Map["SUB"] = "000";
        func3Map["MUL"] = "000";
        func3Map["DIV"] = "100";
        func3Map["REM"] = "110";
        func3Map["SLT"] = "010";
        func3Map["SLTU"] = "011";
        func3Map["AND"] = "111";
        func3Map["OR"] = "110";
        func3Map["XOR"] = "100";
        func3Map["SLL"] = "001";
        func3Map["SRL"] = "101";
        func3Map["SRA"] = "101";
        func3Map["ADDI"] = "000";
        func3Map["SLTI"] = "010";
        func3Map["SLTIU"] = "011";
        func3Map["ANDI"] = "111";
        func3Map["ORI"] = "110";
        func3Map["XORI"] = "100";
        func3Map["SLLI"] = "001";
        func3Map["SRLI"] = "101";
        func3Map["SRAI"] = "101";
        func3Map["LW"] = "010";
        func3Map["SW"] = "010";
        func3Map["LB"] = "000";
        func3Map["LH"] = "001";
        func3Map["LBU"] = "100";
        func3Map["LHU"] = "101";
        func3Map["SB"] = "000";
        func3Map["SH"] = "001";
        func3Map["BEQ"] = "000";
        func3Map["BNE"] = "001";
        func3Map["BLT"] = "100";
        func3Map["BLTU"] = "110";
        func3Map["BGE"] = "101";
        func3Map["BGEU"] = "111";
        func3Map["JALR"] = "000";

        func7Map["ADD"] = "0000000";
        func7Map["SUB"] = "0100000";
        func7Map["MUL"] = "0000001";
        func7Map["DIV"] = "0000001";
        func7Map["REM"] = "0000001";
        func7Map["SLT"] = "0000000";
        func7Map["SLTU"] = "0000000";
        func7Map["AND"] = "0000000";
        func7Map["OR"] = "0000000";
        func7Map["XOR"] = "0000000";
        func7Map["SLL"] = "0000000";
        func7Map["SRL"] = "0000000";
        func7Map["SRA"] = "0100000";
        func7Map["NOP"] = "0000000";
    }

    Instruction *parseInstruction(const string &instruction)
    {
        string opcode, func3, func7, subins;

        // Extract the first field (sub-instruction) which is typically the instruction type or name
        int spacePos = instruction.find(' ');
        subins = instruction.substr(0, spacePos);

        // Check if the subins is a valid opcode in the map
        if (opcodeMap.find(subins) != opcodeMap.end())
        {
            opcode = opcodeMap[subins];
            func3 = func3Map[subins];
            func7 = func7Map[subins];

            // Handle NOP (No operation) instruction
            if (subins == "NOP")
            {
                return parseNOP();
            }

            // Handle R-type instruction
            if (opcode == "0110011")
            {
                return parseRtype(opcode, func3, func7, instruction);
            }
            // Handle I-type, L-type, JALR assemblyLang
            else if (opcode == "0010011" || opcode == "0000011" || opcode == "1100111")
            {
                return parseItype(opcode, func3, instruction);
            }
            // Handle S-type instruction
            else if (opcode == "0100011")
            {
                return parseStype(opcode, func3, instruction);
            }
            // Handle B-type instruction
            else if (opcode == "1100011")
            {
                return parseBtype(opcode, func3, instruction);
            }
            // Handle J-type instruction
            else if (opcode == "1101111" || opcode == "1100111")
            {
                return parseJtype(opcode, instruction);
            }
            // Handle U-type instruction
            else if (opcode == "0110111" || opcode == "0010111")
            {
                return parseUtype(opcode, instruction);
            }
        }

        return nullptr;
    }

    unordered_map<string, string> opcodeMap;
    unordered_map<string, string> func3Map;
    unordered_map<string, string> func7Map;

    vector<string> splitInstruction(const string &instruction)
    {
        vector<string> parts;
        int start = 0, end = instruction.find(' ');
        while (end != string::npos)
        {
            parts.push_back(instruction.substr(start, end - start));
            start = end + 1;
            end = instruction.find(' ', start);
        }
        parts.push_back(instruction.substr(start));
        return parts;
    }

    Rtype *parseRtype(const string &opcode, const string &func3, const string &func7, const string &instruction)
    {
        vector<string> parts = splitInstruction(instruction);
        // Assuming subins, rd, rs1, rs2 are in the expected positions
        string subins = parts[0], rd = parts[1], rs1 = parts[2], rs2 = parts[3];
        return new Rtype(opcode, func3, func7, rd, rs1, rs2);
    }

    Itype *parseItype(const string &opcode, const string &func3, const string &instruction)
    {
        vector<string> parts = splitInstruction(instruction);
        string subins = parts[0], rd = parts[1], rs1 = parts[2], imm1 = parts[3];
        return new Itype(opcode, func3, rd, rs1, imm1);
    }

    Stype *parseStype(const string &opcode, const string &func3, const string &instruction)
    {
        vector<string> parts = splitInstruction(instruction);
        string subins = parts[0], rs2 = parts[1], rs1 = parts[2], imm1 = parts[3];
        return new Stype(opcode, func3, rs1, rs2, imm1);
    }

    Btype *parseBtype(const string &opcode, const string &func3, const string &instruction)
    {
        vector<string> parts = splitInstruction(instruction);
        string subins = parts[0], rs1 = parts[1], rs2 = parts[2], imm1 = parts[3];
        return new Btype(opcode, func3, rs1, rs2, imm1);
    }

    Jtype *parseJtype(const string &opcode, const string &instruction)
    {
        vector<string> parts = splitInstruction(instruction);
        string subins = parts[0], rd = parts[1], imm1 = parts[2];
        return new Jtype(opcode, rd, imm1);
    }

    Utype *parseUtype(const string &opcode, const string &instruction)
    {
        vector<string> parts = splitInstruction(instruction);
        string subins = parts[0], rd = parts[1], imm1 = parts[2];
        return new Utype(opcode, rd, imm1);
    }

    Nop *parseNOP()
    {
        return new Nop();
    }
};

// CPUPipelineProcessing

class PC
{
public:
    int Value;
    bool Valid;

    PC(int pc = 0, bool valid = false)
    {
        Value = pc;
        Valid = valid;
    }
};

class IFID
{
public:
    int DPC, NPC;
    string IR;
    bool Stall;
    bool Valid;

    IFID(int dpc = 0, int npc = 0, string ir = "", bool stall = false, bool valid = false)
    {
        DPC = dpc;
        NPC = npc;
        IR = ir;
        Stall = stall;
        Valid = valid;
    }
};

struct PC2
{
    int Dpc;
    int Jpc;
};

struct ControlWord
{
    bool RegRead;
    bool ALUSrc;
    bool RegWrite;
    bool MemRead;
    bool MemWrite;
    bool Mem2Reg;
    bool Branch;
    bool Jump;
    int ALUOP;

    ControlWord() : RegRead(false), ALUSrc(false), RegWrite(false), MemRead(false), MemWrite(false), Mem2Reg(false), Branch(false), Jump(false), ALUOP(0)
    {
    }

    ControlWord(bool alusrc, bool regRead, bool regWrite, int aluop, bool mem2Reg, bool memRead, bool memWrite, bool branch, bool jump)
        : ALUSrc(alusrc), RegRead(regRead), RegWrite(regWrite), Mem2Reg(mem2Reg), MemRead(memRead), MemWrite(memWrite), Branch(branch), Jump(jump), ALUOP(aluop)
    {
    }
};

class IDEX
{
public:
    PC2 pc2;
    int imm1;
    string imm2;
    string FUNC;
    int RS1;
    int RS2;
    int RS22;
    int RDL;
    ControlWord CW;
    bool Stall;
    bool Valid;

    IDEX(int dpc = 0, int jpc = 0, int imm1 = 0, string func = "", int rs1 = 0, int rs2 = 0, int rs22 = 0, string imm2 = "", int rdl = 0, ControlWord cw = {}, bool stall = false, bool valid = false)
    {
        pc2.Dpc = dpc;
        pc2.Jpc = jpc;
        imm1 = imm1;
        imm2 = imm2;
        FUNC = func;
        RS1 = rs1;
        RS2 = rs2;
        RS22 = rs22;
        RDL = rdl;
        CW = cw;
        Stall = stall;
        Valid = valid;
    }
};

class EXMO
{
public:
    int ALUOUT;
    int RS2;
    int RDL;
    ControlWord CW;
    bool Stall;
    bool Valid;

    EXMO(int aluout = 0, int rs2 = 0, int rdl = 0, ControlWord cw = {}, bool stall = false, bool valid = false)
    {
        ALUOUT = aluout;
        RS2 = rs2;
        CW = cw;
        RDL = rdl;
        Stall = stall;
        Valid = valid;
    }
};

class MOWB
{
public:
    int LDOUT, ALUOUT, RDL;
    ControlWord CW;
    bool Stall;
    bool Valid;

    MOWB(int ldout = 0, int aluout = 0, int rdl = 0, ControlWord cw = {}, bool stall = false, bool valid = false)
    {
        LDOUT = ldout;
        ALUOUT = aluout;
        RDL = rdl;
        CW = cw;
        Stall = stall;
        Valid = valid;
    }
};

int running = 1;

IFID ifid;
IDEX idex;
EXMO exmo;
MOWB mowb;
PC pc;

vector<string> InstructionMemory;

struct Registers
{
    int valid = 0;
    int value = 0;
};
vector<Registers> GPR(32);

vector<int> DM(1024, 0);

void fetch()
{
    if (ifid.Stall || !pc.Valid)
    {
        ifid.Valid = false;
        return;
    }

    if (pc.Value >= InstructionMemory.size())
    {
        ifid.Valid = false;
        pc.Valid = false;
        running = 0;
        return;
    }
    else
    {
        ifid.IR = InstructionMemory[pc.Value];
        ifid.DPC = pc.Value;
        ifid.NPC = pc.Value + 1;
        ifid.Valid = true;
        pc.Value = pc.Value + 1;
    }
}

int signedExtend(const string &str)
{
    bool isNegative = (str[0] == '1');
    int value = stoi(str, nullptr, 2);

    if (isNegative)
    {
        int numBits = str.size();
        value -= (1 << numBits);
    }

    return value;
}

pair<ControlWord, string> controller(string opcode)
{
    // cw ALUSrc,regRead, regWrite, ALUOP,mem2Reg,memRead,memWrite,branch,jump

    if (opcode == "0110011") // R-type
    {
        return {ControlWord(false, true, true, 2, false, false, false, false, false), "R"};
    }
    else if (opcode == "0010011") // I-Type
    {
        return {ControlWord(true, true, true, 3, false, false, false, false, false), "I"};
    }
    else if (opcode == "0000011") // L-type
    {
        return {ControlWord(true, true, true, 0, true, true, false, false, false), "L"};
    }
    else if (opcode == "0100011") // S-type
    {
        return {ControlWord(true, true, false, 0, false, false, true, false, false), "S"};
    }
    else if (opcode == "1100011") // B-type
    {
        return {ControlWord(false, true, false, 1, false, false, false, true, false), "B"};
    }
    else if (opcode == "1101111") // j-type
    {
        return {ControlWord(true, true, true, 0, false, false, false, false, true), "J"};
    }

    return {ControlWord(false, false, false, 0, false, false, false, false, false), "NA"};
}

int genImm(const string &ir, const string &opcode)
{
    int imm1 = INT_MIN;
    if (opcode == "0010011" || opcode == "0000011")
    {
        string temp = ir.substr(0, 12);
        imm1 = signedExtend(temp);
    }
    else if (opcode == "0100011")
    {
        string temp = ir.substr(0, 7) + ir.substr(20, 5);
        imm1 = signedExtend(temp);
    }
    else if (opcode == "1100011")
    {
        string temp = ir.substr(0, 1) + ir.substr(24, 1) + ir.substr(1, 6) + ir.substr(20, 4);
        imm1 = signedExtend(temp);
    }

    return imm1;
}

void decode()
{
    if (idex.Stall || !ifid.Valid)
    {
        idex.Valid = false;
        return;
    }

    string opcode = ifid.IR.substr(25, 7);
    auto res = controller(opcode);
    idex.CW = res.first;
    string format = res.second;

    idex.pc2.Dpc = ifid.DPC;

    idex.imm1 = genImm(ifid.IR, opcode);

    idex.FUNC = ifid.IR.substr(17, 3);
    idex.imm2 = ifid.IR.substr(0, 7);

    string rdl = ifid.IR.substr(20, 5);
    idex.RDL = stoi(rdl, 0, 2);

    string rsl1 = ifid.IR.substr(12, 5);
    int rl1 = stoi(rsl1, 0, 2);

    string rsl2 = ifid.IR.substr(7, 5);
    int rl2 = stoi(rsl2, 0, 2);

    ifid.Stall = false;

    if (idex.CW.RegRead)
    {
        if (GPR[rl1].valid == 0)
            idex.RS1 = GPR[rl1].value;
        else
        {
            ifid.Stall = true;
            idex.Valid = false;
            return;
        }
    }

    if (idex.CW.ALUSrc)
    {
        if (idex.CW.RegRead)
        {
            idex.RS2 = idex.imm1;
            if (GPR[rl2].valid == 0)
                idex.RS22 = GPR[rl2].value;
            else
            {
                ifid.Stall = true;
                idex.Valid = false;
                return;
            }
        }
    }
    else if (idex.CW.RegRead)
    {
        if (GPR[rl2].valid == 0)
            idex.RS2 = GPR[rl2].value;
        else
        {
            ifid.Stall = true;
            idex.Valid = false;
            return;
        }
    }

    if (opcode != "0100011" && opcode != "1100011")
        GPR[idex.RDL].valid += 1;

    ifid.Stall = false;
    idex.Valid = true;
}

string ALUControl(string func7, string func3, int ALUOP)
{
    if (ALUOP == 0)
    {
        return "0010";
    }
    else if (ALUOP == 1)
    {
        return "0110";
    }
    else if (ALUOP == 2)
    {
        if (func7 == "0000000")
        {
            if (func3 == "111")
                return "0000";
            if (func3 == "110")
                return "0001";
            return "0010";
        }
        else if (func7 == "0100000")
        {
            return "0110";
        }
        else if (func7 == "0000001")
        {
            if (func3 == "000")
            {
                return "1000";
            }
            if (func3 == "100")
            {
                return "1100";
            }
            if (func3 == "110")
            {
                return "1110";
            }
        }
    }
    else if (ALUOP == 3)
    {
        if (func3 == "000")
            return "0010";
        if (func3 == "111")
            return "0000";
        if (func3 == "110")
            return "0001";
    }
    return "0000";
}

int ALU(const string &ALUSelect, int rs1, int rs2)
{
    if (ALUSelect == "0000")
        return rs1 & rs2;
    if (ALUSelect == "0001")
        return rs1 | rs2;
    if (ALUSelect == "0010")
        return rs1 + rs2;
    if (ALUSelect == "1000")
        return rs1 * rs2;
    if (ALUSelect == "1100")
        return rs1 / rs2;
    if (ALUSelect == "1110")
        return rs1 % rs2;

    return rs1 - rs2;
}

int ALUFLAG(int a, int b, const string &s)
{
    unsigned ua = a, ub = b;

    if ((s == "000" && a == b) || (s == "001" && a != b) || (s == "100" && a < b) || (s == "101" && a >= b) || (s == "110" && ua < ub) || (s == "111" && ua >= ub))
        return 1;

    return 0;
}

void execute()
{
    if (exmo.Stall || !idex.Valid)
    {
        exmo.Valid = false;
        return;
    }

    string AluSelect = ALUControl(idex.imm2, idex.FUNC, idex.CW.ALUOP);
    exmo.ALUOUT = ALU(AluSelect, idex.RS1, idex.RS2);

    int AluZeroFlag = ALUFLAG(idex.RS1, idex.RS2, idex.FUNC);

    exmo.CW = idex.CW;
    exmo.RDL = idex.RDL;
    exmo.RS2 = idex.RS22;

    if (idex.CW.Branch && AluZeroFlag)
    {
        pc.Value = ((idex.imm1 << 1)) / 4 + idex.pc2.Dpc;
        ifid.Valid = false;
        idex.Valid = false;
        pc.Valid = true;        
    }

    if (idex.CW.Jump)
    {
        pc.Value = idex.pc2.Jpc;
        ifid.Valid = false;
        idex.Valid = false;
        pc.Valid = true;
    }

    idex.Stall = false;
    exmo.Valid = true;
}

void memoryOperation()
{
    if (mowb.Stall || !exmo.Valid)
    {
        mowb.Valid = false;
        return;
    }

    if (exmo.CW.MemWrite)
    {
        DM[exmo.ALUOUT] = exmo.RS2;
        cout << " DM[" << exmo.ALUOUT << "] =  " << DM[exmo.ALUOUT] << endl;
    }
    if (exmo.CW.MemRead)
        mowb.LDOUT = DM[exmo.ALUOUT];

    mowb.ALUOUT = exmo.ALUOUT;

    mowb.CW = exmo.CW;
    mowb.RDL = exmo.RDL;

    exmo.Stall = false;
    mowb.Valid = true;
}

void writeBack()
{
    if (!mowb.Valid)
        return;
    if (!mowb.CW.RegWrite)
        return;

    if (mowb.CW.Mem2Reg && GPR[mowb.RDL].valid > 0)
    {
        GPR[mowb.RDL].value = mowb.LDOUT;
        GPR[mowb.RDL].valid -= 1;
        ifid.Stall = false;
        ifid.Valid = true;
    }
    else
    {
        GPR[mowb.RDL].value = mowb.ALUOUT;
        GPR[mowb.RDL].valid -= 1;
        ifid.Stall = false;
        ifid.Valid = true;
    }
    mowb.Stall = false;
    cout << " GPR[" << mowb.RDL << "] = " << GPR[mowb.RDL].value << endl;
}

void CPUPipelineProcessing(const vector<string> &binaryInst)
{
    InstructionMemory.clear();

    for (const string &inp : binaryInst)
    {
        InstructionMemory.push_back(inp);

        // if (inp.substr(25, 7) == "1100011")
        // {
        //     InstructionMemory.push_back("00000000000000000000000000010011");
        //     InstructionMemory.push_back("00000000000000000000000000010011");
        // }
    }

    pc = PC(0, true);

    int count = 0;
    while (count < 1000)
    {
        writeBack();
        memoryOperation();
        execute();
        decode();
        fetch();
        count++;

        if (running == 0)
            break;
    }

    cout << "Clock: " << count << endl;

    for (int i = 0; i < 4 && running == 0; i++)
    {
        if (i < 4)
            writeBack();

        if (i < 3)
            memoryOperation();

        if (i < 2)
            execute();

        if (i < 1)
            decode();
    }

    cout << "Clocs: " << count + 4 << endl;

    cout << "Final GPR State: ";
    for (int i = 0; i < 32; i++)
        cout << GPR[i].value << " ";
}

int main()
{
    vector<string> assemblyLang = {
        // sum of first n numbers
        // "ADDI x15 x15 10",
        // "ADDI x16 x16 1",
        // "ADDI x17 x17 0",
        // "BNE x0 x15 4",
        // "ADD x17 x17 x15",
        // "SUB x15 x15 x16",
        // "BNE x0 x15 -8",

        // "ADDI x30 x30 0",

        // factorial
        // "ADDI x0 x0 1",
        // "ADDI x1 x1 5",
        // "ADDI x1 x1 1",
        // "ADDI x2 x2 1",
        // "BEQ x0 x1 16",
        // "MUL x2 x2 x0",
        // "ADDI x0 x0 1",
        // "BEQ x0 x0 -12",
        // "ADDI x30 x30 0",
    };

    vector<string> binaryInst;
    assembler assembler;
    for (const auto &inst : assemblyLang)
    {
        Instruction *parse = assembler.parseInstruction(inst);
        bitset<32> binaryInstruction;
        parse->toBinaryArray(binaryInstruction);

        string s = binaryInstruction.to_string();
        binaryInst.push_back(s);
        delete parse;
    }

    CPUPipelineProcessing(binaryInst);

    return 0;
}