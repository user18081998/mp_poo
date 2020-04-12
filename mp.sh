#!/bin/bash
# mp.sh chemin_absolu_pour_repertoire_corpus
rm main/temp.txt || true
touch main/temp.txt
ls $1 >> main/temp.txt
v2=$PWD
v2+="/main"
# main chemin_temp.txt chemin_corpus
main/main "$v2" $1
