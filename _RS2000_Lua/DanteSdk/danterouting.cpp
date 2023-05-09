#include "stable.h"
#include "danterouting.h"
#include <QDebug>
aud_errbuf_t DanteRouting::g_test_errbuf;
////////////////////////////////////////////////////////////////////////////////
/// Callbacks
////////////////////////////////////////////////////////////////////////////////
void DanteRouting::dr_test_request_release(dr_test_request_t * request)
{
    request->id = DANTE_NULL_REQUEST_ID;
    request->description[0] = '\0';
}

dr_test_request_t *DanteRouting::dr_test_allocate_request(dr_test_t * test, const char * description)
{
    unsigned int i;
    for (i = 0; i < DR_TEST_MAX_REQUESTS; i++)
    {
        if (test->requests[i].id == DANTE_NULL_REQUEST_ID)
        {
            aud_strlcpy(test->requests[i].description, description ? description : "", DR_TEST_REQUEST_DESCRIPTION_LENGTH);
            return test->requests + i;
        }
    }
//    qDebug("error allocating request '%s': no more requests", description);
    return NULL;
}

void DanteRouting::onProcessResponse(dr_device_t * device, dante_request_id_t request_id, aud_error_t result)
{
    Q_UNUSED(result)
    unsigned int i;
    dr_test_t * test = (dr_test_t *)  dr_device_get_context(device);

    for (i = 0; i < DR_TEST_MAX_REQUESTS; i++)
    {
        if (test->requests[i].id == request_id)
        {
            dr_test_request_release(test->requests+i);
            return;
        }
    }
//    qDebug("EVENT: completed unknown request %p", request_id);
}


void DanteRouting::dr_test_on_sockets_changed(const dr_devices_t * devices)
{
    dr_test_t * test = (dr_test_t *) dr_devices_get_context(devices);
//    qDebug("EVENT: sockets changed");
    test->sockets_changed = AUD_TRUE;
}

void DanteRouting::dr_test_on_device_addresses_changed(dr_test_t * test)
{
    dante_ipv4_address_t address;
    dr_device_get_address(test->device, &address);
}

void DanteRouting::dr_test_on_device_changed(dr_device_t * device, dr_device_change_flags_t change_flags)
{
    dr_test_t * test = (dr_test_t *) dr_device_get_context(device);
    dr_device_change_index_t i;

//    (void) device;

//    qDebug("EVENT: device changed:");
    for (i = 0; i < DR_DEVICE_CHANGE_INDEX_COUNT; i++)
    {
        if (change_flags & (1 << i))
        {
            /*qDebug(" %s", */dr_device_change_index_to_string(i)/*)*/;
            if (i == DR_DEVICE_CHANGE_INDEX_STATE)
            {
                dr_device_state_t state = dr_device_get_state(device);
                /*qDebug(" (%s)", */dr_device_state_to_string(state)/*)*/;
            }
        }
    }

    if (change_flags & DR_DEVICE_CHANGE_FLAG_STATE)
    {
        dr_test_on_device_state_changed(test);
    }

    if (change_flags & DR_DEVICE_CHANGE_FLAG_ADDRESSES)
    {
        dr_test_on_device_addresses_changed(test);
    }

    /*qDebug("Active Requests: %d/%d", */
    dr_devices_num_requests_pending(test->devices);
    dr_devices_get_request_limit(test->devices);

}


aud_error_t DanteRouting::dr_test_query_capabilities(dr_test_t * test)
{
    aud_error_t result;
    dr_test_request_t * request = dr_test_allocate_request(test, "QueryCapabilities");
    if (!request)
    {
        return AUD_ERR_NOBUFS;
    }

    result = dr_device_query_capabilities(test->device, &onProcessResponse, &request->id);
    if (result != AUD_SUCCESS)
    {
        printAudInfo("Error sending query capabilities: ", result );
        dr_test_request_release(request);
        return result;
    }

    return result;
}

aud_error_t DanteRouting::dr_test_update( dr_test_t * test)
{
    aud_error_t result;
//    dr_device_component_t c;
    for (int c = 0; c < DR_DEVICE_COMPONENT_COUNT; c++)
    {
        dr_test_request_t * request;
        if (!dr_device_is_component_stale(test->device, (dr_device_component_t)c))
        {
            continue;
        }
//        qDebug("Updating stale component %s", dr_device_component_to_string((dr_device_component_t)c));

        request = dr_test_allocate_request(test, NULL);
        if (!request)
        {
            return AUD_ERR_NOBUFS;
        }
        qsnprintf(request->description, DR_TEST_REQUEST_DESCRIPTION_LENGTH, "Update %s", dr_device_component_to_string((dr_device_component_t)c));

        result = dr_device_update_component(test->device, &onProcessResponse, &request->id, (dr_device_component_t)c);
        if (result != AUD_SUCCESS)
        {
//            qDebug("Error sending update %s: %s",
//                dr_device_component_to_string((dr_device_component_t)c), dr_error_message(result, g_test_errbuf));
            dr_test_request_release(request);
            return result;
        }
    }
    return AUD_SUCCESS;
}

void DanteRouting::dr_test_on_device_state_changed(dr_test_t * test)
{
    switch (dr_device_get_state(test->device))
    {
    case DR_DEVICE_STATE_RESOLVED:
        // query capabilities
        if (test->options->automatic_update_on_state_change)
        {
//            qDebug("device resolved, querying capabilities");
            dr_test_query_capabilities(test);
        }
        return;

    case DR_DEVICE_STATE_ACTIVE:
        {
//            qDebug("DR_DEVICE_STATE_ACTIVE");
            // update information
            dr_device_get_txchannels(test->device, &test->ntx, &test->tx);
            dr_device_get_rxchannels(test->device, &test->nrx, &test->rx);
//            qDebug("rx channels %d", test->nrx);


            //dante_ipv4_address_t g_address;
//            memset(&g_address, sizeof(dante_ipv4_address_t), 0);
//            dr_device_get_address(test->device, &g_address);

            // update all components when we enter the active state, unless we have a strange status
            if (test->options->automatic_update_on_state_change)
            {
                dr_device_status_flags_t status_flags;
                aud_error_t result = dr_device_get_status_flags(test->device, &status_flags);
                if (result != AUD_SUCCESS)
                {
                    printAudInfo("Error getting status flags: ", result );
                    return;
                }
                if (status_flags)
                {
                    qDebug("device active, status=0x%08x, not updating", status_flags);
                    return;
                }
//                qDebug("device active, updating");
                dr_test_update(test);
            }
            return;
        }
    case DR_DEVICE_STATE_ERROR:
        {
            aud_error_t error_state_error = dr_device_get_error_state_error(test->device);
            const char * error_state_action = dr_device_get_error_state_action(test->device);
            aud_errbuf_t g_test_errbuf;
            qDebug("device has entered the ERROR state: error=%s, action='%s'",
                dr_error_message(error_state_error, g_test_errbuf), (error_state_action ? error_state_action : ""));
            return;
        }
    default:
        // nothing to do
//        qDebug("device state is now '%s'", dr_device_state_to_string(dr_device_get_state(test->device)));
        return;
    }
}


aud_error_t DanteRouting::dr_test_update_sockets(dr_test_t * test, dante_sockets_t * sockets)
{
    aud_error_t result;
    dante_sockets_clear(sockets);
    result = dr_devices_get_sockets(test->devices, sockets);
    if (result != AUD_SUCCESS)
    {
        printAudInfo("Error getting device sockets: ", result );
        return result;
    }
    test->sockets_changed = AUD_FALSE;

    return AUD_SUCCESS;
}

void DanteRouting::printAudInfo( const char* errorInfo, aud_error_t result)
{
    if(result != AUD_SUCCESS){
        aud_errbuf_t errbuf;
        aud_error_message(result, errbuf);
        qDebug()<<errorInfo << errbuf;
    }
    else qDebug()<<errorInfo;
}


DanteRouting::DanteRouting(QObject *parent) :
    QObject(parent)
{
}

bool DanteRouting::setup(const QString &str)
{
    memset(&options, 0, sizeof(dr_test_options_t));
    memset(&test, 0, sizeof(dr_test_t));

    test.options = &options;
    options.automatic_update_on_state_change = AUD_TRUE;
    std::string s = str.toStdString();
    options.device_name = /*const_cast<char *>*/(s.c_str());
//    options.device_name = str.toStdString().c_str();

    aud_error_t result = aud_env_setup(&_danteEnv);
    if (result != AUD_SUCCESS)
    {
        printAudInfo("Error initialising environment: ", result );
        cleanup();
    }

    // create a devices structure and set its options
    result = dr_devices_new(_danteEnv, &test.devices);
    if (result != AUD_SUCCESS)
    {
        printAudInfo("Error creating device factory: ", result );
        cleanup();
    }

    dr_devices_set_context(test.devices, &test);
    dr_devices_set_sockets_changed_callback(test.devices, dr_test_on_sockets_changed);
    if (options.num_handles)
    {
        result = dr_devices_set_num_handles(test.devices, options.num_handles);
        if (result != AUD_SUCCESS)
        {
            aud_errbuf_t g_test_errbuf;
            qDebug("Error setting num_handles to %d: %s", options.num_handles, dr_error_message(result, g_test_errbuf));
            cleanup();
        }
    }
    if (options.request_limit)
    {
        result = dr_devices_set_request_limit(test.devices, options.request_limit);
        if (result != AUD_SUCCESS)
        {
            aud_errbuf_t g_test_errbuf;
            qDebug("Error setting request_limit to %d: %s", options.request_limit, dr_error_message(result, g_test_errbuf));
            cleanup();
        }
    }

    // open a device connection
    result = dr_test_open(&test, &options);
    if (result != AUD_SUCCESS)
    {
        qDebug("open failed");
        cleanup();
        return result;
    }

    result = main_loop(&test);
    return result;
}

bool DanteRouting::main_loop(dr_test_t *test)
{
    aud_error_t result;
    dante_sockets_t all_sockets;

    test->sockets_changed = AUD_TRUE;
    int select_result;
    while(1)
    {
        dante_sockets_t select_sockets;

        struct timeval timeout;

        // update sockets if needed
        if (test->sockets_changed)
        {
            dr_test_update_sockets(test, &all_sockets);
        }

        // prepare sockets and timeouts for
        timeout.tv_sec = 0;
        timeout.tv_usec = 500000;
        select_sockets = all_sockets;

        select_result = select(select_sockets.n, &select_sockets.read_fds, NULL, NULL, &timeout);

        if (select_result < 0)
        {
            result = aud_error_get_last();
            if (result == AUD_ERR_INTERRUPTED)
            {
                continue;
            }
            printAudInfo("Error select()ing: ", result );
            return result;
        }
        else if (select_result > 0)
        {
            result = dr_devices_process(test->devices, &select_sockets);
            if (result != AUD_SUCCESS)
            {
                return result;
            }
        }
        else if(select_result == 0)
        {
            break;
        }
    }

    if (test->nrx)
    {
        QStringList txNameList;
        for (int i = 0; i < test->nrx; ++i)
        {
            txNameList.append(QString(dr_txchannel_get_canonical_name(test->tx[i])));
        }
        _danteRouterTxNameMap.insert(QString(test->options->device_name), txNameList);
    }

    return AUD_SUCCESS;
}

void DanteRouting::process_router(quint16 recvChannel, QString sendDeviceName, quint16 sendChannel)
{
    aud_error_t result;

    dr_test_request_t * request;

    if (recvChannel < 1 || recvChannel > test.nrx)
    {
        qDebug("Invalid RX channel %u (must be in range 1..%u)", recvChannel, test.nrx);
        return ;
    }

    request = dr_test_allocate_request(&test, "Subscribe");
    if (!request)
    {
        return ;
    }

    if (sendDeviceName != NULL)
    {
        QMap<QString,QStringList>::iterator it = _danteRouterTxNameMap.find(sendDeviceName);
        QString port = it.value().at(sendChannel - 1);

        result = dr_rxchannel_subscribe(test.rx[recvChannel-1], &onProcessResponse, &request->id,
                sendDeviceName.toStdString().c_str(),
                port.toStdString().c_str());

        if (result != AUD_SUCCESS)
        {
            printAudInfo("Error sending subscribe message: ", result );
            dr_test_request_release(request);
            return ;
        }
    }
    else
    {
//        qDebug("ACTION: Unsubscribe ");
//        qDebug()<<dr_rxchannel_get_name(test.rx[recvChannel-1])
//                <<dr_txchannel_get_canonical_name(test.tx[recvChannel-1]);
        result = dr_rxchannel_subscribe(test.rx[recvChannel-1], &onProcessResponse, &request->id, NULL, NULL);
        if (result != AUD_SUCCESS)
        {
            printAudInfo("Error sending unsubscribe message: ", result );
            dr_test_request_release(request);
            return ;
        }
    }
    return ;
}

void DanteRouting::process_add_txMultipleFlow(uint16_t num_slots, quint32 *sendChannel)
{
    dante_name_t name;
    dr_txflow_config_t * config = NULL;
    aud_error_t result;
    dr_test_request_t * request;
    dante_id_t flow_id = 0;
//    uint16_t num_slots = 1;

    name[0] = '\0';

    result = dr_txflow_config_new(test.device, flow_id, num_slots, &config);
    if (result != AUD_SUCCESS)
    {
        qDebug("Error creating txflow config");
        return;
    }

    result = dr_txflow_config_set_name(config, name);
    if (result != AUD_SUCCESS)
    {
        qDebug("Error setting txflow config name");
        dr_txflow_config_discard(config);
        return;
    }

    for (int i = 0; i < num_slots; ++i)
    {
        dr_txchannel_t * tx = dr_device_txchannel_with_id(test.device, (dante_id_t)sendChannel[i]);
        if (tx)
        {
            result = dr_txflow_config_add_channel(config, tx, i);
            if (result != AUD_SUCCESS)
            {
                qDebug("Error setting txflow config slot");
                dr_txflow_config_discard(config);
                return;
            }
        }
    }

    request = dr_test_allocate_request(&test, "AddTxFlow");
    if (!request)
    {
        return;
    }

    result = dr_txflow_config_commit(config, NULL/*dr_test_on_response*/, &request->id);
    if (result != AUD_SUCCESS)
    {
        qDebug("Error sending txflow create request");
        dr_test_request_release(request);
        return;
    }
}

aud_error_t DanteRouting::get_txFlow_Id(dante_id_t *id, uint16_t f)
{
    aud_error_t result;
    dr_txflow_t * flow = NULL;
    char * dest_device;
    char * dest_flow;

    result = dr_device_txflow_at_index(test.device, f, &flow);
    if (result != AUD_SUCCESS)
    {
        qDebug("Error getting flow info");
        return result;
    }

    result = dr_txflow_get_id(flow, id);
    if (result != AUD_SUCCESS)
    {
        qDebug("Error getting flow id");
        dr_txflow_release(&flow);
        return result;
    }

    result = dr_txflow_get_destination(flow, &dest_device, &dest_flow);
    if (result != AUD_SUCCESS)
    {
        qDebug("Error getting flow destination: %s\n",
            dr_error_message(result, g_test_errbuf));
        dr_txflow_release(&flow);
        return result;
    }
    if (dest_device[0] || dest_flow[0])
    {
        dr_txflow_release(&flow);
        return AUD_ERR_INVALIDPARAMETER;
    }

    dr_txflow_release(&flow);

    return AUD_SUCCESS;
}

void DanteRouting::delete_txFlow(dante_id_t id)
{
    dr_txflow_t * flow = NULL;
    aud_error_t result;
    dr_test_request_t * request;

    result = dr_device_txflow_with_id(test.device, id, &flow);
    if (result != AUD_SUCCESS)
    {
        qDebug("Error getting txflow %d: %s\n",
               id, dr_error_message(result, g_test_errbuf));
        return;
    }

    request = dr_test_allocate_request(&test, "DeleteTxFlow");
    if (!request)
    {
        return;
    }

    result = dr_txflow_delete(&flow, NULL/*dr_test_on_response*/, &request->id);
    if (result != AUD_SUCCESS)
    {
        qDebug("Error sending delete txflow request for flow %d : %s\n",
               id, dr_error_message(result, g_test_errbuf));
        dr_txflow_release(&flow);
        dr_test_request_release(request);
        return;
    }
}

void DanteRouting::process_delete_txMultipleFlow()
{
    uint16_t nf, f;
    aud_error_t result;

    nf = dr_device_num_txflows(test.device);
    for (f = 0; f < nf; f++)
    {
        dante_id_t id;
        result = get_txFlow_Id(&id, f);
        if (result != AUD_SUCCESS)
        {
            qDebug() << "Error getting flow id";
            continue;
        }

        delete_txFlow(id);
    }
}

aud_error_t DanteRouting::dr_test_open(dr_test_t * test, const dr_test_options_t *options)
{
    aud_error_t result;
    dr_device_open_t * config = NULL;
    unsigned i;

    if (options->device_name)
    {
        config = dr_device_open_config_new(options->device_name);
        if (! config)
        {
            result = AUD_ERR_NOMEMORY;
            goto l__error;
        }

        if (options->num_local_interfaces)
        {
            qDebug("Opening connection to remote device %s using %d interfaces",
                options->device_name, options->num_local_interfaces);

            //result = dr_device_open_remote_on_interfaces(test->devices,
            //	options->device_name, options->num_local_interface_indexes, options->local_interface_indexes, &test->device);

            for (i = 0; i < options->num_local_interfaces; i++)
            {
                if (options->local_interfaces[i].flags == AUD_INTERFACE_IDENTIFIER_FLAG_NAME)
                {
                    dr_device_open_config_enable_interface_by_name(config, i, options->local_interfaces[i].name);
                }
                else if (options->local_interfaces[i].flags == AUD_INTERFACE_IDENTIFIER_FLAG_INDEX)
                {
                    dr_device_open_config_enable_interface_by_index(config, i, options->local_interfaces[i].index);
                }
            }
        }
        else
        {
//            qDebug("Opening connection to remote device %s", options->device_name);
            //result = dr_device_open_remote(test->devices, options->device_name, &test->device);
        }
        result = dr_device_open_with_config(test->devices, config, &test->device);

        dr_device_open_config_free(config);
        config = NULL;
    }
    else if (options->num_addresses)
    {
        config = dr_device_open_config_new(NULL);
        if (! config)
        {
            result = AUD_ERR_NOMEMORY;
            goto l__error;
        }

        qDebug("Opening connection using %d explicit addresses", options->num_addresses);
        //result = dr_device_open_addresses(test->devices,
        //	options->num_addresses, options->addresses, &test->device);

        for (i = 0; i < options->num_addresses; i++)
        {
            if (options->addresses[i])
            {
                dr_device_open_config_enable_address(config, i, options->addresses[i], 0);
            }
        }
        result = dr_device_open_with_config(test->devices, config, &test->device);

        dr_device_open_config_free(config);
        config = NULL;
    }
    else
    {
        if (options->local_port)
        {
            qDebug("Opening connection to local device on port %d", options->local_port);
            result = dr_device_open_local_on_port(test->devices, options->local_port, &test->device);
        }
        else
        {
            qDebug("Opening connection to local device");
            result = dr_device_open_local(test->devices, &test->device);
        }
    }
    if (result != AUD_SUCCESS)
    {
    l__error:
        printAudInfo("Error creating device: ", result );
        return result;
    }
    dr_device_set_context(test->device, test);
    dr_device_set_changed_callback(test->device, dr_test_on_device_changed);
    // if we are connecting locally we may need to trigger the next state transition,
    // if not this is a no-op

    dr_test_on_device_state_changed(test);

    return AUD_SUCCESS;
}



void DanteRouting::cleanup()
{
    if (test.device)
    {
        dr_device_close(test.device);
    }
    if (test.devices)
    {
        dr_devices_delete(test.devices);
    }
    if (_danteEnv)
    {
        aud_env_release(_danteEnv);
    }
}

void DanteRouting::printfAudinateDevice()
{
    QMap<QString,QStringList>::iterator it; //遍历map
    for ( it = _danteRouterTxNameMap.begin(); it != _danteRouterTxNameMap.end(); ++it )
    {
        qDebug() << it.key() << it.value();
    }
}
