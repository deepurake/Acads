from bs4 import BeautifulSoup as bs
import mechanize
import re
import sys

crn = sys.argv[1]

#email_id = sys.argv[2]

URL = "https://oscar.gatech.edu/pls/bprod/bwckschd.p_disp_detail_sched?term_in=201508&crn_in=<CRN>"

URL = URL.replace("<CRN>", crn)

print URL

br = mechanize.Browser()

br.addheaders = [('User-agent', 'Mozilla/5.0 (X11; U; Linux i686; en-US; rv:1.9.0.1) Gecko/2008071615 Fedora/3.0.1-1.fc9 Firefox/3.0.1')]

br.set_handle_robots(False)

res = br.open(URL)

content = res.read()

soup = bs(content)

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
