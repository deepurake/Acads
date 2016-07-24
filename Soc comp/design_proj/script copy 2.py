##################################### Method 1
import mechanize
import cookielib
from BeautifulSoup import BeautifulSoup
import html2text

# Browser
br = mechanize.Browser()

# Cookie Jar
cj = cookielib.LWPCookieJar()
br.set_cookiejar(cj)

# Browser options
br.set_handle_equiv(True)
br.set_handle_gzip(True)
br.set_handle_redirect(True)
br.set_handle_referer(True)
br.set_handle_robots(False)
br.set_handle_refresh(mechanize._http.HTTPRefreshProcessor(), max_time=1)

br.addheaders = [('User-agent', 'Chrome')]

# The site we will navigate into, handling it's session
print(br.open('https://www.facebook.com/login').read())
br.open('https://www.facebook.com/login')

print(br)

# View available forms
for f in br.forms():
    print f

# Select the second (index one) form (the first form is a search query box)
br.select_form(nr=1)

# User credentials
br.form['email'] = 'srakesh.iitk@gmail.edu'
br.form['pass'] = 'sulaprra'

# Login
br.submit()

print(br.open('https://www.facebook.com/search/str/friends%2Bof%2Bfriends%2Bwho%2Blive%2Bin%2Bnew%2Byork/keywords_users').read())

"""
from bs4 import BeautifulSoup as bs
import mechanize
import re
import sys

crn = sys.argv[1]

#email_id = sys.argv[2]

URL = "https://www.facebook.com/search/str/friends%2Bof%2Bfriends%2Bwho%2Blive%2Bin%2Bnew%2Byork/keywords_users"

#URL = URL.replace("<CRN>", crn)

print URL

br = mechanize.Browser()

br.addheaders = [('User-agent', 'Mozilla/5.0 (X11; U; Linux i686; en-US; rv:1.9.0.1) Gecko/2008071615 Fedora/3.0.1-1.fc9 Firefox/3.0.1')]

br.set_handle_robots(False)

res = br.open(URL)

content = res.read()

soup = bs(content)

print soup


tr_list = soup.find_all('tr')

remaining = None

for tr in tr_list:
    th = tr.find('th')
    if th:
        if re.subn('<.*?>', '', str(th))[0] == 'Seats':
            remaining = int(tr.find_all('td')[-1].string)

if remaining == 0:
    print "None remaining"
    sys.exit(1)

import smtplib

server = smtplib.SMTP('smtp.gmail.com')
server.ehlo()
server.starttls()
server.ehlo()

server.login('deepurake@gmail.com', '**')

msg = "Subject: Seats Availability " + crn + " : "+ str(remaining)

server.sendmail("deepurake@gmail.com", "deepurake@gmail.com", msg)
#server.sendmail("srakesh@gmail.com", email_id, msg)

server.close()
"""
