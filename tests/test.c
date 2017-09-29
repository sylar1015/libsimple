#include "sp.h"

typedef struct
{
    char *name;
    int (*func)();
} test_case_t;

static int test_xml();
static int test_string();
static int test_socket();
static int test_service();


static test_case_t test_cases[] = {

    {"test_xml", test_xml},
    {"test_string", test_string},
    {"test_socket", test_socket},
    {"test_service", test_service},
};

int main()
{
    int i;
    for (i = 0; i < sizeof(test_cases) / sizeof(test_case_t); i++)
    {
        test_case_t *ts = &test_cases[i];

        if (ts->func() < 0)
        {
            printf("%s failed\n", ts->name);
        }
        else
        {
            printf("%s success\n", ts->name);
        }
    }
    return 0;
}

static int test_xml()
{
#if 0
    void *h = sp_xml_parse_file("./HttpConf.xml");
    sp_return_val_if_fail(h, -1);

    xpath_result_t result;
    if (sp_xml_xpath(h, "//NetworkID", &result) < 0)
        return -1;

    void *node = result.nodes[0];
    printf("%s\n", sp_xml_get_text(node));


    if (sp_xml_xpath(h, "//SniffPort", &result) < 0)
        return -1;

    node = result.nodes[0];
    printf("%s\n", sp_xml_get_text(node));

    sp_xml_free(h);

    h = sp_xml_parse_file("./HttpRedirectConf.xml");

    if (sp_xml_xpath(h, "//RedirectRule", &result) < 0)
        return -1;

    int i;
    for (i = 0; i < result.n_nodes; i++)
    {
        node = result.nodes[i];
        printf("prop: %s\n", sp_xml_get_prop(node, "ID"));

        void *type = sp_xml_get_child(node, "RedirectType");
        void *src = sp_xml_get_child(node, "RedirectUrlSrc");
        void *dst = sp_xml_get_child(node, "RedirectUrlDes");
        printf("text: %s %s %s\n",
            sp_xml_get_text(type),
            sp_xml_get_text(src),
            sp_xml_get_text(dst));
    }

    sp_xml_free(h);
#endif
    return 0;
}

static int test_socket()
{
#if 0
    const char *url = "http://192.168.0.1/index.htm";

    char domain[1024];
    char path[1024];
    struct sockaddr_in peer;

    int ret = sp_socket_parse_url(url, domain, path, &peer);
    sp_return_val_if_fail(ret == 0, -1);

    printf("===> %s %s\n", domain, path);

    url = "http://www.baidu.com";
    ret = sp_socket_parse_url(url, domain, path, &peer);
    sp_return_val_if_fail(ret == 0, -1);

    printf("===> %s %s\n", domain, path);
#endif

    return 0;
}

static void service_cb(int msg_type, void *data, int length, void *param)
{
#if 0
    if (msg_type == SERVICE_EV_INIT)
    {
        printf("%d %d %s init\n", sp_service_type(), sp_service_instance(),
            sp_service_name());
    }
    else
    {
        
    }
#endif
}

static int test_service()
{
#if 0
    sp_service_config_t config;
    config.type = 8000;
    config.instance = 1;
    sp_string_copy(config.name, "service 1");
    config.entry = service_cb;
    void *h1 = sp_service_new(&config);

    config.type = 8001;
    config.instance = 1;
    sp_string_copy(config.name, "service 2");
    config.entry = service_cb;
    void *h2 = sp_service_new(&config);
#endif
    return 0;
}

static int test_string()
{
    const char *str = "Hello LibSimple!";
    const char *sub = "LibSimple";

    int pos = sp_string_find(str, sub);
    sp_return_val_if_fail(pos == sp_string_length("Hello "), -1);

    sp_return_val_if_fail(sp_string_endswith(str, "!"), -1);

    return 0;
}
