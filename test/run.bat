@echo off

echo Compiling test_ls_list.c
gcc test_ls_list.c ../src/ls_list.c -o test_ls_list

echo Running test_ls_list.exe
test_ls_list.exe
