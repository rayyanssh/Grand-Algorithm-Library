n = int(input())
a = list(map(int, input().split()))
aLength = len(a)

total = sum(a)
prefix = 0
count = 0

for i in range(aLength - 1):
    prefix += a[i]
    if prefix == total - prefix:
        count += 1

print(count)