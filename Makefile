all: ping

ping: ping.c icmp-utils.c
	gcc -g $^ -o $@
	sudo setcap cap_net_raw+ep $@

#traceroute: traceroute.c
#	gcc -g traceroute.c -o traceroute && sudo setcap cap_net_raw+ep traceroute

clean:
	rm ping
