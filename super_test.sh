#/usr/bin/bash/
for i in $(seq 1 10);
do
	./simple < test$i > /dev/null 
	if [ $? -ne 0 ];	
	then 
		echo "Error at testcase $i"
        else
        	echo "Success at testcase $i"
	fi
done
