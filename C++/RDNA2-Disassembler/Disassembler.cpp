#include <cassert>
#include <cstdint>
#include <iostream>
#include "Disassembler.h"

using namespace std;

static const uint32_t bitmask[] = {
    0x00,
    0x01, 0x03, 0x07, 0x0f,
    0x1f, 0x3f, 0x7f, 0xff,
    0x1ff, 0x3ff, 0x7ff, 0xfff,
    0x1fff, 0x3fff, 0x7fff, 0xfff,
};

static uint32_t highest(uint32_t val, uint32_t n) {
    assert(n > 0 && n <= 32);
    return (val >> (32 - n)) & bitmask[n];
}

void Disassembler::dump_sop1() {

}

void Disassembler::dump() {
    uint32_t val;

    while (!fs_.is_end() && fs_.read_u4(val)) {
        cout << hex << val << endl;
        auto t = inst_type(val);
        switch (t) {
        case Disassembler::InstType::SOP1:
            dump_sop1();
            break;
        default:
            cerr << "Skip " << static_cast<int>(t) << endl;
            break;
        }
    }
    if (fs_.meet_error())
        cerr << "Dump error: " << fs_.message() << endl;
}

Disassembler::InstType Disassembler::inst_type(uint32_t val) const {
    uint32_t v;

    v = highest(val, 9);
    switch (v) {
        case 0b101111101:
            return Disassembler::InstType::SOP1;
        case 0b101111110:
            return Disassembler::InstType::SOPC;
        case 0b101111111:
            return Disassembler::InstType::SOPP;
        default:
            break;
    }
    v = highest(val, 4);
    if (v == 0b1011)
        return Disassembler::InstType::SOPK;
    v = highest(val, 2);
    if (v == 0b10)
        return Disassembler::InstType::SOP2;

    v = highest(val, 7);
    if (v == 0b0111111)
        return Disassembler::InstType::VOP1;
    else if (v == 0b0111110)
        return Disassembler::InstType::VOPC;

    v = highest(val, 6);
    if (v == 0b110010)
        return Disassembler::InstType::VINTRP;
    else if (v == 0b110101)
        return Disassembler::InstType::VOP3A;
    else if (v == 0b110101)
        return Disassembler::InstType::VOP3B;
    else if (v == 0b110011)
        return Disassembler::InstType::VOP3P;

    v = highest(val, 1);
    if (v == 0)
        return Disassembler::InstType::VOP2;

    return Disassembler::InstType::INVALID;
}
