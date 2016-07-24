##################################### Method 1

import urllib, json

url = "https://maps.googleapis.com/maps/api/geocode/json?latlng=40.714224,-73.961452&key=AIzaSyCEsr3f-LbFroDrwaqzy6c4hicdVbI7HYI"
jsonurl = urllib.urlopen(url)
print jsonurl
text = json.loads(jsonurl)
print text
