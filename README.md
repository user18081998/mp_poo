# mp_poo
le travail est realise par **Lobna Channoufi** et **Ahmed Mustapha Ghanmi** de la **1AInfo2**

**COMMANDS** :  new, search, addFile, printFileList
**options** :   -it x   : use x as index
            -ai x   : use x as analyzer
            -s q    : search query
            -f f    : argument f is a file
            -d d    : argument d is a directory

**exemple** :

<pre><font color="#8AE234"><b>ahmed2@shx</b></font>:<font color="#729FCF"><b>~/Desktop/mp_poo</b></font>$ ./main.out new -d /home/ahmed2/Desktop/corpus/petit -at AnalyzerWF -it IndexUmap -s tinnitus

command : new
index type : IndexUmap
analyser type : AnalyzerWF
file : 
directory : /home/ahmed2/Desktop/corpus/petit
search words : tinnitus 

indexing directory &apos;/home/ahmed2/Desktop/corpus/petit&apos;...
making a search query for &apos;tinnitus &apos; ...
9.19928 /home/ahmed2/Desktop/corpus/petit/14815.txt

exporting index ...
changes will be saved to /home/ahmed2/Desktop/mp_poo/engine.txt

Elapsed time: 5.76759 s
<font color="#8AE234"><b>ahmed2@shx</b></font>:<font color="#729FCF"><b>~/Desktop/mp_poo</b></font>$ ./main.out search -s the

command : search
index type : IndexUmap
analyser type : AnalyzerATF
file : 
directory : 
search words : the 

making a search query for &apos;the &apos; ...

exporting index ...
changes will be saved to /home/ahmed2/Desktop/mp_poo/engine.txt

Elapsed time: 2.40886 s
<font color="#8AE234"><b>ahmed2@shx</b></font>:<font color="#729FCF"><b>~/Desktop/mp_poo</b></font>$ ./main.out new -at ksjdhfjf -d /home/ahmed2/Desktop/corpus/petit
ERROR : analyzer type &apos;ksjdhfjf&apos; unrecognized!
command : new
index type : IndexUmap
analyser type : AnalyzerATF
file : 
directory : /home/ahmed2/Desktop/corpus/petit
search words : 

</pre>