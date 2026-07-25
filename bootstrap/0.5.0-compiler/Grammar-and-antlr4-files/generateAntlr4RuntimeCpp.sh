mkdir clyth_antlr_files/
antlr4 -Dlanguage=Cpp -visitor $1
rm -f ClythV1*.{tokens,interp}
rm -rf ../compiler-src/src/clyth_antlr_files/
mv ClythV1*.{cpp,h} clyth_antlr_files/
mv clyth_antlr_files/ ../compiler-src/src/
