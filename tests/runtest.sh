#!/bin/bash
tno=$1
command=$2
#todo change command into various tests 
case $tno in
    1)
    echo "test $ sign with empty input"
    bash test.sh $tno.txt "" "$"
    ;;
    2)
    echo "test $ sign with line feed"
    bash test.sh $tno.txt "\n" "\$\$"
    ;;
    3)
    echo "test exit"
    bash testexit.sh
    ;;
    4)
    echo "test ls shell"
    bash test.sh $tno.txt "ls  myshell" "myshell"
    ;;
    5)
    echo "test ls she../README*"
    bash test.sh $tno.txt "ls  ../README*" "README"
    ;;
    6)
    echo "test tprog1"
    msg=$(tr -dc A-Za-z0-9 </dev/urandom | head -c 13;)
    bash test.sh $tno.txt "tprog1.sh $msg" $msg$msg
    ;;
    7)
    echo "test tprog2"
    msg=$(tr -dc A-Za-z0-9 </dev/urandom | head -c 13;)
    n=$((2 + $SRANDOM % 10))
    nmsg=""
    for (( i=1; i<=$n; i++ ))
    do
        nmsg=$nmsg$msg
    done

    bash test.sh $tno.txt "tprog2.sh $msg $n" $nmsg
    ;;
## writeup sadece current path aranmasi isteniyor,
## bu test gereksiz
#    8)
#    echo "test locate:"
#    cp tprog1.sh ../newprog.sh
#    msg=$(tr -dc A-Za-z0-9 </dev/urandom | head -c 13;)
#    bash test.sh $tno.txt "newprog.sh $msg" $msg$msg
#    ;;
    9)
    echo "test command not found"
    bash test.sh $tno.txt "hofff"  "not found"
    ;;
    10)
    echo "test log.txt"
    bash testlog.sh
    ;;
    *)
    ;;
esac