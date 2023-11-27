import random

def BaRdc(x, mu, m, k):
    q1 = x // (2**(k - 1))
    q2 = q1 * mu
    q3 = q2 // (2**(k + 1))
    r1 = x % (2**(k + 1))
    r2 = (q3 * m) % (2**(k + 1))
    r = r1 - r2
    if r >= m:
        r -= m
    return r

k = 2048
M = random.randint(2**2047, 2**2048)
mu = pow(2, 2*k, M)

a = random.randint(2**2047, 2**2048)
res1 = hex(a % M)
res2 = hex(BaRdc(a, mu, M, k))

print("预期结果:", res1)
print("实际结果:", res2)
print("实际结果的正确性：", (res1 == res2))