import time

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

def CRT_RSA(c, d, n, p, q):
    dp = d % (p - 1)
    dq = d % (q - 1)
    q_1 = invert(q, p)
    q_inv = q_1 % p
    m1 = pow(c, dp, p)
    m2 = pow(c, dq, q)
    h = (q_inv * (m1 - m2)) % p
    m = m2 + h * q
    return m

# CRT-RSA
start_time = time.time()
for i in range(10000):
    m = CRT_RSA(1234567866666666666665, 233333333333449, 144188797283, 379721, 379723)  # 使用合适的输入参数进行测试
end_time = time.time()
print("CRT-RSA Time:", end_time - start_time)

# Standard RSA
start_time = time.time()
for i in range(10000):
    a = pow(1234567866666666666665, 233333333333449, 144188797283)  # 使用合适的输入参数进行测试
end_time = time.time()
print("Standard RSA Time:", end_time - start_time)

print("Correctness:", a == m)