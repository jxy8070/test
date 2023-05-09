#include "dantecommonaudinatelistener.h"
#include <QDebug>
#include <QThread>

static conmon_client_response_fn
    conmon_cb_reg_connect, conmon_cb_reg_monitoring,
    conmon_cb_reg_sub, conmon_cb_reg_sub_all;


QString MAC;
bool running;
static void conmon_cb_network (conmon_client_t * client)
{
    qDebug ("Addresses changed");
    qDebug ("Networks:");
    conmon_client_get_networks (client);
}
static void conmon_cb_dante_device_name (conmon_client_t * client)
{
    qDebug("Dante device name changed to '%s'"
        , conmon_client_get_dante_device_name (client)
    );
}
static void conmon_cb_dns_domain_name (conmon_client_t * client)
{
    qDebug("DNS domain name changed to '%s'"
        , conmon_client_get_dns_domain_name (client)
    );
}
static char *conmon_example_instance_id_to_string(const conmon_instance_id_t * id, char * buf, size_t len)
{
    if (id)
    {
        _snprintf(buf, len,
            "%02x%02x%02x%02x%02x%02x%02x%02x/%04x",
            id->device_id.data[0], id->device_id.data[1],
            id->device_id.data[2], id->device_id.data[3],
            id->device_id.data[4], id->device_id.data[5],
            id->device_id.data[6], id->device_id.data[7],
            id->process_id);
    }
    else
    {
        _snprintf(buf, len, "[null]");
    }
    return buf;
}
static const char *conmon_example_channel_type_to_string(conmon_channel_type_t channel_type)
{
    switch(channel_type)
    {
    case CONMON_CHANNEL_TYPE_CONTROL:    return "control";
    case CONMON_CHANNEL_TYPE_METERING:   return "metering";
    case CONMON_CHANNEL_TYPE_STATUS:     return "status";
    case CONMON_CHANNEL_TYPE_BROADCAST:  return "broadcast";
    case CONMON_CHANNEL_TYPE_LOCAL:      return "local";
    case CONMON_CHANNEL_TYPE_SERIAL:     return "serial";
    case CONMON_CHANNEL_TYPE_KEEPALIVE:  return "keepalive";
    case CONMON_CHANNEL_TYPE_VENDOR_BROADCAST: return "vendor_broadcast";
    case CONMON_CHANNEL_TYPE_MONITORING: return "monitoring";
    default:                             return "unknown";
    }
}

static void conmon_cb_monitoring(conmon_client_t * client,
                                 conmon_channel_type_t channel_type,
                                 conmon_channel_direction_t channel_direction,
                                 const conmon_message_head_t * head,
                                 const conmon_message_body_t * body)
{
    aud_bool_t want_to_print = AUD_FALSE;
    //conmon_info_t * info = conmon_client_context (client);
    conmon_instance_id_t id;
    char id_buf[64];
//    qDebug("conmon_cb_monitoring");
    //const conmon_audinate_message_head_t * aud_msg = (void *) body;
    uint16_t aud_type = conmon_audinate_message_get_type(body);

    conmon_info_t * info = (conmon_info_t *)conmon_client_context(client);

    conmon_message_head_get_instance_id(head, &id);

    if (info->n_filters)
    {
        unsigned i;
        for (i = 0; i < info->n_filters; i++)
        {
            if (aud_type == info->filter[i].mtype)
            {
                if (info->filter_mode == MESSAGE_FILTER_MODE_PASS)
                    goto l__pass_filter;
                else
                    return;
            }
        }
        if (info->filter_mode == MESSAGE_FILTER_MODE_PASS)
            return;
    }
    l__pass_filter:

    if (info->all)
    {
        want_to_print = AUD_TRUE;
    }
    else if (!info->quiet)
    {
        want_to_print = AUD_TRUE;
    }
    else
    {
        // in 'quiet' mode we only print things we know we have specifically requested
        const char * name = conmon_client_device_name_for_instance_id(client, &id);
        if (name)
        {
            if (info->n_targets)
            {
                unsigned int i;
                for (i = 0; i < info->n_targets; i++)
                {
                    if (!_stricmp(name, info->targets[i].name))
                    {
                        want_to_print = AUD_TRUE;
                        break;
                    }
                }
            }
            else if (!_stricmp(name, conmon_client_get_dante_device_name(info->client)))
            {
                want_to_print = AUD_TRUE;
            }
        }
    }
    if (!want_to_print)
    {
        return;
    }

    {
//        const char * device_name = conmon_client_device_name_for_instance_id(client, &id);
//        uint16_t aud_version = conmon_audinate_message_get_version(body);
        running = AUD_FALSE;
//        qDebug(
//            "Received status message from %s (%s)\n:"
//            "  chan=%s (%s) size=%d aud-version=0x%04x aud-type=0x%04x"
//            , conmon_example_instance_id_to_string(&id, id_buf, sizeof(id_buf))
//            , (device_name ? device_name : "[unknown device]")
//            , conmon_example_channel_type_to_string(channel_type)
//            , (channel_direction == CONMON_CHANNEL_DIRECTION_TX ? "tx" : "rx")
//            , conmon_message_head_get_body_size(head)
//            , (unsigned int) aud_version
//            , (unsigned int) aud_type
//        );
    }
}
static aud_error_t register_for_events (conmon_info_t * cm)
{
    aud_error_t result;
    aud_bool_t has_success = AUD_FALSE;
//    qDebug("register_for_events");
    conmon_client_set_networks_changed_callback (
        cm->client, & conmon_cb_network
    );

    conmon_client_set_dante_device_name_changed_callback (
        cm->client, & conmon_cb_dante_device_name
    );
    conmon_client_set_dns_domain_name_changed_callback (
        cm->client, & conmon_cb_dns_domain_name
    );

    // Register for outgoing (TX) status messages
    result =
        conmon_client_register_monitoring_messages (
            cm->client, & conmon_cb_reg_monitoring, NULL,
            CONMON_CHANNEL_TYPE_STATUS, CONMON_CHANNEL_DIRECTION_TX,
            & conmon_cb_monitoring
        );
    if (result == AUD_SUCCESS)
    {
        has_success = AUD_TRUE;
    }
    else
    {
        aud_errbuf_t ebuf;
        qDebug("failed to register monitoring channel (status): %s (%d)", aud_error_message (result, ebuf), result);
    }

    // Register for outgoing (TX) broadcast messages
    result =
        conmon_client_register_monitoring_messages (
            cm->client, & conmon_cb_reg_monitoring, NULL,
            CONMON_CHANNEL_TYPE_BROADCAST, CONMON_CHANNEL_DIRECTION_TX,
            & conmon_cb_monitoring
        );
    if (result == AUD_SUCCESS)
    {
        has_success = AUD_TRUE;
    }
    else
    {
//        aud_errbuf_t ebuf;
//        qDebug("failed to register monitoring channel (broadcast): %s (%d)", aud_error_message (result, ebuf), result);
    }

    // Register for "outgoing" (TX) local messages
    result =
        conmon_client_register_monitoring_messages (
            cm->client, & conmon_cb_reg_monitoring, NULL,
            CONMON_CHANNEL_TYPE_LOCAL, CONMON_CHANNEL_DIRECTION_TX,
            & conmon_cb_monitoring
        );
    if (result == AUD_SUCCESS)
    {
        has_success = AUD_TRUE;
    }
    else
    {
        aud_errbuf_t ebuf;
        qDebug("failed to register monitoring channel (local): %s (%d)", aud_error_message (result, ebuf), result);
    }

    // register for incoming (RX) status messages
    result =
        conmon_client_register_monitoring_messages (
            cm->client, & conmon_cb_reg_monitoring, NULL,
            CONMON_CHANNEL_TYPE_STATUS, CONMON_CHANNEL_DIRECTION_RX,
            & conmon_cb_monitoring
        );
    if (result == AUD_SUCCESS)
    {
        has_success = AUD_TRUE;
    }
    else
    {
        aud_errbuf_t ebuf;
        qDebug("failed to register external monitoring channel (status): %s (%d)", aud_error_message (result, ebuf), result);
    }

    // register for incoming (RX) broadcast messages
    result =
        conmon_client_register_monitoring_messages (
            cm->client, & conmon_cb_reg_monitoring, NULL,
            CONMON_CHANNEL_TYPE_BROADCAST, CONMON_CHANNEL_DIRECTION_RX,
            & conmon_cb_monitoring
        );
    if (result == AUD_SUCCESS)
    {
        has_success = AUD_TRUE;
    }
    else
    {
        aud_errbuf_t ebuf;
        qDebug("failed to register external monitoring channel (rx broadcast): %s (%d)", aud_error_message (result, ebuf), result);
    }

    if (has_success)
    {
        return AUD_SUCCESS;
    }
    else
    {
        return result;
    }
}
static aud_error_t subscribe_to_remote_clients (conmon_info_t * cm)
{
    aud_error_t result;
    unsigned int i;
//qDebug("subscribe_to_remote_clients");
    if (cm->all)
    {
        result =
            conmon_client_subscribe_global (
                cm->client,
                conmon_cb_reg_sub_all,
                & (cm->all_req_id),
                CONMON_CHANNEL_TYPE_STATUS
            );
        if (result != AUD_SUCCESS)
        {
            return result;
        }
    }

    for (i = 0; i < cm->n_targets; i++)
    {
        if (cm->targets[i].name == NULL)
        {
            continue;
        }
        // don't try and subscribe to ourself
        if (!strcmp(conmon_client_get_dante_device_name(cm->client), cm->targets[i].name))
        {
            continue;
        }
        result =
            conmon_client_subscribe (
                cm->client,
                conmon_cb_reg_sub,
                & (cm->targets [i].req_id),
                CONMON_CHANNEL_TYPE_STATUS,
                cm->targets [i].name
            );
        if (result != AUD_SUCCESS)
        {
            return result;
        }
    }

    return AUD_SUCCESS;
        // XXX: should check eventually
}

static cm_target_t *target_for_sub (conmon_info_t * cm, const conmon_client_subscription_t * sub)
{
    unsigned int i;
    const unsigned int n = cm->n_targets;
//qDebug("target_for_sub");
    for (i = 0; i < n; i++)
    {
        cm_target_t * target = cm->targets + i;
        if (target->name && (target->sub == sub))
        {
            return target;
        }
    }

    return NULL;
}
static void conmon_cb_cm_sub_changed (conmon_client_t * client, unsigned int num_changes, const conmon_client_subscription_t * const * changes)
{
    unsigned int i;
    conmon_info_t * cm = (conmon_info_t * )conmon_client_context (client);
//qDebug("conmon_cb_cm_sub_changed");
    for (i = 0; i < num_changes; i++)
    {
        const conmon_client_subscription_t * sub = changes [i];

        cm_target_t * target = target_for_sub (cm, sub);
        if (target)
        {
            conmon_rxstatus_t rxstatus = conmon_client_subscription_get_rxstatus (sub);

            if (target->old_status == rxstatus)
            {
                continue;
            }
            target->old_status = rxstatus;

            switch (rxstatus)
            {
            case CONMON_RXSTATUS_UNICAST:
            case CONMON_RXSTATUS_MULTICAST:
                if (!target->found)
                {
                    target->found = YES;
                    target->conmon_id = conmon_client_subscription_get_instance_id (sub);
                    conmon_example_instance_id_to_string(target->conmon_id, target->id_buf, sizeof(target->id_buf));
                    MAC = QString(target->id_buf).left(16).toUpper();
                    if (MAC.right(4).operator ==("0000"))
                    {
                        MAC.remove("0000");
                    }
                    else if (MAC.contains("FFFE"))
                    {
                        MAC.remove("FFFE");
                    }
                    running = AUD_FALSE;
//                    qDebug("Subscription to '%s' active, id=%s", target->name, target->id_buf);
//                    qDebug()<<mac_temp<<MAC;
                }
                break;

            case CONMON_RXSTATUS_UNRESOLVED:
                target->found = NO;
//                qDebug("Subscription to '%s' is now UNRESOLVED", target->name);
                break;

            // transient states, don't print anything
            case CONMON_RXSTATUS_PREPARING:
            case CONMON_RXSTATUS_RESOLVED:
                target->found = NO;
                target->id_buf [0] = 0;
                target->conmon_id = NULL;
                qDebug("Subscription to '%s' has entered transient state",
                    target->name
                );
                break;

            default:
                target->found = NO;
                target->id_buf [0] = 0;
                target->conmon_id = NULL;
                printf ("Subscription to '%s' has entered error state",
                    target->name
                );
            }
        }
        // else we ignore because we're not interested in this subscription
    }
}
static void conmon_cb_reg_connect(conmon_client_t * client, conmon_client_request_id_t request_id, aud_error_t result)
{
    conmon_info_t * info = (conmon_info_t * )conmon_client_context (client);

    (void) request_id;
//qDebug("conmon_cb_reg_connect");
    if (result == AUD_SUCCESS)
    {
        if (info)
        {
            register_for_events (info);
            subscribe_to_remote_clients (info);
            conmon_client_set_subscriptions_changed_callback(client, conmon_cb_cm_sub_changed);
        }

        conmon_client_get_networks(client);

//        qDebug("Dante device name '%s'"
//        , conmon_client_get_dante_device_name (client));

//        qDebug("DNS domain name '%s'"
//        , conmon_client_get_dns_domain_name (client));
    }
    else
    {
        aud_errbuf_t ebuf;
        qDebug("failed to connect to conmon: %s (%d)", aud_error_message (result, ebuf), result);
//        QThread::sleep(5);
        conmon_client_disconnect(client);
//        conmon_client_connect(info->client, &conmon_cb_reg_connect, NULL);
        running = AUD_FALSE;
    }
}

static void conmon_cb_reg_monitoring(conmon_client_t * client, conmon_client_request_id_t request_id, aud_error_t result)
{
    (void) client;
    (void) request_id;
//qDebug("conmon_cb_reg_monitoring");
    if (result == AUD_SUCCESS)
    {
//        qDebug ("Conmon status registration successful");
    }
    else
    {
        aud_errbuf_t ebuf;
        qDebug("failed to subscribe to conmon status channel: %s (%d)", aud_error_message (result, ebuf), result);
    }
}


static void conmon_cb_reg_sub(conmon_client_t * client, conmon_client_request_id_t request_id, aud_error_t result)
{
    struct conmon_target * target;

    conmon_info_t * info = (conmon_info_t *)conmon_client_context (client);

    aud_errbuf_t ebuf;
//qDebug("conmon_cb_reg_sub");
    if (request_id != CONMON_CLIENT_NULL_REQ_ID)
    {
        const unsigned int n = info->n_targets;
//        const char *p_name;
        unsigned int i;

        // find the remote target we were trying to subscribe
        for (i = 0; i < n; i++)
        {
            if (request_id == info->targets [i].req_id)
            {
                target = info->targets + i;
                goto l__found_target;
            }
        }

        qDebug("invalid subscription request ID: 0x%p", request_id);
        if (result != AUD_SUCCESS)
        {
            qDebug("error %s (%d)", aud_error_message (result, ebuf), result);
        }
    }
    return;

l__found_target:
    if (result == AUD_SUCCESS)
    {
        target->sub = conmon_client_get_subscription(info->client, CONMON_CHANNEL_TYPE_STATUS, target->name);
        if (! target->sub)
        {
            qDebug("Conmon subscription registration to %s - server error", target->name);
        }
    }
    else
    {
        qDebug("failed to subscribe to remote conmon device %s: %s (%d)", target->name, aud_error_message (result, ebuf), result);
    }

    target->req_id = CONMON_CLIENT_NULL_REQ_ID;
}


static void conmon_cb_reg_sub_all(conmon_client_t * client, conmon_client_request_id_t request_id, aud_error_t result)
{
    conmon_info_t * info = (conmon_info *)conmon_client_context (client);

    aud_errbuf_t ebuf;
//qDebug("conmon_cb_reg_sub_all");
//    if (request_id != CONMON_CLIENT_NULL_REQ_ID)
//    {
//        qDebug("conmon_cb_reg_sub_all1");
//        // find the remote target we were trying to subscribe
//        if (request_id == info->all_req_id)
//        {
//            if (result != AUD_SUCCESS)
//            {
//                qDebug("failed to subscribe to all devices: %s (%d)", aud_error_message (result, ebuf), result);
//            }

//            info->all_req_id = CONMON_CLIENT_NULL_REQ_ID;
//            return;
//        }
//    }

    return;
}

aud_error_t DanteCommonAudinateListener::setup_conmon (conmon_info_t * cm)
{
    conmon_client_config_t * config = NULL;
    aud_error_t result;
    aud_errbuf_t ebuf;

    cm->env = NULL;
    cm->client = NULL;
    cm->running = NO;
    cm->result = 0;

    result = aud_env_setup (& cm->env);
    if (result != AUD_SUCCESS)
    {
        qDebug("conmon failed to initialise environment: %s (%d)", aud_error_message (result, ebuf), result);
        return result;
    }

    config = conmon_client_config_new("demo_listener");
    if (!config)
    {
        qDebug("conmon failed to initialise client: NO_MEMORY");
        cleanup();
        return result;
    }

//    qDebug()<<cm->server_port;
//    if (cm->server_port)
//    {
//        conmon_client_config_set_server_port(config, cm->server_port);
//    }

    result = conmon_client_new_config(cm->env, config, &cm->client);
    conmon_client_config_delete(config);
    config = NULL;
    if (result != AUD_SUCCESS)
    {
        qDebug("conmon failed to initialise client: %s (%d)", aud_error_message (result, ebuf), result);
        cleanup();
        return result;
    }

    conmon_client_set_context (cm->client, cm);

    result = conmon_client_connect(cm->client, & conmon_cb_reg_connect, NULL);
    if (result != AUD_SUCCESS)
    {
        qDebug("conmon failed to initiate connection: %s (%d)", aud_error_message (result, ebuf), result);
    }

    return AUD_SUCCESS;
}

DanteCommonAudinateListener::DanteCommonAudinateListener(QObject *parent) :
    QObject(parent)
{
}

bool DanteCommonAudinateListener::setup(const QString &str)
{
    aud_error_t result;
    aud_errbuf_t ebuf;
    int maxCount = 0;

    MAC = QString("0");
    memset(&info, sizeof(conmon_info_t), 0);

    info.n_targets = 1;
    std::string s = str.toStdString();
    info.targets[0].name = (s.c_str());
    info.targets[0].req_id = CONMON_CLIENT_NULL_REQ_ID;
    info.targets[0].found = YES;

    result = setup_conmon (&info);
    if (result != AUD_SUCCESS)
    {
        return 1;
    }

    info.running = AUD_TRUE;
    running = AUD_TRUE;
    while (running)
    {
        int nfds;
        fd_set fdr;
        int count;
        struct timeval timeout;

        aud_socket_t fd = conmon_client_get_socket (info.client);
        FD_ZERO (& fdr);
        FD_SET (fd, & fdr);

        nfds = 1;

        // prepare sockets and timeouts for
        timeout.tv_sec = 1;
        timeout.tv_usec = 0;

        count = select (nfds, & fdr, NULL, NULL, &timeout);
        if (count > 0)
        {
            result = conmon_client_process (info.client);
            if (result != AUD_SUCCESS)
            {
                qDebug("failed processing fd %d: %s (%d)", fd, aud_error_message (result, ebuf), result);
            }
        }
        else if (count == 0)
        {
//            qDebug("unexpected timeout from select");
            break;
        }
        else
        {
//            qDebug("select failed: %s (%d)", strerror (errno), errno);
            break;
        }

        if (maxCount == 20)
        {
            break;
        } else {
            maxCount++;
        }
    }

    cleanup();
    return 1;
}

void DanteCommonAudinateListener::cleanup()
{
    if (info.client)
    {
        conmon_client_delete (info.client);
        info.client = NULL;
    }

    if (info.env)
    {
        aud_env_release (info.env);
        info.env = NULL;
    }
}

QString DanteCommonAudinateListener::getMac()
{
//    QString temp = MAC;
//    qDebug()<<MAC;
//    MAC = "0";
    return MAC;
}
