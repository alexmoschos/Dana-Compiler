#/usr/bin/bash/
for i in "$@"
do
	./do.sh < ${i} > /dev/null
	if [ $? -ne 0 ];
	then
		echo "Error at testcase $i"
        else
        	echo "Success at testcase $i"
	fi
done
