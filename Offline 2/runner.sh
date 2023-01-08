echo -n "" > results.txt 

for file in d-1*txt
do 
	solvers=("bt" "fc")
	variable_heuristics=("vah1" "vah2" "vah3" "vah4" "vah5")
	
	for slvr in "${solvers[@]}"
	do 
		for vah in "${variable_heuristics[@]}" 
		do 
			echo "running: \"./solver ${slvr} ${vah} ${1} ${file} >> results.txt\" "
			echo "running: \"./solver ${slvr} ${vah} ${1} ${file} >> results.txt\" " >> results.txt 
			./solver $slvr $vah $file $1 >> results.txt &
			pid=$!
			
			echo "Kill ?(y/n)"
			#read ans 
			#until read -t 0.5 ans
			#do 
			#	if ps -p $pid; then
			#		continue
			#	else break
			#	fi
			#done 
			while (ps -p $pid | grep $pid > /dev/null)
			do 
				read -rsn1 -t 0.5 ans 
				#echo $ans 
				if [[ $? -eq 0 && ($ans=="y") ]] ; then 
					kill -9 $pid 
				fi
			done 
			
			#if [[ "$ans" = y ]] ; then 
			#	kill -9 $pid 
			#fi
		done
	done 
done 
