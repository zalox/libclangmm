#include "Token.h"

// // // // //
//  Token   //
// // // // //

// returns gets an source location for this token objekt
// based on the translationunit given
clang::SourceLocation clang::Token::get_source_location() {
  return SourceLocation(clang_getTokenLocation(cx_tu, cx_token));
}

// returns a sourcerange that covers this token
clang::SourceRange clang::Token::get_source_range() {
  return SourceRange(clang_getTokenExtent(cx_tu, cx_token));
}
// returns a string description of this tokens kind
std::string clang::Token::get_spelling() {
  CXString s = clang_getTokenSpelling(cx_tu, cx_token);
  return std::string(clang_getCString(s));
}

const clang::TokenKind clang::Token::get_kind() {
  return (TokenKind) clang_getTokenKind(cx_token);
}

//TODO: Is there a way to optimise this?
bool clang::Token::has_type() {
  auto referenced=clang_getCursorReferenced(cx_cursor);
  if(clang_Cursor_isNull(referenced))
    return false;
  auto type=clang_getCursorType(referenced);
  auto cxstr=clang_getTypeSpelling(type);
  std::string spelling=clang_getCString(cxstr);
  clang_disposeString(cxstr);
  return spelling!="";
}

std::string clang::Token::get_type() {
  std::string spelling;
  auto referenced=clang_getCursorReferenced(cx_cursor);
  if(!clang_Cursor_isNull(referenced)) {
    auto type=clang_getCursorType(referenced);
    auto cxstr=clang_getTypeSpelling(type);
    spelling=clang_getCString(cxstr);
    clang_disposeString(cxstr);
    std::string auto_end="";
    //TODO fix const auto
    if((spelling.size()>=4 && spelling.substr(0, 4)=="auto")) {
      auto_end=spelling.substr(4);
      auto type=clang_getCanonicalType(clang_getCursorType(cx_cursor));
      auto cxstr=clang_getTypeSpelling(type);
      spelling=clang_getCString(cxstr);
      clang_disposeString(cxstr);
      if(spelling.find(" ")==std::string::npos)
        spelling+=auto_end;
    }
  }
  return spelling;
}

std::string clang::Token::get_brief_comments() {
  std::string comment_string;
  auto referenced=get_cursor().get_referenced();
  if(referenced) {
    auto cxstr=clang_Cursor_getBriefCommentText(referenced.cx_cursor);
    if(cxstr.data!=NULL)
      comment_string=clang_getCString(cxstr);
    clang_disposeString(cxstr);
  }
  return comment_string;
}
