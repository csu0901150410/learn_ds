@echo off

echo Compiling test_ls_entity.c
gcc test_ls_entity.c ../src/ls_list.c ../src/ls_vector.c ../src/ls_line_segment.c ../src/ls_arc.c ../src/ls_entity.c -o test_ls_entity

echo Running test_ls_entity.exe
test_ls_entity.exe
