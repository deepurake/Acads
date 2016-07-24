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
import numpy as np
#import alchemyapi_python.__future__ import print_function
from alchemyapi_python.alchemyapi import AlchemyAPI

alchemyapi = AlchemyAPI()
demo_text = 'Yesterday dumb Bob destroyed my fancy iPhone in beautiful Denver, Colorado. I guess I will have to head over to the Apple Store and buy a new one.'
response = alchemyapi.entities('text', demo_text, {'sentiment': 1})


key = 'd7d457mawe4jc68nja63sjcn'
search_url1 = 'http://api.rottentomatoes.com/api/public/v1.0/movies.json?apikey='+key+'&q='
search_url2 = '&page_limit=1&page='

rev_url1='http://api.rottentomatoes.com/api/public/v1.0/movies/'
rev_url2='/reviews.json?apikey='+key+'&page_limit=50'

mpaa_conversion = {"G":0, "PG":1, "PG-13":2,"R":3,"NC-17":4}
ratings_conversion = {"A":1.0,"B":0.7,"C":0.4,"D":0.2,"E":0}
def rt_features(search_str):
  search_url = search_url1+search_str+search_url2

  response = urllib.urlopen(search_url+str(1))
  movie = json.loads(response.read())['movies'][0]

  response = urllib.urlopen(rev_url1+movie['id']+rev_url2)
  data = json.loads(response.read())
  
  #myt = data["reviews"]

  feature_list = []
  feature_list.append(mpaa_conversion[movie["mpaa_rating"]])
  feature_list.append(movie["runtime"])
  if ("critics_consensus" not in movie.keys()) or (not movie["critics_consensus"]):
    #print movie["critics_consensus"]
    feature_list.append(0)
  else:
    sentiment = get_sentiment(movie["critics_consensus"])
    feature_list.append(sentiment)
  feature_list.append(movie["ratings"]["critics_score"])
  feature_list.append(movie["ratings"]["audience_score"])

  #reviews
  feature_list.append(data["total"])
  pos_list = []
  score_list = []
  for review in data["reviews"]:
    sentiment = get_sentiment(review["quote"])
    pos_list.append(sentiment)
    if "original_score" in review.keys():
      if review["original_score"][0] in ratings_conversion.keys():
        score_list.append(ratings_conversion[review["original_score"][0]])
      else:
        try:
          score_list.append(float(review["original_score"].split("/")[0])/float(review["original_score"].split("/")[1]))
        except Exception as ex:
          print ex
          print review["original_score"]

  if(len(pos_list) > 0):
    pos_list = np.array(pos_list)
    feature_list.append(np.mean(pos_list))
  else:
    feature_list.append(0)

  if(len(score_list) > 0):
    score_list = np.array(score_list)
    feature_list.append(np.mean(score_list))
  else:
    feature_list.append(0)

  return feature_list

def get_sentiment(review):
  #alchemy_apikey='df798c566bb0d721db86139d2107b3f7f686dcab'
  response = alchemyapi.sentiment('text', review)

  if response['status'] == 'OK':
    """print ('## Response Object ##')
    print (json.dumps(response, indent=4))

    print ('')
    print ('## Document Sentiment ##')
    print ('type: ', response['docSentiment']['type'])"""
    json.dumps(response, indent=4)

  if 'score' in response['docSentiment']:
    return response['docSentiment']['score']

  else:
    print ('Error in sentiment analysis call: ', response)

  return 0

def get_sentiment_old(review):
  command = "curl -d \"text="+review.replace("\"", '').strip()+"\" http://text-processing.com/api/sentiment/"
  p = subprocess.Popen(shlex.split(command), stdout=subprocess.PIPE,stderr=subprocess.PIPE)
  value = p.stdout.read()
  try:
    return simplejson.loads(value)['probability']
  except Exception as ex:
    print value
    return {"pos":0,"neutral":0}
