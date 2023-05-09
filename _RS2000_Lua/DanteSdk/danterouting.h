#ifndef DANTEROUTING_H
#define DANTEROUTING_HS
#include <QObject>
#include "audinate/dante_api.h"

#define DR_TEST_MAX_INTERFACES 2
#define DR_TEST_MAX_ENCODINGS 10
#define DR_TEST_MAX_TXLABELS 128

#define DR_TEST_PRINT_LEGACY_FORMATS 0

#define DR_TEST_REQUEST_DESCRIPTION_LENGTH 64
#define DR_TEST_MAX_REQUESTS 128

typedef struct dr_test_options
{
    unsigned int num_handles;
    unsigned int request_limit;

    // for name-based connection
    uint16_t local_port;
    const char * device_name;

    // for address-based connection
    unsigned int num_addresses;
    unsigned int addresses[DR_TEST_MAX_INTERFACES];

    // for interface-aware / redundant name-based connection
    unsigned int num_local_interfaces;
    aud_interface_identifier_t local_interfaces[DR_TEST_MAX_INTERFACES];

    aud_bool_t automatic_update_on_state_change;
} dr_test_options_t;

typedef struct dr_test_request
{
    dante_request_id_t id;
    char description[DR_TEST_REQUEST_DESCRIPTION_LENGTH];
} dr_test_request_t;

typedef struct
{
    dr_test_options_t * options;

    dr_devices_t *devices;
    dr_device_t * device;
    uint16_t nintf;
    uint16_t ntx;
    uint16_t nrx;
    uint16_t max_txflow_slots;
    uint16_t max_rxflow_slots;

    dr_txchannel_t ** tx;
    dr_rxchannel_t ** rx;

    uint16_t txlabels_buflen;
    dr_txlabel_t * txlabels_buf;

    dr_test_request_t requests[DR_TEST_MAX_REQUESTS];

    aud_bool_t sockets_changed;
} dr_test_t;

class DanteRouting : public QObject
{
    Q_OBJECT
public:
    explicit DanteRouting(QObject *parent = 0);
    bool setup(const QString &str);
    void process_router(quint16 recvChannel, QString sendDeviceName, quint16 sendChannel);
    void process_add_txMultipleFlow(quint16 num_slots, quint32 *sendChannel);
    void process_delete_txMultipleFlow();
    void cleanup();
    void printfAudinateDevice();

signals:

public slots:

private:
    bool main_loop(dr_test_t *test);
    aud_error_t dr_test_open(dr_test_t * test, const dr_test_options_t *options);
    aud_error_t get_txFlow_Id(dante_id_t *id, uint16_t f);
    void delete_txFlow(dante_id_t id);
private:
    static void printAudInfo(const char *errorInfo, aud_error_t result =AUD_SUCCESS );
    static aud_error_t dr_test_update_sockets(dr_test_t * test, dante_sockets_t * sockets);
    static void dr_test_on_device_state_changed(dr_test_t * test);
    static aud_error_t dr_test_update( dr_test_t * test);
    static aud_error_t dr_test_query_capabilities(dr_test_t * test);
    static void dr_test_on_sockets_changed(const dr_devices_t * devices);
    static void dr_test_on_device_addresses_changed(dr_test_t * test);
    static void dr_test_on_device_changed(dr_device_t * device, dr_device_change_flags_t change_flags);
    static void onProcessResponse(dr_device_t * device, dante_request_id_t request_id, aud_error_t result);
    static void dr_test_request_release(dr_test_request_t * request);
    static dr_test_request_t *dr_test_allocate_request(dr_test_t * test, const char * description);
    static aud_errbuf_t g_test_errbuf;
private:
    aud_env_t *_danteEnv;
    dr_test_options_t options;
    dr_test_t test;
    QMap<QString,QStringList>  _danteRouterTxNameMap;
};

#endif // DANTEROUTING_H
