import pandas
import rt_data
import imdb_data
import csv

data1 = pandas.DataFrame.from_csv('movies2014.tsv',sep='\t',header=0)
data2 = pandas.DataFrame.from_csv('imdb_f_3.tsv',sep='\t',header=0)
print data1.keys()
print data2.keys()
data1.merge(data2,how='inner', on='Title').to_csv('merged.tsv',sep='\t')