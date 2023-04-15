#include <iostream>
#include <libssh/libssh.h>
#include <string>

int main() {
    ssh_session session;
    int rc;

    // 初始化SSH会话
    session = ssh_new();
    if (session == NULL) {
        std::cerr << "Failed to create SSH session" << std::endl;
        return 1;
    }

    // 设置SSH连接选项
    ssh_options_set(session, SSH_OPTIONS_HOST, "下位机IP地址");
    ssh_options_set(session, SSH_OPTIONS_USER, "用户名");

    // 连接到下位机
    rc = ssh_connect(session);
    if (rc != SSH_OK) {
        std::cerr << "Failed to connect to the host" << std::endl;
        ssh_free(session);
        return 1;
    }

    // 认证
    rc = ssh_userauth_password(session, NULL, "密码");
    if (rc != SSH_AUTH_SUCCESS) {
        std::cerr << "Failed to authenticate" << std::endl;
        ssh_disconnect(session);
        ssh_free(session);
        return 1;
    }

    // 执行Docker命令来重启指定的容器
    ssh_channel channel;
    channel = ssh_channel_new(session);
    if (channel == NULL) {
        std::cerr << "Failed to create SSH channel" << std::endl;
        ssh_disconnect(session);
        ssh_free(session);
        return 1;
    }

    rc = ssh_channel_open_session(channel);
    if (rc != SSH_OK) {
        std::cerr << "Failed to open SSH channel" << std::endl;
        ssh_channel_free(channel);
        ssh_disconnect(session);
        ssh_free(session);
        return 1;
    }

    // 重启容器
    std::string command = "docker restart 容器ID或名称";
    rc = ssh_channel_request_exec(channel, command.c_str());
    if (rc != SSH_OK) {
        std::cerr << "Failed to execute command: " << command << std::endl;
        ssh_channel_close(channel);
        ssh_channel_free(channel);
        ssh_disconnect(session);
        ssh_free(session);
        return 1;
    }

    // 关闭连接和会话
    ssh_channel_send_eof(channel);
    ssh_channel_close(channel);
    ssh_channel_free(channel);
    ssh_disconnect(session);
    ssh_free(session);

    return 0;
}
