import pygeocoder
import csv
import codecs
import time

def main():
	i=0
	apikeys=3
	delay=10
	#Initialize Google Geocoder API
	geocodes=["AIzaSyALEw3mSl_8MBk8jBtx5FX_iU4Q6QTYlxk","AIzaSyCmw7Sdg3nVMoV3QmloLCOTtIs2jSlSNYk","AIzaSyBbsYKdYK8HUyJ-77ChVswOyZ6envEtIc8"]
	geocoder = pygeocoder.Geocoder(geocodes[0])
	counter=0
	with open("friends.csv") as user_list:
		users=csv.reader(user_list, delimiter='|')
		for user in users:
			name = user[0]
			handler = user[1]
			img_link = user[2]
			location =user[3]
			connections = user[4]
			print user
			result = coordlocation(geocoder,name, handler, img_link, connections, location)
			if result ==0:
				i+=1
				if i==apikeys:
					i=0
					time.sleep(delay)
				geocoder=pygeocoder.Geocoder(geocodes[i])
				result = coordlocation(geocoder,name, handler, img_link, connections, location)

def coordlocation(geocoder,name, handler, img_link, connections, address=""):
	# figure out location latitude and longitude based on address
	if len(address) == 0:
		#address = input("Enter the address: ")
		pass
	try:
		currentlocation = geocoder.geocode(address)
	except ValueError:
		print ("Value Error")
		return 1
	except:
		print("\nGoogle Geocoding API is offline or has reached the limit of queries.\n")
		return 0
	coordlocation = 0
	try:
		f = open("twitter_formatted_data.csv", 'a')
		output = name+","+handler+","+img_link+","+str(currentlocation.latitude) + '|' + str(currentlocation.longitude)+","+address+","+connections
		f.write(output)
		f.write("\n")
		f.close()
	except:
		print("Unable to get location.")
main()