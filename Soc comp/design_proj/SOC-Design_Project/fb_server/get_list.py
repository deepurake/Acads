##################################### Method 1
import browsercookie
import requests

import urllib, urllib2, cookielib, re, io, sys

import BeautifulSoup
import json

def get_data(place,level):
    global requests
    # Cookie Jar
    cj = browsercookie.firefox()
    
    url ='https://www.facebook.com/search/str/friends'
    while (level > 1):
        url += '%2Bof%2Bfriends'
        level -= 1
    url += '%2Bwho%2Blive%2Bin%2B'
    url += place.replace(" ","%2B")
    url += '/keywords_users'

    #scrap friend list and stuff
    page = requests.get(url, cookies=cj)
    return page.content


def get_div(data):
	split =  data.split('code class="hidden_elem"')[1:]
	for d in split:
		if "mutual friends" in d:
			return d.split('!--')[1].split('--!')[0]
			break

def process_data(data):
	html = get_div(data)
	soup = BeautifulSoup.BeautifulSoup(html)
	#soup = BeautifulSoup.BeautifulSoup(
	'''<div>test1</div>
	<div>test2</div>
	<div>
		<div>
			<div></div>
		</div>
	</div>'''
	#)
	data = soup
	for i in range(3):
		for c in data:
			if "mutual" in str(c):
				data = c
				break

	while True:
		if (data.contents != None) and ((len(data.contents) == 1 ) or  not ("mutual" in str(data.contents[0]) and "mutual" in str(data.contents[1]))) :
			print "here"
			for c in data:
				if "mutual" in str(c):
					data = c
					break 
		else:
			break

	out = []
	for c in data.contents:
		address= c.contents[0].contents[0].contents[1].contents[0].contents[2].contents[0].contents[0].contents[0].contents[1].contents[0]
		#print address
		myurl = "https://maps.googleapis.com/maps/api/geocode/json?address="

		for a in address.split(" "):
			myurl += a + "+"
		myurl += "&key=AIzaSyCEsr3f-LbFroDrwaqzy6c4hicdVbI7HYI" 
		#loc =  requests.get(myurl).content["results"][0]["geometry"]["location"]
		obj = {}
		#obj["address"] = c.contents[0].contents[0].contents[1].contents[0].contents[2].contents[0].contents[0].contents[0].contents[1].contents[0]
		obj["address"] = json.loads(requests.get(myurl).content)["results"][0]["geometry"]["location"]
		obj["link"] = c.contents[0].contents[0].contents[0].get('href')
		obj["img"] = c.contents[0].contents[0].contents[0].contents[0].get('src')
		obj["name"] = str(c.contents[0].contents[0].contents[1].contents[0].contents[0].contents[1].contents[0].contents[0].contents[0].contents[0].contents[0])
		out.append(obj)
	return json.dumps(out)

def get_json(place):
	try:
		data = get_data(place,1)
		return process_data(data)
	except:
		return json.dumps([])

def get_json1(place):
	data = get_data(place,1)
	return process_data(data)

print get_json("New York")