# 报文数据类别与Json内容格式

### 枚举类型Connection::DataType

| 名称                | 报文功能说明           | Json内容格式，键列表                         |
| ------------------- | ---------------------- | -------------------------------------------- |
| Undifined           | 无报文，或无效报文     | 无                                           |
| Ping                | 检测连接状态           | 无                                           |
| Pong                | 检测连接状态           | 无                                           |
| Request_Register    |                        |                                              |
| Respond_Register    |                        |                                              |
| Request_Login       |                        |                                              |
| Respond_Login       |                        |                                              |
| Request_ChatMessage | 请求发送一条聊天消息   | sender_uid, receiver_uid, content, send_time |
| Respond_ChatMessage | 聊天消息已经送达服务器 | 不知道怎么写                                 |

