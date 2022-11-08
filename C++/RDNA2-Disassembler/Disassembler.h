#pragma once
#include <utility>
#include "Stream.h"

class Disassembler {
public:
    Disassembler(FileInputStream &fs) : fs_(fs) {}
    void dump();
private:
    enum class InstType {
        INVALID,
        // scalar instructions
        SOP1,
        SOP2,
        SOPK,
        SOPC,
        SOPP,
        // vector instructions
        VOP1,
        VOP2,
        VOPC,
        VINTRP,
        VOP3A,
        VOP3B,
        VOP3P,
    };

    void dump_sop1();
    InstType inst_type(uint32_t val) const;
private:
    FileInputStream &fs_;
};
