#include "sp.h"

typedef struct
{
    char *name;
    int (*func)();
} test_case_t;

static int test_xml();
static int test_log();
static int test_task();
static int test_regex();
static int test_string();
static int test_socket();
static int test_service();


static test_case_t test_cases[] = {

    {"test_xml", test_xml},
    {"test_log", test_log},
    {"test_task", test_task},
    {"test_regex", test_regex},
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

static void task1_cb(int msg, void *data, int length, void *arg)
{
    printf("task_cb1: %d %s %d\n", msg, data, length);
}

static void task2_cb(int msg, void *data, int length, void *arg)
{
    printf("task_cb2: %d %s %d\n", msg, data, length);
}

static int test_task()
{
    void *task1 = sp_task_new("task1", 0x80000000, 1, task1_cb);
    sp_return_val_if_fail(task1, -1);

    void *task2 = sp_task_new("task2", 0x80000001, 1, task2_cb);
    sp_return_val_if_fail(task2, -1);

    sp_task_addr_t task_addr;
    sp_bzero(&task_addr, sizeof(task_addr));
    task_addr.type = 0x80000000;
    task_addr.instance = 1;
    int ret = sp_task_sendto_anywhere(100, "hello", 6, &task_addr);
    sp_return_val_if_fail(ret == 0, -1);

    usleep(1000);

    sp_task_free(task2);
    sp_task_free(task1);

    return 0;
}

static int test_log()
{
    void *h = sp_log_new("./test.conf", "test");
    sp_return_val_if_fail(h, -1);

    sp_log_error(h, "error log should be in log");
    sp_log_info(h, "info log should be in log");
    sp_log_debug(h, "debug log should not be in log");

    sp_free(h);

    return 0;
}

static int test_xml()
{

    void *h = sp_xml_parse_file("./test.xml");
    sp_return_val_if_fail(h, -1);

    sp_xpath_result_t result;
    if (sp_xml_xpath(h, "//user", &result) < 0)
        return -1;

    sp_return_val_if_fail(result.n_nodes == 2, -1);

    if (sp_xml_xpath(h, "//user[@id=\"1\"]", &result) < 0)
        return -1;

    void *node = sp_xml_get_child(result.nodes[0], "name");
    sp_return_val_if_fail(node, -1);

    sp_return_val_if_fail(sp_string_equal("Hello", sp_xml_get_text(node)), -1);
    
    sp_xml_free(h);

    return 0;
}

static int test_regex()
{
    const char *pattern = "age ([0-9]+).*?(\\(\\+86\\)[0-9]+)";
    const char *text = "I am sylar, age 18, my phone number is (+86)1314520.";

    char age[32];
    char phone[32];
    sp_return_val_if_fail(sp_regex(pattern, text, age, phone) == 0, -1);

    sp_return_val_if_fail(sp_string_equal(age, "18"), -1);

    sp_return_val_if_fail(sp_string_equal(phone, "(+86)1314520"), -1);

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
