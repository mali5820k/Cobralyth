mkdir clyth_antlr_files/
antlr4 -Dlanguage=Cpp $1
rm -f ClythV1*.{tokens,interp}
mv ClythV1*.{cpp,h} clyth_antlr_files/
mv clyth_antlr_files/ ../compiler-src/src/
