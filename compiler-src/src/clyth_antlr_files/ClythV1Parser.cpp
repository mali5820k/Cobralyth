
// Generated from ../ClythV1.g4 by ANTLR 4.13.2


#include "ClythV1Listener.h"

#include "ClythV1Parser.h"


using namespace antlrcpp;

using namespace antlr4;

namespace {

struct ClythV1ParserStaticData final {
  ClythV1ParserStaticData(std::vector<std::string> ruleNames,
                        std::vector<std::string> literalNames,
                        std::vector<std::string> symbolicNames)
      : ruleNames(std::move(ruleNames)), literalNames(std::move(literalNames)),
        symbolicNames(std::move(symbolicNames)),
        vocabulary(this->literalNames, this->symbolicNames) {}

  ClythV1ParserStaticData(const ClythV1ParserStaticData&) = delete;
  ClythV1ParserStaticData(ClythV1ParserStaticData&&) = delete;
  ClythV1ParserStaticData& operator=(const ClythV1ParserStaticData&) = delete;
  ClythV1ParserStaticData& operator=(ClythV1ParserStaticData&&) = delete;

  std::vector<antlr4::dfa::DFA> decisionToDFA;
  antlr4::atn::PredictionContextCache sharedContextCache;
  const std::vector<std::string> ruleNames;
  const std::vector<std::string> literalNames;
  const std::vector<std::string> symbolicNames;
  const antlr4::dfa::Vocabulary vocabulary;
  antlr4::atn::SerializedATNView serializedATN;
  std::unique_ptr<antlr4::atn::ATN> atn;
};

::antlr4::internal::OnceFlag clythv1ParserOnceFlag;
#if ANTLR4_USE_THREAD_LOCAL_CACHE
static thread_local
#endif
std::unique_ptr<ClythV1ParserStaticData> clythv1ParserStaticData = nullptr;

void clythv1ParserInitialize() {
#if ANTLR4_USE_THREAD_LOCAL_CACHE
  if (clythv1ParserStaticData != nullptr) {
    return;
  }
#else
  assert(clythv1ParserStaticData == nullptr);
#endif
  auto staticData = std::make_unique<ClythV1ParserStaticData>(
    std::vector<std::string>{
      "program", "topLevelItem", "includeDecl", "includeTarget", "externDecl", 
      "externAbi", "externParamList", "externParam", "structDecl", "structField", 
      "functionDecl", "paramList", "param", "block", "statement", "varDecl", 
      "assignmentStmt", "assignable", "assignmentOp", "exprStmt", "returnStmt", 
      "ifStmt", "elseClause", "whileStmt", "forStmt", "forEachHeader", "forEachBinding", 
      "forEachVar", "forClassicHeader", "forInit", "varDeclNoSemi", "assignmentNoSemi", 
      "breakStmt", "continueStmt", "parenExpr", "expression", "logicalOr", 
      "logicalAnd", "equality", "relation", "comparison", "additive", "multiplicative", 
      "unary", "postfix", "postfixSuffix", "primary", "argumentList", "literal", 
      "collectionLiteral", "listLiteral", "mapLiteral", "mapEntryList", 
      "mapEntry", "setLiteral", "expressionList", "type", "mapType", "collectionType", 
      "baseType", "typeSuffix", "commaOrSemi"
    },
    std::vector<std::string>{
      "", "'('", "')'", "','", "'{'", "'}'", "'='", "'+='", "'-='", "'*='", 
      "'/='", "'%='", "'['", "']'", "':'", "'include'", "'extern'", "'C'", 
      "'struct'", "'if'", "'else'", "'for'", "'while'", "'return'", "'break'", 
      "'continue'", "'is'", "'in'", "'not'", "';'", "'||'", "'&&'", "'=='", 
      "'!='", "'>='", "'<='", "'>'", "'<'", "'++'", "'--'", "'+'", "'-'", 
      "'*'", "'/'", "'%'", "'!'", "'~'", "'&'", "'.'", "'...'", "", "", 
      "'null'"
    },
    std::vector<std::string>{
      "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "INCLUDE", 
      "EXTERN", "C_ABI", "STRUCT", "IF", "ELSE", "FOR", "WHILE", "RETURN", 
      "BREAK", "CONTINUE", "IS", "IN", "NOT", "SEMI", "OR", "AND", "EQ", 
      "NEQ", "GTE", "LTE", "GT", "LT", "INCREMENT", "DECREMENT", "PLUS", 
      "MINUS", "STAR", "SLASH", "PERCENT", "BANG", "TILDE", "AMP", "DOT", 
      "VARARGS", "BUILTIN_TYPE", "BOOLEAN_LITERAL", "NULL_LITERAL", "NUMERIC_LITERAL", 
      "TEMPLATE_STRING_LITERAL", "STRING_LITERAL", "IDENTIFIER", "SINGLE_LINE_COMMENT", 
      "MULTI_LINE_COMMENT", "WS"
    }
  );
  static const int32_t serializedATNSegment[] = {
  	4,1,59,576,2,0,7,0,2,1,7,1,2,2,7,2,2,3,7,3,2,4,7,4,2,5,7,5,2,6,7,6,2,
  	7,7,7,2,8,7,8,2,9,7,9,2,10,7,10,2,11,7,11,2,12,7,12,2,13,7,13,2,14,7,
  	14,2,15,7,15,2,16,7,16,2,17,7,17,2,18,7,18,2,19,7,19,2,20,7,20,2,21,7,
  	21,2,22,7,22,2,23,7,23,2,24,7,24,2,25,7,25,2,26,7,26,2,27,7,27,2,28,7,
  	28,2,29,7,29,2,30,7,30,2,31,7,31,2,32,7,32,2,33,7,33,2,34,7,34,2,35,7,
  	35,2,36,7,36,2,37,7,37,2,38,7,38,2,39,7,39,2,40,7,40,2,41,7,41,2,42,7,
  	42,2,43,7,43,2,44,7,44,2,45,7,45,2,46,7,46,2,47,7,47,2,48,7,48,2,49,7,
  	49,2,50,7,50,2,51,7,51,2,52,7,52,2,53,7,53,2,54,7,54,2,55,7,55,2,56,7,
  	56,2,57,7,57,2,58,7,58,2,59,7,59,2,60,7,60,2,61,7,61,1,0,5,0,126,8,0,
  	10,0,12,0,129,9,0,1,0,1,0,1,1,1,1,1,1,1,1,1,1,3,1,138,8,1,1,2,1,2,1,2,
  	3,2,143,8,2,1,3,1,3,1,4,1,4,3,4,149,8,4,1,4,1,4,1,4,1,4,3,4,155,8,4,1,
  	4,1,4,3,4,159,8,4,1,5,1,5,1,6,1,6,1,6,5,6,166,8,6,10,6,12,6,169,9,6,1,
  	6,3,6,172,8,6,1,6,3,6,175,8,6,1,6,3,6,178,8,6,1,7,1,7,3,7,182,8,7,1,8,
  	1,8,1,8,1,8,5,8,188,8,8,10,8,12,8,191,9,8,1,8,1,8,3,8,195,8,8,1,9,1,9,
  	1,9,3,9,200,8,9,1,10,1,10,1,10,1,10,3,10,206,8,10,1,10,1,10,1,10,1,11,
  	1,11,1,11,5,11,214,8,11,10,11,12,11,217,9,11,1,11,3,11,220,8,11,1,12,
  	1,12,1,12,1,13,1,13,5,13,227,8,13,10,13,12,13,230,9,13,1,13,1,13,1,14,
  	1,14,1,14,1,14,1,14,1,14,1,14,1,14,1,14,1,14,3,14,244,8,14,1,15,1,15,
  	1,15,1,15,3,15,250,8,15,1,15,3,15,253,8,15,1,16,1,16,1,16,1,16,3,16,259,
  	8,16,1,17,1,17,5,17,263,8,17,10,17,12,17,266,9,17,1,18,1,18,1,19,1,19,
  	3,19,272,8,19,1,20,1,20,3,20,276,8,20,1,20,3,20,279,8,20,1,21,1,21,1,
  	21,1,21,3,21,285,8,21,1,21,1,21,1,21,1,21,3,21,291,8,21,3,21,293,8,21,
  	1,22,1,22,1,22,1,22,3,22,299,8,22,1,23,1,23,1,23,1,23,1,23,1,23,1,23,
  	1,23,3,23,309,8,23,1,24,1,24,1,24,1,24,1,24,1,24,1,24,1,24,1,24,1,24,
  	1,24,1,24,1,24,1,24,1,24,1,24,1,24,1,24,1,24,1,24,1,24,1,24,3,24,333,
  	8,24,1,25,1,25,1,25,1,25,1,26,1,26,1,26,5,26,342,8,26,10,26,12,26,345,
  	9,26,1,27,1,27,1,27,1,27,3,27,351,8,27,1,28,3,28,354,8,28,1,28,1,28,3,
  	28,358,8,28,1,28,1,28,3,28,362,8,28,1,29,1,29,1,29,3,29,367,8,29,1,30,
  	1,30,1,30,1,30,3,30,373,8,30,1,31,1,31,1,31,1,31,1,32,1,32,3,32,381,8,
  	32,1,33,1,33,3,33,385,8,33,1,34,1,34,1,34,1,34,1,35,1,35,1,36,1,36,1,
  	36,5,36,396,8,36,10,36,12,36,399,9,36,1,37,1,37,1,37,5,37,404,8,37,10,
  	37,12,37,407,9,37,1,38,1,38,1,38,5,38,412,8,38,10,38,12,38,415,9,38,1,
  	39,1,39,1,39,5,39,420,8,39,10,39,12,39,423,9,39,1,40,1,40,1,40,5,40,428,
  	8,40,10,40,12,40,431,9,40,1,41,1,41,1,41,5,41,436,8,41,10,41,12,41,439,
  	9,41,1,42,1,42,1,42,5,42,444,8,42,10,42,12,42,447,9,42,1,43,1,43,1,43,
  	3,43,452,8,43,1,44,1,44,5,44,456,8,44,10,44,12,44,459,9,44,1,45,1,45,
  	1,45,1,45,1,45,1,45,1,45,1,45,3,45,469,8,45,1,45,1,45,1,45,3,45,474,8,
  	45,1,46,1,46,1,46,1,46,1,46,1,46,1,46,3,46,483,8,46,1,47,1,47,1,47,5,
  	47,488,8,47,10,47,12,47,491,9,47,1,47,3,47,494,8,47,1,48,1,48,1,49,1,
  	49,1,49,3,49,501,8,49,1,50,1,50,3,50,505,8,50,1,50,1,50,1,51,1,51,3,51,
  	511,8,51,1,51,1,51,1,52,1,52,1,52,5,52,518,8,52,10,52,12,52,521,9,52,
  	1,52,3,52,524,8,52,1,53,1,53,1,53,1,53,1,54,1,54,1,54,3,54,533,8,54,1,
  	54,1,54,1,55,1,55,1,55,5,55,540,8,55,10,55,12,55,543,9,55,1,55,3,55,546,
  	8,55,1,56,1,56,3,56,550,8,56,1,57,1,57,1,57,1,57,1,58,1,58,5,58,558,8,
  	58,10,58,12,58,561,9,58,1,59,1,59,1,60,1,60,1,60,1,60,1,60,1,60,1,60,
  	3,60,572,8,60,1,61,1,61,1,61,0,0,62,0,2,4,6,8,10,12,14,16,18,20,22,24,
  	26,28,30,32,34,36,38,40,42,44,46,48,50,52,54,56,58,60,62,64,66,68,70,
  	72,74,76,78,80,82,84,86,88,90,92,94,96,98,100,102,104,106,108,110,112,
  	114,116,118,120,122,0,12,1,0,54,56,2,0,17,17,56,56,1,0,6,11,1,0,32,33,
  	1,0,26,27,1,0,34,37,1,0,40,41,1,0,42,44,3,0,28,28,41,41,45,47,1,0,51,
  	55,2,0,50,50,56,56,2,0,3,3,29,29,601,0,127,1,0,0,0,2,137,1,0,0,0,4,139,
  	1,0,0,0,6,144,1,0,0,0,8,146,1,0,0,0,10,160,1,0,0,0,12,177,1,0,0,0,14,
  	179,1,0,0,0,16,183,1,0,0,0,18,196,1,0,0,0,20,201,1,0,0,0,22,210,1,0,0,
  	0,24,221,1,0,0,0,26,224,1,0,0,0,28,243,1,0,0,0,30,245,1,0,0,0,32,254,
  	1,0,0,0,34,260,1,0,0,0,36,267,1,0,0,0,38,269,1,0,0,0,40,273,1,0,0,0,42,
  	292,1,0,0,0,44,298,1,0,0,0,46,308,1,0,0,0,48,332,1,0,0,0,50,334,1,0,0,
  	0,52,338,1,0,0,0,54,350,1,0,0,0,56,353,1,0,0,0,58,366,1,0,0,0,60,368,
  	1,0,0,0,62,374,1,0,0,0,64,378,1,0,0,0,66,382,1,0,0,0,68,386,1,0,0,0,70,
  	390,1,0,0,0,72,392,1,0,0,0,74,400,1,0,0,0,76,408,1,0,0,0,78,416,1,0,0,
  	0,80,424,1,0,0,0,82,432,1,0,0,0,84,440,1,0,0,0,86,451,1,0,0,0,88,453,
  	1,0,0,0,90,473,1,0,0,0,92,482,1,0,0,0,94,484,1,0,0,0,96,495,1,0,0,0,98,
  	500,1,0,0,0,100,502,1,0,0,0,102,508,1,0,0,0,104,514,1,0,0,0,106,525,1,
  	0,0,0,108,529,1,0,0,0,110,536,1,0,0,0,112,549,1,0,0,0,114,551,1,0,0,0,
  	116,555,1,0,0,0,118,562,1,0,0,0,120,571,1,0,0,0,122,573,1,0,0,0,124,126,
  	3,2,1,0,125,124,1,0,0,0,126,129,1,0,0,0,127,125,1,0,0,0,127,128,1,0,0,
  	0,128,130,1,0,0,0,129,127,1,0,0,0,130,131,5,0,0,1,131,1,1,0,0,0,132,138,
  	3,4,2,0,133,138,3,8,4,0,134,138,3,16,8,0,135,138,3,20,10,0,136,138,3,
  	28,14,0,137,132,1,0,0,0,137,133,1,0,0,0,137,134,1,0,0,0,137,135,1,0,0,
  	0,137,136,1,0,0,0,138,3,1,0,0,0,139,140,5,15,0,0,140,142,3,6,3,0,141,
  	143,5,29,0,0,142,141,1,0,0,0,142,143,1,0,0,0,143,5,1,0,0,0,144,145,7,
  	0,0,0,145,7,1,0,0,0,146,148,5,16,0,0,147,149,3,10,5,0,148,147,1,0,0,0,
  	148,149,1,0,0,0,149,150,1,0,0,0,150,151,3,112,56,0,151,152,5,56,0,0,152,
  	154,5,1,0,0,153,155,3,12,6,0,154,153,1,0,0,0,154,155,1,0,0,0,155,156,
  	1,0,0,0,156,158,5,2,0,0,157,159,5,29,0,0,158,157,1,0,0,0,158,159,1,0,
  	0,0,159,9,1,0,0,0,160,161,7,1,0,0,161,11,1,0,0,0,162,167,3,14,7,0,163,
  	164,5,3,0,0,164,166,3,14,7,0,165,163,1,0,0,0,166,169,1,0,0,0,167,165,
  	1,0,0,0,167,168,1,0,0,0,168,171,1,0,0,0,169,167,1,0,0,0,170,172,5,3,0,
  	0,171,170,1,0,0,0,171,172,1,0,0,0,172,174,1,0,0,0,173,175,5,49,0,0,174,
  	173,1,0,0,0,174,175,1,0,0,0,175,178,1,0,0,0,176,178,5,49,0,0,177,162,
  	1,0,0,0,177,176,1,0,0,0,178,13,1,0,0,0,179,181,3,112,56,0,180,182,5,56,
  	0,0,181,180,1,0,0,0,181,182,1,0,0,0,182,15,1,0,0,0,183,184,5,18,0,0,184,
  	185,5,56,0,0,185,189,5,4,0,0,186,188,3,18,9,0,187,186,1,0,0,0,188,191,
  	1,0,0,0,189,187,1,0,0,0,189,190,1,0,0,0,190,192,1,0,0,0,191,189,1,0,0,
  	0,192,194,5,5,0,0,193,195,5,29,0,0,194,193,1,0,0,0,194,195,1,0,0,0,195,
  	17,1,0,0,0,196,197,3,112,56,0,197,199,5,56,0,0,198,200,3,122,61,0,199,
  	198,1,0,0,0,199,200,1,0,0,0,200,19,1,0,0,0,201,202,3,112,56,0,202,203,
  	5,56,0,0,203,205,5,1,0,0,204,206,3,22,11,0,205,204,1,0,0,0,205,206,1,
  	0,0,0,206,207,1,0,0,0,207,208,5,2,0,0,208,209,3,26,13,0,209,21,1,0,0,
  	0,210,215,3,24,12,0,211,212,5,3,0,0,212,214,3,24,12,0,213,211,1,0,0,0,
  	214,217,1,0,0,0,215,213,1,0,0,0,215,216,1,0,0,0,216,219,1,0,0,0,217,215,
  	1,0,0,0,218,220,5,3,0,0,219,218,1,0,0,0,219,220,1,0,0,0,220,23,1,0,0,
  	0,221,222,3,112,56,0,222,223,5,56,0,0,223,25,1,0,0,0,224,228,5,4,0,0,
  	225,227,3,28,14,0,226,225,1,0,0,0,227,230,1,0,0,0,228,226,1,0,0,0,228,
  	229,1,0,0,0,229,231,1,0,0,0,230,228,1,0,0,0,231,232,5,5,0,0,232,27,1,
  	0,0,0,233,244,3,30,15,0,234,244,3,32,16,0,235,244,3,38,19,0,236,244,3,
  	40,20,0,237,244,3,42,21,0,238,244,3,46,23,0,239,244,3,48,24,0,240,244,
  	3,64,32,0,241,244,3,66,33,0,242,244,3,26,13,0,243,233,1,0,0,0,243,234,
  	1,0,0,0,243,235,1,0,0,0,243,236,1,0,0,0,243,237,1,0,0,0,243,238,1,0,0,
  	0,243,239,1,0,0,0,243,240,1,0,0,0,243,241,1,0,0,0,243,242,1,0,0,0,244,
  	29,1,0,0,0,245,246,3,112,56,0,246,249,5,56,0,0,247,248,5,6,0,0,248,250,
  	3,70,35,0,249,247,1,0,0,0,249,250,1,0,0,0,250,252,1,0,0,0,251,253,5,29,
  	0,0,252,251,1,0,0,0,252,253,1,0,0,0,253,31,1,0,0,0,254,255,3,34,17,0,
  	255,256,3,36,18,0,256,258,3,70,35,0,257,259,5,29,0,0,258,257,1,0,0,0,
  	258,259,1,0,0,0,259,33,1,0,0,0,260,264,5,56,0,0,261,263,3,90,45,0,262,
  	261,1,0,0,0,263,266,1,0,0,0,264,262,1,0,0,0,264,265,1,0,0,0,265,35,1,
  	0,0,0,266,264,1,0,0,0,267,268,7,2,0,0,268,37,1,0,0,0,269,271,3,70,35,
  	0,270,272,5,29,0,0,271,270,1,0,0,0,271,272,1,0,0,0,272,39,1,0,0,0,273,
  	275,5,23,0,0,274,276,3,70,35,0,275,274,1,0,0,0,275,276,1,0,0,0,276,278,
  	1,0,0,0,277,279,5,29,0,0,278,277,1,0,0,0,278,279,1,0,0,0,279,41,1,0,0,
  	0,280,281,5,19,0,0,281,282,3,68,34,0,282,284,3,26,13,0,283,285,3,44,22,
  	0,284,283,1,0,0,0,284,285,1,0,0,0,285,293,1,0,0,0,286,287,5,19,0,0,287,
  	288,3,70,35,0,288,290,3,26,13,0,289,291,3,44,22,0,290,289,1,0,0,0,290,
  	291,1,0,0,0,291,293,1,0,0,0,292,280,1,0,0,0,292,286,1,0,0,0,293,43,1,
  	0,0,0,294,295,5,20,0,0,295,299,3,42,21,0,296,297,5,20,0,0,297,299,3,26,
  	13,0,298,294,1,0,0,0,298,296,1,0,0,0,299,45,1,0,0,0,300,301,5,22,0,0,
  	301,302,3,68,34,0,302,303,3,26,13,0,303,309,1,0,0,0,304,305,5,22,0,0,
  	305,306,3,70,35,0,306,307,3,26,13,0,307,309,1,0,0,0,308,300,1,0,0,0,308,
  	304,1,0,0,0,309,47,1,0,0,0,310,311,5,21,0,0,311,333,3,26,13,0,312,313,
  	5,21,0,0,313,314,3,50,25,0,314,315,3,26,13,0,315,333,1,0,0,0,316,317,
  	5,21,0,0,317,318,5,1,0,0,318,319,3,50,25,0,319,320,5,2,0,0,320,321,3,
  	26,13,0,321,333,1,0,0,0,322,323,5,21,0,0,323,324,3,56,28,0,324,325,3,
  	26,13,0,325,333,1,0,0,0,326,327,5,21,0,0,327,328,5,1,0,0,328,329,3,56,
  	28,0,329,330,5,2,0,0,330,331,3,26,13,0,331,333,1,0,0,0,332,310,1,0,0,
  	0,332,312,1,0,0,0,332,316,1,0,0,0,332,322,1,0,0,0,332,326,1,0,0,0,333,
  	49,1,0,0,0,334,335,3,52,26,0,335,336,5,27,0,0,336,337,3,70,35,0,337,51,
  	1,0,0,0,338,343,3,54,27,0,339,340,5,3,0,0,340,342,3,54,27,0,341,339,1,
  	0,0,0,342,345,1,0,0,0,343,341,1,0,0,0,343,344,1,0,0,0,344,53,1,0,0,0,
  	345,343,1,0,0,0,346,347,3,112,56,0,347,348,5,56,0,0,348,351,1,0,0,0,349,
  	351,5,56,0,0,350,346,1,0,0,0,350,349,1,0,0,0,351,55,1,0,0,0,352,354,3,
  	58,29,0,353,352,1,0,0,0,353,354,1,0,0,0,354,355,1,0,0,0,355,357,5,29,
  	0,0,356,358,3,70,35,0,357,356,1,0,0,0,357,358,1,0,0,0,358,359,1,0,0,0,
  	359,361,5,29,0,0,360,362,3,70,35,0,361,360,1,0,0,0,361,362,1,0,0,0,362,
  	57,1,0,0,0,363,367,3,60,30,0,364,367,3,62,31,0,365,367,3,70,35,0,366,
  	363,1,0,0,0,366,364,1,0,0,0,366,365,1,0,0,0,367,59,1,0,0,0,368,369,3,
  	112,56,0,369,372,5,56,0,0,370,371,5,6,0,0,371,373,3,70,35,0,372,370,1,
  	0,0,0,372,373,1,0,0,0,373,61,1,0,0,0,374,375,3,34,17,0,375,376,3,36,18,
  	0,376,377,3,70,35,0,377,63,1,0,0,0,378,380,5,24,0,0,379,381,5,29,0,0,
  	380,379,1,0,0,0,380,381,1,0,0,0,381,65,1,0,0,0,382,384,5,25,0,0,383,385,
  	5,29,0,0,384,383,1,0,0,0,384,385,1,0,0,0,385,67,1,0,0,0,386,387,5,1,0,
  	0,387,388,3,70,35,0,388,389,5,2,0,0,389,69,1,0,0,0,390,391,3,72,36,0,
  	391,71,1,0,0,0,392,397,3,74,37,0,393,394,5,30,0,0,394,396,3,74,37,0,395,
  	393,1,0,0,0,396,399,1,0,0,0,397,395,1,0,0,0,397,398,1,0,0,0,398,73,1,
  	0,0,0,399,397,1,0,0,0,400,405,3,76,38,0,401,402,5,31,0,0,402,404,3,76,
  	38,0,403,401,1,0,0,0,404,407,1,0,0,0,405,403,1,0,0,0,405,406,1,0,0,0,
  	406,75,1,0,0,0,407,405,1,0,0,0,408,413,3,78,39,0,409,410,7,3,0,0,410,
  	412,3,78,39,0,411,409,1,0,0,0,412,415,1,0,0,0,413,411,1,0,0,0,413,414,
  	1,0,0,0,414,77,1,0,0,0,415,413,1,0,0,0,416,421,3,80,40,0,417,418,7,4,
  	0,0,418,420,3,80,40,0,419,417,1,0,0,0,420,423,1,0,0,0,421,419,1,0,0,0,
  	421,422,1,0,0,0,422,79,1,0,0,0,423,421,1,0,0,0,424,429,3,82,41,0,425,
  	426,7,5,0,0,426,428,3,82,41,0,427,425,1,0,0,0,428,431,1,0,0,0,429,427,
  	1,0,0,0,429,430,1,0,0,0,430,81,1,0,0,0,431,429,1,0,0,0,432,437,3,84,42,
  	0,433,434,7,6,0,0,434,436,3,84,42,0,435,433,1,0,0,0,436,439,1,0,0,0,437,
  	435,1,0,0,0,437,438,1,0,0,0,438,83,1,0,0,0,439,437,1,0,0,0,440,445,3,
  	86,43,0,441,442,7,7,0,0,442,444,3,86,43,0,443,441,1,0,0,0,444,447,1,0,
  	0,0,445,443,1,0,0,0,445,446,1,0,0,0,446,85,1,0,0,0,447,445,1,0,0,0,448,
  	449,7,8,0,0,449,452,3,86,43,0,450,452,3,88,44,0,451,448,1,0,0,0,451,450,
  	1,0,0,0,452,87,1,0,0,0,453,457,3,92,46,0,454,456,3,90,45,0,455,454,1,
  	0,0,0,456,459,1,0,0,0,457,455,1,0,0,0,457,458,1,0,0,0,458,89,1,0,0,0,
  	459,457,1,0,0,0,460,461,5,48,0,0,461,474,5,56,0,0,462,463,5,12,0,0,463,
  	464,3,70,35,0,464,465,5,13,0,0,465,474,1,0,0,0,466,468,5,1,0,0,467,469,
  	3,94,47,0,468,467,1,0,0,0,468,469,1,0,0,0,469,470,1,0,0,0,470,474,5,2,
  	0,0,471,474,5,38,0,0,472,474,5,39,0,0,473,460,1,0,0,0,473,462,1,0,0,0,
  	473,466,1,0,0,0,473,471,1,0,0,0,473,472,1,0,0,0,474,91,1,0,0,0,475,483,
  	3,96,48,0,476,483,3,98,49,0,477,483,5,56,0,0,478,479,5,1,0,0,479,480,
  	3,70,35,0,480,481,5,2,0,0,481,483,1,0,0,0,482,475,1,0,0,0,482,476,1,0,
  	0,0,482,477,1,0,0,0,482,478,1,0,0,0,483,93,1,0,0,0,484,489,3,70,35,0,
  	485,486,5,3,0,0,486,488,3,70,35,0,487,485,1,0,0,0,488,491,1,0,0,0,489,
  	487,1,0,0,0,489,490,1,0,0,0,490,493,1,0,0,0,491,489,1,0,0,0,492,494,5,
  	3,0,0,493,492,1,0,0,0,493,494,1,0,0,0,494,95,1,0,0,0,495,496,7,9,0,0,
  	496,97,1,0,0,0,497,501,3,100,50,0,498,501,3,102,51,0,499,501,3,108,54,
  	0,500,497,1,0,0,0,500,498,1,0,0,0,500,499,1,0,0,0,501,99,1,0,0,0,502,
  	504,5,12,0,0,503,505,3,110,55,0,504,503,1,0,0,0,504,505,1,0,0,0,505,506,
  	1,0,0,0,506,507,5,13,0,0,507,101,1,0,0,0,508,510,5,4,0,0,509,511,3,104,
  	52,0,510,509,1,0,0,0,510,511,1,0,0,0,511,512,1,0,0,0,512,513,5,5,0,0,
  	513,103,1,0,0,0,514,519,3,106,53,0,515,516,5,3,0,0,516,518,3,106,53,0,
  	517,515,1,0,0,0,518,521,1,0,0,0,519,517,1,0,0,0,519,520,1,0,0,0,520,523,
  	1,0,0,0,521,519,1,0,0,0,522,524,5,3,0,0,523,522,1,0,0,0,523,524,1,0,0,
  	0,524,105,1,0,0,0,525,526,3,70,35,0,526,527,5,14,0,0,527,528,3,70,35,
  	0,528,107,1,0,0,0,529,530,5,4,0,0,530,532,3,110,55,0,531,533,5,3,0,0,
  	532,531,1,0,0,0,532,533,1,0,0,0,533,534,1,0,0,0,534,535,5,5,0,0,535,109,
  	1,0,0,0,536,541,3,70,35,0,537,538,5,3,0,0,538,540,3,70,35,0,539,537,1,
  	0,0,0,540,543,1,0,0,0,541,539,1,0,0,0,541,542,1,0,0,0,542,545,1,0,0,0,
  	543,541,1,0,0,0,544,546,5,3,0,0,545,544,1,0,0,0,545,546,1,0,0,0,546,111,
  	1,0,0,0,547,550,3,114,57,0,548,550,3,116,58,0,549,547,1,0,0,0,549,548,
  	1,0,0,0,550,113,1,0,0,0,551,552,3,116,58,0,552,553,5,14,0,0,553,554,3,
  	116,58,0,554,115,1,0,0,0,555,559,3,118,59,0,556,558,3,120,60,0,557,556,
  	1,0,0,0,558,561,1,0,0,0,559,557,1,0,0,0,559,560,1,0,0,0,560,117,1,0,0,
  	0,561,559,1,0,0,0,562,563,7,10,0,0,563,119,1,0,0,0,564,565,5,12,0,0,565,
  	572,5,13,0,0,566,567,5,12,0,0,567,568,5,53,0,0,568,572,5,13,0,0,569,570,
  	5,1,0,0,570,572,5,2,0,0,571,564,1,0,0,0,571,566,1,0,0,0,571,569,1,0,0,
  	0,572,121,1,0,0,0,573,574,7,11,0,0,574,123,1,0,0,0,66,127,137,142,148,
  	154,158,167,171,174,177,181,189,194,199,205,215,219,228,243,249,252,258,
  	264,271,275,278,284,290,292,298,308,332,343,350,353,357,361,366,372,380,
  	384,397,405,413,421,429,437,445,451,457,468,473,482,489,493,500,504,510,
  	519,523,532,541,545,549,559,571
  };
  staticData->serializedATN = antlr4::atn::SerializedATNView(serializedATNSegment, sizeof(serializedATNSegment) / sizeof(serializedATNSegment[0]));

  antlr4::atn::ATNDeserializer deserializer;
  staticData->atn = deserializer.deserialize(staticData->serializedATN);

  const size_t count = staticData->atn->getNumberOfDecisions();
  staticData->decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) { 
    staticData->decisionToDFA.emplace_back(staticData->atn->getDecisionState(i), i);
  }
  clythv1ParserStaticData = std::move(staticData);
}

}

ClythV1Parser::ClythV1Parser(TokenStream *input) : ClythV1Parser(input, antlr4::atn::ParserATNSimulatorOptions()) {}

ClythV1Parser::ClythV1Parser(TokenStream *input, const antlr4::atn::ParserATNSimulatorOptions &options) : Parser(input) {
  ClythV1Parser::initialize();
  _interpreter = new atn::ParserATNSimulator(this, *clythv1ParserStaticData->atn, clythv1ParserStaticData->decisionToDFA, clythv1ParserStaticData->sharedContextCache, options);
}

ClythV1Parser::~ClythV1Parser() {
  delete _interpreter;
}

const atn::ATN& ClythV1Parser::getATN() const {
  return *clythv1ParserStaticData->atn;
}

std::string ClythV1Parser::getGrammarFileName() const {
  return "ClythV1.g4";
}

const std::vector<std::string>& ClythV1Parser::getRuleNames() const {
  return clythv1ParserStaticData->ruleNames;
}

const dfa::Vocabulary& ClythV1Parser::getVocabulary() const {
  return clythv1ParserStaticData->vocabulary;
}

antlr4::atn::SerializedATNView ClythV1Parser::getSerializedATN() const {
  return clythv1ParserStaticData->serializedATN;
}


//----------------- ProgramContext ------------------------------------------------------------------

ClythV1Parser::ProgramContext::ProgramContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ClythV1Parser::ProgramContext::EOF() {
  return getToken(ClythV1Parser::EOF, 0);
}

std::vector<ClythV1Parser::TopLevelItemContext *> ClythV1Parser::ProgramContext::topLevelItem() {
  return getRuleContexts<ClythV1Parser::TopLevelItemContext>();
}

ClythV1Parser::TopLevelItemContext* ClythV1Parser::ProgramContext::topLevelItem(size_t i) {
  return getRuleContext<ClythV1Parser::TopLevelItemContext>(i);
}


size_t ClythV1Parser::ProgramContext::getRuleIndex() const {
  return ClythV1Parser::RuleProgram;
}

void ClythV1Parser::ProgramContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterProgram(this);
}

void ClythV1Parser::ProgramContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitProgram(this);
}

ClythV1Parser::ProgramContext* ClythV1Parser::program() {
  ProgramContext *_localctx = _tracker.createInstance<ProgramContext>(_ctx, getState());
  enterRule(_localctx, 0, ClythV1Parser::RuleProgram);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(127);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 143237778131226642) != 0)) {
      setState(124);
      topLevelItem();
      setState(129);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(130);
    match(ClythV1Parser::EOF);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- TopLevelItemContext ------------------------------------------------------------------

ClythV1Parser::TopLevelItemContext::TopLevelItemContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ClythV1Parser::IncludeDeclContext* ClythV1Parser::TopLevelItemContext::includeDecl() {
  return getRuleContext<ClythV1Parser::IncludeDeclContext>(0);
}

ClythV1Parser::ExternDeclContext* ClythV1Parser::TopLevelItemContext::externDecl() {
  return getRuleContext<ClythV1Parser::ExternDeclContext>(0);
}

ClythV1Parser::StructDeclContext* ClythV1Parser::TopLevelItemContext::structDecl() {
  return getRuleContext<ClythV1Parser::StructDeclContext>(0);
}

ClythV1Parser::FunctionDeclContext* ClythV1Parser::TopLevelItemContext::functionDecl() {
  return getRuleContext<ClythV1Parser::FunctionDeclContext>(0);
}

ClythV1Parser::StatementContext* ClythV1Parser::TopLevelItemContext::statement() {
  return getRuleContext<ClythV1Parser::StatementContext>(0);
}


size_t ClythV1Parser::TopLevelItemContext::getRuleIndex() const {
  return ClythV1Parser::RuleTopLevelItem;
}

void ClythV1Parser::TopLevelItemContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterTopLevelItem(this);
}

void ClythV1Parser::TopLevelItemContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitTopLevelItem(this);
}

ClythV1Parser::TopLevelItemContext* ClythV1Parser::topLevelItem() {
  TopLevelItemContext *_localctx = _tracker.createInstance<TopLevelItemContext>(_ctx, getState());
  enterRule(_localctx, 2, ClythV1Parser::RuleTopLevelItem);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(137);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 1, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(132);
      includeDecl();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(133);
      externDecl();
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(134);
      structDecl();
      break;
    }

    case 4: {
      enterOuterAlt(_localctx, 4);
      setState(135);
      functionDecl();
      break;
    }

    case 5: {
      enterOuterAlt(_localctx, 5);
      setState(136);
      statement();
      break;
    }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- IncludeDeclContext ------------------------------------------------------------------

ClythV1Parser::IncludeDeclContext::IncludeDeclContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ClythV1Parser::IncludeDeclContext::INCLUDE() {
  return getToken(ClythV1Parser::INCLUDE, 0);
}

ClythV1Parser::IncludeTargetContext* ClythV1Parser::IncludeDeclContext::includeTarget() {
  return getRuleContext<ClythV1Parser::IncludeTargetContext>(0);
}

tree::TerminalNode* ClythV1Parser::IncludeDeclContext::SEMI() {
  return getToken(ClythV1Parser::SEMI, 0);
}


size_t ClythV1Parser::IncludeDeclContext::getRuleIndex() const {
  return ClythV1Parser::RuleIncludeDecl;
}

void ClythV1Parser::IncludeDeclContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterIncludeDecl(this);
}

void ClythV1Parser::IncludeDeclContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitIncludeDecl(this);
}

ClythV1Parser::IncludeDeclContext* ClythV1Parser::includeDecl() {
  IncludeDeclContext *_localctx = _tracker.createInstance<IncludeDeclContext>(_ctx, getState());
  enterRule(_localctx, 4, ClythV1Parser::RuleIncludeDecl);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(139);
    match(ClythV1Parser::INCLUDE);
    setState(140);
    includeTarget();
    setState(142);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ClythV1Parser::SEMI) {
      setState(141);
      match(ClythV1Parser::SEMI);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- IncludeTargetContext ------------------------------------------------------------------

ClythV1Parser::IncludeTargetContext::IncludeTargetContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ClythV1Parser::IncludeTargetContext::IDENTIFIER() {
  return getToken(ClythV1Parser::IDENTIFIER, 0);
}

tree::TerminalNode* ClythV1Parser::IncludeTargetContext::STRING_LITERAL() {
  return getToken(ClythV1Parser::STRING_LITERAL, 0);
}

tree::TerminalNode* ClythV1Parser::IncludeTargetContext::TEMPLATE_STRING_LITERAL() {
  return getToken(ClythV1Parser::TEMPLATE_STRING_LITERAL, 0);
}


size_t ClythV1Parser::IncludeTargetContext::getRuleIndex() const {
  return ClythV1Parser::RuleIncludeTarget;
}

void ClythV1Parser::IncludeTargetContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterIncludeTarget(this);
}

void ClythV1Parser::IncludeTargetContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitIncludeTarget(this);
}

ClythV1Parser::IncludeTargetContext* ClythV1Parser::includeTarget() {
  IncludeTargetContext *_localctx = _tracker.createInstance<IncludeTargetContext>(_ctx, getState());
  enterRule(_localctx, 6, ClythV1Parser::RuleIncludeTarget);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(144);
    _la = _input->LA(1);
    if (!((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 126100789566373888) != 0))) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ExternDeclContext ------------------------------------------------------------------

ClythV1Parser::ExternDeclContext::ExternDeclContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ClythV1Parser::ExternDeclContext::EXTERN() {
  return getToken(ClythV1Parser::EXTERN, 0);
}

ClythV1Parser::TypeContext* ClythV1Parser::ExternDeclContext::type() {
  return getRuleContext<ClythV1Parser::TypeContext>(0);
}

tree::TerminalNode* ClythV1Parser::ExternDeclContext::IDENTIFIER() {
  return getToken(ClythV1Parser::IDENTIFIER, 0);
}

ClythV1Parser::ExternAbiContext* ClythV1Parser::ExternDeclContext::externAbi() {
  return getRuleContext<ClythV1Parser::ExternAbiContext>(0);
}

ClythV1Parser::ExternParamListContext* ClythV1Parser::ExternDeclContext::externParamList() {
  return getRuleContext<ClythV1Parser::ExternParamListContext>(0);
}

tree::TerminalNode* ClythV1Parser::ExternDeclContext::SEMI() {
  return getToken(ClythV1Parser::SEMI, 0);
}


size_t ClythV1Parser::ExternDeclContext::getRuleIndex() const {
  return ClythV1Parser::RuleExternDecl;
}

void ClythV1Parser::ExternDeclContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterExternDecl(this);
}

void ClythV1Parser::ExternDeclContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitExternDecl(this);
}

ClythV1Parser::ExternDeclContext* ClythV1Parser::externDecl() {
  ExternDeclContext *_localctx = _tracker.createInstance<ExternDeclContext>(_ctx, getState());
  enterRule(_localctx, 8, ClythV1Parser::RuleExternDecl);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(146);
    match(ClythV1Parser::EXTERN);
    setState(148);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 3, _ctx)) {
    case 1: {
      setState(147);
      externAbi();
      break;
    }

    default:
      break;
    }
    setState(150);
    type();
    setState(151);
    match(ClythV1Parser::IDENTIFIER);
    setState(152);
    match(ClythV1Parser::T__0);
    setState(154);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 73746443898191872) != 0)) {
      setState(153);
      externParamList();
    }
    setState(156);
    match(ClythV1Parser::T__1);
    setState(158);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ClythV1Parser::SEMI) {
      setState(157);
      match(ClythV1Parser::SEMI);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ExternAbiContext ------------------------------------------------------------------

ClythV1Parser::ExternAbiContext::ExternAbiContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ClythV1Parser::ExternAbiContext::C_ABI() {
  return getToken(ClythV1Parser::C_ABI, 0);
}

tree::TerminalNode* ClythV1Parser::ExternAbiContext::IDENTIFIER() {
  return getToken(ClythV1Parser::IDENTIFIER, 0);
}


size_t ClythV1Parser::ExternAbiContext::getRuleIndex() const {
  return ClythV1Parser::RuleExternAbi;
}

void ClythV1Parser::ExternAbiContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterExternAbi(this);
}

void ClythV1Parser::ExternAbiContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitExternAbi(this);
}

ClythV1Parser::ExternAbiContext* ClythV1Parser::externAbi() {
  ExternAbiContext *_localctx = _tracker.createInstance<ExternAbiContext>(_ctx, getState());
  enterRule(_localctx, 10, ClythV1Parser::RuleExternAbi);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(160);
    _la = _input->LA(1);
    if (!(_la == ClythV1Parser::C_ABI

    || _la == ClythV1Parser::IDENTIFIER)) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ExternParamListContext ------------------------------------------------------------------

ClythV1Parser::ExternParamListContext::ExternParamListContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<ClythV1Parser::ExternParamContext *> ClythV1Parser::ExternParamListContext::externParam() {
  return getRuleContexts<ClythV1Parser::ExternParamContext>();
}

ClythV1Parser::ExternParamContext* ClythV1Parser::ExternParamListContext::externParam(size_t i) {
  return getRuleContext<ClythV1Parser::ExternParamContext>(i);
}

tree::TerminalNode* ClythV1Parser::ExternParamListContext::VARARGS() {
  return getToken(ClythV1Parser::VARARGS, 0);
}


size_t ClythV1Parser::ExternParamListContext::getRuleIndex() const {
  return ClythV1Parser::RuleExternParamList;
}

void ClythV1Parser::ExternParamListContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterExternParamList(this);
}

void ClythV1Parser::ExternParamListContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitExternParamList(this);
}

ClythV1Parser::ExternParamListContext* ClythV1Parser::externParamList() {
  ExternParamListContext *_localctx = _tracker.createInstance<ExternParamListContext>(_ctx, getState());
  enterRule(_localctx, 12, ClythV1Parser::RuleExternParamList);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    size_t alt;
    setState(177);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case ClythV1Parser::BUILTIN_TYPE:
      case ClythV1Parser::IDENTIFIER: {
        enterOuterAlt(_localctx, 1);
        setState(162);
        externParam();
        setState(167);
        _errHandler->sync(this);
        alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 6, _ctx);
        while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
          if (alt == 1) {
            setState(163);
            match(ClythV1Parser::T__2);
            setState(164);
            externParam(); 
          }
          setState(169);
          _errHandler->sync(this);
          alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 6, _ctx);
        }
        setState(171);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == ClythV1Parser::T__2) {
          setState(170);
          match(ClythV1Parser::T__2);
        }
        setState(174);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == ClythV1Parser::VARARGS) {
          setState(173);
          match(ClythV1Parser::VARARGS);
        }
        break;
      }

      case ClythV1Parser::VARARGS: {
        enterOuterAlt(_localctx, 2);
        setState(176);
        match(ClythV1Parser::VARARGS);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ExternParamContext ------------------------------------------------------------------

ClythV1Parser::ExternParamContext::ExternParamContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ClythV1Parser::TypeContext* ClythV1Parser::ExternParamContext::type() {
  return getRuleContext<ClythV1Parser::TypeContext>(0);
}

tree::TerminalNode* ClythV1Parser::ExternParamContext::IDENTIFIER() {
  return getToken(ClythV1Parser::IDENTIFIER, 0);
}


size_t ClythV1Parser::ExternParamContext::getRuleIndex() const {
  return ClythV1Parser::RuleExternParam;
}

void ClythV1Parser::ExternParamContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterExternParam(this);
}

void ClythV1Parser::ExternParamContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitExternParam(this);
}

ClythV1Parser::ExternParamContext* ClythV1Parser::externParam() {
  ExternParamContext *_localctx = _tracker.createInstance<ExternParamContext>(_ctx, getState());
  enterRule(_localctx, 14, ClythV1Parser::RuleExternParam);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(179);
    type();
    setState(181);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ClythV1Parser::IDENTIFIER) {
      setState(180);
      match(ClythV1Parser::IDENTIFIER);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- StructDeclContext ------------------------------------------------------------------

ClythV1Parser::StructDeclContext::StructDeclContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ClythV1Parser::StructDeclContext::STRUCT() {
  return getToken(ClythV1Parser::STRUCT, 0);
}

tree::TerminalNode* ClythV1Parser::StructDeclContext::IDENTIFIER() {
  return getToken(ClythV1Parser::IDENTIFIER, 0);
}

std::vector<ClythV1Parser::StructFieldContext *> ClythV1Parser::StructDeclContext::structField() {
  return getRuleContexts<ClythV1Parser::StructFieldContext>();
}

ClythV1Parser::StructFieldContext* ClythV1Parser::StructDeclContext::structField(size_t i) {
  return getRuleContext<ClythV1Parser::StructFieldContext>(i);
}

tree::TerminalNode* ClythV1Parser::StructDeclContext::SEMI() {
  return getToken(ClythV1Parser::SEMI, 0);
}


size_t ClythV1Parser::StructDeclContext::getRuleIndex() const {
  return ClythV1Parser::RuleStructDecl;
}

void ClythV1Parser::StructDeclContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterStructDecl(this);
}

void ClythV1Parser::StructDeclContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitStructDecl(this);
}

ClythV1Parser::StructDeclContext* ClythV1Parser::structDecl() {
  StructDeclContext *_localctx = _tracker.createInstance<StructDeclContext>(_ctx, getState());
  enterRule(_localctx, 16, ClythV1Parser::RuleStructDecl);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(183);
    match(ClythV1Parser::STRUCT);
    setState(184);
    match(ClythV1Parser::IDENTIFIER);
    setState(185);
    match(ClythV1Parser::T__3);
    setState(189);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == ClythV1Parser::BUILTIN_TYPE

    || _la == ClythV1Parser::IDENTIFIER) {
      setState(186);
      structField();
      setState(191);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(192);
    match(ClythV1Parser::T__4);
    setState(194);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ClythV1Parser::SEMI) {
      setState(193);
      match(ClythV1Parser::SEMI);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- StructFieldContext ------------------------------------------------------------------

ClythV1Parser::StructFieldContext::StructFieldContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ClythV1Parser::TypeContext* ClythV1Parser::StructFieldContext::type() {
  return getRuleContext<ClythV1Parser::TypeContext>(0);
}

tree::TerminalNode* ClythV1Parser::StructFieldContext::IDENTIFIER() {
  return getToken(ClythV1Parser::IDENTIFIER, 0);
}

ClythV1Parser::CommaOrSemiContext* ClythV1Parser::StructFieldContext::commaOrSemi() {
  return getRuleContext<ClythV1Parser::CommaOrSemiContext>(0);
}


size_t ClythV1Parser::StructFieldContext::getRuleIndex() const {
  return ClythV1Parser::RuleStructField;
}

void ClythV1Parser::StructFieldContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterStructField(this);
}

void ClythV1Parser::StructFieldContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitStructField(this);
}

ClythV1Parser::StructFieldContext* ClythV1Parser::structField() {
  StructFieldContext *_localctx = _tracker.createInstance<StructFieldContext>(_ctx, getState());
  enterRule(_localctx, 18, ClythV1Parser::RuleStructField);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(196);
    type();
    setState(197);
    match(ClythV1Parser::IDENTIFIER);
    setState(199);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ClythV1Parser::T__2

    || _la == ClythV1Parser::SEMI) {
      setState(198);
      commaOrSemi();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- FunctionDeclContext ------------------------------------------------------------------

ClythV1Parser::FunctionDeclContext::FunctionDeclContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ClythV1Parser::TypeContext* ClythV1Parser::FunctionDeclContext::type() {
  return getRuleContext<ClythV1Parser::TypeContext>(0);
}

tree::TerminalNode* ClythV1Parser::FunctionDeclContext::IDENTIFIER() {
  return getToken(ClythV1Parser::IDENTIFIER, 0);
}

ClythV1Parser::BlockContext* ClythV1Parser::FunctionDeclContext::block() {
  return getRuleContext<ClythV1Parser::BlockContext>(0);
}

ClythV1Parser::ParamListContext* ClythV1Parser::FunctionDeclContext::paramList() {
  return getRuleContext<ClythV1Parser::ParamListContext>(0);
}


size_t ClythV1Parser::FunctionDeclContext::getRuleIndex() const {
  return ClythV1Parser::RuleFunctionDecl;
}

void ClythV1Parser::FunctionDeclContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterFunctionDecl(this);
}

void ClythV1Parser::FunctionDeclContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitFunctionDecl(this);
}

ClythV1Parser::FunctionDeclContext* ClythV1Parser::functionDecl() {
  FunctionDeclContext *_localctx = _tracker.createInstance<FunctionDeclContext>(_ctx, getState());
  enterRule(_localctx, 20, ClythV1Parser::RuleFunctionDecl);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(201);
    type();
    setState(202);
    match(ClythV1Parser::IDENTIFIER);
    setState(203);
    match(ClythV1Parser::T__0);
    setState(205);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ClythV1Parser::BUILTIN_TYPE

    || _la == ClythV1Parser::IDENTIFIER) {
      setState(204);
      paramList();
    }
    setState(207);
    match(ClythV1Parser::T__1);
    setState(208);
    block();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ParamListContext ------------------------------------------------------------------

ClythV1Parser::ParamListContext::ParamListContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<ClythV1Parser::ParamContext *> ClythV1Parser::ParamListContext::param() {
  return getRuleContexts<ClythV1Parser::ParamContext>();
}

ClythV1Parser::ParamContext* ClythV1Parser::ParamListContext::param(size_t i) {
  return getRuleContext<ClythV1Parser::ParamContext>(i);
}


size_t ClythV1Parser::ParamListContext::getRuleIndex() const {
  return ClythV1Parser::RuleParamList;
}

void ClythV1Parser::ParamListContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterParamList(this);
}

void ClythV1Parser::ParamListContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitParamList(this);
}

ClythV1Parser::ParamListContext* ClythV1Parser::paramList() {
  ParamListContext *_localctx = _tracker.createInstance<ParamListContext>(_ctx, getState());
  enterRule(_localctx, 22, ClythV1Parser::RuleParamList);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(210);
    param();
    setState(215);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 15, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(211);
        match(ClythV1Parser::T__2);
        setState(212);
        param(); 
      }
      setState(217);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 15, _ctx);
    }
    setState(219);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ClythV1Parser::T__2) {
      setState(218);
      match(ClythV1Parser::T__2);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ParamContext ------------------------------------------------------------------

ClythV1Parser::ParamContext::ParamContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ClythV1Parser::TypeContext* ClythV1Parser::ParamContext::type() {
  return getRuleContext<ClythV1Parser::TypeContext>(0);
}

tree::TerminalNode* ClythV1Parser::ParamContext::IDENTIFIER() {
  return getToken(ClythV1Parser::IDENTIFIER, 0);
}


size_t ClythV1Parser::ParamContext::getRuleIndex() const {
  return ClythV1Parser::RuleParam;
}

void ClythV1Parser::ParamContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterParam(this);
}

void ClythV1Parser::ParamContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitParam(this);
}

ClythV1Parser::ParamContext* ClythV1Parser::param() {
  ParamContext *_localctx = _tracker.createInstance<ParamContext>(_ctx, getState());
  enterRule(_localctx, 24, ClythV1Parser::RuleParam);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(221);
    type();
    setState(222);
    match(ClythV1Parser::IDENTIFIER);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- BlockContext ------------------------------------------------------------------

ClythV1Parser::BlockContext::BlockContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<ClythV1Parser::StatementContext *> ClythV1Parser::BlockContext::statement() {
  return getRuleContexts<ClythV1Parser::StatementContext>();
}

ClythV1Parser::StatementContext* ClythV1Parser::BlockContext::statement(size_t i) {
  return getRuleContext<ClythV1Parser::StatementContext>(i);
}


size_t ClythV1Parser::BlockContext::getRuleIndex() const {
  return ClythV1Parser::RuleBlock;
}

void ClythV1Parser::BlockContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterBlock(this);
}

void ClythV1Parser::BlockContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitBlock(this);
}

ClythV1Parser::BlockContext* ClythV1Parser::block() {
  BlockContext *_localctx = _tracker.createInstance<BlockContext>(_ctx, getState());
  enterRule(_localctx, 26, ClythV1Parser::RuleBlock);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(224);
    match(ClythV1Parser::T__3);
    setState(228);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 143237778130866194) != 0)) {
      setState(225);
      statement();
      setState(230);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(231);
    match(ClythV1Parser::T__4);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- StatementContext ------------------------------------------------------------------

ClythV1Parser::StatementContext::StatementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ClythV1Parser::VarDeclContext* ClythV1Parser::StatementContext::varDecl() {
  return getRuleContext<ClythV1Parser::VarDeclContext>(0);
}

ClythV1Parser::AssignmentStmtContext* ClythV1Parser::StatementContext::assignmentStmt() {
  return getRuleContext<ClythV1Parser::AssignmentStmtContext>(0);
}

ClythV1Parser::ExprStmtContext* ClythV1Parser::StatementContext::exprStmt() {
  return getRuleContext<ClythV1Parser::ExprStmtContext>(0);
}

ClythV1Parser::ReturnStmtContext* ClythV1Parser::StatementContext::returnStmt() {
  return getRuleContext<ClythV1Parser::ReturnStmtContext>(0);
}

ClythV1Parser::IfStmtContext* ClythV1Parser::StatementContext::ifStmt() {
  return getRuleContext<ClythV1Parser::IfStmtContext>(0);
}

ClythV1Parser::WhileStmtContext* ClythV1Parser::StatementContext::whileStmt() {
  return getRuleContext<ClythV1Parser::WhileStmtContext>(0);
}

ClythV1Parser::ForStmtContext* ClythV1Parser::StatementContext::forStmt() {
  return getRuleContext<ClythV1Parser::ForStmtContext>(0);
}

ClythV1Parser::BreakStmtContext* ClythV1Parser::StatementContext::breakStmt() {
  return getRuleContext<ClythV1Parser::BreakStmtContext>(0);
}

ClythV1Parser::ContinueStmtContext* ClythV1Parser::StatementContext::continueStmt() {
  return getRuleContext<ClythV1Parser::ContinueStmtContext>(0);
}

ClythV1Parser::BlockContext* ClythV1Parser::StatementContext::block() {
  return getRuleContext<ClythV1Parser::BlockContext>(0);
}


size_t ClythV1Parser::StatementContext::getRuleIndex() const {
  return ClythV1Parser::RuleStatement;
}

void ClythV1Parser::StatementContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterStatement(this);
}

void ClythV1Parser::StatementContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitStatement(this);
}

ClythV1Parser::StatementContext* ClythV1Parser::statement() {
  StatementContext *_localctx = _tracker.createInstance<StatementContext>(_ctx, getState());
  enterRule(_localctx, 28, ClythV1Parser::RuleStatement);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(243);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 18, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(233);
      varDecl();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(234);
      assignmentStmt();
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(235);
      exprStmt();
      break;
    }

    case 4: {
      enterOuterAlt(_localctx, 4);
      setState(236);
      returnStmt();
      break;
    }

    case 5: {
      enterOuterAlt(_localctx, 5);
      setState(237);
      ifStmt();
      break;
    }

    case 6: {
      enterOuterAlt(_localctx, 6);
      setState(238);
      whileStmt();
      break;
    }

    case 7: {
      enterOuterAlt(_localctx, 7);
      setState(239);
      forStmt();
      break;
    }

    case 8: {
      enterOuterAlt(_localctx, 8);
      setState(240);
      breakStmt();
      break;
    }

    case 9: {
      enterOuterAlt(_localctx, 9);
      setState(241);
      continueStmt();
      break;
    }

    case 10: {
      enterOuterAlt(_localctx, 10);
      setState(242);
      block();
      break;
    }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- VarDeclContext ------------------------------------------------------------------

ClythV1Parser::VarDeclContext::VarDeclContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ClythV1Parser::TypeContext* ClythV1Parser::VarDeclContext::type() {
  return getRuleContext<ClythV1Parser::TypeContext>(0);
}

tree::TerminalNode* ClythV1Parser::VarDeclContext::IDENTIFIER() {
  return getToken(ClythV1Parser::IDENTIFIER, 0);
}

ClythV1Parser::ExpressionContext* ClythV1Parser::VarDeclContext::expression() {
  return getRuleContext<ClythV1Parser::ExpressionContext>(0);
}

tree::TerminalNode* ClythV1Parser::VarDeclContext::SEMI() {
  return getToken(ClythV1Parser::SEMI, 0);
}


size_t ClythV1Parser::VarDeclContext::getRuleIndex() const {
  return ClythV1Parser::RuleVarDecl;
}

void ClythV1Parser::VarDeclContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterVarDecl(this);
}

void ClythV1Parser::VarDeclContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitVarDecl(this);
}

ClythV1Parser::VarDeclContext* ClythV1Parser::varDecl() {
  VarDeclContext *_localctx = _tracker.createInstance<VarDeclContext>(_ctx, getState());
  enterRule(_localctx, 30, ClythV1Parser::RuleVarDecl);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(245);
    type();
    setState(246);
    match(ClythV1Parser::IDENTIFIER);
    setState(249);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ClythV1Parser::T__5) {
      setState(247);
      match(ClythV1Parser::T__5);
      setState(248);
      expression();
    }
    setState(252);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ClythV1Parser::SEMI) {
      setState(251);
      match(ClythV1Parser::SEMI);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- AssignmentStmtContext ------------------------------------------------------------------

ClythV1Parser::AssignmentStmtContext::AssignmentStmtContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ClythV1Parser::AssignableContext* ClythV1Parser::AssignmentStmtContext::assignable() {
  return getRuleContext<ClythV1Parser::AssignableContext>(0);
}

ClythV1Parser::AssignmentOpContext* ClythV1Parser::AssignmentStmtContext::assignmentOp() {
  return getRuleContext<ClythV1Parser::AssignmentOpContext>(0);
}

ClythV1Parser::ExpressionContext* ClythV1Parser::AssignmentStmtContext::expression() {
  return getRuleContext<ClythV1Parser::ExpressionContext>(0);
}

tree::TerminalNode* ClythV1Parser::AssignmentStmtContext::SEMI() {
  return getToken(ClythV1Parser::SEMI, 0);
}


size_t ClythV1Parser::AssignmentStmtContext::getRuleIndex() const {
  return ClythV1Parser::RuleAssignmentStmt;
}

void ClythV1Parser::AssignmentStmtContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterAssignmentStmt(this);
}

void ClythV1Parser::AssignmentStmtContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitAssignmentStmt(this);
}

ClythV1Parser::AssignmentStmtContext* ClythV1Parser::assignmentStmt() {
  AssignmentStmtContext *_localctx = _tracker.createInstance<AssignmentStmtContext>(_ctx, getState());
  enterRule(_localctx, 32, ClythV1Parser::RuleAssignmentStmt);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(254);
    assignable();
    setState(255);
    assignmentOp();
    setState(256);
    expression();
    setState(258);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ClythV1Parser::SEMI) {
      setState(257);
      match(ClythV1Parser::SEMI);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- AssignableContext ------------------------------------------------------------------

ClythV1Parser::AssignableContext::AssignableContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ClythV1Parser::AssignableContext::IDENTIFIER() {
  return getToken(ClythV1Parser::IDENTIFIER, 0);
}

std::vector<ClythV1Parser::PostfixSuffixContext *> ClythV1Parser::AssignableContext::postfixSuffix() {
  return getRuleContexts<ClythV1Parser::PostfixSuffixContext>();
}

ClythV1Parser::PostfixSuffixContext* ClythV1Parser::AssignableContext::postfixSuffix(size_t i) {
  return getRuleContext<ClythV1Parser::PostfixSuffixContext>(i);
}


size_t ClythV1Parser::AssignableContext::getRuleIndex() const {
  return ClythV1Parser::RuleAssignable;
}

void ClythV1Parser::AssignableContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterAssignable(this);
}

void ClythV1Parser::AssignableContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitAssignable(this);
}

ClythV1Parser::AssignableContext* ClythV1Parser::assignable() {
  AssignableContext *_localctx = _tracker.createInstance<AssignableContext>(_ctx, getState());
  enterRule(_localctx, 34, ClythV1Parser::RuleAssignable);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(260);
    match(ClythV1Parser::IDENTIFIER);
    setState(264);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 282299610435586) != 0)) {
      setState(261);
      postfixSuffix();
      setState(266);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- AssignmentOpContext ------------------------------------------------------------------

ClythV1Parser::AssignmentOpContext::AssignmentOpContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t ClythV1Parser::AssignmentOpContext::getRuleIndex() const {
  return ClythV1Parser::RuleAssignmentOp;
}

void ClythV1Parser::AssignmentOpContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterAssignmentOp(this);
}

void ClythV1Parser::AssignmentOpContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitAssignmentOp(this);
}

ClythV1Parser::AssignmentOpContext* ClythV1Parser::assignmentOp() {
  AssignmentOpContext *_localctx = _tracker.createInstance<AssignmentOpContext>(_ctx, getState());
  enterRule(_localctx, 36, ClythV1Parser::RuleAssignmentOp);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(267);
    _la = _input->LA(1);
    if (!((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 4032) != 0))) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ExprStmtContext ------------------------------------------------------------------

ClythV1Parser::ExprStmtContext::ExprStmtContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ClythV1Parser::ExpressionContext* ClythV1Parser::ExprStmtContext::expression() {
  return getRuleContext<ClythV1Parser::ExpressionContext>(0);
}

tree::TerminalNode* ClythV1Parser::ExprStmtContext::SEMI() {
  return getToken(ClythV1Parser::SEMI, 0);
}


size_t ClythV1Parser::ExprStmtContext::getRuleIndex() const {
  return ClythV1Parser::RuleExprStmt;
}

void ClythV1Parser::ExprStmtContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterExprStmt(this);
}

void ClythV1Parser::ExprStmtContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitExprStmt(this);
}

ClythV1Parser::ExprStmtContext* ClythV1Parser::exprStmt() {
  ExprStmtContext *_localctx = _tracker.createInstance<ExprStmtContext>(_ctx, getState());
  enterRule(_localctx, 38, ClythV1Parser::RuleExprStmt);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(269);
    expression();
    setState(271);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ClythV1Parser::SEMI) {
      setState(270);
      match(ClythV1Parser::SEMI);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ReturnStmtContext ------------------------------------------------------------------

ClythV1Parser::ReturnStmtContext::ReturnStmtContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ClythV1Parser::ReturnStmtContext::RETURN() {
  return getToken(ClythV1Parser::RETURN, 0);
}

ClythV1Parser::ExpressionContext* ClythV1Parser::ReturnStmtContext::expression() {
  return getRuleContext<ClythV1Parser::ExpressionContext>(0);
}

tree::TerminalNode* ClythV1Parser::ReturnStmtContext::SEMI() {
  return getToken(ClythV1Parser::SEMI, 0);
}


size_t ClythV1Parser::ReturnStmtContext::getRuleIndex() const {
  return ClythV1Parser::RuleReturnStmt;
}

void ClythV1Parser::ReturnStmtContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterReturnStmt(this);
}

void ClythV1Parser::ReturnStmtContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitReturnStmt(this);
}

ClythV1Parser::ReturnStmtContext* ClythV1Parser::returnStmt() {
  ReturnStmtContext *_localctx = _tracker.createInstance<ReturnStmtContext>(_ctx, getState());
  enterRule(_localctx, 40, ClythV1Parser::RuleReturnStmt);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(273);
    match(ClythV1Parser::RETURN);
    setState(275);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 24, _ctx)) {
    case 1: {
      setState(274);
      expression();
      break;
    }

    default:
      break;
    }
    setState(278);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ClythV1Parser::SEMI) {
      setState(277);
      match(ClythV1Parser::SEMI);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- IfStmtContext ------------------------------------------------------------------

ClythV1Parser::IfStmtContext::IfStmtContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ClythV1Parser::IfStmtContext::IF() {
  return getToken(ClythV1Parser::IF, 0);
}

ClythV1Parser::ParenExprContext* ClythV1Parser::IfStmtContext::parenExpr() {
  return getRuleContext<ClythV1Parser::ParenExprContext>(0);
}

ClythV1Parser::BlockContext* ClythV1Parser::IfStmtContext::block() {
  return getRuleContext<ClythV1Parser::BlockContext>(0);
}

ClythV1Parser::ElseClauseContext* ClythV1Parser::IfStmtContext::elseClause() {
  return getRuleContext<ClythV1Parser::ElseClauseContext>(0);
}

ClythV1Parser::ExpressionContext* ClythV1Parser::IfStmtContext::expression() {
  return getRuleContext<ClythV1Parser::ExpressionContext>(0);
}


size_t ClythV1Parser::IfStmtContext::getRuleIndex() const {
  return ClythV1Parser::RuleIfStmt;
}

void ClythV1Parser::IfStmtContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterIfStmt(this);
}

void ClythV1Parser::IfStmtContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitIfStmt(this);
}

ClythV1Parser::IfStmtContext* ClythV1Parser::ifStmt() {
  IfStmtContext *_localctx = _tracker.createInstance<IfStmtContext>(_ctx, getState());
  enterRule(_localctx, 42, ClythV1Parser::RuleIfStmt);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(292);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 28, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(280);
      match(ClythV1Parser::IF);
      setState(281);
      parenExpr();
      setState(282);
      block();
      setState(284);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == ClythV1Parser::ELSE) {
        setState(283);
        elseClause();
      }
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(286);
      match(ClythV1Parser::IF);
      setState(287);
      expression();
      setState(288);
      block();
      setState(290);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == ClythV1Parser::ELSE) {
        setState(289);
        elseClause();
      }
      break;
    }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ElseClauseContext ------------------------------------------------------------------

ClythV1Parser::ElseClauseContext::ElseClauseContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ClythV1Parser::ElseClauseContext::ELSE() {
  return getToken(ClythV1Parser::ELSE, 0);
}

ClythV1Parser::IfStmtContext* ClythV1Parser::ElseClauseContext::ifStmt() {
  return getRuleContext<ClythV1Parser::IfStmtContext>(0);
}

ClythV1Parser::BlockContext* ClythV1Parser::ElseClauseContext::block() {
  return getRuleContext<ClythV1Parser::BlockContext>(0);
}


size_t ClythV1Parser::ElseClauseContext::getRuleIndex() const {
  return ClythV1Parser::RuleElseClause;
}

void ClythV1Parser::ElseClauseContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterElseClause(this);
}

void ClythV1Parser::ElseClauseContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitElseClause(this);
}

ClythV1Parser::ElseClauseContext* ClythV1Parser::elseClause() {
  ElseClauseContext *_localctx = _tracker.createInstance<ElseClauseContext>(_ctx, getState());
  enterRule(_localctx, 44, ClythV1Parser::RuleElseClause);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(298);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 29, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(294);
      match(ClythV1Parser::ELSE);
      setState(295);
      ifStmt();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(296);
      match(ClythV1Parser::ELSE);
      setState(297);
      block();
      break;
    }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- WhileStmtContext ------------------------------------------------------------------

ClythV1Parser::WhileStmtContext::WhileStmtContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ClythV1Parser::WhileStmtContext::WHILE() {
  return getToken(ClythV1Parser::WHILE, 0);
}

ClythV1Parser::ParenExprContext* ClythV1Parser::WhileStmtContext::parenExpr() {
  return getRuleContext<ClythV1Parser::ParenExprContext>(0);
}

ClythV1Parser::BlockContext* ClythV1Parser::WhileStmtContext::block() {
  return getRuleContext<ClythV1Parser::BlockContext>(0);
}

ClythV1Parser::ExpressionContext* ClythV1Parser::WhileStmtContext::expression() {
  return getRuleContext<ClythV1Parser::ExpressionContext>(0);
}


size_t ClythV1Parser::WhileStmtContext::getRuleIndex() const {
  return ClythV1Parser::RuleWhileStmt;
}

void ClythV1Parser::WhileStmtContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterWhileStmt(this);
}

void ClythV1Parser::WhileStmtContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitWhileStmt(this);
}

ClythV1Parser::WhileStmtContext* ClythV1Parser::whileStmt() {
  WhileStmtContext *_localctx = _tracker.createInstance<WhileStmtContext>(_ctx, getState());
  enterRule(_localctx, 46, ClythV1Parser::RuleWhileStmt);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(308);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 30, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(300);
      match(ClythV1Parser::WHILE);
      setState(301);
      parenExpr();
      setState(302);
      block();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(304);
      match(ClythV1Parser::WHILE);
      setState(305);
      expression();
      setState(306);
      block();
      break;
    }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ForStmtContext ------------------------------------------------------------------

ClythV1Parser::ForStmtContext::ForStmtContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ClythV1Parser::ForStmtContext::FOR() {
  return getToken(ClythV1Parser::FOR, 0);
}

ClythV1Parser::BlockContext* ClythV1Parser::ForStmtContext::block() {
  return getRuleContext<ClythV1Parser::BlockContext>(0);
}

ClythV1Parser::ForEachHeaderContext* ClythV1Parser::ForStmtContext::forEachHeader() {
  return getRuleContext<ClythV1Parser::ForEachHeaderContext>(0);
}

ClythV1Parser::ForClassicHeaderContext* ClythV1Parser::ForStmtContext::forClassicHeader() {
  return getRuleContext<ClythV1Parser::ForClassicHeaderContext>(0);
}


size_t ClythV1Parser::ForStmtContext::getRuleIndex() const {
  return ClythV1Parser::RuleForStmt;
}

void ClythV1Parser::ForStmtContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterForStmt(this);
}

void ClythV1Parser::ForStmtContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitForStmt(this);
}

ClythV1Parser::ForStmtContext* ClythV1Parser::forStmt() {
  ForStmtContext *_localctx = _tracker.createInstance<ForStmtContext>(_ctx, getState());
  enterRule(_localctx, 48, ClythV1Parser::RuleForStmt);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(332);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 31, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(310);
      match(ClythV1Parser::FOR);
      setState(311);
      block();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(312);
      match(ClythV1Parser::FOR);
      setState(313);
      forEachHeader();
      setState(314);
      block();
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(316);
      match(ClythV1Parser::FOR);
      setState(317);
      match(ClythV1Parser::T__0);
      setState(318);
      forEachHeader();
      setState(319);
      match(ClythV1Parser::T__1);
      setState(320);
      block();
      break;
    }

    case 4: {
      enterOuterAlt(_localctx, 4);
      setState(322);
      match(ClythV1Parser::FOR);
      setState(323);
      forClassicHeader();
      setState(324);
      block();
      break;
    }

    case 5: {
      enterOuterAlt(_localctx, 5);
      setState(326);
      match(ClythV1Parser::FOR);
      setState(327);
      match(ClythV1Parser::T__0);
      setState(328);
      forClassicHeader();
      setState(329);
      match(ClythV1Parser::T__1);
      setState(330);
      block();
      break;
    }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ForEachHeaderContext ------------------------------------------------------------------

ClythV1Parser::ForEachHeaderContext::ForEachHeaderContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ClythV1Parser::ForEachBindingContext* ClythV1Parser::ForEachHeaderContext::forEachBinding() {
  return getRuleContext<ClythV1Parser::ForEachBindingContext>(0);
}

tree::TerminalNode* ClythV1Parser::ForEachHeaderContext::IN() {
  return getToken(ClythV1Parser::IN, 0);
}

ClythV1Parser::ExpressionContext* ClythV1Parser::ForEachHeaderContext::expression() {
  return getRuleContext<ClythV1Parser::ExpressionContext>(0);
}


size_t ClythV1Parser::ForEachHeaderContext::getRuleIndex() const {
  return ClythV1Parser::RuleForEachHeader;
}

void ClythV1Parser::ForEachHeaderContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterForEachHeader(this);
}

void ClythV1Parser::ForEachHeaderContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitForEachHeader(this);
}

ClythV1Parser::ForEachHeaderContext* ClythV1Parser::forEachHeader() {
  ForEachHeaderContext *_localctx = _tracker.createInstance<ForEachHeaderContext>(_ctx, getState());
  enterRule(_localctx, 50, ClythV1Parser::RuleForEachHeader);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(334);
    forEachBinding();
    setState(335);
    match(ClythV1Parser::IN);
    setState(336);
    expression();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ForEachBindingContext ------------------------------------------------------------------

ClythV1Parser::ForEachBindingContext::ForEachBindingContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<ClythV1Parser::ForEachVarContext *> ClythV1Parser::ForEachBindingContext::forEachVar() {
  return getRuleContexts<ClythV1Parser::ForEachVarContext>();
}

ClythV1Parser::ForEachVarContext* ClythV1Parser::ForEachBindingContext::forEachVar(size_t i) {
  return getRuleContext<ClythV1Parser::ForEachVarContext>(i);
}


size_t ClythV1Parser::ForEachBindingContext::getRuleIndex() const {
  return ClythV1Parser::RuleForEachBinding;
}

void ClythV1Parser::ForEachBindingContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterForEachBinding(this);
}

void ClythV1Parser::ForEachBindingContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitForEachBinding(this);
}

ClythV1Parser::ForEachBindingContext* ClythV1Parser::forEachBinding() {
  ForEachBindingContext *_localctx = _tracker.createInstance<ForEachBindingContext>(_ctx, getState());
  enterRule(_localctx, 52, ClythV1Parser::RuleForEachBinding);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(338);
    forEachVar();
    setState(343);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == ClythV1Parser::T__2) {
      setState(339);
      match(ClythV1Parser::T__2);
      setState(340);
      forEachVar();
      setState(345);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ForEachVarContext ------------------------------------------------------------------

ClythV1Parser::ForEachVarContext::ForEachVarContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ClythV1Parser::TypeContext* ClythV1Parser::ForEachVarContext::type() {
  return getRuleContext<ClythV1Parser::TypeContext>(0);
}

tree::TerminalNode* ClythV1Parser::ForEachVarContext::IDENTIFIER() {
  return getToken(ClythV1Parser::IDENTIFIER, 0);
}


size_t ClythV1Parser::ForEachVarContext::getRuleIndex() const {
  return ClythV1Parser::RuleForEachVar;
}

void ClythV1Parser::ForEachVarContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterForEachVar(this);
}

void ClythV1Parser::ForEachVarContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitForEachVar(this);
}

ClythV1Parser::ForEachVarContext* ClythV1Parser::forEachVar() {
  ForEachVarContext *_localctx = _tracker.createInstance<ForEachVarContext>(_ctx, getState());
  enterRule(_localctx, 54, ClythV1Parser::RuleForEachVar);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(350);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 33, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(346);
      type();
      setState(347);
      match(ClythV1Parser::IDENTIFIER);
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(349);
      match(ClythV1Parser::IDENTIFIER);
      break;
    }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ForClassicHeaderContext ------------------------------------------------------------------

ClythV1Parser::ForClassicHeaderContext::ForClassicHeaderContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<tree::TerminalNode *> ClythV1Parser::ForClassicHeaderContext::SEMI() {
  return getTokens(ClythV1Parser::SEMI);
}

tree::TerminalNode* ClythV1Parser::ForClassicHeaderContext::SEMI(size_t i) {
  return getToken(ClythV1Parser::SEMI, i);
}

ClythV1Parser::ForInitContext* ClythV1Parser::ForClassicHeaderContext::forInit() {
  return getRuleContext<ClythV1Parser::ForInitContext>(0);
}

std::vector<ClythV1Parser::ExpressionContext *> ClythV1Parser::ForClassicHeaderContext::expression() {
  return getRuleContexts<ClythV1Parser::ExpressionContext>();
}

ClythV1Parser::ExpressionContext* ClythV1Parser::ForClassicHeaderContext::expression(size_t i) {
  return getRuleContext<ClythV1Parser::ExpressionContext>(i);
}


size_t ClythV1Parser::ForClassicHeaderContext::getRuleIndex() const {
  return ClythV1Parser::RuleForClassicHeader;
}

void ClythV1Parser::ForClassicHeaderContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterForClassicHeader(this);
}

void ClythV1Parser::ForClassicHeaderContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitForClassicHeader(this);
}

ClythV1Parser::ForClassicHeaderContext* ClythV1Parser::forClassicHeader() {
  ForClassicHeaderContext *_localctx = _tracker.createInstance<ForClassicHeaderContext>(_ctx, getState());
  enterRule(_localctx, 56, ClythV1Parser::RuleForClassicHeader);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(353);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 143237778065330194) != 0)) {
      setState(352);
      forInit();
    }
    setState(355);
    match(ClythV1Parser::SEMI);
    setState(357);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 142111878158487570) != 0)) {
      setState(356);
      expression();
    }
    setState(359);
    match(ClythV1Parser::SEMI);
    setState(361);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 36, _ctx)) {
    case 1: {
      setState(360);
      expression();
      break;
    }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ForInitContext ------------------------------------------------------------------

ClythV1Parser::ForInitContext::ForInitContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ClythV1Parser::VarDeclNoSemiContext* ClythV1Parser::ForInitContext::varDeclNoSemi() {
  return getRuleContext<ClythV1Parser::VarDeclNoSemiContext>(0);
}

ClythV1Parser::AssignmentNoSemiContext* ClythV1Parser::ForInitContext::assignmentNoSemi() {
  return getRuleContext<ClythV1Parser::AssignmentNoSemiContext>(0);
}

ClythV1Parser::ExpressionContext* ClythV1Parser::ForInitContext::expression() {
  return getRuleContext<ClythV1Parser::ExpressionContext>(0);
}


size_t ClythV1Parser::ForInitContext::getRuleIndex() const {
  return ClythV1Parser::RuleForInit;
}

void ClythV1Parser::ForInitContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterForInit(this);
}

void ClythV1Parser::ForInitContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitForInit(this);
}

ClythV1Parser::ForInitContext* ClythV1Parser::forInit() {
  ForInitContext *_localctx = _tracker.createInstance<ForInitContext>(_ctx, getState());
  enterRule(_localctx, 58, ClythV1Parser::RuleForInit);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(366);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 37, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(363);
      varDeclNoSemi();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(364);
      assignmentNoSemi();
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(365);
      expression();
      break;
    }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- VarDeclNoSemiContext ------------------------------------------------------------------

ClythV1Parser::VarDeclNoSemiContext::VarDeclNoSemiContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ClythV1Parser::TypeContext* ClythV1Parser::VarDeclNoSemiContext::type() {
  return getRuleContext<ClythV1Parser::TypeContext>(0);
}

tree::TerminalNode* ClythV1Parser::VarDeclNoSemiContext::IDENTIFIER() {
  return getToken(ClythV1Parser::IDENTIFIER, 0);
}

ClythV1Parser::ExpressionContext* ClythV1Parser::VarDeclNoSemiContext::expression() {
  return getRuleContext<ClythV1Parser::ExpressionContext>(0);
}


size_t ClythV1Parser::VarDeclNoSemiContext::getRuleIndex() const {
  return ClythV1Parser::RuleVarDeclNoSemi;
}

void ClythV1Parser::VarDeclNoSemiContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterVarDeclNoSemi(this);
}

void ClythV1Parser::VarDeclNoSemiContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitVarDeclNoSemi(this);
}

ClythV1Parser::VarDeclNoSemiContext* ClythV1Parser::varDeclNoSemi() {
  VarDeclNoSemiContext *_localctx = _tracker.createInstance<VarDeclNoSemiContext>(_ctx, getState());
  enterRule(_localctx, 60, ClythV1Parser::RuleVarDeclNoSemi);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(368);
    type();
    setState(369);
    match(ClythV1Parser::IDENTIFIER);
    setState(372);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ClythV1Parser::T__5) {
      setState(370);
      match(ClythV1Parser::T__5);
      setState(371);
      expression();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- AssignmentNoSemiContext ------------------------------------------------------------------

ClythV1Parser::AssignmentNoSemiContext::AssignmentNoSemiContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ClythV1Parser::AssignableContext* ClythV1Parser::AssignmentNoSemiContext::assignable() {
  return getRuleContext<ClythV1Parser::AssignableContext>(0);
}

ClythV1Parser::AssignmentOpContext* ClythV1Parser::AssignmentNoSemiContext::assignmentOp() {
  return getRuleContext<ClythV1Parser::AssignmentOpContext>(0);
}

ClythV1Parser::ExpressionContext* ClythV1Parser::AssignmentNoSemiContext::expression() {
  return getRuleContext<ClythV1Parser::ExpressionContext>(0);
}


size_t ClythV1Parser::AssignmentNoSemiContext::getRuleIndex() const {
  return ClythV1Parser::RuleAssignmentNoSemi;
}

void ClythV1Parser::AssignmentNoSemiContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterAssignmentNoSemi(this);
}

void ClythV1Parser::AssignmentNoSemiContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitAssignmentNoSemi(this);
}

ClythV1Parser::AssignmentNoSemiContext* ClythV1Parser::assignmentNoSemi() {
  AssignmentNoSemiContext *_localctx = _tracker.createInstance<AssignmentNoSemiContext>(_ctx, getState());
  enterRule(_localctx, 62, ClythV1Parser::RuleAssignmentNoSemi);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(374);
    assignable();
    setState(375);
    assignmentOp();
    setState(376);
    expression();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- BreakStmtContext ------------------------------------------------------------------

ClythV1Parser::BreakStmtContext::BreakStmtContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ClythV1Parser::BreakStmtContext::BREAK() {
  return getToken(ClythV1Parser::BREAK, 0);
}

tree::TerminalNode* ClythV1Parser::BreakStmtContext::SEMI() {
  return getToken(ClythV1Parser::SEMI, 0);
}


size_t ClythV1Parser::BreakStmtContext::getRuleIndex() const {
  return ClythV1Parser::RuleBreakStmt;
}

void ClythV1Parser::BreakStmtContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterBreakStmt(this);
}

void ClythV1Parser::BreakStmtContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitBreakStmt(this);
}

ClythV1Parser::BreakStmtContext* ClythV1Parser::breakStmt() {
  BreakStmtContext *_localctx = _tracker.createInstance<BreakStmtContext>(_ctx, getState());
  enterRule(_localctx, 64, ClythV1Parser::RuleBreakStmt);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(378);
    match(ClythV1Parser::BREAK);
    setState(380);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ClythV1Parser::SEMI) {
      setState(379);
      match(ClythV1Parser::SEMI);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ContinueStmtContext ------------------------------------------------------------------

ClythV1Parser::ContinueStmtContext::ContinueStmtContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ClythV1Parser::ContinueStmtContext::CONTINUE() {
  return getToken(ClythV1Parser::CONTINUE, 0);
}

tree::TerminalNode* ClythV1Parser::ContinueStmtContext::SEMI() {
  return getToken(ClythV1Parser::SEMI, 0);
}


size_t ClythV1Parser::ContinueStmtContext::getRuleIndex() const {
  return ClythV1Parser::RuleContinueStmt;
}

void ClythV1Parser::ContinueStmtContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterContinueStmt(this);
}

void ClythV1Parser::ContinueStmtContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitContinueStmt(this);
}

ClythV1Parser::ContinueStmtContext* ClythV1Parser::continueStmt() {
  ContinueStmtContext *_localctx = _tracker.createInstance<ContinueStmtContext>(_ctx, getState());
  enterRule(_localctx, 66, ClythV1Parser::RuleContinueStmt);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(382);
    match(ClythV1Parser::CONTINUE);
    setState(384);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ClythV1Parser::SEMI) {
      setState(383);
      match(ClythV1Parser::SEMI);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ParenExprContext ------------------------------------------------------------------

ClythV1Parser::ParenExprContext::ParenExprContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ClythV1Parser::ExpressionContext* ClythV1Parser::ParenExprContext::expression() {
  return getRuleContext<ClythV1Parser::ExpressionContext>(0);
}


size_t ClythV1Parser::ParenExprContext::getRuleIndex() const {
  return ClythV1Parser::RuleParenExpr;
}

void ClythV1Parser::ParenExprContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterParenExpr(this);
}

void ClythV1Parser::ParenExprContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitParenExpr(this);
}

ClythV1Parser::ParenExprContext* ClythV1Parser::parenExpr() {
  ParenExprContext *_localctx = _tracker.createInstance<ParenExprContext>(_ctx, getState());
  enterRule(_localctx, 68, ClythV1Parser::RuleParenExpr);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(386);
    match(ClythV1Parser::T__0);
    setState(387);
    expression();
    setState(388);
    match(ClythV1Parser::T__1);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ExpressionContext ------------------------------------------------------------------

ClythV1Parser::ExpressionContext::ExpressionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ClythV1Parser::LogicalOrContext* ClythV1Parser::ExpressionContext::logicalOr() {
  return getRuleContext<ClythV1Parser::LogicalOrContext>(0);
}


size_t ClythV1Parser::ExpressionContext::getRuleIndex() const {
  return ClythV1Parser::RuleExpression;
}

void ClythV1Parser::ExpressionContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterExpression(this);
}

void ClythV1Parser::ExpressionContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitExpression(this);
}

ClythV1Parser::ExpressionContext* ClythV1Parser::expression() {
  ExpressionContext *_localctx = _tracker.createInstance<ExpressionContext>(_ctx, getState());
  enterRule(_localctx, 70, ClythV1Parser::RuleExpression);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(390);
    logicalOr();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- LogicalOrContext ------------------------------------------------------------------

ClythV1Parser::LogicalOrContext::LogicalOrContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<ClythV1Parser::LogicalAndContext *> ClythV1Parser::LogicalOrContext::logicalAnd() {
  return getRuleContexts<ClythV1Parser::LogicalAndContext>();
}

ClythV1Parser::LogicalAndContext* ClythV1Parser::LogicalOrContext::logicalAnd(size_t i) {
  return getRuleContext<ClythV1Parser::LogicalAndContext>(i);
}

std::vector<tree::TerminalNode *> ClythV1Parser::LogicalOrContext::OR() {
  return getTokens(ClythV1Parser::OR);
}

tree::TerminalNode* ClythV1Parser::LogicalOrContext::OR(size_t i) {
  return getToken(ClythV1Parser::OR, i);
}


size_t ClythV1Parser::LogicalOrContext::getRuleIndex() const {
  return ClythV1Parser::RuleLogicalOr;
}

void ClythV1Parser::LogicalOrContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterLogicalOr(this);
}

void ClythV1Parser::LogicalOrContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitLogicalOr(this);
}

ClythV1Parser::LogicalOrContext* ClythV1Parser::logicalOr() {
  LogicalOrContext *_localctx = _tracker.createInstance<LogicalOrContext>(_ctx, getState());
  enterRule(_localctx, 72, ClythV1Parser::RuleLogicalOr);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(392);
    logicalAnd();
    setState(397);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == ClythV1Parser::OR) {
      setState(393);
      match(ClythV1Parser::OR);
      setState(394);
      logicalAnd();
      setState(399);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- LogicalAndContext ------------------------------------------------------------------

ClythV1Parser::LogicalAndContext::LogicalAndContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<ClythV1Parser::EqualityContext *> ClythV1Parser::LogicalAndContext::equality() {
  return getRuleContexts<ClythV1Parser::EqualityContext>();
}

ClythV1Parser::EqualityContext* ClythV1Parser::LogicalAndContext::equality(size_t i) {
  return getRuleContext<ClythV1Parser::EqualityContext>(i);
}

std::vector<tree::TerminalNode *> ClythV1Parser::LogicalAndContext::AND() {
  return getTokens(ClythV1Parser::AND);
}

tree::TerminalNode* ClythV1Parser::LogicalAndContext::AND(size_t i) {
  return getToken(ClythV1Parser::AND, i);
}


size_t ClythV1Parser::LogicalAndContext::getRuleIndex() const {
  return ClythV1Parser::RuleLogicalAnd;
}

void ClythV1Parser::LogicalAndContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterLogicalAnd(this);
}

void ClythV1Parser::LogicalAndContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitLogicalAnd(this);
}

ClythV1Parser::LogicalAndContext* ClythV1Parser::logicalAnd() {
  LogicalAndContext *_localctx = _tracker.createInstance<LogicalAndContext>(_ctx, getState());
  enterRule(_localctx, 74, ClythV1Parser::RuleLogicalAnd);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(400);
    equality();
    setState(405);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == ClythV1Parser::AND) {
      setState(401);
      match(ClythV1Parser::AND);
      setState(402);
      equality();
      setState(407);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- EqualityContext ------------------------------------------------------------------

ClythV1Parser::EqualityContext::EqualityContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<ClythV1Parser::RelationContext *> ClythV1Parser::EqualityContext::relation() {
  return getRuleContexts<ClythV1Parser::RelationContext>();
}

ClythV1Parser::RelationContext* ClythV1Parser::EqualityContext::relation(size_t i) {
  return getRuleContext<ClythV1Parser::RelationContext>(i);
}

std::vector<tree::TerminalNode *> ClythV1Parser::EqualityContext::EQ() {
  return getTokens(ClythV1Parser::EQ);
}

tree::TerminalNode* ClythV1Parser::EqualityContext::EQ(size_t i) {
  return getToken(ClythV1Parser::EQ, i);
}

std::vector<tree::TerminalNode *> ClythV1Parser::EqualityContext::NEQ() {
  return getTokens(ClythV1Parser::NEQ);
}

tree::TerminalNode* ClythV1Parser::EqualityContext::NEQ(size_t i) {
  return getToken(ClythV1Parser::NEQ, i);
}


size_t ClythV1Parser::EqualityContext::getRuleIndex() const {
  return ClythV1Parser::RuleEquality;
}

void ClythV1Parser::EqualityContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterEquality(this);
}

void ClythV1Parser::EqualityContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitEquality(this);
}

ClythV1Parser::EqualityContext* ClythV1Parser::equality() {
  EqualityContext *_localctx = _tracker.createInstance<EqualityContext>(_ctx, getState());
  enterRule(_localctx, 76, ClythV1Parser::RuleEquality);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(408);
    relation();
    setState(413);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == ClythV1Parser::EQ

    || _la == ClythV1Parser::NEQ) {
      setState(409);
      _la = _input->LA(1);
      if (!(_la == ClythV1Parser::EQ

      || _la == ClythV1Parser::NEQ)) {
      _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(410);
      relation();
      setState(415);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- RelationContext ------------------------------------------------------------------

ClythV1Parser::RelationContext::RelationContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<ClythV1Parser::ComparisonContext *> ClythV1Parser::RelationContext::comparison() {
  return getRuleContexts<ClythV1Parser::ComparisonContext>();
}

ClythV1Parser::ComparisonContext* ClythV1Parser::RelationContext::comparison(size_t i) {
  return getRuleContext<ClythV1Parser::ComparisonContext>(i);
}

std::vector<tree::TerminalNode *> ClythV1Parser::RelationContext::IS() {
  return getTokens(ClythV1Parser::IS);
}

tree::TerminalNode* ClythV1Parser::RelationContext::IS(size_t i) {
  return getToken(ClythV1Parser::IS, i);
}

std::vector<tree::TerminalNode *> ClythV1Parser::RelationContext::IN() {
  return getTokens(ClythV1Parser::IN);
}

tree::TerminalNode* ClythV1Parser::RelationContext::IN(size_t i) {
  return getToken(ClythV1Parser::IN, i);
}


size_t ClythV1Parser::RelationContext::getRuleIndex() const {
  return ClythV1Parser::RuleRelation;
}

void ClythV1Parser::RelationContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterRelation(this);
}

void ClythV1Parser::RelationContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitRelation(this);
}

ClythV1Parser::RelationContext* ClythV1Parser::relation() {
  RelationContext *_localctx = _tracker.createInstance<RelationContext>(_ctx, getState());
  enterRule(_localctx, 78, ClythV1Parser::RuleRelation);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(416);
    comparison();
    setState(421);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == ClythV1Parser::IS

    || _la == ClythV1Parser::IN) {
      setState(417);
      _la = _input->LA(1);
      if (!(_la == ClythV1Parser::IS

      || _la == ClythV1Parser::IN)) {
      _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(418);
      comparison();
      setState(423);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ComparisonContext ------------------------------------------------------------------

ClythV1Parser::ComparisonContext::ComparisonContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<ClythV1Parser::AdditiveContext *> ClythV1Parser::ComparisonContext::additive() {
  return getRuleContexts<ClythV1Parser::AdditiveContext>();
}

ClythV1Parser::AdditiveContext* ClythV1Parser::ComparisonContext::additive(size_t i) {
  return getRuleContext<ClythV1Parser::AdditiveContext>(i);
}

std::vector<tree::TerminalNode *> ClythV1Parser::ComparisonContext::GT() {
  return getTokens(ClythV1Parser::GT);
}

tree::TerminalNode* ClythV1Parser::ComparisonContext::GT(size_t i) {
  return getToken(ClythV1Parser::GT, i);
}

std::vector<tree::TerminalNode *> ClythV1Parser::ComparisonContext::LT() {
  return getTokens(ClythV1Parser::LT);
}

tree::TerminalNode* ClythV1Parser::ComparisonContext::LT(size_t i) {
  return getToken(ClythV1Parser::LT, i);
}

std::vector<tree::TerminalNode *> ClythV1Parser::ComparisonContext::GTE() {
  return getTokens(ClythV1Parser::GTE);
}

tree::TerminalNode* ClythV1Parser::ComparisonContext::GTE(size_t i) {
  return getToken(ClythV1Parser::GTE, i);
}

std::vector<tree::TerminalNode *> ClythV1Parser::ComparisonContext::LTE() {
  return getTokens(ClythV1Parser::LTE);
}

tree::TerminalNode* ClythV1Parser::ComparisonContext::LTE(size_t i) {
  return getToken(ClythV1Parser::LTE, i);
}


size_t ClythV1Parser::ComparisonContext::getRuleIndex() const {
  return ClythV1Parser::RuleComparison;
}

void ClythV1Parser::ComparisonContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterComparison(this);
}

void ClythV1Parser::ComparisonContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitComparison(this);
}

ClythV1Parser::ComparisonContext* ClythV1Parser::comparison() {
  ComparisonContext *_localctx = _tracker.createInstance<ComparisonContext>(_ctx, getState());
  enterRule(_localctx, 80, ClythV1Parser::RuleComparison);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(424);
    additive();
    setState(429);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 257698037760) != 0)) {
      setState(425);
      _la = _input->LA(1);
      if (!((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & 257698037760) != 0))) {
      _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(426);
      additive();
      setState(431);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- AdditiveContext ------------------------------------------------------------------

ClythV1Parser::AdditiveContext::AdditiveContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<ClythV1Parser::MultiplicativeContext *> ClythV1Parser::AdditiveContext::multiplicative() {
  return getRuleContexts<ClythV1Parser::MultiplicativeContext>();
}

ClythV1Parser::MultiplicativeContext* ClythV1Parser::AdditiveContext::multiplicative(size_t i) {
  return getRuleContext<ClythV1Parser::MultiplicativeContext>(i);
}

std::vector<tree::TerminalNode *> ClythV1Parser::AdditiveContext::PLUS() {
  return getTokens(ClythV1Parser::PLUS);
}

tree::TerminalNode* ClythV1Parser::AdditiveContext::PLUS(size_t i) {
  return getToken(ClythV1Parser::PLUS, i);
}

std::vector<tree::TerminalNode *> ClythV1Parser::AdditiveContext::MINUS() {
  return getTokens(ClythV1Parser::MINUS);
}

tree::TerminalNode* ClythV1Parser::AdditiveContext::MINUS(size_t i) {
  return getToken(ClythV1Parser::MINUS, i);
}


size_t ClythV1Parser::AdditiveContext::getRuleIndex() const {
  return ClythV1Parser::RuleAdditive;
}

void ClythV1Parser::AdditiveContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterAdditive(this);
}

void ClythV1Parser::AdditiveContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitAdditive(this);
}

ClythV1Parser::AdditiveContext* ClythV1Parser::additive() {
  AdditiveContext *_localctx = _tracker.createInstance<AdditiveContext>(_ctx, getState());
  enterRule(_localctx, 82, ClythV1Parser::RuleAdditive);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(432);
    multiplicative();
    setState(437);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 46, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(433);
        _la = _input->LA(1);
        if (!(_la == ClythV1Parser::PLUS

        || _la == ClythV1Parser::MINUS)) {
        _errHandler->recoverInline(this);
        }
        else {
          _errHandler->reportMatch(this);
          consume();
        }
        setState(434);
        multiplicative(); 
      }
      setState(439);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 46, _ctx);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- MultiplicativeContext ------------------------------------------------------------------

ClythV1Parser::MultiplicativeContext::MultiplicativeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<ClythV1Parser::UnaryContext *> ClythV1Parser::MultiplicativeContext::unary() {
  return getRuleContexts<ClythV1Parser::UnaryContext>();
}

ClythV1Parser::UnaryContext* ClythV1Parser::MultiplicativeContext::unary(size_t i) {
  return getRuleContext<ClythV1Parser::UnaryContext>(i);
}

std::vector<tree::TerminalNode *> ClythV1Parser::MultiplicativeContext::STAR() {
  return getTokens(ClythV1Parser::STAR);
}

tree::TerminalNode* ClythV1Parser::MultiplicativeContext::STAR(size_t i) {
  return getToken(ClythV1Parser::STAR, i);
}

std::vector<tree::TerminalNode *> ClythV1Parser::MultiplicativeContext::SLASH() {
  return getTokens(ClythV1Parser::SLASH);
}

tree::TerminalNode* ClythV1Parser::MultiplicativeContext::SLASH(size_t i) {
  return getToken(ClythV1Parser::SLASH, i);
}

std::vector<tree::TerminalNode *> ClythV1Parser::MultiplicativeContext::PERCENT() {
  return getTokens(ClythV1Parser::PERCENT);
}

tree::TerminalNode* ClythV1Parser::MultiplicativeContext::PERCENT(size_t i) {
  return getToken(ClythV1Parser::PERCENT, i);
}


size_t ClythV1Parser::MultiplicativeContext::getRuleIndex() const {
  return ClythV1Parser::RuleMultiplicative;
}

void ClythV1Parser::MultiplicativeContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterMultiplicative(this);
}

void ClythV1Parser::MultiplicativeContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitMultiplicative(this);
}

ClythV1Parser::MultiplicativeContext* ClythV1Parser::multiplicative() {
  MultiplicativeContext *_localctx = _tracker.createInstance<MultiplicativeContext>(_ctx, getState());
  enterRule(_localctx, 84, ClythV1Parser::RuleMultiplicative);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(440);
    unary();
    setState(445);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 30786325577728) != 0)) {
      setState(441);
      _la = _input->LA(1);
      if (!((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & 30786325577728) != 0))) {
      _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(442);
      unary();
      setState(447);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- UnaryContext ------------------------------------------------------------------

ClythV1Parser::UnaryContext::UnaryContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ClythV1Parser::UnaryContext* ClythV1Parser::UnaryContext::unary() {
  return getRuleContext<ClythV1Parser::UnaryContext>(0);
}

tree::TerminalNode* ClythV1Parser::UnaryContext::NOT() {
  return getToken(ClythV1Parser::NOT, 0);
}

tree::TerminalNode* ClythV1Parser::UnaryContext::BANG() {
  return getToken(ClythV1Parser::BANG, 0);
}

tree::TerminalNode* ClythV1Parser::UnaryContext::MINUS() {
  return getToken(ClythV1Parser::MINUS, 0);
}

tree::TerminalNode* ClythV1Parser::UnaryContext::TILDE() {
  return getToken(ClythV1Parser::TILDE, 0);
}

tree::TerminalNode* ClythV1Parser::UnaryContext::AMP() {
  return getToken(ClythV1Parser::AMP, 0);
}

ClythV1Parser::PostfixContext* ClythV1Parser::UnaryContext::postfix() {
  return getRuleContext<ClythV1Parser::PostfixContext>(0);
}


size_t ClythV1Parser::UnaryContext::getRuleIndex() const {
  return ClythV1Parser::RuleUnary;
}

void ClythV1Parser::UnaryContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterUnary(this);
}

void ClythV1Parser::UnaryContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitUnary(this);
}

ClythV1Parser::UnaryContext* ClythV1Parser::unary() {
  UnaryContext *_localctx = _tracker.createInstance<UnaryContext>(_ctx, getState());
  enterRule(_localctx, 86, ClythV1Parser::RuleUnary);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(451);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case ClythV1Parser::NOT:
      case ClythV1Parser::MINUS:
      case ClythV1Parser::BANG:
      case ClythV1Parser::TILDE:
      case ClythV1Parser::AMP: {
        enterOuterAlt(_localctx, 1);
        setState(448);
        _la = _input->LA(1);
        if (!((((_la & ~ 0x3fULL) == 0) &&
          ((1ULL << _la) & 248489896312832) != 0))) {
        _errHandler->recoverInline(this);
        }
        else {
          _errHandler->reportMatch(this);
          consume();
        }
        setState(449);
        unary();
        break;
      }

      case ClythV1Parser::T__0:
      case ClythV1Parser::T__3:
      case ClythV1Parser::T__11:
      case ClythV1Parser::BOOLEAN_LITERAL:
      case ClythV1Parser::NULL_LITERAL:
      case ClythV1Parser::NUMERIC_LITERAL:
      case ClythV1Parser::TEMPLATE_STRING_LITERAL:
      case ClythV1Parser::STRING_LITERAL:
      case ClythV1Parser::IDENTIFIER: {
        enterOuterAlt(_localctx, 2);
        setState(450);
        postfix();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- PostfixContext ------------------------------------------------------------------

ClythV1Parser::PostfixContext::PostfixContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ClythV1Parser::PrimaryContext* ClythV1Parser::PostfixContext::primary() {
  return getRuleContext<ClythV1Parser::PrimaryContext>(0);
}

std::vector<ClythV1Parser::PostfixSuffixContext *> ClythV1Parser::PostfixContext::postfixSuffix() {
  return getRuleContexts<ClythV1Parser::PostfixSuffixContext>();
}

ClythV1Parser::PostfixSuffixContext* ClythV1Parser::PostfixContext::postfixSuffix(size_t i) {
  return getRuleContext<ClythV1Parser::PostfixSuffixContext>(i);
}


size_t ClythV1Parser::PostfixContext::getRuleIndex() const {
  return ClythV1Parser::RulePostfix;
}

void ClythV1Parser::PostfixContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterPostfix(this);
}

void ClythV1Parser::PostfixContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitPostfix(this);
}

ClythV1Parser::PostfixContext* ClythV1Parser::postfix() {
  PostfixContext *_localctx = _tracker.createInstance<PostfixContext>(_ctx, getState());
  enterRule(_localctx, 88, ClythV1Parser::RulePostfix);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(453);
    primary();
    setState(457);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 49, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(454);
        postfixSuffix(); 
      }
      setState(459);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 49, _ctx);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- PostfixSuffixContext ------------------------------------------------------------------

ClythV1Parser::PostfixSuffixContext::PostfixSuffixContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ClythV1Parser::PostfixSuffixContext::DOT() {
  return getToken(ClythV1Parser::DOT, 0);
}

tree::TerminalNode* ClythV1Parser::PostfixSuffixContext::IDENTIFIER() {
  return getToken(ClythV1Parser::IDENTIFIER, 0);
}

ClythV1Parser::ExpressionContext* ClythV1Parser::PostfixSuffixContext::expression() {
  return getRuleContext<ClythV1Parser::ExpressionContext>(0);
}

ClythV1Parser::ArgumentListContext* ClythV1Parser::PostfixSuffixContext::argumentList() {
  return getRuleContext<ClythV1Parser::ArgumentListContext>(0);
}

tree::TerminalNode* ClythV1Parser::PostfixSuffixContext::INCREMENT() {
  return getToken(ClythV1Parser::INCREMENT, 0);
}

tree::TerminalNode* ClythV1Parser::PostfixSuffixContext::DECREMENT() {
  return getToken(ClythV1Parser::DECREMENT, 0);
}


size_t ClythV1Parser::PostfixSuffixContext::getRuleIndex() const {
  return ClythV1Parser::RulePostfixSuffix;
}

void ClythV1Parser::PostfixSuffixContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterPostfixSuffix(this);
}

void ClythV1Parser::PostfixSuffixContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitPostfixSuffix(this);
}

ClythV1Parser::PostfixSuffixContext* ClythV1Parser::postfixSuffix() {
  PostfixSuffixContext *_localctx = _tracker.createInstance<PostfixSuffixContext>(_ctx, getState());
  enterRule(_localctx, 90, ClythV1Parser::RulePostfixSuffix);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(473);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case ClythV1Parser::DOT: {
        enterOuterAlt(_localctx, 1);
        setState(460);
        match(ClythV1Parser::DOT);
        setState(461);
        match(ClythV1Parser::IDENTIFIER);
        break;
      }

      case ClythV1Parser::T__11: {
        enterOuterAlt(_localctx, 2);
        setState(462);
        match(ClythV1Parser::T__11);
        setState(463);
        expression();
        setState(464);
        match(ClythV1Parser::T__12);
        break;
      }

      case ClythV1Parser::T__0: {
        enterOuterAlt(_localctx, 3);
        setState(466);
        match(ClythV1Parser::T__0);
        setState(468);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if ((((_la & ~ 0x3fULL) == 0) &&
          ((1ULL << _la) & 142111878158487570) != 0)) {
          setState(467);
          argumentList();
        }
        setState(470);
        match(ClythV1Parser::T__1);
        break;
      }

      case ClythV1Parser::INCREMENT: {
        enterOuterAlt(_localctx, 4);
        setState(471);
        match(ClythV1Parser::INCREMENT);
        break;
      }

      case ClythV1Parser::DECREMENT: {
        enterOuterAlt(_localctx, 5);
        setState(472);
        match(ClythV1Parser::DECREMENT);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- PrimaryContext ------------------------------------------------------------------

ClythV1Parser::PrimaryContext::PrimaryContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ClythV1Parser::LiteralContext* ClythV1Parser::PrimaryContext::literal() {
  return getRuleContext<ClythV1Parser::LiteralContext>(0);
}

ClythV1Parser::CollectionLiteralContext* ClythV1Parser::PrimaryContext::collectionLiteral() {
  return getRuleContext<ClythV1Parser::CollectionLiteralContext>(0);
}

tree::TerminalNode* ClythV1Parser::PrimaryContext::IDENTIFIER() {
  return getToken(ClythV1Parser::IDENTIFIER, 0);
}

ClythV1Parser::ExpressionContext* ClythV1Parser::PrimaryContext::expression() {
  return getRuleContext<ClythV1Parser::ExpressionContext>(0);
}


size_t ClythV1Parser::PrimaryContext::getRuleIndex() const {
  return ClythV1Parser::RulePrimary;
}

void ClythV1Parser::PrimaryContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterPrimary(this);
}

void ClythV1Parser::PrimaryContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitPrimary(this);
}

ClythV1Parser::PrimaryContext* ClythV1Parser::primary() {
  PrimaryContext *_localctx = _tracker.createInstance<PrimaryContext>(_ctx, getState());
  enterRule(_localctx, 92, ClythV1Parser::RulePrimary);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(482);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case ClythV1Parser::BOOLEAN_LITERAL:
      case ClythV1Parser::NULL_LITERAL:
      case ClythV1Parser::NUMERIC_LITERAL:
      case ClythV1Parser::TEMPLATE_STRING_LITERAL:
      case ClythV1Parser::STRING_LITERAL: {
        enterOuterAlt(_localctx, 1);
        setState(475);
        literal();
        break;
      }

      case ClythV1Parser::T__3:
      case ClythV1Parser::T__11: {
        enterOuterAlt(_localctx, 2);
        setState(476);
        collectionLiteral();
        break;
      }

      case ClythV1Parser::IDENTIFIER: {
        enterOuterAlt(_localctx, 3);
        setState(477);
        match(ClythV1Parser::IDENTIFIER);
        break;
      }

      case ClythV1Parser::T__0: {
        enterOuterAlt(_localctx, 4);
        setState(478);
        match(ClythV1Parser::T__0);
        setState(479);
        expression();
        setState(480);
        match(ClythV1Parser::T__1);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ArgumentListContext ------------------------------------------------------------------

ClythV1Parser::ArgumentListContext::ArgumentListContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<ClythV1Parser::ExpressionContext *> ClythV1Parser::ArgumentListContext::expression() {
  return getRuleContexts<ClythV1Parser::ExpressionContext>();
}

ClythV1Parser::ExpressionContext* ClythV1Parser::ArgumentListContext::expression(size_t i) {
  return getRuleContext<ClythV1Parser::ExpressionContext>(i);
}


size_t ClythV1Parser::ArgumentListContext::getRuleIndex() const {
  return ClythV1Parser::RuleArgumentList;
}

void ClythV1Parser::ArgumentListContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterArgumentList(this);
}

void ClythV1Parser::ArgumentListContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitArgumentList(this);
}

ClythV1Parser::ArgumentListContext* ClythV1Parser::argumentList() {
  ArgumentListContext *_localctx = _tracker.createInstance<ArgumentListContext>(_ctx, getState());
  enterRule(_localctx, 94, ClythV1Parser::RuleArgumentList);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(484);
    expression();
    setState(489);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 53, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(485);
        match(ClythV1Parser::T__2);
        setState(486);
        expression(); 
      }
      setState(491);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 53, _ctx);
    }
    setState(493);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ClythV1Parser::T__2) {
      setState(492);
      match(ClythV1Parser::T__2);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- LiteralContext ------------------------------------------------------------------

ClythV1Parser::LiteralContext::LiteralContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ClythV1Parser::LiteralContext::TEMPLATE_STRING_LITERAL() {
  return getToken(ClythV1Parser::TEMPLATE_STRING_LITERAL, 0);
}

tree::TerminalNode* ClythV1Parser::LiteralContext::STRING_LITERAL() {
  return getToken(ClythV1Parser::STRING_LITERAL, 0);
}

tree::TerminalNode* ClythV1Parser::LiteralContext::NUMERIC_LITERAL() {
  return getToken(ClythV1Parser::NUMERIC_LITERAL, 0);
}

tree::TerminalNode* ClythV1Parser::LiteralContext::BOOLEAN_LITERAL() {
  return getToken(ClythV1Parser::BOOLEAN_LITERAL, 0);
}

tree::TerminalNode* ClythV1Parser::LiteralContext::NULL_LITERAL() {
  return getToken(ClythV1Parser::NULL_LITERAL, 0);
}


size_t ClythV1Parser::LiteralContext::getRuleIndex() const {
  return ClythV1Parser::RuleLiteral;
}

void ClythV1Parser::LiteralContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterLiteral(this);
}

void ClythV1Parser::LiteralContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitLiteral(this);
}

ClythV1Parser::LiteralContext* ClythV1Parser::literal() {
  LiteralContext *_localctx = _tracker.createInstance<LiteralContext>(_ctx, getState());
  enterRule(_localctx, 96, ClythV1Parser::RuleLiteral);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(495);
    _la = _input->LA(1);
    if (!((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 69805794224242688) != 0))) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- CollectionLiteralContext ------------------------------------------------------------------

ClythV1Parser::CollectionLiteralContext::CollectionLiteralContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ClythV1Parser::ListLiteralContext* ClythV1Parser::CollectionLiteralContext::listLiteral() {
  return getRuleContext<ClythV1Parser::ListLiteralContext>(0);
}

ClythV1Parser::MapLiteralContext* ClythV1Parser::CollectionLiteralContext::mapLiteral() {
  return getRuleContext<ClythV1Parser::MapLiteralContext>(0);
}

ClythV1Parser::SetLiteralContext* ClythV1Parser::CollectionLiteralContext::setLiteral() {
  return getRuleContext<ClythV1Parser::SetLiteralContext>(0);
}


size_t ClythV1Parser::CollectionLiteralContext::getRuleIndex() const {
  return ClythV1Parser::RuleCollectionLiteral;
}

void ClythV1Parser::CollectionLiteralContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterCollectionLiteral(this);
}

void ClythV1Parser::CollectionLiteralContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitCollectionLiteral(this);
}

ClythV1Parser::CollectionLiteralContext* ClythV1Parser::collectionLiteral() {
  CollectionLiteralContext *_localctx = _tracker.createInstance<CollectionLiteralContext>(_ctx, getState());
  enterRule(_localctx, 98, ClythV1Parser::RuleCollectionLiteral);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(500);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 55, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(497);
      listLiteral();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(498);
      mapLiteral();
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(499);
      setLiteral();
      break;
    }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ListLiteralContext ------------------------------------------------------------------

ClythV1Parser::ListLiteralContext::ListLiteralContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ClythV1Parser::ExpressionListContext* ClythV1Parser::ListLiteralContext::expressionList() {
  return getRuleContext<ClythV1Parser::ExpressionListContext>(0);
}


size_t ClythV1Parser::ListLiteralContext::getRuleIndex() const {
  return ClythV1Parser::RuleListLiteral;
}

void ClythV1Parser::ListLiteralContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterListLiteral(this);
}

void ClythV1Parser::ListLiteralContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitListLiteral(this);
}

ClythV1Parser::ListLiteralContext* ClythV1Parser::listLiteral() {
  ListLiteralContext *_localctx = _tracker.createInstance<ListLiteralContext>(_ctx, getState());
  enterRule(_localctx, 100, ClythV1Parser::RuleListLiteral);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(502);
    match(ClythV1Parser::T__11);
    setState(504);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 142111878158487570) != 0)) {
      setState(503);
      expressionList();
    }
    setState(506);
    match(ClythV1Parser::T__12);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- MapLiteralContext ------------------------------------------------------------------

ClythV1Parser::MapLiteralContext::MapLiteralContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ClythV1Parser::MapEntryListContext* ClythV1Parser::MapLiteralContext::mapEntryList() {
  return getRuleContext<ClythV1Parser::MapEntryListContext>(0);
}


size_t ClythV1Parser::MapLiteralContext::getRuleIndex() const {
  return ClythV1Parser::RuleMapLiteral;
}

void ClythV1Parser::MapLiteralContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterMapLiteral(this);
}

void ClythV1Parser::MapLiteralContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitMapLiteral(this);
}

ClythV1Parser::MapLiteralContext* ClythV1Parser::mapLiteral() {
  MapLiteralContext *_localctx = _tracker.createInstance<MapLiteralContext>(_ctx, getState());
  enterRule(_localctx, 102, ClythV1Parser::RuleMapLiteral);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(508);
    match(ClythV1Parser::T__3);
    setState(510);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 142111878158487570) != 0)) {
      setState(509);
      mapEntryList();
    }
    setState(512);
    match(ClythV1Parser::T__4);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- MapEntryListContext ------------------------------------------------------------------

ClythV1Parser::MapEntryListContext::MapEntryListContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<ClythV1Parser::MapEntryContext *> ClythV1Parser::MapEntryListContext::mapEntry() {
  return getRuleContexts<ClythV1Parser::MapEntryContext>();
}

ClythV1Parser::MapEntryContext* ClythV1Parser::MapEntryListContext::mapEntry(size_t i) {
  return getRuleContext<ClythV1Parser::MapEntryContext>(i);
}


size_t ClythV1Parser::MapEntryListContext::getRuleIndex() const {
  return ClythV1Parser::RuleMapEntryList;
}

void ClythV1Parser::MapEntryListContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterMapEntryList(this);
}

void ClythV1Parser::MapEntryListContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitMapEntryList(this);
}

ClythV1Parser::MapEntryListContext* ClythV1Parser::mapEntryList() {
  MapEntryListContext *_localctx = _tracker.createInstance<MapEntryListContext>(_ctx, getState());
  enterRule(_localctx, 104, ClythV1Parser::RuleMapEntryList);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(514);
    mapEntry();
    setState(519);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 58, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(515);
        match(ClythV1Parser::T__2);
        setState(516);
        mapEntry(); 
      }
      setState(521);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 58, _ctx);
    }
    setState(523);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ClythV1Parser::T__2) {
      setState(522);
      match(ClythV1Parser::T__2);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- MapEntryContext ------------------------------------------------------------------

ClythV1Parser::MapEntryContext::MapEntryContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<ClythV1Parser::ExpressionContext *> ClythV1Parser::MapEntryContext::expression() {
  return getRuleContexts<ClythV1Parser::ExpressionContext>();
}

ClythV1Parser::ExpressionContext* ClythV1Parser::MapEntryContext::expression(size_t i) {
  return getRuleContext<ClythV1Parser::ExpressionContext>(i);
}


size_t ClythV1Parser::MapEntryContext::getRuleIndex() const {
  return ClythV1Parser::RuleMapEntry;
}

void ClythV1Parser::MapEntryContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterMapEntry(this);
}

void ClythV1Parser::MapEntryContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitMapEntry(this);
}

ClythV1Parser::MapEntryContext* ClythV1Parser::mapEntry() {
  MapEntryContext *_localctx = _tracker.createInstance<MapEntryContext>(_ctx, getState());
  enterRule(_localctx, 106, ClythV1Parser::RuleMapEntry);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(525);
    expression();
    setState(526);
    match(ClythV1Parser::T__13);
    setState(527);
    expression();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- SetLiteralContext ------------------------------------------------------------------

ClythV1Parser::SetLiteralContext::SetLiteralContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ClythV1Parser::ExpressionListContext* ClythV1Parser::SetLiteralContext::expressionList() {
  return getRuleContext<ClythV1Parser::ExpressionListContext>(0);
}


size_t ClythV1Parser::SetLiteralContext::getRuleIndex() const {
  return ClythV1Parser::RuleSetLiteral;
}

void ClythV1Parser::SetLiteralContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterSetLiteral(this);
}

void ClythV1Parser::SetLiteralContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitSetLiteral(this);
}

ClythV1Parser::SetLiteralContext* ClythV1Parser::setLiteral() {
  SetLiteralContext *_localctx = _tracker.createInstance<SetLiteralContext>(_ctx, getState());
  enterRule(_localctx, 108, ClythV1Parser::RuleSetLiteral);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(529);
    match(ClythV1Parser::T__3);
    setState(530);
    expressionList();
    setState(532);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ClythV1Parser::T__2) {
      setState(531);
      match(ClythV1Parser::T__2);
    }
    setState(534);
    match(ClythV1Parser::T__4);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ExpressionListContext ------------------------------------------------------------------

ClythV1Parser::ExpressionListContext::ExpressionListContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<ClythV1Parser::ExpressionContext *> ClythV1Parser::ExpressionListContext::expression() {
  return getRuleContexts<ClythV1Parser::ExpressionContext>();
}

ClythV1Parser::ExpressionContext* ClythV1Parser::ExpressionListContext::expression(size_t i) {
  return getRuleContext<ClythV1Parser::ExpressionContext>(i);
}


size_t ClythV1Parser::ExpressionListContext::getRuleIndex() const {
  return ClythV1Parser::RuleExpressionList;
}

void ClythV1Parser::ExpressionListContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterExpressionList(this);
}

void ClythV1Parser::ExpressionListContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitExpressionList(this);
}

ClythV1Parser::ExpressionListContext* ClythV1Parser::expressionList() {
  ExpressionListContext *_localctx = _tracker.createInstance<ExpressionListContext>(_ctx, getState());
  enterRule(_localctx, 110, ClythV1Parser::RuleExpressionList);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(536);
    expression();
    setState(541);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 61, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(537);
        match(ClythV1Parser::T__2);
        setState(538);
        expression(); 
      }
      setState(543);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 61, _ctx);
    }
    setState(545);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 62, _ctx)) {
    case 1: {
      setState(544);
      match(ClythV1Parser::T__2);
      break;
    }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- TypeContext ------------------------------------------------------------------

ClythV1Parser::TypeContext::TypeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ClythV1Parser::MapTypeContext* ClythV1Parser::TypeContext::mapType() {
  return getRuleContext<ClythV1Parser::MapTypeContext>(0);
}

ClythV1Parser::CollectionTypeContext* ClythV1Parser::TypeContext::collectionType() {
  return getRuleContext<ClythV1Parser::CollectionTypeContext>(0);
}


size_t ClythV1Parser::TypeContext::getRuleIndex() const {
  return ClythV1Parser::RuleType;
}

void ClythV1Parser::TypeContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterType(this);
}

void ClythV1Parser::TypeContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitType(this);
}

ClythV1Parser::TypeContext* ClythV1Parser::type() {
  TypeContext *_localctx = _tracker.createInstance<TypeContext>(_ctx, getState());
  enterRule(_localctx, 112, ClythV1Parser::RuleType);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(549);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 63, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(547);
      mapType();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(548);
      collectionType();
      break;
    }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- MapTypeContext ------------------------------------------------------------------

ClythV1Parser::MapTypeContext::MapTypeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<ClythV1Parser::CollectionTypeContext *> ClythV1Parser::MapTypeContext::collectionType() {
  return getRuleContexts<ClythV1Parser::CollectionTypeContext>();
}

ClythV1Parser::CollectionTypeContext* ClythV1Parser::MapTypeContext::collectionType(size_t i) {
  return getRuleContext<ClythV1Parser::CollectionTypeContext>(i);
}


size_t ClythV1Parser::MapTypeContext::getRuleIndex() const {
  return ClythV1Parser::RuleMapType;
}

void ClythV1Parser::MapTypeContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterMapType(this);
}

void ClythV1Parser::MapTypeContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitMapType(this);
}

ClythV1Parser::MapTypeContext* ClythV1Parser::mapType() {
  MapTypeContext *_localctx = _tracker.createInstance<MapTypeContext>(_ctx, getState());
  enterRule(_localctx, 114, ClythV1Parser::RuleMapType);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(551);
    collectionType();
    setState(552);
    match(ClythV1Parser::T__13);
    setState(553);
    collectionType();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- CollectionTypeContext ------------------------------------------------------------------

ClythV1Parser::CollectionTypeContext::CollectionTypeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ClythV1Parser::BaseTypeContext* ClythV1Parser::CollectionTypeContext::baseType() {
  return getRuleContext<ClythV1Parser::BaseTypeContext>(0);
}

std::vector<ClythV1Parser::TypeSuffixContext *> ClythV1Parser::CollectionTypeContext::typeSuffix() {
  return getRuleContexts<ClythV1Parser::TypeSuffixContext>();
}

ClythV1Parser::TypeSuffixContext* ClythV1Parser::CollectionTypeContext::typeSuffix(size_t i) {
  return getRuleContext<ClythV1Parser::TypeSuffixContext>(i);
}


size_t ClythV1Parser::CollectionTypeContext::getRuleIndex() const {
  return ClythV1Parser::RuleCollectionType;
}

void ClythV1Parser::CollectionTypeContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterCollectionType(this);
}

void ClythV1Parser::CollectionTypeContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitCollectionType(this);
}

ClythV1Parser::CollectionTypeContext* ClythV1Parser::collectionType() {
  CollectionTypeContext *_localctx = _tracker.createInstance<CollectionTypeContext>(_ctx, getState());
  enterRule(_localctx, 116, ClythV1Parser::RuleCollectionType);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(555);
    baseType();
    setState(559);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == ClythV1Parser::T__0

    || _la == ClythV1Parser::T__11) {
      setState(556);
      typeSuffix();
      setState(561);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- BaseTypeContext ------------------------------------------------------------------

ClythV1Parser::BaseTypeContext::BaseTypeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ClythV1Parser::BaseTypeContext::BUILTIN_TYPE() {
  return getToken(ClythV1Parser::BUILTIN_TYPE, 0);
}

tree::TerminalNode* ClythV1Parser::BaseTypeContext::IDENTIFIER() {
  return getToken(ClythV1Parser::IDENTIFIER, 0);
}


size_t ClythV1Parser::BaseTypeContext::getRuleIndex() const {
  return ClythV1Parser::RuleBaseType;
}

void ClythV1Parser::BaseTypeContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterBaseType(this);
}

void ClythV1Parser::BaseTypeContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitBaseType(this);
}

ClythV1Parser::BaseTypeContext* ClythV1Parser::baseType() {
  BaseTypeContext *_localctx = _tracker.createInstance<BaseTypeContext>(_ctx, getState());
  enterRule(_localctx, 118, ClythV1Parser::RuleBaseType);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(562);
    _la = _input->LA(1);
    if (!(_la == ClythV1Parser::BUILTIN_TYPE

    || _la == ClythV1Parser::IDENTIFIER)) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- TypeSuffixContext ------------------------------------------------------------------

ClythV1Parser::TypeSuffixContext::TypeSuffixContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ClythV1Parser::TypeSuffixContext::NUMERIC_LITERAL() {
  return getToken(ClythV1Parser::NUMERIC_LITERAL, 0);
}


size_t ClythV1Parser::TypeSuffixContext::getRuleIndex() const {
  return ClythV1Parser::RuleTypeSuffix;
}

void ClythV1Parser::TypeSuffixContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterTypeSuffix(this);
}

void ClythV1Parser::TypeSuffixContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitTypeSuffix(this);
}

ClythV1Parser::TypeSuffixContext* ClythV1Parser::typeSuffix() {
  TypeSuffixContext *_localctx = _tracker.createInstance<TypeSuffixContext>(_ctx, getState());
  enterRule(_localctx, 120, ClythV1Parser::RuleTypeSuffix);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(571);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 65, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(564);
      match(ClythV1Parser::T__11);
      setState(565);
      match(ClythV1Parser::T__12);
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(566);
      match(ClythV1Parser::T__11);
      setState(567);
      match(ClythV1Parser::NUMERIC_LITERAL);
      setState(568);
      match(ClythV1Parser::T__12);
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(569);
      match(ClythV1Parser::T__0);
      setState(570);
      match(ClythV1Parser::T__1);
      break;
    }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- CommaOrSemiContext ------------------------------------------------------------------

ClythV1Parser::CommaOrSemiContext::CommaOrSemiContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ClythV1Parser::CommaOrSemiContext::SEMI() {
  return getToken(ClythV1Parser::SEMI, 0);
}


size_t ClythV1Parser::CommaOrSemiContext::getRuleIndex() const {
  return ClythV1Parser::RuleCommaOrSemi;
}

void ClythV1Parser::CommaOrSemiContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterCommaOrSemi(this);
}

void ClythV1Parser::CommaOrSemiContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitCommaOrSemi(this);
}

ClythV1Parser::CommaOrSemiContext* ClythV1Parser::commaOrSemi() {
  CommaOrSemiContext *_localctx = _tracker.createInstance<CommaOrSemiContext>(_ctx, getState());
  enterRule(_localctx, 122, ClythV1Parser::RuleCommaOrSemi);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(573);
    _la = _input->LA(1);
    if (!(_la == ClythV1Parser::T__2

    || _la == ClythV1Parser::SEMI)) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

void ClythV1Parser::initialize() {
#if ANTLR4_USE_THREAD_LOCAL_CACHE
  clythv1ParserInitialize();
#else
  ::antlr4::internal::call_once(clythv1ParserOnceFlag, clythv1ParserInitialize);
#endif
}
