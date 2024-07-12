#include "Sema.h"

namespace{
  class DeclCheck : public ASTVisitor{
    private:
      llvm::StringSet<> Scope;
      std::map<llvm::StringRef, bool> UsedVariable;
      bool HasError;

      enum ErrorType{ Twice, Not };

      void error(ErrorType ET, llvm::StringRef V){
        llvm::errs() << "Variable " << V << " "
          << (ET == Twice ? "already" : "not")
          << " declared\n";
        HasError = true;
      }

      void warning(llvm::StringRef V){
        llvm::errs() << "Variable " << V << " declared, but not used\n";
      }

    public:
      DeclCheck() : HasError(false) {}

      bool hasError(){ return HasError; }

      virtual void visit(Factor &Node) override{
        if(Node.getKind() == Factor::Ident){
          if(Scope.find(Node.getVal()) == Scope.end())
            error(Not, Node.getVal());
          UsedVariable[Node.getVal()] = true;
        }
      }

      virtual void visit(BinaryOp &Node) override{
        if(Node.getLeft())
          Node.getLeft()->accept(*this);
        else
          HasError = true;

        if(Node.getRight())
          Node.getRight()->accept(*this);
        else
          HasError = true;
      }

      virtual void visit(WithDecl &Node) override{
        for(auto I = Node.begin(), E = Node.end(); I != E; ++I){
          // Scope.insert() returns an object of type std::pair<iterator, bool>
          // where the bool tells if we were able to insert the pointer.
          if(!Scope.insert(*I).second){
            error(Twice, *I);
          }else{
            UsedVariable[*I] = false;
          }
        }

        if(Node.getExpr()){
          Node.getExpr()->accept(*this);
        }else{
          HasError = true;
        }

        // This is an additional check suggest by the book to let the user
        // know if a variable is declared but not used. Since issues of 
        // this nature do not prevent the code from executing we do not
        // set the HasError flag.
        for(const auto& it : UsedVariable){
          if(!it.second)
            warning(it.first);
        }
      }
  };
}

bool Sema::semantic(AST* Tree){
  if(!Tree)
    return false;
  DeclCheck Check;
  Tree->accept(Check);
  return Check.hasError();
}
