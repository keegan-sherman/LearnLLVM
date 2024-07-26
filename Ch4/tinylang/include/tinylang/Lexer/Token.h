#ifndef TINYLANG_LEXER_TOKEN_H
#define TINYLANG_LEXER_TOKEN_H

#include "tinylang/Basic/LLVM.h"
#include "tinylang/Basic/TokenKinds.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/Support/SMLoc.h"

namespace tinylang{
  class Token{
    friend class Lexer;

    private:
      const char* Ptr; // location of the token
      size_t Length; // size of the token
      tok::TokenKind Kind; // flavor of the token

    public:
      tok::TokenKind getKind() const { return Kind; }
      void setKind(tok::TokenKind K) { Kind=K; }

      // is/isNot - Predicates to check if this token is a
      // specific kind, as in "if(Tok.is(tok::l_brace)){...}"
      bool is(tok::TokenKind K) const { return Kind == K; }
      bool isNot(tok::TokenKind K) const { return Kind != K; }
      template <typename... Tokens>
      bool isOneOf(Tokens&&... Toks) const{
        return (... || is(Toks)); // Fold expression (C++17)
      }

      const char* getName() const{
        return tok::getTokenName(Kind);
      }

      SMLoc getLocation() const{
        return SMLoc::getFromPointer(Ptr);
      }

      size_t getLength() const { return Length; }

      // We need only provide access to the text of the identifiers and
      // integer or string literals since the text of all other tokens
      // is implied by the token type.
      StringRef getIdentifier(){
        assert(is(tok::identifier) && "Cannot get identifier of non-identifier");
        return StringRef(Ptr, Length);
      }

      StringRef getLiteralData(){
        assert(isOneOf(tok::integer_literal, tok::string_literal)
          && "Cannot get literal of data of non-literal");
        return StringRef(Ptr, Length);
      }
  };
} // namespace tinylang

#endif // TINYLANG_LEXER_TOKEN_H
