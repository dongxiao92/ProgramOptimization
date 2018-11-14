#!/bin/bash
data_dir=../data/

for f in $data_dir/*.list;
do
	basename=`basename "${f}" ".list"`
	log_name="./log/"$basename".log"
	echo $log_name
	./graph_coloring $f 2>&1 | tee $log_name
done
