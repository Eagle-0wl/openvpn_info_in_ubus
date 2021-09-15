SRC:=$(wildcard *.c)
BIN:=openvpn_ubus_info

.PHONY: all clean

all: $(BIN)

$(BIN): $(SRC)
	$(CC) -o $@ $^ -lblobmsg_json -lubox -lubus

clean:
	rm -f $(BIN)
