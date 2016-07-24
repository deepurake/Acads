import json
import requests
import urllib
import time
import pandas
import datetime
#key="8xrv2g8wvvy9ypej64v87vf6"
#f=open('outdva1.txt')
counter=0
link = "http://www.omdbapi.com/?t=myth&y=&plot=short&r=json&tomatoes=true"#http://api.rottentomatoes.com/api/public/v1.0/movies"
#http://api.rottentomatoes.com/api/public/v1.0/movies/770672122/similar.json?apikey=[your_api_key]

def imdb_features(movie):
	url="http://www.omdbapi.com/?t=%s&y=&plot=short&r=json&tomatoes=true"
	url=url % (movie)
	result = requests.get(url)
	data = json.loads(result.content)
	feature_list=[]
	feature_list.extend((data['Rated'].encode("ascii","ignore"), \
		int(data["BoxOffice"].encode("ascii","ignore")), \
		time.mktime(datetime.datetime.strptime(data['Released'].encode("ascii","ignore"),"%d %b %Y").timetuple()), \
		int(data['Runtime'].encode("ascii","ignore").split()[0]), \
		data['Genre'].encode("ascii","ignore"), \
		data['Language'].encode("ascii","ignore"), \
		data['Metascore'].encode("ascii","ignore"), \
		data['imdbRating'].encode("ascii","ignore"), \
		data['imdbVotes'].encode("ascii","ignore"), \
		data['tomatoMeter'].encode("ascii","ignore"), \
		data["tomatoRating"].encode("ascii","ignore"), \
		data["tomatoReviews"].encode("ascii","ignore"), \
		data["tomatoFresh"].encode("ascii","ignore"), \
		data["tomatoRotten"].encode("ascii","ignore"), \
		data["tomatoConsensus"].encode("ascii","ignore"), \
		data["tomatoUserMeter"].encode("ascii","ignore"), \
		data["tomatoUserRating"].encode("ascii","ignore"), \
		data["tomatoUserReviews"].encode("ascii","ignore"), \
		
		))
	print feature_list
	return feature_list



#get_imdb_features("The Myth")

