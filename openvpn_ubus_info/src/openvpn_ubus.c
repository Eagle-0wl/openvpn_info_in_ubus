#include <libubox/blobmsg_json.h>
#include <libubus.h>
#include <unistd.h>
#include <syslog.h>

#include "main.h"
#include "parse_data.h"
#include "linked_list.h"

/**
 * get information about connected clients
 */

static int get_clients(struct ubus_context *ctx, struct ubus_object *obj,
		      struct ubus_request_data *req, const char *method,
		      struct blob_attr *msg);


static struct uloop_timeout event_timer;
static struct ubus_context *ctx;
static void set_event(void);
static void event_handler(struct uloop_timeout *timeout);
static void end_ubus(void);

enum {
	COUNTER_VALUE,
	__COUNTER_MAX
};

static const struct ubus_method openvpn_methods[] = {
    UBUS_METHOD_NOARG("clients", get_clients)
};

static struct ubus_object_type openvpn_object_type =
	UBUS_OBJECT_TYPE(server_config.name, openvpn_methods);

static struct ubus_object openvpn_object = {
	.name = server_config.name,
	.type = &openvpn_object_type,
	.methods = openvpn_methods,
	.n_methods = ARRAY_SIZE(openvpn_methods),
};

//print client info in ubus
static int get_clients(struct ubus_context *ctx, struct ubus_object *obj,
		      struct ubus_request_data *req, const char *method,
		      struct blob_attr *msg)
{
	struct blob_buf b = {};

    int count = 0; //!< variable to tell if connected clients count are more than 0
    
    struct Clients *pfirst = NULL;
    pfirst = clients;
    while(pfirst != NULL) {
            count = 1;
        	blob_buf_init(&b, 0);
            blobmsg_add_string(&b, "Name", pfirst->name);
            blobmsg_add_string(&b, "Address", pfirst->address);
            blobmsg_add_string(&b, "Bytes received", pfirst->bytes_received);
            blobmsg_add_string(&b, "Bytes sent", pfirst->bytes_sent);
            blobmsg_add_string(&b, "Connection date", pfirst->date);
            ubus_send_reply(ctx, req, b.head);
            blob_buf_free(&b);
            pfirst = pfirst->next;
    }

    if (count == 0) {
            blob_buf_init(&b, 0);
            blobmsg_add_string(&b, "info", "There is no clients");
            ubus_send_reply(ctx, req, b.head);
            blob_buf_free(&b);
            return 0;
    }

	return 0;
}

int process_ubus(void)
{
	uloop_init();
    set_event();

	ctx = ubus_connect(NULL);
	if (!ctx) {
            syslog(LOG_ERR, "Failed to connect to ubus");
            return -1;
	}

	ubus_add_uloop(ctx);
	ubus_add_object(ctx, &openvpn_object);
	uloop_run();
    end_ubus();

	return 0;
}
//deletes list and ends ubus
static void end_ubus(void)
{
    delete_list();
	ubus_free(ctx);
	uloop_done();
    exit(1);
}
//checks if socket is still alive and updates ubus info
static void event_handler(struct uloop_timeout *timeout)
{
    if (check_socket_connection_status() != 0)
            end_ubus();       
    gather_status();
    uloop_timeout_set(&event_timer, 400);
}

static void set_event(void)
{
    event_timer.cb = event_handler;
    uloop_timeout_set(&event_timer, 0);
}

