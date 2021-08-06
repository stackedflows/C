import datetime

def lpss():

    start = datetime.datetime.now()

    seq = "aaasadsdafasdhjkagsdfjhasjbjflASHBFHJASKBHFKJASBFASSDHJGVAHDGVsakhjfdsakjfuadsfk"
    n = len(seq) - 1
    max = 0

    # odd
    for i in range(1, n):
        low = i - 1
        high = i + 1
        currMax = 1
        while low >= 0 and high <= n and seq[low] == seq[high]:
            low-=1
            high+=1
            currMax = currMax + 2
        if currMax > max:
            max = currMax

    # even
    for i in range(0, n):
        low = i
        high = i + 1
        currMax = 0
        while low >= 0 and high <= n and seq[low] == seq[high]:
            low-=1
            high+=1
            currMax = currMax + 2
        if currMax > max:
            max = currMax

    print(max)
    end = datetime.datetime.now()
    diff = end - start
    print(diff)

lpss()
