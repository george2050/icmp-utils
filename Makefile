all: ping

ping: ping.c
	gcc -g ping.c -o ping && sudo setcap cap_net_raw+ep ping

clean:
	rm ping
