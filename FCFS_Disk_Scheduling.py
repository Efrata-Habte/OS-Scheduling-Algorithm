from collections import deque

# Assumptions : 
    # 1. 1 sector movement = 1 time unit
    # 2. service time = seek time
    # 3. one process has exactly one sector request

pending_requests = deque()
waiting_time= []
valid_requests = []
seek_time = 0
total_waiting_time = 0
ave_seek_distance = 0
ave_waiting_time = 0
throughput = 0

num = int(input("Enter number of processes: "))
sector = int(input("Enter number of sectors: "))
current_pos = int(input("Current head position: "))

for i in range(num):
    req_sector = int(input(f"Enter request sector for process number {1+i} : "))
    pending_requests.append((1+i,req_sector))

print("")
print(f"Head starts at : {current_pos} ")

for req in pending_requests:

    if 0 > req[1] or req[1] >= sector: 
        print(f"process {req[0]} -> sector {req[1]} :  Invalid request sector")
        continue
    valid_requests.append(req)
    waiting_time.append(seek_time)
    seek_time += abs(current_pos - req[1])
    current_pos = req[1]


num_valid_req = len(valid_requests)

if seek_time == 0:
    throughput = 0
else:
    throughput = num_valid_req / seek_time
total_waiting_time = sum(waiting_time)

if num_valid_req > 0:
    ave_waiting_time = total_waiting_time / num_valid_req
    ave_seek_distance = seek_time / num_valid_req


print("")
print("Request Order: ")
for i in range(num_valid_req):
    print(f"{valid_requests[i]}", end ="->")

print(f"Total head movement : {seek_time}")
print(f"Average seek distance : {ave_seek_distance}")
print(f"Throughput : {throughput} request/time-unit")

for i in range(num_valid_req):
    print(f"Waiting time for process {valid_requests[i][0]} : {waiting_time[i]}")

print(f"Total waiting time: {total_waiting_time}")
print(f"Average waiting time : {ave_waiting_time}")
