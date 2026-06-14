
// Generated from ClythV1.g4 by ANTLR 4.13.2


#include "ClythV1Listener.h"
#include "ClythV1Visitor.h"

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
      "methodBlock", "methodDecl", "methodSimpleName", "functionDecl", "paramList", 
      "param", "block", "statement", "varDecl", "assignmentStmt", "assignable", 
      "assignmentOp", "exprStmt", "returnStmt", "ifStmt", "elseClause", 
      "whileStmt", "forStmt", "forEachHeader", "forEachBinding", "forEachVar", 
      "forClassicHeader", "forInit", "varDeclNoSemi", "assignmentNoSemi", 
      "breakStmt", "continueStmt", "meccBlock", "parenExpr", "expression", 
      "logicalOr", "logicalAnd", "equality", "relation", "comparison", "additive", 
      "multiplicative", "unary", "postfix", "postfixSuffix", "primary", 
      "allocationExpression", "argumentList", "literal", "collectionLiteral", 
      "listLiteral", "mapLiteral", "mapEntryList", "mapEntry", "setLiteral", 
      "expressionList", "type", "mapType", "collectionType", "baseType", 
      "typeSuffix", "commaOrSemi"
    },
    std::vector<std::string>{
      "", "'include'", "'extern'", "'C'", "'struct'", "'mecc'", "'malloc'", 
      "'iso_malloc'", "'constructor'", "'destructor'", "'if'", "'else'", 
      "'for'", "'while'", "'return'", "'break'", "'continue'", "'is'", "'in'", 
      "'not'", "", "'null'", "'('", "')'", "'{'", "'}'", "'['", "']'", "','", 
      "':'", "';'", "'.'", "'||'", "'&&'", "'=='", "'!='", "'>='", "'<='", 
      "'>'", "'<'", "'++'", "'--'", "'+='", "'-='", "'*='", "'/='", "'%='", 
      "'='", "'+'", "'-'", "'*'", "'/'", "'%'", "'!'", "'~'", "'&'", "'...'"
    },
    std::vector<std::string>{
      "", "INCLUDE", "EXTERN", "C_ABI", "STRUCT", "MECC", "MALLOC", "ISO_MALLOC", 
      "CONSTRUCTOR", "DESTRUCTOR", "IF", "ELSE", "FOR", "WHILE", "RETURN", 
      "BREAK", "CONTINUE", "IS", "IN", "NOT", "BOOLEAN_LITERAL", "NULL_LITERAL", 
      "LPAREN", "RPAREN", "LBRACE", "RBRACE", "LBRACKET", "RBRACKET", "COMMA", 
      "COLON", "SEMI", "DOT", "OR", "AND", "EQ", "NEQ", "GTE", "LTE", "GT", 
      "LT", "INCREMENT", "DECREMENT", "PLUS_ASSIGN", "MINUS_ASSIGN", "STAR_ASSIGN", 
      "SLASH_ASSIGN", "PERCENT_ASSIGN", "ASSIGN", "PLUS", "MINUS", "STAR", 
      "SLASH", "PERCENT", "BANG", "TILDE", "AMP", "VARARGS", "BUILTIN_TYPE", 
      "NUMERIC_LITERAL", "TEMPLATE_STRING_LITERAL", "STRING_LITERAL", "IDENTIFIER", 
      "SINGLE_LINE_COMMENT", "MULTI_LINE_COMMENT", "WS"
    }
  );
  static const int32_t serializedATNSegment[] = {
  	4,1,64,641,2,0,7,0,2,1,7,1,2,2,7,2,2,3,7,3,2,4,7,4,2,5,7,5,2,6,7,6,2,
  	7,7,7,2,8,7,8,2,9,7,9,2,10,7,10,2,11,7,11,2,12,7,12,2,13,7,13,2,14,7,
  	14,2,15,7,15,2,16,7,16,2,17,7,17,2,18,7,18,2,19,7,19,2,20,7,20,2,21,7,
  	21,2,22,7,22,2,23,7,23,2,24,7,24,2,25,7,25,2,26,7,26,2,27,7,27,2,28,7,
  	28,2,29,7,29,2,30,7,30,2,31,7,31,2,32,7,32,2,33,7,33,2,34,7,34,2,35,7,
  	35,2,36,7,36,2,37,7,37,2,38,7,38,2,39,7,39,2,40,7,40,2,41,7,41,2,42,7,
  	42,2,43,7,43,2,44,7,44,2,45,7,45,2,46,7,46,2,47,7,47,2,48,7,48,2,49,7,
  	49,2,50,7,50,2,51,7,51,2,52,7,52,2,53,7,53,2,54,7,54,2,55,7,55,2,56,7,
  	56,2,57,7,57,2,58,7,58,2,59,7,59,2,60,7,60,2,61,7,61,2,62,7,62,2,63,7,
  	63,2,64,7,64,2,65,7,65,2,66,7,66,1,0,5,0,136,8,0,10,0,12,0,139,9,0,1,
  	0,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,3,1,150,8,1,1,2,1,2,1,2,3,2,155,8,2,
  	1,3,1,3,1,4,1,4,3,4,161,8,4,1,4,1,4,1,4,1,4,3,4,167,8,4,1,4,1,4,3,4,171,
  	8,4,1,5,1,5,1,6,1,6,1,6,5,6,178,8,6,10,6,12,6,181,9,6,1,6,3,6,184,8,6,
  	1,6,3,6,187,8,6,1,6,3,6,190,8,6,1,7,1,7,3,7,194,8,7,1,8,1,8,1,8,1,8,5,
  	8,200,8,8,10,8,12,8,203,9,8,1,8,1,8,3,8,207,8,8,1,9,1,9,1,9,3,9,212,8,
  	9,1,10,1,10,1,10,5,10,217,8,10,10,10,12,10,220,9,10,1,10,1,10,1,11,3,
  	11,225,8,11,1,11,1,11,1,11,1,11,3,11,231,8,11,1,11,1,11,1,11,1,11,3,11,
  	237,8,11,1,11,1,11,1,11,1,11,1,11,1,11,3,11,245,8,11,1,11,1,11,1,11,3,
  	11,250,8,11,1,12,1,12,1,13,3,13,255,8,13,1,13,1,13,1,13,1,13,3,13,261,
  	8,13,1,13,1,13,1,13,1,14,1,14,1,14,5,14,269,8,14,10,14,12,14,272,9,14,
  	1,14,3,14,275,8,14,1,15,1,15,1,15,1,16,1,16,5,16,282,8,16,10,16,12,16,
  	285,9,16,1,16,1,16,1,17,1,17,1,17,1,17,1,17,1,17,1,17,1,17,1,17,1,17,
  	1,17,3,17,300,8,17,1,18,1,18,1,18,1,18,3,18,306,8,18,1,18,3,18,309,8,
  	18,1,19,1,19,1,19,1,19,3,19,315,8,19,1,20,1,20,5,20,319,8,20,10,20,12,
  	20,322,9,20,1,21,1,21,1,22,1,22,3,22,328,8,22,1,23,1,23,3,23,332,8,23,
  	1,23,3,23,335,8,23,1,24,1,24,1,24,1,24,3,24,341,8,24,1,24,1,24,1,24,1,
  	24,3,24,347,8,24,3,24,349,8,24,1,25,1,25,1,25,1,25,3,25,355,8,25,1,26,
  	1,26,1,26,1,26,1,26,1,26,1,26,1,26,3,26,365,8,26,1,27,1,27,1,27,1,27,
  	1,27,1,27,1,27,1,27,1,27,1,27,1,27,1,27,1,27,1,27,1,27,1,27,1,27,1,27,
  	1,27,1,27,1,27,1,27,3,27,389,8,27,1,28,1,28,1,28,1,28,1,29,1,29,1,29,
  	5,29,398,8,29,10,29,12,29,401,9,29,1,30,1,30,1,30,1,30,3,30,407,8,30,
  	1,31,3,31,410,8,31,1,31,1,31,3,31,414,8,31,1,31,1,31,3,31,418,8,31,1,
  	32,1,32,1,32,3,32,423,8,32,1,33,1,33,1,33,1,33,3,33,429,8,33,1,34,1,34,
  	1,34,1,34,1,35,1,35,3,35,437,8,35,1,36,1,36,3,36,441,8,36,1,37,1,37,1,
  	37,1,38,1,38,1,38,1,38,1,39,1,39,1,40,1,40,1,40,5,40,455,8,40,10,40,12,
  	40,458,9,40,1,41,1,41,1,41,5,41,463,8,41,10,41,12,41,466,9,41,1,42,1,
  	42,1,42,5,42,471,8,42,10,42,12,42,474,9,42,1,43,1,43,1,43,5,43,479,8,
  	43,10,43,12,43,482,9,43,1,44,1,44,1,44,5,44,487,8,44,10,44,12,44,490,
  	9,44,1,45,1,45,1,45,5,45,495,8,45,10,45,12,45,498,9,45,1,46,1,46,1,46,
  	5,46,503,8,46,10,46,12,46,506,9,46,1,47,1,47,1,47,3,47,511,8,47,1,48,
  	1,48,5,48,515,8,48,10,48,12,48,518,9,48,1,49,1,49,1,49,1,49,1,49,1,49,
  	1,49,1,49,3,49,528,8,49,1,49,1,49,1,49,3,49,533,8,49,1,50,1,50,1,50,1,
  	50,1,50,1,50,1,50,1,50,3,50,543,8,50,1,51,1,51,1,51,1,51,1,51,1,52,1,
  	52,1,52,5,52,553,8,52,10,52,12,52,556,9,52,1,52,3,52,559,8,52,1,53,1,
  	53,1,54,1,54,1,54,3,54,566,8,54,1,55,1,55,3,55,570,8,55,1,55,1,55,1,56,
  	1,56,3,56,576,8,56,1,56,1,56,1,57,1,57,1,57,5,57,583,8,57,10,57,12,57,
  	586,9,57,1,57,3,57,589,8,57,1,58,1,58,1,58,1,58,1,59,1,59,1,59,3,59,598,
  	8,59,1,59,1,59,1,60,1,60,1,60,5,60,605,8,60,10,60,12,60,608,9,60,1,60,
  	3,60,611,8,60,1,61,1,61,3,61,615,8,61,1,62,1,62,1,62,1,62,1,63,1,63,5,
  	63,623,8,63,10,63,12,63,626,9,63,1,64,1,64,1,65,1,65,1,65,1,65,1,65,1,
  	65,1,65,3,65,637,8,65,1,66,1,66,1,66,0,0,67,0,2,4,6,8,10,12,14,16,18,
  	20,22,24,26,28,30,32,34,36,38,40,42,44,46,48,50,52,54,56,58,60,62,64,
  	66,68,70,72,74,76,78,80,82,84,86,88,90,92,94,96,98,100,102,104,106,108,
  	110,112,114,116,118,120,122,124,126,128,130,132,0,14,1,0,59,61,2,0,3,
  	3,61,61,2,0,8,9,61,61,1,0,42,47,1,0,34,35,1,0,17,18,1,0,36,39,1,0,48,
  	49,1,0,50,52,3,0,19,19,49,49,53,55,1,0,6,7,2,0,20,21,58,60,2,0,57,57,
  	61,61,2,0,28,28,30,30,672,0,137,1,0,0,0,2,149,1,0,0,0,4,151,1,0,0,0,6,
  	156,1,0,0,0,8,158,1,0,0,0,10,172,1,0,0,0,12,189,1,0,0,0,14,191,1,0,0,
  	0,16,195,1,0,0,0,18,208,1,0,0,0,20,213,1,0,0,0,22,249,1,0,0,0,24,251,
  	1,0,0,0,26,254,1,0,0,0,28,265,1,0,0,0,30,276,1,0,0,0,32,279,1,0,0,0,34,
  	299,1,0,0,0,36,301,1,0,0,0,38,310,1,0,0,0,40,316,1,0,0,0,42,323,1,0,0,
  	0,44,325,1,0,0,0,46,329,1,0,0,0,48,348,1,0,0,0,50,354,1,0,0,0,52,364,
  	1,0,0,0,54,388,1,0,0,0,56,390,1,0,0,0,58,394,1,0,0,0,60,406,1,0,0,0,62,
  	409,1,0,0,0,64,422,1,0,0,0,66,424,1,0,0,0,68,430,1,0,0,0,70,434,1,0,0,
  	0,72,438,1,0,0,0,74,442,1,0,0,0,76,445,1,0,0,0,78,449,1,0,0,0,80,451,
  	1,0,0,0,82,459,1,0,0,0,84,467,1,0,0,0,86,475,1,0,0,0,88,483,1,0,0,0,90,
  	491,1,0,0,0,92,499,1,0,0,0,94,510,1,0,0,0,96,512,1,0,0,0,98,532,1,0,0,
  	0,100,542,1,0,0,0,102,544,1,0,0,0,104,549,1,0,0,0,106,560,1,0,0,0,108,
  	565,1,0,0,0,110,567,1,0,0,0,112,573,1,0,0,0,114,579,1,0,0,0,116,590,1,
  	0,0,0,118,594,1,0,0,0,120,601,1,0,0,0,122,614,1,0,0,0,124,616,1,0,0,0,
  	126,620,1,0,0,0,128,627,1,0,0,0,130,636,1,0,0,0,132,638,1,0,0,0,134,136,
  	3,2,1,0,135,134,1,0,0,0,136,139,1,0,0,0,137,135,1,0,0,0,137,138,1,0,0,
  	0,138,140,1,0,0,0,139,137,1,0,0,0,140,141,5,0,0,1,141,1,1,0,0,0,142,150,
  	3,4,2,0,143,150,3,8,4,0,144,150,3,16,8,0,145,150,3,20,10,0,146,150,3,
  	26,13,0,147,150,3,22,11,0,148,150,3,34,17,0,149,142,1,0,0,0,149,143,1,
  	0,0,0,149,144,1,0,0,0,149,145,1,0,0,0,149,146,1,0,0,0,149,147,1,0,0,0,
  	149,148,1,0,0,0,150,3,1,0,0,0,151,152,5,1,0,0,152,154,3,6,3,0,153,155,
  	5,30,0,0,154,153,1,0,0,0,154,155,1,0,0,0,155,5,1,0,0,0,156,157,7,0,0,
  	0,157,7,1,0,0,0,158,160,5,2,0,0,159,161,3,10,5,0,160,159,1,0,0,0,160,
  	161,1,0,0,0,161,162,1,0,0,0,162,163,3,122,61,0,163,164,5,61,0,0,164,166,
  	5,22,0,0,165,167,3,12,6,0,166,165,1,0,0,0,166,167,1,0,0,0,167,168,1,0,
  	0,0,168,170,5,23,0,0,169,171,5,30,0,0,170,169,1,0,0,0,170,171,1,0,0,0,
  	171,9,1,0,0,0,172,173,7,1,0,0,173,11,1,0,0,0,174,179,3,14,7,0,175,176,
  	5,28,0,0,176,178,3,14,7,0,177,175,1,0,0,0,178,181,1,0,0,0,179,177,1,0,
  	0,0,179,180,1,0,0,0,180,183,1,0,0,0,181,179,1,0,0,0,182,184,5,28,0,0,
  	183,182,1,0,0,0,183,184,1,0,0,0,184,186,1,0,0,0,185,187,5,56,0,0,186,
  	185,1,0,0,0,186,187,1,0,0,0,187,190,1,0,0,0,188,190,5,56,0,0,189,174,
  	1,0,0,0,189,188,1,0,0,0,190,13,1,0,0,0,191,193,3,122,61,0,192,194,5,61,
  	0,0,193,192,1,0,0,0,193,194,1,0,0,0,194,15,1,0,0,0,195,196,5,4,0,0,196,
  	197,5,61,0,0,197,201,5,24,0,0,198,200,3,18,9,0,199,198,1,0,0,0,200,203,
  	1,0,0,0,201,199,1,0,0,0,201,202,1,0,0,0,202,204,1,0,0,0,203,201,1,0,0,
  	0,204,206,5,25,0,0,205,207,5,30,0,0,206,205,1,0,0,0,206,207,1,0,0,0,207,
  	17,1,0,0,0,208,209,3,122,61,0,209,211,5,61,0,0,210,212,3,132,66,0,211,
  	210,1,0,0,0,211,212,1,0,0,0,212,19,1,0,0,0,213,214,5,61,0,0,214,218,5,
  	24,0,0,215,217,3,22,11,0,216,215,1,0,0,0,217,220,1,0,0,0,218,216,1,0,
  	0,0,218,219,1,0,0,0,219,221,1,0,0,0,220,218,1,0,0,0,221,222,5,25,0,0,
  	222,21,1,0,0,0,223,225,5,5,0,0,224,223,1,0,0,0,224,225,1,0,0,0,225,226,
  	1,0,0,0,226,227,3,122,61,0,227,228,3,24,12,0,228,230,5,22,0,0,229,231,
  	3,28,14,0,230,229,1,0,0,0,230,231,1,0,0,0,231,232,1,0,0,0,232,233,5,23,
  	0,0,233,234,3,32,16,0,234,250,1,0,0,0,235,237,5,5,0,0,236,235,1,0,0,0,
  	236,237,1,0,0,0,237,238,1,0,0,0,238,239,3,122,61,0,239,240,5,61,0,0,240,
  	241,5,31,0,0,241,242,3,24,12,0,242,244,5,22,0,0,243,245,3,28,14,0,244,
  	243,1,0,0,0,244,245,1,0,0,0,245,246,1,0,0,0,246,247,5,23,0,0,247,248,
  	3,32,16,0,248,250,1,0,0,0,249,224,1,0,0,0,249,236,1,0,0,0,250,23,1,0,
  	0,0,251,252,7,2,0,0,252,25,1,0,0,0,253,255,5,5,0,0,254,253,1,0,0,0,254,
  	255,1,0,0,0,255,256,1,0,0,0,256,257,3,122,61,0,257,258,5,61,0,0,258,260,
  	5,22,0,0,259,261,3,28,14,0,260,259,1,0,0,0,260,261,1,0,0,0,261,262,1,
  	0,0,0,262,263,5,23,0,0,263,264,3,32,16,0,264,27,1,0,0,0,265,270,3,30,
  	15,0,266,267,5,28,0,0,267,269,3,30,15,0,268,266,1,0,0,0,269,272,1,0,0,
  	0,270,268,1,0,0,0,270,271,1,0,0,0,271,274,1,0,0,0,272,270,1,0,0,0,273,
  	275,5,28,0,0,274,273,1,0,0,0,274,275,1,0,0,0,275,29,1,0,0,0,276,277,3,
  	122,61,0,277,278,5,61,0,0,278,31,1,0,0,0,279,283,5,24,0,0,280,282,3,34,
  	17,0,281,280,1,0,0,0,282,285,1,0,0,0,283,281,1,0,0,0,283,284,1,0,0,0,
  	284,286,1,0,0,0,285,283,1,0,0,0,286,287,5,25,0,0,287,33,1,0,0,0,288,300,
  	3,36,18,0,289,300,3,38,19,0,290,300,3,44,22,0,291,300,3,46,23,0,292,300,
  	3,48,24,0,293,300,3,52,26,0,294,300,3,54,27,0,295,300,3,70,35,0,296,300,
  	3,72,36,0,297,300,3,74,37,0,298,300,3,32,16,0,299,288,1,0,0,0,299,289,
  	1,0,0,0,299,290,1,0,0,0,299,291,1,0,0,0,299,292,1,0,0,0,299,293,1,0,0,
  	0,299,294,1,0,0,0,299,295,1,0,0,0,299,296,1,0,0,0,299,297,1,0,0,0,299,
  	298,1,0,0,0,300,35,1,0,0,0,301,302,3,122,61,0,302,305,5,61,0,0,303,304,
  	5,47,0,0,304,306,3,78,39,0,305,303,1,0,0,0,305,306,1,0,0,0,306,308,1,
  	0,0,0,307,309,5,30,0,0,308,307,1,0,0,0,308,309,1,0,0,0,309,37,1,0,0,0,
  	310,311,3,40,20,0,311,312,3,42,21,0,312,314,3,78,39,0,313,315,5,30,0,
  	0,314,313,1,0,0,0,314,315,1,0,0,0,315,39,1,0,0,0,316,320,5,61,0,0,317,
  	319,3,98,49,0,318,317,1,0,0,0,319,322,1,0,0,0,320,318,1,0,0,0,320,321,
  	1,0,0,0,321,41,1,0,0,0,322,320,1,0,0,0,323,324,7,3,0,0,324,43,1,0,0,0,
  	325,327,3,78,39,0,326,328,5,30,0,0,327,326,1,0,0,0,327,328,1,0,0,0,328,
  	45,1,0,0,0,329,331,5,14,0,0,330,332,3,78,39,0,331,330,1,0,0,0,331,332,
  	1,0,0,0,332,334,1,0,0,0,333,335,5,30,0,0,334,333,1,0,0,0,334,335,1,0,
  	0,0,335,47,1,0,0,0,336,337,5,10,0,0,337,338,3,76,38,0,338,340,3,32,16,
  	0,339,341,3,50,25,0,340,339,1,0,0,0,340,341,1,0,0,0,341,349,1,0,0,0,342,
  	343,5,10,0,0,343,344,3,78,39,0,344,346,3,32,16,0,345,347,3,50,25,0,346,
  	345,1,0,0,0,346,347,1,0,0,0,347,349,1,0,0,0,348,336,1,0,0,0,348,342,1,
  	0,0,0,349,49,1,0,0,0,350,351,5,11,0,0,351,355,3,48,24,0,352,353,5,11,
  	0,0,353,355,3,32,16,0,354,350,1,0,0,0,354,352,1,0,0,0,355,51,1,0,0,0,
  	356,357,5,13,0,0,357,358,3,76,38,0,358,359,3,32,16,0,359,365,1,0,0,0,
  	360,361,5,13,0,0,361,362,3,78,39,0,362,363,3,32,16,0,363,365,1,0,0,0,
  	364,356,1,0,0,0,364,360,1,0,0,0,365,53,1,0,0,0,366,367,5,12,0,0,367,389,
  	3,32,16,0,368,369,5,12,0,0,369,370,3,56,28,0,370,371,3,32,16,0,371,389,
  	1,0,0,0,372,373,5,12,0,0,373,374,5,22,0,0,374,375,3,56,28,0,375,376,5,
  	23,0,0,376,377,3,32,16,0,377,389,1,0,0,0,378,379,5,12,0,0,379,380,3,62,
  	31,0,380,381,3,32,16,0,381,389,1,0,0,0,382,383,5,12,0,0,383,384,5,22,
  	0,0,384,385,3,62,31,0,385,386,5,23,0,0,386,387,3,32,16,0,387,389,1,0,
  	0,0,388,366,1,0,0,0,388,368,1,0,0,0,388,372,1,0,0,0,388,378,1,0,0,0,388,
  	382,1,0,0,0,389,55,1,0,0,0,390,391,3,58,29,0,391,392,5,18,0,0,392,393,
  	3,78,39,0,393,57,1,0,0,0,394,399,3,60,30,0,395,396,5,28,0,0,396,398,3,
  	60,30,0,397,395,1,0,0,0,398,401,1,0,0,0,399,397,1,0,0,0,399,400,1,0,0,
  	0,400,59,1,0,0,0,401,399,1,0,0,0,402,403,3,122,61,0,403,404,5,61,0,0,
  	404,407,1,0,0,0,405,407,5,61,0,0,406,402,1,0,0,0,406,405,1,0,0,0,407,
  	61,1,0,0,0,408,410,3,64,32,0,409,408,1,0,0,0,409,410,1,0,0,0,410,411,
  	1,0,0,0,411,413,5,30,0,0,412,414,3,78,39,0,413,412,1,0,0,0,413,414,1,
  	0,0,0,414,415,1,0,0,0,415,417,5,30,0,0,416,418,3,78,39,0,417,416,1,0,
  	0,0,417,418,1,0,0,0,418,63,1,0,0,0,419,423,3,66,33,0,420,423,3,68,34,
  	0,421,423,3,78,39,0,422,419,1,0,0,0,422,420,1,0,0,0,422,421,1,0,0,0,423,
  	65,1,0,0,0,424,425,3,122,61,0,425,428,5,61,0,0,426,427,5,47,0,0,427,429,
  	3,78,39,0,428,426,1,0,0,0,428,429,1,0,0,0,429,67,1,0,0,0,430,431,3,40,
  	20,0,431,432,3,42,21,0,432,433,3,78,39,0,433,69,1,0,0,0,434,436,5,15,
  	0,0,435,437,5,30,0,0,436,435,1,0,0,0,436,437,1,0,0,0,437,71,1,0,0,0,438,
  	440,5,16,0,0,439,441,5,30,0,0,440,439,1,0,0,0,440,441,1,0,0,0,441,73,
  	1,0,0,0,442,443,5,5,0,0,443,444,3,32,16,0,444,75,1,0,0,0,445,446,5,22,
  	0,0,446,447,3,78,39,0,447,448,5,23,0,0,448,77,1,0,0,0,449,450,3,80,40,
  	0,450,79,1,0,0,0,451,456,3,82,41,0,452,453,5,32,0,0,453,455,3,82,41,0,
  	454,452,1,0,0,0,455,458,1,0,0,0,456,454,1,0,0,0,456,457,1,0,0,0,457,81,
  	1,0,0,0,458,456,1,0,0,0,459,464,3,84,42,0,460,461,5,33,0,0,461,463,3,
  	84,42,0,462,460,1,0,0,0,463,466,1,0,0,0,464,462,1,0,0,0,464,465,1,0,0,
  	0,465,83,1,0,0,0,466,464,1,0,0,0,467,472,3,86,43,0,468,469,7,4,0,0,469,
  	471,3,86,43,0,470,468,1,0,0,0,471,474,1,0,0,0,472,470,1,0,0,0,472,473,
  	1,0,0,0,473,85,1,0,0,0,474,472,1,0,0,0,475,480,3,88,44,0,476,477,7,5,
  	0,0,477,479,3,88,44,0,478,476,1,0,0,0,479,482,1,0,0,0,480,478,1,0,0,0,
  	480,481,1,0,0,0,481,87,1,0,0,0,482,480,1,0,0,0,483,488,3,90,45,0,484,
  	485,7,6,0,0,485,487,3,90,45,0,486,484,1,0,0,0,487,490,1,0,0,0,488,486,
  	1,0,0,0,488,489,1,0,0,0,489,89,1,0,0,0,490,488,1,0,0,0,491,496,3,92,46,
  	0,492,493,7,7,0,0,493,495,3,92,46,0,494,492,1,0,0,0,495,498,1,0,0,0,496,
  	494,1,0,0,0,496,497,1,0,0,0,497,91,1,0,0,0,498,496,1,0,0,0,499,504,3,
  	94,47,0,500,501,7,8,0,0,501,503,3,94,47,0,502,500,1,0,0,0,503,506,1,0,
  	0,0,504,502,1,0,0,0,504,505,1,0,0,0,505,93,1,0,0,0,506,504,1,0,0,0,507,
  	508,7,9,0,0,508,511,3,94,47,0,509,511,3,96,48,0,510,507,1,0,0,0,510,509,
  	1,0,0,0,511,95,1,0,0,0,512,516,3,100,50,0,513,515,3,98,49,0,514,513,1,
  	0,0,0,515,518,1,0,0,0,516,514,1,0,0,0,516,517,1,0,0,0,517,97,1,0,0,0,
  	518,516,1,0,0,0,519,520,5,31,0,0,520,533,5,61,0,0,521,522,5,26,0,0,522,
  	523,3,78,39,0,523,524,5,27,0,0,524,533,1,0,0,0,525,527,5,22,0,0,526,528,
  	3,104,52,0,527,526,1,0,0,0,527,528,1,0,0,0,528,529,1,0,0,0,529,533,5,
  	23,0,0,530,533,5,40,0,0,531,533,5,41,0,0,532,519,1,0,0,0,532,521,1,0,
  	0,0,532,525,1,0,0,0,532,530,1,0,0,0,532,531,1,0,0,0,533,99,1,0,0,0,534,
  	543,3,106,53,0,535,543,3,108,54,0,536,543,3,102,51,0,537,543,5,61,0,0,
  	538,539,5,22,0,0,539,540,3,78,39,0,540,541,5,23,0,0,541,543,1,0,0,0,542,
  	534,1,0,0,0,542,535,1,0,0,0,542,536,1,0,0,0,542,537,1,0,0,0,542,538,1,
  	0,0,0,543,101,1,0,0,0,544,545,7,10,0,0,545,546,5,22,0,0,546,547,3,122,
  	61,0,547,548,5,23,0,0,548,103,1,0,0,0,549,554,3,78,39,0,550,551,5,28,
  	0,0,551,553,3,78,39,0,552,550,1,0,0,0,553,556,1,0,0,0,554,552,1,0,0,0,
  	554,555,1,0,0,0,555,558,1,0,0,0,556,554,1,0,0,0,557,559,5,28,0,0,558,
  	557,1,0,0,0,558,559,1,0,0,0,559,105,1,0,0,0,560,561,7,11,0,0,561,107,
  	1,0,0,0,562,566,3,112,56,0,563,566,3,118,59,0,564,566,3,110,55,0,565,
  	562,1,0,0,0,565,563,1,0,0,0,565,564,1,0,0,0,566,109,1,0,0,0,567,569,5,
  	26,0,0,568,570,3,120,60,0,569,568,1,0,0,0,569,570,1,0,0,0,570,571,1,0,
  	0,0,571,572,5,27,0,0,572,111,1,0,0,0,573,575,5,24,0,0,574,576,3,114,57,
  	0,575,574,1,0,0,0,575,576,1,0,0,0,576,577,1,0,0,0,577,578,5,25,0,0,578,
  	113,1,0,0,0,579,584,3,116,58,0,580,581,5,28,0,0,581,583,3,116,58,0,582,
  	580,1,0,0,0,583,586,1,0,0,0,584,582,1,0,0,0,584,585,1,0,0,0,585,588,1,
  	0,0,0,586,584,1,0,0,0,587,589,5,28,0,0,588,587,1,0,0,0,588,589,1,0,0,
  	0,589,115,1,0,0,0,590,591,3,78,39,0,591,592,5,29,0,0,592,593,3,78,39,
  	0,593,117,1,0,0,0,594,595,5,24,0,0,595,597,3,120,60,0,596,598,5,28,0,
  	0,597,596,1,0,0,0,597,598,1,0,0,0,598,599,1,0,0,0,599,600,5,25,0,0,600,
  	119,1,0,0,0,601,606,3,78,39,0,602,603,5,28,0,0,603,605,3,78,39,0,604,
  	602,1,0,0,0,605,608,1,0,0,0,606,604,1,0,0,0,606,607,1,0,0,0,607,610,1,
  	0,0,0,608,606,1,0,0,0,609,611,5,28,0,0,610,609,1,0,0,0,610,611,1,0,0,
  	0,611,121,1,0,0,0,612,615,3,124,62,0,613,615,3,126,63,0,614,612,1,0,0,
  	0,614,613,1,0,0,0,615,123,1,0,0,0,616,617,3,126,63,0,617,618,5,29,0,0,
  	618,619,3,126,63,0,619,125,1,0,0,0,620,624,3,128,64,0,621,623,3,130,65,
  	0,622,621,1,0,0,0,623,626,1,0,0,0,624,622,1,0,0,0,624,625,1,0,0,0,625,
  	127,1,0,0,0,626,624,1,0,0,0,627,628,7,12,0,0,628,129,1,0,0,0,629,630,
  	5,26,0,0,630,637,5,27,0,0,631,632,5,26,0,0,632,633,5,58,0,0,633,637,5,
  	27,0,0,634,635,5,22,0,0,635,637,5,23,0,0,636,629,1,0,0,0,636,631,1,0,
  	0,0,636,634,1,0,0,0,637,131,1,0,0,0,638,639,7,13,0,0,639,133,1,0,0,0,
  	73,137,149,154,160,166,170,179,183,186,189,193,201,206,211,218,224,230,
  	236,244,249,254,260,270,274,283,299,305,308,314,320,327,331,334,340,346,
  	348,354,364,388,399,406,409,413,417,422,428,436,440,456,464,472,480,488,
  	496,504,510,516,527,532,542,554,558,565,569,575,584,588,597,606,610,614,
  	624,636
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


std::any ClythV1Parser::ProgramContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ClythV1Visitor*>(visitor))
    return parserVisitor->visitProgram(this);
  else
    return visitor->visitChildren(this);
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
    setState(137);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 4531184175180018934) != 0)) {
      setState(134);
      topLevelItem();
      setState(139);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(140);
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

ClythV1Parser::MethodBlockContext* ClythV1Parser::TopLevelItemContext::methodBlock() {
  return getRuleContext<ClythV1Parser::MethodBlockContext>(0);
}

ClythV1Parser::FunctionDeclContext* ClythV1Parser::TopLevelItemContext::functionDecl() {
  return getRuleContext<ClythV1Parser::FunctionDeclContext>(0);
}

ClythV1Parser::MethodDeclContext* ClythV1Parser::TopLevelItemContext::methodDecl() {
  return getRuleContext<ClythV1Parser::MethodDeclContext>(0);
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


std::any ClythV1Parser::TopLevelItemContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ClythV1Visitor*>(visitor))
    return parserVisitor->visitTopLevelItem(this);
  else
    return visitor->visitChildren(this);
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
    setState(149);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 1, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(142);
      includeDecl();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(143);
      externDecl();
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(144);
      structDecl();
      break;
    }

    case 4: {
      enterOuterAlt(_localctx, 4);
      setState(145);
      methodBlock();
      break;
    }

    case 5: {
      enterOuterAlt(_localctx, 5);
      setState(146);
      functionDecl();
      break;
    }

    case 6: {
      enterOuterAlt(_localctx, 6);
      setState(147);
      methodDecl();
      break;
    }

    case 7: {
      enterOuterAlt(_localctx, 7);
      setState(148);
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


std::any ClythV1Parser::IncludeDeclContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ClythV1Visitor*>(visitor))
    return parserVisitor->visitIncludeDecl(this);
  else
    return visitor->visitChildren(this);
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
    setState(151);
    match(ClythV1Parser::INCLUDE);
    setState(152);
    includeTarget();
    setState(154);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ClythV1Parser::SEMI) {
      setState(153);
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


std::any ClythV1Parser::IncludeTargetContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ClythV1Visitor*>(visitor))
    return parserVisitor->visitIncludeTarget(this);
  else
    return visitor->visitChildren(this);
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
    setState(156);
    _la = _input->LA(1);
    if (!((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 4035225266123964416) != 0))) {
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

tree::TerminalNode* ClythV1Parser::ExternDeclContext::LPAREN() {
  return getToken(ClythV1Parser::LPAREN, 0);
}

tree::TerminalNode* ClythV1Parser::ExternDeclContext::RPAREN() {
  return getToken(ClythV1Parser::RPAREN, 0);
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


std::any ClythV1Parser::ExternDeclContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ClythV1Visitor*>(visitor))
    return parserVisitor->visitExternDecl(this);
  else
    return visitor->visitChildren(this);
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
    setState(158);
    match(ClythV1Parser::EXTERN);
    setState(160);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 3, _ctx)) {
    case 1: {
      setState(159);
      externAbi();
      break;
    }

    default:
      break;
    }
    setState(162);
    type();
    setState(163);
    match(ClythV1Parser::IDENTIFIER);
    setState(164);
    match(ClythV1Parser::LPAREN);
    setState(166);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 2522015791327477760) != 0)) {
      setState(165);
      externParamList();
    }
    setState(168);
    match(ClythV1Parser::RPAREN);
    setState(170);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ClythV1Parser::SEMI) {
      setState(169);
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


std::any ClythV1Parser::ExternAbiContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ClythV1Visitor*>(visitor))
    return parserVisitor->visitExternAbi(this);
  else
    return visitor->visitChildren(this);
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
    setState(172);
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

std::vector<tree::TerminalNode *> ClythV1Parser::ExternParamListContext::COMMA() {
  return getTokens(ClythV1Parser::COMMA);
}

tree::TerminalNode* ClythV1Parser::ExternParamListContext::COMMA(size_t i) {
  return getToken(ClythV1Parser::COMMA, i);
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


std::any ClythV1Parser::ExternParamListContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ClythV1Visitor*>(visitor))
    return parserVisitor->visitExternParamList(this);
  else
    return visitor->visitChildren(this);
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
    setState(189);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case ClythV1Parser::BUILTIN_TYPE:
      case ClythV1Parser::IDENTIFIER: {
        enterOuterAlt(_localctx, 1);
        setState(174);
        externParam();
        setState(179);
        _errHandler->sync(this);
        alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 6, _ctx);
        while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
          if (alt == 1) {
            setState(175);
            match(ClythV1Parser::COMMA);
            setState(176);
            externParam(); 
          }
          setState(181);
          _errHandler->sync(this);
          alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 6, _ctx);
        }
        setState(183);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == ClythV1Parser::COMMA) {
          setState(182);
          match(ClythV1Parser::COMMA);
        }
        setState(186);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == ClythV1Parser::VARARGS) {
          setState(185);
          match(ClythV1Parser::VARARGS);
        }
        break;
      }

      case ClythV1Parser::VARARGS: {
        enterOuterAlt(_localctx, 2);
        setState(188);
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


std::any ClythV1Parser::ExternParamContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ClythV1Visitor*>(visitor))
    return parserVisitor->visitExternParam(this);
  else
    return visitor->visitChildren(this);
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
    setState(191);
    type();
    setState(193);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ClythV1Parser::IDENTIFIER) {
      setState(192);
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

tree::TerminalNode* ClythV1Parser::StructDeclContext::LBRACE() {
  return getToken(ClythV1Parser::LBRACE, 0);
}

tree::TerminalNode* ClythV1Parser::StructDeclContext::RBRACE() {
  return getToken(ClythV1Parser::RBRACE, 0);
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


std::any ClythV1Parser::StructDeclContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ClythV1Visitor*>(visitor))
    return parserVisitor->visitStructDecl(this);
  else
    return visitor->visitChildren(this);
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
    setState(195);
    match(ClythV1Parser::STRUCT);
    setState(196);
    match(ClythV1Parser::IDENTIFIER);
    setState(197);
    match(ClythV1Parser::LBRACE);
    setState(201);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == ClythV1Parser::BUILTIN_TYPE

    || _la == ClythV1Parser::IDENTIFIER) {
      setState(198);
      structField();
      setState(203);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(204);
    match(ClythV1Parser::RBRACE);
    setState(206);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ClythV1Parser::SEMI) {
      setState(205);
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


std::any ClythV1Parser::StructFieldContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ClythV1Visitor*>(visitor))
    return parserVisitor->visitStructField(this);
  else
    return visitor->visitChildren(this);
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
    setState(208);
    type();
    setState(209);
    match(ClythV1Parser::IDENTIFIER);
    setState(211);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ClythV1Parser::COMMA

    || _la == ClythV1Parser::SEMI) {
      setState(210);
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

//----------------- MethodBlockContext ------------------------------------------------------------------

ClythV1Parser::MethodBlockContext::MethodBlockContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ClythV1Parser::MethodBlockContext::IDENTIFIER() {
  return getToken(ClythV1Parser::IDENTIFIER, 0);
}

tree::TerminalNode* ClythV1Parser::MethodBlockContext::LBRACE() {
  return getToken(ClythV1Parser::LBRACE, 0);
}

tree::TerminalNode* ClythV1Parser::MethodBlockContext::RBRACE() {
  return getToken(ClythV1Parser::RBRACE, 0);
}

std::vector<ClythV1Parser::MethodDeclContext *> ClythV1Parser::MethodBlockContext::methodDecl() {
  return getRuleContexts<ClythV1Parser::MethodDeclContext>();
}

ClythV1Parser::MethodDeclContext* ClythV1Parser::MethodBlockContext::methodDecl(size_t i) {
  return getRuleContext<ClythV1Parser::MethodDeclContext>(i);
}


size_t ClythV1Parser::MethodBlockContext::getRuleIndex() const {
  return ClythV1Parser::RuleMethodBlock;
}

void ClythV1Parser::MethodBlockContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterMethodBlock(this);
}

void ClythV1Parser::MethodBlockContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitMethodBlock(this);
}


std::any ClythV1Parser::MethodBlockContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ClythV1Visitor*>(visitor))
    return parserVisitor->visitMethodBlock(this);
  else
    return visitor->visitChildren(this);
}

ClythV1Parser::MethodBlockContext* ClythV1Parser::methodBlock() {
  MethodBlockContext *_localctx = _tracker.createInstance<MethodBlockContext>(_ctx, getState());
  enterRule(_localctx, 20, ClythV1Parser::RuleMethodBlock);
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
    setState(213);
    match(ClythV1Parser::IDENTIFIER);
    setState(214);
    match(ClythV1Parser::LBRACE);
    setState(218);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 2449958197289549856) != 0)) {
      setState(215);
      methodDecl();
      setState(220);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(221);
    match(ClythV1Parser::RBRACE);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- MethodDeclContext ------------------------------------------------------------------

ClythV1Parser::MethodDeclContext::MethodDeclContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ClythV1Parser::TypeContext* ClythV1Parser::MethodDeclContext::type() {
  return getRuleContext<ClythV1Parser::TypeContext>(0);
}

ClythV1Parser::MethodSimpleNameContext* ClythV1Parser::MethodDeclContext::methodSimpleName() {
  return getRuleContext<ClythV1Parser::MethodSimpleNameContext>(0);
}

tree::TerminalNode* ClythV1Parser::MethodDeclContext::LPAREN() {
  return getToken(ClythV1Parser::LPAREN, 0);
}

tree::TerminalNode* ClythV1Parser::MethodDeclContext::RPAREN() {
  return getToken(ClythV1Parser::RPAREN, 0);
}

ClythV1Parser::BlockContext* ClythV1Parser::MethodDeclContext::block() {
  return getRuleContext<ClythV1Parser::BlockContext>(0);
}

tree::TerminalNode* ClythV1Parser::MethodDeclContext::MECC() {
  return getToken(ClythV1Parser::MECC, 0);
}

ClythV1Parser::ParamListContext* ClythV1Parser::MethodDeclContext::paramList() {
  return getRuleContext<ClythV1Parser::ParamListContext>(0);
}

tree::TerminalNode* ClythV1Parser::MethodDeclContext::IDENTIFIER() {
  return getToken(ClythV1Parser::IDENTIFIER, 0);
}

tree::TerminalNode* ClythV1Parser::MethodDeclContext::DOT() {
  return getToken(ClythV1Parser::DOT, 0);
}


size_t ClythV1Parser::MethodDeclContext::getRuleIndex() const {
  return ClythV1Parser::RuleMethodDecl;
}

void ClythV1Parser::MethodDeclContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterMethodDecl(this);
}

void ClythV1Parser::MethodDeclContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitMethodDecl(this);
}


std::any ClythV1Parser::MethodDeclContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ClythV1Visitor*>(visitor))
    return parserVisitor->visitMethodDecl(this);
  else
    return visitor->visitChildren(this);
}

ClythV1Parser::MethodDeclContext* ClythV1Parser::methodDecl() {
  MethodDeclContext *_localctx = _tracker.createInstance<MethodDeclContext>(_ctx, getState());
  enterRule(_localctx, 22, ClythV1Parser::RuleMethodDecl);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(249);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 19, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(224);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == ClythV1Parser::MECC) {
        setState(223);
        match(ClythV1Parser::MECC);
      }
      setState(226);
      type();
      setState(227);
      methodSimpleName();
      setState(228);
      match(ClythV1Parser::LPAREN);
      setState(230);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == ClythV1Parser::BUILTIN_TYPE

      || _la == ClythV1Parser::IDENTIFIER) {
        setState(229);
        paramList();
      }
      setState(232);
      match(ClythV1Parser::RPAREN);
      setState(233);
      block();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(236);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == ClythV1Parser::MECC) {
        setState(235);
        match(ClythV1Parser::MECC);
      }
      setState(238);
      type();
      setState(239);
      match(ClythV1Parser::IDENTIFIER);
      setState(240);
      match(ClythV1Parser::DOT);
      setState(241);
      methodSimpleName();
      setState(242);
      match(ClythV1Parser::LPAREN);
      setState(244);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == ClythV1Parser::BUILTIN_TYPE

      || _la == ClythV1Parser::IDENTIFIER) {
        setState(243);
        paramList();
      }
      setState(246);
      match(ClythV1Parser::RPAREN);
      setState(247);
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

//----------------- MethodSimpleNameContext ------------------------------------------------------------------

ClythV1Parser::MethodSimpleNameContext::MethodSimpleNameContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ClythV1Parser::MethodSimpleNameContext::IDENTIFIER() {
  return getToken(ClythV1Parser::IDENTIFIER, 0);
}

tree::TerminalNode* ClythV1Parser::MethodSimpleNameContext::CONSTRUCTOR() {
  return getToken(ClythV1Parser::CONSTRUCTOR, 0);
}

tree::TerminalNode* ClythV1Parser::MethodSimpleNameContext::DESTRUCTOR() {
  return getToken(ClythV1Parser::DESTRUCTOR, 0);
}


size_t ClythV1Parser::MethodSimpleNameContext::getRuleIndex() const {
  return ClythV1Parser::RuleMethodSimpleName;
}

void ClythV1Parser::MethodSimpleNameContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterMethodSimpleName(this);
}

void ClythV1Parser::MethodSimpleNameContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitMethodSimpleName(this);
}


std::any ClythV1Parser::MethodSimpleNameContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ClythV1Visitor*>(visitor))
    return parserVisitor->visitMethodSimpleName(this);
  else
    return visitor->visitChildren(this);
}

ClythV1Parser::MethodSimpleNameContext* ClythV1Parser::methodSimpleName() {
  MethodSimpleNameContext *_localctx = _tracker.createInstance<MethodSimpleNameContext>(_ctx, getState());
  enterRule(_localctx, 24, ClythV1Parser::RuleMethodSimpleName);
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
    setState(251);
    _la = _input->LA(1);
    if (!((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 2305843009213694720) != 0))) {
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

tree::TerminalNode* ClythV1Parser::FunctionDeclContext::LPAREN() {
  return getToken(ClythV1Parser::LPAREN, 0);
}

tree::TerminalNode* ClythV1Parser::FunctionDeclContext::RPAREN() {
  return getToken(ClythV1Parser::RPAREN, 0);
}

ClythV1Parser::BlockContext* ClythV1Parser::FunctionDeclContext::block() {
  return getRuleContext<ClythV1Parser::BlockContext>(0);
}

tree::TerminalNode* ClythV1Parser::FunctionDeclContext::MECC() {
  return getToken(ClythV1Parser::MECC, 0);
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


std::any ClythV1Parser::FunctionDeclContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ClythV1Visitor*>(visitor))
    return parserVisitor->visitFunctionDecl(this);
  else
    return visitor->visitChildren(this);
}

ClythV1Parser::FunctionDeclContext* ClythV1Parser::functionDecl() {
  FunctionDeclContext *_localctx = _tracker.createInstance<FunctionDeclContext>(_ctx, getState());
  enterRule(_localctx, 26, ClythV1Parser::RuleFunctionDecl);
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
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ClythV1Parser::MECC) {
      setState(253);
      match(ClythV1Parser::MECC);
    }
    setState(256);
    type();
    setState(257);
    match(ClythV1Parser::IDENTIFIER);
    setState(258);
    match(ClythV1Parser::LPAREN);
    setState(260);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ClythV1Parser::BUILTIN_TYPE

    || _la == ClythV1Parser::IDENTIFIER) {
      setState(259);
      paramList();
    }
    setState(262);
    match(ClythV1Parser::RPAREN);
    setState(263);
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

std::vector<tree::TerminalNode *> ClythV1Parser::ParamListContext::COMMA() {
  return getTokens(ClythV1Parser::COMMA);
}

tree::TerminalNode* ClythV1Parser::ParamListContext::COMMA(size_t i) {
  return getToken(ClythV1Parser::COMMA, i);
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


std::any ClythV1Parser::ParamListContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ClythV1Visitor*>(visitor))
    return parserVisitor->visitParamList(this);
  else
    return visitor->visitChildren(this);
}

ClythV1Parser::ParamListContext* ClythV1Parser::paramList() {
  ParamListContext *_localctx = _tracker.createInstance<ParamListContext>(_ctx, getState());
  enterRule(_localctx, 28, ClythV1Parser::RuleParamList);
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
    setState(265);
    param();
    setState(270);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 22, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(266);
        match(ClythV1Parser::COMMA);
        setState(267);
        param(); 
      }
      setState(272);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 22, _ctx);
    }
    setState(274);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ClythV1Parser::COMMA) {
      setState(273);
      match(ClythV1Parser::COMMA);
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


std::any ClythV1Parser::ParamContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ClythV1Visitor*>(visitor))
    return parserVisitor->visitParam(this);
  else
    return visitor->visitChildren(this);
}

ClythV1Parser::ParamContext* ClythV1Parser::param() {
  ParamContext *_localctx = _tracker.createInstance<ParamContext>(_ctx, getState());
  enterRule(_localctx, 30, ClythV1Parser::RuleParam);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(276);
    type();
    setState(277);
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

tree::TerminalNode* ClythV1Parser::BlockContext::LBRACE() {
  return getToken(ClythV1Parser::LBRACE, 0);
}

tree::TerminalNode* ClythV1Parser::BlockContext::RBRACE() {
  return getToken(ClythV1Parser::RBRACE, 0);
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


std::any ClythV1Parser::BlockContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ClythV1Visitor*>(visitor))
    return parserVisitor->visitBlock(this);
  else
    return visitor->visitChildren(this);
}

ClythV1Parser::BlockContext* ClythV1Parser::block() {
  BlockContext *_localctx = _tracker.createInstance<BlockContext>(_ctx, getState());
  enterRule(_localctx, 32, ClythV1Parser::RuleBlock);
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
    setState(279);
    match(ClythV1Parser::LBRACE);
    setState(283);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 4531184175180018912) != 0)) {
      setState(280);
      statement();
      setState(285);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(286);
    match(ClythV1Parser::RBRACE);
   
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

ClythV1Parser::MeccBlockContext* ClythV1Parser::StatementContext::meccBlock() {
  return getRuleContext<ClythV1Parser::MeccBlockContext>(0);
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


std::any ClythV1Parser::StatementContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ClythV1Visitor*>(visitor))
    return parserVisitor->visitStatement(this);
  else
    return visitor->visitChildren(this);
}

ClythV1Parser::StatementContext* ClythV1Parser::statement() {
  StatementContext *_localctx = _tracker.createInstance<StatementContext>(_ctx, getState());
  enterRule(_localctx, 34, ClythV1Parser::RuleStatement);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(299);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 25, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(288);
      varDecl();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(289);
      assignmentStmt();
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(290);
      exprStmt();
      break;
    }

    case 4: {
      enterOuterAlt(_localctx, 4);
      setState(291);
      returnStmt();
      break;
    }

    case 5: {
      enterOuterAlt(_localctx, 5);
      setState(292);
      ifStmt();
      break;
    }

    case 6: {
      enterOuterAlt(_localctx, 6);
      setState(293);
      whileStmt();
      break;
    }

    case 7: {
      enterOuterAlt(_localctx, 7);
      setState(294);
      forStmt();
      break;
    }

    case 8: {
      enterOuterAlt(_localctx, 8);
      setState(295);
      breakStmt();
      break;
    }

    case 9: {
      enterOuterAlt(_localctx, 9);
      setState(296);
      continueStmt();
      break;
    }

    case 10: {
      enterOuterAlt(_localctx, 10);
      setState(297);
      meccBlock();
      break;
    }

    case 11: {
      enterOuterAlt(_localctx, 11);
      setState(298);
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

tree::TerminalNode* ClythV1Parser::VarDeclContext::ASSIGN() {
  return getToken(ClythV1Parser::ASSIGN, 0);
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


std::any ClythV1Parser::VarDeclContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ClythV1Visitor*>(visitor))
    return parserVisitor->visitVarDecl(this);
  else
    return visitor->visitChildren(this);
}

ClythV1Parser::VarDeclContext* ClythV1Parser::varDecl() {
  VarDeclContext *_localctx = _tracker.createInstance<VarDeclContext>(_ctx, getState());
  enterRule(_localctx, 36, ClythV1Parser::RuleVarDecl);
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
    setState(301);
    type();
    setState(302);
    match(ClythV1Parser::IDENTIFIER);
    setState(305);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ClythV1Parser::ASSIGN) {
      setState(303);
      match(ClythV1Parser::ASSIGN);
      setState(304);
      expression();
    }
    setState(308);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ClythV1Parser::SEMI) {
      setState(307);
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


std::any ClythV1Parser::AssignmentStmtContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ClythV1Visitor*>(visitor))
    return parserVisitor->visitAssignmentStmt(this);
  else
    return visitor->visitChildren(this);
}

ClythV1Parser::AssignmentStmtContext* ClythV1Parser::assignmentStmt() {
  AssignmentStmtContext *_localctx = _tracker.createInstance<AssignmentStmtContext>(_ctx, getState());
  enterRule(_localctx, 38, ClythV1Parser::RuleAssignmentStmt);
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
    setState(310);
    assignable();
    setState(311);
    assignmentOp();
    setState(312);
    expression();
    setState(314);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ClythV1Parser::SEMI) {
      setState(313);
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


std::any ClythV1Parser::AssignableContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ClythV1Visitor*>(visitor))
    return parserVisitor->visitAssignable(this);
  else
    return visitor->visitChildren(this);
}

ClythV1Parser::AssignableContext* ClythV1Parser::assignable() {
  AssignableContext *_localctx = _tracker.createInstance<AssignableContext>(_ctx, getState());
  enterRule(_localctx, 40, ClythV1Parser::RuleAssignable);
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
    setState(316);
    match(ClythV1Parser::IDENTIFIER);
    setState(320);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 3300753670144) != 0)) {
      setState(317);
      postfixSuffix();
      setState(322);
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

tree::TerminalNode* ClythV1Parser::AssignmentOpContext::ASSIGN() {
  return getToken(ClythV1Parser::ASSIGN, 0);
}

tree::TerminalNode* ClythV1Parser::AssignmentOpContext::PLUS_ASSIGN() {
  return getToken(ClythV1Parser::PLUS_ASSIGN, 0);
}

tree::TerminalNode* ClythV1Parser::AssignmentOpContext::MINUS_ASSIGN() {
  return getToken(ClythV1Parser::MINUS_ASSIGN, 0);
}

tree::TerminalNode* ClythV1Parser::AssignmentOpContext::STAR_ASSIGN() {
  return getToken(ClythV1Parser::STAR_ASSIGN, 0);
}

tree::TerminalNode* ClythV1Parser::AssignmentOpContext::SLASH_ASSIGN() {
  return getToken(ClythV1Parser::SLASH_ASSIGN, 0);
}

tree::TerminalNode* ClythV1Parser::AssignmentOpContext::PERCENT_ASSIGN() {
  return getToken(ClythV1Parser::PERCENT_ASSIGN, 0);
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


std::any ClythV1Parser::AssignmentOpContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ClythV1Visitor*>(visitor))
    return parserVisitor->visitAssignmentOp(this);
  else
    return visitor->visitChildren(this);
}

ClythV1Parser::AssignmentOpContext* ClythV1Parser::assignmentOp() {
  AssignmentOpContext *_localctx = _tracker.createInstance<AssignmentOpContext>(_ctx, getState());
  enterRule(_localctx, 42, ClythV1Parser::RuleAssignmentOp);
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
    setState(323);
    _la = _input->LA(1);
    if (!((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 277076930199552) != 0))) {
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


std::any ClythV1Parser::ExprStmtContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ClythV1Visitor*>(visitor))
    return parserVisitor->visitExprStmt(this);
  else
    return visitor->visitChildren(this);
}

ClythV1Parser::ExprStmtContext* ClythV1Parser::exprStmt() {
  ExprStmtContext *_localctx = _tracker.createInstance<ExprStmtContext>(_ctx, getState());
  enterRule(_localctx, 44, ClythV1Parser::RuleExprStmt);
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
    setState(325);
    expression();
    setState(327);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ClythV1Parser::SEMI) {
      setState(326);
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


std::any ClythV1Parser::ReturnStmtContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ClythV1Visitor*>(visitor))
    return parserVisitor->visitReturnStmt(this);
  else
    return visitor->visitChildren(this);
}

ClythV1Parser::ReturnStmtContext* ClythV1Parser::returnStmt() {
  ReturnStmtContext *_localctx = _tracker.createInstance<ReturnStmtContext>(_ctx, getState());
  enterRule(_localctx, 46, ClythV1Parser::RuleReturnStmt);
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
    setState(329);
    match(ClythV1Parser::RETURN);
    setState(331);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 31, _ctx)) {
    case 1: {
      setState(330);
      expression();
      break;
    }

    default:
      break;
    }
    setState(334);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ClythV1Parser::SEMI) {
      setState(333);
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


std::any ClythV1Parser::IfStmtContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ClythV1Visitor*>(visitor))
    return parserVisitor->visitIfStmt(this);
  else
    return visitor->visitChildren(this);
}

ClythV1Parser::IfStmtContext* ClythV1Parser::ifStmt() {
  IfStmtContext *_localctx = _tracker.createInstance<IfStmtContext>(_ctx, getState());
  enterRule(_localctx, 48, ClythV1Parser::RuleIfStmt);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(348);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 35, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(336);
      match(ClythV1Parser::IF);
      setState(337);
      parenExpr();
      setState(338);
      block();
      setState(340);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == ClythV1Parser::ELSE) {
        setState(339);
        elseClause();
      }
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(342);
      match(ClythV1Parser::IF);
      setState(343);
      expression();
      setState(344);
      block();
      setState(346);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == ClythV1Parser::ELSE) {
        setState(345);
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


std::any ClythV1Parser::ElseClauseContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ClythV1Visitor*>(visitor))
    return parserVisitor->visitElseClause(this);
  else
    return visitor->visitChildren(this);
}

ClythV1Parser::ElseClauseContext* ClythV1Parser::elseClause() {
  ElseClauseContext *_localctx = _tracker.createInstance<ElseClauseContext>(_ctx, getState());
  enterRule(_localctx, 50, ClythV1Parser::RuleElseClause);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(354);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 36, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(350);
      match(ClythV1Parser::ELSE);
      setState(351);
      ifStmt();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(352);
      match(ClythV1Parser::ELSE);
      setState(353);
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


std::any ClythV1Parser::WhileStmtContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ClythV1Visitor*>(visitor))
    return parserVisitor->visitWhileStmt(this);
  else
    return visitor->visitChildren(this);
}

ClythV1Parser::WhileStmtContext* ClythV1Parser::whileStmt() {
  WhileStmtContext *_localctx = _tracker.createInstance<WhileStmtContext>(_ctx, getState());
  enterRule(_localctx, 52, ClythV1Parser::RuleWhileStmt);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(364);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 37, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(356);
      match(ClythV1Parser::WHILE);
      setState(357);
      parenExpr();
      setState(358);
      block();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(360);
      match(ClythV1Parser::WHILE);
      setState(361);
      expression();
      setState(362);
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

tree::TerminalNode* ClythV1Parser::ForStmtContext::LPAREN() {
  return getToken(ClythV1Parser::LPAREN, 0);
}

tree::TerminalNode* ClythV1Parser::ForStmtContext::RPAREN() {
  return getToken(ClythV1Parser::RPAREN, 0);
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


std::any ClythV1Parser::ForStmtContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ClythV1Visitor*>(visitor))
    return parserVisitor->visitForStmt(this);
  else
    return visitor->visitChildren(this);
}

ClythV1Parser::ForStmtContext* ClythV1Parser::forStmt() {
  ForStmtContext *_localctx = _tracker.createInstance<ForStmtContext>(_ctx, getState());
  enterRule(_localctx, 54, ClythV1Parser::RuleForStmt);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(388);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 38, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(366);
      match(ClythV1Parser::FOR);
      setState(367);
      block();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(368);
      match(ClythV1Parser::FOR);
      setState(369);
      forEachHeader();
      setState(370);
      block();
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(372);
      match(ClythV1Parser::FOR);
      setState(373);
      match(ClythV1Parser::LPAREN);
      setState(374);
      forEachHeader();
      setState(375);
      match(ClythV1Parser::RPAREN);
      setState(376);
      block();
      break;
    }

    case 4: {
      enterOuterAlt(_localctx, 4);
      setState(378);
      match(ClythV1Parser::FOR);
      setState(379);
      forClassicHeader();
      setState(380);
      block();
      break;
    }

    case 5: {
      enterOuterAlt(_localctx, 5);
      setState(382);
      match(ClythV1Parser::FOR);
      setState(383);
      match(ClythV1Parser::LPAREN);
      setState(384);
      forClassicHeader();
      setState(385);
      match(ClythV1Parser::RPAREN);
      setState(386);
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


std::any ClythV1Parser::ForEachHeaderContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ClythV1Visitor*>(visitor))
    return parserVisitor->visitForEachHeader(this);
  else
    return visitor->visitChildren(this);
}

ClythV1Parser::ForEachHeaderContext* ClythV1Parser::forEachHeader() {
  ForEachHeaderContext *_localctx = _tracker.createInstance<ForEachHeaderContext>(_ctx, getState());
  enterRule(_localctx, 56, ClythV1Parser::RuleForEachHeader);

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
    forEachBinding();
    setState(391);
    match(ClythV1Parser::IN);
    setState(392);
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

std::vector<tree::TerminalNode *> ClythV1Parser::ForEachBindingContext::COMMA() {
  return getTokens(ClythV1Parser::COMMA);
}

tree::TerminalNode* ClythV1Parser::ForEachBindingContext::COMMA(size_t i) {
  return getToken(ClythV1Parser::COMMA, i);
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


std::any ClythV1Parser::ForEachBindingContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ClythV1Visitor*>(visitor))
    return parserVisitor->visitForEachBinding(this);
  else
    return visitor->visitChildren(this);
}

ClythV1Parser::ForEachBindingContext* ClythV1Parser::forEachBinding() {
  ForEachBindingContext *_localctx = _tracker.createInstance<ForEachBindingContext>(_ctx, getState());
  enterRule(_localctx, 58, ClythV1Parser::RuleForEachBinding);
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
    setState(394);
    forEachVar();
    setState(399);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == ClythV1Parser::COMMA) {
      setState(395);
      match(ClythV1Parser::COMMA);
      setState(396);
      forEachVar();
      setState(401);
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


std::any ClythV1Parser::ForEachVarContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ClythV1Visitor*>(visitor))
    return parserVisitor->visitForEachVar(this);
  else
    return visitor->visitChildren(this);
}

ClythV1Parser::ForEachVarContext* ClythV1Parser::forEachVar() {
  ForEachVarContext *_localctx = _tracker.createInstance<ForEachVarContext>(_ctx, getState());
  enterRule(_localctx, 60, ClythV1Parser::RuleForEachVar);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(406);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 40, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(402);
      type();
      setState(403);
      match(ClythV1Parser::IDENTIFIER);
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(405);
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


std::any ClythV1Parser::ForClassicHeaderContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ClythV1Visitor*>(visitor))
    return parserVisitor->visitForClassicHeader(this);
  else
    return visitor->visitChildren(this);
}

ClythV1Parser::ForClassicHeaderContext* ClythV1Parser::forClassicHeader() {
  ForClassicHeaderContext *_localctx = _tracker.createInstance<ForClassicHeaderContext>(_ctx, getState());
  enterRule(_localctx, 62, ClythV1Parser::RuleForClassicHeader);
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
    setState(409);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 4531184175179890880) != 0)) {
      setState(408);
      forInit();
    }
    setState(411);
    match(ClythV1Parser::SEMI);
    setState(413);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 4387068987104035008) != 0)) {
      setState(412);
      expression();
    }
    setState(415);
    match(ClythV1Parser::SEMI);
    setState(417);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 43, _ctx)) {
    case 1: {
      setState(416);
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


std::any ClythV1Parser::ForInitContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ClythV1Visitor*>(visitor))
    return parserVisitor->visitForInit(this);
  else
    return visitor->visitChildren(this);
}

ClythV1Parser::ForInitContext* ClythV1Parser::forInit() {
  ForInitContext *_localctx = _tracker.createInstance<ForInitContext>(_ctx, getState());
  enterRule(_localctx, 64, ClythV1Parser::RuleForInit);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(422);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 44, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(419);
      varDeclNoSemi();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(420);
      assignmentNoSemi();
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(421);
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

tree::TerminalNode* ClythV1Parser::VarDeclNoSemiContext::ASSIGN() {
  return getToken(ClythV1Parser::ASSIGN, 0);
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


std::any ClythV1Parser::VarDeclNoSemiContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ClythV1Visitor*>(visitor))
    return parserVisitor->visitVarDeclNoSemi(this);
  else
    return visitor->visitChildren(this);
}

ClythV1Parser::VarDeclNoSemiContext* ClythV1Parser::varDeclNoSemi() {
  VarDeclNoSemiContext *_localctx = _tracker.createInstance<VarDeclNoSemiContext>(_ctx, getState());
  enterRule(_localctx, 66, ClythV1Parser::RuleVarDeclNoSemi);
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
    type();
    setState(425);
    match(ClythV1Parser::IDENTIFIER);
    setState(428);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ClythV1Parser::ASSIGN) {
      setState(426);
      match(ClythV1Parser::ASSIGN);
      setState(427);
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


std::any ClythV1Parser::AssignmentNoSemiContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ClythV1Visitor*>(visitor))
    return parserVisitor->visitAssignmentNoSemi(this);
  else
    return visitor->visitChildren(this);
}

ClythV1Parser::AssignmentNoSemiContext* ClythV1Parser::assignmentNoSemi() {
  AssignmentNoSemiContext *_localctx = _tracker.createInstance<AssignmentNoSemiContext>(_ctx, getState());
  enterRule(_localctx, 68, ClythV1Parser::RuleAssignmentNoSemi);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(430);
    assignable();
    setState(431);
    assignmentOp();
    setState(432);
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


std::any ClythV1Parser::BreakStmtContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ClythV1Visitor*>(visitor))
    return parserVisitor->visitBreakStmt(this);
  else
    return visitor->visitChildren(this);
}

ClythV1Parser::BreakStmtContext* ClythV1Parser::breakStmt() {
  BreakStmtContext *_localctx = _tracker.createInstance<BreakStmtContext>(_ctx, getState());
  enterRule(_localctx, 70, ClythV1Parser::RuleBreakStmt);
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
    setState(434);
    match(ClythV1Parser::BREAK);
    setState(436);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ClythV1Parser::SEMI) {
      setState(435);
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


std::any ClythV1Parser::ContinueStmtContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ClythV1Visitor*>(visitor))
    return parserVisitor->visitContinueStmt(this);
  else
    return visitor->visitChildren(this);
}

ClythV1Parser::ContinueStmtContext* ClythV1Parser::continueStmt() {
  ContinueStmtContext *_localctx = _tracker.createInstance<ContinueStmtContext>(_ctx, getState());
  enterRule(_localctx, 72, ClythV1Parser::RuleContinueStmt);
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
    setState(438);
    match(ClythV1Parser::CONTINUE);
    setState(440);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ClythV1Parser::SEMI) {
      setState(439);
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

//----------------- MeccBlockContext ------------------------------------------------------------------

ClythV1Parser::MeccBlockContext::MeccBlockContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ClythV1Parser::MeccBlockContext::MECC() {
  return getToken(ClythV1Parser::MECC, 0);
}

ClythV1Parser::BlockContext* ClythV1Parser::MeccBlockContext::block() {
  return getRuleContext<ClythV1Parser::BlockContext>(0);
}


size_t ClythV1Parser::MeccBlockContext::getRuleIndex() const {
  return ClythV1Parser::RuleMeccBlock;
}

void ClythV1Parser::MeccBlockContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterMeccBlock(this);
}

void ClythV1Parser::MeccBlockContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitMeccBlock(this);
}


std::any ClythV1Parser::MeccBlockContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ClythV1Visitor*>(visitor))
    return parserVisitor->visitMeccBlock(this);
  else
    return visitor->visitChildren(this);
}

ClythV1Parser::MeccBlockContext* ClythV1Parser::meccBlock() {
  MeccBlockContext *_localctx = _tracker.createInstance<MeccBlockContext>(_ctx, getState());
  enterRule(_localctx, 74, ClythV1Parser::RuleMeccBlock);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(442);
    match(ClythV1Parser::MECC);
    setState(443);
    block();
   
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

tree::TerminalNode* ClythV1Parser::ParenExprContext::LPAREN() {
  return getToken(ClythV1Parser::LPAREN, 0);
}

ClythV1Parser::ExpressionContext* ClythV1Parser::ParenExprContext::expression() {
  return getRuleContext<ClythV1Parser::ExpressionContext>(0);
}

tree::TerminalNode* ClythV1Parser::ParenExprContext::RPAREN() {
  return getToken(ClythV1Parser::RPAREN, 0);
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


std::any ClythV1Parser::ParenExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ClythV1Visitor*>(visitor))
    return parserVisitor->visitParenExpr(this);
  else
    return visitor->visitChildren(this);
}

ClythV1Parser::ParenExprContext* ClythV1Parser::parenExpr() {
  ParenExprContext *_localctx = _tracker.createInstance<ParenExprContext>(_ctx, getState());
  enterRule(_localctx, 76, ClythV1Parser::RuleParenExpr);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(445);
    match(ClythV1Parser::LPAREN);
    setState(446);
    expression();
    setState(447);
    match(ClythV1Parser::RPAREN);
   
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


std::any ClythV1Parser::ExpressionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ClythV1Visitor*>(visitor))
    return parserVisitor->visitExpression(this);
  else
    return visitor->visitChildren(this);
}

ClythV1Parser::ExpressionContext* ClythV1Parser::expression() {
  ExpressionContext *_localctx = _tracker.createInstance<ExpressionContext>(_ctx, getState());
  enterRule(_localctx, 78, ClythV1Parser::RuleExpression);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(449);
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


std::any ClythV1Parser::LogicalOrContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ClythV1Visitor*>(visitor))
    return parserVisitor->visitLogicalOr(this);
  else
    return visitor->visitChildren(this);
}

ClythV1Parser::LogicalOrContext* ClythV1Parser::logicalOr() {
  LogicalOrContext *_localctx = _tracker.createInstance<LogicalOrContext>(_ctx, getState());
  enterRule(_localctx, 80, ClythV1Parser::RuleLogicalOr);
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
    setState(451);
    logicalAnd();
    setState(456);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == ClythV1Parser::OR) {
      setState(452);
      match(ClythV1Parser::OR);
      setState(453);
      logicalAnd();
      setState(458);
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


std::any ClythV1Parser::LogicalAndContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ClythV1Visitor*>(visitor))
    return parserVisitor->visitLogicalAnd(this);
  else
    return visitor->visitChildren(this);
}

ClythV1Parser::LogicalAndContext* ClythV1Parser::logicalAnd() {
  LogicalAndContext *_localctx = _tracker.createInstance<LogicalAndContext>(_ctx, getState());
  enterRule(_localctx, 82, ClythV1Parser::RuleLogicalAnd);
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
    setState(459);
    equality();
    setState(464);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == ClythV1Parser::AND) {
      setState(460);
      match(ClythV1Parser::AND);
      setState(461);
      equality();
      setState(466);
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


std::any ClythV1Parser::EqualityContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ClythV1Visitor*>(visitor))
    return parserVisitor->visitEquality(this);
  else
    return visitor->visitChildren(this);
}

ClythV1Parser::EqualityContext* ClythV1Parser::equality() {
  EqualityContext *_localctx = _tracker.createInstance<EqualityContext>(_ctx, getState());
  enterRule(_localctx, 84, ClythV1Parser::RuleEquality);
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
    setState(467);
    relation();
    setState(472);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == ClythV1Parser::EQ

    || _la == ClythV1Parser::NEQ) {
      setState(468);
      _la = _input->LA(1);
      if (!(_la == ClythV1Parser::EQ

      || _la == ClythV1Parser::NEQ)) {
      _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(469);
      relation();
      setState(474);
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


std::any ClythV1Parser::RelationContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ClythV1Visitor*>(visitor))
    return parserVisitor->visitRelation(this);
  else
    return visitor->visitChildren(this);
}

ClythV1Parser::RelationContext* ClythV1Parser::relation() {
  RelationContext *_localctx = _tracker.createInstance<RelationContext>(_ctx, getState());
  enterRule(_localctx, 86, ClythV1Parser::RuleRelation);
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
    setState(475);
    comparison();
    setState(480);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == ClythV1Parser::IS

    || _la == ClythV1Parser::IN) {
      setState(476);
      _la = _input->LA(1);
      if (!(_la == ClythV1Parser::IS

      || _la == ClythV1Parser::IN)) {
      _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(477);
      comparison();
      setState(482);
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


std::any ClythV1Parser::ComparisonContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ClythV1Visitor*>(visitor))
    return parserVisitor->visitComparison(this);
  else
    return visitor->visitChildren(this);
}

ClythV1Parser::ComparisonContext* ClythV1Parser::comparison() {
  ComparisonContext *_localctx = _tracker.createInstance<ComparisonContext>(_ctx, getState());
  enterRule(_localctx, 88, ClythV1Parser::RuleComparison);
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
    setState(483);
    additive();
    setState(488);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 1030792151040) != 0)) {
      setState(484);
      _la = _input->LA(1);
      if (!((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & 1030792151040) != 0))) {
      _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(485);
      additive();
      setState(490);
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


std::any ClythV1Parser::AdditiveContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ClythV1Visitor*>(visitor))
    return parserVisitor->visitAdditive(this);
  else
    return visitor->visitChildren(this);
}

ClythV1Parser::AdditiveContext* ClythV1Parser::additive() {
  AdditiveContext *_localctx = _tracker.createInstance<AdditiveContext>(_ctx, getState());
  enterRule(_localctx, 90, ClythV1Parser::RuleAdditive);
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
    setState(491);
    multiplicative();
    setState(496);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 53, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(492);
        _la = _input->LA(1);
        if (!(_la == ClythV1Parser::PLUS

        || _la == ClythV1Parser::MINUS)) {
        _errHandler->recoverInline(this);
        }
        else {
          _errHandler->reportMatch(this);
          consume();
        }
        setState(493);
        multiplicative(); 
      }
      setState(498);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 53, _ctx);
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


std::any ClythV1Parser::MultiplicativeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ClythV1Visitor*>(visitor))
    return parserVisitor->visitMultiplicative(this);
  else
    return visitor->visitChildren(this);
}

ClythV1Parser::MultiplicativeContext* ClythV1Parser::multiplicative() {
  MultiplicativeContext *_localctx = _tracker.createInstance<MultiplicativeContext>(_ctx, getState());
  enterRule(_localctx, 92, ClythV1Parser::RuleMultiplicative);
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
    setState(499);
    unary();
    setState(504);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 7881299347898368) != 0)) {
      setState(500);
      _la = _input->LA(1);
      if (!((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & 7881299347898368) != 0))) {
      _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(501);
      unary();
      setState(506);
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


std::any ClythV1Parser::UnaryContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ClythV1Visitor*>(visitor))
    return parserVisitor->visitUnary(this);
  else
    return visitor->visitChildren(this);
}

ClythV1Parser::UnaryContext* ClythV1Parser::unary() {
  UnaryContext *_localctx = _tracker.createInstance<UnaryContext>(_ctx, getState());
  enterRule(_localctx, 94, ClythV1Parser::RuleUnary);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(510);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case ClythV1Parser::NOT:
      case ClythV1Parser::MINUS:
      case ClythV1Parser::BANG:
      case ClythV1Parser::TILDE:
      case ClythV1Parser::AMP: {
        enterOuterAlt(_localctx, 1);
        setState(507);
        _la = _input->LA(1);
        if (!((((_la & ~ 0x3fULL) == 0) &&
          ((1ULL << _la) & 63613344737132544) != 0))) {
        _errHandler->recoverInline(this);
        }
        else {
          _errHandler->reportMatch(this);
          consume();
        }
        setState(508);
        unary();
        break;
      }

      case ClythV1Parser::MALLOC:
      case ClythV1Parser::ISO_MALLOC:
      case ClythV1Parser::BOOLEAN_LITERAL:
      case ClythV1Parser::NULL_LITERAL:
      case ClythV1Parser::LPAREN:
      case ClythV1Parser::LBRACE:
      case ClythV1Parser::LBRACKET:
      case ClythV1Parser::NUMERIC_LITERAL:
      case ClythV1Parser::TEMPLATE_STRING_LITERAL:
      case ClythV1Parser::STRING_LITERAL:
      case ClythV1Parser::IDENTIFIER: {
        enterOuterAlt(_localctx, 2);
        setState(509);
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


std::any ClythV1Parser::PostfixContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ClythV1Visitor*>(visitor))
    return parserVisitor->visitPostfix(this);
  else
    return visitor->visitChildren(this);
}

ClythV1Parser::PostfixContext* ClythV1Parser::postfix() {
  PostfixContext *_localctx = _tracker.createInstance<PostfixContext>(_ctx, getState());
  enterRule(_localctx, 96, ClythV1Parser::RulePostfix);

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
    setState(512);
    primary();
    setState(516);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 56, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(513);
        postfixSuffix(); 
      }
      setState(518);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 56, _ctx);
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

tree::TerminalNode* ClythV1Parser::PostfixSuffixContext::LBRACKET() {
  return getToken(ClythV1Parser::LBRACKET, 0);
}

ClythV1Parser::ExpressionContext* ClythV1Parser::PostfixSuffixContext::expression() {
  return getRuleContext<ClythV1Parser::ExpressionContext>(0);
}

tree::TerminalNode* ClythV1Parser::PostfixSuffixContext::RBRACKET() {
  return getToken(ClythV1Parser::RBRACKET, 0);
}

tree::TerminalNode* ClythV1Parser::PostfixSuffixContext::LPAREN() {
  return getToken(ClythV1Parser::LPAREN, 0);
}

tree::TerminalNode* ClythV1Parser::PostfixSuffixContext::RPAREN() {
  return getToken(ClythV1Parser::RPAREN, 0);
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


std::any ClythV1Parser::PostfixSuffixContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ClythV1Visitor*>(visitor))
    return parserVisitor->visitPostfixSuffix(this);
  else
    return visitor->visitChildren(this);
}

ClythV1Parser::PostfixSuffixContext* ClythV1Parser::postfixSuffix() {
  PostfixSuffixContext *_localctx = _tracker.createInstance<PostfixSuffixContext>(_ctx, getState());
  enterRule(_localctx, 98, ClythV1Parser::RulePostfixSuffix);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(532);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case ClythV1Parser::DOT: {
        enterOuterAlt(_localctx, 1);
        setState(519);
        match(ClythV1Parser::DOT);
        setState(520);
        match(ClythV1Parser::IDENTIFIER);
        break;
      }

      case ClythV1Parser::LBRACKET: {
        enterOuterAlt(_localctx, 2);
        setState(521);
        match(ClythV1Parser::LBRACKET);
        setState(522);
        expression();
        setState(523);
        match(ClythV1Parser::RBRACKET);
        break;
      }

      case ClythV1Parser::LPAREN: {
        enterOuterAlt(_localctx, 3);
        setState(525);
        match(ClythV1Parser::LPAREN);
        setState(527);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if ((((_la & ~ 0x3fULL) == 0) &&
          ((1ULL << _la) & 4387068987104035008) != 0)) {
          setState(526);
          argumentList();
        }
        setState(529);
        match(ClythV1Parser::RPAREN);
        break;
      }

      case ClythV1Parser::INCREMENT: {
        enterOuterAlt(_localctx, 4);
        setState(530);
        match(ClythV1Parser::INCREMENT);
        break;
      }

      case ClythV1Parser::DECREMENT: {
        enterOuterAlt(_localctx, 5);
        setState(531);
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

ClythV1Parser::AllocationExpressionContext* ClythV1Parser::PrimaryContext::allocationExpression() {
  return getRuleContext<ClythV1Parser::AllocationExpressionContext>(0);
}

tree::TerminalNode* ClythV1Parser::PrimaryContext::IDENTIFIER() {
  return getToken(ClythV1Parser::IDENTIFIER, 0);
}

tree::TerminalNode* ClythV1Parser::PrimaryContext::LPAREN() {
  return getToken(ClythV1Parser::LPAREN, 0);
}

ClythV1Parser::ExpressionContext* ClythV1Parser::PrimaryContext::expression() {
  return getRuleContext<ClythV1Parser::ExpressionContext>(0);
}

tree::TerminalNode* ClythV1Parser::PrimaryContext::RPAREN() {
  return getToken(ClythV1Parser::RPAREN, 0);
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


std::any ClythV1Parser::PrimaryContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ClythV1Visitor*>(visitor))
    return parserVisitor->visitPrimary(this);
  else
    return visitor->visitChildren(this);
}

ClythV1Parser::PrimaryContext* ClythV1Parser::primary() {
  PrimaryContext *_localctx = _tracker.createInstance<PrimaryContext>(_ctx, getState());
  enterRule(_localctx, 100, ClythV1Parser::RulePrimary);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(542);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case ClythV1Parser::BOOLEAN_LITERAL:
      case ClythV1Parser::NULL_LITERAL:
      case ClythV1Parser::NUMERIC_LITERAL:
      case ClythV1Parser::TEMPLATE_STRING_LITERAL:
      case ClythV1Parser::STRING_LITERAL: {
        enterOuterAlt(_localctx, 1);
        setState(534);
        literal();
        break;
      }

      case ClythV1Parser::LBRACE:
      case ClythV1Parser::LBRACKET: {
        enterOuterAlt(_localctx, 2);
        setState(535);
        collectionLiteral();
        break;
      }

      case ClythV1Parser::MALLOC:
      case ClythV1Parser::ISO_MALLOC: {
        enterOuterAlt(_localctx, 3);
        setState(536);
        allocationExpression();
        break;
      }

      case ClythV1Parser::IDENTIFIER: {
        enterOuterAlt(_localctx, 4);
        setState(537);
        match(ClythV1Parser::IDENTIFIER);
        break;
      }

      case ClythV1Parser::LPAREN: {
        enterOuterAlt(_localctx, 5);
        setState(538);
        match(ClythV1Parser::LPAREN);
        setState(539);
        expression();
        setState(540);
        match(ClythV1Parser::RPAREN);
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

//----------------- AllocationExpressionContext ------------------------------------------------------------------

ClythV1Parser::AllocationExpressionContext::AllocationExpressionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ClythV1Parser::AllocationExpressionContext::LPAREN() {
  return getToken(ClythV1Parser::LPAREN, 0);
}

ClythV1Parser::TypeContext* ClythV1Parser::AllocationExpressionContext::type() {
  return getRuleContext<ClythV1Parser::TypeContext>(0);
}

tree::TerminalNode* ClythV1Parser::AllocationExpressionContext::RPAREN() {
  return getToken(ClythV1Parser::RPAREN, 0);
}

tree::TerminalNode* ClythV1Parser::AllocationExpressionContext::MALLOC() {
  return getToken(ClythV1Parser::MALLOC, 0);
}

tree::TerminalNode* ClythV1Parser::AllocationExpressionContext::ISO_MALLOC() {
  return getToken(ClythV1Parser::ISO_MALLOC, 0);
}


size_t ClythV1Parser::AllocationExpressionContext::getRuleIndex() const {
  return ClythV1Parser::RuleAllocationExpression;
}

void ClythV1Parser::AllocationExpressionContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterAllocationExpression(this);
}

void ClythV1Parser::AllocationExpressionContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitAllocationExpression(this);
}


std::any ClythV1Parser::AllocationExpressionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ClythV1Visitor*>(visitor))
    return parserVisitor->visitAllocationExpression(this);
  else
    return visitor->visitChildren(this);
}

ClythV1Parser::AllocationExpressionContext* ClythV1Parser::allocationExpression() {
  AllocationExpressionContext *_localctx = _tracker.createInstance<AllocationExpressionContext>(_ctx, getState());
  enterRule(_localctx, 102, ClythV1Parser::RuleAllocationExpression);
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
    setState(544);
    _la = _input->LA(1);
    if (!(_la == ClythV1Parser::MALLOC

    || _la == ClythV1Parser::ISO_MALLOC)) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
    setState(545);
    match(ClythV1Parser::LPAREN);
    setState(546);
    type();
    setState(547);
    match(ClythV1Parser::RPAREN);
   
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

std::vector<tree::TerminalNode *> ClythV1Parser::ArgumentListContext::COMMA() {
  return getTokens(ClythV1Parser::COMMA);
}

tree::TerminalNode* ClythV1Parser::ArgumentListContext::COMMA(size_t i) {
  return getToken(ClythV1Parser::COMMA, i);
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


std::any ClythV1Parser::ArgumentListContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ClythV1Visitor*>(visitor))
    return parserVisitor->visitArgumentList(this);
  else
    return visitor->visitChildren(this);
}

ClythV1Parser::ArgumentListContext* ClythV1Parser::argumentList() {
  ArgumentListContext *_localctx = _tracker.createInstance<ArgumentListContext>(_ctx, getState());
  enterRule(_localctx, 104, ClythV1Parser::RuleArgumentList);
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
    setState(549);
    expression();
    setState(554);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 60, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(550);
        match(ClythV1Parser::COMMA);
        setState(551);
        expression(); 
      }
      setState(556);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 60, _ctx);
    }
    setState(558);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ClythV1Parser::COMMA) {
      setState(557);
      match(ClythV1Parser::COMMA);
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


std::any ClythV1Parser::LiteralContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ClythV1Visitor*>(visitor))
    return parserVisitor->visitLiteral(this);
  else
    return visitor->visitChildren(this);
}

ClythV1Parser::LiteralContext* ClythV1Parser::literal() {
  LiteralContext *_localctx = _tracker.createInstance<LiteralContext>(_ctx, getState());
  enterRule(_localctx, 106, ClythV1Parser::RuleLiteral);
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
    setState(560);
    _la = _input->LA(1);
    if (!((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 2017612633065127936) != 0))) {
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

ClythV1Parser::MapLiteralContext* ClythV1Parser::CollectionLiteralContext::mapLiteral() {
  return getRuleContext<ClythV1Parser::MapLiteralContext>(0);
}

ClythV1Parser::SetLiteralContext* ClythV1Parser::CollectionLiteralContext::setLiteral() {
  return getRuleContext<ClythV1Parser::SetLiteralContext>(0);
}

ClythV1Parser::ListLiteralContext* ClythV1Parser::CollectionLiteralContext::listLiteral() {
  return getRuleContext<ClythV1Parser::ListLiteralContext>(0);
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


std::any ClythV1Parser::CollectionLiteralContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ClythV1Visitor*>(visitor))
    return parserVisitor->visitCollectionLiteral(this);
  else
    return visitor->visitChildren(this);
}

ClythV1Parser::CollectionLiteralContext* ClythV1Parser::collectionLiteral() {
  CollectionLiteralContext *_localctx = _tracker.createInstance<CollectionLiteralContext>(_ctx, getState());
  enterRule(_localctx, 108, ClythV1Parser::RuleCollectionLiteral);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(565);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 62, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(562);
      mapLiteral();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(563);
      setLiteral();
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(564);
      listLiteral();
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

tree::TerminalNode* ClythV1Parser::ListLiteralContext::LBRACKET() {
  return getToken(ClythV1Parser::LBRACKET, 0);
}

tree::TerminalNode* ClythV1Parser::ListLiteralContext::RBRACKET() {
  return getToken(ClythV1Parser::RBRACKET, 0);
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


std::any ClythV1Parser::ListLiteralContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ClythV1Visitor*>(visitor))
    return parserVisitor->visitListLiteral(this);
  else
    return visitor->visitChildren(this);
}

ClythV1Parser::ListLiteralContext* ClythV1Parser::listLiteral() {
  ListLiteralContext *_localctx = _tracker.createInstance<ListLiteralContext>(_ctx, getState());
  enterRule(_localctx, 110, ClythV1Parser::RuleListLiteral);
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
    setState(567);
    match(ClythV1Parser::LBRACKET);
    setState(569);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 4387068987104035008) != 0)) {
      setState(568);
      expressionList();
    }
    setState(571);
    match(ClythV1Parser::RBRACKET);
   
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

tree::TerminalNode* ClythV1Parser::MapLiteralContext::LBRACE() {
  return getToken(ClythV1Parser::LBRACE, 0);
}

tree::TerminalNode* ClythV1Parser::MapLiteralContext::RBRACE() {
  return getToken(ClythV1Parser::RBRACE, 0);
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


std::any ClythV1Parser::MapLiteralContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ClythV1Visitor*>(visitor))
    return parserVisitor->visitMapLiteral(this);
  else
    return visitor->visitChildren(this);
}

ClythV1Parser::MapLiteralContext* ClythV1Parser::mapLiteral() {
  MapLiteralContext *_localctx = _tracker.createInstance<MapLiteralContext>(_ctx, getState());
  enterRule(_localctx, 112, ClythV1Parser::RuleMapLiteral);
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
    match(ClythV1Parser::LBRACE);
    setState(575);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 4387068987104035008) != 0)) {
      setState(574);
      mapEntryList();
    }
    setState(577);
    match(ClythV1Parser::RBRACE);
   
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

std::vector<tree::TerminalNode *> ClythV1Parser::MapEntryListContext::COMMA() {
  return getTokens(ClythV1Parser::COMMA);
}

tree::TerminalNode* ClythV1Parser::MapEntryListContext::COMMA(size_t i) {
  return getToken(ClythV1Parser::COMMA, i);
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


std::any ClythV1Parser::MapEntryListContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ClythV1Visitor*>(visitor))
    return parserVisitor->visitMapEntryList(this);
  else
    return visitor->visitChildren(this);
}

ClythV1Parser::MapEntryListContext* ClythV1Parser::mapEntryList() {
  MapEntryListContext *_localctx = _tracker.createInstance<MapEntryListContext>(_ctx, getState());
  enterRule(_localctx, 114, ClythV1Parser::RuleMapEntryList);
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
    setState(579);
    mapEntry();
    setState(584);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 65, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(580);
        match(ClythV1Parser::COMMA);
        setState(581);
        mapEntry(); 
      }
      setState(586);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 65, _ctx);
    }
    setState(588);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ClythV1Parser::COMMA) {
      setState(587);
      match(ClythV1Parser::COMMA);
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

tree::TerminalNode* ClythV1Parser::MapEntryContext::COLON() {
  return getToken(ClythV1Parser::COLON, 0);
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


std::any ClythV1Parser::MapEntryContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ClythV1Visitor*>(visitor))
    return parserVisitor->visitMapEntry(this);
  else
    return visitor->visitChildren(this);
}

ClythV1Parser::MapEntryContext* ClythV1Parser::mapEntry() {
  MapEntryContext *_localctx = _tracker.createInstance<MapEntryContext>(_ctx, getState());
  enterRule(_localctx, 116, ClythV1Parser::RuleMapEntry);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(590);
    expression();
    setState(591);
    match(ClythV1Parser::COLON);
    setState(592);
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

tree::TerminalNode* ClythV1Parser::SetLiteralContext::LBRACE() {
  return getToken(ClythV1Parser::LBRACE, 0);
}

ClythV1Parser::ExpressionListContext* ClythV1Parser::SetLiteralContext::expressionList() {
  return getRuleContext<ClythV1Parser::ExpressionListContext>(0);
}

tree::TerminalNode* ClythV1Parser::SetLiteralContext::RBRACE() {
  return getToken(ClythV1Parser::RBRACE, 0);
}

tree::TerminalNode* ClythV1Parser::SetLiteralContext::COMMA() {
  return getToken(ClythV1Parser::COMMA, 0);
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


std::any ClythV1Parser::SetLiteralContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ClythV1Visitor*>(visitor))
    return parserVisitor->visitSetLiteral(this);
  else
    return visitor->visitChildren(this);
}

ClythV1Parser::SetLiteralContext* ClythV1Parser::setLiteral() {
  SetLiteralContext *_localctx = _tracker.createInstance<SetLiteralContext>(_ctx, getState());
  enterRule(_localctx, 118, ClythV1Parser::RuleSetLiteral);
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
    setState(594);
    match(ClythV1Parser::LBRACE);
    setState(595);
    expressionList();
    setState(597);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ClythV1Parser::COMMA) {
      setState(596);
      match(ClythV1Parser::COMMA);
    }
    setState(599);
    match(ClythV1Parser::RBRACE);
   
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

std::vector<tree::TerminalNode *> ClythV1Parser::ExpressionListContext::COMMA() {
  return getTokens(ClythV1Parser::COMMA);
}

tree::TerminalNode* ClythV1Parser::ExpressionListContext::COMMA(size_t i) {
  return getToken(ClythV1Parser::COMMA, i);
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


std::any ClythV1Parser::ExpressionListContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ClythV1Visitor*>(visitor))
    return parserVisitor->visitExpressionList(this);
  else
    return visitor->visitChildren(this);
}

ClythV1Parser::ExpressionListContext* ClythV1Parser::expressionList() {
  ExpressionListContext *_localctx = _tracker.createInstance<ExpressionListContext>(_ctx, getState());
  enterRule(_localctx, 120, ClythV1Parser::RuleExpressionList);

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
    setState(601);
    expression();
    setState(606);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 68, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(602);
        match(ClythV1Parser::COMMA);
        setState(603);
        expression(); 
      }
      setState(608);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 68, _ctx);
    }
    setState(610);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 69, _ctx)) {
    case 1: {
      setState(609);
      match(ClythV1Parser::COMMA);
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


std::any ClythV1Parser::TypeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ClythV1Visitor*>(visitor))
    return parserVisitor->visitType(this);
  else
    return visitor->visitChildren(this);
}

ClythV1Parser::TypeContext* ClythV1Parser::type() {
  TypeContext *_localctx = _tracker.createInstance<TypeContext>(_ctx, getState());
  enterRule(_localctx, 122, ClythV1Parser::RuleType);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(614);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 70, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(612);
      mapType();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(613);
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

tree::TerminalNode* ClythV1Parser::MapTypeContext::COLON() {
  return getToken(ClythV1Parser::COLON, 0);
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


std::any ClythV1Parser::MapTypeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ClythV1Visitor*>(visitor))
    return parserVisitor->visitMapType(this);
  else
    return visitor->visitChildren(this);
}

ClythV1Parser::MapTypeContext* ClythV1Parser::mapType() {
  MapTypeContext *_localctx = _tracker.createInstance<MapTypeContext>(_ctx, getState());
  enterRule(_localctx, 124, ClythV1Parser::RuleMapType);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(616);
    collectionType();
    setState(617);
    match(ClythV1Parser::COLON);
    setState(618);
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


std::any ClythV1Parser::CollectionTypeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ClythV1Visitor*>(visitor))
    return parserVisitor->visitCollectionType(this);
  else
    return visitor->visitChildren(this);
}

ClythV1Parser::CollectionTypeContext* ClythV1Parser::collectionType() {
  CollectionTypeContext *_localctx = _tracker.createInstance<CollectionTypeContext>(_ctx, getState());
  enterRule(_localctx, 126, ClythV1Parser::RuleCollectionType);
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
    setState(620);
    baseType();
    setState(624);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == ClythV1Parser::LPAREN

    || _la == ClythV1Parser::LBRACKET) {
      setState(621);
      typeSuffix();
      setState(626);
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


std::any ClythV1Parser::BaseTypeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ClythV1Visitor*>(visitor))
    return parserVisitor->visitBaseType(this);
  else
    return visitor->visitChildren(this);
}

ClythV1Parser::BaseTypeContext* ClythV1Parser::baseType() {
  BaseTypeContext *_localctx = _tracker.createInstance<BaseTypeContext>(_ctx, getState());
  enterRule(_localctx, 128, ClythV1Parser::RuleBaseType);
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
    setState(627);
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

tree::TerminalNode* ClythV1Parser::TypeSuffixContext::LBRACKET() {
  return getToken(ClythV1Parser::LBRACKET, 0);
}

tree::TerminalNode* ClythV1Parser::TypeSuffixContext::RBRACKET() {
  return getToken(ClythV1Parser::RBRACKET, 0);
}

tree::TerminalNode* ClythV1Parser::TypeSuffixContext::NUMERIC_LITERAL() {
  return getToken(ClythV1Parser::NUMERIC_LITERAL, 0);
}

tree::TerminalNode* ClythV1Parser::TypeSuffixContext::LPAREN() {
  return getToken(ClythV1Parser::LPAREN, 0);
}

tree::TerminalNode* ClythV1Parser::TypeSuffixContext::RPAREN() {
  return getToken(ClythV1Parser::RPAREN, 0);
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


std::any ClythV1Parser::TypeSuffixContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ClythV1Visitor*>(visitor))
    return parserVisitor->visitTypeSuffix(this);
  else
    return visitor->visitChildren(this);
}

ClythV1Parser::TypeSuffixContext* ClythV1Parser::typeSuffix() {
  TypeSuffixContext *_localctx = _tracker.createInstance<TypeSuffixContext>(_ctx, getState());
  enterRule(_localctx, 130, ClythV1Parser::RuleTypeSuffix);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(636);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 72, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(629);
      match(ClythV1Parser::LBRACKET);
      setState(630);
      match(ClythV1Parser::RBRACKET);
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(631);
      match(ClythV1Parser::LBRACKET);
      setState(632);
      match(ClythV1Parser::NUMERIC_LITERAL);
      setState(633);
      match(ClythV1Parser::RBRACKET);
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(634);
      match(ClythV1Parser::LPAREN);
      setState(635);
      match(ClythV1Parser::RPAREN);
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

tree::TerminalNode* ClythV1Parser::CommaOrSemiContext::COMMA() {
  return getToken(ClythV1Parser::COMMA, 0);
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


std::any ClythV1Parser::CommaOrSemiContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ClythV1Visitor*>(visitor))
    return parserVisitor->visitCommaOrSemi(this);
  else
    return visitor->visitChildren(this);
}

ClythV1Parser::CommaOrSemiContext* ClythV1Parser::commaOrSemi() {
  CommaOrSemiContext *_localctx = _tracker.createInstance<CommaOrSemiContext>(_ctx, getState());
  enterRule(_localctx, 132, ClythV1Parser::RuleCommaOrSemi);
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
    setState(638);
    _la = _input->LA(1);
    if (!(_la == ClythV1Parser::COMMA

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
