import random

file1 = open("dict.in", "w")
file2 = open("query.in", "w")

line1 = 80
line2 = 80

s1 = 'qwertyuiopasdfghjklzxcvbnm'
s2 = 'abc'

def ss(t) :
	tmp = ''
	for i in range(t) :
		#s = chr(random.randint(0, 126))
		#while(s == '\n' or s == '\r' or s == ' ') :
		#	s = chr(random.randint(0, 126))
		tmp += random.choice(s1)
	return tmp

s = ''
for i in range(line1) :
	s += ss(random.randint(200, 250)) + '\n';
file1.write(s);

s = ''

for i in range(line2) :
	s += ss(random.randint(200, 250)) + ' ' + str(random.randint(50, 200)) + '\n';
file2.write(s);