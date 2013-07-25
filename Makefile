all: ping

ping: ping.c
	gcc -g ping.c -o ping 

clean:
	rm ping
