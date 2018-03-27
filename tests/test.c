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
static int test_list();
static int test_string_buffer();
static int test_http();
static int test_ini();


static test_case_t test_cases[] = {

    {"test_xml", test_xml},
    {"test_log", test_log},
    {"test_task", test_task},
    {"test_regex", test_regex},
    {"test_string", test_string},
    {"test_socket", test_socket},
    {"test_service", test_service},
    {"test_list", test_list},
    {"test_string_buffer", test_string_buffer},
    {"test_http", test_http},
    {"test_ini", test_ini},
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
#if 0
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

#endif
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

    str = "  hello\r\n";
    char buffer[64];

    sp_string_trim_left(str, buffer);
    sp_return_val_if_fail(sp_string_equal(buffer, "hello\r\n"), -1);

    sp_string_trim_right(str, buffer);
    sp_return_val_if_fail(sp_string_equal(buffer, "  hello"), -1);

    sp_string_trim(str, buffer);
    sp_return_val_if_fail(sp_string_equal(buffer, "hello"), -1);

    str = "[listen_address]\r\n";
    sp_return_val_if_fail(sp_string_between(str, "[", "]", buffer), -1);

    sp_return_val_if_fail(sp_string_equal(buffer, "listen_address"), -1);

    return 0;
}

static int test_list()
{
    void *list = sp_list_new(NULL);

    int size = sp_list_size(list);
    sp_return_val_if_fail(size == 0, -1);

    sp_list_push_back(list, 1);
    size = sp_list_size(list);
    sp_return_val_if_fail(size == 1, -1);

    sp_list_push_back(list, 2);
    size = sp_list_size(list);
    sp_return_val_if_fail(size == 2, -1);

    int val = sp_list_front(list);
    sp_return_val_if_fail(val == 1, -1);

    val = sp_list_back(list);
    sp_return_val_if_fail(val == 2, -1);

    sp_list_clear(list);
    size = sp_list_size(list);
    sp_return_val_if_fail(size == 0, -1);

    sp_list_free(list);
    return 0;
}

static int test_string_buffer()
{
    void *buffer = sp_string_buffer_new(0);

    const char *str = "hello world";
    sp_string_buffer_append(buffer, str, -1);

    sp_return_val_if_fail(sp_string_buffer_size(buffer) == sp_string_length(str), -1);

    char buf[4096];
    memset(buf, 'A', 4096);
    buf[4095] = 0;

    sp_string_buffer_append(buffer, buf, -1);
    sp_return_val_if_fail(sp_string_buffer_size(buffer) == (4095 + sp_string_length(str)), -1);

    sp_string_buffer_free(buffer);

    return 0;
}

static int test_http()
{
    const char *url = "http://www.baidu.com";

    sp_http_response_t *res = sp_http_get(url, NULL, 1);

    sp_return_val_if_fail(res, -1);
    //printf("headers:%s\n", sp_string_buffer_string(res->raw_headers));
    //printf("body:%s\n", sp_string_buffer_string(res->raw_body));
    //printf("status_code:%d\n", res->status_code);

    sp_return_val_if_fail(res->status_code == 200, -1);

    sp_http_response_free(res);

    url = "http://hlht.zc3u.com/evcs/20160701/query_token";

    sp_json_t *headers = sp_json_object_new();
    sp_json_object_add(headers, "Content-Type", sp_json_string("application/json"));
    sp_json_object_add(headers, "User-Agent", sp_json_string("libcurl client"));

    sp_json_t *payload = sp_json_object_new();

    sp_json_object_add(payload, "LastQueryTime", sp_json_string(""));
    sp_json_object_add(payload, "PageCount", sp_json_int(1));
    sp_json_object_add(payload, "PageNo", sp_json_int(1));

    const char *text = sp_json_text(payload);
    
    res = sp_http_post(url, headers, 1, text, sp_string_length(text));
    sp_return_val_if_fail(res, -1);

    sp_free(text);

    sp_json_free(headers);
    sp_json_free(payload);

    //printf("json: %s\n", sp_string_buffer_string(res->raw_body));
    sp_http_response_free(res);

    //domain is unreachable
    url = "http://www.sylar.com.cn";
    res = sp_http_get(url, NULL, 1);
    sp_return_val_if_fail(!res, -1);

    return 0;
}

static int test_ini()
{
    void *h = sp_ini_parse_file("./test.ini");
    sp_return_val_if_fail(h, -1);

    const char *string = sp_ini_get_string(h, "collect", "address");
    sp_return_val_if_fail(sp_string_equal(string, "0.0.0.0"), -1);

    int port = sp_ini_get_int(h, "collect", "port");
    sp_return_val_if_fail(port == 8080, -1);

    return 0;
}
