import random

import simpy

import math

import time

import matplotlib.pyplot as plt

import numpy as np

RANDOM_SEED = 29

SIM_TIME = 1000000

MU = 1

BUFFER_SIZE = 10

SUCCESSFUL_SLOTS = 0



class node:

	def __init__(self, env, arrival_rate, id_Num):

		self.env = env

		self.server = simpy.Resource(env, capacity=1)

		self.queue_len = 0

		self.flag_processing = 0

		self.packet_time = 0

		self.sum_time_length = 0

		#self.action env.process(self.run())

		self.start_idle_time = 0

		self.id = id_Num

		self.arrival_rate = arrival_rate





		self.Slot_Num = 0 # Added variable to keep track of which slot currently on for transmission. All start at time 0

		self.N_Value = 0 # What value to determine backoff K



##########################################################################################################################################      

	def process_packet(self, env): # is this apart of server or each node? Both! Node will try and transmit at given slot and then server will check every second to see if more than one node trys to transmit

		# 

		#with self.server.request() as req:

		start = env.now

			#print("environment time: ", env.now)

		yield env.timeout(1)

			# Not Sure what the timeout should be here...

			#print("environment time now: ", env.now)

			#yield env.timeout(random.expovariate(MU))

			#Service time is assumed negative exponential distr of 1 MU

			#Service time for sending should be in time slot



		if self.N_Value == 0: # We still run the backoff if N = 0

			backoff =  random.randint(0,2**min(10,self.N_Value))

			self.Slot_Num += backoff

				###print("Node {0} trying slot {1}".format(self.id,self.Slot_Num))

		else:

				#print("{0} tried at node {1}".format(self.id, self.Slot_Num))

			backoff =  random.randint(0,2**min(10,self.N_Value))

			self.Slot_Num += backoff


				#print("{0} will retry at time {1}, having backed off {2}".format(self.id, self.Slot_Num, backoff))

				# Binary Backoff

			#latency = env.now - packet.arrival_time

			#self.Packet_Delay.addNumber(latency)



			###print("Packet number {0} with arrival time {1} latency {2}".format(packet.identifier, packet.arrival_time, latency))

			# Decrement global when this code executes. Means packet has left the buffer.



		if self.queue_len == 0:

			self.flag_processing = 0

			self.start_idle_time = env.now





#######################################################################################################################################              

	def packets_arrival(self, env): # need this part for each thread

		# modification of three variables in here 

# This part is still done by the node. Should have varibles to keep track of according to algoritm. Don't need some of the extra fluff as from previous. Just like the hand algorithm.

		# packet arrivals 



		while True:

			# Infinite loop for generating packets



			yield env.timeout(random.expovariate(self.arrival_rate))

			# arrival time of one packet



			#self.packet_number += 1

			arrival_time = env.now 

			self.packet_time = arrival_time

			#new_packet = Packet(self.packet_number,arrival_time)

			# Packet arrives, added to queue



			if self.flag_processing == 0:

				self.flag_processing = 1

				idle_period = env.now - self.start_idle_time



				#self.Server_Idle_Periods.addNumber(idle_period)

				# May need, not sure how idle periods are used by Stat object



			if self.queue_len == 0:

				self.Slot_Num = math.ceil(env.now)

			self.queue_len += 1

			#env.process(self.process_packet(env))



##########################################################################################################################################



""" Packet class """            

class Packet: # dont need

	def __init__(self, identifier, arrival_time):

		self.identifier = identifier

		self.arrival_time = arrival_time

##########################################################################################################################################



class server:

	def __init__(self, env):

		self.Current_Slot = 0

		self.Successful_Slots = 0

		self.env = env



##########################################################################################################################################

	def server_process(self, env, arrival_rate, container = [], *args):



		while True:

			hold_Contain = []

			self.Current_Slot = math.floor(env.now )

			#print("Slot: ",self.Current_Slot)

			taken = 0;

			for node in container:

				if node.Slot_Num == self.Current_Slot:

					#print("Node {0} Queue Length: {1}".format(node.id,node.queue_len))

					taken += 1



			for node in container:

				if node.Slot_Num == self.Current_Slot:

					if taken == 1:

						node.queue_len -= 1

						#print("Node {0} succeded on slot {1} with n of {2}.".format(node.id, self.Current_Slot, node.N_Value))

						node.N_Value = 0;

						self.Successful_Slots += 1

						node.Slot_Num = math.ceil(env.now) # After the packet is sent, the next one will try at the next slot number

						#successful send

					elif taken > 1:

						node.N_Value += 1

						env.process(node.process_packet(env))



			yield env.timeout(1);

			#check once per second





##########################################################################################################################################



def main():     

	random.seed(RANDOM_SEED)

	container = [] # Empty list of node objects to be had
	list_of_results = []
	arrival_stuff = [0.01, 0.02, 0.03, 0.04, 0.05, 0.06, 0.07, 0.08, 0.09]

	for arrival_rate in [0.01, 0.02, 0.03, 0.04, 0.05, 0.06, 0.07, 0.08, 0.09]:

		env = simpy.Environment()

		for i in range(0, 10):

			router = node(env, arrival_rate, id_Num = i)

			env.process(router.packets_arrival(env))

			container.append(router)

		server1 = server(env)

		env.process(server1.server_process(env, arrival_rate, container))

		env.run(until=SIM_TIME) # Can just make a for loop to initialize all 10 nodes and they will run at the same time once all initialized

		print("Throughput for lamda {0}: {1}".format( arrival_rate, server1.Successful_Slots / server1.Current_Slot))

		list_of_results.append(server1.Successful_Slots / server1.Current_Slot)

	new_Arrive = np.array(arrival_stuff)
	new_list = np.array(list_of_results)
	plt.plot(new_Arrive, new_list)
	plt.show()
if __name__ == '__main__': main()
