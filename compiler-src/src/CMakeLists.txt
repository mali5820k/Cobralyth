target_sources(
	clyth_compiler_bin PUBLIC
	common.hpp
	cxxopts/cxxopts.hpp
	scanner.hpp
	utilFunctions.hpp
	scanner.cpp
	utilFunctions.cpp
	main.cpp
	clyth_antlr_files/ClythV1BaseListener.cpp
	clyth_antlr_files/ClythV1BaseListener.h
	clyth_antlr_files/ClythV1Lexer.cpp
	clyth_antlr_files/ClythV1Lexer.h
	clyth_antlr_files/ClythV1Listener.cpp
	clyth_antlr_files/ClythV1Listener.h
	clyth_antlr_files/ClythV1Parser.cpp
	clyth_antlr_files/ClythV1Parser.h
)
