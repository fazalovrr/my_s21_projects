#!/bin/bash

echo -e "Normal line\n\nLine with\tTAB\nLine with\vtab" > temp_file.txt
echo -e "Line with\rcarriage\nLine with\x7F DEL" >> temp_file.txt
echo -e "\n\n" >> temp_file.txt
echo -e "Line with \x01\x02\x03 control\nLine with \x7F DEL" >> temp_file.txt

echo "$test_text" > temp_file.txt

echo "***Test s21_cat***"

echo "Test 1: without flags"
cat temp_file.txt > cat.txt
../cat/s21_cat temp_file.txt > s21_cat.txt
diff -u cat.txt s21_cat.txt || echo "Test 1 failed"

# флаг -b (нумерация непустых строк)
echo "Test 2: -b flag"
cat -b temp_file.txt > cat.txt
../cat/s21_cat -b temp_file.txt > s21_cat.txt
diff -u cat.txt s21_cat.txt || echo "Test 2 failed"

# флаг -e (отображение $ в конце строк)
echo "Test 3: -e flag"
cat -e temp_file.txt > cat.txt
../cat/s21_cat -e temp_file.txt > s21_cat.txt
diff -u cat.txt s21_cat.txt || echo "Test 3 failed"

# флаг -n (нумерация строк)
echo "Test 4: -n flag"
cat -n temp_file.txt > cat.txt
../cat/s21_cat -n temp_file.txt > s21_cat.txt
diff -u cat.txt s21_cat.txt || echo "Test 4 failed"

# флаг -s (сжатие пустых строк)
echo "Test 5: -s flag"
cat -s temp_file.txt > cat.txt
../cat/s21_cat -s temp_file.txt > s21_cat.txt
diff -u cat.txt s21_cat.txt || echo "Test 5 failed"

# флаг -t (отображение табов)
echo "Test 6: -t flag"
cat -t temp_file.txt > cat.txt
../cat/s21_cat -t temp_file.txt > s21_cat.txt
diff -u cat.txt s21_cat.txt || echo "Test 6 failed"

rm -f temp_file.txt cat.txt s21_cat.txt