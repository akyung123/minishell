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

    # 1. Bash 실행 및 에러 메시지 접두사 제거
    bash $TMP/input.sh > $TMP/bash_out 2> $TMP/bash_err
    BASH_STATUS=$?
    # 'bash: line 1: ' 부분 제거
    sed -i 's/^.*line [0-9]*: //' $TMP/bash_err 2>/dev/null || sed 's/^.*line [0-9]*: //' $TMP/bash_err > $TMP/bash_err_tmp && mv $TMP/bash_err_tmp $TMP/bash_err

    # 2. Minishell 실행
    $MINISHELL < $TMP/input.sh > $TMP/mini_out 2> $TMP/mini_err
    MINI_STATUS=$?

    # --- Minishell 출력물 정제 ---
    # 1. 프롬프트 및 exit 찌꺼기 제거 (STDOUT)
    grep -v "^minishell: " $TMP/mini_out | sed 's/minishell: exit//g' > $TMP/mini_out_tmp
    mv $TMP/mini_out_tmp $TMP/mini_out

    # 2. 'minishell: ' 접두사 제거 (STDERR)
    sed 's/^minishell: //' $TMP/mini_err > $TMP/mini_err_tmp
    mv $TMP/mini_err_tmp $TMP/mini_err

    # 3. 비교
    diff $TMP/bash_out $TMP/mini_out >/dev/null
    OUT=$?
    diff $TMP/bash_err $TMP/mini_err >/dev/null
    ERR=$?

    if [ $OUT -eq 0 ] && [ $ERR -eq 0 ] && [ $BASH_STATUS -eq $MINI_STATUS ]; then
        echo -e "\033[32m[OK]\033[0m ($COUNT) $NAME"
        PASS=$((PASS+1))
    else
        echo -e "\033[31m[FAIL]\033[0m ($COUNT) $NAME"
        FAIL=$((FAIL+1))

        echo "---- CMD ----"
        echo -e "$CMD"

        echo "---- EXPECT STDOUT (Bash) ----"
        cat $TMP/bash_out
        echo "---- YOUR STDOUT (Minishell) ----"
        cat $TMP/mini_out

        echo "---- EXPECT STDERR (Bash) ----"
        cat $TMP/bash_err
        echo "---- YOUR STDERR (Minishell) ----"
        cat $TMP/mini_err

        echo "---- EXIT CODE ----"
        echo "bash: $BASH_STATUS | minishell: $MINI_STATUS"
        echo "-------------------"
        echo ""
    fi
}

echo "============================="
echo "🔥 MINISHELL 100 CASES TEST"
echo "============================="

# 1. ECHO & BASIC (1-15)
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

# 2. QUOTES (16-30)
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
run_test "Quote $ expansion" 'echo "$USER"'
run_test "Single $ expansion" "echo '\$USER'"
run_test "Escaped quote?" "echo \"\\\"\"" # 과제 범위 외이나 테스트
run_test "Multiple empty quotes" "echo ''\"\"''"
run_test "Quote inside word" "echo hel'lo' wor\"ld\""

# 3. EXPANSION & $? (31-45)
run_test "Expand USER" "echo \$USER"
run_test "Expand PWD" "echo \$PWD"
run_test "Expand SHLVL" "echo \$SHLVL"
run_test "Expand non-existent" "echo \$NON_EXISTENT_VAR"
run_test "Expand with text" "echo \$USER_is_here"
run_test "Exit status 0" "ls\necho \$?"
run_test "Exit status cmd not found" "nosuchcmd\necho \$?"
run_test "Exit status fail" "ls /nosuchfile\necho \$?"
run_test "Multiple $?" "ls\necho \$?\nls /fail\necho \$?"
run_test "Expand in double quotes" "echo \"hello \$USER\""
run_test "Expand $? in quotes" "echo \"exit: \$?\""
run_test "Expansion concat" "echo \$USER\$TERM"
run_test "Expansion with special char" "echo \$USER!"
run_test "Expansion $0?" "echo \$0"
run_test "Expansion $$?" "echo \$\$"

# 4. BUILT-INS (46-65)
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
run_test "CD minus" "cd -" # 지원 여부에 따라 결과 다를 수 있음
run_test "Export append?" "export VAR=a\nexport VAR+=b\necho \$VAR"

# 5. REDIRECTIONS (66-80)
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
run_test "Redir Append multiple" "echo 1 >> f1\necho 2 >> f1\ncat f1"
run_test "LS redir" "ls > f1\ngrep f1 f1"
run_test "Here-doc empty" "cat << EOF\nEOF"
run_test "Redir filename with quote" "echo test > 'file with space'\ncat 'file with space'"
run_test "Redir In/Out complex" "cat < f1 > f2 << EOF\nheredoc\nEOF"

# 6. PIPES (81-90)
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

# 7. BONUS: LOGIC & WILDCARD (91-100)
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

echo ""
echo "============================="
echo "📊 RESULT"
echo "TOTAL: $COUNT | PASS: $PASS | FAIL: $FAIL"
echo "============================="

rm -rf $TMP f1 f2