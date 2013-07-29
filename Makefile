all: ping

ping: ping.c
	gcc -g ping.c -o ping && sudo setcap cap_net_raw+ep ping

traceroute: traceroute.c
	gcc -g traceroute.c -o traceroute && sudo setcap cap_net_raw+ep traceroute

clean:
	rm ping
