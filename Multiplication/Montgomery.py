import random

def montgomery_reduction(a, b, m, bit_width):
    base = 2 ** bit_width
    k = 0
    num_mod = m
    cnt = 0

    while num_mod != 0:
        cnt += 1
        num_mod >>= 1

    if cnt % bit_width != 0:
        k = cnt // bit_width + 1
    else:
        k = cnt // bit_width

    R = base ** k
    b = b * R % m

    base_del_1 = 2 ** (bit_width - 1)
    invm = base - pow(m % base, base_del_1 - 1, base)
    r = 0

    for i in range(k):
        u = ((r & (base - 1)) + ((a & (base - 1)) * (b & (base - 1)))) * invm & (base - 1)
        r = (r + (a & (base - 1)) * b + u * m) >> bit_width
        a >>= bit_width

    if r >= m:
        r -= m

    return r

# 测试正确性
a = random.randint(2**2047, 2**2048)  # multiplier
b = random.randint(2**2047, 2**2048)
m = 2**2047 + 3
bit_width = 17

# 使用Python内置的乘法和取模运算符进行计算
expected_result = (a * b) % m

# 使用优化后的代码进行计算
actual_result = montgomery_reduction(a, b, m, bit_width)

print("预期结果:", expected_result)
print("实际结果:", actual_result)
print("实际结果的正确性：", expected_result == actual_result)
