# name of your application
APPLICATION = server_mac

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
USEMODULE += od
USEMODULE += gnrc_txtsnd

include $(RIOTBASE)/Makefile.include
