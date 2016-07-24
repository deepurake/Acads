import json
import urllib
import time


key = 'd7d457mawe4jc68nja63sjcn'
search_url = 'http://api.rottentomatoes.com/api/public/v1.0/movies.json?apikey='+key+'&q=life&page_limit=50&page='
sim_url1='http://api.rottentomatoes.com/api/public/v1.0/movies/'
sim_url2='/similar.json?apikey='+key+'&limit=5'

movie_ID_name = open('movie_ID_name.txt','w')
movie_ID_name_csv = open('movie_ID_name.csv','w')
movie_ID_sim_movie_ID = open('movie_ID_sim_movie_ID.txt','w')
movie_ID_sim_movie_ID_csv = open('movie_ID_sim_movie_ID.csv','w')

def main():
  
  
  movie_ID_list = []
  pair=[]
  calls = 0
  
  #movie_ID_name
  movie_ID_name_csv.write("Id,Label\n")
  for i in range(1,7):
    if (calls == 4):
      time.sleep(1)
    response = urllib.urlopen(search_url+str(i))
    calls = (calls+1)%5
    data = json.loads(response.read())
    #print "ID: ",data["movies"][0]["id"],", Title: ",data["movies"][0]["title"]
    for movie in data["movies"]:
      movie_ID_name.write(movie["id"]+","+movie["title"]+"\n")
      movie_ID_name_csv.write(movie["id"]+","+movie["title"]+"\n")
      movie_ID_list.append(movie["id"])

  #movie_ID_sim_movie_ID
  movie_ID_sim_movie_ID_csv.write("Source,Target,Type\n")
  for movie_id in movie_ID_list:
    if (calls == 4):
      time.sleep(1)
    response = urllib.urlopen(sim_url1+movie_id+sim_url2)
    calls = (calls+1)%5
    data = json.loads(response.read())
    for movie in data["movies"]:
      if([movie["id"],movie_id] in pair):
        continue
      movie_ID_sim_movie_ID.write(movie_id+","+movie["id"]+"\n")
      movie_ID_sim_movie_ID_csv.write(movie_id+","+movie["id"]+",Undirected\n")
      pair.append([movie_id,movie["id"]])


def get_sim(url):
  response = urllib.urlopen(url)
if __name__ == "__main__":
  main()