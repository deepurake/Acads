import json
import urllib
import time
import sys
import requests
import unirest
import os
import json
import simplejson
import subprocess
import shlex


key = 'd7d457mawe4jc68nja63sjcn'
search_url1 = 'http://api.rottentomatoes.com/api/public/v1.0/movies.json?apikey='+key+'&q='
search_url2 = '&page_limit=1&page='

rev_url1='http://api.rottentomatoes.com/api/public/v1.0/movies/'
rev_url2='/reviews.json?apikey='+key+'&page_limit=50'#+'&limit=5'

movie_ID_name = open('movie_ID_name.txt','w')
movie_ID_name_csv = open('movie_ID_name.csv','w')
movie_ID_sim_movie_ID = open('movie_ID_sim_movie_ID.txt','w')
movie_ID_sim_movie_ID_csv = open('movie_ID_sim_movie_ID.csv','w')

def main():
  search_str = sys.argv[1]
  search_url = search_url1+search_str+search_url2
  movie_ID_list = []
  pair=[]
  calls = 0
  
  #movie_ID_name
  movie_ID_name_csv.write("Id,Label\n")
  for i in range(1,2):
    if (calls == 4):
      time.sleep(1)
    response = urllib.urlopen(search_url+str(i))
    calls = (calls+1)%5
    data = json.loads(response.read())
    #print data["movies"][0]["title"]
    
    for movie in data["movies"]:
      movie_ID_name.write(movie["id"]+","+movie["title"]+"\n")
      movie_ID_name_csv.write(movie["id"]+","+movie["title"]+"\n")
      movie_ID_list.append(movie["id"])


  #movie_ID_sim_movie_ID
  movie_ID_sim_movie_ID_csv.write("Source,Target,Type\n")
  for movie_id in movie_ID_list:
    if (calls == 4):
      time.sleep(1)
    response = urllib.urlopen(rev_url1+movie_id+rev_url2)
    calls = (calls+1)%5
    data = json.loads(response.read())
    #print data
    #print len(data["reviews"])
    
    #payload = {'text': 'great'}
    #print requests.get("http://text-processing.com/api/sentiment/", params=payload)
    
    
    response = unirest.post("https://community-sentiment.p.mashape.com/text/",
                            headers={
                            #"X-Mashape-Key": "T3tFQ83dN8mshWpwly2PCqiJup2Ip1NhiSyjsnTVS0Emgy5PBn",
                            "Content-Type": "application/x-www-form-urlencoded",
                            "Accept": "application/json"
                            },
                            params={
                            "txt": "Today is  a good day"
                            }
                            )
    # print response.body
    print data["reviews"][45].keys()
    #print data["reviews"][1]["quote"]
    myt = data["reviews"][45]["quote"]
    #result_str = str(subprocess.check_output(),shell=True)#.replace("'", "\"")
    command = "curl -d \"text="+myt+"\" http://text-processing.com/api/sentiment/"
    p = subprocess.Popen(shlex.split(command), stdout=subprocess.PIPE,stderr=subprocess.PIPE)
    print simplejson.loads(p.stdout.read())
    #print "char:",result_str[1:]
    #result = json.loads("{\"probability\": {\"neg\": 0.091062927148137796, \"neutral\": 0.18765261280477863, \"pos\": 0.9089370728518622}, \"label\": \"pos\"}")
    #result = simplejson.loads((result_str.strip("'<>() ").replace('\'', '\"')))
    #print result_str#["probability"]["neg"]
    """for movie in data["movies"]:
      if([movie["id"],movie_id] in pair):
        continue
      movie_ID_sim_movie_ID.write(movie_id+","+movie["id"]+"\n")
      movie_ID_sim_movie_ID_csv.write(movie_id+","+movie["id"]+",Undirected\n")
      pair.append([movie_id,movie["id"]])

def get_sim(url):
  response = urllib.urlopen(url)
  """

if __name__ == "__main__":
  main()