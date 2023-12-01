import time
from sympy import nextprime

def invert(e, phi):
    d = 0
    x1 = 0
    x2 = 1
    y1 = 1
    temp_phi = phi
    while e > 0:
        temp1 = temp_phi // e
        temp2 = temp_phi - temp1 * e
        temp_phi = e
        e = temp2
        x = x2 - temp1 * x1
        y = d - temp1 * y1
        x2 = x1
        x1 = x
        d = y1
        y1 = y
    if temp_phi == 1:
        return d

def prepare_RSA_CRT(d, p, q):
    dp = d % (p - 1)
    dq = d % (q - 1)
    q_1 = invert(q, p)
    q_inv = q_1 % p
    return dp, dq, q_inv

def CRT_RSA(c, dp, dq, q_inv, p, q):
    m1 = pow(c, dp, p)
    m2 = pow(c, dq, q)
    h = (q_inv * (m1 - m2)) % p
    m = m2 + h * q
    return m

# Generate large prime numbers for RSA
p = nextprime(int(1e100))
q = nextprime(p)
phi = (p - 1) * (q - 1)
n = p * q

# Choose e such that it is co-prime with phi(n)
e = 65537
d = invert(e, phi)

# Prepare RSA-CRT
dp, dq, q_inv = prepare_RSA_CRT(d, p, q) 

# Choose a large number for c
c = int(1e120) % n

# CRT-RSA
start_time = time.time()
for i in range(1000):
    m = CRT_RSA(c, dp, dq, q_inv, p, q) 
end_time = time.time()
print("CRT-RSA Result:", m)
print("CRT-RSA Time:", end_time - start_time)

# Standard RSA
start_time = time.time()
for i in range(1000):
    a = pow(c, d, n)
end_time = time.time()
print("Standard RSA Result:", a)
print("Standard RSA Time:", end_time - start_time)

print("Correctness:", a == m)