#ifndef PARSER_H
#define PARSER_H

#include "AST.h"
#include "Lexer.h"
#include "llvm/Support/raw_ostream.h"

class Parser{
    private:
        Lexer &Lex;
        Token Tok;
        bool HasError;

        void error(){
            llvm::errs() << "Unexpected: " << Tok.getText() << "\n";
            HasError = true;
        }

        void advance(){ Lex.next(Tok); }

        // I'm following along with the book for now but this logic seems backward to me.
        // Why am I returning false is the token I expect is the one I have and true when 
        // it isn't?
        bool expect(Token::TokenKind Kind){
            if(Tok.getKind() != Kind){
                error();
                return true;
            }
            return false;
        }

        bool consume(Token::TokenKind Kind){
            if(expect(Kind))
                return true;
            advance();
            return false;
        }

        AST* panicError();
        
        AST* parseCalc();
        Expr* parseExpr();
        Expr* parseTerm();
        Expr* parseFactor();

    public:
        Parser(Lexer &Lex) : Lex(Lex), HasError(false){
            advance();
        }

        bool hasError(){ return HasError; }
        AST* parse();
};

#endif
