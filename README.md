# RISC-V 5-Stage Pipeline Simulator in C++

This repository contains a **C++ implementation of a 5-stage RISC-V processor pipeline**, complete with a built-in **assembler**. The program takes human-readable RISC-V assembly language instructions, assembles them into 32-bit machine code, and then simulates their execution on a classic 5-stage pipeline (IF, ID, EX, MO, WB).

The simulator models key computer architecture concepts, including instruction decoding, pipeline registers, control signal generation, data hazard detection (stalling), and control hazard handling for branches.

---

## 📘 Overview

This project provides a functional simulation of a simple RISC-V CPU. It is divided into two main parts:

1.  **Assembler**: Parses text-based assembly instructions (e.g., `ADDI x1 x1 5`) and converts them into their corresponding 32-bit binary RISC-V machine code, correctly handling R, I, S, B, U, and J formats.
2.  **Pipeline Simulator**: Takes the binary instructions and executes them through a 5-stage pipeline, simulating the `IFID`, `IDEX`, `EXMO`, and `MOWB` pipeline registers, a General Purpose Register (GPR) file, and data memory.

---

## 🧠 Problem Statement

The goal is to simulate the execution of a RISC-V assembly program. For a given program:

1.  **Assemble**: Convert each assembly instruction into its 32-bit binary representation.
2.  **Simulate**: Execute the binary program on a 5-stage pipeline.
3.  **Handle Hazards**:
    -   **Data Hazards**: Implement stalling (by controlling the `IFID` register) when an instruction in the decode stage (ID) depends on a result from an instruction still in the execute (EX) or memory (MO) stage.
    -   **Control Hazards**: Implement flushing of the pipeline (invalidating instructions in IF/ID) when a branch or jump instruction is taken.
4.  **Produce Output**: Show the final state of the register file (GPR) and data memory (DM) after the program completes.

---

## ⚙️ Tech Stack

-   **C++**: The core language for the simulation.
-   **Standard Template Library (STL)**: For data structures like `vector`, `string`, `unordered_map`, and `bitset`.
-   **Compiler**: A standard C++ compiler like **g++** or **Clang**.
-   **Operating System**: Linux, macOS, or Windows.

---

## 🚀 Features

-   **RISC-V Assembler**: Translates R, I, S, B, U, and J-type assembly instructions to machine code.
-   **5-Stage Pipeline**: Simulates **Fetch**, **Decode**, **Execute**, **Memory**, and **Write-Back** stages.
-   **Pipeline Registers**: Models `IFID`, `IDEX`, `EXMO`, and `MOWB` registers to pass data and control signals between stages.
-   **Control Unit**: A `controller` function dynamically generates control signals (e.g., `RegWrite`, `MemRead`, `ALUSrc`) based on the instruction's opcode.
-   **ALU Simulation**: A simple ALU performs arithmetic and logical operations as directed by the `ALUControl` function.
-   **Data Hazard Detection**: The `decode` stage checks for register dependencies (`GPR[reg].valid`) and stalls the `IFID` register to prevent read-after-write (RAW) hazards.
-   **Control Hazard Handling**: Branch (`BNE`, `BEQ`) and Jump (`JAL`) instructions flush the pipeline by invalidating the `IFID` and `IDEX` registers and updating the Program Counter (PC).

---

## ▶️ How to Run

1.  **Clone the repository**
    ```bash
    git clone https://github.com/Ramcharan2905/cpu-simulation-riscv/
    cd riscv-pipeline-simulator
    ```

2.  **Add Assembly Code**
    Inside `main()`, add your RISC-V assembly instructions to the `assemblyLang` vector:
    ```cpp
    vector<string> assemblyLang = {
        "ADDI x15 x15 10",
        "ADDI x16 x16 1",
        "ADDI x17 x17 0",
        "BNE x0 x15 4",
        "ADD x17 x17 x15",
        "SUB x15 x15 x16",
        "BNE x0 x15 -8"
    };
    ```

3.  **Compile the code**
    You will need a C++ compiler. Use the following command with g++:
    ```bash
    g++ -o riscv_simulator riscv_simulator.cpp -std=c++11
    ```

4.  **Run the executable**
    ```bash
    ./riscv_simulator
    ```

---

## 📊 Results

-   **Model**: 5-Stage Pipelined RISC-V Processor
-   **Language**: C++
-   **Task**: Assembly and Simulation.

The output will show the state of Data Memory (DM) and General Purpose Registers (GPR) as they are written, followed by the total clock cycles and the final state of all registers.

---

## 📈 Learning Purpose

This project is designed to help students and developers understand:
-   The internal operation of a modern CPU pipeline.
-   The structure of the RISC-V Instruction Set Architecture (ISA).
-   How assembly language maps to binary machine code.
-   The causes of and solutions for pipeline hazards (data and control).
-   Object-Oriented design principles applied to system simulation.
