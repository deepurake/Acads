import pandas
import rt_data
import imdb_data
import csv

data = pandas.DataFrame.from_csv('movies2014.tsv',sep='\t',header=0)

movie_list = data.Title.tolist()
csvwrite = csv.writer(open('features.csv', 'wb'),delimiter=',', quotechar='|', quoting=csv.QUOTE_MINIMAL)
#fieldnames = ['', 'last_name']
for movie in movie_list[:2]:
    #print rt_data.rt_features(movie)
    mylist = rt_data.rt_features(movie)
    mylist.extend(imdb_data.imdb_features(movie))
    csvwrite.writerow(mylist)