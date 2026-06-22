target_include_directories(clyth_compiler_bin PUBLIC
    ${CMAKE_SOURCE_DIR}/src/cxxopts/
    ${CMAKE_SOURCE_DIR}/src/clyth_antlr_files/
)

target_sources(clyth_compiler_bin PUBLIC
    scanner.cpp
    util_functions.cpp
    main.cpp
    clyth_ast.cpp
    clyth_llvm_stub.cpp
    clyth_lowering_plan.cpp
    clyth_semantic.cpp
    clyth_antlr_files/ClythV1BaseListener.cpp
    clyth_antlr_files/ClythV1Lexer.cpp
    clyth_antlr_files/ClythV1Listener.cpp
    clyth_antlr_files/ClythV1Parser.cpp
)