import csv
import json
from operator import itemgetter
from itertools import groupby
from dateutil import parser
import numpy as np
import random
import string

def find(buckets_list,row,month):
	idx = 0
	print month
	for buckets in buckets_list:
		if(month in buckets["month_list"]):
			print buckets["gross"][month]
			ratio1 = (np.mean(buckets["gross"][month]))/float(row[crit])
			ratio2 = np.mean(buckets["rating"][month])/float(row["imdb rating"])
		else:
			ratio1 = buckets["gross_avg"]/float(row[crit])
			ratio2 = buckets["avg_rating"]/float(row["imdb rating"])

			if(ratio1 > 0.5 and ratio1 < 2 and ratio2 > 0.8 and ratio2 < 1.4 ):
				return idx
		idx += 1
	return -1



def is_number(s):
    try:
        float(s)
        return True
    except ValueError:
        return False

csvfile = open('features_final.csv', 'rU')
jsonfile = open('features.json', 'w')

fieldnames = ("runtime","Rated","review_score","release_date","raw_time","imdb rating","sentiment","opening","total")
crit = "opening"
reader1 = csv.DictReader( csvfile, fieldnames)
reader = [ row for row in reader1 ] 
print ""
reader.sort(key=itemgetter("release_date"))
print reader[0]

month = ""
buckets = []
for row in reader:
	if( not is_number(row[crit])):
		continue
	idx = -1
	try:
		idx = find(buckets,row,month)
	except ValueError:
		continue
	#month = parser.parse(row["release_date"]).strftime("%B %Y")
	month = int(parser.parse(row["release_date"]).month)
	if(idx == -1):
		idx = len(buckets)
		name = ''.join(random.choice(string.ascii_uppercase + string.digits) for _ in range(100))
		buckets.append({"name":name,"gross_avg":float(row[crit]),"avg_rating":float(row["imdb rating"]),"Rated":row["Rated"],"gross":{month:[float(row[crit])]},"count":{month:1},"rating":{month:[float(row["imdb rating"])]}})
		buckets[idx]["month_list"] = [month]
	else:
		if month not in buckets[idx]["month_list"]:
			buckets[idx]["gross"][month] = []
			buckets[idx]["count"][month] = 0
			buckets[idx]["rating"][month] = []
			buckets[idx]["month_list"].append(month)
		buckets[idx]["gross"][month].append(float(row[crit]))
		buckets[idx]["count"][month] += 1
		buckets[idx]["rating"][month].append(float(row["imdb rating"]))

for bucket in buckets:
	gross = []
	rating = []
	count = []
	for month in bucket["month_list"]:
		bucket["gross"][month] = np.mean(bucket["gross"][month])
		bucket["rating"][month] = np.mean(bucket["rating"][month])
		gross.append([month,bucket["gross"][month]])
		rating.append([month,bucket["rating"][month]])
		count.append([month,bucket["count"][month]])
	bucket["gross"] = gross
	bucket["rating"] = rating
	bucket["count"] = count


print buckets



"""glo = [[x for x,y in g]
       for k,g in  groupby(lki,key=itemgetter("release_date"))]
"""

out = json.dumps( [ row for row in buckets ] )
jsonfile.write(out)