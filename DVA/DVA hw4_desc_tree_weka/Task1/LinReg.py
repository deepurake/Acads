import numpy as np
import sys,csv,math

class LinReg:
	error = False
	train = None
	def train(self, Xtrain, Ytrain):
		#print Ytrain
		Ytrain = 1*self.str2bool(Ytrain)
		#print "working"
		#Xtrain = np.copy(Xtrain)
		Xtrain = Xtrain.astype(np.float,casting='unsafe',subok=True)
		#print Xtrain
		#print "working 2"
		try:
			Xtrain = np.vstack([Xtrain[:,0], Xtrain[:,1], np.ones(len(Xtrain))]).T
			res = np.linalg.lstsq(Xtrain, Ytrain)
			self.train = res[0]
			#print "Working"
		except:
			print "Unexpected error:", sys.exc_info()[0]
			self.error = True

	def query(self, Xtest):
		#Xtest = np.copy(Xtest)
		Xtest = Xtest.astype(np.float,casting='unsafe',subok=True)
		if(self.error):
			return "no"
		else:
			Xtest = np.vstack([Xtest[:,0], Xtest[:,1], np.ones(len(Xtest))]).T
			#print Xtest
			res = np.dot(Xtest,self.train)
			if(res[0]>0.5):
				return "yes"
			return "no"

	def str2bool(self,v):
  		return (v=="yes")
