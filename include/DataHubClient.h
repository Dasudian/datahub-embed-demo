/**
 * @mainpage IoT Datahub Embedded SDK
 *
 * @brief SDK提供了一个简单的基于MQTT协议与服务器交互的方法<br>
 *
 * SDK基于MQTT协议,传输实时的消息到大数点IoT云服务器<br>
 * 你可以收集设备上的数据发送到云上;也可以订阅某个topic,来接收云服务器推送的
 * 消息<br>
 * <br>
 * 如何使用SDK:<br>
 * <b>1</b>: 创建一个客户端实例<br>
 * <b>2</b>: 如果想接收消息,那么就订阅某个topic<br>
 * <b>3</b>: 或者发送消息到服务器<br>
 * <b>4</b>: 退出时,销毁该客户端<br>
 * <br>
 * 功能:<br>
 * <b>1</b>: 当连接丢失时,SDK会尝试自动重连<br>
 * <b>2</b>: SDK为非线程安全, 即不能在多个Task中使用客户端<br>
 */
#ifndef _DATAHUB_CLIENT_H_
#define _DATAHUB_CLIENT_H_

#include "MQTTClient.h"

#define DATAHUB_FALSE 0
#define DATAHUB_TRUE 1

#define DEFAULT_HOST           "try.iotdatahub.net"
#define DEFAULT_PORT            1883
#define DEFAULT_CLEAN_SESSION   DATAHUB_FALSE
#define DEFAULT_CONTEXT         NULL
#define DEFAULT_CALLBACK        NULL
#define DEFAULT_BUF             NULL
#define DEFAULT_BUF_SIZE        0

/** 选项的初始化宏 */
#define DATAHUB_OPTIONS_INITIALIZER {\
    DEFAULT_HOST,\
    DEFAULT_PORT,\
    DEFAULT_CLEAN_SESSION,\
    DEFAULT_CONTEXT,\
    DEFAULT_CALLBACK,\
    DEFAULT_CALLBACK,\
    DEFAULT_BUF,\
    DEFAULT_BUF_SIZE,\
    DEFAULT_BUF,\
    DEFAULT_BUF_SIZE,\
}

/** 消息的初始化宏,默认为"" */
#define DATAHUB_MESSAGE_INITIALIZER {\
    "",\
    1\
}

/**
 * @brief 消息结构体,用于描述一个消息
 *
 */
typedef struct datahub_message {
    /** 指向待发送的消息 */
    void *payload;
    /** 消息的长度 */
    unsigned int payload_len;
} datahub_message;

/**
 * @brief 字符串,不以'\0'结尾
 */
typedef struct lenstring_s {
    /** 字符串的起始位置 */
   char *data;
   /** 字符串的长度 */
   int len;
}lenstring;

/**
 * @brief 收到消息后的回调函数
 *
 * @param context 在选项中设置的context
 * @param topic 消息对应的主题
 * @param msg 描述消息的结构体
 */
typedef void MESSAGE_RECEIVED(void *context, lenstring *topic, datahub_message *msg);

/**
 * @brief 当网络状态发生变化时,通知用户的回调函数
 *
 * @note 该函数只用于通知用户网络状态,请不要在函数中进行耗时操作或者阻塞
 * @param context 在选项中设置的context
 * @param isconnected DATAHUB_TRUE表示已连接, DATAHUB_FALSE表示已断开
 */
typedef void CONNECTION_STATUS_CHANGED(void *context, int isconnected);

/**
 * @brief 选项结构体,用于设置MQTT协议的相关选项
 */
typedef struct datahub_options {

    /**
     * @brief 大数点MQTT服务器域名或地址
     *
     * 默认为 try.iotdatahub.net
     */
    char *host;
    /**
     * @brief 大数点MQTT服务器开放端口号
     *
     * 默认为1883
     */
    int port;

    /**
     * @brief 是否保存客户端的会话信息.
     *
     * cleansession为DATAHUB_FALSE, 当客户端断线或下线后, 保存客户端订阅的
     * topic和发送给客户端的所有消息.<br>
     * cleansession为DATAHUB_TRUE, 当客户端断线或下线后, 不保留客户端订阅的
     * topic和发送给客户端的任何消息.<br>
     */
    int cleansession;

    /**
     * @brief 传递给回调函数message_received和connection_status_changed,
     * 即回调函数的第一个参数context
     *
     * 默认为NULL */
    void *context;
    /**
     * @brief 收到消息后的回调函数
     *
     * 默认为NULL
     */
    MESSAGE_RECEIVED *message_received;
    /**
     * @brief 当网络状态发生变化时, 通知用户的回调函数
     *
     * 默认为NULL
     */
    CONNECTION_STATUS_CHANGED *connection_status_changed;

    /**
     * @brief 发送缓冲区的首地址
     *
     * 必须由用户手动设置, 默认为NULL
     */
    unsigned char *sendbuf;
    /**
     * @brief 发送缓冲区的大小, 不小于128个字节
     *
     * 必须由用户手动设置, 默认为0
     */
    int sendbuf_size;
    /**
     * @brief 接收缓冲区的首地址
     *
     * 必须由用户手动设置, 默认为NULL
     */
    unsigned char *readbuf;
    /**
     * @brief 接收缓冲区的大小, 不小于128个字节
     *
     * 必须由用户手动设置, 默认为0
     */
    int readbuf_size;
} datahub_options;

/**
 * @brief 描述客户端的结构体, 由SDK维护, 用户无需关心
 *
 * 由于SDK非线程安全, 不能在多个Task中使用datahub_client实例, 只能在一个Task中
 * 使用
 */
typedef struct datahub_client {
    char *instance_id;
    char *instance_key;
    char *client_type;  //设备类型，如"sensor", "electricArm"
    char *client_id;
    datahub_options *options;
    Network *network;
    Client c;
} datahub_client;

/**
 * 数据类型
 */
typedef enum datahub_data_type_s {
    /** 数据为JSON格式 */
    JSON = 0,
    /** 数据为文本/字符串 */
    TEXT = 1,
    /** 数据为二进制 */
    BINARY = 2,
    DATA_TYPE_END
}datahub_data_type;

/**
 * @brief 该函数创建一个客户端实例,该实例可用于连接大数点MQTT服务器
 *
 * @param client 如果函数成功调用,则客户端实例可用于发送数据和订阅主题等<br>
 * 注意: 不能为空<br>
 * @param instance_id 用于连接大数点服务器的唯一标识,由大数点提供<br>
 * 注意: 不能为空<br>
 * @param instance_key 用于连接大数点服务器的密码,由大数点提供<br>
 * 注意: 不能为空<br>
 * @param client_type 设备类型. 如传感器"sensor", 充电桩"charging_pile",
 * 车载电池"car_battery"<br>
 * 注意:可以为空<br>
 * @param client_id 设备的id<br>
 * 注意: 不能为空<br>
 * @param network 网络接口,由用户实现网络数据的收发,SDK会调用这些函数<br>
 * 注意：不能为空且传入前需要调用NewNetwork初始化,否则会出现未定义的错误<br>
 * @param options 可选的选项.具体包含的选项可以查看datahub_options结构体<br>
 * 注意:不能为空<br>
 * @return ERROR_NONE 表示成功,其他表示错误.<br>
 * 错误码请查看开发文档<br>
 */
int datahub_create(datahub_client *client, char *instance_id,
        char *instance_key, char *client_type, char *client_id,
        Network *network, datahub_options *options);

/**
 * @brief 同步发送消息
 *
 * @param client 由函数datahub_create成功创建的客户端实例<br>
 * 注意: 不能为空<br>
 * @param topic 消息对应的topic.如果消息发送前另一个客户端订阅该topic,则另一
 * 个客户端就会收到消息.<br>
 * 注意: 不能为空<br>
 * @param message 发送的消息<br>
 * 注意: 不能为空<br>
 * @param data_type 数据类型，只能为JSON或TEXT或BINARY
 * @param qos 消息的服务质量<br>
 * <b>0</b>: 消息可能到达,也可能不到达<br>
 * <b>1</b>: 消息一定会到达,但可能会重复,当然,前提是返回ERROR_NONE<br>
 * <b>2</b>: 消息一定会到达,且只到达一次,当然,前提是返回ERROR_NONE<br>
 * @param timeout 函数阻塞的最大时间,单位为秒<br>
 * 注意：必须 > 0<br>
 * @return ERROR_NONE 表示成功,其他表示错误.<br>
 * 错误码请查看开发文档
 *
 */
int datahub_sendrequest(datahub_client *client, char *topic,
        datahub_message *message, datahub_data_type data_type,
        int qos, int timeout);

/**
 * @brief 同步订阅主题
 *
 * @param client 由函数datahub_create成功创建的客户端实例<br>
 * 注意: 不能为空<br>
 * @param topic 订阅的topic<br>
 * 注意: 不能为空<br>
 * @param qos 订阅消息的服务质量(发送消息的qos和订阅消息的qos共同决定服务器下
 * 发消息的qos)<br>
 * <b>0</b>: 消息可能到达,也可能不到达<br>
 * <b>1</b>: 消息一定会到达,但可能会重复,当然,前提是返回ERROR_NONE<br>
 * <b>2</b>: 消息一定会到达,且只到达一次,当然,前提是返回ERROR_NONE<br>
 * 注意: 有效值为0, 1, 2<br>
 * @param timeout 函数阻塞的最大时间,单位为秒<br>
 * 注意：必须 > 0<br>
 * @return ERROR_NONE 表示成功,其他表示错误.<br>
 * 错误码请查看开发文档
 */
int datahub_subscribe(datahub_client *client, char *topic, int qos,
        int timeout);

/**
 * @brief 同步取消订阅topic
 *
 * @param client 由函数datahub_create成功创建的客户端实例<br>
 * 注意: 不能为空<br>
 * @param topic 取消的topic<br>
 * 注意: 不能为空<br>
 * @param timeout 函数阻塞的最大时间,单位为秒<br>
 * 注意：必须 > 0<br>
 * @return ERROR_NONE 表示成功,其他表示错误.<br>
 * 错误码请查看开发文档API.md<br>
 */
int datahub_unsubscribe(datahub_client *client, char *topic, int timeout);

/**
 * @brief 销毁客户端,销毁后连接不存在,不能收发数据
 *
 * @param client 由函数datahub_create成功创建的客户端实例<br>
 * 注意: 不能为空<br>
 * @return 无
 */
void datahub_destroy(datahub_client *client);

/**
 * @brief 被动接收消息情况下保持和服务器的连接
 *
 * @note 在只订阅主题的情况下, 由于是被动接收服务器发送的消息, 为了维持和服务
 * 器的连接, 需要定时调用datahub_yield; 如果客户端一直调用datahub_sendrequest,
 * 而该函数内嵌了与服务器维持连接的功能, 则不需要调用datahub_yield
 *
 * @param client 由函数datahub_create成功创建的客户端实例<br>
 * 注意: 不能为空<br>
 * @param timeout_ms 函数阻塞的最大时间<br>
 * 注意：单位为毫秒, 必须大于0<br>
 * @return ERROR_NONE 表示操作成功,其他表示错误.<br>
 * 错误码请查看开发文档
 */
int datahub_yield(datahub_client *client, int timeout_ms);

/**
 * @brief 错误码
 */
enum datahub_error_code_s {
    /** 成功 */
    ERROR_NONE = 0,
    /** 某些参数不合法 */
    ERROR_ILLEGAL_PARAMETERS = -1,
    /** 客户端已断开 */
    ERROR_DISCONNECTED = -2,
    /** MQTT服务器不支持当前使用的协议版本号,请联系开发人员 */
    ERROR_UNACCEPT_PROTOCOL_VERSION = -3,
    /** client_id不可用,可能使用了不支持的字符 */
    ERROR_IDENTIFIER_REJECTED = -4,
    /** 服务器不可用 */
    ERROR_SERVER_UNAVAILABLE = -5,
    /** instance_id 或者instance_key不正确,请检查 */
    ERROR_BAD_USERNAME_OR_PASSWD = -6,
    /** 未被授权 */
    ERROR_UNAUTHORIZED = -7,
    /** 验证服务器不可用 */
    ERROR_AUTHORIZED_SERVER_UNAVAILABLE = -8,
    /** 操作失败 */
    ERROR_OPERATION_FAILURE = -9,
    /** 消息过长 */
    ERROR_MESSAGE_TOO_BIG = -10,
    /** 网络不可用 */
    ERROR_NETWORK_UNREACHABLE = -11,
    /** 超时 */
    ERROR_TIMEOUT = -12,
    /** 非法的选项 */
    ERROR_ILLEGAL_OPTION = -200,
    /** 主题过长 */
    ERROR_TOPIC_TOO_LONG = -201,
    /** 不合法的JSON字符串 */
    ERROR_MESSAGE_INVALID_JSON = -202,
    /** 不合法的设备类型字符;设备类型不能包含竖线"|", 也不能以下划线"_"开头，且长度不能超过32个字节 */
    ERROR_INVALID_CLIENT_TYPE = -203
};

#endif
