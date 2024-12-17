
import heapq

def main():
    import sys

    T = int(input())
    for _ in range(T):
        N = int(input())
        jobs = []
        for _ in range(N):
            s_i, t_i = map(int, input().split())
            F_i = s_i + t_i
            jobs.append( (F_i, t_i) )
        
        # Sort jobs based on their latest finish time
        jobs.sort()
        
        heap = []
        total_t = 0
        for F_i, t_i in jobs:
            heapq.heappush(heap, -t_i)  # Using negative to simulate max-heap
            total_t += t_i
            if total_t > F_i:
                removed = -heapq.heappop(heap)
                total_t -= removed
        print(len(heap))

if __name__ == "__main__":
    main()
