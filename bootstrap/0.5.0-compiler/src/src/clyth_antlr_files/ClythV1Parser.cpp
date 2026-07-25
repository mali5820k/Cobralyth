
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
      "externAbi", "externParamList", "externParam", "structDecl", "genericParamList", 
      "structField", "visibilityModifier", "methodBlock", "methodDecl", 
      "methodSimpleName", "functionDecl", "paramList", "param", "block", 
      "statement", "varDecl", "assignmentStmt", "assignable", "assignmentOp", 
      "exprStmt", "returnStmt", "ifStmt", "elseClause", "whileStmt", "forStmt", 
      "forEachHeader", "forEachBinding", "forEachVar", "forClassicHeader", 
      "forInit", "varDeclNoSemi", "assignmentNoSemi", "breakStmt", "continueStmt", 
      "meccBlock", "parenExpr", "expression", "logicalOr", "logicalAnd", 
      "equality", "relation", "comparison", "additive", "multiplicative", 
      "unary", "postfix", "postfixSuffix", "primary", "lambdaExpression", 
      "lambdaParamList", "lambdaParam", "argumentList", "literal", "collectionLiteral", 
      "listLiteral", "curlyLiteral", "curlyEntryList", "curlyEntry", "expressionList", 
      "type", "fixedArrayType", "dynamicArrayType", "functionType", "functionParamTypeList", 
      "genericType", "genericArgList", "typeAtom", "baseType", "commaOrSemi"
    },
    std::vector<std::string>{
      "", "'include'", "'extern'", "'C'", "'public'", "'private'", "'struct'", 
      "'function'", "'mecc'", "'constructor'", "'destructor'", "'if'", "'else'", 
      "'for'", "'while'", "'return'", "'break'", "'continue'", "'is'", "'in'", 
      "'not'", "", "'null'", "'('", "')'", "'{'", "'}'", "'['", "']'", "','", 
      "':'", "';'", "'.'", "'||'", "'&&'", "'=='", "'!='", "'>='", "'<='", 
      "'>'", "'<'", "'++'", "'--'", "'+='", "'-='", "'*='", "'/='", "'%='", 
      "'=>'", "'='", "'+'", "'-'", "'*'", "'/'", "'%'", "'!'", "'~'", "'&'", 
      "'...'"
    },
    std::vector<std::string>{
      "", "INCLUDE", "EXTERN", "C_ABI", "PUBLIC", "PRIVATE", "STRUCT", "FUNCTION", 
      "MECC", "CONSTRUCTOR", "DESTRUCTOR", "IF", "ELSE", "FOR", "WHILE", 
      "RETURN", "BREAK", "CONTINUE", "IS", "IN", "NOT", "BOOLEAN_LITERAL", 
      "NULL_LITERAL", "LPAREN", "RPAREN", "LBRACE", "RBRACE", "LBRACKET", 
      "RBRACKET", "COMMA", "COLON", "SEMI", "DOT", "OR", "AND", "EQ", "NEQ", 
      "GTE", "LTE", "GT", "LT", "INCREMENT", "DECREMENT", "PLUS_ASSIGN", 
      "MINUS_ASSIGN", "STAR_ASSIGN", "SLASH_ASSIGN", "PERCENT_ASSIGN", "ARROW", 
      "ASSIGN", "PLUS", "MINUS", "STAR", "SLASH", "PERCENT", "BANG", "TILDE", 
      "AMP", "VARARGS", "BUILTIN_TYPE", "NUMERIC_LITERAL", "TEMPLATE_STRING_LITERAL", 
      "STRING_LITERAL", "IDENTIFIER", "SINGLE_LINE_COMMENT", "MULTI_LINE_COMMENT", 
      "WS"
    }
  );
  static const int32_t serializedATNSegment[] = {
  	4,1,66,714,2,0,7,0,2,1,7,1,2,2,7,2,2,3,7,3,2,4,7,4,2,5,7,5,2,6,7,6,2,
  	7,7,7,2,8,7,8,2,9,7,9,2,10,7,10,2,11,7,11,2,12,7,12,2,13,7,13,2,14,7,
  	14,2,15,7,15,2,16,7,16,2,17,7,17,2,18,7,18,2,19,7,19,2,20,7,20,2,21,7,
  	21,2,22,7,22,2,23,7,23,2,24,7,24,2,25,7,25,2,26,7,26,2,27,7,27,2,28,7,
  	28,2,29,7,29,2,30,7,30,2,31,7,31,2,32,7,32,2,33,7,33,2,34,7,34,2,35,7,
  	35,2,36,7,36,2,37,7,37,2,38,7,38,2,39,7,39,2,40,7,40,2,41,7,41,2,42,7,
  	42,2,43,7,43,2,44,7,44,2,45,7,45,2,46,7,46,2,47,7,47,2,48,7,48,2,49,7,
  	49,2,50,7,50,2,51,7,51,2,52,7,52,2,53,7,53,2,54,7,54,2,55,7,55,2,56,7,
  	56,2,57,7,57,2,58,7,58,2,59,7,59,2,60,7,60,2,61,7,61,2,62,7,62,2,63,7,
  	63,2,64,7,64,2,65,7,65,2,66,7,66,2,67,7,67,2,68,7,68,2,69,7,69,2,70,7,
  	70,2,71,7,71,2,72,7,72,2,73,7,73,1,0,5,0,150,8,0,10,0,12,0,153,9,0,1,
  	0,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,3,1,164,8,1,1,2,1,2,1,2,3,2,169,8,2,
  	1,3,1,3,1,4,1,4,3,4,175,8,4,1,4,1,4,1,4,1,4,3,4,181,8,4,1,4,1,4,3,4,185,
  	8,4,1,5,1,5,1,6,1,6,1,6,5,6,192,8,6,10,6,12,6,195,9,6,1,6,3,6,198,8,6,
  	1,6,3,6,201,8,6,1,6,3,6,204,8,6,1,7,1,7,3,7,208,8,7,1,8,1,8,1,8,3,8,213,
  	8,8,1,8,1,8,5,8,217,8,8,10,8,12,8,220,9,8,1,8,1,8,3,8,224,8,8,1,9,1,9,
  	1,9,1,9,5,9,230,8,9,10,9,12,9,233,9,9,1,9,1,9,1,10,3,10,238,8,10,1,10,
  	1,10,1,10,3,10,243,8,10,1,11,1,11,1,12,1,12,1,12,5,12,250,8,12,10,12,
  	12,12,253,9,12,1,12,1,12,1,13,3,13,258,8,13,1,13,1,13,1,13,1,13,3,13,
  	264,8,13,1,13,1,13,1,13,1,13,3,13,270,8,13,1,13,1,13,1,13,1,13,1,13,1,
  	13,3,13,278,8,13,1,13,1,13,1,13,3,13,283,8,13,1,14,1,14,1,15,3,15,288,
  	8,15,1,15,1,15,1,15,1,15,3,15,294,8,15,1,15,1,15,1,15,1,16,1,16,1,16,
  	5,16,302,8,16,10,16,12,16,305,9,16,1,16,3,16,308,8,16,1,17,1,17,1,17,
  	1,18,1,18,5,18,315,8,18,10,18,12,18,318,9,18,1,18,1,18,1,19,1,19,1,19,
  	1,19,1,19,1,19,1,19,1,19,1,19,1,19,1,19,3,19,333,8,19,1,20,1,20,1,20,
  	1,20,3,20,339,8,20,1,20,3,20,342,8,20,1,21,1,21,1,21,1,21,3,21,348,8,
  	21,1,22,1,22,5,22,352,8,22,10,22,12,22,355,9,22,1,23,1,23,1,24,1,24,3,
  	24,361,8,24,1,25,1,25,3,25,365,8,25,1,25,3,25,368,8,25,1,26,1,26,1,26,
  	1,26,3,26,374,8,26,1,26,1,26,1,26,1,26,3,26,380,8,26,3,26,382,8,26,1,
  	27,1,27,1,27,1,27,3,27,388,8,27,1,28,1,28,1,28,1,28,1,28,1,28,1,28,1,
  	28,3,28,398,8,28,1,29,1,29,1,29,1,29,1,29,1,29,1,29,1,29,1,29,1,29,1,
  	29,1,29,1,29,1,29,1,29,1,29,1,29,1,29,1,29,1,29,1,29,1,29,3,29,422,8,
  	29,1,30,1,30,1,30,1,30,1,31,1,31,1,31,5,31,431,8,31,10,31,12,31,434,9,
  	31,1,32,1,32,1,32,1,32,3,32,440,8,32,1,33,3,33,443,8,33,1,33,1,33,3,33,
  	447,8,33,1,33,1,33,3,33,451,8,33,1,34,1,34,1,34,3,34,456,8,34,1,35,1,
  	35,1,35,1,35,3,35,462,8,35,1,36,1,36,1,36,1,36,1,37,1,37,3,37,470,8,37,
  	1,38,1,38,3,38,474,8,38,1,39,1,39,1,39,1,40,1,40,1,40,1,40,1,41,1,41,
  	1,42,1,42,1,42,5,42,488,8,42,10,42,12,42,491,9,42,1,43,1,43,1,43,5,43,
  	496,8,43,10,43,12,43,499,9,43,1,44,1,44,1,44,5,44,504,8,44,10,44,12,44,
  	507,9,44,1,45,1,45,1,45,5,45,512,8,45,10,45,12,45,515,9,45,1,46,1,46,
  	1,46,5,46,520,8,46,10,46,12,46,523,9,46,1,47,1,47,1,47,5,47,528,8,47,
  	10,47,12,47,531,9,47,1,48,1,48,1,48,5,48,536,8,48,10,48,12,48,539,9,48,
  	1,49,1,49,1,49,3,49,544,8,49,1,50,1,50,5,50,548,8,50,10,50,12,50,551,
  	9,50,1,51,1,51,1,51,1,51,1,51,1,51,1,51,1,51,3,51,561,8,51,1,51,1,51,
  	1,51,3,51,566,8,51,1,52,1,52,1,52,1,52,1,52,1,52,1,52,1,52,3,52,576,8,
  	52,1,53,1,53,3,53,580,8,53,1,53,1,53,1,53,1,53,1,54,1,54,1,54,5,54,589,
  	8,54,10,54,12,54,592,9,54,1,54,3,54,595,8,54,1,55,1,55,1,55,1,56,1,56,
  	1,56,5,56,603,8,56,10,56,12,56,606,9,56,1,56,3,56,609,8,56,1,57,1,57,
  	1,58,1,58,3,58,615,8,58,1,59,1,59,3,59,619,8,59,1,59,1,59,1,60,1,60,3,
  	60,625,8,60,1,60,1,60,1,61,1,61,1,61,5,61,632,8,61,10,61,12,61,635,9,
  	61,1,61,3,61,638,8,61,1,62,1,62,1,62,3,62,643,8,62,1,63,1,63,1,63,5,63,
  	648,8,63,10,63,12,63,651,9,63,1,63,3,63,654,8,63,1,64,1,64,1,64,1,64,
  	1,64,3,64,661,8,64,1,65,1,65,1,65,1,65,1,65,1,66,1,66,1,66,1,66,1,67,
  	1,67,1,67,1,67,1,67,3,67,677,8,67,1,67,1,67,1,67,1,68,1,68,1,68,5,68,
  	685,8,68,10,68,12,68,688,9,68,1,68,3,68,691,8,68,1,69,1,69,1,69,1,69,
  	1,69,1,70,1,70,1,70,5,70,701,8,70,10,70,12,70,704,9,70,1,71,1,71,3,71,
  	708,8,71,1,72,1,72,1,73,1,73,1,73,0,0,74,0,2,4,6,8,10,12,14,16,18,20,
  	22,24,26,28,30,32,34,36,38,40,42,44,46,48,50,52,54,56,58,60,62,64,66,
  	68,70,72,74,76,78,80,82,84,86,88,90,92,94,96,98,100,102,104,106,108,110,
  	112,114,116,118,120,122,124,126,128,130,132,134,136,138,140,142,144,146,
  	0,15,1,0,61,63,2,0,3,3,63,63,1,0,4,5,2,0,9,10,63,63,2,0,43,47,49,49,1,
  	0,35,36,1,0,18,19,1,0,37,40,1,0,50,51,1,0,52,54,3,0,20,20,51,51,55,57,
  	2,0,21,22,60,62,1,0,29,30,2,0,59,59,63,63,2,0,29,29,31,31,748,0,151,1,
  	0,0,0,2,163,1,0,0,0,4,165,1,0,0,0,6,170,1,0,0,0,8,172,1,0,0,0,10,186,
  	1,0,0,0,12,203,1,0,0,0,14,205,1,0,0,0,16,209,1,0,0,0,18,225,1,0,0,0,20,
  	237,1,0,0,0,22,244,1,0,0,0,24,246,1,0,0,0,26,282,1,0,0,0,28,284,1,0,0,
  	0,30,287,1,0,0,0,32,298,1,0,0,0,34,309,1,0,0,0,36,312,1,0,0,0,38,332,
  	1,0,0,0,40,334,1,0,0,0,42,343,1,0,0,0,44,349,1,0,0,0,46,356,1,0,0,0,48,
  	358,1,0,0,0,50,362,1,0,0,0,52,381,1,0,0,0,54,387,1,0,0,0,56,397,1,0,0,
  	0,58,421,1,0,0,0,60,423,1,0,0,0,62,427,1,0,0,0,64,439,1,0,0,0,66,442,
  	1,0,0,0,68,455,1,0,0,0,70,457,1,0,0,0,72,463,1,0,0,0,74,467,1,0,0,0,76,
  	471,1,0,0,0,78,475,1,0,0,0,80,478,1,0,0,0,82,482,1,0,0,0,84,484,1,0,0,
  	0,86,492,1,0,0,0,88,500,1,0,0,0,90,508,1,0,0,0,92,516,1,0,0,0,94,524,
  	1,0,0,0,96,532,1,0,0,0,98,543,1,0,0,0,100,545,1,0,0,0,102,565,1,0,0,0,
  	104,575,1,0,0,0,106,577,1,0,0,0,108,585,1,0,0,0,110,596,1,0,0,0,112,599,
  	1,0,0,0,114,610,1,0,0,0,116,614,1,0,0,0,118,616,1,0,0,0,120,622,1,0,0,
  	0,122,628,1,0,0,0,124,639,1,0,0,0,126,644,1,0,0,0,128,660,1,0,0,0,130,
  	662,1,0,0,0,132,667,1,0,0,0,134,671,1,0,0,0,136,681,1,0,0,0,138,692,1,
  	0,0,0,140,697,1,0,0,0,142,707,1,0,0,0,144,709,1,0,0,0,146,711,1,0,0,0,
  	148,150,3,2,1,0,149,148,1,0,0,0,150,153,1,0,0,0,151,149,1,0,0,0,151,152,
  	1,0,0,0,152,154,1,0,0,0,153,151,1,0,0,0,154,155,5,0,0,1,155,1,1,0,0,0,
  	156,164,3,4,2,0,157,164,3,8,4,0,158,164,3,16,8,0,159,164,3,24,12,0,160,
  	164,3,30,15,0,161,164,3,26,13,0,162,164,3,38,19,0,163,156,1,0,0,0,163,
  	157,1,0,0,0,163,158,1,0,0,0,163,159,1,0,0,0,163,160,1,0,0,0,163,161,1,
  	0,0,0,163,162,1,0,0,0,164,3,1,0,0,0,165,166,5,1,0,0,166,168,3,6,3,0,167,
  	169,5,31,0,0,168,167,1,0,0,0,168,169,1,0,0,0,169,5,1,0,0,0,170,171,7,
  	0,0,0,171,7,1,0,0,0,172,174,5,2,0,0,173,175,3,10,5,0,174,173,1,0,0,0,
  	174,175,1,0,0,0,175,176,1,0,0,0,176,177,3,128,64,0,177,178,5,63,0,0,178,
  	180,5,23,0,0,179,181,3,12,6,0,180,179,1,0,0,0,180,181,1,0,0,0,181,182,
  	1,0,0,0,182,184,5,24,0,0,183,185,5,31,0,0,184,183,1,0,0,0,184,185,1,0,
  	0,0,185,9,1,0,0,0,186,187,7,1,0,0,187,11,1,0,0,0,188,193,3,14,7,0,189,
  	190,5,29,0,0,190,192,3,14,7,0,191,189,1,0,0,0,192,195,1,0,0,0,193,191,
  	1,0,0,0,193,194,1,0,0,0,194,197,1,0,0,0,195,193,1,0,0,0,196,198,5,29,
  	0,0,197,196,1,0,0,0,197,198,1,0,0,0,198,200,1,0,0,0,199,201,5,58,0,0,
  	200,199,1,0,0,0,200,201,1,0,0,0,201,204,1,0,0,0,202,204,5,58,0,0,203,
  	188,1,0,0,0,203,202,1,0,0,0,204,13,1,0,0,0,205,207,3,128,64,0,206,208,
  	5,63,0,0,207,206,1,0,0,0,207,208,1,0,0,0,208,15,1,0,0,0,209,210,5,6,0,
  	0,210,212,5,63,0,0,211,213,3,18,9,0,212,211,1,0,0,0,212,213,1,0,0,0,213,
  	214,1,0,0,0,214,218,5,25,0,0,215,217,3,20,10,0,216,215,1,0,0,0,217,220,
  	1,0,0,0,218,216,1,0,0,0,218,219,1,0,0,0,219,221,1,0,0,0,220,218,1,0,0,
  	0,221,223,5,26,0,0,222,224,5,31,0,0,223,222,1,0,0,0,223,224,1,0,0,0,224,
  	17,1,0,0,0,225,226,5,40,0,0,226,231,5,63,0,0,227,228,5,29,0,0,228,230,
  	5,63,0,0,229,227,1,0,0,0,230,233,1,0,0,0,231,229,1,0,0,0,231,232,1,0,
  	0,0,232,234,1,0,0,0,233,231,1,0,0,0,234,235,5,39,0,0,235,19,1,0,0,0,236,
  	238,3,22,11,0,237,236,1,0,0,0,237,238,1,0,0,0,238,239,1,0,0,0,239,240,
  	3,128,64,0,240,242,5,63,0,0,241,243,3,146,73,0,242,241,1,0,0,0,242,243,
  	1,0,0,0,243,21,1,0,0,0,244,245,7,2,0,0,245,23,1,0,0,0,246,247,5,63,0,
  	0,247,251,5,25,0,0,248,250,3,26,13,0,249,248,1,0,0,0,250,253,1,0,0,0,
  	251,249,1,0,0,0,251,252,1,0,0,0,252,254,1,0,0,0,253,251,1,0,0,0,254,255,
  	5,26,0,0,255,25,1,0,0,0,256,258,5,8,0,0,257,256,1,0,0,0,257,258,1,0,0,
  	0,258,259,1,0,0,0,259,260,3,128,64,0,260,261,3,28,14,0,261,263,5,23,0,
  	0,262,264,3,32,16,0,263,262,1,0,0,0,263,264,1,0,0,0,264,265,1,0,0,0,265,
  	266,5,24,0,0,266,267,3,36,18,0,267,283,1,0,0,0,268,270,5,8,0,0,269,268,
  	1,0,0,0,269,270,1,0,0,0,270,271,1,0,0,0,271,272,3,128,64,0,272,273,5,
  	63,0,0,273,274,5,32,0,0,274,275,3,28,14,0,275,277,5,23,0,0,276,278,3,
  	32,16,0,277,276,1,0,0,0,277,278,1,0,0,0,278,279,1,0,0,0,279,280,5,24,
  	0,0,280,281,3,36,18,0,281,283,1,0,0,0,282,257,1,0,0,0,282,269,1,0,0,0,
  	283,27,1,0,0,0,284,285,7,3,0,0,285,29,1,0,0,0,286,288,5,8,0,0,287,286,
  	1,0,0,0,287,288,1,0,0,0,288,289,1,0,0,0,289,290,3,128,64,0,290,291,5,
  	63,0,0,291,293,5,23,0,0,292,294,3,32,16,0,293,292,1,0,0,0,293,294,1,0,
  	0,0,294,295,1,0,0,0,295,296,5,24,0,0,296,297,3,36,18,0,297,31,1,0,0,0,
  	298,303,3,34,17,0,299,300,5,29,0,0,300,302,3,34,17,0,301,299,1,0,0,0,
  	302,305,1,0,0,0,303,301,1,0,0,0,303,304,1,0,0,0,304,307,1,0,0,0,305,303,
  	1,0,0,0,306,308,5,29,0,0,307,306,1,0,0,0,307,308,1,0,0,0,308,33,1,0,0,
  	0,309,310,3,128,64,0,310,311,5,63,0,0,311,35,1,0,0,0,312,316,5,25,0,0,
  	313,315,3,38,19,0,314,313,1,0,0,0,315,318,1,0,0,0,316,314,1,0,0,0,316,
  	317,1,0,0,0,317,319,1,0,0,0,318,316,1,0,0,0,319,320,5,26,0,0,320,37,1,
  	0,0,0,321,333,3,40,20,0,322,333,3,42,21,0,323,333,3,48,24,0,324,333,3,
  	50,25,0,325,333,3,52,26,0,326,333,3,56,28,0,327,333,3,58,29,0,328,333,
  	3,74,37,0,329,333,3,76,38,0,330,333,3,78,39,0,331,333,3,36,18,0,332,321,
  	1,0,0,0,332,322,1,0,0,0,332,323,1,0,0,0,332,324,1,0,0,0,332,325,1,0,0,
  	0,332,326,1,0,0,0,332,327,1,0,0,0,332,328,1,0,0,0,332,329,1,0,0,0,332,
  	330,1,0,0,0,332,331,1,0,0,0,333,39,1,0,0,0,334,335,3,128,64,0,335,338,
  	5,63,0,0,336,337,5,49,0,0,337,339,3,82,41,0,338,336,1,0,0,0,338,339,1,
  	0,0,0,339,341,1,0,0,0,340,342,5,31,0,0,341,340,1,0,0,0,341,342,1,0,0,
  	0,342,41,1,0,0,0,343,344,3,44,22,0,344,345,3,46,23,0,345,347,3,82,41,
  	0,346,348,5,31,0,0,347,346,1,0,0,0,347,348,1,0,0,0,348,43,1,0,0,0,349,
  	353,5,63,0,0,350,352,3,102,51,0,351,350,1,0,0,0,352,355,1,0,0,0,353,351,
  	1,0,0,0,353,354,1,0,0,0,354,45,1,0,0,0,355,353,1,0,0,0,356,357,7,4,0,
  	0,357,47,1,0,0,0,358,360,3,82,41,0,359,361,5,31,0,0,360,359,1,0,0,0,360,
  	361,1,0,0,0,361,49,1,0,0,0,362,364,5,15,0,0,363,365,3,82,41,0,364,363,
  	1,0,0,0,364,365,1,0,0,0,365,367,1,0,0,0,366,368,5,31,0,0,367,366,1,0,
  	0,0,367,368,1,0,0,0,368,51,1,0,0,0,369,370,5,11,0,0,370,371,3,80,40,0,
  	371,373,3,36,18,0,372,374,3,54,27,0,373,372,1,0,0,0,373,374,1,0,0,0,374,
  	382,1,0,0,0,375,376,5,11,0,0,376,377,3,82,41,0,377,379,3,36,18,0,378,
  	380,3,54,27,0,379,378,1,0,0,0,379,380,1,0,0,0,380,382,1,0,0,0,381,369,
  	1,0,0,0,381,375,1,0,0,0,382,53,1,0,0,0,383,384,5,12,0,0,384,388,3,52,
  	26,0,385,386,5,12,0,0,386,388,3,36,18,0,387,383,1,0,0,0,387,385,1,0,0,
  	0,388,55,1,0,0,0,389,390,5,14,0,0,390,391,3,80,40,0,391,392,3,36,18,0,
  	392,398,1,0,0,0,393,394,5,14,0,0,394,395,3,82,41,0,395,396,3,36,18,0,
  	396,398,1,0,0,0,397,389,1,0,0,0,397,393,1,0,0,0,398,57,1,0,0,0,399,400,
  	5,13,0,0,400,422,3,36,18,0,401,402,5,13,0,0,402,403,3,60,30,0,403,404,
  	3,36,18,0,404,422,1,0,0,0,405,406,5,13,0,0,406,407,5,23,0,0,407,408,3,
  	60,30,0,408,409,5,24,0,0,409,410,3,36,18,0,410,422,1,0,0,0,411,412,5,
  	13,0,0,412,413,3,66,33,0,413,414,3,36,18,0,414,422,1,0,0,0,415,416,5,
  	13,0,0,416,417,5,23,0,0,417,418,3,66,33,0,418,419,5,24,0,0,419,420,3,
  	36,18,0,420,422,1,0,0,0,421,399,1,0,0,0,421,401,1,0,0,0,421,405,1,0,0,
  	0,421,411,1,0,0,0,421,415,1,0,0,0,422,59,1,0,0,0,423,424,3,62,31,0,424,
  	425,5,19,0,0,425,426,3,82,41,0,426,61,1,0,0,0,427,432,3,64,32,0,428,429,
  	5,29,0,0,429,431,3,64,32,0,430,428,1,0,0,0,431,434,1,0,0,0,432,430,1,
  	0,0,0,432,433,1,0,0,0,433,63,1,0,0,0,434,432,1,0,0,0,435,436,3,128,64,
  	0,436,437,5,63,0,0,437,440,1,0,0,0,438,440,5,63,0,0,439,435,1,0,0,0,439,
  	438,1,0,0,0,440,65,1,0,0,0,441,443,3,68,34,0,442,441,1,0,0,0,442,443,
  	1,0,0,0,443,444,1,0,0,0,444,446,5,31,0,0,445,447,3,82,41,0,446,445,1,
  	0,0,0,446,447,1,0,0,0,447,448,1,0,0,0,448,450,5,31,0,0,449,451,3,82,41,
  	0,450,449,1,0,0,0,450,451,1,0,0,0,451,67,1,0,0,0,452,456,3,70,35,0,453,
  	456,3,72,36,0,454,456,3,82,41,0,455,452,1,0,0,0,455,453,1,0,0,0,455,454,
  	1,0,0,0,456,69,1,0,0,0,457,458,3,128,64,0,458,461,5,63,0,0,459,460,5,
  	49,0,0,460,462,3,82,41,0,461,459,1,0,0,0,461,462,1,0,0,0,462,71,1,0,0,
  	0,463,464,3,44,22,0,464,465,3,46,23,0,465,466,3,82,41,0,466,73,1,0,0,
  	0,467,469,5,16,0,0,468,470,5,31,0,0,469,468,1,0,0,0,469,470,1,0,0,0,470,
  	75,1,0,0,0,471,473,5,17,0,0,472,474,5,31,0,0,473,472,1,0,0,0,473,474,
  	1,0,0,0,474,77,1,0,0,0,475,476,5,8,0,0,476,477,3,36,18,0,477,79,1,0,0,
  	0,478,479,5,23,0,0,479,480,3,82,41,0,480,481,5,24,0,0,481,81,1,0,0,0,
  	482,483,3,84,42,0,483,83,1,0,0,0,484,489,3,86,43,0,485,486,5,33,0,0,486,
  	488,3,86,43,0,487,485,1,0,0,0,488,491,1,0,0,0,489,487,1,0,0,0,489,490,
  	1,0,0,0,490,85,1,0,0,0,491,489,1,0,0,0,492,497,3,88,44,0,493,494,5,34,
  	0,0,494,496,3,88,44,0,495,493,1,0,0,0,496,499,1,0,0,0,497,495,1,0,0,0,
  	497,498,1,0,0,0,498,87,1,0,0,0,499,497,1,0,0,0,500,505,3,90,45,0,501,
  	502,7,5,0,0,502,504,3,90,45,0,503,501,1,0,0,0,504,507,1,0,0,0,505,503,
  	1,0,0,0,505,506,1,0,0,0,506,89,1,0,0,0,507,505,1,0,0,0,508,513,3,92,46,
  	0,509,510,7,6,0,0,510,512,3,92,46,0,511,509,1,0,0,0,512,515,1,0,0,0,513,
  	511,1,0,0,0,513,514,1,0,0,0,514,91,1,0,0,0,515,513,1,0,0,0,516,521,3,
  	94,47,0,517,518,7,7,0,0,518,520,3,94,47,0,519,517,1,0,0,0,520,523,1,0,
  	0,0,521,519,1,0,0,0,521,522,1,0,0,0,522,93,1,0,0,0,523,521,1,0,0,0,524,
  	529,3,96,48,0,525,526,7,8,0,0,526,528,3,96,48,0,527,525,1,0,0,0,528,531,
  	1,0,0,0,529,527,1,0,0,0,529,530,1,0,0,0,530,95,1,0,0,0,531,529,1,0,0,
  	0,532,537,3,98,49,0,533,534,7,9,0,0,534,536,3,98,49,0,535,533,1,0,0,0,
  	536,539,1,0,0,0,537,535,1,0,0,0,537,538,1,0,0,0,538,97,1,0,0,0,539,537,
  	1,0,0,0,540,541,7,10,0,0,541,544,3,98,49,0,542,544,3,100,50,0,543,540,
  	1,0,0,0,543,542,1,0,0,0,544,99,1,0,0,0,545,549,3,104,52,0,546,548,3,102,
  	51,0,547,546,1,0,0,0,548,551,1,0,0,0,549,547,1,0,0,0,549,550,1,0,0,0,
  	550,101,1,0,0,0,551,549,1,0,0,0,552,553,5,32,0,0,553,566,5,63,0,0,554,
  	555,5,27,0,0,555,556,3,82,41,0,556,557,5,28,0,0,557,566,1,0,0,0,558,560,
  	5,23,0,0,559,561,3,112,56,0,560,559,1,0,0,0,560,561,1,0,0,0,561,562,1,
  	0,0,0,562,566,5,24,0,0,563,566,5,41,0,0,564,566,5,42,0,0,565,552,1,0,
  	0,0,565,554,1,0,0,0,565,558,1,0,0,0,565,563,1,0,0,0,565,564,1,0,0,0,566,
  	103,1,0,0,0,567,576,3,114,57,0,568,576,3,116,58,0,569,576,3,106,53,0,
  	570,576,5,63,0,0,571,572,5,23,0,0,572,573,3,82,41,0,573,574,5,24,0,0,
  	574,576,1,0,0,0,575,567,1,0,0,0,575,568,1,0,0,0,575,569,1,0,0,0,575,570,
  	1,0,0,0,575,571,1,0,0,0,576,105,1,0,0,0,577,579,5,23,0,0,578,580,3,108,
  	54,0,579,578,1,0,0,0,579,580,1,0,0,0,580,581,1,0,0,0,581,582,5,24,0,0,
  	582,583,5,48,0,0,583,584,3,36,18,0,584,107,1,0,0,0,585,590,3,110,55,0,
  	586,587,5,29,0,0,587,589,3,110,55,0,588,586,1,0,0,0,589,592,1,0,0,0,590,
  	588,1,0,0,0,590,591,1,0,0,0,591,594,1,0,0,0,592,590,1,0,0,0,593,595,5,
  	29,0,0,594,593,1,0,0,0,594,595,1,0,0,0,595,109,1,0,0,0,596,597,3,128,
  	64,0,597,598,5,63,0,0,598,111,1,0,0,0,599,604,3,82,41,0,600,601,5,29,
  	0,0,601,603,3,82,41,0,602,600,1,0,0,0,603,606,1,0,0,0,604,602,1,0,0,0,
  	604,605,1,0,0,0,605,608,1,0,0,0,606,604,1,0,0,0,607,609,5,29,0,0,608,
  	607,1,0,0,0,608,609,1,0,0,0,609,113,1,0,0,0,610,611,7,11,0,0,611,115,
  	1,0,0,0,612,615,3,118,59,0,613,615,3,120,60,0,614,612,1,0,0,0,614,613,
  	1,0,0,0,615,117,1,0,0,0,616,618,5,27,0,0,617,619,3,126,63,0,618,617,1,
  	0,0,0,618,619,1,0,0,0,619,620,1,0,0,0,620,621,5,28,0,0,621,119,1,0,0,
  	0,622,624,5,25,0,0,623,625,3,122,61,0,624,623,1,0,0,0,624,625,1,0,0,0,
  	625,626,1,0,0,0,626,627,5,26,0,0,627,121,1,0,0,0,628,633,3,124,62,0,629,
  	630,5,29,0,0,630,632,3,124,62,0,631,629,1,0,0,0,632,635,1,0,0,0,633,631,
  	1,0,0,0,633,634,1,0,0,0,634,637,1,0,0,0,635,633,1,0,0,0,636,638,5,29,
  	0,0,637,636,1,0,0,0,637,638,1,0,0,0,638,123,1,0,0,0,639,642,3,82,41,0,
  	640,641,5,30,0,0,641,643,3,82,41,0,642,640,1,0,0,0,642,643,1,0,0,0,643,
  	125,1,0,0,0,644,649,3,82,41,0,645,646,5,29,0,0,646,648,3,82,41,0,647,
  	645,1,0,0,0,648,651,1,0,0,0,649,647,1,0,0,0,649,650,1,0,0,0,650,653,1,
  	0,0,0,651,649,1,0,0,0,652,654,5,29,0,0,653,652,1,0,0,0,653,654,1,0,0,
  	0,654,127,1,0,0,0,655,661,3,130,65,0,656,661,3,132,66,0,657,661,3,134,
  	67,0,658,661,3,138,69,0,659,661,3,144,72,0,660,655,1,0,0,0,660,656,1,
  	0,0,0,660,657,1,0,0,0,660,658,1,0,0,0,660,659,1,0,0,0,661,129,1,0,0,0,
  	662,663,3,142,71,0,663,664,5,27,0,0,664,665,5,60,0,0,665,666,5,28,0,0,
  	666,131,1,0,0,0,667,668,3,142,71,0,668,669,5,27,0,0,669,670,5,28,0,0,
  	670,133,1,0,0,0,671,672,5,7,0,0,672,673,5,40,0,0,673,674,3,128,64,0,674,
  	676,5,23,0,0,675,677,3,136,68,0,676,675,1,0,0,0,676,677,1,0,0,0,677,678,
  	1,0,0,0,678,679,5,24,0,0,679,680,5,39,0,0,680,135,1,0,0,0,681,686,3,128,
  	64,0,682,683,5,29,0,0,683,685,3,128,64,0,684,682,1,0,0,0,685,688,1,0,
  	0,0,686,684,1,0,0,0,686,687,1,0,0,0,687,690,1,0,0,0,688,686,1,0,0,0,689,
  	691,5,29,0,0,690,689,1,0,0,0,690,691,1,0,0,0,691,137,1,0,0,0,692,693,
  	5,63,0,0,693,694,5,40,0,0,694,695,3,140,70,0,695,696,5,39,0,0,696,139,
  	1,0,0,0,697,702,3,128,64,0,698,699,7,12,0,0,699,701,3,128,64,0,700,698,
  	1,0,0,0,701,704,1,0,0,0,702,700,1,0,0,0,702,703,1,0,0,0,703,141,1,0,0,
  	0,704,702,1,0,0,0,705,708,3,138,69,0,706,708,3,144,72,0,707,705,1,0,0,
  	0,707,706,1,0,0,0,708,143,1,0,0,0,709,710,7,13,0,0,710,145,1,0,0,0,711,
  	712,7,14,0,0,712,147,1,0,0,0,82,151,163,168,174,180,184,193,197,200,203,
  	207,212,218,223,231,237,242,251,257,263,269,277,282,287,293,303,307,316,
  	332,338,341,347,353,360,364,367,373,379,381,387,397,421,432,439,442,446,
  	450,455,461,469,473,489,497,505,513,521,529,537,543,549,560,565,575,579,
  	590,594,604,608,614,618,624,633,637,642,649,653,660,676,686,690,702,707
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
    setState(151);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & -322007373173233210) != 0)) {
      setState(148);
      topLevelItem();
      setState(153);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(154);
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
    setState(163);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 1, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(156);
      includeDecl();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(157);
      externDecl();
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(158);
      structDecl();
      break;
    }

    case 4: {
      enterOuterAlt(_localctx, 4);
      setState(159);
      methodBlock();
      break;
    }

    case 5: {
      enterOuterAlt(_localctx, 5);
      setState(160);
      functionDecl();
      break;
    }

    case 6: {
      enterOuterAlt(_localctx, 6);
      setState(161);
      methodDecl();
      break;
    }

    case 7: {
      enterOuterAlt(_localctx, 7);
      setState(162);
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
    setState(165);
    match(ClythV1Parser::INCLUDE);
    setState(166);
    includeTarget();
    setState(168);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ClythV1Parser::SEMI) {
      setState(167);
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
    setState(170);
    _la = _input->LA(1);
    if (!((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & -2305843009213693952) != 0))) {
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
    setState(172);
    match(ClythV1Parser::EXTERN);
    setState(174);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 3, _ctx)) {
    case 1: {
      setState(173);
      externAbi();
      break;
    }

    default:
      break;
    }
    setState(176);
    type();
    setState(177);
    match(ClythV1Parser::IDENTIFIER);
    setState(178);
    match(ClythV1Parser::LPAREN);
    setState(180);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & -8358680908399640448) != 0)) {
      setState(179);
      externParamList();
    }
    setState(182);
    match(ClythV1Parser::RPAREN);
    setState(184);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ClythV1Parser::SEMI) {
      setState(183);
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
    setState(186);
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
    setState(203);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case ClythV1Parser::FUNCTION:
      case ClythV1Parser::BUILTIN_TYPE:
      case ClythV1Parser::IDENTIFIER: {
        enterOuterAlt(_localctx, 1);
        setState(188);
        externParam();
        setState(193);
        _errHandler->sync(this);
        alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 6, _ctx);
        while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
          if (alt == 1) {
            setState(189);
            match(ClythV1Parser::COMMA);
            setState(190);
            externParam(); 
          }
          setState(195);
          _errHandler->sync(this);
          alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 6, _ctx);
        }
        setState(197);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == ClythV1Parser::COMMA) {
          setState(196);
          match(ClythV1Parser::COMMA);
        }
        setState(200);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == ClythV1Parser::VARARGS) {
          setState(199);
          match(ClythV1Parser::VARARGS);
        }
        break;
      }

      case ClythV1Parser::VARARGS: {
        enterOuterAlt(_localctx, 2);
        setState(202);
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
    setState(205);
    type();
    setState(207);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ClythV1Parser::IDENTIFIER) {
      setState(206);
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

ClythV1Parser::GenericParamListContext* ClythV1Parser::StructDeclContext::genericParamList() {
  return getRuleContext<ClythV1Parser::GenericParamListContext>(0);
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
    setState(209);
    match(ClythV1Parser::STRUCT);
    setState(210);
    match(ClythV1Parser::IDENTIFIER);
    setState(212);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ClythV1Parser::LT) {
      setState(211);
      genericParamList();
    }
    setState(214);
    match(ClythV1Parser::LBRACE);
    setState(218);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & -8646911284551352144) != 0)) {
      setState(215);
      structField();
      setState(220);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(221);
    match(ClythV1Parser::RBRACE);
    setState(223);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ClythV1Parser::SEMI) {
      setState(222);
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

//----------------- GenericParamListContext ------------------------------------------------------------------

ClythV1Parser::GenericParamListContext::GenericParamListContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ClythV1Parser::GenericParamListContext::LT() {
  return getToken(ClythV1Parser::LT, 0);
}

std::vector<tree::TerminalNode *> ClythV1Parser::GenericParamListContext::IDENTIFIER() {
  return getTokens(ClythV1Parser::IDENTIFIER);
}

tree::TerminalNode* ClythV1Parser::GenericParamListContext::IDENTIFIER(size_t i) {
  return getToken(ClythV1Parser::IDENTIFIER, i);
}

tree::TerminalNode* ClythV1Parser::GenericParamListContext::GT() {
  return getToken(ClythV1Parser::GT, 0);
}

std::vector<tree::TerminalNode *> ClythV1Parser::GenericParamListContext::COMMA() {
  return getTokens(ClythV1Parser::COMMA);
}

tree::TerminalNode* ClythV1Parser::GenericParamListContext::COMMA(size_t i) {
  return getToken(ClythV1Parser::COMMA, i);
}


size_t ClythV1Parser::GenericParamListContext::getRuleIndex() const {
  return ClythV1Parser::RuleGenericParamList;
}

void ClythV1Parser::GenericParamListContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterGenericParamList(this);
}

void ClythV1Parser::GenericParamListContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitGenericParamList(this);
}


std::any ClythV1Parser::GenericParamListContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ClythV1Visitor*>(visitor))
    return parserVisitor->visitGenericParamList(this);
  else
    return visitor->visitChildren(this);
}

ClythV1Parser::GenericParamListContext* ClythV1Parser::genericParamList() {
  GenericParamListContext *_localctx = _tracker.createInstance<GenericParamListContext>(_ctx, getState());
  enterRule(_localctx, 18, ClythV1Parser::RuleGenericParamList);
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
    setState(225);
    match(ClythV1Parser::LT);
    setState(226);
    match(ClythV1Parser::IDENTIFIER);
    setState(231);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == ClythV1Parser::COMMA) {
      setState(227);
      match(ClythV1Parser::COMMA);
      setState(228);
      match(ClythV1Parser::IDENTIFIER);
      setState(233);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(234);
    match(ClythV1Parser::GT);
   
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

ClythV1Parser::VisibilityModifierContext* ClythV1Parser::StructFieldContext::visibilityModifier() {
  return getRuleContext<ClythV1Parser::VisibilityModifierContext>(0);
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
  enterRule(_localctx, 20, ClythV1Parser::RuleStructField);
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
    setState(237);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ClythV1Parser::PUBLIC

    || _la == ClythV1Parser::PRIVATE) {
      setState(236);
      visibilityModifier();
    }
    setState(239);
    type();
    setState(240);
    match(ClythV1Parser::IDENTIFIER);
    setState(242);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ClythV1Parser::COMMA

    || _la == ClythV1Parser::SEMI) {
      setState(241);
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

//----------------- VisibilityModifierContext ------------------------------------------------------------------

ClythV1Parser::VisibilityModifierContext::VisibilityModifierContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ClythV1Parser::VisibilityModifierContext::PUBLIC() {
  return getToken(ClythV1Parser::PUBLIC, 0);
}

tree::TerminalNode* ClythV1Parser::VisibilityModifierContext::PRIVATE() {
  return getToken(ClythV1Parser::PRIVATE, 0);
}


size_t ClythV1Parser::VisibilityModifierContext::getRuleIndex() const {
  return ClythV1Parser::RuleVisibilityModifier;
}

void ClythV1Parser::VisibilityModifierContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterVisibilityModifier(this);
}

void ClythV1Parser::VisibilityModifierContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitVisibilityModifier(this);
}


std::any ClythV1Parser::VisibilityModifierContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ClythV1Visitor*>(visitor))
    return parserVisitor->visitVisibilityModifier(this);
  else
    return visitor->visitChildren(this);
}

ClythV1Parser::VisibilityModifierContext* ClythV1Parser::visibilityModifier() {
  VisibilityModifierContext *_localctx = _tracker.createInstance<VisibilityModifierContext>(_ctx, getState());
  enterRule(_localctx, 22, ClythV1Parser::RuleVisibilityModifier);
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
    setState(244);
    _la = _input->LA(1);
    if (!(_la == ClythV1Parser::PUBLIC

    || _la == ClythV1Parser::PRIVATE)) {
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
  enterRule(_localctx, 24, ClythV1Parser::RuleMethodBlock);
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
    setState(246);
    match(ClythV1Parser::IDENTIFIER);
    setState(247);
    match(ClythV1Parser::LBRACE);
    setState(251);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & -8646911284551351936) != 0)) {
      setState(248);
      methodDecl();
      setState(253);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(254);
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
  enterRule(_localctx, 26, ClythV1Parser::RuleMethodDecl);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(282);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 22, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(257);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == ClythV1Parser::MECC) {
        setState(256);
        match(ClythV1Parser::MECC);
      }
      setState(259);
      type();
      setState(260);
      methodSimpleName();
      setState(261);
      match(ClythV1Parser::LPAREN);
      setState(263);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if ((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & -8646911284551352192) != 0)) {
        setState(262);
        paramList();
      }
      setState(265);
      match(ClythV1Parser::RPAREN);
      setState(266);
      block();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(269);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == ClythV1Parser::MECC) {
        setState(268);
        match(ClythV1Parser::MECC);
      }
      setState(271);
      type();
      setState(272);
      match(ClythV1Parser::IDENTIFIER);
      setState(273);
      match(ClythV1Parser::DOT);
      setState(274);
      methodSimpleName();
      setState(275);
      match(ClythV1Parser::LPAREN);
      setState(277);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if ((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & -8646911284551352192) != 0)) {
        setState(276);
        paramList();
      }
      setState(279);
      match(ClythV1Parser::RPAREN);
      setState(280);
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
  enterRule(_localctx, 28, ClythV1Parser::RuleMethodSimpleName);
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
    setState(284);
    _la = _input->LA(1);
    if (!((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & -9223372036854774272) != 0))) {
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
  enterRule(_localctx, 30, ClythV1Parser::RuleFunctionDecl);
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
    setState(287);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ClythV1Parser::MECC) {
      setState(286);
      match(ClythV1Parser::MECC);
    }
    setState(289);
    type();
    setState(290);
    match(ClythV1Parser::IDENTIFIER);
    setState(291);
    match(ClythV1Parser::LPAREN);
    setState(293);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & -8646911284551352192) != 0)) {
      setState(292);
      paramList();
    }
    setState(295);
    match(ClythV1Parser::RPAREN);
    setState(296);
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
  enterRule(_localctx, 32, ClythV1Parser::RuleParamList);
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
    setState(298);
    param();
    setState(303);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 25, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(299);
        match(ClythV1Parser::COMMA);
        setState(300);
        param(); 
      }
      setState(305);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 25, _ctx);
    }
    setState(307);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ClythV1Parser::COMMA) {
      setState(306);
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
  enterRule(_localctx, 34, ClythV1Parser::RuleParam);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(309);
    type();
    setState(310);
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
  enterRule(_localctx, 36, ClythV1Parser::RuleBlock);
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
    setState(312);
    match(ClythV1Parser::LBRACE);
    setState(316);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & -322007373173233280) != 0)) {
      setState(313);
      statement();
      setState(318);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(319);
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
  enterRule(_localctx, 38, ClythV1Parser::RuleStatement);

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
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 28, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(321);
      varDecl();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(322);
      assignmentStmt();
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(323);
      exprStmt();
      break;
    }

    case 4: {
      enterOuterAlt(_localctx, 4);
      setState(324);
      returnStmt();
      break;
    }

    case 5: {
      enterOuterAlt(_localctx, 5);
      setState(325);
      ifStmt();
      break;
    }

    case 6: {
      enterOuterAlt(_localctx, 6);
      setState(326);
      whileStmt();
      break;
    }

    case 7: {
      enterOuterAlt(_localctx, 7);
      setState(327);
      forStmt();
      break;
    }

    case 8: {
      enterOuterAlt(_localctx, 8);
      setState(328);
      breakStmt();
      break;
    }

    case 9: {
      enterOuterAlt(_localctx, 9);
      setState(329);
      continueStmt();
      break;
    }

    case 10: {
      enterOuterAlt(_localctx, 10);
      setState(330);
      meccBlock();
      break;
    }

    case 11: {
      enterOuterAlt(_localctx, 11);
      setState(331);
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
  enterRule(_localctx, 40, ClythV1Parser::RuleVarDecl);
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
    setState(334);
    type();
    setState(335);
    match(ClythV1Parser::IDENTIFIER);
    setState(338);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ClythV1Parser::ASSIGN) {
      setState(336);
      match(ClythV1Parser::ASSIGN);
      setState(337);
      expression();
    }
    setState(341);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ClythV1Parser::SEMI) {
      setState(340);
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
  enterRule(_localctx, 42, ClythV1Parser::RuleAssignmentStmt);
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
    setState(343);
    assignable();
    setState(344);
    assignmentOp();
    setState(345);
    expression();
    setState(347);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ClythV1Parser::SEMI) {
      setState(346);
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
  enterRule(_localctx, 44, ClythV1Parser::RuleAssignable);
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
    setState(349);
    match(ClythV1Parser::IDENTIFIER);
    setState(353);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 6601507340288) != 0)) {
      setState(350);
      postfixSuffix();
      setState(355);
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
  enterRule(_localctx, 46, ClythV1Parser::RuleAssignmentOp);
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
    setState(356);
    _la = _input->LA(1);
    if (!((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 835628837109760) != 0))) {
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
  enterRule(_localctx, 48, ClythV1Parser::RuleExprStmt);
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
    setState(358);
    expression();
    setState(360);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ClythV1Parser::SEMI) {
      setState(359);
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
  enterRule(_localctx, 50, ClythV1Parser::RuleReturnStmt);
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
    setState(362);
    match(ClythV1Parser::RETURN);
    setState(364);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 34, _ctx)) {
    case 1: {
      setState(363);
      expression();
      break;
    }

    default:
      break;
    }
    setState(367);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ClythV1Parser::SEMI) {
      setState(366);
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
  enterRule(_localctx, 52, ClythV1Parser::RuleIfStmt);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(381);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 38, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(369);
      match(ClythV1Parser::IF);
      setState(370);
      parenExpr();
      setState(371);
      block();
      setState(373);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == ClythV1Parser::ELSE) {
        setState(372);
        elseClause();
      }
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(375);
      match(ClythV1Parser::IF);
      setState(376);
      expression();
      setState(377);
      block();
      setState(379);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == ClythV1Parser::ELSE) {
        setState(378);
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
  enterRule(_localctx, 54, ClythV1Parser::RuleElseClause);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(387);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 39, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(383);
      match(ClythV1Parser::ELSE);
      setState(384);
      ifStmt();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(385);
      match(ClythV1Parser::ELSE);
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
  enterRule(_localctx, 56, ClythV1Parser::RuleWhileStmt);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(397);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 40, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(389);
      match(ClythV1Parser::WHILE);
      setState(390);
      parenExpr();
      setState(391);
      block();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(393);
      match(ClythV1Parser::WHILE);
      setState(394);
      expression();
      setState(395);
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
  enterRule(_localctx, 58, ClythV1Parser::RuleForStmt);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(421);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 41, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(399);
      match(ClythV1Parser::FOR);
      setState(400);
      block();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(401);
      match(ClythV1Parser::FOR);
      setState(402);
      forEachHeader();
      setState(403);
      block();
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(405);
      match(ClythV1Parser::FOR);
      setState(406);
      match(ClythV1Parser::LPAREN);
      setState(407);
      forEachHeader();
      setState(408);
      match(ClythV1Parser::RPAREN);
      setState(409);
      block();
      break;
    }

    case 4: {
      enterOuterAlt(_localctx, 4);
      setState(411);
      match(ClythV1Parser::FOR);
      setState(412);
      forClassicHeader();
      setState(413);
      block();
      break;
    }

    case 5: {
      enterOuterAlt(_localctx, 5);
      setState(415);
      match(ClythV1Parser::FOR);
      setState(416);
      match(ClythV1Parser::LPAREN);
      setState(417);
      forClassicHeader();
      setState(418);
      match(ClythV1Parser::RPAREN);
      setState(419);
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
  enterRule(_localctx, 60, ClythV1Parser::RuleForEachHeader);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(423);
    forEachBinding();
    setState(424);
    match(ClythV1Parser::IN);
    setState(425);
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
  enterRule(_localctx, 62, ClythV1Parser::RuleForEachBinding);
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
    setState(427);
    forEachVar();
    setState(432);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == ClythV1Parser::COMMA) {
      setState(428);
      match(ClythV1Parser::COMMA);
      setState(429);
      forEachVar();
      setState(434);
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
  enterRule(_localctx, 64, ClythV1Parser::RuleForEachVar);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(439);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 43, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(435);
      type();
      setState(436);
      match(ClythV1Parser::IDENTIFIER);
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(438);
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
  enterRule(_localctx, 66, ClythV1Parser::RuleForClassicHeader);
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
    setState(442);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & -322007373173489536) != 0)) {
      setState(441);
      forInit();
    }
    setState(444);
    match(ClythV1Parser::SEMI);
    setState(446);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & -898468125476913152) != 0)) {
      setState(445);
      expression();
    }
    setState(448);
    match(ClythV1Parser::SEMI);
    setState(450);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 46, _ctx)) {
    case 1: {
      setState(449);
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
  enterRule(_localctx, 68, ClythV1Parser::RuleForInit);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(455);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 47, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(452);
      varDeclNoSemi();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(453);
      assignmentNoSemi();
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(454);
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
  enterRule(_localctx, 70, ClythV1Parser::RuleVarDeclNoSemi);
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
    setState(457);
    type();
    setState(458);
    match(ClythV1Parser::IDENTIFIER);
    setState(461);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ClythV1Parser::ASSIGN) {
      setState(459);
      match(ClythV1Parser::ASSIGN);
      setState(460);
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
  enterRule(_localctx, 72, ClythV1Parser::RuleAssignmentNoSemi);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(463);
    assignable();
    setState(464);
    assignmentOp();
    setState(465);
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
  enterRule(_localctx, 74, ClythV1Parser::RuleBreakStmt);
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
    match(ClythV1Parser::BREAK);
    setState(469);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ClythV1Parser::SEMI) {
      setState(468);
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
  enterRule(_localctx, 76, ClythV1Parser::RuleContinueStmt);
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
    setState(471);
    match(ClythV1Parser::CONTINUE);
    setState(473);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ClythV1Parser::SEMI) {
      setState(472);
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
  enterRule(_localctx, 78, ClythV1Parser::RuleMeccBlock);

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
    match(ClythV1Parser::MECC);
    setState(476);
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
  enterRule(_localctx, 80, ClythV1Parser::RuleParenExpr);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(478);
    match(ClythV1Parser::LPAREN);
    setState(479);
    expression();
    setState(480);
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
  enterRule(_localctx, 82, ClythV1Parser::RuleExpression);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(482);
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
  enterRule(_localctx, 84, ClythV1Parser::RuleLogicalOr);
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
    setState(484);
    logicalAnd();
    setState(489);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == ClythV1Parser::OR) {
      setState(485);
      match(ClythV1Parser::OR);
      setState(486);
      logicalAnd();
      setState(491);
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
  enterRule(_localctx, 86, ClythV1Parser::RuleLogicalAnd);
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
    setState(492);
    equality();
    setState(497);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == ClythV1Parser::AND) {
      setState(493);
      match(ClythV1Parser::AND);
      setState(494);
      equality();
      setState(499);
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
  enterRule(_localctx, 88, ClythV1Parser::RuleEquality);
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
    setState(500);
    relation();
    setState(505);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == ClythV1Parser::EQ

    || _la == ClythV1Parser::NEQ) {
      setState(501);
      _la = _input->LA(1);
      if (!(_la == ClythV1Parser::EQ

      || _la == ClythV1Parser::NEQ)) {
      _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(502);
      relation();
      setState(507);
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
  enterRule(_localctx, 90, ClythV1Parser::RuleRelation);
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
    comparison();
    setState(513);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == ClythV1Parser::IS

    || _la == ClythV1Parser::IN) {
      setState(509);
      _la = _input->LA(1);
      if (!(_la == ClythV1Parser::IS

      || _la == ClythV1Parser::IN)) {
      _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(510);
      comparison();
      setState(515);
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
  enterRule(_localctx, 92, ClythV1Parser::RuleComparison);
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
    setState(516);
    additive();
    setState(521);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 2061584302080) != 0)) {
      setState(517);
      _la = _input->LA(1);
      if (!((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & 2061584302080) != 0))) {
      _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(518);
      additive();
      setState(523);
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
  enterRule(_localctx, 94, ClythV1Parser::RuleAdditive);
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
    setState(524);
    multiplicative();
    setState(529);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 56, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(525);
        _la = _input->LA(1);
        if (!(_la == ClythV1Parser::PLUS

        || _la == ClythV1Parser::MINUS)) {
        _errHandler->recoverInline(this);
        }
        else {
          _errHandler->reportMatch(this);
          consume();
        }
        setState(526);
        multiplicative(); 
      }
      setState(531);
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
  enterRule(_localctx, 96, ClythV1Parser::RuleMultiplicative);
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
    setState(532);
    unary();
    setState(537);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 31525197391593472) != 0)) {
      setState(533);
      _la = _input->LA(1);
      if (!((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & 31525197391593472) != 0))) {
      _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(534);
      unary();
      setState(539);
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
  enterRule(_localctx, 98, ClythV1Parser::RuleUnary);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(543);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case ClythV1Parser::NOT:
      case ClythV1Parser::MINUS:
      case ClythV1Parser::BANG:
      case ClythV1Parser::TILDE:
      case ClythV1Parser::AMP: {
        enterOuterAlt(_localctx, 1);
        setState(540);
        _la = _input->LA(1);
        if (!((((_la & ~ 0x3fULL) == 0) &&
          ((1ULL << _la) & 254453378947481600) != 0))) {
        _errHandler->recoverInline(this);
        }
        else {
          _errHandler->reportMatch(this);
          consume();
        }
        setState(541);
        unary();
        break;
      }

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
        setState(542);
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
  enterRule(_localctx, 100, ClythV1Parser::RulePostfix);

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
    setState(545);
    primary();
    setState(549);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 59, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(546);
        postfixSuffix(); 
      }
      setState(551);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 59, _ctx);
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
  enterRule(_localctx, 102, ClythV1Parser::RulePostfixSuffix);
  size_t _la = 0;

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
    switch (_input->LA(1)) {
      case ClythV1Parser::DOT: {
        enterOuterAlt(_localctx, 1);
        setState(552);
        match(ClythV1Parser::DOT);
        setState(553);
        match(ClythV1Parser::IDENTIFIER);
        break;
      }

      case ClythV1Parser::LBRACKET: {
        enterOuterAlt(_localctx, 2);
        setState(554);
        match(ClythV1Parser::LBRACKET);
        setState(555);
        expression();
        setState(556);
        match(ClythV1Parser::RBRACKET);
        break;
      }

      case ClythV1Parser::LPAREN: {
        enterOuterAlt(_localctx, 3);
        setState(558);
        match(ClythV1Parser::LPAREN);
        setState(560);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if ((((_la & ~ 0x3fULL) == 0) &&
          ((1ULL << _la) & -898468125476913152) != 0)) {
          setState(559);
          argumentList();
        }
        setState(562);
        match(ClythV1Parser::RPAREN);
        break;
      }

      case ClythV1Parser::INCREMENT: {
        enterOuterAlt(_localctx, 4);
        setState(563);
        match(ClythV1Parser::INCREMENT);
        break;
      }

      case ClythV1Parser::DECREMENT: {
        enterOuterAlt(_localctx, 5);
        setState(564);
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

ClythV1Parser::LambdaExpressionContext* ClythV1Parser::PrimaryContext::lambdaExpression() {
  return getRuleContext<ClythV1Parser::LambdaExpressionContext>(0);
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
  enterRule(_localctx, 104, ClythV1Parser::RulePrimary);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(575);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 62, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(567);
      literal();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(568);
      collectionLiteral();
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(569);
      lambdaExpression();
      break;
    }

    case 4: {
      enterOuterAlt(_localctx, 4);
      setState(570);
      match(ClythV1Parser::IDENTIFIER);
      break;
    }

    case 5: {
      enterOuterAlt(_localctx, 5);
      setState(571);
      match(ClythV1Parser::LPAREN);
      setState(572);
      expression();
      setState(573);
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

//----------------- LambdaExpressionContext ------------------------------------------------------------------

ClythV1Parser::LambdaExpressionContext::LambdaExpressionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ClythV1Parser::LambdaExpressionContext::LPAREN() {
  return getToken(ClythV1Parser::LPAREN, 0);
}

tree::TerminalNode* ClythV1Parser::LambdaExpressionContext::RPAREN() {
  return getToken(ClythV1Parser::RPAREN, 0);
}

tree::TerminalNode* ClythV1Parser::LambdaExpressionContext::ARROW() {
  return getToken(ClythV1Parser::ARROW, 0);
}

ClythV1Parser::BlockContext* ClythV1Parser::LambdaExpressionContext::block() {
  return getRuleContext<ClythV1Parser::BlockContext>(0);
}

ClythV1Parser::LambdaParamListContext* ClythV1Parser::LambdaExpressionContext::lambdaParamList() {
  return getRuleContext<ClythV1Parser::LambdaParamListContext>(0);
}


size_t ClythV1Parser::LambdaExpressionContext::getRuleIndex() const {
  return ClythV1Parser::RuleLambdaExpression;
}

void ClythV1Parser::LambdaExpressionContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterLambdaExpression(this);
}

void ClythV1Parser::LambdaExpressionContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitLambdaExpression(this);
}


std::any ClythV1Parser::LambdaExpressionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ClythV1Visitor*>(visitor))
    return parserVisitor->visitLambdaExpression(this);
  else
    return visitor->visitChildren(this);
}

ClythV1Parser::LambdaExpressionContext* ClythV1Parser::lambdaExpression() {
  LambdaExpressionContext *_localctx = _tracker.createInstance<LambdaExpressionContext>(_ctx, getState());
  enterRule(_localctx, 106, ClythV1Parser::RuleLambdaExpression);
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
    setState(577);
    match(ClythV1Parser::LPAREN);
    setState(579);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & -8646911284551352192) != 0)) {
      setState(578);
      lambdaParamList();
    }
    setState(581);
    match(ClythV1Parser::RPAREN);
    setState(582);
    match(ClythV1Parser::ARROW);
    setState(583);
    block();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- LambdaParamListContext ------------------------------------------------------------------

ClythV1Parser::LambdaParamListContext::LambdaParamListContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<ClythV1Parser::LambdaParamContext *> ClythV1Parser::LambdaParamListContext::lambdaParam() {
  return getRuleContexts<ClythV1Parser::LambdaParamContext>();
}

ClythV1Parser::LambdaParamContext* ClythV1Parser::LambdaParamListContext::lambdaParam(size_t i) {
  return getRuleContext<ClythV1Parser::LambdaParamContext>(i);
}

std::vector<tree::TerminalNode *> ClythV1Parser::LambdaParamListContext::COMMA() {
  return getTokens(ClythV1Parser::COMMA);
}

tree::TerminalNode* ClythV1Parser::LambdaParamListContext::COMMA(size_t i) {
  return getToken(ClythV1Parser::COMMA, i);
}


size_t ClythV1Parser::LambdaParamListContext::getRuleIndex() const {
  return ClythV1Parser::RuleLambdaParamList;
}

void ClythV1Parser::LambdaParamListContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterLambdaParamList(this);
}

void ClythV1Parser::LambdaParamListContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitLambdaParamList(this);
}


std::any ClythV1Parser::LambdaParamListContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ClythV1Visitor*>(visitor))
    return parserVisitor->visitLambdaParamList(this);
  else
    return visitor->visitChildren(this);
}

ClythV1Parser::LambdaParamListContext* ClythV1Parser::lambdaParamList() {
  LambdaParamListContext *_localctx = _tracker.createInstance<LambdaParamListContext>(_ctx, getState());
  enterRule(_localctx, 108, ClythV1Parser::RuleLambdaParamList);
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
    setState(585);
    lambdaParam();
    setState(590);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 64, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(586);
        match(ClythV1Parser::COMMA);
        setState(587);
        lambdaParam(); 
      }
      setState(592);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 64, _ctx);
    }
    setState(594);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ClythV1Parser::COMMA) {
      setState(593);
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

//----------------- LambdaParamContext ------------------------------------------------------------------

ClythV1Parser::LambdaParamContext::LambdaParamContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ClythV1Parser::TypeContext* ClythV1Parser::LambdaParamContext::type() {
  return getRuleContext<ClythV1Parser::TypeContext>(0);
}

tree::TerminalNode* ClythV1Parser::LambdaParamContext::IDENTIFIER() {
  return getToken(ClythV1Parser::IDENTIFIER, 0);
}


size_t ClythV1Parser::LambdaParamContext::getRuleIndex() const {
  return ClythV1Parser::RuleLambdaParam;
}

void ClythV1Parser::LambdaParamContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterLambdaParam(this);
}

void ClythV1Parser::LambdaParamContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitLambdaParam(this);
}


std::any ClythV1Parser::LambdaParamContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ClythV1Visitor*>(visitor))
    return parserVisitor->visitLambdaParam(this);
  else
    return visitor->visitChildren(this);
}

ClythV1Parser::LambdaParamContext* ClythV1Parser::lambdaParam() {
  LambdaParamContext *_localctx = _tracker.createInstance<LambdaParamContext>(_ctx, getState());
  enterRule(_localctx, 110, ClythV1Parser::RuleLambdaParam);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(596);
    type();
    setState(597);
    match(ClythV1Parser::IDENTIFIER);
   
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
  enterRule(_localctx, 112, ClythV1Parser::RuleArgumentList);
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
    setState(599);
    expression();
    setState(604);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 66, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(600);
        match(ClythV1Parser::COMMA);
        setState(601);
        expression(); 
      }
      setState(606);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 66, _ctx);
    }
    setState(608);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ClythV1Parser::COMMA) {
      setState(607);
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
  enterRule(_localctx, 114, ClythV1Parser::RuleLiteral);
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
    setState(610);
    _la = _input->LA(1);
    if (!((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 8070450532254220288) != 0))) {
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

ClythV1Parser::CurlyLiteralContext* ClythV1Parser::CollectionLiteralContext::curlyLiteral() {
  return getRuleContext<ClythV1Parser::CurlyLiteralContext>(0);
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
  enterRule(_localctx, 116, ClythV1Parser::RuleCollectionLiteral);

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
    switch (_input->LA(1)) {
      case ClythV1Parser::LBRACKET: {
        enterOuterAlt(_localctx, 1);
        setState(612);
        listLiteral();
        break;
      }

      case ClythV1Parser::LBRACE: {
        enterOuterAlt(_localctx, 2);
        setState(613);
        curlyLiteral();
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
  enterRule(_localctx, 118, ClythV1Parser::RuleListLiteral);
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
    setState(616);
    match(ClythV1Parser::LBRACKET);
    setState(618);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & -898468125476913152) != 0)) {
      setState(617);
      expressionList();
    }
    setState(620);
    match(ClythV1Parser::RBRACKET);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- CurlyLiteralContext ------------------------------------------------------------------

ClythV1Parser::CurlyLiteralContext::CurlyLiteralContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ClythV1Parser::CurlyLiteralContext::LBRACE() {
  return getToken(ClythV1Parser::LBRACE, 0);
}

tree::TerminalNode* ClythV1Parser::CurlyLiteralContext::RBRACE() {
  return getToken(ClythV1Parser::RBRACE, 0);
}

ClythV1Parser::CurlyEntryListContext* ClythV1Parser::CurlyLiteralContext::curlyEntryList() {
  return getRuleContext<ClythV1Parser::CurlyEntryListContext>(0);
}


size_t ClythV1Parser::CurlyLiteralContext::getRuleIndex() const {
  return ClythV1Parser::RuleCurlyLiteral;
}

void ClythV1Parser::CurlyLiteralContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterCurlyLiteral(this);
}

void ClythV1Parser::CurlyLiteralContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitCurlyLiteral(this);
}


std::any ClythV1Parser::CurlyLiteralContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ClythV1Visitor*>(visitor))
    return parserVisitor->visitCurlyLiteral(this);
  else
    return visitor->visitChildren(this);
}

ClythV1Parser::CurlyLiteralContext* ClythV1Parser::curlyLiteral() {
  CurlyLiteralContext *_localctx = _tracker.createInstance<CurlyLiteralContext>(_ctx, getState());
  enterRule(_localctx, 120, ClythV1Parser::RuleCurlyLiteral);
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
    setState(622);
    match(ClythV1Parser::LBRACE);
    setState(624);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & -898468125476913152) != 0)) {
      setState(623);
      curlyEntryList();
    }
    setState(626);
    match(ClythV1Parser::RBRACE);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- CurlyEntryListContext ------------------------------------------------------------------

ClythV1Parser::CurlyEntryListContext::CurlyEntryListContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<ClythV1Parser::CurlyEntryContext *> ClythV1Parser::CurlyEntryListContext::curlyEntry() {
  return getRuleContexts<ClythV1Parser::CurlyEntryContext>();
}

ClythV1Parser::CurlyEntryContext* ClythV1Parser::CurlyEntryListContext::curlyEntry(size_t i) {
  return getRuleContext<ClythV1Parser::CurlyEntryContext>(i);
}

std::vector<tree::TerminalNode *> ClythV1Parser::CurlyEntryListContext::COMMA() {
  return getTokens(ClythV1Parser::COMMA);
}

tree::TerminalNode* ClythV1Parser::CurlyEntryListContext::COMMA(size_t i) {
  return getToken(ClythV1Parser::COMMA, i);
}


size_t ClythV1Parser::CurlyEntryListContext::getRuleIndex() const {
  return ClythV1Parser::RuleCurlyEntryList;
}

void ClythV1Parser::CurlyEntryListContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterCurlyEntryList(this);
}

void ClythV1Parser::CurlyEntryListContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitCurlyEntryList(this);
}


std::any ClythV1Parser::CurlyEntryListContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ClythV1Visitor*>(visitor))
    return parserVisitor->visitCurlyEntryList(this);
  else
    return visitor->visitChildren(this);
}

ClythV1Parser::CurlyEntryListContext* ClythV1Parser::curlyEntryList() {
  CurlyEntryListContext *_localctx = _tracker.createInstance<CurlyEntryListContext>(_ctx, getState());
  enterRule(_localctx, 122, ClythV1Parser::RuleCurlyEntryList);
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
    setState(628);
    curlyEntry();
    setState(633);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 71, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(629);
        match(ClythV1Parser::COMMA);
        setState(630);
        curlyEntry(); 
      }
      setState(635);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 71, _ctx);
    }
    setState(637);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ClythV1Parser::COMMA) {
      setState(636);
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

//----------------- CurlyEntryContext ------------------------------------------------------------------

ClythV1Parser::CurlyEntryContext::CurlyEntryContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<ClythV1Parser::ExpressionContext *> ClythV1Parser::CurlyEntryContext::expression() {
  return getRuleContexts<ClythV1Parser::ExpressionContext>();
}

ClythV1Parser::ExpressionContext* ClythV1Parser::CurlyEntryContext::expression(size_t i) {
  return getRuleContext<ClythV1Parser::ExpressionContext>(i);
}

tree::TerminalNode* ClythV1Parser::CurlyEntryContext::COLON() {
  return getToken(ClythV1Parser::COLON, 0);
}


size_t ClythV1Parser::CurlyEntryContext::getRuleIndex() const {
  return ClythV1Parser::RuleCurlyEntry;
}

void ClythV1Parser::CurlyEntryContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterCurlyEntry(this);
}

void ClythV1Parser::CurlyEntryContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitCurlyEntry(this);
}


std::any ClythV1Parser::CurlyEntryContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ClythV1Visitor*>(visitor))
    return parserVisitor->visitCurlyEntry(this);
  else
    return visitor->visitChildren(this);
}

ClythV1Parser::CurlyEntryContext* ClythV1Parser::curlyEntry() {
  CurlyEntryContext *_localctx = _tracker.createInstance<CurlyEntryContext>(_ctx, getState());
  enterRule(_localctx, 124, ClythV1Parser::RuleCurlyEntry);
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
    setState(639);
    expression();
    setState(642);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ClythV1Parser::COLON) {
      setState(640);
      match(ClythV1Parser::COLON);
      setState(641);
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
  enterRule(_localctx, 126, ClythV1Parser::RuleExpressionList);
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
    setState(644);
    expression();
    setState(649);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 74, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(645);
        match(ClythV1Parser::COMMA);
        setState(646);
        expression(); 
      }
      setState(651);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 74, _ctx);
    }
    setState(653);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ClythV1Parser::COMMA) {
      setState(652);
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

//----------------- TypeContext ------------------------------------------------------------------

ClythV1Parser::TypeContext::TypeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ClythV1Parser::FixedArrayTypeContext* ClythV1Parser::TypeContext::fixedArrayType() {
  return getRuleContext<ClythV1Parser::FixedArrayTypeContext>(0);
}

ClythV1Parser::DynamicArrayTypeContext* ClythV1Parser::TypeContext::dynamicArrayType() {
  return getRuleContext<ClythV1Parser::DynamicArrayTypeContext>(0);
}

ClythV1Parser::FunctionTypeContext* ClythV1Parser::TypeContext::functionType() {
  return getRuleContext<ClythV1Parser::FunctionTypeContext>(0);
}

ClythV1Parser::GenericTypeContext* ClythV1Parser::TypeContext::genericType() {
  return getRuleContext<ClythV1Parser::GenericTypeContext>(0);
}

ClythV1Parser::BaseTypeContext* ClythV1Parser::TypeContext::baseType() {
  return getRuleContext<ClythV1Parser::BaseTypeContext>(0);
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
  enterRule(_localctx, 128, ClythV1Parser::RuleType);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(660);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 76, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(655);
      fixedArrayType();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(656);
      dynamicArrayType();
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(657);
      functionType();
      break;
    }

    case 4: {
      enterOuterAlt(_localctx, 4);
      setState(658);
      genericType();
      break;
    }

    case 5: {
      enterOuterAlt(_localctx, 5);
      setState(659);
      baseType();
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

//----------------- FixedArrayTypeContext ------------------------------------------------------------------

ClythV1Parser::FixedArrayTypeContext::FixedArrayTypeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ClythV1Parser::TypeAtomContext* ClythV1Parser::FixedArrayTypeContext::typeAtom() {
  return getRuleContext<ClythV1Parser::TypeAtomContext>(0);
}

tree::TerminalNode* ClythV1Parser::FixedArrayTypeContext::LBRACKET() {
  return getToken(ClythV1Parser::LBRACKET, 0);
}

tree::TerminalNode* ClythV1Parser::FixedArrayTypeContext::NUMERIC_LITERAL() {
  return getToken(ClythV1Parser::NUMERIC_LITERAL, 0);
}

tree::TerminalNode* ClythV1Parser::FixedArrayTypeContext::RBRACKET() {
  return getToken(ClythV1Parser::RBRACKET, 0);
}


size_t ClythV1Parser::FixedArrayTypeContext::getRuleIndex() const {
  return ClythV1Parser::RuleFixedArrayType;
}

void ClythV1Parser::FixedArrayTypeContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterFixedArrayType(this);
}

void ClythV1Parser::FixedArrayTypeContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitFixedArrayType(this);
}


std::any ClythV1Parser::FixedArrayTypeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ClythV1Visitor*>(visitor))
    return parserVisitor->visitFixedArrayType(this);
  else
    return visitor->visitChildren(this);
}

ClythV1Parser::FixedArrayTypeContext* ClythV1Parser::fixedArrayType() {
  FixedArrayTypeContext *_localctx = _tracker.createInstance<FixedArrayTypeContext>(_ctx, getState());
  enterRule(_localctx, 130, ClythV1Parser::RuleFixedArrayType);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(662);
    typeAtom();
    setState(663);
    match(ClythV1Parser::LBRACKET);
    setState(664);
    match(ClythV1Parser::NUMERIC_LITERAL);
    setState(665);
    match(ClythV1Parser::RBRACKET);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- DynamicArrayTypeContext ------------------------------------------------------------------

ClythV1Parser::DynamicArrayTypeContext::DynamicArrayTypeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ClythV1Parser::TypeAtomContext* ClythV1Parser::DynamicArrayTypeContext::typeAtom() {
  return getRuleContext<ClythV1Parser::TypeAtomContext>(0);
}

tree::TerminalNode* ClythV1Parser::DynamicArrayTypeContext::LBRACKET() {
  return getToken(ClythV1Parser::LBRACKET, 0);
}

tree::TerminalNode* ClythV1Parser::DynamicArrayTypeContext::RBRACKET() {
  return getToken(ClythV1Parser::RBRACKET, 0);
}


size_t ClythV1Parser::DynamicArrayTypeContext::getRuleIndex() const {
  return ClythV1Parser::RuleDynamicArrayType;
}

void ClythV1Parser::DynamicArrayTypeContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterDynamicArrayType(this);
}

void ClythV1Parser::DynamicArrayTypeContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitDynamicArrayType(this);
}


std::any ClythV1Parser::DynamicArrayTypeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ClythV1Visitor*>(visitor))
    return parserVisitor->visitDynamicArrayType(this);
  else
    return visitor->visitChildren(this);
}

ClythV1Parser::DynamicArrayTypeContext* ClythV1Parser::dynamicArrayType() {
  DynamicArrayTypeContext *_localctx = _tracker.createInstance<DynamicArrayTypeContext>(_ctx, getState());
  enterRule(_localctx, 132, ClythV1Parser::RuleDynamicArrayType);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(667);
    typeAtom();
    setState(668);
    match(ClythV1Parser::LBRACKET);
    setState(669);
    match(ClythV1Parser::RBRACKET);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- FunctionTypeContext ------------------------------------------------------------------

ClythV1Parser::FunctionTypeContext::FunctionTypeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ClythV1Parser::FunctionTypeContext::FUNCTION() {
  return getToken(ClythV1Parser::FUNCTION, 0);
}

tree::TerminalNode* ClythV1Parser::FunctionTypeContext::LT() {
  return getToken(ClythV1Parser::LT, 0);
}

ClythV1Parser::TypeContext* ClythV1Parser::FunctionTypeContext::type() {
  return getRuleContext<ClythV1Parser::TypeContext>(0);
}

tree::TerminalNode* ClythV1Parser::FunctionTypeContext::LPAREN() {
  return getToken(ClythV1Parser::LPAREN, 0);
}

tree::TerminalNode* ClythV1Parser::FunctionTypeContext::RPAREN() {
  return getToken(ClythV1Parser::RPAREN, 0);
}

tree::TerminalNode* ClythV1Parser::FunctionTypeContext::GT() {
  return getToken(ClythV1Parser::GT, 0);
}

ClythV1Parser::FunctionParamTypeListContext* ClythV1Parser::FunctionTypeContext::functionParamTypeList() {
  return getRuleContext<ClythV1Parser::FunctionParamTypeListContext>(0);
}


size_t ClythV1Parser::FunctionTypeContext::getRuleIndex() const {
  return ClythV1Parser::RuleFunctionType;
}

void ClythV1Parser::FunctionTypeContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterFunctionType(this);
}

void ClythV1Parser::FunctionTypeContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitFunctionType(this);
}


std::any ClythV1Parser::FunctionTypeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ClythV1Visitor*>(visitor))
    return parserVisitor->visitFunctionType(this);
  else
    return visitor->visitChildren(this);
}

ClythV1Parser::FunctionTypeContext* ClythV1Parser::functionType() {
  FunctionTypeContext *_localctx = _tracker.createInstance<FunctionTypeContext>(_ctx, getState());
  enterRule(_localctx, 134, ClythV1Parser::RuleFunctionType);
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
    setState(671);
    match(ClythV1Parser::FUNCTION);
    setState(672);
    match(ClythV1Parser::LT);
    setState(673);
    type();
    setState(674);
    match(ClythV1Parser::LPAREN);
    setState(676);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & -8646911284551352192) != 0)) {
      setState(675);
      functionParamTypeList();
    }
    setState(678);
    match(ClythV1Parser::RPAREN);
    setState(679);
    match(ClythV1Parser::GT);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- FunctionParamTypeListContext ------------------------------------------------------------------

ClythV1Parser::FunctionParamTypeListContext::FunctionParamTypeListContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<ClythV1Parser::TypeContext *> ClythV1Parser::FunctionParamTypeListContext::type() {
  return getRuleContexts<ClythV1Parser::TypeContext>();
}

ClythV1Parser::TypeContext* ClythV1Parser::FunctionParamTypeListContext::type(size_t i) {
  return getRuleContext<ClythV1Parser::TypeContext>(i);
}

std::vector<tree::TerminalNode *> ClythV1Parser::FunctionParamTypeListContext::COMMA() {
  return getTokens(ClythV1Parser::COMMA);
}

tree::TerminalNode* ClythV1Parser::FunctionParamTypeListContext::COMMA(size_t i) {
  return getToken(ClythV1Parser::COMMA, i);
}


size_t ClythV1Parser::FunctionParamTypeListContext::getRuleIndex() const {
  return ClythV1Parser::RuleFunctionParamTypeList;
}

void ClythV1Parser::FunctionParamTypeListContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterFunctionParamTypeList(this);
}

void ClythV1Parser::FunctionParamTypeListContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitFunctionParamTypeList(this);
}


std::any ClythV1Parser::FunctionParamTypeListContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ClythV1Visitor*>(visitor))
    return parserVisitor->visitFunctionParamTypeList(this);
  else
    return visitor->visitChildren(this);
}

ClythV1Parser::FunctionParamTypeListContext* ClythV1Parser::functionParamTypeList() {
  FunctionParamTypeListContext *_localctx = _tracker.createInstance<FunctionParamTypeListContext>(_ctx, getState());
  enterRule(_localctx, 136, ClythV1Parser::RuleFunctionParamTypeList);
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
    setState(681);
    type();
    setState(686);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 78, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(682);
        match(ClythV1Parser::COMMA);
        setState(683);
        type(); 
      }
      setState(688);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 78, _ctx);
    }
    setState(690);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ClythV1Parser::COMMA) {
      setState(689);
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

//----------------- GenericTypeContext ------------------------------------------------------------------

ClythV1Parser::GenericTypeContext::GenericTypeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ClythV1Parser::GenericTypeContext::IDENTIFIER() {
  return getToken(ClythV1Parser::IDENTIFIER, 0);
}

tree::TerminalNode* ClythV1Parser::GenericTypeContext::LT() {
  return getToken(ClythV1Parser::LT, 0);
}

ClythV1Parser::GenericArgListContext* ClythV1Parser::GenericTypeContext::genericArgList() {
  return getRuleContext<ClythV1Parser::GenericArgListContext>(0);
}

tree::TerminalNode* ClythV1Parser::GenericTypeContext::GT() {
  return getToken(ClythV1Parser::GT, 0);
}


size_t ClythV1Parser::GenericTypeContext::getRuleIndex() const {
  return ClythV1Parser::RuleGenericType;
}

void ClythV1Parser::GenericTypeContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterGenericType(this);
}

void ClythV1Parser::GenericTypeContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitGenericType(this);
}


std::any ClythV1Parser::GenericTypeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ClythV1Visitor*>(visitor))
    return parserVisitor->visitGenericType(this);
  else
    return visitor->visitChildren(this);
}

ClythV1Parser::GenericTypeContext* ClythV1Parser::genericType() {
  GenericTypeContext *_localctx = _tracker.createInstance<GenericTypeContext>(_ctx, getState());
  enterRule(_localctx, 138, ClythV1Parser::RuleGenericType);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(692);
    match(ClythV1Parser::IDENTIFIER);
    setState(693);
    match(ClythV1Parser::LT);
    setState(694);
    genericArgList();
    setState(695);
    match(ClythV1Parser::GT);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- GenericArgListContext ------------------------------------------------------------------

ClythV1Parser::GenericArgListContext::GenericArgListContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<ClythV1Parser::TypeContext *> ClythV1Parser::GenericArgListContext::type() {
  return getRuleContexts<ClythV1Parser::TypeContext>();
}

ClythV1Parser::TypeContext* ClythV1Parser::GenericArgListContext::type(size_t i) {
  return getRuleContext<ClythV1Parser::TypeContext>(i);
}

std::vector<tree::TerminalNode *> ClythV1Parser::GenericArgListContext::COMMA() {
  return getTokens(ClythV1Parser::COMMA);
}

tree::TerminalNode* ClythV1Parser::GenericArgListContext::COMMA(size_t i) {
  return getToken(ClythV1Parser::COMMA, i);
}

std::vector<tree::TerminalNode *> ClythV1Parser::GenericArgListContext::COLON() {
  return getTokens(ClythV1Parser::COLON);
}

tree::TerminalNode* ClythV1Parser::GenericArgListContext::COLON(size_t i) {
  return getToken(ClythV1Parser::COLON, i);
}


size_t ClythV1Parser::GenericArgListContext::getRuleIndex() const {
  return ClythV1Parser::RuleGenericArgList;
}

void ClythV1Parser::GenericArgListContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterGenericArgList(this);
}

void ClythV1Parser::GenericArgListContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitGenericArgList(this);
}


std::any ClythV1Parser::GenericArgListContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ClythV1Visitor*>(visitor))
    return parserVisitor->visitGenericArgList(this);
  else
    return visitor->visitChildren(this);
}

ClythV1Parser::GenericArgListContext* ClythV1Parser::genericArgList() {
  GenericArgListContext *_localctx = _tracker.createInstance<GenericArgListContext>(_ctx, getState());
  enterRule(_localctx, 140, ClythV1Parser::RuleGenericArgList);
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
    setState(697);
    type();
    setState(702);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == ClythV1Parser::COMMA

    || _la == ClythV1Parser::COLON) {
      setState(698);
      _la = _input->LA(1);
      if (!(_la == ClythV1Parser::COMMA

      || _la == ClythV1Parser::COLON)) {
      _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(699);
      type();
      setState(704);
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

//----------------- TypeAtomContext ------------------------------------------------------------------

ClythV1Parser::TypeAtomContext::TypeAtomContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ClythV1Parser::GenericTypeContext* ClythV1Parser::TypeAtomContext::genericType() {
  return getRuleContext<ClythV1Parser::GenericTypeContext>(0);
}

ClythV1Parser::BaseTypeContext* ClythV1Parser::TypeAtomContext::baseType() {
  return getRuleContext<ClythV1Parser::BaseTypeContext>(0);
}


size_t ClythV1Parser::TypeAtomContext::getRuleIndex() const {
  return ClythV1Parser::RuleTypeAtom;
}

void ClythV1Parser::TypeAtomContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterTypeAtom(this);
}

void ClythV1Parser::TypeAtomContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ClythV1Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitTypeAtom(this);
}


std::any ClythV1Parser::TypeAtomContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ClythV1Visitor*>(visitor))
    return parserVisitor->visitTypeAtom(this);
  else
    return visitor->visitChildren(this);
}

ClythV1Parser::TypeAtomContext* ClythV1Parser::typeAtom() {
  TypeAtomContext *_localctx = _tracker.createInstance<TypeAtomContext>(_ctx, getState());
  enterRule(_localctx, 142, ClythV1Parser::RuleTypeAtom);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(707);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 81, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(705);
      genericType();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(706);
      baseType();
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
  enterRule(_localctx, 144, ClythV1Parser::RuleBaseType);
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
    setState(709);
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
  enterRule(_localctx, 146, ClythV1Parser::RuleCommaOrSemi);
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
    setState(711);
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
