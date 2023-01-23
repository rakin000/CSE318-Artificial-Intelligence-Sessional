echo -n "" > results.txt 


constructive_heuristics=("ld" "le" "rnd" "dsat"); 
penalty_strategy=("exp" "lin"); 
	
for ch in "${constructive_heuristics[@]}" 
do 
	for ps in "${penalty_strategy[@]}"
	do
		for file in ./Toronto/*
		do
			filename="${file%.*}" 
			echo "running: \"./solver ${ch} ${ps} 10000 ${filename} >> results.txt\" "
			echo "running: \"./solver ${ch} ${ps} 10000 ${filename} >> results.txt\" " >> results.txt
			./solver $ch $ps 10000 $filename >> results.txt &
			pid=$!
			
			echo "Kill ?(y/n)"
			while (ps -p $pid | grep $pid > /dev/null)
			do 
				read -rsn1 -t 0.5 ans 
				#echo $ans 
				if [[ $? -eq 0 && ($ans=="y") ]] ; then 
					kill -9 $pid 
				fi
			done 
			echo "" >> results.txt
			echo "============================================================================" >> results.txt
			echo "" >> results.txt 
		done
	done 
done 
