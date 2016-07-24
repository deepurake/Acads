package edu.gatech.cse6242

import org.apache.spark.SparkContext
import org.apache.spark.SparkContext._
import org.apache.spark.SparkConf

object Task2 {
  def main(args: Array[String]) {
    val sc = new SparkContext(new SparkConf().setAppName("Task2"))

    // read the file
    val file = sc.textFile("hdfs://localhost:8020" + args(0))

 	// split each document into words
    val tokenized = file.flatMap(_.split("\n").map(a => (a.split("\t")(1),a.split("\t")(2).toInt))).reduceByKey(_ + _)

    /* TODO: Needs to be implemented */
	val sum = tokenized.map{case (a,b)=>(a.toString + "\t"+b.toString)}

    // store output on given HDFS path.
    // YOU NEED TO CHANGE THIS
    sum.saveAsTextFile("hdfs://localhost:8020" + args(1))
  }
}
