#!/usr/bin/env bash

Program="./bin/Release/arger"
Input=("2+3+56+1.2" "2-1.69+124+0.69" "2*3*4*0.5*0.2" "2/2/0.5/1/0.01" "2+3-1(1+2)/3" "2(1+(1/2*0.5)-69+420/3)")
Answers=("62.2" "125.0" "2.4" "200.0" "4.0" "144.5")

for i in "${!Input[@]}"; do
    result=$("$Program" "${Input[$i]}")
    if [[ "$result" != "${Answers[$i]}" ]]; then
        tput setaf 1
        echo "${Input[$i]} = ${Answers[$i]} Failed [X]" 
        tput sgr 0
    else
        tput setaf 2
        echo "${Input[$i]} = ${Answers[$i]} Passed [✓]"
        tput sgr 0
    fi
done

