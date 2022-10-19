#!/bin/bash
#Pour lancer le test, donnez lui un droit d'excecution, et renseignez les "path" vers vos répertoires.
#Your sudoku project folder
PROJECT_PATH="./sudoku"

#Stores the previous working directory
PREVIOUS_PWD=${PWD}
ERROR_FILE_TEST=${PREVIOUS_PWD}/stderr_file

#Your test folders
TEST_PATH="/"
#TEST_PATH2="/home/.../.../Sudoku/test/grid-solver/"

ERR=0
SUCC=0
EXIT_SUCCESS=0
EXIT_FAILURE=1
EXIT_FAILURE2=139
VAL_FAILURE=10

VERBOSE=0

FILE_OUTPUT="/dev/null"

test_exit_success(){
	echo -n "Result : "
	if ((EXIT_SUCCESS == $1))
        then
                echo -e "*Passed*\n" && SUCC=$((SUCC + 1)) && return
        fi
        echo -e "***Failed!***\n"
	cat $ERROR_FILE_TEST
        ERR=$((ERR+1))
        return

}

test_exit_failure(){
	echo -n "Result : "
	(((EXIT_FAILURE == $1)) || ((EXIT_FAILURE2 == $1))) && test_stderr && SUCC=$((SUCC+1)) &&echo -e "*Passed*\n"  && return
	echo -e "***Failed!***\n"
	cat $ERROR_FILE_TEST
	ERR=$((ERR+1))
	return
}

test_stderr_empty(){
	if(test -f $ERROR_FILE_TEST); then
		return  `cat $ERROR_FILE_TEST | (($(wc -l) == 0))`
	else
		echo '$ERROR_FILE_TEST'
	fi
}

test_stderr(){
	if(test -f $ERROR_FILE_TEST); then
		return  `cat $ERROR_FILE_TEST | (($(wc -l) > 0))`
	else
		echo '$ERROR_FILE_TEST'
	fi
}

test_file(){
	if(test -f $1); then
		echo "file $1 exist !"
	fi
}


test_valgrind(){
	echo -n "Result : "
	if (($1 == $VAL_FAILURE))
	then
		echo -e "***Failed*** : memory problems detected\n"
		ERR=$((ERR+1))
	else
		echo -e "*Passed* : no memory problem\n"
		SUCC=$((SUCC+1))
	fi
}

start_time="$(date -u +%s.%N)"

echo "moving to ${PROJECT_PATH}"
cd ${PROJECT_PATH}

echo -e '---------- ( Build System ) ----------\n'

echo "* Check 'make clean'"
echo "Expected result: EXIT_SUCCESS"
make clean > /dev/null
test_exit_success $?

echo "* Check 'make'"
echo "Expected result: EXIT_SUCCESS"
make &> /dev/null
test_exit_success $?

#Check requested files
#Check gcc call

echo "* Check 'make clean'"
echo "Expected result: clean target must remove all built files"
make clean > /dev/null
#check all built files was remove
test_exit_success $?

echo "* Check 'make help'"
echo "Expected result: EXIT_SUCCESS"
make help > /dev/null
test_exit_success $?

echo -e '---------- ( Option Parser ) ----------\n'

make &> /dev/null

echo "* Check './sudoku'"
echo "Expected result: EXIT_FAILURE and stderr is written"
./sudoku 1> /dev/null 2> $ERROR_FILE_TEST
test_exit_failure $?


echo "* Check './sudoku -x'"
echo "Expected result: EXIT_FAILURE and stderr is written"
./sudoku -x 1> /dev/null 2> $ERROR_FILE_TEST
test_exit_failure $?

echo "* Check './sudoku --xenophon'"
echo "Expected result: EXIT_FAILURE and stderr is written"
./sudoku --xenophon 1> /dev/null 2> $ERROR_FILE_TEST
test_exit_failure $?

echo "* Check './sudoku -v'"
echo "Expected result: EXIT_FAILURE and stderr is written"
./sudoku -v 1> /dev/null 2> $ERROR_FILE_TEST
test_exit_failure $?

echo "* Check './sudoku --verbose'"
echo "Expected result: EXIT_FAILURE and stderr is written"
./sudoku --verbose 1> /dev/null 2> $ERROR_FILE_TEST
test_exit_failure $?


echo "* Check './sudoku -V'"
echo "Expected result: EXIT_SUCCESS"
./sudoku -V 1> /dev/null 2> $ERROR_FILE_TEST
test_exit_success $?

echo "* Check './sudoku --version'"
echo "Expected result: EXIT_SUCCESS"
./sudoku --version 1> /dev/null 2> $ERROR_FILE_TEST
test_exit_success $?

echo "* Check './sudoku -h'"
echo "Expected result: EXIT_SUCCESS"
./sudoku -h 1> /dev/null 2> $ERROR_FILE_TEST
test_exit_success $?

echo "* Check './sudoku --help'"
echo "Expected result: EXIT_SUCCESS"
./sudoku --help 1> /dev/null 2> $ERROR_FILE_TEST
test_exit_success $?

echo "* Check './sudoku --he'"
echo "Expected result: EXIT_SUCCESS"
./sudoku --he 1> /dev/null 2> $ERROR_FILE_TEST
test_exit_success $?

echo "* Check './sudoku grid-01-size_1.sku'"
echo "Expected result: EXIT_SUCCESS"
./sudoku $TEST_PATH/grid-01-size_1-pass.sku 1> /dev/null 2> $ERROR_FILE_TEST
test_exit_success $?

echo "* Check './sudoku grid-01-size_1.sku grid-02-size_4.sku grid-03-size_9.sku grid-04-size_16.sku'"
echo "Expected result: EXIT_SUCCESS"
./sudoku $TEST_PATH/grid-01-size_1-pass.sku $TEST_PATH/grid-02-size_4-pass.sku $TEST_PATH/grid-03-size_9-pass.sku $TEST_PATH/grid-04-size_16-pass.sku 1> /dev/null 2> $ERROR_FILE_TEST
test_exit_success $?

echo "* Check './sudoku -v grid-01-size_1.sku grid-02-size_4.sku grid-03-size_9.sku grid-04-size_16.sku'"
echo "Expected result: EXIT_SUCCESS"
./sudoku -v $TEST_PATH/grid-01-size_1-pass.sku $TEST_PATH/grid-02-size_4-pass.sku $TEST_PATH/grid-03-size_9-pass.sku $TEST_PATH/grid-04-size_16-pass.sku 1> /dev/null 2> $ERROR_FILE_TEST
test_exit_success $?

echo "* Check './sudoku -x grid-01-size_1.sku'"
echo "Expected result: EXIT_FAILURE and stderr is written"
./sudoku -x $TEST_PATH/grid-01-size_1-pass.sku 1> /dev/null 2> $ERROR_FILE_TEST
test_exit_failure $?

echo "* Check './sudoku -v grid-01-size_1.sku'"
echo "Expected result: EXIT_SUCCESS"
./sudoku -v $TEST_PATH/grid-01-size_1-pass.sku 1> /dev/null 2> $ERROR_FILE_TEST
test_exit_success $?

echo "* Check './sudoku --verbose grid-01-size_1.sku'"
echo "Expected result: EXIT_SUCCESS"
./sudoku --verbose $TEST_PATH/grid-01-size_1-pass.sku 1> /dev/null 2> $ERROR_FILE_TEST
test_exit_success $?

echo "* Check './sudoku -o'"
echo "Expected result: EXIT_FAILURE and stderr is written"
./sudoku -o 1> /dev/null 2> $ERROR_FILE_TEST
test_exit_failure $?

echo "* Check './sudoku --output'"
echo "Expected result: EXIT_FAILURE and stderr is written"
./sudoku --output 1> /dev/null 2> $ERROR_FILE_TEST
test_exit_failure $?

echo "* Check './sudoku -o test.sku'"
echo "Expected result: EXIT_FAILURE and stderr is written"
./sudoku -o $PREVIOUS_PWD/test.sku 1> /dev/null 2> $ERROR_FILE_TEST
test_exit_failure $?

echo "* Check './sudoku --output test.sku'"
echo "Expected result: EXIT_FAILURE and stderr is written"
./sudoku --output $PREVIOUS_PWD/test.sku 1> /dev/null 2> $ERROR_FILE_TEST
test_exit_failure $?

echo "* Check './sudoku -o test.sku grid-01-size_1.sku'"
echo "Expected result: EXIT_SUCCESS"
./sudoku -o $PREVIOUS_PWD/test.sku $TEST_PATH/grid-01-size_1-pass.sku 1> /dev/null 2> $ERROR_FILE_TEST
test_exit_success $?

echo "* Check './sudoku --output test.sku grid-01-size_1.sku'"
echo "Expected result: EXIT_SUCCESS"
./sudoku --output $PREVIOUS_PWD/test.sku $TEST_PATH/grid-01-size_1-pass.sku 1> /dev/null 2> $ERROR_FILE_TEST
test_exit_success $?


echo "* Check './sudoku -o /test.sku grid-01-size_1.sku'"
echo "Expected result: EXIT_FAILURE and stderr is written"
./sudoku -o /test.sku $TEST_PATH/grid-01-size_1-pass.sku 1> /dev/null 2> $ERROR_FILE_TEST
#test if file was create ?
test_exit_failure $?

echo "* Check './sudoku --output /test.sku grid-01-size_1.sku'"
echo "Expected result: EXIT_FAILURE and stderr is written"
./sudoku --output /test.sku $TEST_PATH/grid-01-size_1-pass.sku 1> /dev/null 2> $ERROR_FILE_TEST
#test if file was create ?
test_exit_failure $?


echo "* Check './sudoku -g'"
echo "Expected result: EXIT_SUCCESS"
./sudoku -g 1> /dev/null 2> $ERROR_FILE_TEST
test_exit_success $?

echo "* Check './sudoku -g'"
echo "Expected result: EXIT_SUCCESS"
./sudoku -g 1> /dev/null 2> $ERROR_FILE_TEST
test_exit_success $?

echo "* Check './sudoku --generate'"
echo "Expected result: EXIT_SUCCESS"
./sudoku --generate 1> /dev/null 2> $ERROR_FILE_TEST
test_exit_success $?

echo "* Check './sudoku -g1'"
echo "Expected result: EXIT_SUCCESS"
./sudoku -g1 1> /dev/null 2> $ERROR_FILE_TEST
test_exit_success $?

echo "* Check './sudoku --generate=1'"
echo "Expected result: EXIT_SUCCESS"
./sudoku --generate=1 1> /dev/null 2> $ERROR_FILE_TEST
test_exit_success $?

echo "* Check './sudoku -g4'"
echo "Expected result: EXIT_SUCCESS"
./sudoku -g4 1> /dev/null 2> $ERROR_FILE_TEST
test_exit_success $?

echo "* Check './sudoku --generate=4'"
echo "Expected result: EXIT_SUCCESS"
./sudoku --generate=4 1> /dev/null 2> $ERROR_FILE_TEST
test_exit_success $?

echo "* Check './sudoku -g9'"
echo "Expected result: EXIT_SUCCESS"
./sudoku -g9 1> /dev/null 2> $ERROR_FILE_TEST
test_exit_success $?

echo "* Check './sudoku -g16'"
echo "Expected result: EXIT_SUCCESS"
./sudoku -g16 1> /dev/null 2> $ERROR_FILE_TEST
test_exit_success $?

echo "* Check './sudoku --generate=16'"
echo "Expected result: EXIT_SUCCESS"
./sudoku --generate=16 1> /dev/null 2> $ERROR_FILE_TEST
test_exit_success $?

echo "* Check './sudoku -g25'"
echo "Expected result: EXIT_SUCCESS"
./sudoku -g25 1> /dev/null 2> $ERROR_FILE_TEST
test_exit_success $?

echo "* Check './sudoku --generate=25'"
echo "Expected result: EXIT_SUCCESS"
./sudoku --generate=25 1> /dev/null 2> $ERROR_FILE_TEST
test_exit_success $?

echo "* Check './sudoku -g36'"
echo "Expected result: EXIT_SUCCESS"
./sudoku -g36 1> /dev/null 2> $ERROR_FILE_TEST
test_exit_success $?

echo "* Check './sudoku --generate=36'"
echo "Expected result: EXIT_SUCCESS"
./sudoku --generate=36 1> /dev/null 2> $ERROR_FILE_TEST
test_exit_success $?

echo "* Check './sudoku -g49'"
echo "Expected result: EXIT_SUCCESS"
./sudoku -g49 1> /dev/null 2> $ERROR_FILE_TEST
test_exit_success $?

echo "* Check './sudoku --generate=49'"
echo "Expected result: EXIT_SUCCESS"
./sudoku --generate=49 1> /dev/null 2> $ERROR_FILE_TEST
test_exit_success $?

echo "* Check './sudoku -g64'"
echo "Expected result: EXIT_SUCCESS"
./sudoku -g49 1> /dev/null 2> $ERROR_FILE_TEST
test_exit_success $?

echo "* Check './sudoku --generate=64'"
echo "Expected result: EXIT_SUCCESS"
./sudoku --generate=64 1> /dev/null 2> $ERROR_FILE_TEST
test_exit_success $?

echo "* Check './sudoku -g10'"
echo "Expected result: EXIT_FAILURE and stderr is written"
./sudoku -g10 1> /dev/null 2> $ERROR_FILE_TEST
test_exit_failure $?

echo "* Check './sudoku --generate=10'"
echo "Expected result: EXIT_FAILURE and stderr is written"
./sudoku --generate=10 1> /dev/null 2> $ERROR_FILE_TEST
test_exit_failure $?



echo -e "----------( Grid Parser )----------\n"

echo "* Check './sudoku grid-00-empty_file-fail.sku'"
echo "Expected result: EXIT_FAILURE and stderr is written"
./sudoku $TEST_PATH/grid-00-empty_file-fail.sku 1> /dev/null 2> $ERROR_FILE_TEST
test_exit_failure $?

echo "* Check './sudoku grid-01-size_1.sku'"
echo "Expected result: EXIT_SUCCESS"
./sudoku $TEST_PATH/grid-01-size_1-pass.sku 1> /dev/null 2> $ERROR_FILE_TEST
test_exit_success $?

echo "* Check './sudoku grid-02-size_4.sku'"
echo "Expected result: EXIT_SUCCESS"
./sudoku $TEST_PATH/grid-02-size_4-pass.sku 1> /dev/null 2> $ERROR_FILE_TEST
test_exit_success $?

echo "* Check './sudoku grid-03-size_9.sku'"
echo "Expected result: EXIT_SUCCESS"
./sudoku $TEST_PATH/grid-03-size_9-pass.sku 1> /dev/null 2> $ERROR_FILE_TEST
test_exit_success $?

echo "* Check './sudoku grid-04-size_16.sku'"
echo "Expected result: EXIT_SUCCESS"
./sudoku $TEST_PATH/grid-04-size_16-pass.sku 1> /dev/null 2> $ERROR_FILE_TEST
test_exit_success $?

echo "* Check './sudoku grid-05-size_25.sku'"
echo "Expected result: EXIT_SUCCESS"
./sudoku $TEST_PATH/grid-05-size_25-pass.sku 1> /dev/null 2> $ERROR_FILE_TEST
test_exit_success $?

echo "* Check './sudoku grid-06-size_36.sku'"
echo "Expected result: EXIT_SUCCESS"
./sudoku $TEST_PATH/grid-06-size_36-pass.sku 1> /dev/null 2> $ERROR_FILE_TEST
test_exit_success $?

echo "* Check './sudoku grid-07-size_49.sku'"
echo "Expected result: EXIT_SUCCESS"
./sudoku $TEST_PATH/grid-07-size_49-pass.sku 1> /dev/null 2> $ERROR_FILE_TEST
test_exit_success $?

echo "* Check './sudoku grid-08-size_64.sku'"
echo "Expected result: EXIT_SUCCESS"
./sudoku $TEST_PATH/grid-08-size_64-pass.sku 1> /dev/null 2> $ERROR_FILE_TEST
test_exit_success $?

echo "* Check './sudoku grid-09-unknown_char_l1_c1-fail.sku'"
echo "Expected result: EXIT_FAILURE and stderr is written"
./sudoku $TEST_PATH/grid-09-unknown_char_l1_c1-fail.sku 1> /dev/null 2> $ERROR_FILE_TEST
test_exit_failure $?

echo "* Check './sudoku  grid-10-unknown_char_l7_c12-fail.sku'"
echo "Expected result: EXIT_FAILURE and stderr is written"
./sudoku $TEST_PATH/grid-10-unknown_char_l7_c12-fail.sku 1> /dev/null 2> $ERROR_FILE_TEST
#test if file was create ?
test_exit_failure $?

echo "* Check './sudoku  grid-11-missing_char_l9-fail.sku'"
echo "Expected result: EXIT_FAILURE and stderr is written"
./sudoku $TEST_PATH/grid-11-missing_char_l9-fail.sku 1> /dev/null 2> $ERROR_FILE_TEST
test_exit_failure $?

echo "* Check './sudoku  grid-12-extra_char_l10-fail.sku'"
echo "Expected result: EXIT_FAILURE and stderr is written"
./sudoku $TEST_PATH/grid-12-extra_char_l10-fail.sku 1> /dev/null 2> $ERROR_FILE_TEST
#test if file was create ?
test_exit_failure $?

echo "* Check './sudoku  grid-13-missing_line-fail.sku'"
echo "Expected result: EXIT_FAILURE and stderr is written"
./sudoku $TEST_PATH/grid-13-missing_line-fail.sku 1> /dev/null 2> $ERROR_FILE_TEST
#test if file was create ?
test_exit_failure $?

echo "* Check './sudoku  grid-14-extra_line-fail.sku'"
echo "Expected result: EXIT_FAILURE and stderr is written"
./sudoku $TEST_PATH/grid-14-extra_line-fail.sku 1> /dev/null 2> $ERROR_FILE_TEST
#test if file was create ?
test_exit_failure $?

echo "* Check './sudoku grid-15-empty_line.sku'"
echo "Expected result: EXIT_SUCCESS"
./sudoku $TEST_PATH/grid-15-empty_line-pass.sku 1> /dev/null 2> $ERROR_FILE_TEST
test_exit_success $?

echo "* Check './sudoku grid-16-whole_line_commented_out.sku'"
echo "Expected result: EXIT_SUCCESS"
./sudoku $TEST_PATH/grid-16-whole_line_commented_out-pass.sku 1> /dev/null 2> $ERROR_FILE_TEST
test_exit_success $?

echo "* Check './sudoku grid-17-end_with_EOF_no_endofline.sku'"
echo "Expected result: EXIT_SUCCESS"
./sudoku $TEST_PATH/grid-17-end_with_EOF_no_endofline-pass.sku 1> /dev/null 2> $ERROR_FILE_TEST
test_exit_success $?

echo "* Check './sudoku grid-18-size_1_end_with_EOF_no_endofline.sku'"
echo "Expected result: EXIT_SUCCESS"
./sudoku $TEST_PATH/grid-18-size_1_end_with_EOF_no_endofline-pass.sku 1> /dev/null 2> $ERROR_FILE_TEST
test_exit_success $?

echo "* Check './sudoku grid-19-start_with_comment_and_empty_line.sku'"
echo "Expected result: EXIT_SUCCESS"
./sudoku $TEST_PATH/grid-19-start_with_comment_and_empty_line-pass.sku 1> /dev/null 2> $ERROR_FILE_TEST
test_exit_success $?

echo "* Check './sudoku  grid-20-size_17-fail.sku'"
echo "Expected result: EXIT_FAILURE and stderr is written"
./sudoku  $TEST_PATH/grid-20-size_17-fail.sku 1> /dev/null 2> $ERROR_FILE_TEST
#test if file was create ?
test_exit_failure $?

echo "* Check './sudoku  grid-21-commented_char_l4-fail.sku'"
echo "Expected result: EXIT_FAILURE and stderr is written"
./sudoku  $TEST_PATH/grid-21-commented_char_l4-fail.sku 1> /dev/null 2> $ERROR_FILE_TEST
#test if file was create ?
test_exit_failure $?

echo "* Check './sudoku  grid-22-comment_at_last_line.sku'"
echo "Expected result: EXIT_FAILURE and stderr is written"
./sudoku  $TEST_PATH/grid-22-comment_at_last_line-pass.sku 1> /dev/null 2> $ERROR_FILE_TEST
test_exit_success $?

echo "* Check './sudoku  grid-23-missing_char_with_EOF_no_endofline-fail.sku'"
echo "Expected result: EXIT_FAILURE and stderr is written"
./sudoku  $TEST_PATH/grid-23-missing_char_with_EOF_no_endofline-fail.sku 1> /dev/null 2> $ERROR_FILE_TEST
#test if file was create ?
test_exit_failure $?

echo "* Check './sudoku  grid-24-only_a_comment_no_grid-fail.sku'"
echo "Expected result: EXIT_FAILURE and stderr is written"
./sudoku  $TEST_PATH/grid-24-only_a_comment_no_grid-fail.sku 1> /dev/null 2> $ERROR_FILE_TEST
#test if file was create ?
test_exit_failure $?

echo "* Check './sudoku  grid-25-only_a_comment_grid_end_with_no_endofline_before_EOF-fail.sku'"
echo "Expected result: EXIT_FAILURE and stderr is written"
./sudoku  $TEST_PATH/grid-25-only_a_comment_grid_end_with_no_endofline_before_EOF-fail 1> /dev/null 2> $ERROR_FILE_TEST
#test if file was create ?
test_exit_failure $?

echo "* Check './sudoku grid-26-random_number_of_spaces_tabs.sku'"
echo "Expected result: EXIT_SUCCESS"
./sudoku $TEST_PATH/grid-26-random_number_of_spaces_tabs-pass.sku 1> /dev/null 2> $ERROR_FILE_TEST
test_exit_success $?

echo "* Check './sudoku grid-27-wrong_char_on_first_line-fail.sku'"
echo "Expected result: EXIT_FAILURE"
./sudoku $TEST_PATH/grid-27-wrong_char_on_first_line-fail.sku 1> /dev/null 2> $ERROR_FILE_TEST
test_exit_failure $?

echo "* Check './sudoku grid-28-size_1_with_EOF_no_endofline_and_wrong_char-fail.sku"
echo "Expected result: EXIT FAILURE"
./sudoku $TEST_PATH/grid-28-size_1_with_EOF_no_endofline_and_wrong_char-fail.sku 1> /dev/null 2> $ERROR_FILE_TEST
test_exit_failure $?

echo "* Check './sudoku grid-29-commented_valid_line-pass.sku"
echo "Expected result: EXIT FAILURE"
./sudoku $TEST_PATH/grid-29-commented_valid_line-pass.sku 1> /dev/null 2> $ERROR_FILE_TEST
test_exit_success $?

echo "* Check './sudoku grid-02-size_4.sku  grid-03-size_9.sku'"
echo "Expected result: EXIT_SUCCESS"
./sudoku $TEST_PATH/grid-02-size_4-pass.sku $TEST_PATH/grid-03-size_9-pass.sku 1> /dev/null 2> $ERROR_FILE_TEST
test_exit_success $?

echo "* Check './sudoku  grid-02-size_4.sku  grid-09-unknown_char_l9-fail.sku'"
echo "Expected result: EXIT_FAILURE and stderr is written"
./sudoku  $TEST_PATH/grid-02-size_4-pass.sku $TEST_PATH/grid-10-unknown_char_l7_c12-fail.sku 1> /dev/null 2> $ERROR_FILE_TEST
#test if file was create ?
test_exit_failure $?

echo "* Check './sudoku  grid-09-unknown_char_l9-fail.sku  grid-02-size_4.sku '"
echo "Expected result: EXIT_FAILURE and stderr is written"
./sudoku  $TEST_PATH/grid-09-unknown_char_l1_c1-fail.sku $TEST_PATH/grid-02-size_4-pass.sku 1> /dev/null 2> $ERROR_FILE_TEST
#test if file was create ?
test_exit_failure $?

echo "* Check './sudoku grid-28-size_1_with_EOF_no_endofline_and_wrong_char-fail.sku"
echo "Expected result: EXIT FAILURE"
./sudoku $TEST_PATH/grid-28-size_1_with_EOF_no_endofline_and_wrong_char-fail.sku 1> /dev/null 2> $ERROR_FILE_TEST
test_exit_failure $?


echo -e "-------- ( Grid Solver ) --------\n"
#- grid-solver/grid-01x01-01.sku
#- grid-solver/grid-04x04-01.sku
#- grid-solver/grid-04x04-02.sku
#- grid-solver/grid-04x04-03.sku
#- grid-solver/grid-04x04-04.sku
#- grid-solver/grid-49x49-01.sku
#- grid-solver/grid-49x49-02.sku

#- grid-solver/grid-36x36-03-inconsistent.sku
#- grid-solver/grid-49x49-03-inconsistent.sku
#- grid-solver/grid-16x16-06-twice_char_8_in_line-inconsistent.sku
#- grid-solver/grid-16x16-07-twice_char_A_in_columns-inconsistent.sku
#- grid-solver/grid-16x16-08-twice_char_B_in_a_block-inconsistent.sku

echo "* Check './sudoku  grid-01x01-01.sku"
echo "Expected result: EXIT_SUCCESS"
#./sudoku $TEST_PATH2/grid-01x01-01.sku 1> /dev/null 2> $ERROR_FILE_TEST
#test_exit_success $?

echo "* Check './sudoku  grid-04x04-01.sku"
echo "Expected result: EXIT_SUCCESS"
#./sudoku $TEST_PATH2/grid-04x04-01.sku 1> /dev/null 2> $ERROR_FILE_TEST
#test_exit_success $?

echo "* Check './sudoku  grid-04x04-02.sku"
echo "Expected result: EXIT_SUCCESS"
#./sudoku $TEST_PATH2/grid-04x04-02.sku 1> /dev/null 2> $ERROR_FILE_TEST
#test_exit_success $?

echo "* Check './sudoku  grid-04x04-03.sku"
echo "Expected result: EXIT_SUCCESS"
#./sudoku $TEST_PATH2/grid-04x04-03.sku 1> /dev/null 2> $ERROR_FILE_TEST
#test_exit_success $?

echo "* Check './sudoku  grid-04x04-04.sku"
echo "Expected result: EXIT_SUCCESS"
#./sudoku $TEST_PATH2/grid-04x04-04.sku 1> /dev/null 2> $ERROR_FILE_TEST
#test_exit_success $?

echo "* Check './sudoku  grid-16x16-06-twice_char_8_in_line-inconsistent.sku '"
echo "Expected result: EXIT_FAILURE and stderr is written"
#./sudoku  $TEST_PATH2/grid-16x16-06-twice_char_8_in_line-inconsistent.sku 1> /dev/null 2> $ERROR_FILE_TEST
#test if file was create ?
#test_exit_failure $?

echo "* Check './sudoku  grid-16x16-07-twice_char_A_in_columns-inconsistent.sku '"
echo "Expected result: EXIT_FAILURE and stderr is written"
#./sudoku  $TEST_PATH2/grid-16x16-07-twice_char_A_in_columns-inconsistent.sku 1> /dev/null 2> $ERROR_FILE_TEST
#test if file was create ?
#test_exit_failure $?

echo "* Check './sudoku  grid-16x16-08-twice_char_B_in_a_block-inconsistent.sku '"
echo "Expected result: EXIT_FAILURE and stderr is written"
#./sudoku  $TEST_PATH2/grid-16x16-08-twice_char_B_in_a_block-inconsistent.sku 1> /dev/null 2> $ERROR_FILE_TEST
#test if file was create ?
#test_exit_failure $?

echo "* Check './sudoku  grid-36x36-03-inconsistent.sku.sku '"
echo "Expected result: EXIT_FAILURE and stderr is written"
#./sudoku  $TEST_PATH2/grid-36x36-03-inconsistent.sku 1> /dev/null 2> $ERROR_FILE_TEST
#test if file was create ?
#test_exit_failure $?

echo "* Check './sudoku  grid-49x49-03-inconsistent.sku.sku '"
echo "Expected result: EXIT_FAILURE and stderr is written"
#./sudoku  $TEST_PATH2/grid-49x49-03-inconsistent.sku 1> /dev/null 2> $ERROR_FILE_TEST
#test if file was create ?
#test_exit_failure $?


echo "* Check './sudoku  grid-04x04-03.sku grid-04x04-01.sku grid-04x04-02.sku '"
echo "Expected result: EXIT_SUCCESS"
#./sudoku  $TEST_PATH2/grid-04x04-03.sku  $TEST_PATH2/grid-04x04-01.sku $TEST_PATH2/grid-04x04-02.sku 1> /dev/null 2> $ERROR_FILE_TEST
#test_exit_success $?

echo "* Check './sudoku  grid-04x04-03.sku grid-16x16-06-twice_char_8_in_line-inconsistent.sku grid-04x04-02.sku '"
echo "Expected result: EXIT_FAILURE and stderr is written"
#./sudoku  $TEST_PATH2/grid-04x04-03.sku $TEST_PATH2/grid-16x16-06-twice_char_8_in_line-inconsistent.sku $TEST_PATH2/grid-04x04-02.sku 1> /dev/null 2> $ERROR_FILE_TEST
#test if file was create ?
#test_exit_failure $?

echo "* Check './sudoku  grid-49x49-01.sku grid-49x49-02.sku grid-49x49-03-inconsistent.sku '"
echo "Expected result: EXIT_FAILURE and stderr is written"
#./sudoku  $TEST_PATH2/grid-49x49-01.sku $TEST_PATH2/grid-49x49-02.sku $TEST_PATH2/grid-49x49-03-inconsistent.sku 1> /dev/null 2> $ERROR_FILE_TEST
#test if file was create ?
#test_exit_failure $?



echo -e "-------- ( Checking memory ) --------\n"

echo "* Check 'valgrind ./sudoku -h'"
valgrind --error-exitcode=$VAL_FAILURE --leak-check=full --log-file=$PREVIOUS_PWD/log.txt ./sudoku -h 1> /dev/null 2> $ERROR_FILE_TEST
test_valgrind $?
cat $PREVIOUS_PWD/log.txt
echo -e "\n"

echo "* Check 'valgrind ./sudoku grid-01-size_1.sku'"
valgrind --error-exitcode=$VAL_FAILURE --leak-check=full --log-file=$PREVIOUS_PWD/log.txt ./sudoku $TEST_PATH/grid-01-size_1.sku 1> /dev/null 2> $ERROR_FILE_TEST
test_valgrind $?
cat $PREVIOUS_PWD/log.txt
echo -e "\n"

echo "* Check 'valgrind ./sudoku grid-05-size_25.sku'"
valgrind --error-exitcode=$VAL_FAILURE --leak-check=full --log-file=$PREVIOUS_PWD/log.txt ./sudoku $TEST_PATH/grid-05-size_25.sku 1> /dev/null 2> $ERROR_FILE_TEST
test_valgrind $?
cat $PREVIOUS_PWD/log.txt
echo -e "\n"

echo "* Check 'valgrind ./sudoku grid-08-size_64.sku'"
valgrind --error-exitcode=$VAL_FAILURE --leak-check=full --log-file=$PREVIOUS_PWD/log.txt ./sudoku $TEST_PATH/grid-08-size_64.sku 1> /dev/null 2> $ERROR_FILE_TEST
test_valgrind $?
cat $PREVIOUS_PWD/log.txt
echo -e "\n"

echo "* Check 'valgrind ./sudoku grid-14-empty_line.sku'"
valgrind --error-exitcode=$VAL_FAILURE --leak-check=full --log-file=$PREVIOUS_PWD/log.txt ./sudoku $TEST_PATH/grid-14-empty_line.sku 1> /dev/null 2> $ERROR_FILE_TEST
test_valgrind $?
cat $PREVIOUS_PWD/log.txt
echo -e "\n"

echo "* Check 'valgrind ./sudoku grid-16-end_with_EOF_no_endofline.sku'"
valgrind --error-exitcode=$VAL_FAILURE --leak-check=full --log-file=$PREVIOUS_PWD/log.txt ./sudoku $TEST_PATH/grid-16-end_with_EOF_no_endofline.sku 1> /dev/null 2> $ERROR_FILE_TEST
test_valgrind $?
cat $PREVIOUS_PWD/log.txt
echo -e "\n"

echo "* Check 'valgrind ./sudoku grid-25-random_number_of_spaces_tabs.sku'"
valgrind --error-exitcode=$VAL_FAILURE --leak-check=full --log-file=$PREVIOUS_PWD/log.txt ./sudoku $TEST_PATH/grid-25-random_number_of_spaces_tabs.sku 1> /dev/null 2> $ERROR_FILE_TEST
test_valgrind $?
cat $PREVIOUS_PWD/log.txt
echo -e "\n"

echo "* Check 'valgrind ./sudoku grid-03-size_9.sku  grid-05-size_25.sku'"
valgrind --error-exitcode=$VAL_FAILURE --leak-check=full --log-file=$PREVIOUS_PWD/log.txt ./sudoku $TEST_PATH/grid-03-size_9.sku $TEST_PATH/grid-05-size_25.sku 1> /dev/null 2> $ERROR_FILE_TEST
test_valgrind $?
cat $PREVIOUS_PWD/log.txt
echo -e "\n"

echo "* Check 'valgrind ./sudoku grid-04x04-03.sku'"
valgrind --error-exitcode=$VAL_FAILURE --leak-check=full --log-file=$PREVIOUS_PWD/log.txt ./sudoku $TEST_PATH2/grid-04x04-03.sku 1> /dev/null 2> $ERROR_FILE_TEST
test_valgrind $?
cat $PREVIOUS_PWD/log.txt
echo -e "\n"

echo "* Check 'valgrind ./sudoku grid-16x16-06-twice_char_8_in_line-inconsistent.sku'"
valgrind --error-exitcode=$VAL_FAILURE --leak-check=full --log-file=$PREVIOUS_PWD/log.txt ./sudoku $TEST_PATH2/grid-16x16-06-twice_char_8_in_line-inconsistent.sku 1> /dev/null 2> $ERROR_FILE_TEST
test_valgrind $?
cat $PREVIOUS_PWD/log.txt
echo -e "\n"

echo "* Check 'valgrind ./sudoku grid-04x04-02.sku  grid-04x04-03.sku'"
valgrind --error-exitcode=$VAL_FAILURE --leak-check=full --log-file=$PREVIOUS_PWD/log.txt ./sudoku $TEST_PATH2/grid-04x04-02.sku  $TEST_PATH2/grid-04x04-03.sku 1> /dev/null 2> $ERROR_FILE_TEST
test_valgrind $?
cat $PREVIOUS_PWD/log.txt
echo -e "\n"

echo "* Check 'valgrind ./sudoku grid-04x04-01.sku  grid-16x16-07-twice_char_A_in_columns-inconsistent.sku grid-04x04-03.sku'"
valgrind --error-exitcode=$VAL_FAILURE --leak-check=full --log-file=$PREVIOUS_PWD/log.txt ./sudoku $TEST_PATH2/grid-04x04-01.sku  $TEST_PATH2/grid-16x16-07-twice_char_A_in_columns-inconsistent.sku $TEST_PATH2/grid-04x04-03.sku 1> /dev/null 2> $ERROR_FILE_TEST
test_valgrind $?
cat $PREVIOUS_PWD/log.txt
echo -e "\n"





end_time="$(date -u +%s.%N)"
elapsed="$(bc <<<"$end_time-$start_time")"

echo -e "------- ( Final result ) --------\n"
echo "Passed: $SUCC ;Failed: $ERR"
echo "Total of $elapsed seconds elapsed for process"
cd $PREVIOUS_PWD
