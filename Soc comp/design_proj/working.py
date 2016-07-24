##################################### Method 1
import browsercookie
import requests
from BeautifulSoup import BeautifulSoup

import urllib, urllib2, cookielib, re, io, sys

def get_friend_list(place,level):
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
    page = requests.get(url, cookies=cj).content
    #soup=BeautifulSoup(page.text)
    #div=soup.find('div',{'id':'BrowseResultsContainer'})
    print page
    return ''


print get_friend_list("Hyderabad",1)