import tweepy
class TwitterUser:
	def __init__(self, name,url,image,location, screen_name, mutual_friend):
		self.name = name
		self.url = url
		self.profile_pic = image
		self.location = location
		self.screen_name = screen_name
		self.mutualFriend = mutual_friend

	def getUser(self):
		return self.name + "," +  self.url + "," + self.profile_pic + "," + self.location + "," + self.mutualFriend+"\n"

def createFriendList(username,api):
	friends = []
	user = api.get_user(username)
	for friend in user.friends():

		friends.append(TwitterUser(friend.name,'https://twitter.com/'+friend.screen_name,friend.profile_image_url,friend.location, friend.screen_name, user.name ))
	return friends



consumer_key = "Q6axXAH8pHIin65tsDRVOjeid"
consumer_secret = "PfVwlUQ3epjJHC3QUENhDQvjqVdhtEstZV04d5FepOuqiRaWSa"
access_token = "3277886006-mkAJ78iE2UaMiD44dTjIWgxjqZCitklRU9DUgJr"
access_token_secret = "6pb25BDEUEwBUvZfTpznketI4Pe1Nez0yhQA72AAixllB"
auth = tweepy.OAuthHandler(consumer_key, consumer_secret)
auth.set_access_token(access_token, access_token_secret)

api = tweepy.API(auth)

users = createFriendList('manasvini_s', api)
all_friends = [] + users

for u in users:
	all_friends += createFriendList(u.screen_name, api)


writer = open('friends.csv', 'w')
for a in all_friends:
	print a.getUser()
	writer.write(a.getUser().encode("UTF-8"))

writer.close()
  
