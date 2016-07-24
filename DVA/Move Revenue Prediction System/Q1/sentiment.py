import json
import urllib
import time
import sys


key = '5281b00d7824f7bf662493a3b63f4031'
search_url1 = 'http://api.peoplebrowsr.com/sentiment?last='
search_url2 = '&count='

def main():
  
  search_str = sys.argv[1]
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