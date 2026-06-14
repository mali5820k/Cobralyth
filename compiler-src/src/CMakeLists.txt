
target_include_directories(clyth_compiler_bin PUBLIC ${CMAKE_SOURCE_DIR}/src/cxxopts/
	${CMAKE_SOURCE_DIR}/src/clyth_antlr_files/
)
target_sources(
	clyth_compiler_bin PUBLIC
	scanner.cpp
	utilFunctions.cpp
	main.cpp
	clythAST.cpp
	clythLLVMStub.cpp
	clythLoweringPlan.cpp
	clythSemantic.cpp
	clyth_antlr_files/ClythV1BaseListener.cpp
	clyth_antlr_files/ClythV1Lexer.cpp
	clyth_antlr_files/ClythV1Listener.cpp
	clyth_antlr_files/ClythV1Parser.cpp
)
