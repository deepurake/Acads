# CSE6242/CX4242 Homework 4 Pseudocode
# You can use this skeleton code for Task 1 of Homework 4.
# You don't have to use this code. You can implement your own code from scratch if you want.

import csv
import math
import numpy as np
from collections import Counter
from datetime import datetime
from random import random,randint
from LinReg import LinReg

# Implement your decision tree below
class DecisionTree():
	#tree = {}
	root = None

	trans = {}

	def str2bool(self,v):
  		return (v=="yes")

	def learn(self, training_set):
		#print training_set
		#data = np.array(training_set)[:,:-1]
		#decision = np.array(training_set)[:,-1]
		data = np.empty([len(training_set),0])
		training_set = np.array(training_set)
		for idx in range(len(training_set[0])-1):
			if(DecisionNode.is_number(training_set[0][idx])):
				continue
			affinity = {}

			decision = np.unique(training_set[:,idx])
			for val in decision:
				results = training_set[training_set[:,idx] == val][:,-1]
				affinity[val] = float(len(results[results == "yes"]))/float(len(results))
			split_val = sorted(affinity, key=lambda key: affinity[key])
			split_val = split_val[:(len(split_val)/2)]
			for rows in range(len(training_set)):
				val = 1
				if(training_set[rows][idx] in split_val):
					val = 0
				training_set[rows][idx] = str(val)
			self.trans[idx] = split_val

			#training_set[:,-1] = 1*str2bool(training_set[])
		self.root = DecisionNode(0,0,training_set)
		#self.tree = {}

	def classify(self, test_instance1):
		test_instance = np.copy(test_instance1)
		for key in self.trans:
			if(test_instance[key] in self.trans[key]):
				#print test_instance[key]
				test_instance[key] = 0
			else:
				test_instance[key] = 1
		val = self.root.classify(test_instance) # baseline: always classifies as no
		if ((val != "no") and (val != "yes")):
			print val
		return val
		#return "no"

	def IG():
		pass

	def  Entropy():
		return 0

class DecisionNode():
	Entropy = 0
	depth = 0
	split_attr = -1
	split_val = -1
	left = None
	right = None
	training_set = None
	is_leaf = False
	value = "No"
	leaf = None

	# Routines to construct decision tree
	def __init__(self, depth,Entropy,training_set):
		#initialize the data
		self.Entropy = Entropy


		#print "list",training_set[:,-1]

		results = np.array(training_set[:,-1].tolist())

		

		most_common,num_most_common = Counter(results).most_common(1)[0]
		self.value = most_common
		self.depth = depth

		if self.depth >= 19:
			column = randint(0,len(training_set[0])-2)
			self.split_attr = column
			#print column, len(training_set[0])
			Xtrain = training_set[:,column]
			Ytrain = training_set[:,-1]
			self.leaf = LinReg()
			self.leaf.train(training_set[:,:-1],Ytrain)
			
			self.is_leaf = True
		else:	
			IG,en_right,en_left = self.calc_max_EN(training_set)

			if(DecisionNode.is_number(self.split_val)):
				if(self.split_attr != -1):
					left = training_set[training_set[:,self.split_attr].astype(np.float)<=self.split_val]
					right = training_set[training_set[:,self.split_attr].astype(np.float)>self.split_val]
					#print len(left[:,-1]),len(right[:,-1]), len(training_set[:,-1])
					"""if(len(left)*len(right) == 0):
						self.is_leaf = True
					else:"""
					self.left = DecisionNode(depth+1,en_left,left)
					self.right = DecisionNode(depth+1,en_right,right)
					self.is_leaf = False
				else:
					self.is_leaf = True
			else:
				#print self.split_val
				left = []
				right = []
				for row in training_set:
					if(row[self.split_attr] in self.split_val):
						left.append(row)
					else:
						right.append(row)
				right = np.array(right)
				left = np.array(left)
				self.left = DecisionNode(depth+1,en_left,left)
				self.right = DecisionNode(depth+1,en_right,right)
				self.is_leaf = False

	def calc_max_EN(self,training_set):
		IG = 0
		EN_Max_left = 0 
		EN_Max_right = 0
		EN = 0
		value = 0
		#loop over all the features and find the best split
		for split_attr in range(len(training_set[0])-1):
			if ( DecisionNode.is_number(training_set[0][split_attr]) == False):
				#print "Not dealing with enum type currently"
				rand = random()
				if(rand < 0.0):
					continue
				#print "started"
				EN, EN_left,EN_right,value = self.calc_split_val_enum(training_set,split_attr)
				#print self.split_val
				#print "end"
			else:
				rand = random()
				if(rand < 0.0):
					continue
				EN, EN_left,EN_right,value = self.calc_split_val(training_set,split_attr)
			if(EN == 0):
				continue
			if ((self.Entropy-EN) > IG):
				IG = (self.Entropy-EN)
				EN_Max_left = EN_left
				EN_Max_right = EN_right
				self.split_attr = split_attr
				self.split_val = value

		#if(IG != 0):
		#	print "extending", IG, self.Entropy, EN_Max_left,EN_Max_right

		return IG,EN_Max_left,EN_Max_right

	def calc_split_val_enum(self,training_set,split_attr):
		EN_Max = 0
		EN_Max_left = 0
		EN_Max_right = 0
		split_val = []
		#training_set[:,split_attr] = training_set[:,split_attr].astype(np.float)
		decision = np.unique(training_set[:,split_attr])
		if(len(decision) <= 1):
			return 0,0,0,0
		# count yes/no affinity of each feature
		"""
		affinity = {}
		for val in decision:
			results = training_set[training_set[:,split_attr] == val][:,-1]
			affinity[val] = float(len(results[results == "yes"]))/float(len(results))
		split_val = sorted(affinity, key=lambda key: affinity[key])
		split_val = split_val[:(len(split_val)/2)]
		"""

		for val in decision:
			rand = random()
			if(rand < 0.5):
				split_val.append(val)

		if((len(decision) == len(split_val)) or (len(split_val) < 1)):
			return 0,0,0,0

		EN_Max_left,EN_Max_right = self.calc_EN_enum(training_set,split_attr,split_val)

		EN_Max = EN_Max_left+EN_Max_right

		return EN_Max,EN_Max_right,EN_Max_left,split_val

	def calc_EN_enum(self,training_set,split_attr,value):
		left = []
		right = []
		for row in training_set:
			if(row[split_attr] in value):
				left.append(row)
			else:
				right.append(row)
		right = np.array(right)
		left = np.array(left)

		#calculate right entropy
		length = len(right[:,0])
		pos = len(right[right[:,-1] == "yes"][:,0])
		neg = len(right[right[:,-1] == "no"][:,0])
		#print pos, neg
		left_en = DecisionNode.log_EN(pos)+DecisionNode.log_EN(neg)

		#calculate left entropy
		length = len(left[:,0])
		pos = len(left[left[:,-1] == "yes"][:,0])
		neg = len(left[left[:,-1] == "no"][:,0])
		right_en = DecisionNode.log_EN(pos)+DecisionNode.log_EN(neg)

		if (left_en*right_en == 0):
			left_en = 0
			right_en = 0
		return left_en,right_en

	def calc_split_val(self,training_set,split_attr):
		EN_Max = 0
		EN_Max_left = 0
		EN_Max_right = 0
		split_val = -1
		#training_set[:,split_attr] = training_set[:,split_attr].astype(np.float)
		decision = np.unique(training_set[:,split_attr]).astype(np.float)
		#print decision
		maximum = np.max(decision)
		minimum = np.min(decision)
		loop_over = np.array([])
		if(len(decision) > 4):
			for i in range(1,4):
				loop_over=np.append(loop_over,minimum + (float(i))*(maximum-minimum)/float(4))
		else:
			loop_over = decision

		for value in loop_over:
			EN_left,EN_right = self.calc_EN(training_set,split_attr,value)
			if(EN_left+EN_right == 0):
				continue
			if (EN_left+EN_right < EN_Max):
				EN_Max = EN_left+EN_right
				EN_Max_right =EN_right
				EN_Max_left = EN_left
				split_val = value
		return EN_Max,EN_Max_right,EN_Max_left,split_val

	def calc_EN(self,training_set,split_attr,value):
		right = training_set[training_set[:,split_attr].astype(np.float)>value]
		left = training_set[training_set[:,split_attr].astype(np.float)<=value]

		#calculate right entropy
		length = len(right[:,0])
		pos = len(right[right[:,-1] == "yes"][:,0])
		neg = len(right[right[:,-1] == "no"][:,0])
		#print pos, neg
		left_en = DecisionNode.log_EN(pos)+DecisionNode.log_EN(neg)

		#calculate left entropy
		length = len(left[:,0])
		pos = len(left[left[:,-1] == "yes"][:,0])
		neg = len(left[left[:,-1] == "no"][:,0])
		right_en = DecisionNode.log_EN(pos)+DecisionNode.log_EN(neg)

		if (left_en*right_en == 0):
			left_en = 0
			right_en = 0
		return left_en,right_en

	@staticmethod
	def log_EN(val):
		if(val == 0):
			return 0
		return -1*(float)(val)*math.log((float)(val),2)

	@staticmethod
	def is_number(s):
	    try:
	    	#print s.strip(), len(s.strip())
	        val = float(s) # for int, long and float
	        return True
	    except (ValueError,AttributeError,TypeError):
    		return False
		return True

	def classify(self,test_instance):
		if(self.is_leaf):
			if(self.leaf != None):
				return self.leaf.query(np.array([test_instance[:-1]]))
			return self.value
		else:
			if(DecisionNode.is_number(self.split_val)):
				if(float(test_instance[self.split_attr]) > self.split_val):
					return self.right.classify(test_instance)
				else:
					self.left.classify(test_instance)
			else:
				if((test_instance[self.split_attr]) in self.split_val):
					return self.left.classify(test_instance)
				else:
					self.right.classify(test_instance)

		return self.left.classify(test_instance)

def run_decision_tree():
	

	# Load data set
	with open("hw4-data.tsv") as tsv:
		data = [tuple(line) for line in csv.reader(tsv, delimiter="\t")]
	print "Number of records: %d" % len(data)

	tstart = datetime.now()
	# Split training/test sets
	# You need to modify the following code for cross validation.
	K = 10
	accuracy = []
	for div in range(10):
		training_set = [x for i, x in enumerate(data) if i % K != div]
		test_set = [x for i, x in enumerate(data) if i % K == div]

		tree = DecisionTree()
		# Construct a tree using training set
		tree.learn( training_set )

		# Classify the test set using the tree we just constructed
		results = []
		for instance in test_set:
			result = tree.classify( instance[:-1] )
			results.append( result == instance[-1] )

		# Accuracy
		acc = float(results.count(True))/float(len(results))
		print "acc:",acc
		accuracy.append(acc)


	tend = datetime.now()
	diff = tend-tstart
	print "accuracy: %.4f" % np.mean(np.array(accuracy))
	print accuracy
	print "time: ",diff.seconds
	
	# Writing results to a file (DO NOT CHANGE)
	f = open("result.txt", "w")
	f.write("accuracy: %.4f" % np.mean(np.array(accuracy)))
	f.close()


if __name__ == "__main__":
	run_decision_tree()
