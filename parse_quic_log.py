import json
import numpy as np
import matplotlib.pyplot as plt

LOG_PATH = "./server.qlog"
LOG_TYPE_INDEX = 2
LOG_OBJ_INDEX = 3
TIME_INDEX = 0
TIME_TO_MILLISECONDS_MULTIPLIER = 0.001 # Microseconds to milliseconds

total_bytes_sent = 0
total_packets_sent = 0
total_packets_received = 0
total_bytes_received = 0

rtts = []
rtt_times = []
bytes_in_flight = []
packet_lost_times = []
packet_lost_sizes = []

with open(LOG_PATH, 'r') as log_file:
    log_data = json.load(log_file)
    for trace in log_data["traces"]:
        print(len(trace["events"]))
        for event in trace["events"]:
            # print(event)
            rel_time = event[TIME_INDEX]*TIME_TO_MILLISECONDS_MULTIPLIER;
            if event[LOG_TYPE_INDEX] == "metrics_updated":
                metrics = event[LOG_OBJ_INDEX]
                rel_time = event[TIME_INDEX]*TIME_TO_MILLISECONDS_MULTIPLIER;
                if "smoothed_rtt" in metrics:
                   rtt =  metrics["smoothed_rtt"]*TIME_TO_MILLISECONDS_MULTIPLIER;
                   rtts.append(rtt)
                   rtt_times.append(rel_time)
                if "bytes_in_flight" in metrics:
                    bytes_in_flight_now = metrics["bytes_in_flight"]
                    bytes_in_flight.append(bytes_in_flight_now)
            elif event[LOG_TYPE_INDEX] == "packet_lost":
                packet_lost_times.append(rel_time)
                packet_lost_sizes.append(event[LOG_OBJ_INDEX]["header"]["packet_size"])
            elif event[LOG_TYPE_INDEX] == "packet_received": 
                # Increment total bytes received                               
                total_packets_received += 1
                bytes_received = event[LOG_OBJ_INDEX]["header"]["packet_size"]
                total_bytes_received += bytes_received
            elif event[LOG_TYPE_INDEX] == "packet_sent":
                # Increment total bytes sent
                total_packets_sent += 1
                bytes_sent = event[LOG_OBJ_INDEX]["header"]["packet_size"]
                total_bytes_sent += bytes_sent
                

# Distribution of RTT / Histogram of RTT
print(np.mean(rtts))
print(np.var(rtts))
print(np.percentile(rtts, 95))
print(np.percentile(rtts, 99))

# RTT vs time
fig = plt.figure()
plt.plot(rtt_times, rtts)  
plt.xlabel('x - axis')  
plt.ylabel('y - axis')  
plt.title('RTT vs time')  
    
# function to show the plot  
plt.show()  
fig.show()

# Bytes in flight vs time

# Packets lost

# Bytes received 

# Bytes sent

input(".....")