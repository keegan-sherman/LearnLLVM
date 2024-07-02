#ifndef SEMA_H
#define SEMA_H

#include "AST.h"
#include "Lexer.h"
#include "llvm/ADT/StringSet.h"
#include "llvm/Support/raw_ostream.h"
#include <map>

class Sema{
    public:
        bool semantic(AST* Tree);
};

#endif
