import math


print([hex(math.floor(abs(math.sin(i + 1) * 2**32))) for i in range(64)])