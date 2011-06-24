#!/bin/bash
if [ $# -ne 1 ]; then
	echo "Usage: $0 input.arff"
	exit 1
fi
. /usr/share/java-utils/java-functions
set_classpath "weka.jar"
set_classpath "libsvm.jar"
set_classpath "java_cup.jar"
set_classpath "postgresql-jdbc"
set_classpath "hsqldb"
set_classpath "mysql-connector-java"
export CLASSPATH="$CLASSPATH:/usr/lib/javasqlite/sqlite.jar"
java weka.filters.unsupervised.attribute.AddCluster -W "weka.clusterers.XMeans -I 1 -M 1000 -J 1000 -L 1 -H 4 -B 1.0 -C 0.5 -D \"weka.core.EuclideanDistance -R first-last\" -S 10"  -i $1
