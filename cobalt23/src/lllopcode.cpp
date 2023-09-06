/* ============================================================================== //
// This file is apart of the Cobalt Programming Language. Cobalt is under the MIT //
// License. Read `cobalt.h` for license information.                              //
// ============================================================================== */


#include "lllcompilerstate.h"
#include "lllopcode.h"

namespace lll {

Opcode::Opcode(CompilerState& cs, Stack& stack) :
    cs_(cs),
    stack_(stack),
    entry_(cs.blocks_[cs.curr_]),
    exit_(cs.blocks_[cs.curr_ + 1]) {
}

llvm::Value* Opcode::CreatePHI(llvm::Type* type, const IncomingList& incoming,
            const std::string& name) {
    auto phi = cs_.B_.CreatePHI(type, incoming.size(), name);
    for (auto& i : incoming)
        phi->addIncoming(i.first, i.second);
    return phi;
}

}

