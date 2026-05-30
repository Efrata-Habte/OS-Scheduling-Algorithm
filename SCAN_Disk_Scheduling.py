# Assumptions : 
    # 1. 1 sector movement = 1 time unit
    # 2. service time = seek time
    # 3. one process has exactly one sector request
    # 4. Seek direction is from left to right

pending_requests = []
waiting_time= []
valid_requests = []
service_order = []
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


valid_requests.sort(key= lambda x:x[1])
n = len(valid_requests)
start = 0

for i in range(n):
    if current_pos <= valid_requests[i][1]:
        start = i
        break

for i in range(start,n):
    service_order.append(valid_requests[i])
    waiting_time.append(seek_time)
    seek_time += abs(current_pos - valid_requests[i][1])
    current_pos = valid_requests[i][1]

for i in range(start-1,-1,-1):
    service_order.append(valid_requests[i])
    waiting_time.append(seek_time)
    seek_time += abs(current_pos - valid_requests[i][1])
    current_pos = valid_requests[i][1]

if n > 0:
    ave_seek_distance = seek_time / n
    total_waiting_time = sum(waiting_time)
    ave_waiting_time = total_waiting_time / n

if seek_time > 0:
    throughput = n / seek_time 

print("")
print("Request Order: ")
for i in range(n):
    print(f"{service_order[i][1]}", end ="->")

print(f"Total head movement : {seek_time}")
print(f"Average seek distance : {ave_seek_distance}")
print(f"Throughput : {throughput} request/time-unit")

for i in range(n):
    print(f"Waiting time for process {service_order[i][0]} : {waiting_time[i]}")

print(f"Total waiting time: {total_waiting_time}")
print(f"Average waiting time : {ave_waiting_time}")


# there are some issues fix it tomorrow 