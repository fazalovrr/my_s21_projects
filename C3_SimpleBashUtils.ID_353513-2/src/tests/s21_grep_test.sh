#!/bin/bash

# Создаем тестовые файлы
echo -e "school\noschool\university\school21" > school.txt
echo -e "red\nblue\ngreen\nblueberry" > colors.txt
echo "pattern" > pattern.txt

echo "***Test s21_grep***"

# поиск без флагов
echo "Test 1: without flags"
grep "school" school.txt > grep.txt
../grep/s21_grep "school" school.txt > s21_grep.txt
diff grep.txt s21_grep.txt || echo "Test 1 failed"

# флаг -e (Шаблон)
echo "Test 2: -e flag"
grep -e "school21" -e "Kazan" school.txt > grep.txt
../grep/s21_grep -e "school21" -e "Kazan" school.txt > s21_grep.txt
diff grep.txt s21_grep.txt || echo "Test 2 failed"

# флаг -i (игнорирование регистра)
echo "Test 3: -i flag"
grep -i "SCHOOL" school.txt > grep.txt
../grep/s21_grep -i "SCHOOL" school.txt > s21_grep.txt
diff grep.txt s21_grep.txt || echo "Test 3 failed"

# флаг -v (инвертированный поиск)
echo "Test 4: -v flag"
grep -v "school" school.txt > grep.txt
../grep/s21_grep -v "school" school.txt > s21_grep.txt
diff grep.txt s21_grep.txt || echo "Test 4 failed"

# флаг -c (подсчет совпадений)
echo "Test 5: -c flag"
grep -c "blue" colors.txt > grep.txt
../grep/s21_grep -c "blue" colors.txt > s21_grep.txt
diff grep.txt s21_grep.txt || echo "Test 5 failed"

# флаг -l (совпадающие файлы)
echo "Test 6: -l flag"
grep -l "school21" *.txt school.txt > grep.txt
../grep/s21_grep -l "school21" *.txt school.txt > s21_grep.txt
diff grep.txt s21_grep.txt || echo "Test 6 failed"

# флаг -n (номера строк)
echo "Test 7: -n flag"
grep -n "blue" colors.txt > grep.txt
../grep/s21_grep -n "blue" colors.txt > s21_grep.txt
diff grep.txt s21_grep.txt || echo "Test 7 failed"

# флаг -h (без имен файлов)
echo "Test 8: -h flag"
grep -h "blue" colors.txt school.txt > grep.txt
../grep/s21_grep -h "blue" colors.txt school.txt > s21_grep.txt
diff grep.txt s21_grep.txt || echo "Test 8 failed"

# флаг -s (без error msg)
echo "Test 9: -s flag"
grep -s "blue" nonschool.txt > grep.txt
../grep/s21_grep -s "blue" nonschool.txt > s21_grep.txt
diff grep.txt s21_grep.txt || echo "Test 9 failed"

# флаг -f (чтение шаблона из файла)
echo "Test 10: -f flag"
grep -f pattern.txt school.txt > grep.txt
../grep/s21_grep -f pattern.txt school.txt > s21_grep.txt
diff grep.txt s21_grep.txt || echo "Test 10 failed"

# флаг -o (только совпадающая часть)
echo "Test 11: -o flag"
grep -o "blue" colors.txt > grep.txt
../grep/s21_grep -o "blue" colors.txt > s21_grep.txt
diff grep.txt s21_grep.txt || echo "Test 11 failed"

# комбинацию флагов
echo "Test 12: -iv flags"
grep -iv "SCHOOL" school.txt > grep.txt
../grep/s21_grep -iv "SCHOOL" school.txt > s21_grep.txt
diff grep.txt s21_grep.txt || echo "Test 12 failed"

# комбинацию флагов
echo "Test 13: -in flags"
grep -in "SCHOOL" school.txt > grep.txt
../grep/s21_grep -in "SCHOOL" school.txt > s21_grep.txt
diff grep.txt s21_grep.txt || echo "Test 13 failed"

# Удаляем временные файлы
rm school.txt colors.txt pattern.txt grep.txt s21_grep.txt
