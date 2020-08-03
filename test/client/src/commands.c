#include "../head/commands.h"

CMD_NO get_cmd_no(char ** cmd_list, int max_cmd_no, const char * cmd) {
    if (0 == strlen(cmd)) {
        return EMPTY;
    }
    for (int cmd_no = 1; cmd_no < max_cmd_no; ++cmd_no) {
        if (0 == strncmp(cmd_list[cmd_no], cmd, strlen(cmd_list[cmd_no]))) {
            return (CMD_NO) cmd_no;
        }
    }
    return INVALID;
}

int cmd_cd(int sfd, const cmd_t * cmd, char * buf, int buf_size) {
    send(sfd, &cmd, sizeof(cmd_t), 0);
    memset(buf, 0, buf_size);
    recv(sfd, buf, sizeof(buf), 0);
    printf("Path: %s\n", buf);

    return 0;
}

int cmd_pwd(int sfd, const cmd_t * cmd, char * buf, int buf_size) {
    send(sfd, cmd, sizeof(cmd_t), 0);
    send(sfd, "pwd", 3, 0);
    memset(buf, 0, buf_size);
    recv(sfd, buf, buf_size, 0);
    printf("%s\n", buf);

    return 0;
}
