#!/bin/bash

SUCCESS=0
FAIL=0
COUNTER=0
DIFF_RES=""

declare -a multy_testing=(
"for VAR test_6_grep.txt"
"for VAR test_1_grep.txt test_5_grep.txt test_2_grep.txt"
"-e a -e b -e c VAR test_6_grep.txt"
"-e for -e ^int VAR test_1_grep.txt test_5_grep.txt test_2_grep.txt"
"-e for -e ^int -f test_4_grep.txt VAR test_1_grep.txt test_5_grep.txt test_2_grep.txt"
"-e for -e ^int -f test_4_grep.txt -f test_3_grep.txt VAR test_1_grep.txt test_5_grep.txt test_2_grep.txt"
)

declare -a o_mac_testing=(
"for VAR test_6_grep.txt"
"for VAR test_6_grep.txt test_5_grep.txt test_2_grep.txt"
"-e a VAR test_6_grep.txt"
"-e for VAR test_6_grep.txt test_5_grep.txt test_2_grep.txt"
"-f test_4_grep.txt VAR test_1_grep.txt test_5_grep.txt test_2_grep.txt"
)

testing()
{
    t=$(echo $@ | sed "s/VAR/$var/")
    ../s21_grep $t > test_s21_grep.log
    grep $t > test_sys_grep.log
    DIFF_RES="$(diff -s test_s21_grep.log test_sys_grep.log)"
    (( COUNTER++ ))
    if [ "$DIFF_RES" == "Files test_s21_grep.log and test_sys_grep.log are identical" ]
    then
      (( SUCCESS++ ))
        echo "\033[32mTEST\t$COUNTER:\tLUCKY BOY"
        echo "\033[0m grep $t"
    else
      (( FAIL++ ))
        echo "\033[31mTEST\t$COUNTER:\tNO WAY :("
        echo "\033[0m grep $t"
    fi
    rm test_s21_grep.log test_sys_grep.log
}

for var1 in i v c l n h s
do
  for i in "${multy_testing[@]}"
  do
    var="-$var1"
    testing $i
  done
done

for var1 in i v c l n h s
do
  for var2 in i v c l n h s
  do
    for i in "${multy_testing[@]}"
      do
        var="-$var1$var2"
        testing $i
      done
  done
done

for var1 in i v c l n h s
do
  for var2 in i v c l n h s
  do
    for var3 in i v c l n h s
    do
      for i in "${multy_testing[@]}"
      do
        var="-$var1$var2$var3"
        testing $i
      done   
    done
  done
done

for var1 in i v c l n h s o
do
  for var2 in i v c l n h s o
  do
    for var3 in i v c l n h s o
    do
      for i in "${o_mac_testing[@]}"
      do
        var="-$var1$var2$var3"
        testing $i
      done   
    done
  done
done

for i in "${unique_testing[@]}"
do
    var="-"
    testing $i
done

echo "\033[31mFAIL: $FAIL\033[0m"
echo "\033[32mSUCCESS: $SUCCESS\033[0m"
echo "ALL: $COUNTER"