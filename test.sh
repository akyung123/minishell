#!/usr/bin/env bash

# 설정
MINISHELL=./minishell
TMP=./test_tmp
mkdir -p $TMP
PASS=0
FAIL=0
COUNT=0

run_test() {
    NAME="$1"
    CMD="$2"
    COUNT=$((COUNT+1))

    # 입력 파일 생성
    printf "%b\n" "$CMD" > $TMP/input.sh

    # 1. Bash 실행
    bash $TMP/input.sh > $TMP/bash_out 2> $TMP/bash_err
    BASH_STATUS=$?
    if [ -s $TMP/bash_err ]; then
        sed 's/^.*line [0-9]*: //' $TMP/bash_err > $TMP/bash_err_tmp
        mv $TMP/bash_err_tmp $TMP/bash_err
    fi

    # 2. Minishell 실행 (Valgrind)
    rm -f $TMP/valgrind_*.log 
    valgrind --leak-check=full \
             --show-leak-kinds=all \
             --track-fds=yes \
             --trace-children=yes \
             --trace-children-skip="/usr/bin/*,/bin/*" \
             --log-file=$TMP/valgrind_%p.log \
             $MINISHELL < $TMP/input.sh > $TMP/mini_out 2> $TMP/mini_err
    MINI_STATUS=$?

    # STDOUT/ERR 정제
    # run_test 함수 내 정제 부분 수정
    # Minishell 출력 정제
    grep -v "^minishell: " $TMP/mini_out | sed 's/minishell: exit//g' | grep -v "test_tmp" > $TMP/mini_out_tmp
    mv $TMP/mini_out_tmp $TMP/mini_out
    
    if [ -s $TMP/mini_err ]; then
        sed 's/^minishell: //' $TMP/mini_err > $TMP/mini_err_tmp
        mv $TMP/mini_err_tmp $TMP/mini_err
    fi

    # Bash 출력 정제 (Minishell과 동일하게 test_tmp 줄 제거)
    grep -v "test_tmp" $TMP/bash_out > $TMP/bash_out_tmp
    mv $TMP/bash_out_tmp $TMP/bash_out

    # 3. Valgrind 로그 분석
    LEAK_FAIL=0
    V_MSG=""

    # [1] 메모리 누수 체크
    if grep -E "definitely lost:|indirectly lost:" $TMP/valgrind_*.log | grep -v "0 bytes" > /dev/null; then
        LEAK_FAIL=1
        V_MSG="$V_MSG [Mem Leak]"
    fi

    # [2] FD 누수 체크 (VS Code + 테스트 임시 파일 모두 완벽 필터링)
    FD_LEAKS=$(grep -E "Open file descriptor [4-9][0-9]?( |:)" $TMP/valgrind_*.log | \
               grep -vE "mini_out|mini_err|valgrind_|input.sh|v8_context_snapshot|/usr/share/code")
    
    if [ ! -z "$FD_LEAKS" ]; then
        LEAK_FAIL=1
        V_MSG="$V_MSG [FD Leak]"
    fi

    # [3] 잘못된 FD 접근
    if grep -iE "invalid file descriptor|Bad file descriptor" $TMP/valgrind_*.log > /dev/null; then
        LEAK_FAIL=1
        V_MSG="$V_MSG [Bad FD]"
    fi

    # 4. 결과 비교
    diff -Z $TMP/bash_out $TMP/mini_out >/dev/null
    OUT=$?
    diff -Z $TMP/bash_err $TMP/mini_err >/dev/null
    ERR=$?

    if [ $OUT -eq 0 ] && [ $ERR -eq 0 ] && [ $BASH_STATUS -eq $MINI_STATUS ] && [ $LEAK_FAIL -eq 0 ]; then
        echo -e "\033[32m[OK]\033[0m ($COUNT) $NAME"
        PASS=$((PASS+1))
    else
        echo -e "\033[31m[FAIL]\033[0m ($COUNT) $NAME \033[33m$V_MSG\033[0m"
        FAIL=$((FAIL+1))
        echo "---- CMD ----"
        echo -e "$CMD"
        
        # 복구된 부분: Bash vs Minishell 결과 상세 비교
        if [ $OUT -ne 0 ] || [ $ERR -ne 0 ] || [ $BASH_STATUS -ne $MINI_STATUS ]; then
            echo "---- EXPECT STDOUT (Bash) ----"
            cat -e $TMP/bash_out
            echo "---- YOUR STDOUT (Minishell) ----"
            cat -e $TMP/mini_out
            
            echo "---- EXPECT STDERR (Bash) ----"
            cat -e $TMP/bash_err
            echo "---- YOUR STDERR (Minishell) ----"
            cat -e $TMP/mini_err
            
            echo "---- EXIT CODE ----"
            echo "bash: $BASH_STATUS | minishell: $MINI_STATUS"
        fi
        
        # Valgrind 누수 상세 리포트
        if [ $LEAK_FAIL -ne 0 ]; then
            echo "---- VALGRIND REPORT (Cleaned) ----"
            grep -E "definitely lost:|indirectly lost:|Invalid read|Invalid write" $TMP/valgrind_*.log | grep -v "0 bytes"
            if [ ! -z "$FD_LEAKS" ]; then
                echo "$FD_LEAKS"
            fi
        fi
        echo "-------------------"
    fi
}

echo "============================="
echo "🔥 MINISHELL COMPREHENSIVE TEST (with Valgrind)"
echo "============================="

# 1. ECHO & BASIC
run_test "Echo hello" "echo hello"
run_test "Echo -n" "echo -n hello"
run_test "Echo -n-n" "echo -n -n hello"
run_test "Echo empty" "echo"
run_test "Echo spaces" "echo      hello      world"
run_test "Basic ls" "ls"
run_test "Basic pwd" "pwd"
run_test "Ls -la" "ls -la"
run_test "Echo arg with space" "echo 'a   b'"
run_test "Echo -nnnn" "echo -nnnn hello"
run_test "Echo -n -a" "echo -n -a hello"
run_test "Echo multiple" "echo a b c d"
run_test "Echo -n empty" "echo -n"
run_test "Double echo" "echo hello\necho world"
run_test "Basic /bin/ls" "/bin/ls"

# 2. QUOTES
run_test "Single Quote" "echo 'hello world'"
run_test "Double Quote" 'echo "hello world"'
run_test "Mixed 1" "echo \"'hello'\""
run_test "Mixed 2" "echo '\"hello\"'"
run_test "Nested quotes" "echo \"'\"'\"'\""
run_test "Empty Single" "echo ''"
run_test "Empty Double" 'echo ""'
run_test "Quotes with spaces" "echo '  abc  '"
run_test "Quotes in path" "ls '.'"
run_test "Quote concat" "echo 'hello'\"world\""
run_test "Quote \$ expansion" 'echo "$USER"'
run_test "Single \$ expansion" "echo '\$USER'"
run_test "Escaped quote?" "echo \"\\\"\""
run_test "Multiple empty quotes" "echo ''\"\"''"
run_test "Quote inside word" "echo hel'lo' wor\"ld\""

# 3. EXPANSION & $?
run_test "Expand USER" "echo \$USER"
run_test "Expand PWD" "echo \$PWD"
run_test "Expand SHLVL" "echo \$SHLVL"
run_test "Expand non-existent" "echo \$NON_EXISTENT_VAR"
run_test "Expand with text" "echo \$USER_is_here"
run_test "Exit status 0" "ls\necho \$?"
run_test "Exit status cmd not found" "nosuchcmd\necho \$?"
run_test "Exit status fail" "ls /nosuchfile\necho \$?"
run_test "Multiple \$?" "ls\necho \$?\nls /fail\necho \$?"
run_test "Expand in double quotes" "echo \"hello \$USER\""
run_test "Expand \$? in quotes" "echo \"exit: \$?\""
run_test "Expansion concat" "echo \$USER\$TERM"
run_test "Expansion with special char" "echo \$USER!"
run_test "Expansion \$0?" "echo \$0"
run_test "Expansion \$\$?" "echo \$\$"

# 4. BUILT-INS
run_test "CD .." "cd ..\npwd"
run_test "CD /" "cd /\npwd"
run_test "CD home" "cd\npwd"
run_test "CD relative" "mkdir -p test_dir\ncd test_dir\npwd"
run_test "Export simple" "export MYVAR=42\necho \$MYVAR"
run_test "Export update" "export MYVAR=1\nexport MYVAR=2\necho \$MYVAR"
run_test "Export multiple" "export A=1 B=2 C=3\necho \$A\$B\$C"
run_test "Unset simple" "export VAR=test\nunset VAR\necho \$VAR"
run_test "Unset multiple" "export A=1 B=2\nunset A B\necho \$A\$B"
run_test "Env" "env | grep USER"
run_test "Export no arg" "export | head -n 5"
run_test "Exit 0" "exit"
run_test "Exit 42" "exit 42"
run_test "Exit non-numeric" "exit abc"
run_test "PWD with args" "pwd hello"
run_test "CD non-existent" "cd /blahblah"
run_test "Export invalid key" "export 123VAR=test"
run_test "Unset non-existent" "unset NON_EXISTENT"
run_test "CD minus" "cd -" 
run_test "Export append?" "export VAR=a\nexport VAR+=b\necho \$VAR"

# 5. REDIRECTIONS
run_test "Redir Out >" "echo hello > f1\ncat f1"
run_test "Redir Append >>" "echo a > f1\necho b >> f1\ncat f1"
run_test "Redir In <" "echo world > f1\ncat < f1"
run_test "Redir multi >" "echo test > f1 > f2\ncat f1\ncat f2"
run_test "Redir Out & In" "echo 42 > f1\ncat < f1 > f2\ncat f2"
run_test "Redir Error <" "cat < non_existent"
run_test "Redir Permission" "touch no_perm\nchmod 000 no_perm\necho test > no_perm"
run_test "Here-doc <<" "cat << EOF\nhello\nworld\nEOF"
run_test "Here-doc with quotes" "cat << 'EOF'\n\$USER\nEOF"
run_test "Redir with builtin" "echo hello > f1\ncd .. > f2\npwd"
run_test "Redir Append multiple" "echo 1 >> f1\necho 2 >> f1\ncat f1\nrm f1"
run_test "LS redir" "ls > f1\ngrep f1 f1"
run_test "Here-doc empty" "cat << EOF\nEOF"
run_test "Redir filename with quote" "echo test > 'file with space'\ncat 'file with space'"
run_test "Redir In/Out complex" "cat < f1 > f2 << EOF\nheredoc\nEOF"

# 6. PIPES
run_test "Pipe simple" "ls | grep test"
run_test "Pipe multiple" "echo hello | cat | cat | wc -c"
run_test "Pipe with builtin" "env | grep USER | wc -l"
run_test "Pipe exit code" "ls | nosuchcmd\necho \$?"
run_test "Pipe with redir" "ls | grep f > outfile\ncat outfile"
run_test "Pipe and heredoc" "cat << EOF | grep a\nabc\ndef\nEOF"
run_test "Pipe long" "echo a | echo b | echo c"
run_test "Pipe cat" "echo hello | cat"
run_test "Pipe grep" "ls -l | grep .c"
run_test "Pipe with export" "export A=1 | echo \$A"

# 7. BONUS: LOGIC & WILDCARD (기존)
run_test "Logic AND success" "ls && echo OK"
run_test "Logic AND fail" "ls /fail && echo OK"
run_test "Logic OR success" "ls || echo NO"
run_test "Logic OR fail" "ls /fail || echo OK"
run_test "Logic mix" "true && false || echo OK"
run_test "Wildcard *" "ls *.c"
run_test "Wildcard all" "echo *"
run_test "Wildcard in dir" "ls src/*"
run_test "Logic with parens" "(ls /fail || echo Handled) && echo Next"
run_test "Complex Logic" "ls && echo a || echo b && echo c"

# 8. EDGE & EVALUATION CASES
run_test "Empty input" " "
run_test "Tab input" "\t"
run_test "Echo invalid flag" "echo -nnnnnnnm hello"
run_test "Echo flag end" "echo hello -n"
run_test "Exit large number" "exit 999999999999999999999"
run_test "Quotes parsing edge" "e''\"\"c'h'\"o\" hi"
run_test "Quote expansion edge 1" "echo \"'\$USER'\""
run_test "Quote expansion edge 2" "echo '\"\$USER\"'"
run_test "Pipe invalid cmd status" "aaaaa | echo \$?"
run_test "Return value math" "ls /nofile\nexpr \$? + \$?"
run_test "PATH Unset" "unset PATH\nls"
run_test "PATH Export" "unset PATH\nexport PATH=/bin\nls"
run_test "Cmd execution via env" "export TEST=\"ls -a\"\n\$TEST"
run_test "Middle Redirection" "touch f2\nls > f1 -a < f2 -l\ncat f1"
run_test "Heredoc multiple redirects" "cat << EOF > f1 > f2\nhello\nEOF\ncat f1\ncat f2"

# 9. IFS (Internal Field Separator) CASES
run_test "IFS default behavior" "export VAR=\"a b   c\"\necho \$VAR"
run_test "IFS custom char split" "export IFS=\":\"\nexport VAR=\"a:b:c\"\necho \$VAR"
run_test "IFS ignore split inside quotes" "export IFS=\":\"\nexport VAR=\"a:b:c\"\necho \"\$VAR\""
run_test "IFS empty (no split)" "export IFS=\"\"\nexport VAR=\"a b c\"\necho \$VAR"
run_test "IFS unset (default behavior)" "unset IFS\nexport VAR=\"a b   c\"\necho \$VAR"
run_test "IFS with multiple variables" "export IFS=\"x\"\nexport VAR1=\"axb\"\nexport VAR2=\"cxd\"\necho \$VAR1 \$VAR2"
run_test "IFS command args split" "export IFS=\"/\"\nexport VAR=\"ls/-l\"\n\$VAR"

echo ""
echo "============================="
echo "📊 RESULT"
echo "TOTAL: $COUNT | PASS: $PASS | FAIL: $FAIL"
echo "============================="

# 테스트용 임시 파일 삭제
rm -rf $TMP f1 f2 no_perm test_dir 'file with space' outfile
