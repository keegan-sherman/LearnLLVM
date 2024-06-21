#include "Parser.h"

AST* Parser::parse(){
    AST* Res = parseCalc();
    expect(Token::eoi);
    return Res;
} 

// This method parses the following grammar for the calc rule
// calc : ("with" ident ("," ident)* ":")? expr;
AST* Parser::parseCalc(){
    Expr* E;
    llvm::SmallVector<llvm::StringRef, 8> Vars;

    if( Tok.is(Token::KW_with) ){
        advance();
        if( expect(Token::ident) )
            return panicError(); // Replace the goto with a function call.
            // goto _error; // I'm a little surprised by the use of goto here. I'll probably come back and change this later.
        Vars.push_back(Tok.getText());
        advance();
        
        while( Tok.is(Token::comma) ){
            advance();
            if( expect(Token::ident) )
                return panicError(); // Replace the goto with a function call.
                // goto _error; // Another goto?
            Vars.push_back(Tok.getText());
            advance();
        }

        if(consume(Token::colon))
            return panicError(); // Replace the goto with a function call.
            //goto _error;
    }

    E = parseExpr();

    if(Vars.empty()){
        return E;
    }else{
        return new WithDecl(Vars, E);
    }
/*
    _error:
        while(!Tok.is(Token::eoi))
            advance();
        return nullptr;
*/
}

AST* Parser::panicError(){
    while(!Tok.is(Token::eoi))
        advance();
    return nullptr;
}

// Parse grammar for expr rule
// expr : term (( "+" | "-" ) term)* ;
Expr* Parser::parseExpr(){
    Expr* Left = parseTerm();
    while(Tok.isOneOf(Token::plus, Token::minus)){
        BinaryOp::Operator Op = Tok.is(Token::plus) ? BinaryOp::Plus : BinaryOp::Minus;
        advance();
        Expr* Right = parseTerm();
        Left = new BinaryOp(Op, Left, Right);
    }

    return Left;
}

// Parse grammar for term rule
// term : factor (( "*" | "/" ) factor)* ;
Expr* Parser::parseTerm(){
    Expr* Left = parseFactor();
    while(Tok.isOneOf(Token::star, Token::slash)){
        BinaryOp::Operator Op = Tok.is(Token::star) ? BinaryOp::Mul : BinaryOp::Div;
        advance();
        Expr* Right = parseFactor();
        Left = new BinaryOp(Op, Left, Right);
    }

    return Left;
}

// Parse grammar for factor rule
// factor : ident | number | "(" expr ")" ;
Expr* Parser::parseFactor(){
    Expr* Res = nullptr;
    switch(Tok.getKind()){
        case Token::number:
            Res = new Factor(Factor::Number, Tok.getText());
            advance();
            break;
        case Token::ident:
            Res = new Factor(Factor::Ident, Tok.getText());
            advance();
            break;
        case Token::l_paren:
            advance();
            Res = parseExpr();
            if(!consume(Token::r_paren)) break;
        default:
            if(!Res) error();
            while(!Tok.isOneOf(Token::r_paren, Token::star, Token::plus,
                               Token::minus, Token::slash, Token::eoi)){
                advance();
            }
    }

    return Res;
}
