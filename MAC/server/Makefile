# name of your application
APPLICATION = server_udp

# If no BOARD is found in the environment, use this default:
BOARD ?= native

# This has to be the absolute path to the RIOT base directory:
RIOTBASE ?= $(CURDIR)/../../../RIOT

# Comment this out to disable code in RIOT that does safety checking
# which is not needed in a production environment but helps in the
# development process:
CFLAGS += -DDEVELHELP

# Change this to 0 show compiler invocation lines by default:
QUIET ?= 1

# Modules to include:
USEMODULE += shell
USEMODULE += shell_commands
USEMODULE += ps
USEMODULE += xtimer
#To include the default network device(s) on your board
USEMODULE += gnrc_netdev_default
#To auto-initialize these network devices as GNRC network interfaces
USEMODULE += auto_init_gnrc_netif
#You may choose to build either as an IPv6 Node
#USEMODULE += gnrc_ipv6_default
#For ICMPv6 echo request/reply (ping) functionality:
#USEMODULE += gnrc_icmpv6_echo
#For UDP support include
#USEMODULE += gnrc_udp
USEMODULE += od
#USEMODULE += gnrc_pktdump
USEMODULE += gnrc_txtsnd

include $(RIOTBASE)/Makefile.include
