import random

file1 = open("dict.txt", "w")
file2 = open("query.txt", "w")

line1 = 100
line2 = 100

s1 = 'qwertyuiopasdfghjklzxcvbnm'
s2 = 'abc'

len = 100

def ss(t) :
	tmp = ''
	for i in range(t) :
		tmp += random.choice(s2)
	return tmp

for i in range(line1) :
	file1.write(ss(random.randint(50, 150)) + '\n');

for i in range(line2) :
	file2.write(ss(random.randint(50, 150)) + ' ' + str(random.randint(30, 60)) + '\n');