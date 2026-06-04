/* 
Assumptions
 1. 1 sector movement = 1 time unit
 2. service time = seek time
 3. One process may have many requests
 4. Each request has an arrival time 
 */

#include <stdio.h>
#include <stdlib.h>

// Define maximum structural limits
#define MAX_PROCESSES 20
#define MAX_REQUESTS 100

typedef struct {
    int pid;
    int sector;
    int arrival_time;
} Request;

int sector_count;

Request nearest(Request requests[], int size, int current_pos, int *index) {
    int min_diff = sector_count * 2;
    Request ans = requests[0];

    *index = 0;

    for (int i = 0; i < size; i++) {
        int diff = abs(current_pos - requests[i].sector);

        if (diff <= min_diff) {
            min_diff = diff;
            ans = requests[i];
            *index = i;
        }
    }

    return ans;
}

int main() {
    int num;
    int current_pos;

    int seek_time = 0;
    int current_time = 0;

    int total_waiting_time = 0;
    float ave_seek_distance = 0;
    float ave_waiting_time = 0;
    float throughput = 0;

    printf("Enter number of processes: ");
    scanf("%d", &num);
    
    // protect array overflow
    if (num > MAX_PROCESSES) {
        printf("Error: Maximum allowed processes is %d\n", MAX_PROCESSES);
        return 1;
    }

    printf("Enter number of sectors: ");
    scanf("%d", &sector_count);

    printf("Current head position: ");
    scanf("%d", &current_pos);

    int request_counts[MAX_PROCESSES];
    int total_requests = 0;

    for (int i = 0; i < num; i++) {
        printf("How many requests for process %d? ", i + 1);
        scanf("%d", &request_counts[i]);

        total_requests += request_counts[i];
    }

    // Guard against total array overflow
    if (total_requests > MAX_REQUESTS) {
        printf("Error: Maximum allowed total requests is %d\n", MAX_REQUESTS);
        return 1;
    }

    Request pending[MAX_REQUESTS];
    Request valid[MAX_REQUESTS];
    Request service_order[MAX_REQUESTS];
    int waiting_time[MAX_REQUESTS];

    int k = 0;

    printf("\nEnter request details:\n");

    for (int i = 0; i < num; i++) {
        printf("\nProcess %d\n", i + 1);

        for (int j = 0; j < request_counts[i]; j++) {
            pending[k].pid = i + 1;

            printf("Request %d sector: ", j + 1);
            scanf("%d", &pending[k].sector);

            printf("Request %d arrival time: ", j + 1);
            scanf("%d", &pending[k].arrival_time);

            k++;
        }
    }

    printf("\nHead starts at : %d\n", current_pos);

    int valid_count = 0;

    for (int i = 0; i < total_requests; i++) {
        if (pending[i].sector < 0 || pending[i].sector >= sector_count) {
            printf("Process %d -> sector %d : Invalid request sector\n",
                   pending[i].pid, pending[i].sector);
            continue;
        }
        valid[valid_count] = pending[i];
        valit_count++;
    }

    int serviced = 0;

    while (valid_count > 0) {
        Request available[MAX_REQUESTS];
        int available_indices[MAX_REQUESTS];
        int available_count = 0;

        // Find arrived requests 
        for (int i = 0; i < valid_count; i++) {
            if (valid[i].arrival_time <= current_time) {
                available[available_count] = valid[i];
                available_indices[available_count] = i;
                available_count++;
            }
        }

        // No request has arrived yet 
        if (available_count == 0) {
            int next_arrival = valid[0].arrival_time;

            for (int i = 1; i < valid_count; i++) {
                if (valid[i].arrival_time < next_arrival)
                    next_arrival = valid[i].arrival_time;
            }

            current_time = next_arrival;
            continue;
        }

        int local_index;

        Request near = nearest(
            available,
            available_count,
            current_pos,
            &local_index
        );

        int actual_index = available_indices[local_index];

        service_order[serviced] = near;
        waiting_time[serviced] = current_time - near.arrival_time;

        int movement = abs(current_pos - near.sector);

        seek_time += movement;
        current_time += movement;
        current_pos = near.sector;

        // Remove serviced request 
        for (int i = actual_index; i < valid_count - 1; i++) {
            valid[i] = valid[i + 1];
        }

        valid_count--;
        serviced++;
    }

    if (serviced > 0) {
        ave_seek_distance = (float)seek_time / serviced;

        for (int i = 0; i < serviced; i++) {
            total_waiting_time += waiting_time[i];
        }

        ave_waiting_time = (float)total_waiting_time / serviced;
    }

    if (current_time > 0) {
        throughput = (float)serviced / current_time;
    }

    printf("\nRequest Order:\n");

    for (int i = 0; i < serviced; i++) {
        printf("%d -> ", service_order[i].sector);
    }

    printf("END\n");

    printf("\nTotal head movement : %d\n", seek_time);
    printf("Average seek distance : %.2f\n", ave_seek_distance);
    printf("Throughput : %.4f request/time-unit\n", throughput);

    printf("\n");

    for (int i = 0; i < serviced; i++) {
        printf("Waiting time for Process %d (sector %d) : %d\n",
               service_order[i].pid,
               service_order[i].sector,
               waiting_time[i]);
    }

    printf("\nTotal waiting time : %d\n", total_waiting_time);
    printf("Average waiting time : %.2f\n", ave_waiting_time);

    return 0;
}