#include "llvm/Support/InitLLVM.h"
#include "llvm/Support/raw_ostream.h"
#include "tinylang/Basic/Version.h"

int main(int argc, const char* argv[]){
    llvm::InitLLVM X(argc, argv);
    llvm::outs() << "Hello, I am Tinylang "
                 << tinylang::getTinylangVersion()
                 << "\n";
}
