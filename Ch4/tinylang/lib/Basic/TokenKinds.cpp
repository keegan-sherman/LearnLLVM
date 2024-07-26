#include "tinylang/Basic/TokenKinds.h"
#include "llvm/Support/ErrorHandling.h"

using namespace tinylang;

static const char* const TokNames[] = {
  #define TOK(ID) #ID,
  #define KEYWORD(ID ,FLAG) #ID,
  #include "tinylang/Basic/TokenKinds.def"
  nullptr // this accounts for the NUM_TOKENS in the TokenKind enum
};

const char* tok::getTokenName(TokenKind Kind){
  if(Kind < tok::NUM_TOKENS)
    return TokNames[Kind];
  llvm_unreachable("unknown TokenKind");
  return nullptr;
}

const char* tok::getPunctuatorSpelling(TokenKind Kind){
  switch(Kind){
    #define PUNCTUATOR(ID, SP) case ID: return SP;
    #include "tinylang/Basic/TokenKinds.def"
    default: break;
  }
  return nullptr;
}

const char* tok::getKeywordSpelling(TokenKind Kind){
  switch(Kind){
    #define KEYWORD(ID, FLAG) case kw_ ## ID: return #ID;
    #include "tinylang/Basic/TokenKinds.def"
    default: break;
  }
  return nullptr;
}
