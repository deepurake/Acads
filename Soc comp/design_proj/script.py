##################################### Method 1

import pickle
import selenium.webdriver
import browsercookie
import sqlite3
import cookielib
 
def get_cookies(cj, ff_cookies):
    con = sqlite3.connect(ff_cookies)
    cur = con.cursor()
    cur.execute("SELECT host, path, isSecure, expiry, name, value FROM moz_cookies")
    for item in cur.fetchall():
        c = cookielib.Cookie(0, item[4], item[5],
            None, False,
            item[0], item[0].startswith('.'), item[0].startswith('.'),
            item[1], False,
            item[2],
            item[3], item[3]=="",
            None, None, {})
        print c
        cj.set_cookie(c)



cj = browsercookie.firefox()
driver = selenium.webdriver.Firefox()
for c in driver.get_cookies():
    print c
for c in cj:    
    if( "facebook" in c.domain ):
        print c
        #print c.name, c.value, c.domain
        driver.add_cookie({'name':c.name, 'value':c.value})


driver.get('https://www.facebook.com/search/115200305158163/residents/present/me/friends/intersect')
f = open('test.html', 'w')
f.write(driver.page_source.encode('utf-8'))

"""
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
    page = requests.get(url, cookies=cj)
    soup=BeautifulSoup(page.content)
    div=soup.find('div',{'id':'test'})
    #print page.content
    print div
    return ''


print get_friend_list("Hyderabad",1)"""